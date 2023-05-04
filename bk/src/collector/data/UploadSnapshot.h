#ifndef __UPLOADSNAPSHOT_H__
#define __UPLOADSNAPSHOT_H__

#include <stdint.h>
#include <string>
#include <casajson/json.h>
#include <vector>

#include "UploadCow.h"
#include "UploadCowClass.h"
#include "UploadHent.h"

struct UploadSnapshot
{
	std::vector<UploadCowClass*> classes;
	std::vector<UploadHent*> hents;
	std::vector<UploadCow*> cows;
	uint32_t herdIndex;

	json::value ToJson() const;
};


#endif
