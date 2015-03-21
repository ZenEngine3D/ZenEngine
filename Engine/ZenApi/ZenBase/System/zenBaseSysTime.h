#pragma once
#ifndef __zenApi_Base_System_Time_h__
#define __zenApi_Base_System_Time_h__

namespace zen { namespace zenSys 
{
    const zDateTime&	GetDateTime();
	zTimeStamp			GetTimeStamp();
	zU64				GetTimeUSec();
    double				GetElapsedUSec();	
	double				GetElapsedMSec();
	double				GetElapsedSec();
}} // namespace zen { namespace zenSys

#endif
