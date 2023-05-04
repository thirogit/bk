#ifndef __NAMEDVALUE_H__
#define __NAMEDVALUE_H__

#include "../ctrls/ownclasswnd/OwnClassWnd.h"
#include "../ctrls/xcolorstatic/XColorStatic.h"

class CNamedValue : public COwnClassWnd 
{
public:
	CNamedValue();
	void Create(const std::wstring& sName,const std::wstring& sValue,const CRect& rect,CWnd* pParent,UINT nID);
	void SetValue(const std::wstring& sValue);
	void SetHeaderColor(COLORREF clr);
	void SetValueColor(COLORREF clr);
private:
	CXColorStatic m_header;
	CXColorStatic m_value;
};

#endif