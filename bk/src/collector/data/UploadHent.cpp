#include "stdafx.h"
#include "UploadHent.h"
#include <data\henttypes\HentTypes.h>

json::value UploadHent::ToJson() const
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
