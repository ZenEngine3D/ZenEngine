#include "ZendoPCH.h"
#include "CtrlProperty.h"
#include "../ZendoApp.h"

WX_PG_IMPLEMENT_VARIANT_DATA(wxVector2s)
WX_PG_IMPLEMENT_VARIANT_DATA(wxVector3s)
WX_PG_IMPLEMENT_VARIANT_DATA(wxVector4s)
WX_PG_IMPLEMENT_VARIANT_DATA(wxVector2f)
WX_PG_IMPLEMENT_VARIANT_DATA(wxVector3f)
WX_PG_IMPLEMENT_VARIANT_DATA(wxVector4f)

namespace BCtrl
{
const wxString wxPG_ATTR_STEP(wxT("Step"));
const wxString wxPG_ATTR_WRAP(wxT("Wrap"));
const wxColour ControlBGColor[]={wxColour(230,240,250,255),wxColour(240,245,240,255)};

//=================================================================================================
//! @brief		Common setup applied to each property control
//! @details	Used to avoid code duplication
//-------------------------------------------------------------------------------------------------
//! @param _Value		- Asset value to edit
//! @return				- Created property control
//=================================================================================================
wxPGProperty* CreateAssetValueControl(const zenAss::zAssetItemRef& _rAsset, zenAss::PropertyValueRef& _Value)
{
	wxPGProperty* pProperty(NULL);
	switch( _Value.GetType() )
	{
	case zenConst::keAssProp_Bool:		pProperty=zenNewDefault wxZenBoolProperty(_rAsset, _Value);		break;
 	case zenConst::keAssProp_Enum:		pProperty=zenNewDefault wxZenEnumProperty(_rAsset, _Value);		break;
 	case zenConst::keAssProp_File:		pProperty=zenNewDefault wxZenFileProperty(_rAsset, _Value);		break;
 	case zenConst::keAssProp_Float:		pProperty=zenNewDefault wxZenFloatProperty(_rAsset, _Value);		break;	
 	case zenConst::keAssProp_Float2:	pProperty=zenNewDefault wxZenFloat2fProperty(_rAsset, _Value);		break;
 	case zenConst::keAssProp_Float3:	pProperty=zenNewDefault wxZenFloat3fProperty(_rAsset, _Value);		break;
 	case zenConst::keAssProp_Float4:	pProperty=zenNewDefault wxZenFloat4fProperty(_rAsset, _Value);		break;
 	case zenConst::keAssProp_Int:		pProperty=zenNewDefault wxZenIntProperty(_rAsset, _Value);			break;
 	case zenConst::keAssProp_Int2:		pProperty=zenNewDefault wxZenInt2fProperty(_rAsset, _Value);		break;
 	case zenConst::keAssProp_Int3:		pProperty=zenNewDefault wxZenInt3fProperty(_rAsset, _Value);		break;
 	case zenConst::keAssProp_Int4:		pProperty=zenNewDefault wxZenInt4fProperty(_rAsset, _Value);		break;
	case zenConst::keAssProp_Asset:		pProperty=zenNewDefault wxZenAssetProperty(_rAsset, _Value);		break;
 	case zenConst::keAssProp_Array:		pProperty=zenNewDefault wxZenArrayProperty(_rAsset, _Value);		break;
 	case zenConst::keAssProp_Struct:	pProperty=zenNewDefault wxZenStructProperty(_rAsset, _Value);		break;
	default:							ZENAssertMsg(0, "Unknown property type, implement it.")				break;
	}	

	if( pProperty )
	{	
		const zenAss::PropertyDefinition& PropertyDef = _Value.GetDefinition();
		pProperty->SetName	( (const char*)PropertyDef.mName.mzName );
		pProperty->SetLabel	( (const char*)PropertyDef.mzDisplayName );
		pProperty->Enable	(  PropertyDef.mbIsEditable );
	}	
	return pProperty;
}

//=================================================================================================
//! @brief		Set property configuration for regular scalar values
//! @details	Used to avoid code duplication
//-------------------------------------------------------------------------------------------------
//! @param _Property	- Parameter control to configure
//! @param _Min			- Minimum valid value
//! @param _Max			- Maximum valid value
//! @param _Inc			- Amount ticker increase value
//=================================================================================================
void ConfigurePropertyScalar(wxPGProperty& _Property, const wxVariant& _Min, const wxVariant& _Max, const wxVariant& _Inc)
{
	_Property.SetEditor		( wxPGEditor_SpinCtrl );	
	_Property.SetAttribute	( wxPG_ATTR_MIN, _Min );  
	_Property.SetAttribute	( wxPG_ATTR_MAX, _Max );  
	_Property.SetAttribute	( wxPG_ATTR_STEP, _Inc );
	_Property.SetAttribute	( wxPG_ATTR_WRAP, false );
}

void PropertyMetaData::SetControlState()
{
	mpOwner->SetBackgroundColour( ControlBGColor[ mrAssetValue.IsDefault() ], 0 );
	mpOwner->SetModifiedStatus	( mpOwner->GetValue() != mOriginalValue );
}

//=================================================================================================
// PROPERTY : BOOL
//=================================================================================================
wxZenBoolProperty::wxZenBoolProperty(const zenAss::zAssetItemRef& _rAsset, zenAss::PropertyValueRef& _rAssetValue)
{
	static zenMem::zAllocatorPool sPoolMetaData("Betl::TypedMetaData Pool", sizeof(TypedMetaData), 100, 100);
	zenAss::PropertyBool::ValueRef	rPropertyVal	= _rAssetValue;
	const zenAss::PropertyBool&		PropertyDef		= rPropertyVal.GetDefinition();	
	const wxString					zFalseTrue[2]	= {wxT("False"), wxT("True")};	
	SetClientData		( zenNew(&sPoolMetaData)TypedMetaData(this, _rAsset, _rAssetValue, rPropertyVal.GetValue()) );
	SetDefaultValue		( wxVariant(PropertyDef.mDefault) );
	SetValue			( rPropertyVal.GetValue() );
	SetHelpString		( wxString::Format("%s\n(Default %s)", (const char*)PropertyDef.mzDescription, zFalseTrue[PropertyDef.mDefault]));
	SetAttribute		( wxPG_BOOL_USE_CHECKBOX, true );
}

wxZenBoolProperty::~wxZenBoolProperty()
{
	zenDel(GetClientData());
}

bool wxZenBoolProperty::TypedMetaData::Save()
{
	ZENAssert( mpOwner != NULL && mrAssetValue.IsValid() );	
	if( mpOwner->GetValue() != mOriginalValue )
	{
		zenAss::PropertyBool::ValueRef rPropertyVal(mrAssetValue);
		rPropertyVal	= mpOwner->GetValue();
		mOriginalValue	= mpOwner->GetValue();
		return true;
	}	
	return false;	
}

//=================================================================================================
// PROPERTY : FLOAT
//=================================================================================================
wxZenFloatProperty::wxZenFloatProperty(const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rAssetValue)
{
	static zenMem::zAllocatorPool sPoolMetaData("Betl::TypedMetaData Pool", sizeof(TypedMetaData), 100, 100);
	ZENAssert( _rAssetValue.IsValid() );
	zenAss::PropertyFloat::ValueRef	rAssetVal	= _rAssetValue;
	const zenAss::PropertyFloat&	AssetProp	= rAssetVal.GetDefinition();	
	SetClientData		( zenNew(&sPoolMetaData)TypedMetaData(this, _rAsset, _rAssetValue, rAssetVal.GetValue()) );
	SetDefaultValue		( wxVariant(AssetProp.mDefault) );
	SetValue			( rAssetVal.GetValue() );
	SetHelpString		( wxString::Format("%s\n(Default %.3f) (Min %.3f) (Max %.3f)", (const char*)AssetProp.mzDescription, AssetProp.mDefault, AssetProp.mValMin, AssetProp.mValMax));
	ConfigurePropertyScalar(*this, AssetProp.mValMin, AssetProp.mValMax, AssetProp.mValInc);
}

wxZenFloatProperty::~wxZenFloatProperty()
{
	zenDel(GetClientData());
}

bool wxZenFloatProperty::TypedMetaData::Save()
{
	ZENAssert( mpOwner != NULL && mrAssetValue.IsValid() );	
	if( mpOwner->GetValue() != mOriginalValue )
	{
		zenAss::PropertyFloat::ValueRef rPropertyVal(mrAssetValue);
		rPropertyVal	= static_cast<float>(mpOwner->GetValue().GetDouble());
		mOriginalValue	= mpOwner->GetValue();
		return true;
	}	
	return false;
}

//=================================================================================================
// PROPERTY : INT
//=================================================================================================
wxZenIntProperty::wxZenIntProperty(const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rAssetValue)
{	
	static zenMem::zAllocatorPool sPoolMetaData("Betl::TypedMetaData Pool", sizeof(TypedMetaData), 100, 100);
	ZENAssert( _rAssetValue.IsValid() );
	zenAss::PropertyInt::ValueRef	rAssetVal	= _rAssetValue;
	const zenAss::PropertyInt&		AssetProp	= rAssetVal.GetDefinition();	
	SetClientData		( zenNew(&sPoolMetaData)TypedMetaData(this, _rAsset, _rAssetValue, rAssetVal.GetValue()) );
	SetDefaultValue		( wxVariant(AssetProp.mDefault) );
	SetValue			( rAssetVal.GetValue() );	
	SetHelpString		( wxString::Format("%s\n(Default %i) (Min %i) (Max %i)", (const char*)AssetProp.mzDescription, AssetProp.mDefault, AssetProp.mValMin, AssetProp.mValMax));
	ConfigurePropertyScalar(*this, AssetProp.mValMin, AssetProp.mValMax, AssetProp.mValInc);
}

wxZenIntProperty::~wxZenIntProperty()
{
	zenDel(GetClientData());
}

bool wxZenIntProperty::TypedMetaData::Save()
{
	ZENAssert( mpOwner != NULL && mrAssetValue.IsValid() );	
	if( mpOwner->GetValue() != mOriginalValue )
	{
		zenAss::PropertyInt::ValueRef rPropertyVal(mrAssetValue);
		rPropertyVal	= static_cast<zI32>(mpOwner->GetValue().GetLong());
		mOriginalValue	= mpOwner->GetValue();
		return true;
	}	
	return false;
}

//=================================================================================================
// PROPERTY : VECTOR
//=================================================================================================
template< class TPropertyClass, class TElementCast, class TWxVector, class TWxProperty >
wxZenVectorProperty<TPropertyClass, TElementCast, TWxVector, TWxProperty>::wxZenVectorProperty(const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rAssetValue, const char* _zTooltip, const char* _zTooltipElement)
{		
	static zenMem::zAllocatorPool sPoolMetaData("Betl::TypedMetaData Pool", sizeof(TypedMetaData), 100, 100);
	ZENAssert( _rAssetValue.IsValid() );
	TPropertyClass::ValueRef	rAssetVal	= _rAssetValue;
	const TPropertyClass&		AssetProp	= rAssetVal.GetDefinition();	

	TWxVector wxValue;
	TWxVector wxDefault;
	for(zUInt idx(0); idx<ZENArrayCount(AssetProp.mDefault.values); ++idx)
	{
		wxValue.values[idx]		= rAssetVal.GetValue().values[idx];
		wxDefault.values[idx]	= AssetProp.mDefault.values[idx];
	}

	//PropertyMetaData* pMetaData	= zenNew(&sPoolMetaData)PropertyMetaData(_rAssetValue, WXVARIANT(wxValue));
	for(zUInt idx(0); idx<ZENArrayCount(rAssetVal.GetValue().values); ++idx)
	{
		const wxString zNames[]={wxT("    X"),wxT("    Y"),wxT("    Z"),wxT("    W")};
		TWxProperty* pProp		= zenNewDefault TWxProperty( wxString::Format("%s", zNames[idx]),wxPG_LABEL);

		pProp->SetClientData	( zenNew(&sPoolMetaData)PropertyMetaData(this, _rAsset, _rAssetValue, rAssetVal.GetValue().values[idx]) );
		pProp->SetDefaultValue	( wxVariant(AssetProp.mDefault.values[idx]) );
		pProp->SetValue			( wxVariant(rAssetVal.GetValue().values[idx]) );
		pProp->SetHelpString	( wxString::Format(_zTooltipElement, (const char*)AssetProp.mzDescription, AssetProp.mDefault.values[idx]));
		ConfigurePropertyScalar(*pProp, AssetProp.mValMin.values[idx], AssetProp.mValMax.values[idx], AssetProp.mValInc.values[idx]);
		AddPrivateChild(pProp);
	}
	SetValue					( WXVARIANT(wxValue) );
	SetDefaultValue				( WXVARIANT(wxDefault) );
	SetClientData				( zenNew(&sPoolMetaData)TypedMetaData(this, _rAsset, _rAssetValue, WXVARIANT(wxValue)) );
	switch( ZENArrayCount(AssetProp.mDefault.values) )
	{
	case 1: SetHelpString( wxString::Format(_zTooltip, (const char*)AssetProp.mzDescription, AssetProp.mDefault.values[0]));	break;
	case 2: SetHelpString( wxString::Format(_zTooltip, (const char*)AssetProp.mzDescription, AssetProp.mDefault.values[0],AssetProp.mDefault.values[1]));	break;
	case 3: SetHelpString( wxString::Format(_zTooltip, (const char*)AssetProp.mzDescription, AssetProp.mDefault.values[0],AssetProp.mDefault.values[1],AssetProp.mDefault.values[2]));	break;
	case 4: SetHelpString( wxString::Format(_zTooltip, (const char*)AssetProp.mzDescription, AssetProp.mDefault.values[0],AssetProp.mDefault.values[1],AssetProp.mDefault.values[2],AssetProp.mDefault.values[3]));	break;
	}	
}

template< class TPropertyClass, class TElementCast, class TWxVector, class TWxProperty >
wxZenVectorProperty<TPropertyClass, TElementCast, TWxVector, TWxProperty>::~wxZenVectorProperty() 
{
	zenDel(GetClientData());
}

template< class TPropertyClass, class TElementCast, class TWxVector, class TWxProperty >
void wxZenVectorProperty<TPropertyClass, TElementCast, TWxVector, TWxProperty>::RefreshChildren()
{
	if ( !GetChildCount() ) return;
	TWxVector vector;
	vector << m_value;
	for( zUInt idx(0); idx<ZENArrayCount(vector.values); ++idx)
		Item(idx)->SetValue( vector.values[idx] );
}

template< class TPropertyClass, class TElementCast, class TWxVector, class TWxProperty >
wxVariant wxZenVectorProperty<TPropertyClass, TElementCast, TWxVector, TWxProperty>::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{
	TWxVector vector;
	vector << thisValue;
	vector.values[childIndex] = (TElementCast)childValue;
	wxVariant newVariant;
	newVariant << vector;
	return newVariant;
}

template< class TPropertyClass, class TElementCast, class TWxVector, class TWxProperty >
bool wxZenVectorProperty<TPropertyClass, TElementCast, TWxVector, TWxProperty>::TypedMetaData::Save()
{
	ZENAssert( mpOwner != NULL && mrAssetValue.IsValid() );	
	if( mpOwner->GetValue() != mOriginalValue )
	{
 		TPropertyClass::ValueRef rPropertyVal(mrAssetValue);
		TWxVector vector;
		vector << mpOwner->GetValue();
		for(zUInt idx(0); idx<ZENArrayCount(vector.values); ++idx )
 			rPropertyVal.GetValue().values[idx]	= vector.values[idx];
 		mOriginalValue	= mpOwner->GetValue();
 		return true;
	}	
	return false;
}

template< class TPropertyClass, class TElementCast, class TWxVector, class TWxProperty >
void wxZenVectorProperty<TPropertyClass, TElementCast, TWxVector, TWxProperty>::TypedMetaData::SetControlState()
{
	PropertyMetaData::SetControlState();
	TPropertyClass::ValueRef rPropertyVal(mrAssetValue);
	for(zUInt idx(0), count(mpOwner->GetChildCount()); idx<count; ++idx )
	{
		wxPGProperty* pChild				= mpOwner->Item(idx);
		PropertyMetaData* pChildMetaData	= static_cast<PropertyMetaData*>( pChild->GetClientData() );				
		pChild->SetBackgroundColour	( ControlBGColor[  pChild->GetValue() == (TElementCast)rPropertyVal.GetDefinition().mDefault.values[idx] ], 0 );
		pChild->SetModifiedStatus	( pChild->GetValue() != pChildMetaData->mOriginalValue );
	}
}

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxZenFloat2fProperty, wxPGProperty, wxVector2f, const wxVector2f&, TextCtrl)
WX_PG_IMPLEMENT_PROPERTY_CLASS(wxZenFloat3fProperty, wxPGProperty, wxVector3f, const wxVector3f&, TextCtrl)
WX_PG_IMPLEMENT_PROPERTY_CLASS(wxZenFloat4fProperty, wxPGProperty, wxVector4f, const wxVector4f&, TextCtrl)
WX_PG_IMPLEMENT_PROPERTY_CLASS(wxZenInt2fProperty, wxPGProperty, wxVector2s, const wxVector2s&, TextCtrl)
WX_PG_IMPLEMENT_PROPERTY_CLASS(wxZenInt3fProperty, wxPGProperty, wxVector3s, const wxVector3s&, TextCtrl)
WX_PG_IMPLEMENT_PROPERTY_CLASS(wxZenInt4fProperty, wxPGProperty, wxVector4s, const wxVector4s&, TextCtrl)

