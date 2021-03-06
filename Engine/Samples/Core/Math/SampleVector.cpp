#include "zenEngine.h"
#include <iostream> //for cout

//=================================================================================================
//! @example SampleSimd.cpp 
//! Tests and metrics on simd maths
//=================================================================================================

namespace sample
{

void TestSimd(zU32 auCount)
{
	std::cout << "\n" << zenConst::kzLineA40 << "\nLoop : " << auCount;
	zU64 uStartA, uStartB;
	zU64 uStopA, uStopB;

	float ZENAlign(uVal1[4], 16)				= {0.0f,1.0f,2.0f,3.0f};
	float ZENAlign(uVal2[4],	16)				= {10.0f,10.0f,10.0f,10.0f};
	float ZENAlign(uVal3[4],	16)				= {1.0f,2.0f,3.0f,4.0f};
	float ZENAlign(uResult[4], 16)			= {0,0,0,0};
	float ZENAlign(uResult2[4], 16)			= {0,0,0,0};
	volatile float ZENAlign(volVal1[4],16)	= {0.0f,1.0f,2.0f,3.0f};
	volatile float ZENAlign(volVal2[4],16)	= {10.0f,10.0f,10.0f,10.0f};
	volatile float ZENAlign(volVal3[4],16)	= {1.0f,2.0f,3.0f,4.0f};

	zenMath::V4 vVal1		= zenMath::simdXYZW(uVal1);
	zenMath::V4 vVal2       = zenMath::simdXYZW(uVal2);
	zenMath::V4 vVal3       = zenMath::simdXYZW(uVal3);
	zenMath::V4 vResult     = zenMath::v4Zero;

	//---------------------------------------------------------------
	// Add
	{
		uStartA = zenSys::GetTimeUSec();
		for(zU32 i=0; i<auCount; ++i)
		{
			uResult[0] += uVal1[0];
			uResult[1] += uVal1[1];
			uResult[2] += uVal1[2];
			uResult[3] += uVal1[3];
		}
		uStopA = zenSys::GetTimeUSec();

		uStartB = zenSys::GetTimeUSec();
		for(zU32 i=0; i<auCount; ++i)
		{
			vResult = zenMath::simdAdd(vVal1, vResult);
		}
		uStopB = zenSys::GetTimeUSec();

		zenMath::simdXYZW(vResult,  uResult2);
		zenIO::Printf(zenConst::keLog_Game, "\n%15s Cpu:%5i vs Sse:%5i ", "Add", zU32(uStopA - uStartA), zU32(uStopB - uStartB), uResult[0], uResult[1], uResult[2], uResult[3], uResult2[0], uResult2[1], uResult2[2], uResult2[3] );
	}
	//---------------------------------------------------------------
	// MAdd
	{
		uStartA = zenSys::GetTimeUSec();
		for(zU32 i=0; i<auCount; ++i)
		{
			uResult[0] = (volVal1[0] * volVal2[0])+uResult[0];
			uResult[1] = (volVal1[1] * volVal2[1])+uResult[1];
			uResult[2] = (volVal1[2] * volVal2[2])+uResult[2];
			uResult[3] = (volVal1[3] * volVal2[3])+uResult[3];
		}
		uStopA = zenSys::GetTimeUSec();

		uStartB = zenSys::GetTimeUSec();
		zenMath::V4 vResult1 = zenMath::v4Zero;
		for(zU32 i=0; i<auCount; ++i)
		{
			vResult1 = zenMath::simdMAdd(vVal1, vVal2, vResult1);
		}
		uStopB = zenSys::GetTimeUSec();

		zenMath::simdXYZW(vResult1,  uResult2);
		zenIO::Printf(zenConst::keLog_Game, "\n%15s Cpu:%5i vs Sse:%5i ", "Mult+Add", zU32(uStopA - uStartA), zU32(uStopB - uStartB), uResult[0], uResult[1], uResult[2], uResult[3], uResult2[0], uResult2[1], uResult2[2], uResult2[3] );
	}
	//---------------------------------------------------------------
	// Magnitude
	{
		uStartA = zenSys::GetTimeUSec();
		float fResA = 0;    
		for(zU32 i=0; i<auCount; ++i)
		{
			fResA += sqrt( volVal1[0]*volVal1[0] + volVal1[1]*volVal1[1] + volVal1[2]*volVal1[2] + volVal1[3]*volVal1[3] );
			volVal1[0] += volVal2[0];
			volVal1[1] += volVal2[1];
			volVal1[2] += volVal2[2];
		}
		uStopA = zenSys::GetTimeUSec();

		uStartB = zenSys::GetTimeUSec();
		float fResB = 0;
		for(zU32 i=0; i<auCount; ++i)
		{
			vResult = zenMath::simdLengthV(vVal1);
			vVal1 = zenMath::simdAdd(vVal1, vVal2);
		} 
		zenMath::simdXYZW(vResult, uResult);
		uStopB = zenSys::GetTimeUSec();
		zenIO::Printf(zenConst::keLog_Game, "\n%15s Cpu:%5i vs Sse:%5i ", "Magnitude", zU32(uStopA - uStartA), zU32(uStopB - uStartB), fResA, uResult );
	}
	//---------------------------------------------------------------
	// Normalize
	{
		uStartA = zenSys::GetTimeUSec();
		float fResA = 0;
		uResult[0] = volVal3[0];    
		uResult[1] = volVal3[1];    
		uResult[2] = volVal3[2];    
		uResult[3] = volVal3[3];
		for(zU32 i=0; i<auCount; ++i)
		{
			fResA = 1.0f/sqrt( uResult[0]*uResult[0] + uResult[1]*uResult[1] + uResult[2]*uResult[2] + uResult[3]*uResult[3] );
			uResult[0] *= fResA;
			uResult[1] *= fResA;
			uResult[2] *= fResA;
			uResult[3] *= fResA;
		}
		uStopA = zenSys::GetTimeUSec();

		uStartB = zenSys::GetTimeUSec();
		vResult = vVal3;
		for(zU32 i=0; i<auCount; ++i)
		{
			vResult = zenMath::simdNormalize(vResult);
		}
		uStopB = zenSys::GetTimeUSec();

		zenMath::simdXYZW(vResult, uResult2);
		zenIO::Printf(zenConst::keLog_Game, "\n%15s Cpu:%5i vs Sse:%5i ", "Normalize", zU32(uStopA - uStartA), zU32(uStopB - uStartB), uResult[0], uResult[1], uResult[2], uResult[3], uResult2[0], uResult2[1], uResult2[2], uResult2[3] );
	}
}

void SampleVector()
{
	TestSimd(1000);
	TestSimd(1000000);

	float ZENAlign(uVal1[4], 16)  = {0.0f,1.0f,2.0f,3.0f};

	zenMath::V4 vTestRes;
	zenMath::V4 vTestVal		= zenMath::simdXYZW(1, 2, 3, 4);
	zenMath::Matrix matTest1	= zenMath::Matrix::sIdentity;
	zenMath::Matrix matTest2	= zenMath::Matrix::sIdentity;
	vTestRes				= vTestVal*matTest1;

	matTest1 *= matTest2;

	vTestRes = zenMath::simdXYZW(uVal1);
	vTestRes = zenMath::simdSpreadX(vTestVal);
	vTestRes = zenMath::simdSpreadY(vTestVal);
	vTestRes = zenMath::simdSpreadZ(vTestVal);
	vTestRes = zenMath::simdSpreadW(vTestVal);
	std::cout << "\n" << zenMath::simdX(vTestRes);

	const zU32 uValue=16;
	volatile zU32 uAdd(3);

	zU32 uResult32 = uAdd + uValue;
	std::cout << "\nResult : " << uResult32;

}

}