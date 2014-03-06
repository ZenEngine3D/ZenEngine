#pragma once
#ifndef __LibRessource_Resource_Manager_h__
#define __LibRessource_Resource_Manager_h__

namespace ERes
{
	class ResourceAnyRef;
	
	class ManagerResource : public zbType::Manager
	{
	ZENClassDeclare(ManagerResource, zbType::Manager)
	public:		
		zenRes::awResourceRef	GetResource				(const zenResID& _ResID, bool _bSupportDefault=TRUE);
		zenRes::awResourceRef	GetResourceAnySource	(const zenResID& _ResID, bool _bSupportDefault=TRUE);		
		zenRes::awResourceRef	Add						(zenRes::awResource* _pResource);
	protected:
		zenMap<zenRes::awResource*>::Key64	mdResources;											//!< All resource currently loaded
		zenRes::awResource*					maResourcesDefault[zenConst::keResType__Invalid+1];	//!< Default resource used when resource at a key entry isn't loaded

	//---------------------------------------------------------
	// ManagerBase Section
	//---------------------------------------------------------
	public:
		virtual	bool				Load();
		virtual	bool				Unload();
	};
}

namespace EMgr { extern ERes::ManagerResource Resources; }

#endif
