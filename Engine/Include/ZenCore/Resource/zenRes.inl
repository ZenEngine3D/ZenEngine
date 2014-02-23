namespace zen { namespace awres {	

//#################################################################################################
// RESOURCE
//#################################################################################################

//=================================================================================================
//! @details	Prevents being able to instantiate a resource outside of children class's Create()
//=================================================================================================
awResource::awResource()
{
}

const awResourceID& awResource::GetResID()const
{ 
	return mResID; 
}

bool awResource::IsValid()const		
{ 
	return mResID.meType<awconst::keResType__Invalid; 
}

//#################################################################################################
// RESOURCE REF
//#################################################################################################
awResourceRef::awResourceRef()
: mpResource(NULL)
{ 
	AWDbgCode( mSupportedTypeMask.Invert(); )	//Set default value to support all resource type
}

awResourceRef::awResourceRef(awResource* _pResource)
: mpResource(NULL)
{ 
	AWDbgCode( mSupportedTypeMask.Invert(); );	//Set default value to support all resource type
	SetResource(_pResource); 
}

void awResourceRef::SetResource(awResource* _pResource)
{
	if( mpResource )
		mpResource->ReferenceRem();
	AWAssert(!_pResource || mSupportedTypeMask.Any(_pResource->GetResID().Type()) );
	mpResource = _pResource;
	if( mpResource )
		mpResource->ReferenceAdd();
}	

awResourceRef::awResourceRef(awFlagResType _SupportedTypes)
: mpResource(NULL)
{
	AWDbgCode( mSupportedTypeMask = _SupportedTypes; )
}

awResourceRef::awResourceRef(awFlagResType _SupportedTypes, awResource* _pResource)
: mpResource(NULL)
{
	AWDbgCode( mSupportedTypeMask = _SupportedTypes; )
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

awResourceID awResourceRef::GetResID()const		
{ 
	return mpResource ? mpResource->GetResID() : awResourceID(); 
}

bool awResourceRef::operator==(const awResourceRef& _rCmp)const	
{
	return mpResource == _rCmp.mpResource;
}

bool awResourceRef::operator!=(const awResourceRef& _rCmp)const	
{
	return mpResource != _rCmp.mpResource;
}

awResourceRef::operator awResourceID()const
{
	return mpResource ? mpResource->GetResID() : awResourceID(); 
}

//#################################################################################################
// RESOURCE TYPED REF
//#################################################################################################
template<awconst::eResType TType>
awResourceTypedRef<TType>::awResourceTypedRef()								
: awResourceRef(awFlagResType(TType))
{}

template<awconst::eResType TType>
awResourceTypedRef<TType>::awResourceTypedRef(awResource* _pResource)	
: awResourceRef(awFlagResType(TType), _pResource)
{}

template<awconst::eResType TType>
awResourceTypedRef<TType>::awResourceTypedRef(awResourceID _ResourceID)		
: awResourceRef(awFlagResType(TType), _ResourceID)
{}

//#################################################################################################
// RESOURCE TYPED2 REF
//#################################################################################################
template<awconst::eResType TType1, awconst::eResType TType2>
awResourceTyped2Ref<TType1,TType2>::awResourceTyped2Ref()							
: awResourceRef(awFlagResType(TType1,TType2))
{}

template<awconst::eResType TType1, awconst::eResType TType2>
awResourceTyped2Ref<TType1,TType2>::awResourceTyped2Ref(awResource* _pResource)	
: awResourceRef(awFlagResType(TType1,TType2), _pResource)
{}

template<awconst::eResType TType1, awconst::eResType TType2>
awResourceTyped2Ref<TType1,TType2>::awResourceTyped2Ref(awResourceID _ResourceID)	
: awResourceRef(awFlagResType(TType1,TType2), _ResourceID)
{}

}} // namespace zen, awres

//#################################################################################################
// Used for declaration of awResourceTypedRef classes
//#################################################################################################
#define AWClassResourceRefDeclare(_ClassName_, _Type_)								\
class _ClassName_ : public awres::awResourceTypedRef<_Type_>						\
{																					\
AWClassDeclare(_ClassName_, awres::awResourceTypedRef<_Type_>)						\
public:																				\
	inline	_ClassName_(){}															\
	inline	_ClassName_(awres::awResource* _pResource):Super(_pResource){}			\
	inline	_ClassName_(awResourceID _ResourceID):Super(_ResourceID){}

//#################################################################################################
// Used for declaration of awResourceTyped2Ref classes
//#################################################################################################
#define AWClassResourceRef2Declare(_ClassName_, _Type1_, _Type2_)					\
class _ClassName_ : public awres::awResourceTyped2Ref<_Type1_ , _Type2_>			\
{																					\
AWClassDeclare(_ClassName_, awres::awResourceTyped2Ref<_Type1_ AWComma _Type2_> )	\
public:																				\
	inline	_ClassName_(){}															\
	inline	_ClassName_(awres::awResource* _pResource):Super(_pResource){}			\
	inline	_ClassName_(awResourceID _ResourceID):Super(_ResourceID){}
