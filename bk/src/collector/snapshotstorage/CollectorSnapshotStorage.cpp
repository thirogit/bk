#include "stdafx.h"
#include "CollectorSnapshotStorage.h"
#include <string/TextUtils.h>
#include <filesystem\paths.h>
#include <filesystem\files.h>

#include <fstream>
#include <boost/locale.hpp>
#include <boost/locale/encoding.hpp>
#include <data\IIdable.h>


static const wchar_t* HEADER_EXT = L".header";
static const wchar_t* DATA_EXT = L".data";

CollectorSnapshotStorage::CollectorSnapshotStorage() : m_generator()
{
	
}

void CollectorSnapshotStorage::SaveJson(const std::wstring& filePath, const json::value& value)
{
	std::wofstream file;
	std::auto_ptr<boost::locale::util::base_converter> cvt = boost::locale::util::create_utf8_converter();
	std::locale encodinglocale = boost::locale::util::create_codecvt(std::locale(), cvt, boost::locale::wchar_t_facet);
	file.imbue(encodinglocale);

	file.open(filePath.c_str(), std::ofstream::out | std::ofstream::binary);
	if (file.is_open())
	{
		value.serialize(file);
		file.flush();
		file.close();
	}	
}

uint32_t CollectorSnapshotStorage::Save(const DownloadSnapshot& snapshot)
{
	do
	{
		uint32_t id = m_generator();
		std::wstring filename = GetFileName(id);
			
		
		std::wstring snapshotStorageDir = GetSnapshotStorageDir();
		paths_EnsurePathExist(snapshotStorageDir);
		std::wstring snapshotHeaderFile = paths_ConcatPath(snapshotStorageDir, filename + HEADER_EXT);
		std::wstring snapshotDataFile = paths_ConcatPath(snapshotStorageDir, filename + DATA_EXT);

		if (IsFileExists(snapshotHeaderFile) || IsFileExists(snapshotDataFile))
		{
			continue;
		}

		DownloadSnapshotHeader header(id);
		header.SetDownloadDtTm(snapshot.GetDownloadDtTm());
		header.SetUploadDtTm(snapshot.GetUploadDtTm());
		header.SetHerdIndex(snapshot.GetHerdIndex());
		header.SetUploadUser(snapshot.GetUploadUser());

		json::value jsonHeader = header.ToJson();
		json::value jsonSnapshot = snapshot.ToJson();

		SaveJson(snapshotHeaderFile, jsonHeader);
		SaveJson(snapshotDataFile, jsonSnapshot);
		
		return id;

	} while (1);

	return NULL_ID;
}

std::wstring CollectorSnapshotStorage::GetFileName(uint32_t id) const
{
	return TextUtils::Format(L"%08X", id);
}

std::wstring CollectorSnapshotStorage::GetSnapshotStorageDir()
{
	std::wstring storageDir = GetStorageDirectory();
	std::wstring snapshotStorageDir = paths_ConcatPath(storageDir, L"snapshots");
	return snapshotStorageDir;
}

std::unique_ptr<DownloadSnapshotHeader> CollectorSnapshotStorage::GetHeader(const std::wstring& id)
{
	std::unique_ptr<DownloadSnapshotHeader> result;
	std::wstring snapshotStorageDir = GetSnapshotStorageDir();
	paths_EnsurePathExist(snapshotStorageDir);
	std::wstring snapshotHeaderFile = paths_ConcatPath(snapshotStorageDir, id + HEADER_EXT);

	std::wifstream file;
	std::auto_ptr<boost::locale::util::base_converter> cvt = boost::locale::util::create_utf8_converter();
	std::locale encodinglocale = boost::locale::util::create_codecvt(std::locale(), cvt, boost::locale::wchar_t_facet);
	file.imbue(encodinglocale);

	file.open(snapshotHeaderFile.c_str(), std::ofstream::in | std::ofstream::binary);
	if (file.is_open())
	{
		std::error_code error;
		json::value json = json::value::parse(file,error);

		if (!error)
		{
			result.reset(new DownloadSnapshotHeader(DownloadSnapshotHeader::FromJson(json)));
		}

		file.close();
	}

	return result;
}

std::unique_ptr<DownloadSnapshotHeader> CollectorSnapshotStorage::GetHeader(uint32_t id)
{
	return GetHeader(GetFileName(id));
}

snapshot_iterator CollectorSnapshotStorage::List()
{
	fs::path storagePath(GetSnapshotStorageDir());
	dir_filter_iter_maker di_maker(boost::wregex(L".*\\.header"));
	return snapshot_iterator(di_maker.make(storagePath));
}

void CollectorSnapshotStorage::Delete(uint32_t id)
{
	fs::path storagePath = fs::path(GetSnapshotStorageDir());
	std::wstring fileName = GetFileName(id);
	fs::remove(storagePath / fs::path(fileName + HEADER_EXT));
	fs::remove(storagePath / fs::path(fileName + DATA_EXT));

}

std::unique_ptr<DownloadSnapshot> CollectorSnapshotStorage::Load(const std::wstring& id)
{
	std::unique_ptr<DownloadSnapshot> result;
	std::wstring snapshotStorageDir = GetSnapshotStorageDir();
	paths_EnsurePathExist(snapshotStorageDir);
	std::wstring snapshotHeaderFile = paths_ConcatPath(snapshotStorageDir, id + DATA_EXT);

	std::wifstream file;
	std::auto_ptr<boost::locale::util::base_converter> cvt = boost::locale::util::create_utf8_converter();
	std::locale encodinglocale = boost::locale::util::create_codecvt(std::locale(), cvt, boost::locale::wchar_t_facet);
	file.imbue(encodinglocale);

	file.open(snapshotHeaderFile.c_str(), std::ofstream::in | std::ofstream::binary);
	if (file.is_open())
	{
		std::error_code error;
		json::value json = json::value::parse(file, error);

		if (!error)
		{
			result = std::make_unique<DownloadSnapshot>(DownloadSnapshot::FromJson(json));		
		}

		file.close();
	}

	return result;
}

std::unique_ptr<DownloadSnapshot> CollectorSnapshotStorage::Load(uint32_t id)
{
	return Load(GetFileName(id));
}