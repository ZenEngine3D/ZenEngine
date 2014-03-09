#include "libZenEngine.h"

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss
{
#if 0
//=================================================================================================
//! @brief		
//! @details	
//-------------------------------------------------------------------------------------------------
//! @return		
//=================================================================================================
void AssetBase::InitDefault()
{	
	ZENAssert(maPropertyPointer.Count() > 0 );

	// Only need to reset value
	if( maPropertyValue.Count() > 0 )
	{
		zenAss::PropertyValue* pValCur	= maPropertyValue.First();
		zenAss::PropertyValue* pValLast = maPropertyValue.Last();
		while( pValCur <= pValLast )
		{
			pValCur->Reset();
			++pValCur;
		}
	}
	// Need to allocate value (which sets it to default)
	else
	{
		const zenAss::PropertyArray& aProperties = GetProperties();	
		maPropertyValue.SetCount( aProperties.Count() );
		const zenAss::PropertyBase* const*	pPropCur	= aProperties.First();
		zenAss::PropertyValue*				pValCur		= maPropertyValue.First();
		zenAss::PropertyValue*				pValLast	= maPropertyValue.Last();
		while( pValCur <= pValLast )
		{
			pValCur->Allocate(**pPropCur);
			++pValCur;
			++pPropCur;
		}
	}
}

//=================================================================================================
//! @brief		Build a string description based on asset property definitions/values
//! @details	Take all flagged properties and make a string description from it. Used for
//!				Asset short description in editor
//-------------------------------------------------------------------------------------------------
//! @return		
//=================================================================================================
void AssetBase::RebuiltDescription()
{	
	mzDescription = "";
	/*
	//! @TODO
	for(zenUInt idx(0), count(maPropertyValue.Count()); idx<count; ++idx)
	{
		const PropertyDefBase& propDef = maPropertyValue[idx].GetBase()->mParentDef;
		if( propDef.mbShowInAssetDesc )
		{
			char zBuffer[128];			
			char zValue[64];
			maPropertyValue[idx].GetBase()->ValueToString( zValue, sizeof(zValue) );
			sprintf(zBuffer, "(%s : %s) ", propDef.mzDisplayName, zValue);
			mzDescription += zBuffer;
		}		
	}
	*/
}
#endif

}} //namespace zen { namespace zenAss

#endif //ZEN_ENGINETOOL
