#pragma once
#ifndef __zenApi_Base_Type_Flag_h__
#define __zenApi_Base_Type_Flag_h__

#define FLAG_ParamPrefix(_N_)	, TEnumFlag v##_N_
#define FLAG_ValuePrefix(_N_)	| (TStorage(1)<<v##_N_)
#define FLAG_CONSTRUCTOR(_N_)	ZENInlineForce zFlag( TEnumFlag v0 ZENRepeat( _N_, FLAG_ParamPrefix, ZENEmpty1) )		\
								: muFlags( (TStorage(1)<<v0)  ZENRepeat( _N_, FLAG_ValuePrefix, ZENEmpty1) )			\
								{ CheckValid(muFlags); }
#define FLAG_ANY(_N_)			ZENInlineForce bool Any( TEnumFlag v0 ZENRepeat( _N_, FLAG_ParamPrefix, ZENEmpty1) )	\
								{ return Any( (TStorage(1)<<v0) ZENRepeat( _N_, FLAG_ValuePrefix, ZENEmpty1)); }
#define FLAG_ALL(_N_)			ZENInlineForce bool All( TEnumFlag v0 ZENRepeat( _N_, FLAG_ParamPrefix, ZENEmpty1) )	\
								{ return All( (TStorage(1)<<v0) ZENRepeat( _N_, FLAG_ValuePrefix, ZENEmpty1)); }

namespace zen { namespace zenType {

template<typename TEnumFlag, int TEnumMax, typename TStorage=zU32>
class zFlag
{
public:
	ZENInline				zFlag();
	ZENInline				zFlag(bool _bInitAllActive);

	FLAG_CONSTRUCTOR(0);	FLAG_CONSTRUCTOR(1);	FLAG_CONSTRUCTOR(2);	FLAG_CONSTRUCTOR(3);	FLAG_CONSTRUCTOR(4);
	FLAG_CONSTRUCTOR(5);	FLAG_CONSTRUCTOR(6);	FLAG_CONSTRUCTOR(7);	FLAG_CONSTRUCTOR(8);	FLAG_CONSTRUCTOR(9);

	FLAG_ALL(0);			FLAG_ALL(1);			FLAG_ALL(2);			FLAG_ALL(3);			FLAG_ALL(4);
	FLAG_ALL(5);			FLAG_ALL(6);			FLAG_ALL(7);			FLAG_ALL(8);			FLAG_ALL(9);

	FLAG_ANY(0);			FLAG_ANY(1);			FLAG_ANY(2);			FLAG_ANY(3);			FLAG_ANY(4);
	FLAG_ANY(5);			FLAG_ANY(6);			FLAG_ANY(7);			FLAG_ANY(8);			FLAG_ANY(9);
	
	ZENInline const zFlag&	operator= (const zFlag& _Cpy);
	ZENInline const zFlag&	operator+=(const zFlag& _Add);
	ZENInline const zFlag&	operator-=(const zFlag& _Rem);
	ZENInline const zFlag&	operator&=(const zFlag& _And);
	ZENInline const zFlag&	operator|=(const zFlag& _Or);
	ZENInline const zFlag&	operator^=(const zFlag& _Xor);

	ZENInline zFlag			operator&(const zFlag& _And);
	ZENInline zFlag			operator|(const zFlag& _And);
	ZENInline zFlag			operator^(const zFlag& _And);
	
	ZENInline const zFlag&	Reset()	;
	ZENInline const zFlag&	Invert();

	ZENInline bool			Any( )const						;
	ZENInline bool			Any( const zFlag& _Cmp )const	;
	ZENInline bool			All( const zFlag& _Cmp )const	;
	ZENInline zUInt			CountActive();
	ZENInline zUInt			CountInactive();

	ZENInline bool			operator==( const zFlag& _Cmp )const;
	ZENInline bool			operator!=( const zFlag& _Cmp )const;
	
protected:
	ZENInline				zFlag(TStorage _Mask);
	ZENInlineForce void		CheckValid(TStorage _Value);
	ZENInline bool			Any( TStorage _uFlags );
	ZENInline bool			All( TStorage _uFlags );
	TStorage				muFlags;
};

} } //namespace zen, Type

#include "zenBaseTypeFlag.inl"

#undef FLAG_ParamPrefix
#undef FLAG_ValuePrefix
#undef FLAG_CONSTRUCTOR
#undef FLAG_ANY
#undef FLAG_ALL

#endif
