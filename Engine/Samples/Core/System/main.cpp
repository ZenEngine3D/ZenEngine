#include "zenEngine.h"

namespace sample
{
	extern void SampleThread();

	class SampleBaseThread : public zenSys::zSampleEngineInstance
	{
	public:
		virtual const char* GetAppName()const
		{
			return "Sample Thread";
		}

		virtual void Update()
		{
			SampleThread();
			SetDone();
		}
	};
}

int main(int argc, char * const argv[])
{
	sample::SampleBaseThread Sample;
	zenSys::LaunchEngine(&Sample, argc, argv);
	return 0;
}