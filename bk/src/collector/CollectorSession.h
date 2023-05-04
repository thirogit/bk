#ifndef __COLLECTORSESSION_H__
#define __COLLECTORSESSION_H__

#include "ICollector.h"
#include "data\UploadHent.h"
#include "data\UploadCow.h"
#include "data\UploadCowClass.h"
#include "data\UploadProperties.h"
#include "data\DownloadCow.h"
#include "data\DownloadDoc.h"
#include <progress/SimpleProgress.h>

class CollectorSession
{
public:
	virtual ~CollectorSession() {}
	virtual const ICollector* GetCollector() const = 0;	
	
	virtual void BeginUpload(int herdIndex,const  std::wstring& userName,SimpleProgress* progress) = 0;
	virtual void UploadHents(const std::vector<UploadHent*>& hents,SimpleProgress* progress) = 0;
	virtual void UploadClasses(const std::vector<UploadCowClass*>& classes, SimpleProgress* progress) = 0;
	virtual void UploadCows(const std::vector<UploadCow*>& cows, SimpleProgress* progress) = 0;
	virtual void EndUpload(SimpleProgress* progress) = 0;

	virtual UploadProperties BeginDownload(SimpleProgress* progress) = 0;
	virtual std::vector<DownloadCow*> DownloadCows(SimpleProgress* progress) = 0;
	virtual std::vector<DownloadDoc*> DownloadInDocs(SimpleProgress* progress) = 0;
	virtual std::vector<DownloadDoc*> DownloadOutDocs(SimpleProgress* progress) = 0;
	virtual void EndDownload(SimpleProgress* progress) = 0;
};

#endif
