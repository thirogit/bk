#ifndef __IIF_H__
#define __IIF_H__

#include <type_traits>

template<typename T> T& iif(bool condition,T& valIfTrue,T& valIfFalse)
{
	return condition ? valIfTrue : valIfFalse;
}

#endif