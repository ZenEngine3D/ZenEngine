#pragma once
#ifndef __LibZenEngine_Asset_Serializer_h__
#define __LibZenEngine_Asset_Serializer_h__

#if ZEN_ENGINETOOL
namespace zen { namespace zeAss
{	

class AssetSerializer
{
ZENClassDeclareNoParent(AssetSerializer)
public:	
	virtual bool	LoadPackages()=0;	 
	virtual bool	Save		( const zenAss::zPackageRef& _rPackage)=0;
	virtual bool	Save		( const zenAss::zPackageGroupRef& _rGroup)=0;
	virtual bool	Delete		( const zenAss::zPackageRef& _rPackage)=0;
	virtual bool	Delete		( const zenAss::zPackageGroupRef& _rGroup)=0;	//!< By the time this is called, there's nothing left in the group
	
protected:
	void			AddPackage	(const zenAss::zPackageRef& _rPackage);
	void			AddAsset	(const zenAss::zAssetItemRef& _rAsset);
};

}} //namespace zen { namespace zeAss


#endif
#endif