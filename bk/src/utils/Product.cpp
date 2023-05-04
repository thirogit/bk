#include "stdafx.h"
#include "Product.h"
#include "afx.h"
#include <memory>

Product::Product()
{	
	
}

std::wstring Product::GetVerInfoLang(void *fi)
{
	struct TranslationInfo
	{
		WORD language;
		WORD codepage;
	} *translations;
	unsigned translationsCnt;

	if (!VerQueryValue(fi, TEXT("\\VarFileInfo\\Translation"),
		(LPVOID*)&translations, &translationsCnt))
	{
		return L"";
	}

	translationsCnt /= sizeof(struct TranslationInfo);
	if (translationsCnt == 0)
	{
		return L"";
	}

	// TODO: be smarter about which language to use:
	//       1. use app main thread's locale
	//       2. failing that, try language-neutral or English
	//       3. use the first one as fallback
	const size_t idx = 0;

	wchar_t lang[9];
	HRESULT hr = _snwprintf_s(lang, 9, 8,
		L"%04x%04x",
		translations[idx].language,
		translations[idx].codepage);
	if (FAILED(hr))
		return L"";

	return lang;
}

std::wstring Product::GetFileInfo(const wchar_t* key)
{
	// get the filename of the executable containing the version resource
	TCHAR szFilename[MAX_PATH + 1] = { 0 };
	if (GetModuleFileName(NULL, szFilename, MAX_PATH) == 0)
	{
		return L"";
	}

	// allocate a block of memory for the version info
	DWORD dummy;
	DWORD dwSize = GetFileVersionInfoSize(szFilename, &dummy);
	if (dwSize == 0)
	{
		return L"";
	}

	std::auto_ptr<BYTE> data(new BYTE[dwSize]);

	// load the version info
	if (!GetFileVersionInfo(szFilename, NULL, dwSize, data.get()))
	{
		return L"";
	}

	const std::wstring key_path =
		TEXT("\\StringFileInfo\\") + GetVerInfoLang(data.get()) + TEXT("\\") + key;
	//LPTSTR key_str = (LPTSTR)key.c_str(); // explicit cast to work around VC2005 bug

	TCHAR *value;
	UINT len;
	if (!VerQueryValue(data.get(), key_path.c_str(), (LPVOID*)&value, &len))
	{
		return std::wstring();
	}
		
	return value;
}


std::wstring Product::Version()
{
	return GetFileInfo(L"ProductVersion");
}

std::wstring Product::Name()
{
	return GetFileInfo(L"ProductName");
}

std::wstring Product::NameAndVersion()
{
	return Name() + L" " + Version();
}
