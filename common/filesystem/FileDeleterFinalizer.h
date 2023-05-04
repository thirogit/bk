#ifndef  __FILEDELETEFINALIZER_H__
#define __FILEDELETEFINALIZER_H__

#include <string>
#include <vector>

class FileDeleteFinalizer
{
public:
	FileDeleteFinalizer();
	void AddFile(const std::wstring &filePath);
	~FileDeleteFinalizer();
	void ClearNoDelete();
private:
	std::vector<std::wstring> m_files;
};



#endif