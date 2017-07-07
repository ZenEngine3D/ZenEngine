#pragma once

class SampleLib
{
public:	
	SampleLib(int aiValue );
	void Print();

	static void StaticPrint();

protected:
	int	mi_Value;
};
