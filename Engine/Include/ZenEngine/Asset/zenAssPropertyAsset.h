#pragma once
#ifndef __zenEngine_Asset_PropertyAsset_h__
#define __zenEngine_Asset_PropertyAsset_h__

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss 
{

class PropertyAsset : public TPropertyDefinition<zenConst::keAssProp_Asset, PropertyAsset, zAssetID>
{
	ZENPropertyDefinitionDeclare( PropertyAsset )
	static PropertyDefRef Create( const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, 
									ValueStorage _Default, zFlagAssetType _TypeMask=zFlagAssetType(true) );
	
	zFlagAssetType mSupportedType;
};

}} //namespace zen { namespace zenAss

#endif
#endif