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

struct PropertyMetaData : public zenSig::zSlot
{
								PropertyMetaData(wxPGProperty* _pOwner, const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rAssetValue, const wxVariant& _OriginalValue);
	virtual void				UpdateControl(){}
	virtual void				UpdateControlState();
	virtual bool				Save();
	virtual void				SetDefaultValue();
	virtual void				SetOriginalValue();
	void						slotUpdateProperty(zenAss::PropertyValueRef _rPropertyUpdated);
	wxPGProperty*				mpOwner;
	zenAss::zAssetItemRef		mrAsset;
	zenAss::PropertyValueRef	mrPropertyValue;
	wxVariant					mOriginalValue;	
};

wxPGProperty* CreateAssetValueControl(wxPropertyGridInterface& _GridControl, const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _Value);

class wxZenBoolProperty : public wxBoolProperty
{
public:
	struct TypedMetaData : public PropertyMetaData
	{
		TypedMetaData( wxZenBoolProperty* _pOwner, const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rAssetValue, const wxVariant& _OriginalValue)
		: PropertyMetaData(_pOwner, _rAsset, _rAssetValue, _OriginalValue){}
		virtual void UpdateControl();
		virtual bool Save();		
	};

	wxZenBoolProperty(const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rAssetValue);
	virtual ~wxZenBoolProperty();
};

class wxZenFloatProperty : public wxFloatProperty
{
public:
	struct TypedMetaData : public PropertyMetaData
	{
		TypedMetaData( wxZenFloatProperty* _pOwner, const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rAssetValue, const wxVariant& _OriginalValue)
		: PropertyMetaData(_pOwner, _rAsset, _rAssetValue, _OriginalValue){}
		virtual void UpdateControl();
		virtual bool Save();
	};

	wxZenFloatProperty(const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rAssetValue);
	virtual	~wxZenFloatProperty();
};

class wxZenIntProperty : public wxIntProperty
{
public:
	struct TypedMetaData : public PropertyMetaData
	{
		TypedMetaData( wxZenIntProperty* _pOwner, const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rAssetValue, const wxVariant& _OriginalValue)
		: PropertyMetaData(_pOwner, _rAsset, _rAssetValue, _OriginalValue){}
		virtual bool Save();
		virtual void UpdateControl();
	};

	wxZenIntProperty(const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rAssetValue);
	virtual ~wxZenIntProperty();
};

template< class TPropertyClass, class TElementCast, class TWxVector, class TWxProperty >
class wxZenVectorProperty : public wxPGProperty
{ 
public:						
	struct TypedMetaData : public PropertyMetaData
	{
		TypedMetaData( wxZenVectorProperty<TPropertyClass, TElementCast, TWxVector, TWxProperty>* _pOwner, const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rAssetValue, const wxVariant& _OriginalValue)
		: PropertyMetaData(_pOwner, _rAsset, _rAssetValue, _OriginalValue){}
		virtual void UpdateControl();
		virtual void UpdateControlState();
		virtual bool Save();		
	};
	wxZenVectorProperty(){}
	wxZenVectorProperty(const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rAssetValue, const char* _zTooltip, const char* _zTooltipElement);
	virtual	~wxZenVectorProperty();
	virtual wxVariant	ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const;
	virtual void		RefreshChildren();	
};

class wxZenFloat2fProperty : public wxZenVectorProperty<zenAss::PropertyFloat2, double, wxVector2f, wxFloatProperty>
{
WX_PG_DECLARE_PROPERTY_CLASS( wxZenFloat2fProperty )
public:
	wxZenFloat2fProperty(){ZENAssertMsg(0, "Shouldn't be reaching this constructor")}
	wxZenFloat2fProperty(const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rAssetValue): wxZenVectorProperty(_rAsset, _rAssetValue, "%s\n(Default [ %.3f, %.3f ])", "%s\n(Default %.3f)"){};
};

class wxZenFloat3fProperty : public wxZenVectorProperty<zenAss::PropertyFloat3, double, wxVector3f, wxFloatProperty>
{
WX_PG_DECLARE_PROPERTY_CLASS( wxZenFloat3fProperty )
public:
	wxZenFloat3fProperty(){ZENAssertMsg(0, "Shouldn't be reaching this constructor")}
	wxZenFloat3fProperty(const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rAssetValue):wxZenVectorProperty(_rAsset, _rAssetValue, "%s\n(Default [ %.3f, %.3f, %.3f ])", "%s\n(Default %.3f)"){};
};

class wxZenFloat4fProperty : public wxZenVectorProperty<zenAss::PropertyFloat4, double, wxVector4f, wxFloatProperty>
{
WX_PG_DECLARE_PROPERTY_CLASS( wxZenFloat4fProperty )
public:
	wxZenFloat4fProperty(){ZENAssertMsg(0, "Shouldn't be reaching this constructor")}
	wxZenFloat4fProperty(const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rAssetValue):wxZenVectorProperty(_rAsset, _rAssetValue, "%s\n(Default [ %.3f, %.3f, %.3f, %.3f ])", "%s\n(Default %.3f)"){};
};

