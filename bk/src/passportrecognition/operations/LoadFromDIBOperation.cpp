#include "stdafx.h"
#include "LoadFromDIBOperation.h"

LoadFromDIBOperation::LoadFromDIBOperation(HGLOBAL hDib, UINT resolution) : m_hDib(hDib), m_resolution(resolution)
{
}

void LoadFromDIBOperation::DoOperation(PassportImageObject* pImage)
{
	if (pImage->m_RawPassportImage.copyFromHDIB(m_hDib))
	{
		pImage->m_dpiResolution = m_resolution;
	}
	else
	{				
		throw PassportImageOperationException(L"fail_to_load_img_from_dib");
	}
}
