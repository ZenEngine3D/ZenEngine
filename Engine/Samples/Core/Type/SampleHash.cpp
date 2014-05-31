#include "libZenEngine.h"

//=================================================================================================
//! @example SampleHash.cpp
//! Informations on the usage of various string Hashing functionalities
//=================================================================================================

namespace sample
{
	//==================================================================================================
	//! @brief		Test the result of the Hashing class	
	//==================================================================================================
	void SampleHash()
	{	
		zbMgr::Log.Log(zbLog::keLog_Game, zenConst::kzLineA40);
		zbMgr::Log.Log(zbLog::keLog_Game, " HASHING");
		zbMgr::Log.Log(zbLog::keLog_Game, zenConst::kzLineA40);

		//-------------------------------------------------
		// Precompile hash vs runtime hash
		const char zRuntimeFixed[]	= "Test";
		const char* pRuntimePointer	= zRuntimeFixed;
		zHash32 h32Precompile("Test");					// Precomputed during compilation
		zHash32 h32RuntimeFixed(zRuntimeFixed);			// Runtime evaluation, but using constructor expansion (since array is of known size)
		zHash32 h32RuntimePointer(pRuntimePointer);		// Runtime evaluation, inside a loop
		zbMgr::Log.Log(zbLog::keLog_Game, "Hashing32: Precompiled    0x%08X", h32Precompile);
		zbMgr::Log.Log(zbLog::keLog_Game, "           RuntimeFixed   0x%08X", h32RuntimeFixed );
		zbMgr::Log.Log(zbLog::keLog_Game, "           RuntimePointer 0x%08X (Valid: %s)", h32RuntimePointer, zenConst::kzFalseTrue[h32Precompile==h32RuntimeFixed && h32RuntimeFixed==h32RuntimePointer] );
		//-------------------------------------------------
		// Value copy between hashes
		zHash32 h32Assign1("TestAssign"); 
		zHash32 h32Assign2; 
		zHash32 h32Assign3("Test");
		h32Assign3 = h32Assign2 = h32Assign1;		
		zbMgr::Log.Log(zbLog::keLog_Game, "Copy32:    Value 1        0x%08X ", h32Assign1);
		zbMgr::Log.Log(zbLog::keLog_Game, "           Value 2        0x%08X ", h32Assign2);
		zbMgr::Log.Log(zbLog::keLog_Game, "           Value 3        0x%08X (Valid: %s)", h32Assign3, zenConst::kzFalseTrue[h32Assign1==h32Assign2 && h32Assign3==h32Assign1] );
		//-------------------------------------------------
		// Resuming string hashing
		zHash32 h32Once("TestResume"); 
		zHash32 h32Partial("Test"); 
		zHash32 h32Resumed; 
		h32Resumed = h32Partial;
		h32Resumed.Append("Resume");
		zbMgr::Log.Log(zbLog::keLog_Game, "Resume32:  Once           0x%08X", h32Once);
		zbMgr::Log.Log(zbLog::keLog_Game, "           Partial        0x%08X", h32Partial );
		zbMgr::Log.Log(zbLog::keLog_Game, "           Resumed        0x%08X (Valid: %s)", h32Resumed, zenConst::kzFalseTrue[h32Once==h32Resumed] );
	
		//-------------------------------------------------
		// Precompile hash vs runtime hash
		zHash64 h64Precompile("Test");					// Precomputed during compilation
		zHash64 h64RuntimeFixed(zRuntimeFixed);		// Runtime evaluation, but using constructor expansion (since array is of known size)
		zHash64 h64RuntimePointer(pRuntimePointer);	// Runtime evaluation, inside a loop
		zbMgr::Log.Log(zbLog::keLog_Game, "Hashing64: Precompiled    0x%016llX", h64Precompile);
		zbMgr::Log.Log(zbLog::keLog_Game, "           RuntimeFixed   0x%016llX", h64RuntimeFixed );
		zbMgr::Log.Log(zbLog::keLog_Game, "           RuntimePointer 0x%016llX (Valid: %s)", h64RuntimePointer, zenConst::kzFalseTrue[h64Precompile==h64RuntimeFixed && h64RuntimeFixed==h64RuntimePointer] );
		//-------------------------------------------------
		// Value copy between hashes
		zHash64 h64Assign1("TestAssign"); 
		zHash64 h64Assign2; 
		zHash64 h64Assign3("Test");
		h64Assign3 = h64Assign2 = h64Assign1;		
		zbMgr::Log.Log(zbLog::keLog_Game, "Copy64:    Value 1        0x%016llX ", h64Assign1);
		zbMgr::Log.Log(zbLog::keLog_Game, "           Value 2        0x%016llX ", h64Assign2);
		zbMgr::Log.Log(zbLog::keLog_Game, "           Value 3        0x%016llX (Valid: %s)", h64Assign3, zenConst::kzFalseTrue[h64Assign1==h64Assign2 && h64Assign3==h64Assign1] );
		//-------------------------------------------------
		// Resuming string hashing
		zHash64 h64Once("TestResume"); 
		zHash64 h64Partial("Test"); 
		zHash64 h64Resumed; 
		h64Resumed = h64Partial;
		h64Resumed.Append("Resume");
		zbMgr::Log.Log(zbLog::keLog_Game, "Resume64:  Once           0x%016llX", h64Once);
		zbMgr::Log.Log(zbLog::keLog_Game, "           Partial        0x%016llX", h64Partial );
		zbMgr::Log.Log(zbLog::keLog_Game, "           Resumed        0x%016llX (Valid: %s)", h64Resumed, zenConst::kzFalseTrue[h64Once==h64Resumed] );
	}

}