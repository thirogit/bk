#pragma once

template<class T>
class Predicate
{
public:
	virtual bool Test(T* object) = 0;
};
