#include "libZenEngine.h"

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss
{

zPackage::zPackage()
:Super()
{
}

zPackage::zPackage(const zPackage& _Copy)
: Super(_Copy.mpReference)
{
}

zPackage::zPackage(zeAss::Package* _pAsset)
: Super(_pAsset)
{
}

zPackage& zPackage::operator=(const zPackage& _Copy)
{
	Super::operator=(_Copy.mpReference);
	return *this;
}

zHash64 zPackage::GetID()const									
{ 
	ZENAssert(mpReference); 
	return mpReference->GetID(); 
}
const zString& zPackage::GetName()const							
{ 
	ZENAssert(mpReference); 
	return mpReference->GetName(); 
}
const zArrayStatic<zString>& zPackage::GetGroupAndName()const	
{ 
	ZENAssert(mpReference); 
	return mpReference->GetGroupAndName(); 
}
const zMap<zAssetItem>::Key64& zPackage::GetAsset(zenConst::eAssetType _eType)
{
	ZENAssert(mpReference); 
	return mpReference->GetAsset(_eType); 
}

//zHash32 zPackage::GetGroupID()const							{ return Get()->GetGroupID(); }
//const zString& zAssetItem::GetDescription()const				{ return Get()->GetDescription(); }

}} //namespace zen { namespace zenAss

#endif //ZEN_ENGINETOOL
