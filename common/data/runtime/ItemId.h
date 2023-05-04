#pragma once

#include <boost/uuid/uuid.hpp>
#include <string>

typedef boost::uuids::uuid ItemId;

namespace ItemIdText
{
	std::wstring ToString(const ItemId& id);
}

