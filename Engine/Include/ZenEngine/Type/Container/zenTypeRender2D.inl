#pragma once
#ifndef __LibFramework_Api_Type_Render2D_inl__
#define __LibFramework_Api_Type_Render2D_inl__

namespace zen { namespace awtype {

	AWRender2D::AWRender2D(void)
		: mbInit(false)
	{

	}

	AWRender2D::~AWRender2D(void)
	{
		if(mbInit)
		{
			Shutdown();
			mbInit = false;
		}
	}

}}  //namespace zen, Type 

#endif
