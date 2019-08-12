#include "zenEngine.h"

namespace sample
{
	extern void SampleListIntrusive();
	extern void SampleHash();
	extern void SampleHamt();
	extern void SampleArray();
	extern void SampleFlags();
	extern void SampleTypeMisc();

	class SampleBaseType : public zenSys::zSampleEngineInstance
	{
	public:
		virtual const char* GetAppName()const
		{
			return "Sample Datatypes";
		}
		virtual void Update()
		{
			SampleFlags();			
			// SampleHash();
			SampleHamt(); 
			SampleArray();
			SampleTypeMisc();
			SampleListIntrusive();			
			SetDone();
		}
	};
}

int main(int argc, char * const argv[])
{
	sample::SampleBaseType Sample;
	zenSys::LaunchEngine(&Sample, argc, argv);
	return 0;
}