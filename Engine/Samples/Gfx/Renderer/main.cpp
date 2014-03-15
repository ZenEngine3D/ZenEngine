#include "zenEngine.h"
#include "SampleRenderer.h"

int main (int argc, char * const argv[])
{
	sample::SampleRendererInstance EngineInstance;
	zenSys::LaunchEngine( &EngineInstance, argc, argv );	
	return 0;	
}