#include "zcCore.h"

namespace zcMisc 
{

void ManagerUpdate::Update( zenConst::eUpdatePriority _eUpdateGroup )
{
	ZENAssert(_eUpdateGroup < zenConst::keUpdt__Count);
	zList<zenMisc::IUpdate>::Iterator pUpdateIt = mlstUpdateable[_eUpdateGroup].GetHeadIt();
}

void ManagerUpdate::Update( zenConst::eUpdatePriority _eUpdateGroupFirst, zenConst::eUpdatePriority _eUpdateGroupLast )
{
	ZENAssert(_eUpdateGroupLast < zenConst::keUpdt__Count);
	for(zInt idx=_eUpdateGroupFirst; idx<=_eUpdateGroupLast; ++idx)
	{
		zList<zenMisc::IUpdate>::Iterator pUpdateIt = mlstUpdateable[idx].GetHeadIt();
	}
}

} //namespace zenMgr {