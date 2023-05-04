#include "stdafx.h"
#include "paths.h"
//#include <direct.h>
//#include <mbctype.h> // MBCS (multibyte codepage stuff)
#include <set>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp>

#if defined MSVC
#include <Shlobj.h>
#else
#include <sys/types.h>
#include <pwd.h>
#include <limits.h>
#include <unistd.h>

#define MAX_PATH PATH_MAX

#endif

namespace fs = boost::filesystem;

bool paths_DoesPathExist(const std::wstring& path)
{
	if (path.empty()) return false;
	 return fs::exists(fs::path(path));
}



/**
 * @brief Return true if path exists or if we successfully create it
 * Expand any environment strings
 * Create missing parts (as far as possible)
 */
bool paths_CreateIfNeeded(const std::wstring & path)
{
	if (path.empty()) return false;

	return fs::create_directories(fs::path(path));
}

/**
 * @brief Return specified path if it exists or we can create it, else return empty
 */
void paths_EnsurePathExist(const std::wstring& path)
{
	if(!paths_DoesPathExist(path))
		paths_CreateIfNeeded(path);	
}


std::wstring paths_GetApplicationDir()
{
		
#ifdef MSVC
	wchar_t szAppPath[MAX_PATH+1];
	::GetModuleFileName(0, szAppPath, MAX_PATH);
#else
	char szAppPath[MAX_PATH + 1];
	/* the easiest case: we are in linux */
	readlink("/proc/self/exe", szAppPath, MAX_PATH);
#endif	

	fs::path appPath(szAppPath);

	return appPath.parent_path().wstring();
}


std::wstring paths_ConcatPath(const std::wstring & path, const std::wstring & subpath)
{
	if (path.empty()) return subpath;
	if (subpath.empty()) return path;
	return (fs::path(path) / fs::path(subpath)).wstring();
}

std::wstring paths_GetHomePath()
{
#if defined MSVC

	TCHAR szPath[MAX_PATH];

	SHGetFolderPath(NULL, 
                             CSIDL_PROFILE|CSIDL_FLAG_CREATE, 
                             NULL, 
                             0, 
                             szPath);

	return szPath;
#else
	std::string path;
	struct passwd* pwd = getpwuid(getuid());
	if (pwd)
		path = pwd->pw_dir;
	else
	{
		pwd = getpwuid(geteuid());
		if (pwd)
			path = pwd->pw_dir;
		else
		{
			const char* val = getenv("HOME");
			path = 	val;
		}
	}
	
	return fs::path(path).wstring();
#endif
}

std::wstring paths_RemoveInvalidCharacters(const std::wstring& fname)
{
	std::wstring result;
	static std::set<wchar_t> invalid_characters = { 
		'/',// (forward slash)
		'<',// (less than)
		'>',// (greater than)
		':',// (colon)
		'"',// (double quote)
		'/',// (forward slash)
		'\\',// (backslash)
		'|',// (vertical bar or pipe)
		'?',// (question mark)
		'*' 
	};

	std::for_each(fname.begin(), fname.end(), [&result](wchar_t c){
		if (invalid_characters.find(c) == invalid_characters.end())
		{
			result += c;
		}
	});

	return result;
}

#if 0

bool paths_IsDots(LPCSTR str) 
{
	if(_tcscmp(str,".") && _tcscmp(str,"..")) return false;
	return true;
}
#endif
