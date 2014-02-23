#include "zenEngine.h"
#include "SampleRenderer.h"

int main (int argc, char * const argv[])
{
	sample::SampleRendererInstance EngineInstance;
	awsys::LaunchEngine( &EngineInstance, argc, argv );	
	return 0;	
}