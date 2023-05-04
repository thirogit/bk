#include "stdafx.h"
#include "LogoService.h"
#include <filesystem\paths.h>
#include <string\TextUtils.h>
#include <filesystem\files.h>
#include "../resource.h"

LogoService::LogoService()
{
	
}

void LogoService::Start()
{
	std::wstring appPath = paths_GetApplicationDir();
	std::wstring logoPath = paths_ConcatPath(appPath, L"LOGO.png");

	if (IsFileExists(logoPath))
	{
		m_logo.m_logoImgPtr.reset(new fipImage());
		m_logo.m_logoImgPtr->load(TextUtils::ToUTF8(logoPath).c_str());

		if (m_logo.m_logoImgPtr->getWidth() > 200 || m_logo.m_logoImgPtr->getHeight() > 200)
		{
			LoadInvalidLogo();
		}
	}
	else
	{
		LoadDefaultLogo();
	}
}

void LogoService::LoadDefaultLogo()
{
	LoadLogoFromResources(IDPNG_LOGO);
}

void LogoService::LoadInvalidLogo()
{
	LoadLogoFromResources(IDPNG_INVALIDLOGO);
}

void LogoService::LoadLogoFromResources(int resourceId)
{
	HMODULE hInstance = AfxGetInstanceHandle();
	HRSRC logoResource = ::FindResource(hInstance, MAKEINTRESOURCE(resourceId), L"PNG");
	if (logoResource != NULL)
	{
		unsigned int logoResourceSize = ::SizeofResource(hInstance, logoResource);
		HGLOBAL hLogoResource = ::LoadResource(hInstance, logoResource);
		BYTE* pLogo = (BYTE*)::LockResource(hLogoResource);
		m_logo.m_logoImgPtr.reset(new fipImage());
		fipMemoryIO memIO(pLogo, logoResourceSize);
		m_logo.m_logoImgPtr->loadFromMemory(memIO);

		::UnlockResource(hLogoResource);
	}
}

void LogoService::Stop()
{
	
}

void LogoService::Init(ServiceRegistry* registry)
{
	
}

const Logo& LogoService::GetLogo() const
{
	return m_logo;
};

