#include "stdafx.h"
#include "DownloadSnapshotHeader.h"
#include <utils\JsonUtils.h>
#include "DownloadSnapshot.h"


DownloadSnapshotHeader::DownloadSnapshotHeader(uint32_t id) : m_id(id)
{
}


void DownloadSnapshotHeader::CopyFrom(const DownloadSnapshot* snapshot)
{	
	m_herdIndex = snapshot->GetHerdIndex();	
	m_uploadUser = snapshot->GetUploadUser();
	m_downloadDtTm = snapshot->GetDownloadDtTm();
	m_uploadDtTm = snapshot->GetUploadDtTm();	
}

void DownloadSnapshotHeader::SetHerdIndex(uint16_t herdIndex) 
{
	m_herdIndex = herdIndex;
}

void DownloadSnapshotHeader::SetUploadUser(const std::wstring& user)
{
	m_uploadUser = user;
}

void DownloadSnapshotHeader::SetDownloadDtTm(const DateTime& downloadDtTm)
{
	m_downloadDtTm = downloadDtTm;
}

void DownloadSnapshotHeader::SetUploadDtTm(const DateTime& uploadDtTm)
{
	m_uploadDtTm = uploadDtTm;
}

uint16_t DownloadSnapshotHeader::GetHerdIndex() const
{
	return m_herdIndex;
}

const std::wstring& DownloadSnapshotHeader::GetUploadUser() const
{
	return m_uploadUser;
}

const DateTime& DownloadSnapshotHeader::GetDownloadDtTm() const
{
	return m_downloadDtTm;
}

const DateTime& DownloadSnapshotHeader::GetUploadDtTm() const
{
	return m_uploadDtTm;
}

uint32_t DownloadSnapshotHeader::GetId() const
{
	return m_id;
}

json::value DownloadSnapshotHeader::ToJson() const
{
	json::value result;
	result[_U("id")] = json::value(this->m_id);
	result[_U("herd")] = json::value(this->m_herdIndex);	
	result[_U("user")] = json::value(this->m_uploadUser);
	result[_U("downloaddttm")] = json_DateTime(this->m_downloadDtTm);
	result[_U("uploaddttm")] = json_DateTime(this->m_uploadDtTm);
	
	return result;
}

DownloadSnapshotHeader DownloadSnapshotHeader::FromJson(const json::value& jsonObj)
{
	DownloadSnapshotHeader result(jsonObj[_U("id")].as_integer());
	 
	result.m_herdIndex = jsonObj[_U("herd")].as_integer();
	result.m_uploadUser = jsonObj[_U("user")].as_string();
	result.m_downloadDtTm = json_DateTime(jsonObj[_U("downloaddttm")]);
	result.m_uploadDtTm = json_DateTime(jsonObj[_U("uploaddttm")]);

	return result;
}

