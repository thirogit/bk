#ifndef __FINDPASSPORTFRAMEOPERATION_H__
#define __FINDPASSPORTFRAMEOPERATION_H__

#include "PassportImageOperation.h"

class FindPassportFrameOperation : public PassportImageOperation
{
public:
	virtual void DoOperation(PassportImageObject* passportImgObj);
protected:
	void Median(fipWinImage &inImage, fipWinImage &outImage);
	void SORT_SLICE(BYTE a[]);
	void MERGE_SLICE(BYTE a[], BYTE b[], BYTE bc[]);
	BYTE CALC_MEDIAN(BYTE x[], BYTE bc[]);
};


#endif