#pragma once

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

class IUpdate : public zListItem<>
{
public:
								IUpdate();
								IUpdate(zenConst::eUpdatePriority _ePriorityGroup, zU32 _uPriority=0);
	virtual						~IUpdate();
	virtual void				Update();
	void						Activate(zenConst::eUpdatePriority _ePriorityGroup, zU32 _uPriority=0);
	void						Deactivate();
	void						Reactivate();
	zenInline bool				operator<(const IUpdate& _Cmp)const;	
protected:
	zU64						mTimeLastUpdate;
	zenConst::eUpdatePriority	mePriorityGroup;
	zU32						muPriority;
	
public:
	using List = zList<IUpdate>;
};

bool IUpdate::operator<(const IUpdate& _Cmp)const
{
	return muPriority < _Cmp.muPriority;
}

}} //namespace zen { namespace zenMisc {
