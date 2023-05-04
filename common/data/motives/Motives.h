#ifndef __MOTIVES_H__
#define __MOTIVES_H__

#include "../Motive.h"
#include "../DocType.h"
#include <string>

namespace Motives
{
	Motive_Direction GetMotiveDirection(Motive motive);
	Motive_Direction GetMotiveDirection(DocType type);
	std::wstring ToString(Motive motive);
	Motive FromString(const std::wstring& s);
}
#endif