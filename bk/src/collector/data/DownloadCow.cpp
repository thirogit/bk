#include "stdafx.h"
#include "DownloadCow.h"
#include <utils\JsonUtils.h>
#include <data\motives\Motives.h>
#include <data\IIdable.h>

json::value DownloadCow::ToJson() const
{
	json::value result;
	result[_U("id")] = json::value(this->Id);
	result[_U("externalid")] = json::value(this->ExternalId);
	result[_U("cowno")] = json::value(this->CowNo);
	result[_U("sex")] = json_CowSex(this->Sex);
	result[_U("weight")] = json_Decimal(this->Weight);
	result[_U("class")] = json::value(this->CowClass);
	result[_U("ingroup")] = json_NullInt(this->InGroup);
	result[_U("outgroup")] = json_NullInt(this->OutGroup);	
	result[_U("termbuyprice")] = json_NullDecimal(this->TermBuyPrice);
	result[_U("termbuyweight")] = json_NullDecimal(this->TermBuyWeight);
	result[_U("termbuyclass")] = json_emptyToNull(this->TermBuyClass);
	result[_U("termsellprice")] = json_NullDecimal(this->TermSellPrice);
	result[_U("termsellweight")] = json_NullDecimal(this->TermSellWeight);
	result[_U("termsellclass")] = json_emptyToNull(this->TermSellClass);
	result[_U("indocid")] = this->InDocId;
	result[_U("outdocid")] = this->OutDocId;	

	return result;
}


DownloadCow DownloadCow::FromJson(const json::value& jsonCow)
{
	DownloadCow cow;
	cow.Id = jsonCow[_U("id")].as_integer();
	cow.ExternalId = json_NullInt(jsonCow[_U("externalid")]).ToInt(NULL_ID);
	cow.CowNo  = jsonCow[_U("cowno")].as_string();
	cow.Sex = json_CowSex(jsonCow[_U("sex")]);
	cow.Weight = json_Decimal(jsonCow[_U("weight")]);
	cow.CowClass = jsonCow[_U("class")].as_string();
	cow.InGroup = json_NullInt(jsonCow[_U("ingroup")]);	
	cow.OutGroup = json_NullInt(jsonCow[_U("outgroup")]);

	cow.TermBuyPrice = json_NullDecimal(jsonCow[_U("termbuyprice")]);
	cow.TermBuyWeight = json_NullDecimal(jsonCow[_U("termbuyweight")]);
	cow.TermBuyClass = json_nullToEmpty(jsonCow[_U("termbuyclass")]);
	cow.TermSellPrice = json_NullDecimal(jsonCow[_U("termsellprice")]);
	cow.TermSellWeight = json_NullDecimal(jsonCow[_U("termsellweight")]);
	cow.TermSellClass = json_nullToEmpty(jsonCow[_U("termsellclass")]);
	cow.InDocId = json_NullInt(jsonCow[_U("indocid")]).ToInt(NULL_ID);
	cow.OutDocId = json_NullInt(jsonCow[_U("outdocid")]).ToInt(NULL_ID);
	
	return cow;
}