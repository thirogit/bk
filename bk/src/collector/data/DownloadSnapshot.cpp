#include "stdafx.h"
#include "DownloadSnapshot.h"
#include <arrays\DeepDelete.h>
#include <data\IIdable.h>
#include <iterator>
#include <boost\foreach.hpp>
#include <utils\JsonUtils.h>

DownloadSnapshot::DownloadSnapshot()
{
}

DownloadSnapshot::DownloadSnapshot(const DownloadSnapshot& src)
{
	operator=(src);
}

DownloadSnapshot::DownloadSnapshot(DownloadSnapshot&& src)
{
	std::copy(src.m_InDocs.begin(), src.m_InDocs.end(), std::back_inserter(m_InDocs));
	src.m_InDocs.clear();

	std::copy(src.m_OutDocs.begin(), src.m_OutDocs.end(), std::back_inserter(m_OutDocs));
	src.m_OutDocs.clear();
	
	
	std::copy(src.m_Cows.begin(), src.m_Cows.end(), std::back_inserter(m_Cows));
	src.m_Cows.clear();

	m_herdIndex = src.m_herdIndex;
	src.m_herdIndex = 0;

	m_uploadUser = src.m_uploadUser;
	src.m_uploadUser.clear();

	m_downloadDtTm = src.m_downloadDtTm;
	src.m_downloadDtTm.MakeNull();

	m_uploadDtTm = src.m_uploadDtTm;
	src.m_uploadDtTm.MakeNull();

}

void DownloadSnapshot::CopyFrom(const DownloadSnapshot &right)
{
	this->operator=(right);
}

DownloadSnapshot::~DownloadSnapshot()
{
	Empty();
}

DownloadSnapshot& DownloadSnapshot::operator=(const DownloadSnapshot &right)
{
	Empty();

	BOOST_FOREACH(DownloadDoc* indoc, right.m_InDocs)
	{
		m_InDocs.push_back(new DownloadDoc(*indoc));
	}
	
	BOOST_FOREACH(DownloadDoc* outdoc, right.m_OutDocs)
	{
		m_OutDocs.push_back(new DownloadDoc(*outdoc));
	}

	BOOST_FOREACH(DownloadCow* cow, right.m_Cows)
	{
		m_Cows.push_back(new DownloadCow(*cow));
	}

	m_herdIndex = right.m_herdIndex;
	m_uploadUser = right.m_uploadUser;
	m_downloadDtTm = right.m_downloadDtTm;
	m_uploadDtTm = right.m_uploadDtTm;
	return *this;
}

void DownloadSnapshot::Empty()
{
	DeepDelete(m_InDocs);
	m_InDocs.clear();
	DeepDelete(m_OutDocs);
	m_OutDocs.clear();
	DeepDelete(m_Cows); 
	m_Cows.clear();

	m_herdIndex = 0;
	m_uploadUser.clear();
	m_downloadDtTm.MakeNull();
	m_uploadDtTm.MakeNull();
}

void DownloadSnapshot::AddInDoc(const DownloadDoc& indoc)
{
	m_InDocs.push_back(new DownloadDoc(indoc));
}

void DownloadSnapshot::AddOutDoc(const DownloadDoc& outdoc)
{
	m_OutDocs.push_back(new DownloadDoc(outdoc));
}

void DownloadSnapshot::AddCow(const DownloadCow& cow)
{
	m_Cows.push_back(new DownloadCow(cow));
}

void DownloadSnapshot::SetHerdIndex(uint16_t herdId)
{
	m_herdIndex = herdId;
}

void DownloadSnapshot::SetUploadUser(const std::wstring& user)
{
	m_uploadUser = user;
}

void DownloadSnapshot::SetDownloadDtTm(const DateTime& downloadDtTm)
{
	m_downloadDtTm = downloadDtTm;
}

void DownloadSnapshot::SetUploadDtTm(const DateTime& uploadDtTm)
{
	m_uploadDtTm = uploadDtTm;
}

