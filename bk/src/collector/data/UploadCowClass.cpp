#include "stdafx.h"
#include "UploadCowClass.h"
#include <utils\JsonUtils.h>

json::value UploadCowClass::ToJson() const
{
	json::value result;
	result[_U("code")] = json::value(this->Code);
	result[_U("sex")] = json_CowSex(this->Sex);

	return result;
}


