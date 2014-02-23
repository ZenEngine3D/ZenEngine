#pragma once
#ifndef __LibCore_Api_Math_Bits_h__
#define __LibCore_Api_Math_Bits_h__

namespace zen { namespace zenMath 
{
	awUInt	BitsCount( awU16 _uValue );			// Number of bits set to 1 in value
	awUInt	BitsCount( awU32 _uValue );
	awUInt	BitsCount( awU64 _uValue );
	awU8	BitsReverse( awU8 _uValue );			// Reverser bits order (0x0100 -> 0x0010)
	awU16	BitsReverse( awU16 _uValue );
	awU32	BitsReverse( awU32 _uValue );
	awU64	BitsReverse( awU64 _uValue );
	int		BitsScan( awU8 _uValue );				// Index of 1st bit set to 1 (starting from 0, <0 if none)
	int		BitsScan( awU16 _uValue );			
	int		BitsScan( awU32 _uValue );			
	int		BitsScan( awU64 _uValue );
	int		BitsScanReverse( awU8 _uValue );		// Index of 1st bit set to 1 (starting from last bits, <0 if none)
	int		BitsScanReverse( awU16 _uValue );		
	int		BitsScanReverse( awU32 _uValue );	
	int		BitsScanReverse( awU64 _uValue );

}  } // namespace zen, zenMath

//#include AWInlinePlatform( CMathBits )

#endif
