#pragma once
#ifndef __LibRessource_Resource_Manager_h__
#define __LibRessource_Resource_Manager_h__

namespace ERes
{
	class ResourceAnyRef;
	
	class ManagerResource : public zbType::Manager
	{
	AWClassDeclare(ManagerResource, zbType::Manager)
	public:		
		awres::awResourceRef	GetResource				(const awResourceID& _ResID, bool _bSupportDefault=TRUE);
		awres::awResourceRef	GetResourceAnySource	(const awResourceID& _ResID, bool _bSupportDefault=TRUE);		
		awres::awResourceRef	Add						(awres::awResource* _pResource);
	protected:
		Hashmap<awres::awResource*>::Key64	mdResources;											//!< All resource currently loaded
		awres::awResource*					maResourcesDefault[awconst::keResType__Invalid+1];	//!< Default resource used when resource at a key entry isn't loaded

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