json::value DownloadSnapshot::ToJson() const
{
	json::value result;
	result[_U("herd")] = json::value(this->m_herdIndex);
	result[_U("user")] = json::value(this->m_uploadUser);
	result[_U("downloaddttm")] = json_DateTime(this->m_downloadDtTm);
	result[_U("uploaddttm")] = json_DateTime(this->m_uploadDtTm);
	
	std::vector<json::value> jsonInDocs;
	BOOST_FOREACH(DownloadDoc* indoc, m_InDocs)
	{
		jsonInDocs.push_back(indoc->ToJson());
	}
	result[_U("indocs")] = json::value::array(jsonInDocs);

	std::vector<json::value> jsonOutDocs;
	BOOST_FOREACH(DownloadDoc* outdoc, m_OutDocs)
	{
		jsonOutDocs.push_back(outdoc->ToJson());
	}
	result[_U("outdocs")] = json::value::array(jsonOutDocs);


	std::vector<json::value> jsonCows;
	BOOST_FOREACH(DownloadCow* cow, m_Cows)
	{
		jsonCows.push_back(cow->ToJson());
	}
	result[_U("cows")] = json::value::array(jsonCows);

	return result;
}

DownloadSnapshot DownloadSnapshot::FromJson(const json::value& jsonObj)
{
	DownloadSnapshot result;

	result.m_herdIndex = jsonObj[_U("herd")].as_integer();
	result.m_uploadUser = jsonObj[_U("user")].as_string();
	result.m_downloadDtTm = json_DateTime(jsonObj[_U("downloaddttm")]);
	result.m_uploadDtTm = json_DateTime(jsonObj[_U("uploaddttm")]);


	json::array jsonInDocs = jsonObj[_U("indocs")].as_array();
	BOOST_FOREACH(json::value jsonInDoc, jsonInDocs)
	{
		result.m_InDocs.push_back(new DownloadDoc(DownloadDoc::FromJson(jsonInDoc)));
	}

	json::array jsonOutDocs = jsonObj[_U("outdocs")].as_array();
	BOOST_FOREACH(json::value jsonOutDoc, jsonOutDocs)
	{
		result.m_OutDocs.push_back(new DownloadDoc(DownloadDoc::FromJson(jsonOutDoc)));
	}

	json::array jsonCows = jsonObj[_U("cows")].as_array();
	BOOST_FOREACH(json::value jsonCow, jsonCows)
	{
		result.m_Cows.push_back(new DownloadCow(DownloadCow::FromJson(jsonCow)));
	}	

	return std::move(result);
}


uint16_t DownloadSnapshot::GetHerdIndex() const
{
	return m_herdIndex;
}

const std::wstring& DownloadSnapshot::GetUploadUser() const
{
	return m_uploadUser;
}

const DateTime& DownloadSnapshot::GetDownloadDtTm() const
{
	return m_downloadDtTm;
}

const DateTime& DownloadSnapshot::GetUploadDtTm() const
{
	return m_uploadDtTm;
}

PtrEnumerator<DownloadDoc> DownloadSnapshot::EnumInDocs()
{
	return PtrEnumerator<DownloadDoc>(m_InDocs);
}

PtrEnumerator<DownloadDoc> DownloadSnapshot::EnumOutDocs()
{
	return PtrEnumerator<DownloadDoc>(m_OutDocs);
}

PtrEnumerator<DownloadCow> DownloadSnapshot::EnumCows()
{
	return PtrEnumerator<DownloadCow>(m_Cows);
}

DownloadDocRange DownloadSnapshot::InDocs() const
{
	return DownloadDocRange(m_InDocs);
}


DownloadDocRange DownloadSnapshot::OutDocs() const
{
	return DownloadDocRange(m_OutDocs);
}


DownloadCowRange DownloadSnapshot::Cows() const
{
	return DownloadCowRange(m_Cows);
}
