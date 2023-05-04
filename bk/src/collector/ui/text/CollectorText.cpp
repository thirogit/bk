#include "stdafx.h"
#include "CollectorText.h"
#include <string/TextUtils.h>
#include "CollectorStatusString.h"

std::wstring CollectorText::Text(const ICollector *pCollector, uint32_t memberId)
{
	std::wstring text;
   
	switch(memberId)
	{
	
	case CollectorMember::DeviceName:
		text = pCollector->GetDeviceName();
		break;

	case CollectorMember::ClientVersion:
		text = pCollector->GetClientVersion();
		break;
	
	case CollectorMember::OsVersion:
		text = pCollector->GetOS();
		break;
	case CollectorMember::IpAddress:
		text = TextUtils::FromUTF8(pCollector->GetEndpoint().address().to_v4().to_string());
		break;
	case CollectorMember::Status:
		text = CollectorStatusString(pCollector->GetStatus()).ToString();
		break;
	default:
		ASSERT(0);
		break;
	}
	return text;
}
