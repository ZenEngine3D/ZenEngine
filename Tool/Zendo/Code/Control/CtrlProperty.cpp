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
static zenMem::AllocatorPool sPoolMetaData("Betl::PropertyMetaData Pool", sizeof(PropertyMetaData), 100, 100);

//=================================================================================================
//! @brief		Common setup applied to each property control
//! @details	Used to avoid code duplication
//-------------------------------------------------------------------------------------------------
//! @param _GridControl	- PropertyGrid control to add the new value edition
//! @param _Value		- Asset value to edit
//! @return				- Created property control
//=================================================================================================
wxPGProperty* CreateAssetValueControl(wxPropertyGridInterface& _GridControl, zeAss::ValuePointer& _Value)
{
	static const wxColour bgColors[]={wxColour(230,240,250,255),wxColour(240,245,240,255)};
	static zenUInt suLineIndex(0);
	wxPGProperty* pProperty(NULL);
	switch( _Value.GetType() )
	{
	case zenConst::keAssProp_Bool:	pProperty=zenNewDefault wxBetlBoolProperty(*_Value.GetBool());			break;
	case zenConst::keAssProp_File:	pProperty=zenNewDefault wxBetlFileProperty(*_Value.GetFile());			break;
#if 0
	case zenConst::keAssProp_Int:		pProperty=zenNewDefault wxBetlIntProperty(*_Value.GetInt());				break;
		// 		case AAss::zenConst::keAssProp_Int2:			break;
		// 		case AAss::zenConst::keAssProp_Int3:			break;
		// 		case AAss::zenConst::keAssProp_Int4:			break;
	case zenConst::keAssProp_Float:	pProperty=zenNewDefault wxBetlFloatProperty(*_Value.GetFloat());			break;
	case zenConst::keAssProp_Float2:	pProperty=zenNewDefault wxBetlVector2fProperty(*_Value.GetFloat2());		break;
		// 		case AAss::zenConst::keAssProp_Float3:			break;
		// 		case AAss::zenConst::keAssProp_Float4:			break;
	case zenConst::keAssProp_Enum:	pProperty=zenNewDefault wxBetlEnumProperty(*_Value.GetEnum());			break;
#endif
	default:									ZENAssertMsg(0, "Unknown property type, implement it")			break;
	}	
	
	if( pProperty )
	{		
		const zeAss::PropertyDefBase& PropertyDef = _Value.GetBase()->mParentDef;
		pProperty->SetName				( PropertyDef.mzName );
		pProperty->SetLabel				( PropertyDef.mzDisplayName );				
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
// PROPERTY :
//=================================================================================================
wxBetlBoolProperty::wxBetlBoolProperty(zeAss::PropertyDefBool::Value& _AssetValue)
{
	const zeAss::PropertyDefBool&	PropertyDef		= (const zeAss::PropertyDefBool&)_AssetValue.mParentDef;
	PropertyMetaData*				pMetaData		= zenNew(&sPoolMetaData)PropertyMetaData(&_AssetValue, _AssetValue.mValue);
	const wxString					zFalseTrue[2]	= {wxT("False"), wxT("True")};	
	SetClientData		( pMetaData );
	SetDefaultValue		( wxVariant(PropertyDef.mDefault) );
	SetValue			( _AssetValue.mValue );	
	SetHelpString		( wxString::Format("%s\n(Default %s)", PropertyDef.mzDescription, zFalseTrue[PropertyDef.mDefault]));
	SetAttribute		( wxPG_BOOL_USE_CHECKBOX, true );
}

wxBetlBoolProperty::~wxBetlBoolProperty()
{
	zenDel(GetClientData());
}

//=================================================================================================
// PROPERTY :
//=================================================================================================
wxBetlFileProperty::wxBetlFileProperty(zeAss::PropertyDefFile::Value& _AssetValue)
{
	wxFileName Value((const char*)_AssetValue.mValue);
	const zeAss::PropertyDefFile& PropertyDef	= (const zeAss::PropertyDefFile&)_AssetValue.mParentDef;
	PropertyMetaData* pMetaData					= zenNew(&sPoolMetaData)PropertyMetaData(&_AssetValue, Value.GetFullPath());
	SetClientData		( pMetaData );
	SetDefaultValue		( wxVariant((const char*)PropertyDef.mDefault) );
	SetValue			( Value.GetFullPath() );
	SetHelpString		( PropertyDef.mzDescription );
	SetAttribute		( wxPG_FILE_INITIAL_PATH,	Value.GetPath());
	SetAttribute		( wxPG_FILE_WILDCARD,		PropertyDef.mzFileExt);
	SetAttribute		( wxPG_FILE_DIALOG_TITLE,	wxString::Format("Open file for '%s'", PropertyDef.mzDisplayName));
	SetAttribute		( wxPG_FILE_SHOW_FULL_PATH,	true);
}

wxBetlFileProperty::~wxBetlFileProperty()
{
	zenDel(GetClientData());
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
wxBetlFloatProperty::wxBetlFloatProperty(zeAss::PropertyDefFloat::Value& _AssetValue)
{
	const zeAss::PropertyDefFloat& PropertyDef	= (const zeAss::PropertyDefFloat&)_AssetValue.mParentDef;
	PropertyMetaData* pMetaData					= zenNew(&sPoolMetaData)PropertyMetaData(&_AssetValue, wxVariant(_AssetValue.mValue));
	SetClientData		( pMetaData );
	SetDefaultValue		( wxVariant(PropertyDef.mDefault) );
	SetValue			( wxVariant(_AssetValue.mValue) );
	SetEditor			( wxPGEditor_SpinCtrl );
	SetHelpString		( wxString::Format("%s\n(Default %.3f) (Min %.3f) (Max %.3f)", PropertyDef.mzDescription, PropertyDef.mDefault, PropertyDef.mfMin, PropertyDef.mfMax));
	ConfigurePropertyScalar(*this, PropertyDef.mfMin, PropertyDef.mfMax, PropertyDef.mfIncrement);
}

wxBetlFloatProperty::~wxBetlFloatProperty()
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
	for(zenUInt enumIdx(0), enumCount(PropertyDef.maEntry.Count()); enumIdx<enumCount; ++enumIdx)
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

//=================================================================================================
// PROPERTY :
//=================================================================================================
WX_PG_IMPLEMENT_PROPERTY_CLASS(wxBetlVector2fProperty, wxPGProperty, wxVector2f, const wxVector2f&, TextCtrl)
wxBetlVector2fProperty::wxBetlVector2fProperty(zeAss::PropertyDefFloat2::Value& _AssetValue )
{	
	const zeAss::PropertyDefFloat2& PropertyDef	= (const zeAss::PropertyDefFloat2&)_AssetValue.mParentDef;
	wxVector2f vValue( _AssetValue.mValue.x, _AssetValue.mValue.y);	
	wxVector2f vDefault( PropertyDef.mDefault.x, PropertyDef.mDefault.y);
	PropertyMetaData* pMetaData					= zenNew(&sPoolMetaData)PropertyMetaData(&_AssetValue, WXVARIANT(vValue));
		
	for(zenUInt idx(0); idx<2; ++idx)
	{
		const wxString zNames[]={wxT("-->X"),wxT("-->Y")};
		wxFloatProperty* pProp				= zenNewDefault wxFloatProperty(zNames[idx],wxPG_LABEL);
		PropertyMetaData* pMetaDataChild	= zenNew(&sPoolMetaData)PropertyMetaData(&_AssetValue, _AssetValue.mValue.xy[idx]);
		pProp->SetClientData	( pMetaDataChild );
		pProp->SetDefaultValue	( wxVariant(PropertyDef.mDefault.xy[idx]) );
		pProp->SetValue			( wxVariant(_AssetValue.mValue.xy[idx]) );
		pProp->SetHelpString	( wxString::Format("%s\n(Default %.3f)", PropertyDef.mzDescription, PropertyDef.mDefault.xy[idx]));
		ConfigurePropertyScalar(*pProp, PropertyDef.mvfMin.xy[idx], PropertyDef.mvfMax.xy[idx], PropertyDef.mvfIncrement.xy[idx]);
		AddPrivateChild(pProp);
	}
	SetValue					( WXVARIANT(vValue) );
	SetDefaultValue				( WXVARIANT(vDefault) );
	SetHelpString				( wxString::Format("%s\n(Default [ %.3f, %.3f ])", PropertyDef.mzDescription, PropertyDef.mDefault.x, PropertyDef.mDefault.y));	
}

wxBetlVector2fProperty::~wxBetlVector2fProperty() 
{
	zenDel(GetClientData());
}

void wxBetlVector2fProperty::RefreshChildren()
{
 	if ( !GetChildCount() ) return;
 	const wxVector2f& vector = wxVector2fRefFromVariant(m_value);
 	Item(0)->SetValue( vector.x );
 	Item(1)->SetValue( vector.y );
}

wxVariant wxBetlVector2fProperty::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
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

#endif

}
