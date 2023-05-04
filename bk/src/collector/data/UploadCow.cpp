#include "stdafx.h"
#include "UploadCow.h"
#include <utils\JsonUtils.h>
#include <data\motives\Motives.h>
#include <data\IIdable.h>

json::value UploadCow::ToJson() const
{
	json::value result;
	result[_U("id")] = json::value(this->Id);
	result[_U("externalid")] = json::value(this->ExternalId);
	result[_U("cowno")] = json::value(this->CowNo);
	result[_U("sex")] = json_CowSex(this->Sex);
	result[_U("weight")] = json_Decimal(this->Weight);
	result[_U("class")] = json::value(this->CowClass);
	result[_U("ingroup")] = json_emptyToNull(this->InGroup.ToString());
	result[_U("buyfromhent")] = json_emptyToNull(this->BuyFromHentName);
	result[_U("termbuyprice")] = json_NullDecimal(this->TermBuyPrice);
	result[_U("termbuyweight")] = json_NullDecimal(this->TermBuyWeight);
	result[_U("termbuyclass")] = json_emptyToNull(this->TermBuyClass);
	result[_U("termsellprice")] = json_NullDecimal(this->TermSellPrice);
	result[_U("termsellweight")] = json_NullDecimal(this->TermSellWeight);
	result[_U("termsellclass")] = json_emptyToNull(this->TermSellClass);
	result[_U("plusdocid")] = json_emptyToNull(this->PlusDocId);
	result[_U("plusdocnum")] = json_emptyToNull(this->PlusDocNumber);
	result[_U("loaddt")] = json_DayDate(this->LoadDate);
	result[_U("deliverhent")] = json::value(this->DeliverHentName);
	result[_U("plateno")] = json_emptyToNull(this->DeliverPlateNo);
	result[_U("motive")] = json_emptyToNull(Motives::ToString(this->PlusMotive));			
	return result;
}


UploadCow UploadCow::FromJson(const json::value& jsonCow)
{
	UploadCow cow;
	cow.Id = jsonCow[_U("id")].as_integer();
	cow.ExternalId = json_NullInt(jsonCow[_U("externalid")]).ToInt(NULL_ID);
	cow.CowNo  = jsonCow[_U("cowno")].as_string();
	cow.Sex = json_CowSex(jsonCow[_U("sex")]);
	cow.Weight = json_Decimal(jsonCow[_U("weight")]);
	cow.CowClass = jsonCow[_U("class")].as_string();
	cow.InGroup = json_NullInt(jsonCow[_U("ingroup")]);
	cow.BuyFromHentName = json_nullToEmpty(jsonCow[_U("buyfromhent")]);
	cow.TermBuyPrice = json_NullDecimal(jsonCow[_U("termbuyprice")]);
	cow.TermBuyWeight = json_NullDecimal(jsonCow[_U("termbuyweight")]);
	cow.TermBuyClass = json_nullToEmpty(jsonCow[_U("termbuyclass")]);
	cow.TermSellPrice = json_NullDecimal(jsonCow[_U("termsellprice")]);
	cow.TermSellWeight = json_NullDecimal(jsonCow[_U("termsellweight")]);
	cow.TermSellClass = json_nullToEmpty(jsonCow[_U("termsellclass")]);
	cow.PlusDocId = json_nullToEmpty(jsonCow[_U("plusdocid")]);
	cow.PlusDocNumber = json_nullToEmpty(jsonCow[_U("plusdocnum")]);
	cow.LoadDate = json_DayDate(jsonCow[_U("loaddt")]);
	cow.DeliverHentName = json_nullToEmpty(jsonCow[_U("deliverhent")]) ;
	cow.DeliverPlateNo = json_nullToEmpty(jsonCow[_U("plateno")]);
	cow.PlusMotive = Motives::FromString(jsonCow[_U("motive")].as_string());

	return cow;
}