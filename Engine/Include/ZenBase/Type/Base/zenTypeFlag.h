#pragma once
#ifndef __LibCore_Api_Type_Flag_h__
#define __LibCore_Api_Type_Flag_h__

#define FLAG_ParamPrefix(_N_)	, TEnumFlag v##_N_
#define FLAG_ValuePrefix(_N_)	| (TStorage(1)<<v##_N_)
#define FLAG_CONSTRUCTOR(_N_)	AWForceInline awFlag( TEnumFlag v0 AWRepeat( _N_, FLAG_ParamPrefix, AWEmpty1) )		\
								: muFlags( (TStorage(1)<<v0)  AWRepeat( _N_, FLAG_ValuePrefix, AWEmpty1) )			\
								{ CheckValid(muFlags); }
#define FLAG_ANY(_N_)			AWForceInline bool Any( TEnumFlag v0 AWRepeat( _N_, FLAG_ParamPrefix, AWEmpty1) )	\
								{ return Any( (TStorage(1)<<v0) AWRepeat( _N_, FLAG_ValuePrefix, AWEmpty1)); }
#define FLAG_ALL(_N_)			AWForceInline bool All( TEnumFlag v0 AWRepeat( _N_, FLAG_ParamPrefix, AWEmpty1) )	\
								{ return All( (TStorage(1)<<v0) AWRepeat( _N_, FLAG_ValuePrefix, AWEmpty1)); }

namespace zen { namespace awtype {

template<typename TEnumFlag, int TEnumMax, typename TStorage=awU32>
class awFlag
{
public:
	inline					awFlag();

	FLAG_CONSTRUCTOR(0);	FLAG_CONSTRUCTOR(1);	FLAG_CONSTRUCTOR(2);	FLAG_CONSTRUCTOR(3);	FLAG_CONSTRUCTOR(4);
	FLAG_CONSTRUCTOR(5);	FLAG_CONSTRUCTOR(6);	FLAG_CONSTRUCTOR(7);	FLAG_CONSTRUCTOR(8);	FLAG_CONSTRUCTOR(9);

	FLAG_ALL(0);			FLAG_ALL(1);			FLAG_ALL(2);			FLAG_ALL(3);			FLAG_ALL(4);
	FLAG_ALL(5);			FLAG_ALL(6);			FLAG_ALL(7);			FLAG_ALL(8);			FLAG_ALL(9);

	FLAG_ANY(0);			FLAG_ANY(1);			FLAG_ANY(2);			FLAG_ANY(3);			FLAG_ANY(4);
	FLAG_ANY(5);			FLAG_ANY(6);			FLAG_ANY(7);			FLAG_ANY(8);			FLAG_ANY(9);
	
	inline const awFlag&	operator= (const awFlag& _Cpy);
	inline const awFlag&	operator+=(const awFlag& _Add);
	inline const awFlag&	operator-=(const awFlag& _Rem);
	inline const awFlag&	operator&=(const awFlag& _And);
	inline const awFlag&	operator|=(const awFlag& _Or);
	inline const awFlag&	operator^=(const awFlag& _Xor);

	inline awFlag			operator&(const awFlag& _And);
	inline awFlag			operator|(const awFlag& _And);
	inline awFlag			operator^(const awFlag& _And);
	
	inline const awFlag&	Reset()	;
	inline const awFlag&	Invert();
	//inline const awFlag&	ForceSet(TStorage _FlagValues);

	inline bool				Any( )const						;
	inline bool				Any( const awFlag& _Cmp )const	;
	inline bool				All( const awFlag& _Cmp )const	;
	
	inline bool				operator==( const awFlag& _Cmp )const;
	inline bool				operator!=( const awFlag& _Cmp )const;
		
protected:
	inline					awFlag(TStorage _Mask);
	AWForceInline void		CheckValid(TStorage _Value);
	inline bool				Any( TStorage _uFlags );
	inline bool				All( TStorage _uFlags );
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