//=================================================================================================
// PROPERTY : ENUM
//=================================================================================================
wxZenEnumProperty::wxZenEnumProperty(const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rAssetValue)
{
	static zenMem::zAllocatorPool sPoolMetaData("Betl::TypedMetaData Pool", sizeof(TypedMetaData), 100, 100);
	ZENAssert( _rAssetValue.IsValid() );
	zenAss::PropertyEnum::ValueRef	rAssetVal	= _rAssetValue;
	const zenAss::PropertyEnum&		AssetProp	= rAssetVal.GetDefinition();	

	wxPGChoices soc;
	for(zUInt enumIdx(0), enumCount(AssetProp.maEnumValues.Count()); enumIdx<enumCount; ++enumIdx)
	{
		soc.Add( (const char*)AssetProp.maEnumValues[enumIdx].mzDescription, AssetProp.maEnumValues[enumIdx].mValue );
	}
	//soc[PropertyDef.muDefault].SetBgCol( wxColour(200,255,200,1) );
	//soc[PropertyDef.muDefault].SetBitmap(wxArtProvider::GetBitmap(wxART_INFORMATION));
	SetClientData		( zenNew(&sPoolMetaData)TypedMetaData(this, _rAsset, _rAssetValue, long(rAssetVal.GetValue())) );
	SetChoices			( soc );	
	SetDefaultValue		( wxVariant(long(AssetProp.mDefault)) );
	SetValue			( long(rAssetVal.GetValue()) );
	SetHelpString		( wxString::Format("%s\n(Default '%s')", (const char*)AssetProp.mzDescription, (const char*)AssetProp.GetEnumEntry(AssetProp.mDefault).mzDescription) );
}

