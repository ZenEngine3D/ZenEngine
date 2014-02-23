#pragma once
#ifndef __LibCore_Api_Math_Bits_h__
#define __LibCore_Api_Math_Bits_h__

namespace zen { namespace zenMath 
{
	zenUInt	BitsCount( zenU16 _uValue );			// Number of bits set to 1 in value
	zenUInt	BitsCount( zenU32 _uValue );
	zenUInt	BitsCount( zenU64 _uValue );
	zenU8	BitsReverse( zenU8 _uValue );			// Reverser bits order (0x0100 -> 0x0010)
	zenU16	BitsReverse( zenU16 _uValue );
	zenU32	BitsReverse( zenU32 _uValue );
	zenU64	BitsReverse( zenU64 _uValue );
	int		BitsScan( zenU8 _uValue );				// Index of 1st bit set to 1 (starting from 0, <0 if none)
	int		BitsScan( zenU16 _uValue );			
	int		BitsScan( zenU32 _uValue );			
	int		BitsScan( zenU64 _uValue );
	int		BitsScanReverse( zenU8 _uValue );		// Index of 1st bit set to 1 (starting from last bits, <0 if none)
	int		BitsScanReverse( zenU16 _uValue );		
	int		BitsScanReverse( zenU32 _uValue );	
	int		BitsScanReverse( zenU64 _uValue );

}  } // namespace zen, zenMath

//#include AWInlinePlatform( CMathBits )

#endif
