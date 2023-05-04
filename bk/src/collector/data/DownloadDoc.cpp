#include "stdafx.h"
#include "DownloadDoc.h"
#include <utils\JsonUtils.h>
#include <data\motives\Motives.h>

json::value DownloadDoc::ToJson() const
{
	json::value result;
	result[_U("loaddt")] = json_DayDate(this->LoadDate);
	result[_U("id")] = this->Id;
	result[_U("motive")] = json_emptyToNull(Motives::ToString(this->DocumentMotive));


	json::value jsonHent = json::value::null();

	if (this->Hent.is_initialized())
	{
		jsonHent = this->ToJson();
	}

	result[_U("hent")] = jsonHent;
	
	
	result[_U("plateno")] = json_emptyToNull(this->PlateNo);

	return result;
}

DownloadDoc DownloadDoc::FromJson(const json::value& jsonDoc)
{
	DownloadDoc result;
	result.LoadDate = json_DayDate(jsonDoc[_U("loaddt")]);
	result.Id = jsonDoc[_U("id")].as_integer();
	result.DocumentMotive = Motives::FromString(jsonDoc[_U("motive")].as_string());


	json::value jsonHent = jsonDoc[_U("hent")];

	if (!jsonHent.is_null())
	{
		result.Hent = DownloadHent::FromJson(jsonHent);
	}
	result.PlateNo = jsonDoc[_U("plateno")].as_string();

	return result;
}