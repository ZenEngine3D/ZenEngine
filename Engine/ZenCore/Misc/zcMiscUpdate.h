#pragma once
#ifndef __zCore_Misc_Update_h__
#define __zCore_Misc_Update_h__

namespace zcMisc {

class ManagerUpdate
{
public:
	void Update( zenConst::eUpdatePriority _eUpdateGroup );
	void Update( zenConst::eUpdatePriority _eUpdateGroupFirst, zenConst::eUpdatePriority _eUpdateGroupLast );
protected:
	zListDeclare(zenMisc::IUpdate, mLink) mlstUpdateable[zenConst::keUpdt__Count];
};


}

#endif
