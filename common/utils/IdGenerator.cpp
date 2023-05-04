#include "stdafx.h"
#include "IdGenerator.h"
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
//#include <boost/archive/iterators/mb_from_wchar.hpp>

std::wstring IdGenerator::Generate() const
{
	boost::uuids::uuid tokenUUID = m_Generator();
	using namespace boost::archive::iterators;
	using encoderIt = base64_from_binary<    // convert binary values to base64 characters
		transform_width<   // retrieve 6 bit integers from a sequence of 8 bit bytes
		const char *,
		6,
		8
		>
	>;

	std::wstring id(encoderIt(tokenUUID.begin()), encoderIt(tokenUUID.end()));


	//boost::archive::iterators::mb_from_wchar<std::wstring::iterator> bit(id.begin());
	//boost::archive::iterators::mb_from_wchar<std::wstring::iterator> eit(id.end());

	//std::string idd(bit, eit);
	//idd.size();

	return id;
}
