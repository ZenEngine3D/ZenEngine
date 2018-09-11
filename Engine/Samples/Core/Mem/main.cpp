#include "zenEngine.h"

namespace sample
{
	extern void SampleMalloc();

	class SampleBaseMem : public zenSys::zSampleEngineInstance
	{
	public:
		virtual const char* GetAppName()const
		{
			return "Sample Memory";
		}
		virtual void Update()
		{
			SampleMalloc();
			SetDone();
		}
	};
}

int main(int argc, char * const argv[])
{
	sample::SampleBaseMem Sample;
	zenSys::LaunchEngine(&Sample, argc, argv);
	return 0;
}