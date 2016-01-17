#include "zenEngine.h"

namespace sample
{
	extern void SampleSignal();

	class SampleBaseSignal : public zenSys::zSampleEngineInstance
	{
	public:
		virtual const char* GetAppName()const
		{
			return "Sample Signal/Slot";
		}
		virtual void Update()
		{			
			SampleSignal();
			SetDone();
		}
	};

}

int main (int argc, char * const argv[])
{	
	sample::SampleBaseSignal Sample;
	zenSys::LaunchEngine(&Sample, argc,argv);
	return 0;
}