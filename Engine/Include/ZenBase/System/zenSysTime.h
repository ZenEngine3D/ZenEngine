#pragma once
#ifndef __zenBase_System_Time_h__
#define __zenBase_System_Time_h__

namespace zen { namespace zenSys 
{
    const zenDateTime&	GetDateTime();
	zenTimeStamp			GetTimeStamp();
	zenU64				GetTimeUSec();
    double				GetElapsedUSec();	
	double				GetElapsedMSec();
	double				GetElapsedSec();
}} // namespace zen { namespace zenSys



#endif