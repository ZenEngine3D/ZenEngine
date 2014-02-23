#pragma once
#ifndef __Sample_Multiplatform_Base_h__
#define __Sample_Multiplatform_Base_h__ 

class IMultiplatform
{
public:   
    IMultiplatform(int aival);
    void print();
    
protected:
    int miVal;
};

#include AWHeaderPlatform( Multiplatform )

#endif