wxZenEnumProperty::~wxZenEnumProperty()
{
	zenDel(GetClientData());
}

bool wxZenEnumProperty::TypedMetaData::Save()
{
	ZENAssert( mpOwner != NULL && mrAssetValue.IsValid() );	
	if( mpOwner->GetValue() != mOriginalValue )
	{
		zenAss::PropertyEnum::ValueRef rPropertyVal(mrAssetValue);
		rPropertyVal	= static_cast<zU32>(mpOwner->GetValue().GetLong());
		mOriginalValue	= mpOwner->GetValue();
		return true;
	}	
	return false;
}

//=================================================================================================
// PROPERTY : ASSET
//=================================================================================================
wxZenAssetProperty::wxZenAssetProperty(const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rAssetValue)
{
	static zenMem::zAllocatorPool sPoolMetaData("Betl::TypedMetaData Pool", sizeof(TypedMetaData), 100, 100);
	ZENAssert( _rAssetValue.IsValid() );
	zenAss::PropertyAsset::ValueRef	rAssetVal	= _rAssetValue;
	const zenAss::PropertyAsset&	AssetProp	= rAssetVal.GetDefinition();
		
	SetClientData( zenNew(&sPoolMetaData)TypedMetaData(this, _rAsset, _rAssetValue) );
	ChangeFlag(wxPG_PROP_READONLY, true);

	wxString zHelp		= wxString::Format("%s\nSupported Asset(s) : ", (const char*)AssetProp.mzDescription);
	
	if( AssetProp.mSupportedType == zFlagAssetType(true) )
	{
		zHelp += "All";
	}
	else
	{
		wxString zSupported	= "";
		for( zUInt idx(0); idx< zenConst::keAssType__Count; ++idx)
		{
			if( AssetProp.mSupportedType.Any( (zenConst::eAssetType)idx) )
			{
				if( !zSupported.IsEmpty() )
					zSupported += ", ";
				zSupported += zenAss::AssetTypeToString( (zenConst::eAssetType)idx);				
			}
		}
		zHelp += zSupported;
	}		

	zHelp += "\n[HOME] Find asset\n[INS]  Assign selected asset\n[DEL]  Remove assigned value";
	SetHelpString( zHelp );
	SetAssetValue(rAssetVal.GetValue());
}

