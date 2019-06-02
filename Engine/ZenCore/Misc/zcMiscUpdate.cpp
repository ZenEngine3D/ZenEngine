#include "zcCore.h"

namespace zcMgr
{
	zcMisc::ManagerUpdate Updater;
}

namespace zcMisc 
{

void ManagerUpdate::Update( zenConst::eUpdatePriority _eUpdateGroup )
{
	zenAssert(_eUpdateGroup < zenConst::keUpdt__Count);
	for( auto& Item : mlstUpdateable[_eUpdateGroup] )
		Item.Update();	
}

void ManagerUpdate::Update( zenConst::eUpdatePriority _eUpdateGroupFirst, zenConst::eUpdatePriority _eUpdateGroupLast )
{
	zenAssert(_eUpdateGroupLast < zenConst::keUpdt__Count);
	for(zInt idx=_eUpdateGroupFirst; idx<=_eUpdateGroupLast; ++idx)
		Update((zenConst::eUpdatePriority)idx);	
}

} //namespace zenMgr {