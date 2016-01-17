#pragma once
#ifndef __zenApi_Base_Type_Singleton_h__
#define __zenApi_Base_Type_Singleton_h__

namespace zen { namespace zenType {

template<class TClass>
class zSingleton
{
public:
	static TClass& Get()
	{
		static TClass sObject;
		return sObject;
	}

protected:
	zSingleton(){};
};

} } //namespace zen, Type

#endif
