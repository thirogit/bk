#include "stdafx.h"
#include "LoadFromFileOperation.h"
#include <errortoken/ErrorToken.h>

LoadFromFileOperation::LoadFromFileOperation(const std::wstring& pathToFile, UINT resolution) : m_pathToFile(pathToFile), m_resolution(resolution)
{
}

void LoadFromFileOperation::DoOperation(PassportImageObject* pImage)
{
	if (pImage->m_RawPassportImage.loadU(m_pathToFile.c_str()))
	{
		pImage->m_dpiResolution = m_resolution;
	}
	else
	{				
		throw PassportImageOperationException(ErrorToken(L"fail_to_load_img_from_file")(L"file", m_pathToFile).str());
	}
}
