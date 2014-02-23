#pragma once
#ifndef __Sample_SampleLib_h__
#define __Sample_SampleLib_h__

class SampleLib
{
public:	
	SampleLib(int aiValue );
	void Print();

	static void StaticPrint();

protected:
	int	mi_Value;
};


#endif