wxZenAssetProperty::~wxZenAssetProperty()
{
	zenDel(GetClientData());
}

void wxZenAssetProperty::SetAssetValue( zenAss::zAssetID _AssetID )
{
	wxString zTextValue(L"None");
	zenAss::zAssetItemRef rAsset	= zenAss::AssetGet(_AssetID);
	TypedMetaData*	pMeta		= static_cast<TypedMetaData*>(GetClientData());
	if( rAsset.IsValid() )
	{
		zTextValue				= L"";
		pMeta->mAssetIDValue	= _AssetID;
		zenAss::zPackageRef rPackage = rAsset.GetPackage();
		if (rPackage.IsValid())
		{
			//zArrayStatic<zString> aGroupName = rPackage.GetGroupAndName();
			//for (zUInt idx(0), count(aGroupName.Count()); idx < count; ++idx)
			//	zTextValue += aGroupName[idx] + ".";
			//aGroupName = rAsset.GetGroupAndName();
			//for (zUInt idx(0), count(aGroupName.Count()); idx < count; ++idx)
			//	zTextValue += aGroupName[idx] + ".";
			zTextValue = L"[";
			zTextValue += rPackage.GetName().c_str();
			zTextValue += L"]";
			zTextValue += rAsset.GetName();
		}
	}
	else
	{
		pMeta->mAssetIDValue = zenAss::zAssetID();
	}

	SetValue(zTextValue);	
}

