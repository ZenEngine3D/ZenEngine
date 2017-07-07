#pragma once

#include <map>

namespace zcExp
{

class DepotResourceData : public zbType::Manager
{
zenClassDeclare(DepotResourceData, zbType::Manager)
public:
									DepotResourceData();			
	void							Remove(const zResID& _ResID);
	void							Set(zenRes::zExportData* _pExportData);
	zEngineRef<zenRes::zExportData>	Get(const zResID& _ResID);//todo remove this
	zEngineRef<zenRes::zExportData>	GetAnySource(const zResID& _ResID);
	
	bool							IsValid(const zResID& _ResID);
	bool							IsValid(const zArrayBase<zResID>& _aResID);

	zenConst::eEngineVersion		GetEngineVersion(zenConst::eResType _eResType)const;

	//! @todo clean should check object type
	template<class TType>
	zEngineRef<TType> GetTyped(const zResID _ResID)
	{
		zEngineRef<zenRes::zExportData> rExportData = Get(_ResID);
		if( rExportData.IsValid() )
			return static_cast<TType*>(rExportData.Get());
		return nullptr;
	}

protected:
	std::map<zU64, zenRes::zExportData*, std::less<zU64>>	mdResourceData;
	
//---------------------------------------------------------
// ManagerBase Section
//---------------------------------------------------------
public:
	virtual	bool	Load();
	virtual	bool	Unload();
};

}

namespace zcDepot{ extern zcExp::DepotResourceData ExportData; }
