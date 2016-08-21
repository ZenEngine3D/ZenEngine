namespace zen { namespace zenRes {	

//############################################################################################

template<class TResourceClass, class TResourceHAL, zU64 TTypeMask>
zResourceConstRef<TResourceClass, TResourceHAL, TTypeMask>::zResourceConstRef()
{
}

template<class TResourceClass, class TResourceHAL, zU64 TTypeMask>
zResourceConstRef<TResourceClass, TResourceHAL, TTypeMask>::zResourceConstRef(const zResourceConstRef& _rResource)
{
	operator=(_rResource);
}

template<class TResourceClass, class TResourceHAL, zU64 TTypeMask>
template<class TCopyClass, class TCopyHAL, zU64 TCopyMask >
zResourceConstRef<TResourceClass, TResourceHAL, TTypeMask>::zResourceConstRef(const zResourceConstRef<TCopyClass, TCopyHAL, TCopyMask>& _rResource)
{
	operator=(_rResource);
}

template<class TResourceClass, class TResourceHAL, zU64 TTypeMask>
zResourceConstRef<TResourceClass, TResourceHAL, TTypeMask>::zResourceConstRef(TResourceClass* _pResource)
{
	operator=(_pResource);
}

template<class TResourceClass, class TResourceHAL, zU64 TTypeMask>
zResourceConstRef<TResourceClass, TResourceHAL, TTypeMask>::zResourceConstRef(zResID _ResourceID)
{
	operator=(_ResourceID);
}

template<class TResourceClass, class TResourceHAL, zU64 TTypeMask>
zResourceConstRef<TResourceClass, TResourceHAL, TTypeMask>::~zResourceConstRef()
{
	SetResource(nullptr);
}

template<class TResourceClass, class TResourceHAL, zU64 TTypeMask>
const zResourceConstRef<TResourceClass, TResourceHAL, TTypeMask>& zResourceConstRef<TResourceClass, TResourceHAL, TTypeMask>::operator=(TResourceClass* _pResource)
{
	SetResource( (zExportData*)_pResource );
	return *this;
}

template<class TResourceClass, class TResourceHAL, zU64 TTypeMask>
const zResourceConstRef<TResourceClass, TResourceHAL, TTypeMask>& zResourceConstRef<TResourceClass, TResourceHAL, TTypeMask>::operator=(const zResourceConstRef& _rResource)
{
	zExportData* pExportData	= reinterpret_cast<zExportData*>(_rResource.mpResource);
	SetResource(pExportData);
	return *this;
}

template<class TResourceClass, class TResourceHAL, zU64 TTypeMask>
template<class TCopyClass, class TCopyHAL, zU64 TCopyMask >
const zResourceConstRef<TResourceClass, TResourceHAL, TTypeMask>& zResourceConstRef<TResourceClass, TResourceHAL, TTypeMask>::operator=(const zResourceConstRef<TCopyClass, TCopyHAL, TCopyMask>& _rResource)
{
	static_assert((TTypeMask & TCopyMask) != 0, "Trying to assign an unsupported resource to this object");
	zExportData* pExportData	= reinterpret_cast<zExportData*>(_rResource.mpResource);
	SetResource(pExportData);
	return *this;
}

template<class TResourceClass, class TResourceHAL, zU64 TTypeMask>
const zResourceConstRef<TResourceClass, TResourceHAL, TTypeMask>& zResourceConstRef<TResourceClass, TResourceHAL, TTypeMask>::operator=(const zResID& _ResourceID)
{
	zEngineRef<zExportData> rExportData = zcDepot::ExportData.Get(_ResourceID);
	SetResource(rExportData.Get());
	return *this;
}

template<class TResourceClass, class TResourceHAL, zU64 TTypeMask>
bool zResourceConstRef<TResourceClass, TResourceHAL, TTypeMask>::operator==( const zResourceConstRef& _rCmpResource ) const
{
	return mpResource == _rCmpResource.mpResource;
}

template<class TResourceClass, class TResourceHAL, zU64 TTypeMask>
bool zResourceConstRef<TResourceClass, TResourceHAL, TTypeMask>::operator!=( const zResourceConstRef& _rCmpResource ) const
{
	return mpResource != _rCmpResource.mpResource;
}

template<class TResourceClass, class TResourceHAL, zU64 TTypeMask>
const TResourceClass* zResourceConstRef<TResourceClass, TResourceHAL, TTypeMask>::operator->() const
{
	return Get();
}

template<class TResourceClass, class TResourceHAL, zU64 TTypeMask>
const TResourceClass* zResourceConstRef<TResourceClass, TResourceHAL, TTypeMask>::Get() const
{
	return mpResource;
}

template<class TResourceClass, class TResourceHAL, zU64 TTypeMask>
const TResourceHAL* zResourceConstRef<TResourceClass, TResourceHAL, TTypeMask>::HAL()const
{
	return reinterpret_cast<const TResourceHAL*>(mpResource);
}

template<class TResourceClass, class TResourceHAL, zU64 TTypeMask>
zResID zResourceConstRef<TResourceClass, TResourceHAL, TTypeMask>::GetResID() const
{
	return mpResource ? reinterpret_cast<const zExportData*>(mpResource)->mResID : zResID();
}

template<class TResourceClass, class TResourceHAL, zU64 TTypeMask>
void zResourceConstRef<TResourceClass, TResourceHAL, TTypeMask>::SetResource(zExportData* _pExportData)
{
	if( (void*)_pExportData != (void*)mpResource )
	{
		const bool bSupportedResourceType = _pExportData &&
											(	(_pExportData->mResID.GetType() == zenConst::keResType__Invalid) ||	// When in middle of creating, but not assigned a ResId yet
												(TTypeMask & (zU64(1)<<_pExportData->mResID.GetType()))
											);
		zenAssert( _pExportData == nullptr || bSupportedResourceType );

		if( mpResource )
			reinterpret_cast<zExportData*>(mpResource)->ReferenceRem();

		mpResource = bSupportedResourceType ? reinterpret_cast<TResourceClass*>(_pExportData) : nullptr;

		if( mpResource)
			reinterpret_cast<zExportData*>(mpResource)->ReferenceAdd();		
	}
}

template<class TResourceClass, class TResourceHAL, zU64 TTypeMask>
TResourceClass* zResourceRef<TResourceClass, TResourceHAL, TTypeMask>::operator->() const
{
	return Get();
}

template<class TResourceClass, class TResourceHAL, zU64 TTypeMask>
TResourceClass* zResourceRef<TResourceClass, TResourceHAL, TTypeMask>::Get() const
{
	return mpResource;
}

template<class TResourceClass, class TResourceHAL, zU64 TTypeMask>
TResourceHAL* zResourceRef<TResourceClass, TResourceHAL, TTypeMask>::HAL() const
{
	return reinterpret_cast<TResourceHAL*>(mpResource);
}

}} // namespace zen, zenRes