#pragma once
#ifndef __zCore_Res_Manager_h__
#define __zCore_Res_Manager_h__

namespace zcRes
{
	class ResourceAnyRef;
	
	class ManagerResource : public zbType::Manager
	{
	ZENClassDeclare(ManagerResource, zbType::Manager)
	public:		
		zenRes::zResourceRef				GetResource				(const zResID& _ResID, bool _bSupportDefault=TRUE);
		zenRes::zResourceRef				GetResourceAnySource	(const zResID& _ResID, bool _bSupportDefault=TRUE);		
		zenRes::zResourceRef				Add						(zenRes::zResource* _pResource);
	protected:
		zMap<zenRes::zResource*>::Key64		mdResources;										//!< All resource currently loaded
		zenRes::zResource*					maResourcesDefault[zenConst::keResType__Invalid+1];	//!< Default resource used when resource at a key entry isn't loaded

	//---------------------------------------------------------
	// ManagerBase Section
	//---------------------------------------------------------
	public:
		virtual	bool						Load();
		virtual	bool						Unload();
	};
}

namespace zcDepot { extern zcRes::ManagerResource Resources; }

#endif
