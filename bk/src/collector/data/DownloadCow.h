#ifndef __DOWNLOADCOW_H__
#define __DOWNLOADCOW_H__

#include <stdint.h>
#include <string>
#include <types\NullDecimal.h>
#include <casajson/json.h>
#include <data\types\CowSex.h>
#include <types\NullInt.h>
#include <types\DateTime.h>
#include <data\Motive.h>

struct DownloadCow
{
	uint32_t Id;
	uint32_t ExternalId;
	std::wstring CowNo;
	CowSex Sex;
	Decimal Weight;
	std::wstring CowClass;
	NullInt OutGroup;
	NullInt InGroup;	
	NullDecimal TermBuyPrice;
	NullDecimal TermBuyWeight;
	std::wstring TermBuyClass;
	NullDecimal TermSellPrice;
	NullDecimal TermSellWeight;
	std::wstring TermSellClass;
	uint32_t InDocId;
	uint32_t OutDocId;
	
	json::value ToJson() const;
	static DownloadCow FromJson(const json::value& jsonCow);
};



#endif
