#pragma once
#ifndef __zenApiCore_Misc_Update_h__
#define __zenApiCore_Misc_Update_h__

namespace zen { 

namespace zenConst
{
	enum eUpdatePriority
	{
		keUpdt__First,
		keUpdt_FrameStart = keUpdt__First,
		keUpdt_FrameEnd,		
		keUpdt__Count,
		keUpdt__Last = keUpdt__Count-1,
		keUpdt__Invalid,		
	};
}

namespace zenMisc {

class IUpdate
{
public:
								IUpdate();
								IUpdate(zenConst::eUpdatePriority _ePriorityGroup, zU32 _uPriority=0);
	virtual						~IUpdate();
	virtual void				Update();
	void						Activate(zenConst::eUpdatePriority _ePriorityGroup, zU32 _uPriority=0);
	void						Deactivate();
	void						Reactivate();
	
protected:
	zU64						mTimeLastUpdate;
	zenConst::eUpdatePriority	mePriorityGroup;
	zU32						muPriority;
	zListLink					mLink;

public:
	typedef zList<IUpdate, &IUpdate::mLink, true> List;
};


}} //namespace zen { namespace zenMisc {

#endif