#include "libZenEngine.h"
#include <Engine/libZenBase/libZenBase.h>

//=================================================================================================
//! @example SampleThread.cpp
//! Informations on the usage of how to use AWThreads
//=================================================================================================


// main.cpp    
#include <iostream>
#include <string>

namespace sample
{
	class SomeSampleClass
	{
	public:
		zU32 print ()
		{
			std::cout << "\n" << std::endl;
			for (int i = 0; i < 1000; ++i)
			{
				std::cout << "sample thread! (" << i << ")\n" << std::endl;
			}			
			return 0;
		}
	};

	void Create_PC_Thread()
	{
		// Random object with DWORD method (void)
		SomeSampleClass myClass;
		// thread should call print method of world.
		CSys::AWThread_PC<SomeSampleClass> thread0(&myClass, &SomeSampleClass::print);

		if(thread0.Start())
		{
			std::cout << "Thread started \n" << std::endl;

			if(thread0.Start())
			{
				std::cout << "Thread started again ?\n" << std::endl;
			}
			else
			{
				std::cout << "Cannot start  the same thread again \n" << std::endl;
			}
		}
		else
		{
			std::cout << "\nCouldn't start the thread!\n" << std::endl;
		}

		if(thread0.Interrupt())
		{
			std::cout << "Thread interrupted \n" << std::endl;
		}
		else
		{
			std::cout << "Couldn't interrupt the Thread \n" << std::endl;
		}
	}

	void Create_Base_Thread()
	{
		// Random object with DWORD method (void)
		SomeSampleClass myClass;
		// thread should call print method of world.
		CSys::AWThread<SomeSampleClass> thread0(&myClass, &SomeSampleClass::print);

		if(thread0.Start())
		{
			std::cout << "Thread started \n" << std::endl;

			if(thread0.Start())
			{
				std::cout << "Thread started again ?\n" << std::endl;
			}
			else
			{
				std::cout << "Cannot start  the same thread again \n" << std::endl;
			}
		}
		else
		{
			std::cout << "\nCouldn't start the thread!\n" << std::endl;
		}

		if(thread0.Interrupt())
		{
			std::cout << "Thread interrupted \n" << std::endl;
		}
		else
		{
			std::cout << "Couldn't interrupt the Thread \n" << std::endl;
		}
	}

	void SampleThread()
	{	
		Create_PC_Thread();
		//Create_Base_Thread();

		//thread0.Join(); // wait for thread
		
		for (int i = 0; i < 1000; ++i)
		{
			std::cout << "MAIN THREAD! (" << i << ")\n" << std::endl;
		}
		
	}

}