bool wxZenAssetProperty::OnEvent(wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event)
{
	if (event.GetEventType() == wxEVT_CHAR)
	{
		wxKeyEvent&	keyEvent = *static_cast<wxKeyEvent*>(&event);		
		TypedMetaData* pMeta = static_cast<TypedMetaData*>(GetClientData());
		if( keyEvent.GetKeyCode() == WXK_INSERT )
		{
			zenAss::PropertyAsset::ValueRef	rAssetVal	= pMeta->mrAssetValue;
			const zenAss::PropertyAsset& AssetProp		= rAssetVal.GetDefinition();
			zenAss::zAssetItemRef rAsset					= wxGetApp().mpFrame->GetWndAssetBrowser()->GetFirstSelectedItem( AssetProp.mSupportedType );
			if( rAsset.IsValid() && rAsset != pMeta->mrAsset )
			{
				SetAssetValue(rAsset.GetID());
				pMeta->SetControlState();
				return true;
			}
		}
		else if( keyEvent.GetKeyCode() == WXK_DELETE && pMeta->mAssetIDValue.IsValid() )
		{
			SetAssetValue(zenAss::zAssetID());
			pMeta->SetControlState();
			return true;
		}
		else if( keyEvent.GetKeyCode() == WXK_HOME && pMeta->mAssetIDValue.IsValid() )
		{
			wxGetApp().mpFrame->GetWndAssetBrowser()->SelectItem( zenAss::AssetGet(pMeta->mAssetIDValue) );
		}
	}
	return false;
}

