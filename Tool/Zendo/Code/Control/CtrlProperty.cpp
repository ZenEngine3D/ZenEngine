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
wxBetlBoolProperty::wxBetlBoolProperty(zenAss::PropertyValue& _AssetValue)
{
	const zenAss::PropertyBool&	Property		= _AssetValue.GetPropertyBool();
	const wxString				zFalseTrue[2]	= {wxT("False"), wxT("True")};	
	SetClientData	( zenNew(&sPoolMetaData)PropertyMetaData(_AssetValue, _AssetValue.GetValueBool()) );
	SetDefaultValue	( wxVariant(Property.mDefault) );
	SetValue		( _AssetValue.GetValueBool() );	
	SetHelpString	( wxString::Format("%s\n(Default %s)", Property.mzDescription, zFalseTrue[Property.mDefault]));
	SetAttribute	( wxPG_BOOL_USE_CHECKBOX, true );
}

wxBetlBoolProperty::~wxBetlBoolProperty()
{
	zenDel(GetClientData());
}

//=================================================================================================
// PROPERTY : FLOAT
//=================================================================================================
wxBetlFloatProperty::wxBetlFloatProperty(zenAss::PropertyValue& _AssetValue)
{
	const zenAss::PropertyFloat& Property = _AssetValue.GetPropertyFloat();
	SetClientData		( zenNew(&sPoolMetaData)PropertyMetaData(_AssetValue, _AssetValue.GetValueFloat()) );
	SetDefaultValue		( wxVariant(Property.mDefault) );
	SetValue			( wxVariant(_AssetValue.GetValueFloat()) );
	SetEditor			( wxPGEditor_SpinCtrl );
	SetHelpString		( wxString::Format("%s\n(Default %.3f) (Min %.3f) (Max %.3f)", Property.mzDescription, Property.mDefault, Property.mValMin, Property.mValMax));
	ConfigurePropertyScalar(*this, Property.mValMin, Property.mValMax, Property.mValInc);
}

wxBetlFloatProperty::~wxBetlFloatProperty()
{
	zenDel(GetClientData());
}

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxBetlFloat2fProperty, wxPGProperty, wxVector2f, const wxVector2f&, TextCtrl)
WX_PG_IMPLEMENT_PROPERTY_CLASS(wxBetlFloat3fProperty, wxPGProperty, wxVector3f, const wxVector3f&, TextCtrl)
WX_PG_IMPLEMENT_PROPERTY_CLASS(wxBetlFloat4fProperty, wxPGProperty, wxVector4f, const wxVector4f&, TextCtrl)

//=================================================================================================
// PROPERTY : INT
//=================================================================================================
wxBetlIntProperty::wxBetlIntProperty(zenAss::PropertyValue& _AssetValue)
{
	const zenAss::PropertyInt& Property	= _AssetValue.GetPropertyInt();
	SetClientData		( zenNew(&sPoolMetaData)PropertyMetaData(_AssetValue, _AssetValue.GetValueInt()) );
	SetDefaultValue		( wxVariant(Property.mDefault) );
	SetValue			( wxVariant(_AssetValue.GetValueInt()) );
	SetEditor			( wxPGEditor_SpinCtrl );
	SetHelpString		( wxString::Format("%s\n(Default %i) (Min %i) (Max %i)", Property.mzDescription, Property.mDefault, Property.mValMin, Property.mValMax));
	ConfigurePropertyScalar(*this, Property.mValMin, Property.mValMax, Property.mValInc);
}

wxBetlIntProperty::~wxBetlIntProperty()
{
	zenDel(GetClientData());
}

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxBetlInt2fProperty, wxPGProperty, wxVector2s, const wxVector2s&, TextCtrl)
WX_PG_IMPLEMENT_PROPERTY_CLASS(wxBetlInt3fProperty, wxPGProperty, wxVector3s, const wxVector3s&, TextCtrl)
WX_PG_IMPLEMENT_PROPERTY_CLASS(wxBetlInt4fProperty, wxPGProperty, wxVector4s, const wxVector4s&, TextCtrl)

//=================================================================================================
// PROPERTY : FILE
//=================================================================================================
wxBetlFileProperty::wxBetlFileProperty(zenAss::PropertyValue& _AssetValue)
{
	wxFileName Value((const char*)_AssetValue.GetValueFile());
	const zenAss::PropertyFile& Property	= _AssetValue.GetPropertyFile();
	SetClientData		( zenNew(&sPoolMetaData)PropertyMetaData(_AssetValue, Value.GetFullPath()) );
	SetDefaultValue		( wxVariant((const char*)Property.mDefault) );
	SetValue			( Value.GetFullPath() );
	SetHelpString		( Property.mzDescription );
	SetAttribute		( wxPG_FILE_INITIAL_PATH,	Value.GetPath());
	SetAttribute		( wxPG_FILE_WILDCARD,		Property.mzFileExt);
	SetAttribute		( wxPG_FILE_DIALOG_TITLE,	wxString::Format("Open file for '%s'", Property.mzDisplayName));
	SetAttribute		( wxPG_FILE_SHOW_FULL_PATH,	true);
}

wxBetlFileProperty::~wxBetlFileProperty()
{
	zenDel(GetClientData());
}

