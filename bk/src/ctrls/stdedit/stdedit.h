#ifndef __STDEDIT_H__
#define __STDEDIT_H__

#include <afxwin.h>
#include <string>

class CStdEdit : public CEdit
{
public:
	virtual std::wstring GetText() const;
	virtual void SetText(const std::wstring& sText);
};


void DDX_StdText(CDataExchange* pDX, int nIDC, std::wstring& value);


#endif