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
wxPGProperty* CreateAssetValueControl(const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _Value)
{
	wxPGProperty* pProperty(nullptr);
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
	default:							zenAssertMsg(0, "Unknown property type, implement it.")				break;
	}	

	if( pProperty )
	{	
		const zenAss::PropertyDefinition& PropertyDef	= _Value.GetDefinition();
		PropertyMetaData* pMetadata						= static_cast<PropertyMetaData*>(pProperty->GetClientData());
		pProperty->SetName	( (const char*)PropertyDef.mName.mzName );
		pProperty->SetLabel	( (const char*)PropertyDef.mzDisplayName );
		pProperty->Enable	(  PropertyDef.mbIsEditable );
		if(pMetadata)
			pMetadata->UpdateControl();
	}	
	return pProperty;
}

//=================================================================================================
//! @brief		Create new control for an asset property
//! @details	
//-------------------------------------------------------------------------------------------------
//! @param _GridControl	- PropertyGrid control to add the new value edition
//! @param _Value		- Asset value to edit
//! @return				- Created property control
//=================================================================================================
wxPGProperty* CreateAssetValueControl(wxPropertyGridInterface& _GridControl, const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _Value)
{
	static zUInt suLineIndex(0);
	wxPGProperty* pProperty = CreateAssetValueControl(_rAsset, _Value);
	if (pProperty)
		_GridControl.Append(pProperty);

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

PropertyMetaData::PropertyMetaData(wxPGProperty* _pOwner, const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rPropertyValue, const wxVariant& _OriginalValue)
: mpOwner(_pOwner)
, mrAsset(_rAsset)
, mrPropertyValue(_rPropertyValue)
, mOriginalValue(_OriginalValue)
{
	mrAsset.GetSignalUpdatedProperty().Connect(*this, &PropertyMetaData::slotUpdateProperty);
}

void PropertyMetaData::SetDefaultValue()
{
	mpOwner->SetValue(mpOwner->GetDefaultValue());
}

void PropertyMetaData::SetOriginalValue()
{
	mpOwner->SetValue(mOriginalValue);
}

void PropertyMetaData::UpdateControlState()
{
	mpOwner->SetBackgroundColour(ControlBGColor[mrPropertyValue.IsDefault()], 0);
	mpOwner->SetModifiedStatus(mpOwner->GetValue() != mOriginalValue);
}

bool PropertyMetaData::Save()
{
	return true;
}

void PropertyMetaData::slotUpdateProperty(zenAss::PropertyValueRef _rPropertyUpdated)
{
	if( _rPropertyUpdated == mrPropertyValue )
	{
		UpdateControl();
		UpdateControlState();
	}	
}

//=================================================================================================
// PROPERTY : BOOL
//=================================================================================================
wxZenBoolProperty::wxZenBoolProperty(const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rPropertyValue)
{
	static zenMem::zAllocatorPool sPoolMetaData("Betl::TypedMetaData Pool", sizeof(TypedMetaData), 100, 100);
	zenAss::PropertyBool::ValueRef rPropertyVal	= _rPropertyValue;
	const zenAss::PropertyBool&	PropertyDef		= rPropertyVal.GetDefinition();
	SetClientData( zenNew(&sPoolMetaData)TypedMetaData(this, _rAsset, _rPropertyValue, rPropertyVal.GetValue()) );
	SetAttribute(wxPG_BOOL_USE_CHECKBOX, true);
	SetDefaultValue(wxVariant(PropertyDef.mDefault));
}

wxZenBoolProperty::~wxZenBoolProperty()
{
	zenDel(GetClientData());
}

void wxZenBoolProperty::TypedMetaData::UpdateControl()
{
	zenAssert(mpOwner != nullptr && mrPropertyValue.IsValid());
	zenAss::PropertyBool::ValueRef	rPropertyVal = mrPropertyValue;
	const zenAss::PropertyBool&		PropertyDef = rPropertyVal.GetDefinition();
	const wxString					zFalseTrue[2] = { wxT("False"), wxT("True") };	
	mpOwner->SetValue(rPropertyVal.GetValue());
	mpOwner->SetHelpString(wxString::Format("%s\n(Default %s)", (const char*)PropertyDef.mzDescription, zFalseTrue[PropertyDef.mDefault]));
}

bool wxZenBoolProperty::TypedMetaData::Save()
{
	zenAssert( mpOwner != nullptr && mrPropertyValue.IsValid() );
	if( mpOwner->GetValue() != mOriginalValue )
	{
		zenAss::PropertyBool::ValueRef rPropertyVal(mrPropertyValue);
		rPropertyVal	= mpOwner->GetValue();
		mOriginalValue	= mpOwner->GetValue();
		return true;
	}	
	return false;	
}

//=================================================================================================
// PROPERTY : FLOAT
//=================================================================================================
wxZenFloatProperty::wxZenFloatProperty(const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rPropertyValue)
{
	static zenMem::zAllocatorPool sPoolMetaData("Betl::TypedMetaData Pool", sizeof(TypedMetaData), 100, 100);
	zenAssert( _rPropertyValue.IsValid() );
	zenAss::PropertyFloat::ValueRef	rPropertyVal	= _rPropertyValue;
	const zenAss::PropertyFloat&	PropertyDef	= rPropertyVal.GetDefinition();
	SetClientData		( zenNew(&sPoolMetaData)TypedMetaData(this, _rAsset, _rPropertyValue, rPropertyVal.GetValue()) );
	SetDefaultValue		( wxVariant(PropertyDef.mDefault) );	
	ConfigurePropertyScalar(*this, PropertyDef.mValMin, PropertyDef.mValMax, PropertyDef.mValInc);
}

wxZenFloatProperty::~wxZenFloatProperty()
{
	zenDel(GetClientData());
}

void wxZenFloatProperty::TypedMetaData::UpdateControl()
{
	zenAss::PropertyFloat::ValueRef	rPropertyVal = mrPropertyValue;
	const zenAss::PropertyFloat&	PropertyDef = rPropertyVal.GetDefinition();
	mpOwner->SetValue(rPropertyVal.GetValue());
	mpOwner->SetHelpString(wxString::Format("%s\n(Default %.3f) (Min %.3f) (Max %.3f)", (const char*)PropertyDef.mzDescription, PropertyDef.mDefault, PropertyDef.mValMin, PropertyDef.mValMax));
}

bool wxZenFloatProperty::TypedMetaData::Save()
{
	zenAssert( mpOwner != nullptr && mrPropertyValue.IsValid() );	
	if( mpOwner->GetValue() != mOriginalValue )
	{
		zenAss::PropertyFloat::ValueRef rPropertyVal(mrPropertyValue);
		rPropertyVal	= static_cast<float>(mpOwner->GetValue().GetDouble());
		mOriginalValue	= mpOwner->GetValue();
		return true;
	}	
	return false;
}

//=================================================================================================
// PROPERTY : INT
//=================================================================================================
wxZenIntProperty::wxZenIntProperty(const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rPropertyValue)
{	
	static zenMem::zAllocatorPool sPoolMetaData("Betl::TypedMetaData Pool", sizeof(TypedMetaData), 100, 100);
	zenAssert( _rPropertyValue.IsValid() );
	zenAss::PropertyInt::ValueRef	rPropertyVal	= _rPropertyValue;
	const zenAss::PropertyInt&		PropertyDef	= rPropertyVal.GetDefinition();	
	SetClientData			(zenNew(&sPoolMetaData)TypedMetaData(this, _rAsset, _rPropertyValue, rPropertyVal.GetValue()));
	SetDefaultValue			(wxVariant(PropertyDef.mDefault));
	ConfigurePropertyScalar	(*this, PropertyDef.mValMin, PropertyDef.mValMax, PropertyDef.mValInc);
}

wxZenIntProperty::~wxZenIntProperty()
{
	zenDel(GetClientData());
}

void wxZenIntProperty::TypedMetaData::UpdateControl()
{
	zenAss::PropertyInt::ValueRef	rPropertyVal = mrPropertyValue;
	const zenAss::PropertyInt&		PropertyDef = rPropertyVal.GetDefinition();	
	mpOwner->SetValue(rPropertyVal.GetValue());
	mpOwner->SetHelpString(wxString::Format("%s\n(Default %i) (Min %i) (Max %i)", (const char*)PropertyDef.mzDescription, PropertyDef.mDefault, PropertyDef.mValMin, PropertyDef.mValMax));
}

bool wxZenIntProperty::TypedMetaData::Save()
{
	zenAssert( mpOwner != nullptr && mrPropertyValue.IsValid() );	
	if( mpOwner->GetValue() != mOriginalValue )
	{
		zenAss::PropertyInt::ValueRef rPropertyVal(mrPropertyValue);
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
wxZenVectorProperty<TPropertyClass, TElementCast, TWxVector, TWxProperty>::wxZenVectorProperty(const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rPropertyValue, const char* _zTooltip, const char* _zTooltipElement)
{		
	static zenMem::zAllocatorPool sPoolMetaData("Betl::TypedMetaData Pool", sizeof(TypedMetaData), 100, 100);
	zenAssert( _rPropertyValue.IsValid() );
	TPropertyClass::ValueRef	rPropertyVal	= _rPropertyValue;
	const TPropertyClass&		PropertyDef	= rPropertyVal.GetDefinition();	

	TWxVector wxValue;
	TWxVector wxDefault;
	for(zUInt idx(0); idx<ZENArrayCount(PropertyDef.mDefault.values); ++idx)
	{
		wxValue.values[idx]		= rPropertyVal.GetValue().values[idx];
		wxDefault.values[idx]	= PropertyDef.mDefault.values[idx];
	}

	for(zUInt idx(0); idx<ZENArrayCount(rPropertyVal.GetValue().values); ++idx)
	{
		const wxString zNames[]={wxT("    X"),wxT("    Y"),wxT("    Z"),wxT("    W")};
		TWxProperty* pProp		= zenNewDefault TWxProperty( wxString::Format("%s", zNames[idx]),wxPG_LABEL);

		pProp->SetClientData	( zenNew(&sPoolMetaData)PropertyMetaData(this, _rAsset, _rPropertyValue, rPropertyVal.GetValue().values[idx]) );
		pProp->SetDefaultValue	( wxVariant(PropertyDef.mDefault.values[idx]) );
		pProp->SetHelpString	( wxString::Format(_zTooltipElement, (const char*)PropertyDef.mzDescription, PropertyDef.mDefault.values[idx]));
		ConfigurePropertyScalar(*pProp, PropertyDef.mValMin.values[idx], PropertyDef.mValMax.values[idx], PropertyDef.mValInc.values[idx]);
		AddPrivateChild(pProp);
	}
	
	SetDefaultValue				( WXVARIANT(wxDefault) );
	SetClientData				( zenNew(&sPoolMetaData)TypedMetaData(this, _rAsset, _rPropertyValue, WXVARIANT(wxValue)) );
	switch( ZENArrayCount(PropertyDef.mDefault.values) )
	{
	case 1: SetHelpString( wxString::Format(_zTooltip, (const char*)PropertyDef.mzDescription, PropertyDef.mDefault.values[0]));	break;
	case 2: SetHelpString( wxString::Format(_zTooltip, (const char*)PropertyDef.mzDescription, PropertyDef.mDefault.values[0],PropertyDef.mDefault.values[1]));	break;
	case 3: SetHelpString( wxString::Format(_zTooltip, (const char*)PropertyDef.mzDescription, PropertyDef.mDefault.values[0],PropertyDef.mDefault.values[1],PropertyDef.mDefault.values[2]));	break;
	case 4: SetHelpString( wxString::Format(_zTooltip, (const char*)PropertyDef.mzDescription, PropertyDef.mDefault.values[0],PropertyDef.mDefault.values[1],PropertyDef.mDefault.values[2],PropertyDef.mDefault.values[3]));	break;
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
void wxZenVectorProperty<TPropertyClass, TElementCast, TWxVector, TWxProperty>::TypedMetaData::UpdateControl()
{
	TPropertyClass::ValueRef rPropertyVal = mrPropertyValue;
	const TPropertyClass& PropertyDef = rPropertyVal.GetDefinition();

	// Vector value
	TWxVector wxValue;	
	for (zUInt idx(0); idx < ZENArrayCount(PropertyDef.mDefault.values); ++idx)
		wxValue.values[idx] = rPropertyVal.GetValue().values[idx];	
	mpOwner->SetValue( WXVARIANT(wxValue) );

	// Child controls values
	for (zUInt idx(0); idx < mpOwner->GetChildCount(); ++idx)
	{
		TWxProperty* pProp = static_cast<TWxProperty*>(mpOwner->Item(idx));
		pProp->SetValue			( wxVariant(rPropertyVal.GetValue().values[idx]) );
	}
}

template< class TPropertyClass, class TElementCast, class TWxVector, class TWxProperty >
bool wxZenVectorProperty<TPropertyClass, TElementCast, TWxVector, TWxProperty>::TypedMetaData::Save()
{
	zenAssert( mpOwner != nullptr && mrPropertyValue.IsValid() );	
	if( mpOwner->GetValue() != mOriginalValue )
	{
 		TPropertyClass::ValueRef rPropertyVal(mrPropertyValue);
		TWxVector vector;
		vector << mpOwner->GetValue();
		TPropertyClass::ValueStorage vValue;
		for(zUInt idx(0); idx<ZENArrayCount(vector.values); ++idx )
			vValue = vector.values[idx];

 		rPropertyVal	= vValue;
 		mOriginalValue	= mpOwner->GetValue();
 		return true;
	}	
	return false;
}

template< class TPropertyClass, class TElementCast, class TWxVector, class TWxProperty >
void wxZenVectorProperty<TPropertyClass, TElementCast, TWxVector, TWxProperty>::TypedMetaData::UpdateControlState()
{
	PropertyMetaData::UpdateControlState();
	TPropertyClass::ValueRef rPropertyVal(mrPropertyValue);
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
wxZenEnumProperty::wxZenEnumProperty(const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rPropertyValue)
{
	static zenMem::zAllocatorPool sPoolMetaData("Betl::TypedMetaData Pool", sizeof(TypedMetaData), 100, 100);
	zenAssert( _rPropertyValue.IsValid() );
	zenAss::PropertyEnum::ValueRef	rPropertyVal	= _rPropertyValue;
	const zenAss::PropertyEnum&		PropertyDef	= rPropertyVal.GetDefinition();	

	wxPGChoices soc;
	for(zUInt enumIdx(0), enumCount(PropertyDef.maEnumValues.Count()); enumIdx<enumCount; ++enumIdx)
	{
		soc.Add( (const char*)PropertyDef.maEnumValues[enumIdx].mzDescription, PropertyDef.maEnumValues[enumIdx].mValue );
	}

	SetClientData		( zenNew(&sPoolMetaData)TypedMetaData(this, _rAsset, _rPropertyValue, long(rPropertyVal.GetValue())) );
	SetChoices			( soc );	
	SetDefaultValue		( wxVariant(long(PropertyDef.mDefault)) );	
}

wxZenEnumProperty::~wxZenEnumProperty()
{
	zenDel(GetClientData());
}

void wxZenEnumProperty::TypedMetaData::UpdateControl()
{
	zenAss::PropertyEnum::ValueRef	rPropertyVal = mrPropertyValue;
	const zenAss::PropertyEnum&		PropertyDef = rPropertyVal.GetDefinition();
	mpOwner->SetValue(long(rPropertyVal.GetValue()));
	mpOwner->SetHelpString(wxString::Format("%s\n(Default '%s')", (const char*)PropertyDef.mzDescription, (const char*)PropertyDef.GetEnumEntry(PropertyDef.mDefault).mzDescription));
}

bool wxZenEnumProperty::TypedMetaData::Save()
{
	zenAssert( mpOwner != nullptr && mrPropertyValue.IsValid() );	
	if( mpOwner->GetValue() != mOriginalValue )
	{
		zenAss::PropertyEnum::ValueRef rPropertyVal(mrPropertyValue);
		rPropertyVal	= static_cast<zU32>(mpOwner->GetValue().GetLong());
		mOriginalValue	= mpOwner->GetValue();
		return true;
	}	
	return false;
}

//=================================================================================================
// PROPERTY : ASSET
//=================================================================================================
wxZenAssetProperty::wxZenAssetProperty(const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rPropertyValue)
{
	static zenMem::zAllocatorPool sPoolMetaData("Betl::TypedMetaData Pool", sizeof(TypedMetaData), 100, 100);
	zenAssert( _rPropertyValue.IsValid() );
	zenAss::PropertyAsset::ValueRef	rPropertyVal	= _rPropertyValue;
	const zenAss::PropertyAsset&	PropertyDef	= rPropertyVal.GetDefinition();
		
	SetClientData( zenNew(&sPoolMetaData)TypedMetaData(this, _rAsset, _rPropertyValue) );
	ChangeFlag(wxPG_PROP_READONLY, true);

	wxString zHelp		= wxString::Format("%s\nSupported Asset(s) : ", (const char*)PropertyDef.mzDescription);	
	if( PropertyDef.mSupportedType == zFlagAssetType(true) )
	{
		zHelp += "All";
	}
	else
	{
		wxString zSupported	= "";
		for( zUInt idx(0); idx< zenConst::keAssType__Count; ++idx)
		{
			if( PropertyDef.mSupportedType.Any( (zenConst::eAssetType)idx) )
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
}

wxZenAssetProperty::~wxZenAssetProperty()
{
	zenDel(GetClientData());
}

bool wxZenAssetProperty::OnEvent(wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event)
{
	if (event.GetEventType() == wxEVT_CHAR)
	{
		wxKeyEvent&	keyEvent = *static_cast<wxKeyEvent*>(&event);		
		TypedMetaData* pMeta = static_cast<TypedMetaData*>(GetClientData());
		if( keyEvent.GetKeyCode() == WXK_INSERT )
		{
			zenAss::PropertyAsset::ValueRef	rPropertyVal	= pMeta->mrPropertyValue;
			const zenAss::PropertyAsset& PropertyDef		= rPropertyVal.GetDefinition();
			zenAss::zAssetItemRef rAsset					= wxGetApp().mpFrame->GetWndAssetBrowser()->GetFirstSelectedItem( PropertyDef.mSupportedType );
			if( rAsset.IsValid() && rAsset != pMeta->mrAsset )
			{
				pMeta->AssignValue(rAsset.GetID());
				pMeta->UpdateControlState();
				return true;
			}
		}
		else if( keyEvent.GetKeyCode() == WXK_DELETE && pMeta->mAssetIDValue.IsValid() )
		{
			pMeta->AssignValue(zenAss::zAssetID());
			pMeta->UpdateControlState();
			return true;
		}
		else if( keyEvent.GetKeyCode() == WXK_HOME && pMeta->mAssetIDValue.IsValid() )
		{
			wxGetApp().mpFrame->GetWndAssetBrowser()->SelectItem( zenAss::AssetGet(pMeta->mAssetIDValue) );
		}
	}
	return false;
}

wxZenAssetProperty::TypedMetaData::TypedMetaData( wxZenAssetProperty* _pOwner, const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rPropertyValue)
: PropertyMetaData(_pOwner, _rAsset, _rPropertyValue, 0)
{
	zenAssert( mpOwner != nullptr && mrPropertyValue.IsValid() );
	zenAss::PropertyAsset::ValueRef rValue = _rPropertyValue;
	mAssetIDValue			= rValue.GetValue();
	mAssetIDOriginalValue	= mAssetIDValue;
}

void wxZenAssetProperty::TypedMetaData::UpdateControlState()
{
	zenAssert( mpOwner != nullptr && mrPropertyValue.IsValid() );
	mpOwner->SetBackgroundColour( ControlBGColor[ mrPropertyValue.IsDefault() ], 0 );
	mpOwner->SetModifiedStatus	( mAssetIDValue != mAssetIDOriginalValue );	
}

void wxZenAssetProperty::TypedMetaData::UpdateControl()
{
	zenAss::PropertyAsset::ValueRef rValue = mrPropertyValue;
	AssignValue(rValue.GetValue());
}

void wxZenAssetProperty::TypedMetaData::AssignValue(zenAss::zAssetID _AssetID)
{
	wxString zTextValue(L"None");
	zenAss::zAssetItemRef rAsset	= zenAss::AssetGet(_AssetID);
	mAssetIDValue					= zenAss::zAssetID();
	if (rAsset.IsValid())
	{
		zTextValue = L"";
		mAssetIDValue = _AssetID;
		zenAss::zPackageRef rPackage = rAsset.GetPackage();
		if (rPackage.IsValid())
		{
			zTextValue = L"[";
			zTextValue += rPackage.GetName().c_str();
			zTextValue += L"]";
			zTextValue += rAsset.GetName();
		}
	}
	mpOwner->SetValue(zTextValue);
}

bool wxZenAssetProperty::TypedMetaData::Save()
{
 	zenAssert( mpOwner != nullptr && mrPropertyValue.IsValid() );	
 	if( mAssetIDValue != mAssetIDOriginalValue )
 	{
 		zenAss::PropertyAsset::ValueRef rPropertyVal(mrPropertyValue);
 		rPropertyVal			= mAssetIDValue;
 		mAssetIDOriginalValue	= mAssetIDValue;
 		return true;
 	}	
	return false;
}

void wxZenAssetProperty::TypedMetaData::SetDefaultValue()
{
	zenAss::PropertyAsset::ValueRef	rPropertyVal = mrPropertyValue;
	const zenAss::PropertyAsset& PropertyDef = rPropertyVal.GetDefinition();
	AssignValue(PropertyDef.mDefault);
}

void wxZenAssetProperty::TypedMetaData::SetOriginalValue()
{
	AssignValue(mAssetIDOriginalValue);
}

//=================================================================================================
// PROPERTY : FILE
//=================================================================================================
wxZenFileProperty::wxZenFileProperty(const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rPropertyValue)
{
	static zenMem::zAllocatorPool sPoolMetaData("Betl::TypedMetaData Pool", sizeof(TypedMetaData), 100, 100);
	zenAss::PropertyFile::ValueRef	rPropertyVal		= _rPropertyValue;
	const zenAss::PropertyFile&		PropertyDef		= rPropertyVal.GetDefinition();
	wxFileName						Value			= (/*zenIO::GetRootPackage() + */rPropertyVal.GetValue()).c_str();
	wxFileName						ValueDefault	= (/*zenIO::GetRootPackage() +*/ PropertyDef.mDefault).c_str();
	SetClientData		( zenNew(&sPoolMetaData)TypedMetaData(this, _rAsset, _rPropertyValue, Value.GetFullPath()) );
	SetDefaultValue		( wxVariant(ValueDefault.GetFullPath()) );
		
	SetAttribute		( wxPG_FILE_WILDCARD,		(const zChar*)PropertyDef.mzFileExt);
	SetAttribute		( wxPG_FILE_DIALOG_TITLE,	wxString::Format("Select file for property : %s", (const char*)PropertyDef.mzDisplayName));
	SetAttribute		( wxPG_FILE_SHOW_RELATIVE_PATH, zenIO::GetRoot() );
	//SetAttribute		( wxPG_FILE_SHOW_FULL_PATH,	true);	
}

wxZenFileProperty::~wxZenFileProperty()
{
	zenDel(GetClientData());
}

void wxZenFileProperty::TypedMetaData::UpdateControl()
{
	zenAss::PropertyFile::ValueRef	rPropertyVal	= mrPropertyValue;
	const zenAss::PropertyFile&		PropertyDef	= rPropertyVal.GetDefinition();
	wxFileName						Value		= (/*zenIO::GetRootPackage() + */rPropertyVal.GetValue()).c_str();
	mpOwner->SetValue(Value.GetFullPath());
	mpOwner->SetHelpString((const char*)PropertyDef.mzDescription);
	mpOwner->SetAttribute( wxPG_FILE_INITIAL_PATH,	Value.GetPath());
	//! @todo Asset: Add validation for files not in root path
	//! @todo Asset: Root path not working when file not found?
}

bool wxZenFileProperty::TypedMetaData::Save()
{
	zenAssert( mpOwner != nullptr && mrPropertyValue.IsValid() );	
	if( mpOwner->GetValue() != mOriginalValue )
	{
		zenAss::PropertyFile::ValueRef rPropertyVal(mrPropertyValue);
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
wxZenArrayProperty::wxZenArrayProperty(const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rPropertyValue)
{
	static zenMem::zAllocatorPool sPoolMetaData("Betl::TypedMetaData Pool", sizeof(TypedMetaData), 100, 100);
	zenAssert( _rPropertyValue.IsValid() );
	zenAss::PropertyArray::ValueRef	rPropertyVal				= _rPropertyValue;
	const zenAss::PropertyArray&	PropertyDef					= rPropertyVal.GetDefinition();	
	const zArrayDynamic<zenAss::PropertyValueRef>& valueArray	= rPropertyVal.GetValue();
	SetClientData	( zenNew(&sPoolMetaData)TypedMetaData(this, _rAsset, _rPropertyValue, (long)valueArray.Count()) );
	SetHelpString	( wxString::Format("%s", (const char*)PropertyDef.mzDescription) );
	ChangeFlag		( wxPG_PROP_NOEDITOR, true );
	ChangeFlag		( wxPG_PROP_COLLAPSED, true );	
}

wxZenArrayProperty::~wxZenArrayProperty()
{
	zenDel(GetClientData());
}

void wxZenArrayProperty::TypedMetaData::UpdateControl()
{
	//! @todo Asset: Add/remove items as needed, instead of recreating 
	zenAss::PropertyArray::ValueRef	rPropertyVal				= mrPropertyValue;
	const zenAss::PropertyArray&	PropertyDef					= rPropertyVal.GetDefinition();
	const zArrayDynamic<zenAss::PropertyValueRef>& valueArray	= rPropertyVal.GetValue();
	mpOwner->DeleteChildren();
	for (zUInt idx(0), count(valueArray.Count()); idx < count; ++idx)
	{
		wxPGProperty* pProperty = CreateAssetValueControl(mrAsset, valueArray[idx]);
		pProperty->SetLabel(wxString::Format("%s[%i]", (const char*)PropertyDef.mzDisplayName, idx));
		mpOwner->AppendChild(pProperty); // AddPrivateChild doesn't let us update data?
	}
}

bool wxZenArrayProperty::TypedMetaData::Save()
{
	zenAssert( mpOwner != nullptr && mrPropertyValue.IsValid() );	
	zenAss::PropertyArray::ValueRef rPropertyVal(mrPropertyValue);
	zUInt uChilCount				= mpOwner->GetChildCount();	
	bool bChanged					= mOriginalValue.GetLong() != uChilCount;	
	mOriginalValue					= (long)uChilCount;
	for(zUInt idx(0); idx<uChilCount; ++idx)
	{
		wxPGProperty* pProperty		= mpOwner->Item(idx);	
		PropertyMetaData* pMetaData	= pProperty ? static_cast<PropertyMetaData*>(pProperty->GetClientData()) : nullptr;
		if( pMetaData )
			bChanged				|= pMetaData->Save();
	}	
	return bChanged;
}

void wxZenArrayProperty::TypedMetaData::UpdateControlState()
{
	PropertyMetaData::UpdateControlState();
	for(zUInt idx(0), count(mpOwner->GetChildCount()); idx<count; ++idx )
	{
		PropertyMetaData* pMetaData = static_cast<PropertyMetaData*>( mpOwner->Item(idx)->GetClientData() );
		if( pMetaData )
			pMetaData->UpdateControlState();
	}
}

//=================================================================================================
// PROPERTY : STRUCT
//=================================================================================================
wxZenStructProperty::wxZenStructProperty(const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rPropertyValue)
{
	static zenMem::zAllocatorPool sPoolMetaData("Betl::TypedMetaData Pool", sizeof(TypedMetaData), 100, 100);
	zenAssert( _rPropertyValue.IsValid() );
	zenAss::PropertyStruct::ValueRef rPropertyVal				= _rPropertyValue;
	const zenAss::PropertyStruct& PropertyDef					= rPropertyVal.GetDefinition();	
	const zArrayStatic<zenAss::PropertyValueRef>& aStructValue	= rPropertyVal.GetValue();
	SetClientData	( zenNew(&sPoolMetaData)TypedMetaData(this, _rAsset, _rPropertyValue, 0) );
	SetHelpString	( wxString::Format("%s", (const char*)PropertyDef.mzDescription) );
	ChangeFlag		( wxPG_PROP_NOEDITOR, true );
	ChangeFlag		( wxPG_PROP_COLLAPSED, true );		
	
}

wxZenStructProperty::~wxZenStructProperty()
{
	zenDel(GetClientData());
}

void wxZenStructProperty::TypedMetaData::UpdateControl()
{
	//! @todo Asset: Add/remove items as needed, instead of recreating 
	zenAss::PropertyStruct::ValueRef rPropertyVal				= mrPropertyValue;
	const zenAss::PropertyStruct& PropertyDef					= rPropertyVal.GetDefinition();
	const zArrayStatic<zenAss::PropertyValueRef>& aStructValue	= rPropertyVal.GetValue();
	mpOwner->DeleteChildren();
	for (zUInt idx(0), count(aStructValue.Count()); idx < count; ++idx)
	{
		wxPGProperty* pProperty = CreateAssetValueControl(mrAsset, aStructValue[idx]);
		mpOwner->AppendChild(pProperty);
	}
}

void wxZenStructProperty::TypedMetaData::UpdateControlState()
{
	PropertyMetaData::UpdateControlState();
	for (zUInt idx(0), count(mpOwner->GetChildCount()); idx < count; ++idx)
	{
		PropertyMetaData* pMetaData = static_cast<PropertyMetaData*>(mpOwner->Item(idx)->GetClientData());
		if (pMetaData)
			pMetaData->UpdateControlState();
	}
}

bool wxZenStructProperty::TypedMetaData::Save()
{
	zenAssert( mpOwner != nullptr && mrPropertyValue.IsValid() );	
	zUInt uChilCount	= mpOwner->GetChildCount();
	bool bChanged		= false;
	for (zUInt idx(0); idx < uChilCount; ++idx)
	{
		wxPGProperty* pProperty = mpOwner->Item(idx);
		PropertyMetaData* pMetaData = pProperty ? static_cast<PropertyMetaData*>(pProperty->GetClientData()) : nullptr;
		if (pMetaData)
			bChanged |= pMetaData->Save();
	}

	return bChanged;
}



}
