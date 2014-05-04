#pragma once
#ifndef __zenBase_Type_Hash_h__
#define __zenBase_Type_Hash_h__

//=================================================================================================
//! @file		CTypHash.h
//! @brief		Various type structure for time 
//! @details	Used over the OS time keeping
// http://altdevblogaday.com/2011/10/27/quasi-compile-time-string-hashing/
//=================================================================================================
namespace zen { namespace zenType 
{
	#define FNV32_Seed					2166136261u
	#define FNV32_Prime					16777619u
	#define FNV32_Prefix(_N)			((
	#define FNV32_Postfix(_N)			^ _zStr[_N-1]) * FNV32_Prime)
	#define FNV32_HASH_CONSTRUCTOR(_N)	ZENInlineForce zHash32(const char (&_zStr)[_N+1]) :  muHash( ZENRepeat(_N, FNV32_Prefix, ZENEmpty1) FNV32_Seed ZENRepeat(_N, ZENEmpty1, FNV32_Postfix) ){}
	#define FNV64_Seed					14695981039346656037ul
	#define FNV64_Prime					1099511628211ul
	#define FNV64_Prefix(_N)			((
	#define FNV64_Postfix(_N)			^ _zStr[_N-1]) * FNV64_Prime)
	#define FNV64_HASH_CONSTRUCTOR(_N)	ZENInlineForce zHash64(const char (&_zStr)[_N+1]) :  muHash( ZENRepeat(_N, FNV64_Prefix, ZENEmpty1) FNV64_Seed ZENRepeat(_N, ZENEmpty1, FNV64_Postfix) ){}

	//=================================================================================================	
	//! @brief Structure used to trick compiler in using HashFNV32() for string pointer, 
	//!			and preprocess hash value for const char array with other constructors
	//=================================================================================================	
	struct ConstCharWrapper
	{ 
		ZENInline ConstCharWrapper(const char* _zStr) 
		: mzStr(_zStr){} 
		const char* mzStr;
	};		


	//=================================================================================================
	//! @brief		Transform a string to a zU32 value
	//! @details	Use FNV-1a algorithm
	//!				Any string under 64 character will be evaluated at compile time
	//!				reducing runtime hashing cost to 0
	//!				
	//!				Class made to preprocess as much as possible the hashing of 
	//!				strings (when receiving a string literal) and runtime compute
	//!				it for char* variable received as input
	//!
	//!				More infos here : http://altdevblogaday.com/2011/10/27/quasi-compile-time-string-hashing/
	//!				And here for hashing : http://isthe.com/chongo/tech/comp/fnv/
	//=================================================================================================	
	class zHash32
	{
	public:		
		//=================================================================================================	
		// Append new value to hash
		//=================================================================================================	
		ZENInlineForce zHash32&	Append(const char* _zString);
		ZENInlineForce zHash32&	Append(const wchar_t* _zString);
		ZENInlineForce zHash32&	Append(const void* _pData, zUInt _uSize);
		ZENInlineForce zHash32&	Append( zHash32 _hHashToAdd);

		//=================================================================================================	
		// Comparators and assignation
		//=================================================================================================	
		ZENInline bool					operator==(const zHash32& _hCmpr);
		ZENInline bool					operator!=(const zHash32& _hCmpr);
		ZENInline zHash32&			operator=(const zHash32& _hCopy);
		ZENInline zHash32&			operator=(const zU32& _uCopy);
		ZENInline operator const		zU32&()const;

		//=================================================================================================	
		// Constructors
		//=================================================================================================	
		ZENInlineForce				zHash32();
		ZENInlineForce				zHash32(const zHash32& _hCopy);
		ZENInlineForce				zHash32(const zU32& _uCopy);
		ZENInlineForce				zHash32(const zI32& _iCopy);
		ZENInlineForce				zHash32(ConstCharWrapper _zStr);
		ZENInlineForce				zHash32(char* _zStr);
		ZENInlineForce				zHash32(const wchar_t* _zStr);
		ZENInlineForce				zHash32(const void* _pData, zUInt _uSize);

