#pragma once
#ifndef __LibFramework_Api_Type_Render2D_inl__
#define __LibFramework_Api_Type_Render2D_inl__

namespace zen { namespace zenType {

	zenRender2D::zenRender2D(void)
		: mbInit(false)
	{

	}

	zenRender2D::~zenRender2D(void)
	{
		if(mbInit)
		{
			Shutdown();
			mbInit = false;
		}
	}

}}  //namespace zen, Type 

#endif
