#pragma once
#ifndef __zenEngine_Asset_PropertyBool_h__
#define __zenEngine_Asset_PropertyBool_h__

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss 
{

class PropertyBool : public TPropertyDefinition<zenConst::keAssProp_Bool, PropertyBool, bool>
{
	ZENPropertyDefinitionDeclare( PropertyBool  )
	static PropertyDefRef Create( const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, 
									ValueStorage _Default );

};

}} //namespace zen { namespace zenAss

#endif
#endif