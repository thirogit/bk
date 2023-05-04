#ifndef __DOCUMENTOPTIONCTRL_H__
#define __DOCUMENTOPTIONCTRL_H__

#include <afxwin.h>
#include <configuration\print\PrintOption.h>

class DocumentOptionCtrl : public CStatic
{
public:
	virtual PrintOption GetOptionValue() = 0;
	virtual void SetOptionValue(const PrintOption& option) = 0;
	void Create(const std::wstring& sOptionName,CWnd* pParent);
protected:
	const std::wstring& GetOptionCode() const;
	virtual void CreateContent()  = 0;
private:
	UINT ObtainId(CWnd* pWnd);


private:
	std::wstring m_optionName;
	
	
};


#endif