									FNV32_HASH_CONSTRUCTOR(1)	FNV32_HASH_CONSTRUCTOR(2)	FNV32_HASH_CONSTRUCTOR(3)	FNV32_HASH_CONSTRUCTOR(4)	
		FNV32_HASH_CONSTRUCTOR(5)	FNV32_HASH_CONSTRUCTOR(6)	FNV32_HASH_CONSTRUCTOR(7)	FNV32_HASH_CONSTRUCTOR(8)	FNV32_HASH_CONSTRUCTOR(9)		
		FNV32_HASH_CONSTRUCTOR(10)	FNV32_HASH_CONSTRUCTOR(11)	FNV32_HASH_CONSTRUCTOR(12)	FNV32_HASH_CONSTRUCTOR(13)	FNV32_HASH_CONSTRUCTOR(14)	
		FNV32_HASH_CONSTRUCTOR(15)	FNV32_HASH_CONSTRUCTOR(16)	FNV32_HASH_CONSTRUCTOR(17)	FNV32_HASH_CONSTRUCTOR(18)	FNV32_HASH_CONSTRUCTOR(19)
		FNV32_HASH_CONSTRUCTOR(20)	FNV32_HASH_CONSTRUCTOR(21)	FNV32_HASH_CONSTRUCTOR(22)	FNV32_HASH_CONSTRUCTOR(23)	FNV32_HASH_CONSTRUCTOR(24)
		FNV32_HASH_CONSTRUCTOR(25)	FNV32_HASH_CONSTRUCTOR(26)	FNV32_HASH_CONSTRUCTOR(27)	FNV32_HASH_CONSTRUCTOR(28)	FNV32_HASH_CONSTRUCTOR(29)
		FNV32_HASH_CONSTRUCTOR(30)	FNV32_HASH_CONSTRUCTOR(31)	FNV32_HASH_CONSTRUCTOR(32)	FNV32_HASH_CONSTRUCTOR(33)	FNV32_HASH_CONSTRUCTOR(34)
		FNV32_HASH_CONSTRUCTOR(35)	FNV32_HASH_CONSTRUCTOR(36)	FNV32_HASH_CONSTRUCTOR(37)	FNV32_HASH_CONSTRUCTOR(38)	FNV32_HASH_CONSTRUCTOR(39)
		FNV32_HASH_CONSTRUCTOR(40)	FNV32_HASH_CONSTRUCTOR(41)	FNV32_HASH_CONSTRUCTOR(42)	FNV32_HASH_CONSTRUCTOR(43)	FNV32_HASH_CONSTRUCTOR(44)
		FNV32_HASH_CONSTRUCTOR(45)	FNV32_HASH_CONSTRUCTOR(46)	FNV32_HASH_CONSTRUCTOR(47)	FNV32_HASH_CONSTRUCTOR(48)	FNV32_HASH_CONSTRUCTOR(49)
		FNV32_HASH_CONSTRUCTOR(50)	FNV32_HASH_CONSTRUCTOR(51)	FNV32_HASH_CONSTRUCTOR(52)	FNV32_HASH_CONSTRUCTOR(53)	FNV32_HASH_CONSTRUCTOR(54)
		FNV32_HASH_CONSTRUCTOR(55)	FNV32_HASH_CONSTRUCTOR(56)	FNV32_HASH_CONSTRUCTOR(57)	FNV32_HASH_CONSTRUCTOR(58)	FNV32_HASH_CONSTRUCTOR(59)
		FNV32_HASH_CONSTRUCTOR(60)	FNV32_HASH_CONSTRUCTOR(61)	FNV32_HASH_CONSTRUCTOR(62)	FNV32_HASH_CONSTRUCTOR(63)		

	protected:
		zU32 muHash;
	};
	

	//=================================================================================================
	//! @brief		Transform a string to a zU64 value
	//! @details	Use FNV-1a algorithm
	//!				Any string under 64 character will be evaluated at compile time
	//!				reducing runtime hashing cost to 0
	//!				
	//!				Class made to preprocess as much as possible the hashing of 
	//!				strings (when receiving a string literal) and runtime compute
	//!				it for char* variable received as input
	//!
	//!				More infos here : http://altdevblogaday.com/2011/10/27/quasi-compile-time-string-hashing/
	//=================================================================================================	
	class zHash64
	{
	public:
		//=================================================================================================	
		// Append new value to hash
		//=================================================================================================	
		ZENInlineForce zHash64&	Append(const char* _zString);
		ZENInlineForce zHash64&	Append(const wchar_t* _zString);
		ZENInlineForce zHash64&	Append(const zU8* _pData, zUInt _uSize );
		ZENInlineForce zHash64&	Append(zHash64 _hHashToAdd);

		//=================================================================================================	
		// Comparators and assignation
		//=================================================================================================	
		ZENInline bool					operator==(const zHash64& _hCmpr);
		ZENInline bool					operator!=(const zHash64& _hCmpr);
		ZENInline zHash64&			operator=(const zHash64& _hCopy);
		ZENInline zHash64&			operator=(const zU64& _uCopy);
		ZENInline						operator const zU64&()const;

