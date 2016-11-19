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
	zenInline					zArrayBits(zU32 _uCount);
	zenInline					zArrayBits(std::initializer_list<bool> _Entries);
	zenInline					zArrayBits(const zArrayBits& _Copy);
	zenInline					~zArrayBits();

	zenInline bool				Get(zUInt _uIndex);
	zenInline zUInt				GetFirstTrue();
	zenInline zUInt				GetFirstFalse();
	zenInline zUInt				GetLastTrue();
	zenInline zUInt				GetLastFalse();
	zenInline zUInt				AddIndexTrue();
	zenInline zUInt				AddIndexFalse();

	zenInline bool				operator[](zUInt _uBitIndex)const;
	
	zenInline bool				Toggle(zUInt _uIndex);
	zenInline void				Set(zUInt _uIndex, bool _bValue);	
	zenInline void				SetRange(zUInt _uFirst, zUInt _uLast, bool _bValue);
	
	zenInline void				Reset();

protected:
	zenInline void				Resize(zUInt _uIndexCount);
	zenInline void				GrowIfNeeded(zUInt _uIndexCount);
	zUInt*						mpDataArray=nullptr;
	zUInt						muDataArrayCount=0;
};

} } //namespace zen, Type

#include "zenBaseTypeArrayBits.inl"

#endif
