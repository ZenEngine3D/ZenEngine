#include "libZenEngine.h"

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss
{

PropertyDefRef PropertyArray::Create( const char* _zName, const char* _zDisplayName, const char* _zDescription, bool _bShowInAssetDesc, const PropertyDefRef& _rPropertyDef, zUInt _uEntryCountMin, zUInt _uEntryCountMax )
{		
	static zenMem::zAllocatorPool sAllocPool( "PropertyDefinition::Create", sizeof(PropertyArray), 256, 256 );
	ZENAssert( _rPropertyDef.IsValid() );
	ZENAssert( _uEntryCountMin <= _uEntryCountMax);
	PropertyArray* pNewDefinition		= zenNew(&sAllocPool) zenAss::PropertyArray(_zName, _zDisplayName, _zDescription, _bShowInAssetDesc);
	pNewDefinition->mrArrayPropertyDef	= _rPropertyDef;
	pNewDefinition->mDefault.SetCount(_uEntryCountMin);
	for(zUInt idx(0); idx<_uEntryCountMin; ++idx)
		pNewDefinition->mDefault[idx] = _rPropertyDef->Allocate();
	return pNewDefinition;
}

PropertyValueRef PropertyArray::ValueRef::AddEntry()
{
	ZENAssert( IsValid() );
	const PropertyArray& Definition		= GetDefinition();
	PropertyValueRef newValue			= Definition.mrArrayPropertyDef->Allocate();
	ValueStorage& arrayValue			= Get();
	arrayValue.Append(newValue);
	return newValue;
}

void PropertyArray::ValueRef::RemEntry(const PropertyValueRef& _ToRemove)
{
	ZENAssert( IsValid() );
	ValueStorage& arrayValue = Get();
	for( zUInt idx(0), count(arrayValue.Count()); idx<count; ++idx)
	{
		if( arrayValue[idx] == _ToRemove )
		{
			for(zUInt idxCopy(idx); idxCopy+1<count; ++idxCopy)
				arrayValue[idxCopy] = arrayValue[idxCopy+1];
			arrayValue.Pop();
			break;
		}
	}
}

}} //namespace zen { namespace zenAss

#endif //ZEN_ENGINETOOL
