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
	case zenConst::keAssProp_File:		pProperty=zenNewDefault wxBetlFileProperty(_Value);			break;
	case zenConst::keAssProp_Float:		pProperty=zenNewDefault wxBetlFloatProperty(_Value);		break;	
	case zenConst::keAssProp_Float2:	pProperty=zenNewDefault wxBetlFloat2fProperty(_Value);		break;
#if 0
	case zenConst::keAssProp_Int:		pProperty=zenNewDefault wxBetlIntProperty(_Value.GetInt());				break;
		// 		case AAss::zenConst::keAssProp_Int2:			break;
		// 		case AAss::zenConst::keAssProp_Int3:			break;
		// 		case AAss::zenConst::keAssProp_Int4:			break;

	
		// 		case AAss::zenConst::keAssProp_Float3:			break;
		// 		case AAss::zenConst::keAssProp_Float4:			break;
	case zenConst::keAssProp_Enum:	pProperty=zenNewDefault wxBetlEnumProperty(_Value.GetEnum());			break;
#endif
	default:									ZENAssertMsg(0, "Unknown property type, implement it")			break;
	}	
	
	if( pProperty )
	{		
		const zenAss::PropertyBase* pPropertyDef = _Value.GetProperty();
		pProperty->SetName				( pPropertyDef->mName.mzName );
		pProperty->SetLabel				( pPropertyDef->mzDisplayName );				
		pProperty->SetModifiedStatus	( pProperty->GetValue() != pProperty->GetDefaultValue() );
		_GridControl.Append				( pProperty );
		pProperty->SetBackgroundColour	( bgColors[++suLineIndex%ZENArrayCount(bgColors)] );
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
wxBetlBoolProperty::wxBetlBoolProperty(zenAss::PropertyValue& _AssetValue)
{
	const zenAss::PropertyBool&	Property		= _AssetValue.GetPropertyBool();
	PropertyMetaData*			pMetaData		= zenNew(&sPoolMetaData)PropertyMetaData(_AssetValue, _AssetValue.GetValueBool());
	const wxString				zFalseTrue[2]	= {wxT("False"), wxT("True")};	
	SetClientData	( pMetaData );
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
	const zenAss::PropertyFloat& Property	= _AssetValue.GetPropertyFloat();
	PropertyMetaData* pMetaData				= zenNew(&sPoolMetaData)PropertyMetaData(_AssetValue, _AssetValue.GetValueFloat());
	SetClientData		( pMetaData );
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

//=================================================================================================
// PROPERTY : FILE
//=================================================================================================
wxBetlFileProperty::wxBetlFileProperty(zenAss::PropertyValue& _AssetValue)
{
	wxFileName Value((const char*)_AssetValue.GetValueFile());
	const zenAss::PropertyFile& Property	= _AssetValue.GetPropertyFile();
	PropertyMetaData* pMetaData				= zenNew(&sPoolMetaData)PropertyMetaData(_AssetValue, Value.GetFullPath());
	SetClientData		( pMetaData );
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
// PROPERTY : FLOAT2
//=================================================================================================
WX_PG_IMPLEMENT_PROPERTY_CLASS(wxBetlFloat2fProperty, wxPGProperty, wxVector2f, const wxVector2f&, TextCtrl)
wxBetlFloat2fProperty::wxBetlFloat2fProperty(zenAss::PropertyValue& _AssetValue)
{		
	const zenAss::PropertyFloat2&		Property	= _AssetValue.GetPropertyFloat2();
	const zenAss::PropertyFloat2::Data& Value		= _AssetValue.GetValueFloat2();
	
	wxVector2f vValue(Value.x, Value.y);	
	wxVector2f vDefault( Property.mDefault.x, Property.mDefault.y);
	PropertyMetaData* pMetaData						= zenNew(&sPoolMetaData)PropertyMetaData(_AssetValue, WXVARIANT(vValue));

	for(zUInt idx(0); idx<2; ++idx)
	{
		const wxString zNames[]={wxT("-->X"),wxT("-->Y")};
		wxFloatProperty* pProp				= zenNewDefault wxFloatProperty(zNames[idx],wxPG_LABEL);
		PropertyMetaData* pMetaDataChild	= zenNew(&sPoolMetaData)PropertyMetaData(_AssetValue, Value.xy[idx]);
		pProp->SetClientData	( pMetaDataChild );
		pProp->SetDefaultValue	( wxVariant(Property.mDefault.xy[idx]) );
		pProp->SetValue			( wxVariant(Value.xy[idx]) );
		pProp->SetHelpString	( wxString::Format("%s\n(Default %.3f)", Property.mzDescription, Property.mDefault.xy[idx]));
		ConfigurePropertyScalar(*pProp, Property.mValMin.xy[idx], Property.mValMax.xy[idx], Property.mValInc.xy[idx]);
		AddPrivateChild(pProp);
	}
	SetValue					( WXVARIANT(vValue) );
	SetDefaultValue				( WXVARIANT(vDefault) );
	SetHelpString				( wxString::Format("%s\n(Default [ %.3f, %.3f ])", Property.mzDescription, Property.mDefault.x, Property.mDefault.y));	
}

wxBetlFloat2fProperty::~wxBetlFloat2fProperty() 
{
	zenDel(GetClientData());
}

void wxBetlFloat2fProperty::RefreshChildren()
{
	if ( !GetChildCount() ) return;
	const wxVector2f& vector = wxVector2fRefFromVariant(m_value);
	Item(0)->SetValue( vector.x );
	Item(1)->SetValue( vector.y );
}

wxVariant wxBetlFloat2fProperty::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{
	wxVector2f vector;
	vector << thisValue;
	switch ( childIndex )
	{
	case 0: vector.x = childValue.GetDouble(); break;
	case 1: vector.y = childValue.GetDouble(); break;
	}
	wxVariant newVariant;
	newVariant << vector;
	return newVariant;
}

#if 0
//=================================================================================================
// PROPERTY :
//=================================================================================================
wxBetlIntProperty::wxBetlIntProperty(zeAss::PropertyDefInt::Value& _AssetValue)
{
	const zeAss::PropertyDefInt& PropertyDef = (const zeAss::PropertyDefInt&)_AssetValue.mParentDef;
	PropertyMetaData* pMetaData				= zenNew(&sPoolMetaData)PropertyMetaData(&_AssetValue, wxVariant(_AssetValue.mValue));
	SetClientData		( pMetaData );
	SetDefaultValue		( wxVariant(PropertyDef.mDefault) );
	SetValue			( _AssetValue.mValue );
	SetHelpString		( wxString::Format("%s\n(Default %i) (Min %i) (Max %i)", PropertyDef.mzDescription, PropertyDef.mDefault, PropertyDef.miMin, PropertyDef.miMax));
	ConfigurePropertyScalar(*this, PropertyDef.miMin, PropertyDef.miMax, PropertyDef.miIncrement);
}

wxBetlIntProperty::~wxBetlIntProperty()
{
	zenDel(GetClientData());
}

//=================================================================================================
// PROPERTY :
//=================================================================================================
wxBetlEnumProperty::wxBetlEnumProperty(zeAss::PropertyDefEnum::Value& _AssetValue)
{
	const zeAss::PropertyDefEnum& PropertyDef	= (const zeAss::PropertyDefEnum&)_AssetValue.mParentDef;
	PropertyMetaData* pMetaData					= zenNew(&sPoolMetaData)PropertyMetaData(&_AssetValue, long(_AssetValue.mValue));
	const char* zDefaultDesc(NULL);
	SetClientData		( pMetaData );
	
	wxPGChoices soc;
	for(zUInt enumIdx(0), enumCount(PropertyDef.maEntry.Count()); enumIdx<enumCount; ++enumIdx)
	{
		soc.Add( PropertyDef.maEntry[enumIdx].zDescription, long(PropertyDef.maEntry[enumIdx].hKey) );
		if( PropertyDef.maEntry[enumIdx].hKey == PropertyDef.mDefault )
			zDefaultDesc = PropertyDef.maEntry[enumIdx].zDescription;
	}
	//soc[PropertyDef.muDefault].SetBgCol( wxColour(200,255,200,1) );
	//soc[PropertyDef.muDefault].SetBitmap(wxArtProvider::GetBitmap(wxART_INFORMATION));
	SetChoices			( soc );	
	SetDefaultValue		( wxVariant(long(PropertyDef.mDefault)) );
	SetValue			( long(_AssetValue.mValue) );
	SetHelpString		( wxString::Format("%s\n(Default '%s')", PropertyDef.mzDescription, zDefaultDesc));
}

wxBetlEnumProperty::~wxBetlEnumProperty()
{
	zenDel(GetClientData());
}



#endif

}
