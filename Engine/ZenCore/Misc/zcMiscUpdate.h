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
	zenMisc::IUpdate::List mlstUpdateable[zenConst::keUpdt__Count];
	friend class zenMisc::IUpdate;
};

}

namespace zcMgr
{
	extern zcMisc::ManagerUpdate Updater; //! @todo clean See about not having a 'manager' object
}

#endif
