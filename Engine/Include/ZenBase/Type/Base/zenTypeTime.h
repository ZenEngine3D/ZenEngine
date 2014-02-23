#pragma once
#ifndef __LibCore_Type_Time_h__
#define __LibCore_Type_Time_h__

namespace zen { namespace awtype {

//=================================================================================================
//! Used to store informations about the OS current date/time
//=================================================================================================
struct awDateTime
{   
    awU16 muYear;
    awU16 muDayOfYear;
    awU8  muDayOfMonth;
    awU8  muDayOfWeek;    
    awU8  muMonth;
    awU8  muHour;        
    awU8  muMinute;
    awU8  muSecond;
};

typedef time_t awTimeStamp;

}}  //namespace zen, Type 

#endif
