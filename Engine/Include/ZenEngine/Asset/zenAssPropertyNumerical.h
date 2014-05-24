#pragma once
#ifndef __zenEngine_Asset_PropertyNumerical_h__
#define __zenEngine_Asset_PropertyNumerical_h__

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss 
{

template<zenConst::eAssetPropertyType TPropertyType, class TClassStorage>
class PropertyNumerical : public TPropertyDefinition<TPropertyType, PropertyNumerical<TPropertyType,TClassStorage>, TClassStorage>
{
	ZENPropertyDefinitionDeclare( PropertyNumerical )	
	static PropertyDefRef Create( const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, ValueStorage _Default, ValueStorage _Min=0.f, ValueStorage _Max=1.f, ValueStorage _Inc=1  )
	{		
		typedef PropertyNumerical<TPropertyType, TClassStorage> TPropertyNumerical;
		static zenMem::zAllocatorPool sAllocPool( "PropertyDefinition::Create", sizeof(TPropertyNumerical), 256, 256 );
		TPropertyNumerical* pNewDefinition	= zenNew(&sAllocPool) TPropertyNumerical(_zName, _zDisplayName, _zDescription, _bShowInAssetDesc);
		pNewDefinition->mDefault			= _Default;
		pNewDefinition->mValMin				= _Min;
		pNewDefinition->mValMax				= _Max;
		pNewDefinition->mValInc				= _Inc;
		return pNewDefinition;
	}
	ValueStorage	mValMin;
	ValueStorage	mValMax;
	ValueStorage	mValInc;	
};

typedef PropertyNumerical<zenConst::keAssProp_Int, zI32>		PropertyInt;
typedef PropertyNumerical<zenConst::keAssProp_Int2, zVec2S32>	PropertyInt2;
typedef PropertyNumerical<zenConst::keAssProp_Int3, zVec3S32>	PropertyInt3;
typedef PropertyNumerical<zenConst::keAssProp_Int4, zVec4S32>	PropertyInt4;	
typedef PropertyNumerical<zenConst::keAssProp_Float, float>		PropertyFloat;
typedef PropertyNumerical<zenConst::keAssProp_Float2, zVec2F>	PropertyFloat2;
typedef PropertyNumerical<zenConst::keAssProp_Float3, zVec3F>	PropertyFloat3;
typedef PropertyNumerical<zenConst::keAssProp_Float4, zVec4F>	PropertyFloat4;

}} //namespace zen { namespace zenAss

#endif
#endif