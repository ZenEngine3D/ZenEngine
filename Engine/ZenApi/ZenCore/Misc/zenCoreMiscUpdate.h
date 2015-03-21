#pragma once
#ifndef __zenApiCore_Misc_Update_h__
#define __zenApiCore_Misc_Update_h__

namespace zen { 

namespace zenConst
{
	enum eUpdatePriority
	{
		keUpdt_FrameStart,
		keUpdt_FrameEnd,
		keUpdt__Count,
	};
}

namespace zenMisc {

class IUpdate
{
public:
							IUpdate();
	virtual					~IUpdate();
	virtual void			Update();
	void					Activate(zenConst::eUpdatePriority _ePriorityGroup, zUInt _uPriority);
	void					Deactivate();

	zList<IUpdate>::Link	mLink;
protected:
	zU64					mTimeLastUpdate;
	
};


}} //namespace zen { namespace zenMisc {

#endif