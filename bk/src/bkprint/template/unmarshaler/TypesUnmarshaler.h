#ifndef __TYPESUNMARSHALER_H__
#define __TYPESUNMARSHALER_H__

#include "../TextStyle.h"
#include "../Border.h"
#include "../../../uitypes/Color.h"
#include <pugixml.hpp>

namespace bkprint
{

	class TypesUnmarshaler
	{
	public:
		TextStyle UnmarshalTextStyle(const pugi::xml_node & node);
		Border UnmarshalBorder(const pugi::xml_node & node);
		Color UnmarshalColor(const pugi::xml_node & node);
	};
}
#endif