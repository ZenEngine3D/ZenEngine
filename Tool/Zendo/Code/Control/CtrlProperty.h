#pragma once
#ifndef __BetlCtrlAssetProperty_h__
#define __BetlCtrlAssetProperty_h__ 

template<class TType>
class wxVector2
{
public:
	wxVector2():x(0),y(0) {}
	wxVector2( TType _val):x(_val),y(_val){}
	wxVector2( TType _x, TType _y ):x(_x), y(_y) {}
	bool operator==(const wxVector2& _Compare)
	{
		return values[0]==_Compare.values[0] && values[1]==_Compare.values[1]; 
	}
	union{	TType values[2];
			struct {TType x, y;}; };
};

template<class TType>
class wxVector3
{
public:
	wxVector3():x(0),y(0),z(0) {}
	wxVector3( TType _val):x(_val),y(_val),z(_val){}
	wxVector3( TType _x, TType _y, TType _z ):x(_x),y(_y),z(_z){}
	bool operator==(const wxVector3& _Compare){return values[0]==_Compare.values[0] && values[1]==_Compare.values[1] && values[2]==_Compare.values[2]; }
	union{	TType values[3];
			struct {TType x, y, z;}; };
};

template<class TType>
class wxVector4
{
public:
	wxVector4():x(0),y(0),z(0),w(0) {}
	wxVector4( TType _val):x(_val),y(_val),z(_val),w(_val){}
	wxVector4( TType _x, TType _y, TType _z, TType _w ):x(_x),y(_y),z(_z),w(_w){}
	bool operator==(const wxVector4& _Compare){return values[0]==_Compare.values[0] && values[1]==_Compare.values[1] && values[2]==_Compare.values[2] && values[3]==_Compare.values[3]; }
	union{	TType values[4];
			struct {TType x, y, z, w;}; };
};


typedef wxVector2<int> wxVector2s;
typedef wxVector3<int> wxVector3s;
typedef wxVector4<int> wxVector4s;
typedef wxVector2<double> wxVector2f;
typedef wxVector3<double> wxVector3f;
typedef wxVector4<double> wxVector4f;
WX_PG_DECLARE_VARIANT_DATA(wxVector2s)
WX_PG_DECLARE_VARIANT_DATA(wxVector3s)
WX_PG_DECLARE_VARIANT_DATA(wxVector4s)
WX_PG_DECLARE_VARIANT_DATA(wxVector2f)
WX_PG_DECLARE_VARIANT_DATA(wxVector3f)
WX_PG_DECLARE_VARIANT_DATA(wxVector4f)

namespace BCtrl
{

struct PropertyMetaData
{
	PropertyMetaData(const zenAss::PropertyValue& _AssetValue, const wxVariant& _OriginalValue)
	: mAssetValue(_AssetValue)
	, mOriginalValue(_OriginalValue)
	{}
	zenAss::PropertyValue	mAssetValue;
	wxVariant				mOriginalValue;
};

wxPGProperty* CreateAssetValueControl(wxPropertyGridInterface& _GridControl, zenAss::PropertyValue& _Value);

class wxBetlBoolProperty : public wxBoolProperty
{
public:
						wxBetlBoolProperty(zenAss::PropertyValue& _AssetValue);
	virtual				~wxBetlBoolProperty();
};

class wxBetlFileProperty : public wxFileProperty
{
public:
						wxBetlFileProperty(zenAss::PropertyValue& _AssetValue);
	virtual				~wxBetlFileProperty();
};

class wxBetlFloatProperty : public wxFloatProperty
{
public:
						wxBetlFloatProperty(zenAss::PropertyValue& _AssetValue);
	virtual				~wxBetlFloatProperty();
};

class wxBetlFloat2fProperty : public wxPGProperty
{
WX_PG_DECLARE_PROPERTY_CLASS(wxBetlFloat2fProperty)
public:
						wxBetlFloat2fProperty(){ZENAssertMsg(0, "Shouldn't be reaching this constructor")}
						wxBetlFloat2fProperty(zenAss::PropertyValue& _AssetValue);
	virtual				~wxBetlFloat2fProperty();
	virtual wxVariant	ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const;
	virtual void		RefreshChildren();
};


#if 0
class wxBetlIntProperty : public wxIntProperty
{
public:
						wxBetlIntProperty(zeAss::PropertyDefInt::Value& _AssetValue);
	virtual				~wxBetlIntProperty();
};

class wxBetlEnumProperty : public wxEnumProperty
{
public:
						wxBetlEnumProperty(zeAss::PropertyDefEnum::Value& _AssetValue);
	virtual				~wxBetlEnumProperty();
};

#endif

}

#endif