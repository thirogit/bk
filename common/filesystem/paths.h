/** 
 * @file  paths.h
 *
 * @brief Declaration file for path routines
 */
// RCS ID line follows -- this is updated by CVS
// $Id: paths.h,v 1.16 2005/08/07 11:29:33 kimmov Exp $

#ifndef paths_h_included
#define paths_h_included

#include <string>



std::wstring paths_RemoveInvalidCharacters(const std::wstring& fname);
bool paths_DoesPathExist(const std::wstring& path);
//void paths_normalize(CString & sPath);
//CString paths_GetLongPath(const CString & sPath);
std::wstring paths_GetFileName(const std::wstring & sPath);
std::wstring paths_GetFileExt(const std::wstring & sPath);
bool paths_CreateIfNeeded(const std::wstring & path);
//LPCTSTR paths_GetTempPath(int * pnerr=NULL);
//CString paths_GetTempFileName(LPCTSTR lpPathName, LPCTSTR lpPrefixString, int * pnerr=NULL);
//PATH_EXISTENCE GetPairComparability(LPCTSTR pszLeft, LPCTSTR pszRight);
//CString ExpandShortcut(const CString &inFile);
std::wstring paths_ConcatPath(const std::wstring & path, const std::wstring & subpath);
//CString paths_GetParentPath(CString path);
//CString paths_GetLastSubdir(CString path);
//BOOL paths_IsPathAbsolute(const CString &path);
void paths_EnsurePathExist(const std::wstring& path);
std::wstring paths_GetApplicationDir();
std::wstring paths_GetHomePath();
//bool paths_IsDots(LPCSTR str);

#endif // paths_h_included
