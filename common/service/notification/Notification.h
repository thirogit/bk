#pragma once

#include <SF/Archive.hpp>
#include "NotificationRelay.h"
#include <casajson/json.h>

namespace NotificationJsonMembers
{
	static const wchar_t* _type_attribute_name = L"_type";
}

class Notification
{
public:
	virtual Notification* Clone() = 0;
	virtual void serialize(SF::Archive &archive) = 0;
	virtual void Relay(NotificationRelay* pRelay) const = 0;
	virtual json::value ToJson() const = 0;
	virtual ~Notification() = 0;
};
