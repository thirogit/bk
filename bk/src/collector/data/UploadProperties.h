#ifndef __UPLOADPROPERTIES_H__
#define __UPLOADPROPERTIES_H__

#include <stdint.h>
#include <string>
#include <casajson/json.h>
#include <types\DateTime.h>
#include <better-enums\enum.h>

BETTER_ENUM(UploadStatus, uint8_t, completed, inprogress, none)


struct UploadProperties
{
	UploadProperties();

	
	uint32_t herdIndex;
	std::wstring userName;
	DateTime uploadDtTm;
	UploadStatus status;

	json::value ToJson() const;

	static UploadProperties FromJson(json::value jsonobj);

};

#endif
