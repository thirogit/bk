#include "stdafx.h"
#include "Motives.h"


#include <boost/assign/list_of.hpp>
#include <boost/assign/list_inserter.hpp>

#include <boost/bimap/bimap.hpp>
#include <boost/bimap/multiset_of.hpp>
#include <boost/bimap/list_of.hpp>
#include <boost/bimap/unordered_set_of.hpp>

using namespace boost::bimaps;
using namespace boost;

namespace Motives
{

	struct motive_value {};
	struct motive_string {};	

	typedef bimap< unordered_set_of<tagged< Motive , motive_value > >, unordered_set_of< tagged<std::wstring , motive_string> > > bm_type;

	bm_type _motive_mapping = assign::list_of< bm_type::relation >
		(Motive_Birth, L"birth")
		(Motive_Buy, L"buy")
		(Motive_Import, L"import")
		(Motive_Sell, L"sell")
		(Motive_Export, L"export")
		(Motive_Death, L"death")
		(Motive_Kill, L"kill")
		(Motive_Slaugher, L"slaugher");
	
	std::wstring ToString(Motive motive)
	{
		bm_type::map_by<motive_value>::const_iterator ie
			= _motive_mapping.by<motive_value>().find(motive);

		if (ie != _motive_mapping.by<motive_value>().end())
		{
			return ie->get<motive_string>();
		}

		return L"";
	}

	Motive FromString(const std::wstring& s)
	{
		if (s.empty())
			return Motive_None;

		bm_type::map_by<motive_string>::const_iterator ie
			= _motive_mapping.by<motive_string>().find(s);

		if (ie != _motive_mapping.by<motive_string>().end())
		{
			return ie->get<motive_value>();
		}

		return Motive_None;
	}
	
	Motive_Direction GetMotiveDirection(Motive motive)
	{
		switch(motive)
		{
			case Motive_Birth:
			case Motive_Buy:
			case Motive_Import:
				return MotiveDirection_In;
			case Motive_Sell:
			case Motive_Export:
			case Motive_Death:
			case Motive_Kill:
			case Motive_Slaugher:
				return MotiveDirection_Out;
		}
		return MotiveDirection_None;
	}

	Motive_Direction GetMotiveDirection(DocType type)
	{
		switch(type)
		{
			case DocType_In:
			case DocType_Buy:
				return MotiveDirection_In;			
			case DocType_Sell:
			case DocType_Out:
				return MotiveDirection_Out;		
		}
		return MotiveDirection_None;
	}

	
}
