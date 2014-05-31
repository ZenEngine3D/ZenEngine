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
	PropertyMetaData(wxPGProperty* _pOwner, const zenAss::PropertyValueRef& _rAssetValue, const wxVariant& _OriginalValue)
	: mpOwner(_pOwner)
	, mrAssetValue(_rAssetValue)
	, mOriginalValue(_OriginalValue)
	{}
	virtual void SetControlState();
	virtual bool Save(){return false;}

	wxPGProperty*				mpOwner;
	zenAss::PropertyValueRef	mrAssetValue;
	wxVariant					mOriginalValue;	
};

wxPGProperty* CreateAssetValueControl(wxPropertyGridInterface& _GridControl, zenAss::PropertyValueRef& _Value);

class wxBetlBoolProperty : public wxBoolProperty
{
public:
	struct TypedMetaData : public PropertyMetaData
	{
		TypedMetaData( wxBetlBoolProperty* _pOwner, const zenAss::PropertyValueRef& _rAssetValue, const wxVariant& _OriginalValue)
		: PropertyMetaData(_pOwner, _rAssetValue, _OriginalValue){}
		virtual bool Save();
	};

	wxBetlBoolProperty(zenAss::PropertyValueRef& _rAssetValue);
	virtual ~wxBetlBoolProperty();
};

class wxBetlFloatProperty : public wxFloatProperty
{
public:
	struct TypedMetaData : public PropertyMetaData
	{
		TypedMetaData( wxBetlFloatProperty* _pOwner, const zenAss::PropertyValueRef& _rAssetValue, const wxVariant& _OriginalValue)
		: PropertyMetaData(_pOwner, _rAssetValue, _OriginalValue){}
		virtual bool Save();
	};

	wxBetlFloatProperty(const zenAss::PropertyValueRef& _rAssetValue);
	virtual	~wxBetlFloatProperty();
};

class wxBetlIntProperty : public wxIntProperty
{
public:
	struct TypedMetaData : public PropertyMetaData
	{
		TypedMetaData( wxBetlIntProperty* _pOwner, const zenAss::PropertyValueRef& _rAssetValue, const wxVariant& _OriginalValue)
		: PropertyMetaData(_pOwner, _rAssetValue, _OriginalValue){}
		virtual bool Save();
	};

	wxBetlIntProperty(const zenAss::PropertyValueRef& _rAssetValue);
	virtual ~wxBetlIntProperty();
};

template< class TPropertyClass, class TElementCast, class TWxVector, class TWxProperty >
class wxBetlVectorProperty : public wxPGProperty
{ 
public:						
	struct TypedMetaData : public PropertyMetaData
	{
		TypedMetaData( wxBetlVectorProperty<TPropertyClass, TElementCast, TWxVector, TWxProperty>* _pOwner, const zenAss::PropertyValueRef& _rAssetValue, const wxVariant& _OriginalValue)
		: PropertyMetaData(_pOwner, _rAssetValue, _OriginalValue){}
		virtual bool Save();
		virtual void SetControlState();
	};
	wxBetlVectorProperty(){}
	wxBetlVectorProperty(const zenAss::PropertyValueRef& _rAssetValue, const char* _zTooltip, const char* _zTooltipElement);
	virtual	~wxBetlVectorProperty();
	virtual wxVariant	ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const;
	virtual void		RefreshChildren();
};

class wxBetlFloat2fProperty : public wxBetlVectorProperty<zenAss::PropertyFloat2, double, wxVector2f, wxFloatProperty>
{
WX_PG_DECLARE_PROPERTY_CLASS( wxBetlFloat2fProperty )
public:
	wxBetlFloat2fProperty(){ZENAssertMsg(0, "Shouldn't be reaching this constructor")}
	wxBetlFloat2fProperty(const zenAss::PropertyValueRef& _rAssetValue): wxBetlVectorProperty(_rAssetValue, "%s\n(Default [ %.3f, %.3f ])", "%s\n(Default %.3f)"){};
};

class wxBetlFloat3fProperty : public wxBetlVectorProperty<zenAss::PropertyFloat3, double, wxVector3f, wxFloatProperty>
{
WX_PG_DECLARE_PROPERTY_CLASS( wxBetlFloat3fProperty )
public:
	wxBetlFloat3fProperty(){ZENAssertMsg(0, "Shouldn't be reaching this constructor")}
	wxBetlFloat3fProperty(const zenAss::PropertyValueRef& _rAssetValue):wxBetlVectorProperty(_rAssetValue, "%s\n(Default [ %.3f, %.3f, %.3f ])", "%s\n(Default %.3f)"){};
};

