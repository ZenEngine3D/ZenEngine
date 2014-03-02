#pragma once
#ifndef __LibFramework_Api_System_Launch_h__
#define __LibFramework_Api_System_Launch_h__

namespace zen { namespace awsys {	

//=============================================================================
//! @class	Game should derive from this class, to start the engine
//=============================================================================
class awEngineInstance
{
public:
	virtual bool		Init();
	virtual void		Destroy();
	virtual void		Update();	
	virtual bool		IsDone()=0;	
	void				CreateGfxWindow(const zenVec2U16& _vDim, const zenVec2U16& _vPos);
	void				SetWindow(const zenRes::GfxWindow& _rGfxWindow);
protected:
	zenRes::GfxWindow	mrMainGfxWindow;
};

void					LaunchEngine(awEngineInstance* _pEngineInstance, int argc, const char* const * argv);
void					SetEngine(awEngineInstance* _pEngineInstance);
awEngineInstance*		GetEngineInstance();

}} // namespace zen, zenRes

#endif
