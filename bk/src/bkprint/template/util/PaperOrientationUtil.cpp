#include "stdafx.h"
#include "PaperOrientationUtil.h"
#include <map>
#include <boost/bimap.hpp>
#include <boost/assign/list_of.hpp>

namespace bkprint
{

	namespace PaperOrientationUtil
	{

		typedef boost::bimaps::bimap< PaperOrientation, std::wstring> map_type;

	
		static map_type m = boost::assign::list_of< map_type::relation >(PaperOrientation_Landscape, L"landscape")
			                                          (PaperOrientation_Portrait, L"portrait");

		std::wstring ToString(PaperOrientation orientation)
		{
			auto it = m.left.find(orientation);
			if (it != m.left.end())
			{
				return it->second;
			}
			return L"";			
		}

		PaperOrientation FromString(const std::wstring& orientation)
		{
			
			auto it = m.right.find(orientation);
			if (it != m.right.end())
			{
				return it->second;
			}	

			return PaperOrientation_None;
			
		}
	}
}