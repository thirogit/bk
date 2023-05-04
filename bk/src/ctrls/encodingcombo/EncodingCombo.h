#ifndef __ENCODINGCOMBO_H__
#define __ENCODINGCOMBO_H__

#include <data\types\CowSex.h>
#include <afxwin.h>
#include <string>

class CEncodingCombo : public CComboBox
{

	struct EncodingItem
	{
		char* name;
		wchar_t* alias;
	};
	
	static EncodingItem _encodings[];
	

public:
	void InitCombo(const std::string& encoding);
	std::string GetEncoding();
	void SelectEncoding(const std::string& encoding);
	
};

#endif