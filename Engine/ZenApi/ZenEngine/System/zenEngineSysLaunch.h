#pragma once
#ifndef __zenEngine_System_Launch_h__
#define __zenEngine_System_Launch_h__

namespace zen { namespace zenSys {	

//=============================================================================
//! @class	Game should derive from this class, to start the engine
//! @todo cleanup Rely on update callback instead of virtual function?
//=============================================================================
class zEngineInstance
{
ZENClassDeclareNoParent(zEngineInstance)
public:
	virtual bool										Init();
	virtual void										Destroy();
	virtual void										Update();
	virtual bool										IsDone()=0;
	void												CreateGfxWindow(const zVec2U16& _vDim, const zVec2U16& _vPos);
	void												SetWindow(const zenRes::zGfxWindow& _rGfxWindow);
	zenSig::zSignalEmitter1<zenConst::eUpdatePriority>&	GetSignalUpdate();
	zenSig::zSignalEmitter1<zenConst::eUpdatePriority>	msigUpdate;

protected:	
	zenRes::zGfxWindow									mrMainGfxWindow;

private:
	void												MainLoop();
	friend void	LaunchEngine(zEngineInstance* _pEngineInstance, int argc, const char* const * argv);
};

//! @todo move this to a more appropriate location
class zSampleEngineInstance : public zEngineInstance
{
ZENClassDeclare(zSampleEngineInstance, zEngineInstance)
public:
							zSampleEngineInstance( void (*_pFunctionToCall)() );
	virtual bool			IsDone();
	virtual void			Update();	

protected:
	void					(*mpFunctionToCall)();
	bool					mbDone;
};

void						LaunchEngine(zEngineInstance* _pEngineInstance, int argc, const char* const * argv);
//void						SetEngine(zEngineInstance* _pEngineInstance);
zEngineInstance*			GetEngineInstance();

}} // namespace zen, zenRes

#endif
