#ifndef __PRODUCT_H__
#define __PRODUCT_H__

#include <string>

class Product
{
public:
	Product();

	std::wstring Version();
	std::wstring Name();
	std::wstring NameAndVersion();
	
private:
	std::wstring GetFileInfo(const wchar_t* key);
	std::wstring GetVerInfoLang(void *fi);	
	
};

#endif