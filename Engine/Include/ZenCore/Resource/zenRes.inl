namespace zen { namespace zenRes {	

//#################################################################################################
// RESOURCE
//#################################################################################################

//=================================================================================================
//! @details	Prevents being able to instantiate a resource outside of children class's Create()
//=================================================================================================
awResource::awResource()
{
}

const zenResID& awResource::GetResID()const
{ 
	return mResID; 
}

bool awResource::IsValid()const		
{ 
	return mResID.meType<zenConst::keResType__Invalid; 
}

//#################################################################################################
// RESOURCE REF
//#################################################################################################
awResourceRef::awResourceRef()
: mpResource(NULL)
{ 
	ZENDbgCode( mSupportedTypeMask.Invert(); )	//Set default value to support all resource type
}

awResourceRef::awResourceRef(awResource* _pResource)
: mpResource(NULL)
{ 
	ZENDbgCode( mSupportedTypeMask.Invert(); );	//Set default value to support all resource type
	SetResource(_pResource); 
}

void awResourceRef::SetResource(awResource* _pResource)
{
	if( mpResource )
		mpResource->ReferenceRem();
	ZENAssert(!_pResource || mSupportedTypeMask.Any(_pResource->GetResID().Type()) );
	mpResource = _pResource;
	if( mpResource )
		mpResource->ReferenceAdd();
}	

awResourceRef::awResourceRef(zenFlagResType _SupportedTypes)
: mpResource(NULL)
{
	ZENDbgCode( mSupportedTypeMask = _SupportedTypes; )
}

awResourceRef::awResourceRef(zenFlagResType _SupportedTypes, awResource* _pResource)
: mpResource(NULL)
{
	ZENDbgCode( mSupportedTypeMask = _SupportedTypes; )
	SetResource( _pResource );
}

const awResourceRef& awResourceRef::operator=(awResource* _pResource)
{
	SetResource( _pResource );
	return *this;
}

const awResourceRef& awResourceRef::operator=(const awResourceRef& _ResourceRef)
{
	SetResource( _ResourceRef.mpResource );
	return *this;
}

bool awResourceRef::IsValid()const		
{ 
	return mpResource!=NULL; 
}

zenResID awResourceRef::GetResID()const		
{ 
	return mpResource ? mpResource->GetResID() : zenResID(); 
}

bool awResourceRef::operator==(const awResourceRef& _rCmp)const	
{
	return mpResource == _rCmp.mpResource;
}

bool awResourceRef::operator!=(const awResourceRef& _rCmp)const	
{
	return mpResource != _rCmp.mpResource;
}

awResourceRef::operator zenResID()const
{
	return mpResource ? mpResource->GetResID() : zenResID(); 
}

//#################################################################################################
// RESOURCE TYPED REF
//#################################################################################################
template<zenConst::eResType TType>
awResourceTypedRef<TType>::awResourceTypedRef()								
: awResourceRef(zenFlagResType(TType))
{}

template<zenConst::eResType TType>
awResourceTypedRef<TType>::awResourceTypedRef(awResource* _pResource)	
: awResourceRef(zenFlagResType(TType), _pResource)
{}

template<zenConst::eResType TType>
awResourceTypedRef<TType>::awResourceTypedRef(zenResID _ResourceID)		
: awResourceRef(zenFlagResType(TType), _ResourceID)
{}

//#################################################################################################
// RESOURCE TYPED2 REF
//#################################################################################################
template<zenConst::eResType TType1, zenConst::eResType TType2>
awResourceTyped2Ref<TType1,TType2>::awResourceTyped2Ref()							
: awResourceRef(zenFlagResType(TType1,TType2))
{}

template<zenConst::eResType TType1, zenConst::eResType TType2>
awResourceTyped2Ref<TType1,TType2>::awResourceTyped2Ref(awResource* _pResource)	
: awResourceRef(zenFlagResType(TType1,TType2), _pResource)
{}

template<zenConst::eResType TType1, zenConst::eResType TType2>
awResourceTyped2Ref<TType1,TType2>::awResourceTyped2Ref(zenResID _ResourceID)	
: awResourceRef(zenFlagResType(TType1,TType2), _ResourceID)
{}

}} // namespace zen, zenRes

//#################################################################################################
// Used for declaration of awResourceTypedRef classes
//#################################################################################################
#define AWClassResourceRefDeclare(_ClassName_, _Type_)								\
class _ClassName_ : public zenRes::awResourceTypedRef<_Type_>						\
{																					\
ZENClassDeclare(_ClassName_, zenRes::awResourceTypedRef<_Type_>)						\
public:																				\
	ZENInline	_ClassName_(){}															\
	ZENInline	_ClassName_(zenRes::awResource* _pResource):Super(_pResource){}			\
	ZENInline	_ClassName_(zenResID _ResourceID):Super(_ResourceID){}

//#################################################################################################
// Used for declaration of awResourceTyped2Ref classes
//#################################################################################################
#define AWClassResourceRef2Declare(_ClassName_, _Type1_, _Type2_)					\
class _ClassName_ : public zenRes::awResourceTyped2Ref<_Type1_ , _Type2_>			\
{																					\
ZENClassDeclare(_ClassName_, zenRes::awResourceTyped2Ref<_Type1_ ZENComma _Type2_> )	\
public:																				\
	ZENInline	_ClassName_(){}															\
	ZENInline	_ClassName_(zenRes::awResource* _pResource):Super(_pResource){}			\
	ZENInline	_ClassName_(zenResID _ResourceID):Super(_ResourceID){}
