#include "zenEngine.h"

namespace sample
{
	extern void SampleVector();

	class SampleBaseMath : public zenSys::zSampleEngineInstance
	{
	public:
		virtual const char* GetAppName()const
		{
			return "Sample Math";
		}
		virtual void Update()
		{
			SampleVector();
			SetDone();
		}
	};
}

int main(int argc, char * const argv[])
{
	sample::SampleBaseMath Sample;
	zenSys::LaunchEngine(&Sample, argc, argv);
	return 0;
}