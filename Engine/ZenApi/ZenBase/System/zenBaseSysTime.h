#pragma once

namespace zen { namespace zenSys 
{
    const zDateTime&	GetDateTime();
	zTimeStamp			GetTimeStamp();
	zU64				GetTimeUSec();
    double				GetElapsedUSec();	
	double				GetElapsedMSec();
	double				GetElapsedSec();
}} // namespace zen { namespace zenSys

