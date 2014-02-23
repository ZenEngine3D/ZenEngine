#include "libZenEngine.h"

//=================================================================================================
//! @example SampleArray.cpp
//! Informations on the usage of various arrays functionalities
//=================================================================================================

namespace sample
{
	//==================================================================================================
	//! @brief		Test the result of static and dynamic arrays
	//==================================================================================================
	void SampleFlags()
	{	
		enum enumSample
		{
			keSample_Val1,
			keSample_Val2,
			keSample_Val3,
			keSample_Val4,
			keSample_Val5,
			keSample__Count
		};
		typedef zenFlag<enumSample, keSample__Count>	FlagSample;
		typedef zenFlag<enumSample, 10, zenU8>				FlagSampleWrong;
		
		//Test to make sure storage is enough for max value
		//FlagSampleWrong testWrongEnum; 

		//Should assert with bad value
		//FlagSample flagTestWrong(keSample__Count); 

		// Test different constructors
		
		FlagSample flagTest1(keSample_Val1);
		FlagSample flagTest2(keSample_Val1, keSample_Val2);		
		FlagSample flagTestA(flagTest1);
		FlagSample flagTestB;
		flagTestB = flagTest2;

		// Should be true
		bool anyA1 = flagTestA.Any(flagTestB);
		bool anyA2 = flagTestA.Any(keSample_Val1, keSample_Val2);
		bool anyA3 = flagTestA.Any();

		// Should be false
		bool allB1 = flagTestA.All(flagTestB);
		bool allB2 = flagTestA.All(keSample_Val1, keSample_Val2);
		
		// Set/Unset values
		flagTestA += FlagSample(keSample_Val1);
		flagTestA -= FlagSample(keSample_Val1, keSample_Val2);
		
		// Bin operation on flags
		flagTestA = FlagSample( keSample_Val1, keSample_Val2 );
		flagTestA &= FlagSample( keSample_Val2 );
		
		flagTestA = FlagSample( keSample_Val1, keSample_Val2 );
		flagTestA |= FlagSample(keSample_Val1, keSample_Val2, keSample_Val3);

		flagTestA = FlagSample( keSample_Val1, keSample_Val2 );
		flagTestA ^= FlagSample(keSample_Val1, keSample_Val2, keSample_Val3);

		flagTestA = FlagSample( keSample_Val1 );
		flagTestA.Invert();

		// Concatenated operations
		flagTestA = FlagSample( keSample_Val1, keSample_Val2 ) | FlagSample( keSample_Val3 );
		flagTestA = FlagSample( keSample_Val1, keSample_Val2 ) & FlagSample( keSample_Val1 );
		flagTestA = FlagSample( keSample_Val1, keSample_Val2 ) ^ FlagSample( keSample_Val1, keSample_Val3 );
	}
}