#include "libZenEngine.h"
#include <iostream> //for cout

//=================================================================================================
//! @example SampleSimd.cpp 
//! Tests and metrics on simd maths
//=================================================================================================

namespace sample
{

void TestSimd(awU32 auCount)
{
	std::cout << "\n" << awconst::kzLineA40 << "\nLoop : " << auCount;
	awU64 uStartA, uStartB;
	awU64 uStopA, uStopB;

	float AWAlign(uVal1[4], 16)				= {0.0f,1.0f,2.0f,3.0f};
	float AWAlign(uVal2[4],	16)				= {10.0f,10.0f,10.0f,10.0f};
	float AWAlign(uVal3[4],	16)				= {1.0f,2.0f,3.0f,4.0f};
	float AWAlign(uResult[4], 16)			= {0,0,0,0};
	float AWAlign(uResult2[4], 16)			= {0,0,0,0};
	volatile float AWAlign(volVal1[4],16)	= {0.0f,1.0f,2.0f,3.0f};
	volatile float AWAlign(volVal2[4],16)	= {10.0f,10.0f,10.0f,10.0f};
	volatile float AWAlign(volVal3[4],16)	= {1.0f,2.0f,3.0f,4.0f};

	zenMath::V4 vVal1       = zenMath::XYZW(uVal1);
	zenMath::V4 vVal2       = zenMath::XYZW(uVal2);
	zenMath::V4 vVal3       = zenMath::XYZW(uVal3);
	zenMath::V4 vResult     = zenMath::v4Zero;

	//---------------------------------------------------------------
	// Add
	{
		uStartA = zenSys::GetTimeUSec();
		for(awU32 i=0; i<auCount; ++i)
		{
			uResult[0] += uVal1[0];
			uResult[1] += uVal1[1];
			uResult[2] += uVal1[2];
			uResult[3] += uVal1[3];
		}
		uStopA = zenSys::GetTimeUSec();

		uStartB = zenSys::GetTimeUSec();
		for(awU32 i=0; i<auCount; ++i)
		{
			vResult = zenMath::Add(vVal1, vResult);
		}
		uStopB = zenSys::GetTimeUSec();

		zenMath::XYZW(vResult,  uResult2);
		CMgr::Log.Printf(CLog::keLog_Game, "\n%15s Cpu:%5i vs Sse:%5i ", "Add", awU32(uStopA - uStartA), awU32(uStopB - uStartB), uResult[0], uResult[1], uResult[2], uResult[3], uResult2[0], uResult2[1], uResult2[2], uResult2[3] );
	}
	//---------------------------------------------------------------
	// MAdd
	{
		uStartA = zenSys::GetTimeUSec();
		for(awU32 i=0; i<auCount; ++i)
		{
			uResult[0] = (volVal1[0] * volVal2[0])+uResult[0];
			uResult[1] = (volVal1[1] * volVal2[1])+uResult[1];
			uResult[2] = (volVal1[2] * volVal2[2])+uResult[2];
			uResult[3] = (volVal1[3] * volVal2[3])+uResult[3];
		}
		uStopA = zenSys::GetTimeUSec();

		uStartB = zenSys::GetTimeUSec();
		zenMath::V4 vResult1 = zenMath::v4Zero;
		for(awU32 i=0; i<auCount; ++i)
		{
			vResult1 = zenMath::MAdd(vVal1, vVal2, vResult1);
		}
		uStopB = zenSys::GetTimeUSec();

		zenMath::XYZW(vResult1,  uResult2);
		CMgr::Log.Printf(CLog::keLog_Game, "\n%15s Cpu:%5i vs Sse:%5i ", "Mult+Add", awU32(uStopA - uStartA), awU32(uStopB - uStartB), uResult[0], uResult[1], uResult[2], uResult[3], uResult2[0], uResult2[1], uResult2[2], uResult2[3] );
	}
	//---------------------------------------------------------------
	// Magnitude
	{
		uStartA = zenSys::GetTimeUSec();
		float fResA = 0;    
		for(awU32 i=0; i<auCount; ++i)
		{
			fResA += sqrt( volVal1[0]*volVal1[0] + volVal1[1]*volVal1[1] + volVal1[2]*volVal1[2] + volVal1[3]*volVal1[3] );
			volVal1[0] += volVal2[0];
			volVal1[1] += volVal2[1];
			volVal1[2] += volVal2[2];
		}
		uStopA = zenSys::GetTimeUSec();

		uStartB = zenSys::GetTimeUSec();
		float fResB = 0;
		for(awU32 i=0; i<auCount; ++i)
		{
			vResult = zenMath::LengthV(vVal1);
			vVal1 = zenMath::Add(vVal1, vVal2);
		} 
		zenMath::XYZW(vResult,  uResult);
		uStopB = zenSys::GetTimeUSec();
		CMgr::Log.Printf(CLog::keLog_Game, "\n%15s Cpu:%5i vs Sse:%5i ", "Magnitude", awU32(uStopA - uStartA), awU32(uStopB - uStartB), fResA, uResult );
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
		for(awU32 i=0; i<auCount; ++i)
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
		for(awU32 i=0; i<auCount; ++i)
		{
			vResult = zenMath::Normalize(vResult);
		}
		uStopB = zenSys::GetTimeUSec();

		zenMath::XYZW(vResult,  uResult2);
		CMgr::Log.Printf(CLog::keLog_Game, "\n%15s Cpu:%5i vs Sse:%5i ", "Normalize", awU32(uStopA - uStartA), awU32(uStopB - uStartB), uResult[0], uResult[1], uResult[2], uResult[3], uResult2[0], uResult2[1], uResult2[2], uResult2[3] );
	}
}

void SampleVector()
{
	TestSimd(1000);
	TestSimd(1000000);

	float AWAlign(uVal1[4], 16)  = {0.0f,1.0f,2.0f,3.0f};

	zenMath::V4 vTestRes;
	zenMath::V4 vTestVal	= zenMath::XYZW(1,2,3,4);	
	zenMath::Matrix matTest1	= zenMath::Matrix::sIdentity;
	zenMath::Matrix matTest2	= zenMath::Matrix::sIdentity;
	vTestRes				= vTestVal*matTest1;

	matTest1 *= matTest2;

	vTestRes = zenMath::XYZW(uVal1);
	vTestRes = zenMath::SpreadX(vTestVal);
	vTestRes = zenMath::SpreadY(vTestVal);
	vTestRes = zenMath::SpreadZ(vTestVal);
	vTestRes = zenMath::SpreadW(vTestVal);    
	std::cout << "\n" << zenMath::X(vTestRes);

	const awU32 uValue=16;
	volatile awU32 uAdd(3);

	awU32 uResult32 = uAdd + uValue;
	std::cout << "\nResult : " << uResult32;

}

}