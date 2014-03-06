#pragma once
#ifndef __zenBase_Type_Flag_h__
#define __zenBase_Type_Flag_h__

#define FLAG_ParamPrefix(_N_)	, TEnumFlag v##_N_
#define FLAG_ValuePrefix(_N_)	| (TStorage(1)<<v##_N_)
#define FLAG_CONSTRUCTOR(_N_)	AWForceInline zenFlag( TEnumFlag v0 ZENRepeat( _N_, FLAG_ParamPrefix, ZENEmpty1) )		\
								: muFlags( (TStorage(1)<<v0)  ZENRepeat( _N_, FLAG_ValuePrefix, ZENEmpty1) )			\
								{ CheckValid(muFlags); }
#define FLAG_ANY(_N_)			AWForceInline bool Any( TEnumFlag v0 ZENRepeat( _N_, FLAG_ParamPrefix, ZENEmpty1) )	\
								{ return Any( (TStorage(1)<<v0) ZENRepeat( _N_, FLAG_ValuePrefix, ZENEmpty1)); }
#define FLAG_ALL(_N_)			AWForceInline bool All( TEnumFlag v0 ZENRepeat( _N_, FLAG_ParamPrefix, ZENEmpty1) )	\
								{ return All( (TStorage(1)<<v0) ZENRepeat( _N_, FLAG_ValuePrefix, ZENEmpty1)); }

namespace zen { namespace zenType {

template<typename TEnumFlag, int TEnumMax, typename TStorage=zenU32>
class zenFlag
{
public:
	ZENInline					zenFlag();

	FLAG_CONSTRUCTOR(0);	FLAG_CONSTRUCTOR(1);	FLAG_CONSTRUCTOR(2);	FLAG_CONSTRUCTOR(3);	FLAG_CONSTRUCTOR(4);
	FLAG_CONSTRUCTOR(5);	FLAG_CONSTRUCTOR(6);	FLAG_CONSTRUCTOR(7);	FLAG_CONSTRUCTOR(8);	FLAG_CONSTRUCTOR(9);

	FLAG_ALL(0);			FLAG_ALL(1);			FLAG_ALL(2);			FLAG_ALL(3);			FLAG_ALL(4);
	FLAG_ALL(5);			FLAG_ALL(6);			FLAG_ALL(7);			FLAG_ALL(8);			FLAG_ALL(9);

	FLAG_ANY(0);			FLAG_ANY(1);			FLAG_ANY(2);			FLAG_ANY(3);			FLAG_ANY(4);
	FLAG_ANY(5);			FLAG_ANY(6);			FLAG_ANY(7);			FLAG_ANY(8);			FLAG_ANY(9);
	
	ZENInline const zenFlag&	operator= (const zenFlag& _Cpy);
	ZENInline const zenFlag&	operator+=(const zenFlag& _Add);
	ZENInline const zenFlag&	operator-=(const zenFlag& _Rem);
	ZENInline const zenFlag&	operator&=(const zenFlag& _And);
	ZENInline const zenFlag&	operator|=(const zenFlag& _Or);
	ZENInline const zenFlag&	operator^=(const zenFlag& _Xor);

	ZENInline zenFlag			operator&(const zenFlag& _And);
	ZENInline zenFlag			operator|(const zenFlag& _And);
	ZENInline zenFlag			operator^(const zenFlag& _And);
	
	ZENInline const zenFlag&	Reset()	;
	ZENInline const zenFlag&	Invert();
	//ZENInline const zenFlag&	ForceSet(TStorage _FlagValues);

	ZENInline bool				Any( )const						;
	ZENInline bool				Any( const zenFlag& _Cmp )const	;
	ZENInline bool				All( const zenFlag& _Cmp )const	;
	ZENInline zenUInt			CountActive();
	ZENInline zenUInt			CountInactive();

	ZENInline bool				operator==( const zenFlag& _Cmp )const;
	ZENInline bool				operator!=( const zenFlag& _Cmp )const;

protected:
	ZENInline					zenFlag(TStorage _Mask);
	AWForceInline void		CheckValid(TStorage _Value);
	ZENInline bool				Any( TStorage _uFlags );
	ZENInline bool				All( TStorage _uFlags );
	TStorage				muFlags;
};

} } //namespace zen, Type

#include "zenTypeFlag.inl"

#undef FLAG_ParamPrefix
#undef FLAG_ValuePrefix
#undef FLAG_CONSTRUCTOR
#undef FLAG_ANY
#undef FLAG_ALL
#endif