wxZenAssetProperty::TypedMetaData::TypedMetaData( wxZenAssetProperty* _pOwner, const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rAssetValue)
: PropertyMetaData(_pOwner, _rAsset, _rAssetValue, 0)
{
	ZENAssert( mpOwner != NULL && mrAssetValue.IsValid() );
	zenAss::PropertyAsset::ValueRef rValue = _rAssetValue;
	mAssetIDValue			= rValue.GetValue();
	mAssetIDOriginalValue	= mAssetIDValue;
}

void wxZenAssetProperty::TypedMetaData::SetControlState()
{
	ZENAssert( mpOwner != NULL && mrAssetValue.IsValid() );
	mpOwner->SetBackgroundColour( ControlBGColor[ mrAssetValue.IsDefault() ], 0 );
	mpOwner->SetModifiedStatus	( mAssetIDValue != mAssetIDOriginalValue );	
}

bool wxZenAssetProperty::TypedMetaData::Save()
{
 	ZENAssert( mpOwner != NULL && mrAssetValue.IsValid() );	
 	if( mAssetIDValue != mAssetIDOriginalValue )
 	{
 		zenAss::PropertyAsset::ValueRef rPropertyVal(mrAssetValue);
 		rPropertyVal			= mAssetIDValue;
 		mAssetIDOriginalValue	= mAssetIDValue;
 		return true;
 	}	
	return false;
}

