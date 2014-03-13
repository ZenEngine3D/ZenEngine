#pragma once
#ifndef __zenBase_Type_Time_h__
#define __zenBase_Type_Time_h__

namespace zen { namespace zenType {

//=================================================================================================
//! Used to store informations about the OS current date/time
//=================================================================================================
struct zDateTime
{   
    zU16 muYear;
    zU16 muDayOfYear;
    zU8  muDayOfMonth;
    zU8  muDayOfWeek;    
    zU8  muMonth;
    zU8  muHour;        
    zU8  muMinute;
    zU8  muSecond;
};

typedef time_t zTimeStamp;

}}  //namespace zen, Type 

#endif
