#pragma once
#ifndef __zenApi_Base_Type_Hash_h__
#define __zenApi_Base_Type_Hash_h__

namespace zen { namespace zenType 
{
	template<typename TStorage, TStorage TSeed, TStorage TPrime, std::size_t TLen>
	constexpr TStorage HashFNV(const char(&_zString)[TLen])
	{
		return (HashFNV<TStorage, TSeed, TPrime>((const char(&)[TLen - 1])_zString) ^ _zString[TLen - 2]) * TPrime;
	}

	template<typename TStorage, TStorage TSeed, TStorage TPrime>
	constexpr TStorage HashFNV(const char(&_zString)[1])
	{
		return TSeed;
	}

	//=================================================================================================	
	//! @brief Structure used to trick compiler in using HashFNV32() for string pointer, 
	//!			and preprocess hash value for const char array with other constructors
	//=================================================================================================	
	struct ConstCharWrapper
	{ 
		ZENInline ConstCharWrapper(const char* _zStr) : mzStr(_zStr){} 
		const char* mzStr;
	};		

	//=================================================================================================
	//! @brief		Transform a string to a zU32 value
	//! @details	Use FNV-1a algorithm
	//!				Any constant string will be evaluated at compile time reducing runtime 
	//!				hashing cost to 0
	//!				
	//!				Class made to preprocess as much as possible the hashing of 
	//!				strings (when receiving a string literal) and runtime compute
	//!				it for char* variable received as input
	//!
	//!				More infos here : http://altdevblogaday.com/2011/10/27/quasi-compile-time-string-hashing/
	//!				And here for hashing : http://isthe.com/chongo/tech/comp/fnv/
	//! @note:		Added templated recursion instead of using recursive maccro
	//=================================================================================================	
	class zHash32
	{
		enum eConstant
		{
			keFNV32_Seed = 2166136261u,
			keFNV32_Prime = 16777619u
		};
	public:	
		//=================================================================================================	
		// Append new value to hash
		//=================================================================================================	
		ZENInlineForce zHash32&		Append(const char* _zString);
		ZENInlineForce zHash32&		Append(const wchar_t* _zString);
		ZENInlineForce zHash32&		Append(const void* _pData, zUInt _uSize);
		
		//=================================================================================================	
		// Comparators and assignation
		//=================================================================================================	
		ZENInline bool				operator==(const zHash32& _hCmpr);
		ZENInline bool				operator!=(const zHash32& _hCmpr);
		ZENInline zHash32&			operator=(const zHash32& _hCopy);
		ZENInline zHash32&			operator=(const zU32& _uCopy);
		ZENInline operator const	zU32&()const;

		//=================================================================================================	
		// Constructors
		//=================================================================================================	
		ZENInlineForce				zHash32();		
		template<std::size_t TLen>	zHash32(const char(&_zString)[TLen]);
		ZENInlineForce				zHash32(char* _zString);
		ZENInlineForce				zHash32(ConstCharWrapper _zStr);
		ZENInlineForce				zHash32(const void* _pData, zUInt _uSize);
		ZENInlineForce				zHash32(const wchar_t* _zStr);
		ZENInlineForce				zHash32(const zU32& _uCopy);
		ZENInlineForce				zHash32(const zI32& _iCopy);
		
	protected:
		zU32 muHash;
	};
	

	//=================================================================================================
	//! @brief		Transform a string to a zU64 value
	//! @details	Use FNV-1a algorithm
	//!				Any constant string will be evaluated at compile time reducing runtime
	//!				hashing cost to 0
	//!				
	//!				Class made to preprocess as much as possible the hashing of 
	//!				strings (when receiving a string literal) and runtime compute
	//!				it for char* variable received as input
	//!
	//!				More infos here : http://altdevblogaday.com/2011/10/27/quasi-compile-time-string-hashing/
	//! @note:		Added templated recursion instead of using recursive maccro
	//=================================================================================================	
	class zHash64
	{
		enum eConstant : zU64
		{
			keFNV64_Seed	= 14695981039346656037ul,
			keFNV64_Prime	= 1099511628211ul
		};
	public:
		//=================================================================================================	
		// Append new value to hash
		//=================================================================================================	
		ZENInlineForce zHash64&		Append(const char* _zString);
		ZENInlineForce zHash64&		Append(const wchar_t* _zString);
		ZENInlineForce zHash64&		Append(const void* _pData, zUInt _uSize);

		//=================================================================================================	
		// Comparators and assignation
		//=================================================================================================	
		ZENInline bool				operator==(const zHash64& _hCmpr);
		ZENInline bool				operator!=(const zHash64& _hCmpr);
		ZENInline zHash64&			operator=(const zHash64& _hCopy);
		ZENInline zHash64&			operator=(const zU64& _uCopy);
		ZENInline					operator const zU64&()const;

		//=================================================================================================	
		// Constructors
		//=================================================================================================	
		ZENInlineForce				zHash64();
		template<std::size_t TLen>	zHash64(const char(&_zString)[TLen]);
		ZENInlineForce				zHash64(ConstCharWrapper _zString);
		ZENInlineForce				zHash64(char* _zString);
		ZENInlineForce				zHash64(const wchar_t* _zString);
		ZENInlineForce				zHash64(const void* _pData, zUInt _uSize);
		ZENInlineForce				zHash64(const zHash64& _hCopy);
		ZENInlineForce				zHash64(const zU64& _uCopy);
		ZENInlineForce				zHash64(const zI64& _iCopy);
		

	protected:
		zU64 muHash;
	};

}} //namespace zen, Type 

#include "zenBaseTypeHash.inl"

#endif
