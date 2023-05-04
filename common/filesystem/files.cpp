#include "stdafx.h"
#include "files.h"
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

bool IsFileExists(const std::wstring& sFilePath)
{
	return boost::filesystem::exists(boost::filesystem::path(sFilePath));	
}