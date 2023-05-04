#include "stdafx.h"
#include "DownloadHent.h"
#include <data\henttypes\HentTypes.h>

json::value DownloadHent::ToJson() const
{
	json::value result;
	result[_U("id")] = json::value(this->Id);
	result[_U("name")] = json::value(this->Name);
	result[_U("alias")] = json::value(this->Alias);
	result[_U("street")] = json::value(this->Street);
	result[_U("pobox")] = json::value(this->POBox);
	result[_U("city")] = json::value(this->City);
	result[_U("zip")] = json::value(this->Zip);
	result[_U("hentno")] = json::value(this->HentNo);
	result[_U("plate")] = json::value(this->Plate);
	result[_U("henttype")] = json::value(HentTypes::ToString(this->Type));
			
	return result;
}

DownloadHent DownloadHent::FromJson(const json::value& jsonDoc)
{
	DownloadHent result;
	result.Id = jsonDoc[_U("id")].as_integer();
	result.Name = jsonDoc[_U("name")].as_string();
	result.Alias = jsonDoc[_U("alias")].as_string();
	result.Street = jsonDoc[_U("street")].as_string();
	result.POBox = jsonDoc[_U("pobox")].as_string();
	result.City = jsonDoc[_U("city")].as_string();
	result.Zip = jsonDoc[_U("zip")].as_string();
	result.HentNo = jsonDoc[_U("hentno")].as_string();
	result.Plate = jsonDoc[_U("plate")].as_string();
	result.Type = HentTypes::FromString(jsonDoc[_U("henttype")].as_string());

	return result;
}
