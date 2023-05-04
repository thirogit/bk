#ifndef __ENGINESETTINGS_H__
#define __ENGINESETTINGS_H__

#include <string>

class EngineSettings
{
public:
	EngineSettings();
		
	const std::wstring& GetEnginePath() const;
	const std::wstring& GetLicenseFilePath() const;
	const std::wstring& GetDeveloperSN() const;
	const std::wstring& GetProjectId() const;
	
	void SetEnginePath(const std::wstring& sEnginePath);
	void SetLicenseFilePath(const std::wstring& sLicenseFilePath);
	void SetDeveloperSN(const std::wstring& sDeveloperSN);
	void SetProjectId(const std::wstring& sProjectId);
	

private:
	std::wstring m_EnginePath;
	std::wstring m_LicenseFilePath;
	std::wstring m_DeveloperSN;
	std::wstring m_ProjectId;
	
};

#endif