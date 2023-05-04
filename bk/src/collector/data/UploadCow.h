#ifndef __UPLOADCOW_H__
#define __UPLOADCOW_H__

#include <stdint.h>
#include <string>
#include <types\NullDecimal.h>
#include <casajson/json.h>
#include <data\types\CowSex.h>
#include <types\NullInt.h>
#include <types\DateTime.h>
#include <data\Motive.h>

struct UploadCow
{
	uint32_t Id;
	uint32_t ExternalId;
	std::wstring CowNo;
	CowSex Sex;
	Decimal Weight;
	std::wstring CowClass;
	NullInt OutGroup;
	NullInt InGroup;
	std::wstring BuyFromHentName;	
	NullDecimal TermBuyPrice;
	NullDecimal TermBuyWeight;
	std::wstring TermBuyClass;
	NullDecimal TermSellPrice;
	NullDecimal TermSellWeight;
	std::wstring TermSellClass;
	std::wstring PlusDocId;
	std::wstring PlusDocNumber;
	DateTime LoadDate;
	std::wstring DeliverHentName;
	std::wstring DeliverPlateNo;
	Motive PlusMotive;
	
	json::value ToJson() const;
	static UploadCow FromJson(const json::value& jsonCow);
};



#endif
