#pragma once
#ifndef __zenBase_Type_Time_h__
#define __zenBase_Type_Time_h__

namespace zen { namespace zenType {

//=================================================================================================
//! Used to store informations about the OS current date/time
//=================================================================================================
struct zenDateTime
{   
    zenU16 muYear;
    zenU16 muDayOfYear;
    zenU8  muDayOfMonth;
    zenU8  muDayOfWeek;    
    zenU8  muMonth;
    zenU8  muHour;        
    zenU8  muMinute;
    zenU8  muSecond;
};

typedef time_t zenTimeStamp;

}}  //namespace zen, Type 

#endif