class wxBetlFloat4fProperty : public wxBetlVectorProperty<zenAss::PropertyFloat4, double, wxVector4f, wxFloatProperty>
{
WX_PG_DECLARE_PROPERTY_CLASS( wxBetlFloat4fProperty )
public:
	wxBetlFloat4fProperty(){ZENAssertMsg(0, "Shouldn't be reaching this constructor")}
	wxBetlFloat4fProperty(const zenAss::PropertyValueRef& _rAssetValue):wxBetlVectorProperty(_rAssetValue, "%s\n(Default [ %.3f, %.3f, %.3f, %.3f ])", "%s\n(Default %.3f)"){};
};

class wxBetlInt2fProperty : public wxBetlVectorProperty<zenAss::PropertyInt2, long, wxVector2s, wxIntProperty>
{
WX_PG_DECLARE_PROPERTY_CLASS( wxBetlInt2fProperty )
public:
	wxBetlInt2fProperty(){ZENAssertMsg(0, "Shouldn't be reaching this constructor")}
	wxBetlInt2fProperty(const zenAss::PropertyValueRef& _rAssetValue): wxBetlVectorProperty(_rAssetValue, "%s\n(Default [ %3i, %3i ])", "%s\n(Default %3i)"){};
};

class wxBetlInt3fProperty : public wxBetlVectorProperty<zenAss::PropertyInt3, long, wxVector3s, wxIntProperty>
{
WX_PG_DECLARE_PROPERTY_CLASS( wxBetlInt3fProperty )
public:
	wxBetlInt3fProperty(){ZENAssertMsg(0, "Shouldn't be reaching this constructor")}
	wxBetlInt3fProperty(const zenAss::PropertyValueRef& _rAssetValue):wxBetlVectorProperty(_rAssetValue, "%s\n(Default [ %3i, %3i, %3i ])", "%s\n(Default %3i)"){};
};

class wxBetlInt4fProperty : public wxBetlVectorProperty<zenAss::PropertyInt4, long, wxVector4s, wxIntProperty>
{
WX_PG_DECLARE_PROPERTY_CLASS( wxBetlInt4fProperty )
public:
	wxBetlInt4fProperty(){ZENAssertMsg(0, "Shouldn't be reaching this constructor")}
	wxBetlInt4fProperty(const zenAss::PropertyValueRef& _rAssetValue):wxBetlVectorProperty(_rAssetValue, "%s\n(Default [ %3i, %3i, %3i, %3i ])", "%s\n(Default %3i)"){};
};

class wxBetlEnumProperty : public wxEnumProperty
{
public:
	struct TypedMetaData : public PropertyMetaData
	{
		TypedMetaData( wxBetlEnumProperty* _pOwner, const zenAss::PropertyValueRef& _rAssetValue, const wxVariant& _OriginalValue)
		: PropertyMetaData(_pOwner, _rAssetValue, _OriginalValue){}
		virtual bool Save();
	};
	wxBetlEnumProperty(const zenAss::PropertyValueRef& _rAssetValue);
	virtual	~wxBetlEnumProperty();
};

class wxBetlFileProperty : public wxFileProperty
{
public:
	struct TypedMetaData : public PropertyMetaData
	{
		TypedMetaData( wxBetlFileProperty* _pOwner, const zenAss::PropertyValueRef& _rAssetValue, const wxVariant& _OriginalValue)
		: PropertyMetaData(_pOwner, _rAssetValue, _OriginalValue){}
		virtual bool Save();
	};

	wxBetlFileProperty(const zenAss::PropertyValueRef& _rAssetValue);
	virtual ~wxBetlFileProperty();
};

class wxBetlArrayProperty : public wxPGProperty
{ 
public:	
	struct TypedMetaData : public PropertyMetaData
	{
		TypedMetaData( wxBetlArrayProperty* _pOwner, const zenAss::PropertyValueRef& _rAssetValue, const wxVariant& _OriginalValue)
		: PropertyMetaData(_pOwner, _rAssetValue, _OriginalValue){}
		virtual bool Save();
		virtual void SetControlState();
	};
	wxBetlArrayProperty(const zenAss::PropertyValueRef& _rAssetValue);
	virtual	~wxBetlArrayProperty();
};

class wxBetlStructProperty : public wxPGProperty
{ 
public:
	struct TypedMetaData : public PropertyMetaData
	{
		TypedMetaData( wxBetlStructProperty* _pOwner, const zenAss::PropertyValueRef& _rAssetValue, const wxVariant& _OriginalValue)
		: PropertyMetaData(_pOwner, _rAssetValue, _OriginalValue){}
		virtual bool Save();
		virtual void SetControlState();
	};
	wxBetlStructProperty(const zenAss::PropertyValueRef& _rAssetValue);
	virtual	~wxBetlStructProperty();
};

}

#endif
