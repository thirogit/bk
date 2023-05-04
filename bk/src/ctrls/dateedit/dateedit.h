#ifndef __DATEEDIT_H__
#define __DATEEDIT_H__

#include <afxwin.h>
#include <types/DateTime.h>
#include "../maskededit/OXMaskedEdit.h"
#include <boost/optional.hpp>
#include "../tipedit/TipEdit.h"

class CDateEdit : public  CTipEditWrapper<COXMaskedEdit>
{
public:
	CDateEdit();
	void SetNowDate();
	void SetDate(const DateTime &date);
	DateTime GetDate() const;
	bool IsDateValid();
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
private:
	virtual void PreSubclassWindow();
	static const wchar_t m_DateMask[]; 	
protected:
	virtual BOOL OnValidate();

	bool m_bDynCreate;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};


#endif