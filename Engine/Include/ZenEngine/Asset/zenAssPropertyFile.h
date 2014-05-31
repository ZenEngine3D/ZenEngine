#pragma once
#ifndef __zenEngine_Asset_PropertyFile_h__
#define __zenEngine_Asset_PropertyFile_h__

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss 
{
	
class PropertyFile : public TPropertyDefinition<zenConst::keAssProp_File, PropertyFile, zWString>
{
	ZENPropertyDefinitionDeclare( PropertyFile )
	static PropertyDefRef Create( const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, 
									const zChar* _zDefault, const zChar* _zFileExt=zenT("Any(*.*)|*.*") );
	
	zWString mzFileExt;	//!< List of supported files extensions to display in file dialog
};
	
}} //namespace zen { namespace zenAss

#endif
#endif