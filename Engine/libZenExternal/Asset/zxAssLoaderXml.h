#pragma once
#ifndef __LibZenExternal_Asset_LoaderXml_h__
#define __LibZenExternal_Asset_LoaderXml_h__

#if ZEN_ENGINETOOL

#include <Engine/ThirdParty/pugixml/src/pugixml.hpp>

namespace zen { namespace zxAss
{	

class AssetLoaderXml : public zeAss::AssetLoader
{
ZENClassDeclare(AssetLoaderXml, zeAss::AssetLoader)
public:
	virtual bool	LoadPackages();
	virtual bool	Save(zenAss::zPackage& _rPackage);

protected:
	bool			LoadPackage(const zbFile::Filename& _Filename);	
	bool			LoadAsset(zeAss::Package& _Package, const pugi::xml_node& _XmlNodeAsset);	
	bool			SaveAsset(zenAss::zAssetItem _rAsset, pugi::xml_node& _XmlNodeDoc);
};

}} //namespace zen { namespace zxAss


#endif
#endif