#include "stdafx.h"
#include "TresholdOperation.h"


TresholdOperation::TresholdOperation(uint8_t level) : m_level(level)
{
}

void TresholdOperation::DoOperation(PassportImageObject* passportImgObj)
{
	if(!passportImgObj->m_PreparedPassportImage.isValid())		
		passportImgObj->m_PreparedPassportImage = passportImgObj->m_RawPassportImage;
	
	passportImgObj->m_PreparedPassportImage.convertTo8Bits();
	
	passportImgObj->m_PreparedPassportImage.adjustContrast(30);
	passportImgObj->m_PreparedPassportImage.threshold(m_level);
		
}
