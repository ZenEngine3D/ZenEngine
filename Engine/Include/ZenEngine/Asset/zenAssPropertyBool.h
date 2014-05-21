#pragma once
#ifndef __zenEngine_Asset_PropertyBool_h__
#define __zenEngine_Asset_PropertyBool_h__

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss 
{

class PropertyBool : public TPropertyDefinition<zenConst::keAssProp_Bool, PropertyBool, bool>
{
ZENClassDeclare(PropertyBool, TPropertyDefinition);	
public:	
	PropertyBool(const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, ValueStorage _Default)
	: TPropertyDefinition(_zName, _zDisplayName, _zDescription, _bShowInAssetDesc)
	, mDefault(_Default)
	{}

	ValueStorage mDefault;
};

}} //namespace zen { namespace zenAss

#endif
#endif