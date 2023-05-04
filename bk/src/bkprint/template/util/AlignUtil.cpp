#include "stdafx.h"
#include "AlignUtil.h"
#include <boost/assign.hpp>

namespace AlignUtil
{
	std::wstring ToString(HorizontalAlignment alignment)
	{
		switch (alignment)
		{
		case HorizontalAlignment_Left:
			return L"LEFT";
		case HorizontalAlignment_Right:
			return L"RIGHT";
		case HorizontalAlignment_Center:
			return L"CENTER";
		default:
			return L"NONE";
		}
	}

	HorizontalAlignment FromStringH(const std::wstring& alignment)
	{
		std::map<std::wstring, HorizontalAlignment> stringToAlign = boost::assign::map_list_of<std::wstring, HorizontalAlignment>
			(L"LEFT", HorizontalAlignment_Left)
			(L"RIGHT", HorizontalAlignment_Right)
			(L"CENTER", HorizontalAlignment_Center);

		auto it = stringToAlign.find(alignment);
		if (stringToAlign.end() != it)
			return it->second;

		return HorizontalAlignment_None;
	}

	std::wstring ToString(VerticalAlignment alignment)
	{
		switch (alignment)
		{
		case VerticalAlignment_Top:
			return L"TOP";
		case VerticalAlignment_Bottom:
			return L"BOTTOM";
		case VerticalAlignment_Center:
			return L"CENTER";
		default:
			return L"NONE";
		}
	}

	VerticalAlignment FromStringV(const std::wstring& alignment)
	{
		std::map<std::wstring, VerticalAlignment> stringToAlign = boost::assign::map_list_of<std::wstring, VerticalAlignment>
			(L"TOP", VerticalAlignment_Top)
			(L"BOTTOM", VerticalAlignment_Bottom)
			(L"CENTER", VerticalAlignment_Center);

		auto it = stringToAlign.find(alignment);
		if (stringToAlign.end() != it)
			return it->second;

		return VerticalAlignment_None;
	}

};
