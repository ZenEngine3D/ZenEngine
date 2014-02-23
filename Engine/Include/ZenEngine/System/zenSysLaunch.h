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
	void				CreateGfxWindow(const awVec2U16& _vDim, const awVec2U16& _vPos);
	void				SetWindow(const awres::awGfxWindow& _rGfxWindow);
protected:
	awres::awGfxWindow	mrMainGfxWindow;
};

void					LaunchEngine(awEngineInstance* _pEngineInstance, int argc, const char* const * argv);
awEngineInstance*		GetEngineInstance();

}} // namespace zen, awres

#endif
