#pragma once
#ifndef __zCore_Exp_ResourceData_Manager_h__
#define __zCore_Exp_ResourceData_Manager_h__

#include <map>

namespace zcExp
{

class DepotResourceData : public zbType::Manager
{
ZENClassDeclare(DepotResourceData, zbType::Manager)
public:
										DepotResourceData();			
	void								SetItem(const zEngineRef<ResourceData>& _rResData);
	void								ClearItem(const zResID& _ResID);

	zEngineConstRef<ResourceData>		GetItemBase(const zResID& _ResID);
	zEngineConstRef<ResourceData>		GetItemBaseAnySource(const zResID& _ResID);
	bool								IsValid(const zResID& _ResID);
	bool								IsValid(const zArrayBase<zResID>& _aResID);

	zenConst::eEngineVersion			GetEngineVersion(zenConst::eResType _eResType)const;

	//! @todo clean should check object type
	template<class TType>
	zEngineConstRef<TType> GetItem(const zResID _ResID)
	{
		zEngineConstRef<ResourceData> rResData = GetItemBase(_ResID);
		if( rResData.IsValid() )
			return static_cast<const TType*>(rResData.Get());
		return nullptr;
	} 

	template<class TType>
	zEngineConstRef<TType> GetItemAnySource(const zResID _ResID)
	{
		zEngineConstRef<ResourceData> rResData = GetItemBaseAnySource(_ResID);
		if( rResData.IsValid() )
			return static_cast<const TType*>(rResData.Get());
		return nullptr;
	}
	
protected:
	//zMap<zEngineRef<ResourceData>>::Key64	mdResourceData;		//! @todo Urgent need to fix hamt implementation to properly support non plain data type
	std::map<zU64, zEngineRef<ResourceData>, std::less<zU64>> mdResourceData;
	
//---------------------------------------------------------
// ManagerBase Section
//---------------------------------------------------------
public:
	virtual	bool	Load();
	virtual	bool	Unload();
};

}

namespace zcDepot{ extern zcExp::DepotResourceData ResourceData; }

#endif