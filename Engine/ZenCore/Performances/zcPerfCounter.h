#pragma once

namespace zcGfx
{ 
	class CommandList;
	using CommandListRef = zEngineRef<CommandList>; 
}

namespace zcPerf 
{

using EventBaseRef = zEngineRef<class EventBase>;

class EventBase : public zRefCounted
{
zenClassDeclare(EventBase, zRefCounted)
protected:	zListLink				mlnkChild;
public:		using					TypeListChild = zList<EventBase, &EventBase::mlnkChild, false>;
public:
									EventBase(const zStringHash32& _zEventName);
	virtual							~EventBase();
	virtual void 					CPUStart(){};
	virtual void 					CPUStop(){};

	virtual void 					GPUStart(const zcGfx::CommandListRef& _rDrawlist){};
	virtual void 					GPUStop(const zcGfx::CommandListRef& _rDrawlist){};

	virtual double					GetElapsedMs();

	bool							IsActive()const;
	void							AddChild(EventBase& _Child);	
	zenInline const zStringHash32&	GetName()const;
	
	zenInline EventBaseRef			GetFirstChild()const;
	zenInline EventBaseRef			GetNext()const;
	zenInline EventBaseRef			GetPrev()const;

protected:	
	zStringHash32					mzEventName		= zStringHash32("Unassigned");
	zU64							muTimeStart		= 0;	// In microseconds (us)
	zU64							muTimeElapsed	= 0;	// In microseconds (us)
	bool							mbActive		= false;
	TypeListChild					mlstChilds;
};


const zStringHash32& EventBase::GetName()const
{
	return mzEventName;
}

EventBaseRef EventBase::GetFirstChild()const
{
	return mlstChilds.GetHead();
}

EventBaseRef EventBase::GetNext() const
{
	return zcPerf::EventBase::TypeListChild::GetNext(*this);
}

EventBaseRef EventBase::GetPrev() const
{
	return zcPerf::EventBase::TypeListChild::GetPrev(*this);
}


}
