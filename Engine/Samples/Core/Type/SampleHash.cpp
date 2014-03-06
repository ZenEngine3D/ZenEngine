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
		CMgr::Log.Log(CLog::keLog_Game, zenConst::kzLineA40);
		CMgr::Log.Log(CLog::keLog_Game, " HASHING");
		CMgr::Log.Log(CLog::keLog_Game, zenConst::kzLineA40);

		//-------------------------------------------------
		// Precompile hash vs runtime hash
		const char zRuntimeFixed[]	= "Test";
		const char* pRuntimePointer	= zRuntimeFixed;
		zenHash32 h32Precompile("Test");					// Precomputed during compilation
		zenHash32 h32RuntimeFixed(zRuntimeFixed);			// Runtime evaluation, but using constructor expansion (since array is of known size)
		zenHash32 h32RuntimePointer(pRuntimePointer);		// Runtime evaluation, inside a loop
		CMgr::Log.Log(CLog::keLog_Game, "Hashing32: Precompiled    0x%08X", h32Precompile);
		CMgr::Log.Log(CLog::keLog_Game, "           RuntimeFixed   0x%08X", h32RuntimeFixed );
		CMgr::Log.Log(CLog::keLog_Game, "           RuntimePointer 0x%08X (Valid: %s)", h32RuntimePointer, zenConst::kzFalseTrue[h32Precompile==h32RuntimeFixed && h32RuntimeFixed==h32RuntimePointer] );
		//-------------------------------------------------
		// Value copy between hashes
		zenHash32 h32Assign1("TestAssign"); 
		zenHash32 h32Assign2; 
		zenHash32 h32Assign3("Test");
		h32Assign3 = h32Assign2 = h32Assign1;		
		CMgr::Log.Log(CLog::keLog_Game, "Copy32:    Value 1        0x%08X ", h32Assign1);
		CMgr::Log.Log(CLog::keLog_Game, "           Value 2        0x%08X ", h32Assign2);
		CMgr::Log.Log(CLog::keLog_Game, "           Value 3        0x%08X (Valid: %s)", h32Assign3, zenConst::kzFalseTrue[h32Assign1==h32Assign2 && h32Assign3==h32Assign1] );
		//-------------------------------------------------
		// Resuming string hashing
		zenHash32 h32Once("TestResume"); 
		zenHash32 h32Partial("Test"); 
		zenHash32 h32Resumed; 
		h32Resumed = h32Partial;
		h32Resumed.Append("Resume");
		CMgr::Log.Log(CLog::keLog_Game, "Resume32:  Once           0x%08X", h32Once);
		CMgr::Log.Log(CLog::keLog_Game, "           Partial        0x%08X", h32Partial );
		CMgr::Log.Log(CLog::keLog_Game, "           Resumed        0x%08X (Valid: %s)", h32Resumed, zenConst::kzFalseTrue[h32Once==h32Resumed] );
	
		//-------------------------------------------------
		// Precompile hash vs runtime hash
		zenHash64 h64Precompile("Test");					// Precomputed during compilation
		zenHash64 h64RuntimeFixed(zRuntimeFixed);		// Runtime evaluation, but using constructor expansion (since array is of known size)
		zenHash64 h64RuntimePointer(pRuntimePointer);	// Runtime evaluation, inside a loop
		CMgr::Log.Log(CLog::keLog_Game, "Hashing64: Precompiled    0x%016llX", h64Precompile);
		CMgr::Log.Log(CLog::keLog_Game, "           RuntimeFixed   0x%016llX", h64RuntimeFixed );
		CMgr::Log.Log(CLog::keLog_Game, "           RuntimePointer 0x%016llX (Valid: %s)", h64RuntimePointer, zenConst::kzFalseTrue[h64Precompile==h64RuntimeFixed && h64RuntimeFixed==h64RuntimePointer] );
		//-------------------------------------------------
		// Value copy between hashes
		zenHash64 h64Assign1("TestAssign"); 
		zenHash64 h64Assign2; 
		zenHash64 h64Assign3("Test");
		h64Assign3 = h64Assign2 = h64Assign1;		
		CMgr::Log.Log(CLog::keLog_Game, "Copy64:    Value 1        0x%016llX ", h64Assign1);
		CMgr::Log.Log(CLog::keLog_Game, "           Value 2        0x%016llX ", h64Assign2);
		CMgr::Log.Log(CLog::keLog_Game, "           Value 3        0x%016llX (Valid: %s)", h64Assign3, zenConst::kzFalseTrue[h64Assign1==h64Assign2 && h64Assign3==h64Assign1] );
		//-------------------------------------------------
		// Resuming string hashing
		zenHash64 h64Once("TestResume"); 
		zenHash64 h64Partial("Test"); 
		zenHash64 h64Resumed; 
		h64Resumed = h64Partial;
		h64Resumed.Append("Resume");
		CMgr::Log.Log(CLog::keLog_Game, "Resume64:  Once           0x%016llX", h64Once);
		CMgr::Log.Log(CLog::keLog_Game, "           Partial        0x%016llX", h64Partial );
		CMgr::Log.Log(CLog::keLog_Game, "           Resumed        0x%016llX (Valid: %s)", h64Resumed, zenConst::kzFalseTrue[h64Once==h64Resumed] );
	}

}