//=================================================================================================
// PROPERTY : VECTOR
//=================================================================================================
template< class TPropertyClass, class TElementCast, class TWxVector, class TWxProperty >
wxBetlVectorProperty<TPropertyClass, TElementCast, TWxVector, TWxProperty>::wxBetlVectorProperty(zenAss::PropertyValue& _AssetValue, const char* _zTooltip, const char* _zTooltipElement)
{		
	const TPropertyClass*		pProperty	= static_cast<const TPropertyClass*>(_AssetValue.GetProperty());
	const TPropertyClass::Data* pValue		= static_cast<const TPropertyClass::Data*>(_AssetValue.GetValue());

	TWxVector wxValue;
	TWxVector wxDefault;
	for(zUInt idx(0); idx<ZENArrayCount(pValue->values); ++idx)
	{
		wxValue.values[idx]		= pValue->values[idx];
		wxDefault.values[idx]	= pProperty->mDefault.values[idx];
	}
	
	PropertyMetaData* pMetaData						= zenNew(&sPoolMetaData)PropertyMetaData(_AssetValue, WXVARIANT(wxValue));
	for(zUInt idx(0); idx<ZENArrayCount(pValue->values); ++idx)
	{
		const wxString zNames[]={wxT("    X"),wxT("    Y"),wxT("    Z"),wxT("    W")};
		TWxProperty* pProp					= zenNewDefault TWxProperty( wxString::Format("%s", zNames[idx]),wxPG_LABEL);
		
		pProp->SetClientData	( zenNew(&sPoolMetaData)PropertyMetaData(_AssetValue, pValue->values[idx]) );
		pProp->SetDefaultValue	( wxVariant(pProperty->mDefault.values[idx]) );
		pProp->SetValue			( wxVariant(pValue->values[idx]) );
		pProp->SetHelpString	( wxString::Format(_zTooltipElement, pProperty->mzDescription, pProperty->mDefault.values[idx]));
		ConfigurePropertyScalar(*pProp, pProperty->mValMin.values[idx], pProperty->mValMax.values[idx], pProperty->mValInc.values[idx]);
		AddPrivateChild(pProp);
	}
	SetValue					( WXVARIANT(wxValue) );
	SetDefaultValue				( WXVARIANT(wxDefault) );
	switch( ZENArrayCount(pProperty->mDefault.values) )
	{
	case 1: SetHelpString( wxString::Format(_zTooltip, pProperty->mzDescription, pProperty->mDefault.values[0]));	break;
	case 2: SetHelpString( wxString::Format(_zTooltip, pProperty->mzDescription, pProperty->mDefault.values[0],pProperty->mDefault.values[1]));	break;
	case 3: SetHelpString( wxString::Format(_zTooltip, pProperty->mzDescription, pProperty->mDefault.values[0],pProperty->mDefault.values[1],pProperty->mDefault.values[2]));	break;
	case 4: SetHelpString( wxString::Format(_zTooltip, pProperty->mzDescription, pProperty->mDefault.values[0],pProperty->mDefault.values[1],pProperty->mDefault.values[2],pProperty->mDefault.values[3]));	break;
	}	
}

template< class TPropertyClass, class TElementCast, class TWxVector, class TWxProperty >
wxBetlVectorProperty<TPropertyClass, TElementCast, TWxVector, TWxProperty>::~wxBetlVectorProperty() 
{
	zenDel(GetClientData());
	//! @todo Asset: Delete child client data
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

//=================================================================================================
// PROPERTY :
//=================================================================================================
wxBetlEnumProperty::wxBetlEnumProperty(zenAss::PropertyValue& _AssetValue)
{
	const zenAss::PropertyEnum& Property = _AssetValue.GetPropertyEnum();	
	wxPGChoices soc;
	for(zUInt enumIdx(0), enumCount(Property.maEnumValues.Count()); enumIdx<enumCount; ++enumIdx)
	{
		soc.Add( (const char*)Property.maEnumValues[enumIdx].mzDescription, Property.maEnumValues[enumIdx].miValue );
	}
	//soc[PropertyDef.muDefault].SetBgCol( wxColour(200,255,200,1) );
	//soc[PropertyDef.muDefault].SetBitmap(wxArtProvider::GetBitmap(wxART_INFORMATION));
	SetClientData		( zenNew(&sPoolMetaData)PropertyMetaData(_AssetValue, _AssetValue.GetValueEnum()) );
	SetChoices			( soc );	
	SetDefaultValue		( wxVariant(Property.mDefault) );
	SetValue			( _AssetValue.GetValueEnum() );
	SetHelpString		( wxString::Format("%s\n(Default '%s')", Property.mzDescription, (const char*)Property.GetEnumEntry(Property.mDefault).mzDescription) );
}

wxBetlEnumProperty::~wxBetlEnumProperty()
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
wxPGProperty* CreateAssetValueControl(wxPropertyGridInterface& _GridControl, zenAss::PropertyValue& _Value)
{
	static const wxColour bgColors[]={wxColour(230,240,250,255),wxColour(240,245,240,255)};
	static zUInt suLineIndex(0);
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
	default:							ZENAssertMsg(0, "Unknown property type, implement it.")		break;
	}	

	if( pProperty )
	{		
		const zenAss::PropertyBase* pPropertyDef = _Value.GetProperty();
		pProperty->SetName				( (const char*)pPropertyDef->mName.mzName );
		pProperty->SetLabel				( pPropertyDef->mzDisplayName );				
		pProperty->SetModifiedStatus	( pProperty->GetValue() != pProperty->GetDefaultValue() );
		_GridControl.Append				( pProperty );
		pProperty->SetBackgroundColour	( bgColors[++suLineIndex%ZENArrayCount(bgColors)] );
	}	
	return pProperty;
}

}
