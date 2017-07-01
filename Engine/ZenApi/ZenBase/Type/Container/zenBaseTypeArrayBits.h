#pragma once
#ifndef __zenApi_Base_Type_ArrayBits_h__
#define __zenApi_Base_Type_ArrayBits_h__

namespace zen { namespace zenType {

//! @todo 1 Support fixed size, finish implementing methods
class zArrayBits
{
zenClassDeclareNoParent(zArrayBits)
public:
	enum eConstant{kuInvalid=0xFFFFFFFF};
	zenInline					zArrayBits();
	zenInline					zArrayBits(zU32 _uBitCount);
	zenInline					zArrayBits(std::initializer_list<bool> _Entries);
	zenInline					zArrayBits(const zArrayBits& _Copy);
	zenInline					~zArrayBits();

	zenInline bool				Get(zUInt _uBitIndex);	
	zenInline bool				operator[](zUInt _uBitIndex)const;

	zenInline zUInt				GetNextTrue(zUInt _uStartBitIndex=kuInvalid); 
	zenInline zUInt				GetNextFalse(zUInt _uStartBitIndex=kuInvalid);

	zenInline zUInt				AddTrue();
	zenInline zUInt				AddFalse();
	zenInline zUInt				AddRangeTrue(zUInt _uCount);
	zenInline zUInt				AddRangeFalse(zUInt _uCount);
		
	zenInline bool				Toggle(zUInt _uBitIndex);
	zenInline void				Set(zUInt _uBitIndex, bool _bValue);	
	zenInline void				SetRange(zUInt _uBitIndexFirst, zUInt _uBitIndexLast, bool _bValue);
	
	zenInline void				Reset();

protected:
	zenInline void				Resize(zUInt _uBitCount);
	zenInline void				GrowIfNeeded(zUInt _uBitCount);
	zUInt*						mpDataArray=nullptr;
	zUInt						muDataArrayCount=0;
};

} } //namespace zen, Type

#include "zenBaseTypeArrayBits.inl"

#endif
