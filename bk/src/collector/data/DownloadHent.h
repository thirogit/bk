#ifndef __DOWNLOADHENT_H__
#define __DOWNLOADHENT_H__

#include <stdint.h>
#include <string>
#include <data\HentType.h>
#include <casajson/json.h>

struct DownloadHent
{
	uint32_t Id;
	std::wstring Alias;
	std::wstring Name;
	std::wstring Zip;
	std::wstring City;
	std::wstring Street;
	std::wstring POBox;
	std::wstring HentNo;
	std::wstring Plate;
	HentType Type;

	json::value ToJson() const;
	static DownloadHent FromJson(const json::value& jsonDoc);
};

#endif
