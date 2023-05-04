#ifndef __LOADFROMFILEOPERATION_H__
#define __LOADFROMFILEOPERATION_H__

#include "PassportImageOperation.h"

class LoadFromFileOperation : public PassportImageOperation
{
public:
	LoadFromFileOperation(const std::wstring& pathToFile, UINT resolution);
	virtual void DoOperation(PassportImageObject* passportImgObj);
private:
	std::wstring m_pathToFile;
	UINT m_resolution;
};


#endif