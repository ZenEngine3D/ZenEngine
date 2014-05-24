#include "libZenEngine.h"

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss
{

PropertyDefRef PropertyBool::Create( const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, ValueStorage _Default )
{		
	static zenMem::zAllocatorPool sAllocPool( "PropertyDefinition::Create", sizeof(PropertyBool), 256, 256 );
	PropertyBool* pNewDefinition	= zenNew(&sAllocPool) zenAss::PropertyBool(_zName, _zDisplayName, _zDescription, _bShowInAssetDesc);
	pNewDefinition->mDefault		= _Default;
	return pNewDefinition;
}

PropertyDefRef PropertyFile::Create( const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, const char* _zDefault, const char* _zFileExt )
{		
	static zenMem::zAllocatorPool sAllocPool( "PropertyDefinition::Create", sizeof(PropertyFile), 256, 256 );
	PropertyFile* pNewDefinition	= zenNew(&sAllocPool) zenAss::PropertyFile(_zName, _zDisplayName, _zDescription, _bShowInAssetDesc);
	pNewDefinition->mDefault		= _zDefault;
	pNewDefinition->mzFileExt		= _zFileExt;
	return pNewDefinition;
}

}} //namespace zen { namespace zenAss

#endif //ZEN_ENGINETOOL
