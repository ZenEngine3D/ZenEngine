//=================================================================================================
// Defines used when declaring a new resource class (engine side)
//=================================================================================================
#define ZENResourceDeclare(_Class_, _ClassInstance_, _ResourceType_)																	\
	ZENResourceDeclareParent(_Class_, Resource, _ClassInstance_, _ResourceType_)

#define ZENResourceDeclareParent(_Class_, _ClassParent_, _ClassInstance_, _ResourceType_)											\
	ZENClassDeclare(_Class_, _ClassParent_)																							\
public:																																\
	ZENInline const _ClassInstance_&	Get()const								{	return mInstanceInfo;};							\
	ZENInline _ClassInstance_&	Get()											{	return mInstanceInfo;};							\
	static _Class_*					Create(EExp::ExportInfoBase& _ExportInfo)	{	_Class_* pNewRes = zenNewDefault _Class_();		\
																					return pNewRes->ResourceCreate(pNewRes->mInstanceInfo, _ExportInfo) ? pNewRes : NULL; }\
	static zenConst::eResType		GetResourceType()							{	return _ResourceType_; }						\
protected:																															\
	_ClassInstance_					mInstanceInfo;																					\
private:																															\
	_Class_();	

namespace ERes
{	

template<class TResourceClass, class TReferenceClass>
ResourceRef<TResourceClass, TReferenceClass>::ResourceRef()
{
}

template<class TResourceClass, class TReferenceClass>
ResourceRef<TResourceClass, TReferenceClass>::ResourceRef(zenRes::zResource* _pResource)
: Super(_pResource)
{
}

template<class TResourceClass, class TReferenceClass>
ResourceRef<TResourceClass, TReferenceClass>::ResourceRef(zResID _ResourceID)
: Super(_ResourceID)
{
}

template<class TResourceClass, class TReferenceClass>
ResourceRef<TResourceClass, TReferenceClass>::ResourceRef(const TReferenceClass& _Copy)
: Super(_Copy)
{
}

template<class TResourceClass, class TReferenceClass>
TResourceClass* ResourceRef<TResourceClass, TReferenceClass>::operator->()
{ 
	ZENAssert(mpResource); 
	return static_cast<TResourceClass*>(mpResource); 
}		

template<class TResourceClass, class TReferenceClass>
const TResourceClass* ResourceRef<TResourceClass, TReferenceClass>::operator->()const	
{ 
	ZENAssert(mpResource); 
	return static_cast<const TResourceClass*>(mpResource); 
}

}
