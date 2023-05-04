#pragma once

#include <string>
#include <types\DateTime.h>
#include <casajson/json.h>

class DownloadSnapshot;

class DownloadSnapshotHeader
{
public:
	DownloadSnapshotHeader(uint32_t id);
	void CopyFrom(const DownloadSnapshot* snapshot);
	
	
	void SetHerdIndex(uint16_t herdIndex);
	void SetUploadUser(const std::wstring& user);
	void SetDownloadDtTm(const DateTime& downloadDtTm);
	void SetUploadDtTm(const DateTime& uploadDtTm);

	uint16_t GetHerdIndex() const;
	const std::wstring& GetUploadUser() const;
	const DateTime& GetDownloadDtTm() const;
	const DateTime& GetUploadDtTm() const;
	uint32_t GetId() const;

	json::value ToJson() const;
	static DownloadSnapshotHeader FromJson(const json::value& jsonObj);
private:
	uint16_t m_herdIndex;
	std::wstring m_uploadUser;
	DateTime m_downloadDtTm;
	DateTime m_uploadDtTm;
	uint32_t m_id;

	
};

