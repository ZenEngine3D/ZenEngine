#pragma once
#ifndef __zenBase_System_Time_h__
#define __zenBase_System_Time_h__

namespace zen { namespace zenSys 
{
    const awDateTime&	GetDateTime();
	awTimeStamp			GetTimeStamp();
	awU64				GetTimeUSec();
    double				GetElapsedUSec();	
	double				GetElapsedMSec();
	double				GetElapsedSec();
}} // namespace zen { namespace zenSys



#endif