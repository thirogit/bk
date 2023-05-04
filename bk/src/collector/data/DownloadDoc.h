#ifndef __DOWNLOADDOC_H__
#define __DOWNLOADDOC_H__

#include <stdint.h>
#include <string>
#include <casajson/json.h>
#include <types\DateTime.h>
#include <data\Motive.h>
#include <boost/optional.hpp>
#include  "DownloadHent.h"

struct DownloadDoc
{
	DateTime LoadDate;
	uint32_t Id;
	Motive DocumentMotive;
	boost::optional<DownloadHent> Hent;
	std::wstring PlateNo;
	
	json::value ToJson() const;
	static DownloadDoc FromJson(const json::value& jsonDoc);
};


#endif
