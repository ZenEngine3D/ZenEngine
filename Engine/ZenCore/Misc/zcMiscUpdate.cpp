#include "zcCore.h"

namespace zcMgr
{
	zcMisc::ManagerUpdate Updater;
}

namespace zcMisc 
{

void ManagerUpdate::Update( zenConst::eUpdatePriority _eUpdateGroup )
{
	ZENAssert(_eUpdateGroup < zenConst::keUpdt__Count);
	auto UpdateIt = mlstUpdateable[_eUpdateGroup].GetHeadIt();
	while( UpdateIt.IsValid() )
	{
		UpdateIt->Update();
		++UpdateIt;
	}
}

void ManagerUpdate::Update( zenConst::eUpdatePriority _eUpdateGroupFirst, zenConst::eUpdatePriority _eUpdateGroupLast )
{
	ZENAssert(_eUpdateGroupLast < zenConst::keUpdt__Count);
	for(zInt idx=_eUpdateGroupFirst; idx<=_eUpdateGroupLast; ++idx)
	{
		auto UpdateIt = mlstUpdateable[idx].GetHeadIt();
		while( UpdateIt.IsValid() )
		{
			UpdateIt->Update();
			++UpdateIt;
		}
	}
}

} //namespace zenMgr {