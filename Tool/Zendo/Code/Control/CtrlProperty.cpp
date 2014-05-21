#include "ZendoPCH.h"
#include "CtrlProperty.h"

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
static zenMem::zAllocatorPool sPoolMetaData("Betl::PropertyMetaData Pool", sizeof(PropertyMetaData), 100, 100);

//=================================================================================================
//! @brief		Common setup applied to each property control
//! @details	Used to avoid code duplication
//-------------------------------------------------------------------------------------------------
//! @param _Value		- Asset value to edit
//! @return				- Created property control
//=================================================================================================
wxPGProperty* CreateAssetValueControl(zenAss::PropertyValueRef& _Value)
{
	wxPGProperty* pProperty(NULL);
	switch( _Value.GetType() )
	{
	case zenConst::keAssProp_Bool:		pProperty=zenNewDefault wxBetlBoolProperty(_Value);			break;
 	case zenConst::keAssProp_Enum:		pProperty=zenNewDefault wxBetlEnumProperty(_Value);			break;
 	case zenConst::keAssProp_File:		pProperty=zenNewDefault wxBetlFileProperty(_Value);			break;
 	case zenConst::keAssProp_Float:		pProperty=zenNewDefault wxBetlFloatProperty(_Value);		break;	
 	case zenConst::keAssProp_Float2:	pProperty=zenNewDefault wxBetlFloat2fProperty(_Value);		break;
 	case zenConst::keAssProp_Float3:	pProperty=zenNewDefault wxBetlFloat3fProperty(_Value);		break;
 	case zenConst::keAssProp_Float4:	pProperty=zenNewDefault wxBetlFloat4fProperty(_Value);		break;
 	case zenConst::keAssProp_Int:		pProperty=zenNewDefault wxBetlIntProperty(_Value);			break;
 	case zenConst::keAssProp_Int2:		pProperty=zenNewDefault wxBetlInt2fProperty(_Value);		break;
 	case zenConst::keAssProp_Int3:		pProperty=zenNewDefault wxBetlInt3fProperty(_Value);		break;
 	case zenConst::keAssProp_Int4:		pProperty=zenNewDefault wxBetlInt4fProperty(_Value);		break;
 	case zenConst::keAssProp_Array:		pProperty=zenNewDefault wxBetlArrayProperty(_Value);		break;
 	case zenConst::keAssProp_Struct:	pProperty=zenNewDefault wxBetlStructProperty(_Value);		break;
	default:							ZENAssertMsg(0, "Unknown property type, implement it.")		break;
	}	

	if( pProperty )
	{		
		const zenAss::PropertyDefinition& PropertyDef = _Value.GetDefinition();
		pProperty->SetName				( (const char*)PropertyDef.mName.mzName );
		pProperty->SetLabel				( (const char*)PropertyDef.mzDisplayName );				
		pProperty->SetModifiedStatus	( !_Value.IsDefault() );
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

//=================================================================================================
// PROPERTY : BOOL
//=================================================================================================
wxBetlBoolProperty::wxBetlBoolProperty(const zenAss::PropertyValueRef& _rAssetValue)
{
	zenAss::PropertyBool::ValueRef	rPropertyVal	= _rAssetValue;
	const zenAss::PropertyBool&		PropertyDef		= rPropertyVal.GetDefinition();	
	const wxString					zFalseTrue[2]	= {wxT("False"), wxT("True")};	
	SetClientData		( zenNew(&sPoolMetaData)PropertyMetaData(_rAssetValue, rPropertyVal.Get()) );
	SetDefaultValue		( wxVariant(PropertyDef.mDefault) );
	SetValue			( rPropertyVal.Get() );
	SetHelpString		( wxString::Format("%s\n(Default %s)", (const char*)PropertyDef.mzDescription, zFalseTrue[PropertyDef.mDefault]));
	SetAttribute		( wxPG_BOOL_USE_CHECKBOX, true );
}

wxBetlBoolProperty::~wxBetlBoolProperty()
{
	zenDel(GetClientData());
}

//=================================================================================================
// PROPERTY : FLOAT
//=================================================================================================
wxBetlFloatProperty::wxBetlFloatProperty(const zenAss::PropertyValueRef& _rAssetValue)
{
	ZENAssert( _rAssetValue.IsValid() );
	zenAss::PropertyFloat::ValueRef	rAssetVal	= _rAssetValue;
	const zenAss::PropertyFloat&	AssetProp	= rAssetVal.GetDefinition();	
	SetClientData		( zenNew(&sPoolMetaData)PropertyMetaData(_rAssetValue, rAssetVal.Get()) );
	SetDefaultValue		( wxVariant(AssetProp.mDefault) );
	SetValue			( rAssetVal.Get() );
	SetHelpString		( wxString::Format("%s\n(Default %.3f) (Min %.3f) (Max %.3f)", (const char*)AssetProp.mzDescription, AssetProp.mDefault, AssetProp.mValMin, AssetProp.mValMax));
	ConfigurePropertyScalar(*this, AssetProp.mValMin, AssetProp.mValMax, AssetProp.mValInc);
}

wxBetlFloatProperty::~wxBetlFloatProperty()
{
	zenDel(GetClientData());
}

//=================================================================================================
// PROPERTY : INT
//=================================================================================================
wxBetlIntProperty::wxBetlIntProperty(const zenAss::PropertyValueRef& _rAssetValue)
{	
	ZENAssert( _rAssetValue.IsValid() );
	zenAss::PropertyInt::ValueRef	rAssetVal	= _rAssetValue;
	const zenAss::PropertyInt&		AssetProp	= rAssetVal.GetDefinition();	
	SetClientData		( zenNew(&sPoolMetaData)PropertyMetaData(_rAssetValue, rAssetVal.Get()) );
	SetDefaultValue		( wxVariant(AssetProp.mDefault) );
	SetValue			( rAssetVal.Get() );	
	SetHelpString		( wxString::Format("%s\n(Default %i) (Min %i) (Max %i)", (const char*)AssetProp.mzDescription, AssetProp.mDefault, AssetProp.mValMin, AssetProp.mValMax));
	ConfigurePropertyScalar(*this, AssetProp.mValMin, AssetProp.mValMax, AssetProp.mValInc);
}

wxBetlIntProperty::~wxBetlIntProperty()
{
	zenDel(GetClientData());
}

//=================================================================================================
// PROPERTY : VECTOR
//=================================================================================================
template< class TPropertyClass, class TElementCast, class TWxVector, class TWxProperty >
wxBetlVectorProperty<TPropertyClass, TElementCast, TWxVector, TWxProperty>::wxBetlVectorProperty(const zenAss::PropertyValueRef& _rAssetValue, const char* _zTooltip, const char* _zTooltipElement)
{		
	ZENAssert( _rAssetValue.IsValid() );
	TPropertyClass::ValueRef	rAssetVal	= _rAssetValue;
	const TPropertyClass&		AssetProp	= rAssetVal.GetDefinition();	

	TWxVector wxValue;
	TWxVector wxDefault;
	for(zUInt idx(0); idx<ZENArrayCount(AssetProp.mDefault.values); ++idx)
	{
		wxValue.values[idx]		= rAssetVal.Get().values[idx];
		wxDefault.values[idx]	= AssetProp.mDefault.values[idx];
	}

	//PropertyMetaData* pMetaData	= zenNew(&sPoolMetaData)PropertyMetaData(_rAssetValue, WXVARIANT(wxValue));
	for(zUInt idx(0); idx<ZENArrayCount(rAssetVal.Get().values); ++idx)
	{
		const wxString zNames[]={wxT("    X"),wxT("    Y"),wxT("    Z"),wxT("    W")};
		TWxProperty* pProp					= zenNewDefault TWxProperty( wxString::Format("%s", zNames[idx]),wxPG_LABEL);

		pProp->SetClientData	( zenNew(&sPoolMetaData)PropertyMetaData(_rAssetValue, rAssetVal.Get().values[idx]) );
		pProp->SetDefaultValue	( wxVariant(AssetProp.mDefault.values[idx]) );
		pProp->SetValue			( wxVariant(rAssetVal.Get().values[idx]) );
		pProp->SetHelpString	( wxString::Format(_zTooltipElement, (const char*)AssetProp.mzDescription, AssetProp.mDefault.values[idx]));
		ConfigurePropertyScalar(*pProp, AssetProp.mValMin.values[idx], AssetProp.mValMax.values[idx], AssetProp.mValInc.values[idx]);
		AddPrivateChild(pProp);
	}
	SetValue					( WXVARIANT(wxValue) );
	SetDefaultValue				( WXVARIANT(wxDefault) );
	SetClientData				( zenNew(&sPoolMetaData)PropertyMetaData(_rAssetValue, WXVARIANT(wxValue)) );
	switch( ZENArrayCount(AssetProp.mDefault.values) )
	{
	case 1: SetHelpString( wxString::Format(_zTooltip, (const char*)AssetProp.mzDescription, AssetProp.mDefault.values[0]));	break;
	case 2: SetHelpString( wxString::Format(_zTooltip, (const char*)AssetProp.mzDescription, AssetProp.mDefault.values[0],AssetProp.mDefault.values[1]));	break;
	case 3: SetHelpString( wxString::Format(_zTooltip, (const char*)AssetProp.mzDescription, AssetProp.mDefault.values[0],AssetProp.mDefault.values[1],AssetProp.mDefault.values[2]));	break;
	case 4: SetHelpString( wxString::Format(_zTooltip, (const char*)AssetProp.mzDescription, AssetProp.mDefault.values[0],AssetProp.mDefault.values[1],AssetProp.mDefault.values[2],AssetProp.mDefault.values[3]));	break;
	}	
}

template< class TPropertyClass, class TElementCast, class TWxVector, class TWxProperty >
wxBetlVectorProperty<TPropertyClass, TElementCast, TWxVector, TWxProperty>::~wxBetlVectorProperty() 
{
	zenDel(GetClientData());
}

template< class TPropertyClass, class TElementCast, class TWxVector, class TWxProperty >
void wxBetlVectorProperty<TPropertyClass, TElementCast, TWxVector, TWxProperty>::RefreshChildren()
{
	if ( !GetChildCount() ) return;
	TWxVector vector;
	vector << m_value;
	for( zUInt idx(0); idx<ZENArrayCount(vector.values); ++idx)
		Item(idx)->SetValue( vector.values[idx] );
}

template< class TPropertyClass, class TElementCast, class TWxVector, class TWxProperty >
wxVariant wxBetlVectorProperty<TPropertyClass, TElementCast, TWxVector, TWxProperty>::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{
	TWxVector vector;
	vector << thisValue;
	vector.values[childIndex] = (TElementCast)childValue;
	wxVariant newVariant;
	newVariant << vector;
	return newVariant;
}
WX_PG_IMPLEMENT_PROPERTY_CLASS(wxBetlFloat2fProperty, wxPGProperty, wxVector2f, const wxVector2f&, TextCtrl)
WX_PG_IMPLEMENT_PROPERTY_CLASS(wxBetlFloat3fProperty, wxPGProperty, wxVector3f, const wxVector3f&, TextCtrl)
WX_PG_IMPLEMENT_PROPERTY_CLASS(wxBetlFloat4fProperty, wxPGProperty, wxVector4f, const wxVector4f&, TextCtrl)
WX_PG_IMPLEMENT_PROPERTY_CLASS(wxBetlInt2fProperty, wxPGProperty, wxVector2s, const wxVector2s&, TextCtrl)
WX_PG_IMPLEMENT_PROPERTY_CLASS(wxBetlInt3fProperty, wxPGProperty, wxVector3s, const wxVector3s&, TextCtrl)
WX_PG_IMPLEMENT_PROPERTY_CLASS(wxBetlInt4fProperty, wxPGProperty, wxVector4s, const wxVector4s&, TextCtrl)

//=================================================================================================
// PROPERTY : ENUM
//=================================================================================================
wxBetlEnumProperty::wxBetlEnumProperty(const zenAss::PropertyValueRef& _rAssetValue)
{
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
	SetClientData		( zenNew(&sPoolMetaData)PropertyMetaData(_rAssetValue, long(rAssetVal.Get())) );
	SetChoices			( soc );	
	SetDefaultValue		( wxVariant(long(AssetProp.mDefault)) );
	SetValue			( long(rAssetVal.Get()) );
	SetHelpString		( wxString::Format("%s\n(Default '%s')", (const char*)AssetProp.mzDescription, (const char*)AssetProp.GetEnumEntry(AssetProp.mDefault).mzDescription) );
}

wxBetlEnumProperty::~wxBetlEnumProperty()
{
	zenDel(GetClientData());
}

//=================================================================================================
// PROPERTY : FILE
//=================================================================================================
wxBetlFileProperty::wxBetlFileProperty(const zenAss::PropertyValueRef& _rAssetValue)
{
	zenAss::PropertyFile::ValueRef	rAssetVal		= _rAssetValue;
	const zenAss::PropertyFile&		AssetProp		= rAssetVal.GetDefinition();	
	wxFileName						Value			= ((const char*)rAssetVal.Get());
	wxFileName						ValueDefault	= (const char*)AssetProp.mDefault;
	SetClientData		( zenNew(&sPoolMetaData)PropertyMetaData(_rAssetValue, Value.GetFullPath()) );
	SetDefaultValue		( wxVariant(ValueDefault.GetFullPath()) );
	SetValue			( Value.GetFullPath() );
	SetHelpString		( (const char*)AssetProp.mzDescription );
	SetAttribute		( wxPG_FILE_INITIAL_PATH,	Value.GetPath());
	SetAttribute		( wxPG_FILE_WILDCARD,		(const char*)AssetProp.mzFileExt);
	SetAttribute		( wxPG_FILE_DIALOG_TITLE,	wxString::Format("Open file for '%s'", (const char*)AssetProp.mzDisplayName));
	SetAttribute		( wxPG_FILE_SHOW_FULL_PATH,	true);
}

wxBetlFileProperty::~wxBetlFileProperty()
{
	zenDel(GetClientData());
}

//=================================================================================================
// PROPERTY : ARRAY
//=================================================================================================
wxBetlArrayProperty::wxBetlArrayProperty(const zenAss::PropertyValueRef& _rAssetValue)
{
	ZENAssert( _rAssetValue.IsValid() );
	zenAss::PropertyArray::ValueRef	rAssetVal			= _rAssetValue;
	const zenAss::PropertyArray&	AssetProp			= rAssetVal.GetDefinition();	
	zArrayDynamic<zenAss::PropertyValueRef>& valueArray	= rAssetVal.Get();
	SetClientData	( zenNew(&sPoolMetaData)PropertyMetaData(_rAssetValue, 0) );
	ChangeFlag		( wxPG_PROP_NOEDITOR, true );	
	ChangeFlag		( wxPG_PROP_COLLAPSED, true );		
	for(zUInt idx(0), count(valueArray.Count()); idx<count; ++idx)
	{
		wxPGProperty* pProperty = CreateAssetValueControl(valueArray[idx]);	
		pProperty->SetLabel( wxString::Format("[%i]", idx) );
		AppendChild(pProperty);
	}
}

wxBetlArrayProperty::~wxBetlArrayProperty()
{
	zenDel(GetClientData());
}

//=================================================================================================
// PROPERTY : STRUCT
//=================================================================================================
wxBetlStructProperty::wxBetlStructProperty(const zenAss::PropertyValueRef& _rAssetValue)
{
	ZENAssert( _rAssetValue.IsValid() );
	zenAss::PropertyStruct::ValueRef	rAssetVal			= _rAssetValue;
	const zenAss::PropertyStruct&		AssetProp			= rAssetVal.GetDefinition();	
	zArrayStatic<zenAss::PropertyValueRef>& aStructValue	= rAssetVal.Get();
	SetClientData	( zenNew(&sPoolMetaData)PropertyMetaData(_rAssetValue, 0) );
	ChangeFlag		( wxPG_PROP_NOEDITOR, true );
	ChangeFlag		( wxPG_PROP_COLLAPSED, true );		
	for(zUInt idx(0), count(aStructValue.Count()); idx<count; ++idx)
	{
		wxPGProperty* pProperty = CreateAssetValueControl(aStructValue[idx]);
		AppendChild(pProperty);
	}
}

wxBetlStructProperty::~wxBetlStructProperty()
{
	zenDel(GetClientData());
}

//=================================================================================================
//! @brief		Common setup applied to each property control
//! @details	Used to avoid code duplication
//-------------------------------------------------------------------------------------------------
//! @param _GridControl	- PropertyGrid control to add the new value edition
//! @param _Value		- Asset value to edit
//! @return				- Created property control
//=================================================================================================
wxPGProperty* CreateAssetValueControl(wxPropertyGridInterface& _GridControl, zenAss::PropertyValueRef& _Value)
{
	static const wxColour bgColors[]={wxColour(230,240,250,255),wxColour(240,245,240,255)};
	static zUInt suLineIndex(0);
	wxPGProperty* pProperty = CreateAssetValueControl(_Value);
	if( pProperty )
	{		
		_GridControl.Append				( pProperty );
		pProperty->SetBackgroundColour	( bgColors[++suLineIndex%ZENArrayCount(bgColors)] );
	}	
	return pProperty;
}

}