		//=================================================================================================	
		// Constructors
		//=================================================================================================	
		ZENInlineForce				zHash64();
		ZENInlineForce				zHash64(const zHash64& _hCopy);
		ZENInlineForce				zHash64(const zU64& _uCopy);
		ZENInlineForce				zHash64(const zI64& _iCopy);
		ZENInlineForce				zHash64(ConstCharWrapper _zStr);
		ZENInlineForce				zHash64(char* _zStr);
		ZENInlineForce				zHash64(wchar_t* _zStr);
		ZENInlineForce				zHash64(const zU8* _pData, zUInt _uSize);

									FNV64_HASH_CONSTRUCTOR(1)	FNV64_HASH_CONSTRUCTOR(2)	FNV64_HASH_CONSTRUCTOR(3)	FNV64_HASH_CONSTRUCTOR(4)	
		FNV64_HASH_CONSTRUCTOR(5)	FNV64_HASH_CONSTRUCTOR(6)	FNV64_HASH_CONSTRUCTOR(7)	FNV64_HASH_CONSTRUCTOR(8)	FNV64_HASH_CONSTRUCTOR(9)		
		FNV64_HASH_CONSTRUCTOR(10)	FNV64_HASH_CONSTRUCTOR(11)	FNV64_HASH_CONSTRUCTOR(12)	FNV64_HASH_CONSTRUCTOR(13)	FNV64_HASH_CONSTRUCTOR(14)	
		FNV64_HASH_CONSTRUCTOR(15)	FNV64_HASH_CONSTRUCTOR(16)	FNV64_HASH_CONSTRUCTOR(17)	FNV64_HASH_CONSTRUCTOR(18)	FNV64_HASH_CONSTRUCTOR(19)
		FNV64_HASH_CONSTRUCTOR(20)	FNV64_HASH_CONSTRUCTOR(21)	FNV64_HASH_CONSTRUCTOR(22)	FNV64_HASH_CONSTRUCTOR(23)	FNV64_HASH_CONSTRUCTOR(24)
		FNV64_HASH_CONSTRUCTOR(25)	FNV64_HASH_CONSTRUCTOR(26)	FNV64_HASH_CONSTRUCTOR(27)	FNV64_HASH_CONSTRUCTOR(28)	FNV64_HASH_CONSTRUCTOR(29)
		FNV64_HASH_CONSTRUCTOR(30)	FNV64_HASH_CONSTRUCTOR(31)	FNV64_HASH_CONSTRUCTOR(32)	FNV64_HASH_CONSTRUCTOR(33)	FNV64_HASH_CONSTRUCTOR(34)
		FNV64_HASH_CONSTRUCTOR(35)	FNV64_HASH_CONSTRUCTOR(36)	FNV64_HASH_CONSTRUCTOR(37)	FNV64_HASH_CONSTRUCTOR(38)	FNV64_HASH_CONSTRUCTOR(39)
		FNV64_HASH_CONSTRUCTOR(40)	FNV64_HASH_CONSTRUCTOR(41)	FNV64_HASH_CONSTRUCTOR(42)	FNV64_HASH_CONSTRUCTOR(43)	FNV64_HASH_CONSTRUCTOR(44)
		FNV64_HASH_CONSTRUCTOR(45)	FNV64_HASH_CONSTRUCTOR(46)	FNV64_HASH_CONSTRUCTOR(47)	FNV64_HASH_CONSTRUCTOR(48)	FNV64_HASH_CONSTRUCTOR(49)
		FNV64_HASH_CONSTRUCTOR(50)	FNV64_HASH_CONSTRUCTOR(51)	FNV64_HASH_CONSTRUCTOR(52)	FNV64_HASH_CONSTRUCTOR(53)	FNV64_HASH_CONSTRUCTOR(54)
		FNV64_HASH_CONSTRUCTOR(55)	FNV64_HASH_CONSTRUCTOR(56)	FNV64_HASH_CONSTRUCTOR(57)	FNV64_HASH_CONSTRUCTOR(58)	FNV64_HASH_CONSTRUCTOR(59)
		FNV64_HASH_CONSTRUCTOR(60)	FNV64_HASH_CONSTRUCTOR(61)	FNV64_HASH_CONSTRUCTOR(62)	FNV64_HASH_CONSTRUCTOR(63)		
	protected:
		zU64 muHash;
	};

}} //namespace zen, Type 

#include "zenTypeHash.inl"

#endif