void wxZenAssetProperty::TypedMetaData::SetDefaultValue()
{
	zenAss::PropertyAsset::ValueRef	rAssetVal = mrAssetValue;
	const zenAss::PropertyAsset& AssetProp = rAssetVal.GetDefinition();
	static_cast<wxZenAssetProperty*>(mpOwner)->SetAssetValue(AssetProp.mDefault);
}

void wxZenAssetProperty::TypedMetaData::SetOriginalValue()
{
	static_cast<wxZenAssetProperty*>(mpOwner)->SetAssetValue(mAssetIDOriginalValue);
}

//=================================================================================================
// PROPERTY : FILE
//=================================================================================================
wxZenFileProperty::wxZenFileProperty(const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rAssetValue)
{
	static zenMem::zAllocatorPool sPoolMetaData("Betl::TypedMetaData Pool", sizeof(TypedMetaData), 100, 100);
	zenAss::PropertyFile::ValueRef	rAssetVal		= _rAssetValue;
	const zenAss::PropertyFile&		AssetProp		= rAssetVal.GetDefinition();	
	wxFileName						Value			= (/*zenIO::GetRootPackage() + */rAssetVal.GetValue()).c_str();
	wxFileName						ValueDefault	= (/*zenIO::GetRootPackage() +*/ AssetProp.mDefault).c_str();
	SetClientData		( zenNew(&sPoolMetaData)TypedMetaData(this, _rAsset, _rAssetValue, Value.GetFullPath()) );
	SetDefaultValue		( wxVariant(ValueDefault.GetFullPath()) );
	SetValue			( Value.GetFullPath() );
	SetHelpString		( (const char*)AssetProp.mzDescription );
	SetAttribute		( wxPG_FILE_INITIAL_PATH,	Value.GetPath());
	SetAttribute		( wxPG_FILE_WILDCARD,		(const zChar*)AssetProp.mzFileExt);
	SetAttribute		( wxPG_FILE_DIALOG_TITLE,	wxString::Format("Open file for '%s'", (const char*)AssetProp.mzDisplayName));
	SetAttribute		( wxPG_FILE_SHOW_RELATIVE_PATH, zenIO::GetRoot() );
	//SetAttribute		( wxPG_FILE_SHOW_FULL_PATH,	true);
	//! @todo Asset: Add validation for files not in root path
}

wxZenFileProperty::~wxZenFileProperty()
{
	zenDel(GetClientData());
}

bool wxZenFileProperty::TypedMetaData::Save()
{
	ZENAssert( mpOwner != NULL && mrAssetValue.IsValid() );	
	if( mpOwner->GetValue() != mOriginalValue )
	{
		zenAss::PropertyFile::ValueRef rPropertyVal(mrAssetValue);
		rPropertyVal	= (const zChar*)mpOwner->GetValue().GetString();
		mOriginalValue	= mpOwner->GetValue();
		//! @todo Log: Warning for file not found
		return true;
	}	
	return false;	
}

//=================================================================================================
// PROPERTY : ARRAY
//=================================================================================================
wxZenArrayProperty::wxZenArrayProperty(const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rAssetValue)
{
	static zenMem::zAllocatorPool sPoolMetaData("Betl::TypedMetaData Pool", sizeof(TypedMetaData), 100, 100);
	ZENAssert( _rAssetValue.IsValid() );
	zenAss::PropertyArray::ValueRef	rAssetVal			= _rAssetValue;
	const zenAss::PropertyArray&	AssetProp			= rAssetVal.GetDefinition();	
	zArrayDynamic<zenAss::PropertyValueRef>& valueArray	= rAssetVal.GetValue();
	SetClientData	( zenNew(&sPoolMetaData)TypedMetaData(this, _rAsset, _rAssetValue, (long)valueArray.Count()) );
	SetHelpString	( wxString::Format("%s", (const char*)AssetProp.mzDescription) );
	ChangeFlag		( wxPG_PROP_NOEDITOR, true );	
	ChangeFlag		( wxPG_PROP_COLLAPSED, true );		
	for(zUInt idx(0), count(valueArray.Count()); idx<count; ++idx)
	{
		wxPGProperty* pProperty = CreateAssetValueControl(_rAsset, valueArray[idx]);	
		pProperty->SetLabel( wxString::Format("%s[%i]", (const char*)AssetProp.mzDisplayName, idx) );
		AddPrivateChild(pProperty);
	}
}

wxZenArrayProperty::~wxZenArrayProperty()
{
	zenDel(GetClientData());
}

