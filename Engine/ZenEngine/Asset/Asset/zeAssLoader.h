#pragma once

#if ZEN_ENGINETOOL
namespace zeAss
{	

class AssetSerializer
{
zenClassDeclareNoParent(AssetSerializer)
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

} //namespace zeAss

#endif
