#ifndef __LOADFROMDIBOPERATION_H__
#define __LOADFROMDIBOPERATION_H__

#include "PassportImageOperation.h"

class LoadFromDIBOperation : public PassportImageOperation
{
public:
	LoadFromDIBOperation(HGLOBAL hDib, UINT resolution);
	virtual void DoOperation(PassportImageObject* passportImgObj);
private:
	HGLOBAL m_hDib;
	UINT m_resolution;
};


#endif