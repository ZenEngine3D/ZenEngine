#include "zeEngine.h"

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss
{

PropertyDefRef PropertyBool::Create( const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, bool _bIsEditable, ValueStorage _Default )
{		
	PropertyBool* pNewDefinition	= zenMem::NewPool<zenAss::PropertyBool>(_zName, _zDisplayName, _zDescription, _bShowInAssetDesc, _bIsEditable);
	pNewDefinition->mDefault		= _Default;
	return pNewDefinition;
}

PropertyDefRef PropertyFile::Create( const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, bool _bIsEditable, const zChar* _zDefault, const zChar* _zFileExt )
{		
	PropertyFile* pNewDefinition	= zenMem::NewPool<zenAss::PropertyFile>(_zName, _zDisplayName, _zDescription, _bShowInAssetDesc, _bIsEditable);
	pNewDefinition->mDefault		= _zDefault;
	pNewDefinition->mzFileExt		= _zFileExt;
	return pNewDefinition;
}

PropertyDefRef PropertyAsset::Create( const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, bool _bIsEditable, ValueStorage _Default, zFlagAssetType _TypeMask )
{
	PropertyAsset* pNewDefinition	= zenMem::NewPool<zenAss::PropertyAsset>(_zName, _zDisplayName, _zDescription, _bShowInAssetDesc, _bIsEditable);
	pNewDefinition->mDefault		= _Default;
	pNewDefinition->mSupportedType	= _TypeMask;
	return pNewDefinition;
}


}} //namespace zen { namespace zenAss
#else

zenPreventEmptyFileWarning

#endif //ZEN_ENGINETOOL
