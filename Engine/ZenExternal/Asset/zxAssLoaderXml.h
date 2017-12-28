#pragma once

#if ZEN_ENGINETOOL

#include <ThirdParty/pugixml/src/pugixml.hpp>

namespace zen { namespace zxAss
{	

class AssetLoaderXml : public zeAss::AssetSerializer
{
zenClassDeclare(AssetLoaderXml, zeAss::AssetSerializer)
public:
	virtual bool	LoadPackages();	
	virtual bool	Save			(const zenAss::zPackageRef& _rPackage);	
	virtual bool	Delete			(const zenAss::zPackageRef& _rPackage);
	virtual bool	Save			(const zenAss::zPackageGroupRef& _rGroup);
	virtual bool	Delete			(const zenAss::zPackageGroupRef& _rGroup);
	
protected:
	void			GetGroupFilename(zWString& _zFilenameOut, const zenAss::zPackageGroupRef& _rGroup);
	bool			LoadGroup		(const zenAss::zPackageGroupRef& _rParent, const zbFile::Filename& _Filename);
	bool			LoadPackage		(const zenAss::zPackageGroupRef& _rParent, const zbFile::Filename& _Filename);
	bool			LoadAsset		(const zenAss::zPackageRef& _rPackage, const pugi::xml_node& _XmlNodeAsset);	
	bool			SaveAsset		(const zenAss::zAssetItemRef& _rAsset, pugi::xml_node& _XmlNodeDoc);
};

}} //namespace zen { namespace zxAss


#endif
