#include "zenEngine.h"
#include "SampleDebugUI.h"

int main (int argc, char * const argv[])
{
	sample::SampleDebugUIInstance EngineInstance;
	zenSys::LaunchEngine( &EngineInstance, argc, argv );	
	return 0;	
}