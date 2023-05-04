#include "stdafx.h"
#include "PassportImage.h"
#include "operations\PassportImageOperation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


PassportImage::PassportImage() 
{
	m_object.m_dpiResolution = 0;
	m_object.m_Skew = 0.0;
}

void PassportImage::Perform(PassportImageOperation* operation)
{
	operation->DoOperation(&m_object);
}

const fipWinImageEx& PassportImage::GetRawPassportImage() const
{
	return m_object.m_RawPassportImage;
}

const fipWinImageEx& PassportImage::GetPreparedPassportImage() const
{
	return m_object.m_PreparedPassportImage;
}

const NullInt& PassportImage::GetResolution() const
{
	return m_object.m_dpiResolution;
}

const NullDouble& PassportImage::GetSkew() const
{
	return m_object.m_Skew;
}

const NullPoint<int>& PassportImage::GetOrigin() const
{
	return m_object.m_Org;
}