bool wxZenArrayProperty::TypedMetaData::Save()
{
	ZENAssert( mpOwner != NULL && mrAssetValue.IsValid() );	
	zenAss::PropertyArray::ValueRef rPropertyVal(mrAssetValue);
	zUInt uChilCount				= mpOwner->GetChildCount();	
	bool bChanged					= mOriginalValue.GetLong() != uChilCount;	
	mOriginalValue					= (long)uChilCount;
	for(zUInt idx(0); idx<uChilCount; ++idx)
	{
		wxPGProperty* pProperty		= mpOwner->Item(idx);	
		PropertyMetaData* pMetaData	= pProperty ? static_cast<PropertyMetaData*>(pProperty->GetClientData()) : NULL;
		if( pMetaData )
			bChanged				|= pMetaData->Save();
	}	
	return bChanged;
}

void wxZenArrayProperty::TypedMetaData::SetControlState()
{
	PropertyMetaData::SetControlState();
	for(zUInt idx(0), count(mpOwner->GetChildCount()); idx<count; ++idx )
	{
		PropertyMetaData* pMetaData = static_cast<PropertyMetaData*>( mpOwner->Item(idx)->GetClientData() );
		if( pMetaData )
			pMetaData->SetControlState();
	}
}

//=================================================================================================
// PROPERTY : STRUCT
//=================================================================================================
wxZenStructProperty::wxZenStructProperty(const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rAssetValue)
{
	static zenMem::zAllocatorPool sPoolMetaData("Betl::TypedMetaData Pool", sizeof(TypedMetaData), 100, 100);
	ZENAssert( _rAssetValue.IsValid() );
	zenAss::PropertyStruct::ValueRef	rAssetVal			= _rAssetValue;
	const zenAss::PropertyStruct&		AssetProp			= rAssetVal.GetDefinition();	
	zArrayStatic<zenAss::PropertyValueRef>& aStructValue	= rAssetVal.GetValue();
	SetClientData	( zenNew(&sPoolMetaData)TypedMetaData(this, _rAsset, _rAssetValue, 0) );
	SetHelpString	( wxString::Format("%s", (const char*)AssetProp.mzDescription) );
	ChangeFlag		( wxPG_PROP_NOEDITOR, true );
	ChangeFlag		( wxPG_PROP_COLLAPSED, true );		
	for(zUInt idx(0), count(aStructValue.Count()); idx<count; ++idx)
	{
		wxPGProperty* pProperty = CreateAssetValueControl(_rAsset, aStructValue[idx]);
		AddPrivateChild(pProperty);
	}
}

wxZenStructProperty::~wxZenStructProperty()
{
	zenDel(GetClientData());
}

bool wxZenStructProperty::TypedMetaData::Save()
{
	ZENAssert( mpOwner != NULL && mrAssetValue.IsValid() );	
	zUInt uChilCount	= mpOwner->GetChildCount();
	bool bChanged		= false;
	for (zUInt idx(0); idx < uChilCount; ++idx)
	{
		wxPGProperty* pProperty = mpOwner->Item(idx);
		PropertyMetaData* pMetaData = pProperty ? static_cast<PropertyMetaData*>(pProperty->GetClientData()) : NULL;
		if (pMetaData)
			bChanged |= pMetaData->Save();
	}

	return bChanged;
}

void wxZenStructProperty::TypedMetaData::SetControlState()
{
	PropertyMetaData::SetControlState();
	for(zUInt idx(0), count(mpOwner->GetChildCount()); idx<count; ++idx )
	{
		PropertyMetaData* pMetaData = static_cast<PropertyMetaData*>( mpOwner->Item(idx)->GetClientData() );
		if( pMetaData )
			pMetaData->SetControlState();
	}
}

//=================================================================================================
//! @brief		Common setup applied to each property control
//! @details	Used to avoid code duplication
//-------------------------------------------------------------------------------------------------
//! @param _GridControl	- PropertyGrid control to add the new value edition
//! @param _Value		- Asset value to edit
//! @return				- Created property control
//=================================================================================================
wxPGProperty* CreateAssetValueControl(wxPropertyGridInterface& _GridControl, const zenAss::zAssetItemRef& _rAsset, zenAss::PropertyValueRef& _Value)
{	
	static zUInt suLineIndex(0);
	wxPGProperty* pProperty = CreateAssetValueControl(_rAsset, _Value);
	if( pProperty )
		_GridControl.Append( pProperty );

	return pProperty;
}

}
