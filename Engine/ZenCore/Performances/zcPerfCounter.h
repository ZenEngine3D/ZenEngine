#pragma once
#ifndef __LibZenEngine_Perf_Counter_h__
#define __LibZenEngine_Perf_Counter_h__

namespace zcPerf 
{

class zeEventBase : public zRefCounted
{
ZENClassDeclare(zeEventBase, zRefCounted)
protected:	
	zListLink				mlnkChild;
public:		
	typedef zList<zeEventBase, &zeEventBase::mlnkChild> TypeListChild;
	
							zeEventBase(const zStringHash32& _zEventName);
	virtual					~zeEventBase();
	virtual void 			Start() = 0;
	virtual void 			Stop() = 0;
	void					AddChild(zeEventBase& _Child);
	ZENInline double		GetElapsedMs(){ return muTimeElapsed/1000.0; }
	void					ShowStats( const zEngineRef<zeEventBase>& _rParent, double _fTotalTime, zUInt& _uItemCount, zUInt _uDepth );
//! @todo Urgent add accessors
//protected:
	zStringHash32			mzEventName		= zStringHash32("Unassigned");
	zU64					muTimeStart		= 0;
	zU64					muTimeElapsed	= 0;
	
	TypeListChild			mlstChilds;
};

}

#endif
