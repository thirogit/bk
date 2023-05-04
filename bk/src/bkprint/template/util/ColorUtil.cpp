#include "stdafx.h"
#include "ColorUtil.h"
#include <string\TextUtils.h>

namespace ColorUtil
{

	std::wstring ToString(const Color& color)
	{
		return TextUtils::Format(L"#%02X%02X%02X",color.R(), color.G(), color.B());
	}

	Color FromString(const std::wstring& color)
	{
		std::wstring colorvalue = color;

		if (TextUtils::startsWith(color, L"#"))
		{
			colorvalue = color.substr(1);
		}

		unsigned int red, green, blue;
		switch (colorvalue.length())
		{
		case 6:
			red = std::stoul(colorvalue.substr(0, 2), nullptr, 16);
			green = std::stoul(colorvalue.substr(2, 2), nullptr, 16);
			blue = std::stoul(colorvalue.substr(4, 2), nullptr, 16);

			break;
		case 3:
			red = std::stoul(colorvalue.substr(0, 1), nullptr, 16);
			green = std::stoul(colorvalue.substr(1, 1), nullptr, 16);
			blue = std::stoul(colorvalue.substr(2, 1), nullptr, 16);

			break;
		case 1:
			red = green = blue = std::stoul(colorvalue.substr(0, 1), nullptr, 16);

			break;
		default:
			throw new InvalidColorException(L"Invalid color: " + colorvalue);
		}

		return Color(red, green, blue);
	}
}