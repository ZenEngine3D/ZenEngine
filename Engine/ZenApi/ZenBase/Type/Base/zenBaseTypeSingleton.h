#pragma once

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