class wxZenInt2fProperty : public wxZenVectorProperty<zenAss::PropertyInt2, long, wxVector2s, wxIntProperty>
{
WX_PG_DECLARE_PROPERTY_CLASS( wxZenInt2fProperty )
public:
	wxZenInt2fProperty(){ZENAssertMsg(0, "Shouldn't be reaching this constructor")}
	wxZenInt2fProperty(const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rAssetValue): wxZenVectorProperty(_rAsset, _rAssetValue, "%s\n(Default [ %3i, %3i ])", "%s\n(Default %3i)"){};
};

class wxZenInt3fProperty : public wxZenVectorProperty<zenAss::PropertyInt3, long, wxVector3s, wxIntProperty>
{
WX_PG_DECLARE_PROPERTY_CLASS( wxZenInt3fProperty )
public:
	wxZenInt3fProperty(){ZENAssertMsg(0, "Shouldn't be reaching this constructor")}
	wxZenInt3fProperty(const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rAssetValue):wxZenVectorProperty(_rAsset, _rAssetValue, "%s\n(Default [ %3i, %3i, %3i ])", "%s\n(Default %3i)"){};
};

class wxZenInt4fProperty : public wxZenVectorProperty<zenAss::PropertyInt4, long, wxVector4s, wxIntProperty>
{
WX_PG_DECLARE_PROPERTY_CLASS( wxZenInt4fProperty )
public:
	wxZenInt4fProperty(){ZENAssertMsg(0, "Shouldn't be reaching this constructor")}
	wxZenInt4fProperty(const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rAssetValue):wxZenVectorProperty(_rAsset, _rAssetValue, "%s\n(Default [ %3i, %3i, %3i, %3i ])", "%s\n(Default %3i)"){};
};

class wxZenEnumProperty : public wxEnumProperty
{
public:
	struct TypedMetaData : public PropertyMetaData
	{
		TypedMetaData( wxZenEnumProperty* _pOwner, const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rAssetValue, const wxVariant& _OriginalValue)
		: PropertyMetaData(_pOwner, _rAsset, _rAssetValue, _OriginalValue){}
		virtual void UpdateControl();
		virtual bool Save();
	};
	wxZenEnumProperty(const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rAssetValue);
	virtual	~wxZenEnumProperty();
};

class wxZenFileProperty : public wxFileProperty
{
public:
	struct TypedMetaData : public PropertyMetaData
	{
		TypedMetaData( wxZenFileProperty* _pOwner, const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rAssetValue, const wxVariant& _OriginalValue)
		: PropertyMetaData(_pOwner, _rAsset, _rAssetValue, _OriginalValue){}
		virtual void UpdateControl();
		virtual bool Save();
	};

	wxZenFileProperty(const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rAssetValue);
	virtual ~wxZenFileProperty();
};

class wxZenAssetProperty : public wxStringProperty
{
public:
	struct TypedMetaData : public PropertyMetaData
	{
		TypedMetaData( wxZenAssetProperty* _pOwner, const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rAssetValue);
		virtual void UpdateControl();
		virtual void UpdateControlState();
		virtual void SetDefaultValue();
		virtual void SetOriginalValue();
		virtual bool Save();
		void AssignValue(zenAss::zAssetID _AssetID);
		zenAss::zAssetID mAssetIDValue;
		zenAss::zAssetID mAssetIDOriginalValue;
	};

					wxZenAssetProperty(const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rAssetValue);
	virtual			~wxZenAssetProperty();
	virtual bool	OnEvent(wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event);
protected:
	//void			SetAssetValue(zenAss::zAssetID _AssetID);
};

class wxZenArrayProperty : public wxPGProperty //wxPropertyCategory?
{ 
public:	
	struct TypedMetaData : public PropertyMetaData
	{
		TypedMetaData( wxZenArrayProperty* _pOwner, const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rAssetValue, const wxVariant& _OriginalValue)
		: PropertyMetaData(_pOwner, _rAsset, _rAssetValue, _OriginalValue){}
		virtual void UpdateControl();
		virtual void UpdateControlState();
		virtual bool Save();		
	};
	wxZenArrayProperty(const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rAssetValue);
	virtual	~wxZenArrayProperty();
};

class wxZenStructProperty : public wxPGProperty
{ 
public:
	struct TypedMetaData : public PropertyMetaData
	{
		TypedMetaData( wxZenStructProperty* _pOwner, const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rAssetValue, const wxVariant& _OriginalValue)
		: PropertyMetaData(_pOwner, _rAsset, _rAssetValue, _OriginalValue){}
		virtual void UpdateControl();
		virtual bool Save();
		virtual void UpdateControlState();
	};
	wxZenStructProperty(const zenAss::zAssetItemRef& _rAsset, const zenAss::PropertyValueRef& _rAssetValue);
	virtual	~wxZenStructProperty();
};

}

#endif
