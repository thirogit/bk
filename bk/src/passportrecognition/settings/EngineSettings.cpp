#include "stdafx.h"
#include "EngineSettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


EngineSettings::EngineSettings()
{

}
	
const std::wstring& EngineSettings::GetEnginePath() const
{
	return m_EnginePath;
}

const std::wstring& EngineSettings::GetLicenseFilePath() const
{
	return m_LicenseFilePath;
}

const std::wstring& EngineSettings::GetDeveloperSN() const
{
	return m_DeveloperSN;
}

const std::wstring& EngineSettings::GetProjectId() const
{
	return m_ProjectId;
}

void EngineSettings::SetEnginePath(const std::wstring& sEnginePath)
{
	m_EnginePath = sEnginePath;
}

void EngineSettings::SetLicenseFilePath(const std::wstring& sLicenseFilePath)
{
	m_LicenseFilePath = sLicenseFilePath;
}

void EngineSettings::SetDeveloperSN(const std::wstring& sDeveloperSN)
{
	m_DeveloperSN = sDeveloperSN;
}

void EngineSettings::SetProjectId(const std::wstring& sProjectId)
{
	m_ProjectId  = sProjectId;
}

