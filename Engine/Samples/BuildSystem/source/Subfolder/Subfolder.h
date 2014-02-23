#pragma once
#ifndef __Sample_Subfolder_Base_h__
#define __Sample_Subfolder_Base_h__ 

class ISubfolder
{
public:   
    ISubfolder(int aival);
    void print();
    
protected:
    int miVal;
};

#include AWHeaderPlatform(Subfolder)

#endif