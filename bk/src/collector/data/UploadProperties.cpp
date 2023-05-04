#include "stdafx.h"
#include "UploadProperties.h"
#include <utils\JsonUtils.h>

static const wchar_t* _herd_attribute_name = L"herd";
static const wchar_t* _user_attribute_name = L"user";
static const wchar_t* _date_attribute_name = L"date";
static const wchar_t* _status_attribute_name = L"status";


UploadProperties::UploadProperties() : status(UploadStatus::none)
{

}

json::value UploadProperties::ToJson() const
{
	json::value result;
	result[_herd_attribute_name] = json::value(this->herdIndex);
	result[_user_attribute_name] = json::value(this->userName);
	result[_date_attribute_name] = json_DateTime(this->uploadDtTm);
	result[_status_attribute_name] = json::value(this->status._to_string());

	return result;
}


UploadProperties UploadProperties::FromJson(json::value jsonobj)
{
	UploadProperties result;

	result.herdIndex = jsonobj[_herd_attribute_name].as_integer();
	result.userName = jsonobj[_user_attribute_name].as_string();
	result.uploadDtTm = json_DateTime(jsonobj[_date_attribute_name]);
	result.status = UploadStatus::_from_string(jsonobj[_status_attribute_name].as_string().c_str());

	return result;
}

