namespace zen { namespace zenRes {	

//#################################################################################################
// RESOURCE
//#################################################################################################

//=================================================================================================
//! @details	Prevents being able to instantiate a resource outside of children class's Create()
//=================================================================================================
zResource::zResource()
{
}

const zResID& zResource::GetResID()const
{ 
	return mResID; 
}

bool zResource::IsValid()const		
{ 
	return mResID.meType<zenConst::keResType__Invalid; 
}

//#################################################################################################
// RESOURCE REF
//#################################################################################################
zResourceRef::zResourceRef()
: mpResource(NULL)
{ 
	ZENDbgCode( mSupportedTypeMask.Invert(); )	//Set default value to support all resource type
}

zResourceRef::zResourceRef(zResource* _pResource)
: mpResource(NULL)
{ 
	ZENDbgCode( mSupportedTypeMask.Invert(); );	//Set default value to support all resource type
	SetResource(_pResource); 
}

void zResourceRef::SetResource(zResource* _pResource)
{
	if( mpResource )
		mpResource->ReferenceRem();
	ZENAssert(!_pResource || mSupportedTypeMask.Any(_pResource->GetResID().Type()) );
	mpResource = _pResource;
	if( mpResource )
		mpResource->ReferenceAdd();
}	

zResourceRef::zResourceRef(zFlagResType _SupportedTypes)
: mpResource(NULL)
{
	ZENDbgCode( mSupportedTypeMask = _SupportedTypes; )
}

zResourceRef::zResourceRef(zFlagResType _SupportedTypes, zResource* _pResource)
: mpResource(NULL)
{
	ZENDbgCode( mSupportedTypeMask = _SupportedTypes; )
	SetResource( _pResource );
}

const zResourceRef& zResourceRef::operator=(zResource* _pResource)
{
	SetResource( _pResource );
	return *this;
}

const zResourceRef& zResourceRef::operator=(const zResourceRef& _ResourceRef)
{
	SetResource( _ResourceRef.mpResource );
	return *this;
}

bool zResourceRef::IsValid()const		
{ 
	return mpResource!=NULL; 
}

zResID zResourceRef::GetResID()const		
{ 
	return mpResource ? mpResource->GetResID() : zResID(); 
}

bool zResourceRef::operator==(const zResourceRef& _rCmp)const	
{
	return mpResource == _rCmp.mpResource;
}

bool zResourceRef::operator!=(const zResourceRef& _rCmp)const	
{
	return mpResource != _rCmp.mpResource;
}

zResourceRef::operator zResID()const
{
	return mpResource ? mpResource->GetResID() : zResID(); 
}

//#################################################################################################
// RESOURCE TYPED REF
//#################################################################################################
template<zenConst::eResType TType>
zResourceTypedRef<TType>::zResourceTypedRef()								
: zResourceRef(zFlagResType(TType))
{}

template<zenConst::eResType TType>
zResourceTypedRef<TType>::zResourceTypedRef(zResource* _pResource)	
: zResourceRef(zFlagResType(TType), _pResource)
{}

template<zenConst::eResType TType>
zResourceTypedRef<TType>::zResourceTypedRef(zResID _ResourceID)		
: zResourceRef(zFlagResType(TType), _ResourceID)
{}

//#################################################################################################
// RESOURCE TYPED2 REF
//#################################################################################################
template<zenConst::eResType TType1, zenConst::eResType TType2>
zResourceTyped2Ref<TType1,TType2>::zResourceTyped2Ref()							
: zResourceRef(zFlagResType(TType1,TType2))
{}

template<zenConst::eResType TType1, zenConst::eResType TType2>
zResourceTyped2Ref<TType1,TType2>::zResourceTyped2Ref(zResource* _pResource)	
: zResourceRef(zFlagResType(TType1,TType2), _pResource)
{}

template<zenConst::eResType TType1, zenConst::eResType TType2>
zResourceTyped2Ref<TType1,TType2>::zResourceTyped2Ref(zResID _ResourceID)	
: zResourceRef(zFlagResType(TType1,TType2), _ResourceID)
{}

}} // namespace zen, zenRes

//#################################################################################################
// Used for declaration of zResourceTypedRef classes
//#################################################################################################
#define ZENClassResourceRefDeclare(_ClassName_, _Type_)								\
class _ClassName_ : public zenRes::zResourceTypedRef<_Type_>						\
{																					\
ZENClassDeclare(_ClassName_, zenRes::zResourceTypedRef<_Type_>)						\
public:																				\
	ZENInline	_ClassName_(){}														\
	ZENInline	_ClassName_(zenRes::zResource* _pResource):Super(_pResource){}		\
	ZENInline	_ClassName_(zResID _ResourceID):Super(_ResourceID){}

//#################################################################################################
// Used for declaration of zResourceTyped2Ref classes
//#################################################################################################
#define ZENClassResourceRef2Declare(_ClassName_, _Type1_, _Type2_)					\
class _ClassName_ : public zenRes::zResourceTyped2Ref<_Type1_ , _Type2_>			\
{																					\
ZENClassDeclare(_ClassName_, zenRes::zResourceTyped2Ref<_Type1_ ZENComma _Type2_> )	\
public:																				\
	ZENInline	_ClassName_(){}														\
	ZENInline	_ClassName_(zenRes::zResource* _pResource):Super(_pResource){}		\
	ZENInline	_ClassName_(zResID _ResourceID):Super(_ResourceID){}
