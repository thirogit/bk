#pragma once

#include <string>

class LocalStorage
{
protected:
	std::wstring GetStorageFile(const std::wstring& file) const;
	std::wstring GetStorageDirectory() const;
	
};