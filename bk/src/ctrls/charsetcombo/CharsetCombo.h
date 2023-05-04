#ifndef __CHARSETCOMBO_H__
#define __CHARSETCOMBO_H__

#include <afxwin.h>

class CCharsetCombo : public CComboBox
{
public:
	void InitCombo(const std::wstring& charset);
	std::wstring Get();
	void SelectCharset(const std::wstring& charset);
	
};

#endif