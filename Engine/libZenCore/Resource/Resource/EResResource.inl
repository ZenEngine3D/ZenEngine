//=================================================================================================
// Defines used when declaring a new resource class (engine side)
//=================================================================================================
#define AWResourceDeclare(_Class_, _ClassInstance_, _ResourceType_)																	\
	AWResourceDeclareParent(_Class_, Resource, _ClassInstance_, _ResourceType_)

#define AWResourceDeclareParent(_Class_, _ClassParent_, _ClassInstance_, _ResourceType_)											\
	AWClassDeclare(_Class_, _ClassParent_)																							\
public:																																\
	inline const _ClassInstance_&	Get()const									{	return mInstanceInfo;};							\
	inline _ClassInstance_&	Get()												{	return mInstanceInfo;};							\
	static _Class_*					Create(EExp::ExportInfoBase& _ExportInfo)	{	_Class_* pNewRes = zenNewDefault _Class_();		\
																					return pNewRes->ResourceCreate(pNewRes->mInstanceInfo, _ExportInfo) ? pNewRes : NULL; }\
	static awconst::eResType		GetResourceType()							{	return _ResourceType_; }						\
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
ResourceRef<TResourceClass, TReferenceClass>::ResourceRef(awres::awResource* _pResource)
: Super(_pResource)
{
}

template<class TResourceClass, class TReferenceClass>
ResourceRef<TResourceClass, TReferenceClass>::ResourceRef(zenResID _ResourceID)
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
	AWAssert(mpResource); 
	return static_cast<TResourceClass*>(mpResource); 
}		

template<class TResourceClass, class TReferenceClass>
const TResourceClass* ResourceRef<TResourceClass, TReferenceClass>::operator->()const	
{ 
	AWAssert(mpResource); 
	return static_cast<const TResourceClass*>(mpResource); 
}

}
