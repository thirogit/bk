#include "stdafx.h"
#include "FileDeleterFinalizer.h"
#include <boost/foreach.hpp>


FileDeleteFinalizer::FileDeleteFinalizer() 
{
}
void FileDeleteFinalizer::AddFile(const std::wstring &filePath)
{
	m_files.push_back(filePath);
}
FileDeleteFinalizer::~FileDeleteFinalizer()
{
	BOOST_FOREACH(std::wstring file, m_files)
	{
		::DeleteFile(file.c_str());
	}
	
}
void FileDeleteFinalizer::ClearNoDelete()
{
	m_files.clear();
}

