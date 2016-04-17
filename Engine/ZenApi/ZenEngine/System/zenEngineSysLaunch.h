#pragma once
#ifndef __zenEngine_System_Launch_h__
#define __zenEngine_System_Launch_h__

namespace zen { namespace zenWnd {class Window;} } 

namespace zen { namespace zenSys {	

//=============================================================================
//! @class	Game should derive from this class, to start the engine
//! @todo cleanup Rely on update callback instead of virtual function?
//=============================================================================
class zEngineInstance : public zenSig::zSlot
{
ZENClassDeclare(zEngineInstance, zenSig::zSlot)
public:
	virtual bool										Init();
	virtual void										Destroy();
	virtual void										Update();
	virtual bool										IsDone()=0;	
	virtual const char*									GetAppName()const=0;
	void												CreateGfxWindow(const zVec2U16& _vDim, const zVec2U16& _vPos);
	zenSig::zSignalEmitter1<zenConst::eUpdatePriority>&	GetSignalUpdate();	

protected:	
	virtual void										UIRender();
	zenRes::zGfxWindow									mrMainWindowGfx		= nullptr;
	zenWnd::Window*										mpMainWindowOS		= nullptr;
	zVec2U16											muWindowSize;
	zenSig::zSignalEmitter1<zenConst::eUpdatePriority>	msigUpdate;	
	
private:
	void												UIRenderCB();
	void												MainLoop();
	friend void											LaunchEngine(zEngineInstance* _pEngineInstance, int argc, const char* const * argv);
	
};

//! @todo clean move this to a more appropriate location
class zSampleEngineInstance : public zEngineInstance
{
ZENClassDeclare(zSampleEngineInstance, zEngineInstance)
public:
	virtual bool			IsDone();
	void					SetDone();

protected:	
	virtual void			UIRender();
	bool					mbDone				= false;
	bool					mbUIShowImGUITest	= false;
};

void						LaunchEngine(zEngineInstance* _pEngineInstance, int argc, const char* const * argv);
zEngineInstance*			GetEngineInstance();

}} // namespace zen, zenRes

#endif
