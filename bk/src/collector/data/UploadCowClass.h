#ifndef __UPLOADCOWCLASS_H__
#define __UPLOADCOWCLASS_H__

#include <string>
#include <casajson/json.h>
#include <data\types\CowSex.h>

struct UploadCowClass
{
	std::wstring Code;
	CowSex Sex;

	json::value ToJson() const;
};





#endif
