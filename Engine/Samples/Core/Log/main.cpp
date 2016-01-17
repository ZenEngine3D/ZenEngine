#include "zenEngine.h"

namespace sample
{
	extern void SampleLog();
	extern void SampleAssert();

	class SampleBaseLog : public zenSys::zSampleEngineInstance
	{
	public:
		virtual const char* GetAppName()const
		{
			return "Sample Logging";
		}
		virtual void Update()
		{
			SampleLog();
			SampleAssert();
			SetDone();
		}
	};

}

int main(int argc, char * const argv[])
{
	sample::SampleBaseLog Sample;
	zenSys::LaunchEngine(&Sample, argc, argv);
	return 0;
}

