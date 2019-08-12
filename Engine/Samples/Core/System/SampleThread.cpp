#include "zenEngine.h"

//=================================================================================================
//! @example SampleThread.cpp
//! Informations and test on the usage of fibers
//=================================================================================================
zUInt* gpSortOriginal	= nullptr;
zUInt* gpSortDataSingle	= nullptr;
zUInt* gpSortDataTask	= nullptr;

namespace sample
{
	zenInline zUInt* SortSplit(zUInt* _pLeft, zUInt* _pRight)
	{
		zUInt* Pivot( _pLeft );
		zenSwap(*Pivot, _pLeft[(_pRight-_pLeft)/2]);
		
		while( _pLeft < _pRight )
		{
			if( *_pLeft > *Pivot &&  *_pRight <= *Pivot )
				zenSwap(*_pLeft, *_pRight);

			_pLeft	+= (*_pLeft <= *Pivot)	? 1 : 0;
			_pRight	-= (*_pRight > *Pivot)	? 1 : 0;			
		}

		_pRight	-= (*_pRight > *Pivot)	? 1 : 0;
		zenSwap(*Pivot, *_pRight);
		return _pRight;
	}

	zenInline void Sort(zUInt* _pStart, zUInt* _pEnd)
	{
		zUInt* pSplit = SortSplit(_pStart, _pEnd);
		if( pSplit-_pStart >= 2 )
			Sort(_pStart, pSplit-1);
		if( _pEnd-pSplit >= 2 )
			Sort(pSplit+1, _pEnd);
	}

	class TaskSort : public zenSys::zTask
	{
	zenClassDeclare(TaskSort,zenSys::zTask);
	public:
		TaskSort(zUInt* _pStart, zUInt* _pEnd, zUInt _uDepth=0)
		: mpStart(_pStart)
		, mpEnd(_pEnd)
		, muDepth(_uDepth)
		{
		}

		virtual void TaskSort::Run()
		{
			zU64 uSortStartTime = zenSys::GetTimeUSec();
			zI32 iEntryCount	= zI32(mpEnd-mpStart+1);
			if( (zUInt(1)<<muDepth) <= zTask::GetCPUCoreCount() && iEntryCount > 2 )
			{			
				zUInt* pSplit = SortSplit(mpStart,	mpEnd);
				TaskSort SubTask[2]={ {mpStart,pSplit-1,muDepth+1}, {pSplit+1, mpEnd,muDepth+1} };
				if( pSplit-mpStart >= 2 )
					SubTask[0].AddToPending(&mGate);
				if( mpEnd-pSplit >= 2 )
					SubTask[1].AddToPending(&mGate);
				Suspend();
			}
			else
			{
				Sort(mpStart, mpEnd);
			}

			if( muDepth == 0 )
			{			
				zU32 uElapsedUs = zU32(zenSys::GetTimeUSec() - uSortStartTime);	
				int bSortOk(true);
				for(int idx(0); idx<iEntryCount-1 && bSortOk; ++idx)
					bSortOk = mpStart[idx] <= mpStart[idx+1];
				zenIO::Printf(zenConst::keLog_Game, "\n    Task Threads  (Valid %i) : %6.02fms", bSortOk, float(uElapsedUs) / 1000.0f);
			}
		}

	protected:
		zUInt* mpStart;
		zUInt* mpEnd;
		zUInt muDepth;		
	};

	//================================================================================================
	
	void SampleThread()
	{	
		const zI32 iEntryCount	= 1024*1024*2;
		gpSortOriginal			= zenMem::NewArray<zUInt>(iEntryCount);
		gpSortDataSingle		= zenMem::NewArray<zUInt>(iEntryCount);
		gpSortDataTask			= zenMem::NewArray<zUInt>(iEntryCount);
		for(zUInt idx(0); idx<iEntryCount; ++idx)
		{
			gpSortOriginal[idx]		= rand();
			gpSortDataSingle[idx]	= gpSortOriginal[idx];
			gpSortDataTask[idx]		= gpSortOriginal[idx];
		} 

		zenIO::Printf(zenConst::keLog_Game, "\nSorting %i Entries. %i Core(s) supported", iEntryCount, zenSys::zTask::GetCPUCoreCount());		

		zU64 uSortSingleTime = zenSys::GetTimeUSec();
		Sort(gpSortDataSingle, gpSortDataSingle+iEntryCount-1);
		uSortSingleTime = zenSys::GetTimeUSec() - uSortSingleTime;	
		int bSortOk(true);
		for(int idx(0); idx<iEntryCount-1 && bSortOk; ++idx)
			bSortOk = gpSortDataSingle[idx] <= gpSortDataSingle[idx+1];
		zenIO::Printf(zenConst::keLog_Game, "\n    Single Thread (Valid %i) : %6.02fms", bSortOk, float(uSortSingleTime) / 1000.0f);		

		TaskSort TaskSorting(gpSortDataTask, gpSortDataTask+iEntryCount-1);
		TaskSorting.AddToPending();
		zenSys::StartTaskProcessing();
				
		zenMem::Del(gpSortOriginal);
		zenMem::Del(gpSortDataSingle);
		zenMem::Del(gpSortDataTask);
	}

}