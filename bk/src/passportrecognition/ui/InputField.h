#ifndef __INPUTFIELD_H__
#define __INPUTFIELD_H__

#include <afxwin.h>

class CInputField 
{
public:
	CInputField(CWnd* pFieldWnd)  : m_bEnabled(true),m_bInvalid(false),m_pFieldWnd(pFieldWnd)
	{
	}

	void SetInvalid(bool bInvalid)
	{
		m_bInvalid = bInvalid;
	}

	void SetEnabled(bool bEnabled)
	{
		m_bEnabled = bEnabled;
	}

	bool IsInvalid()
	{
		return m_bInvalid;
	}

	bool IsFieldEnabledForInput()
	{		
		return m_bEnabled;
	}

	CWnd* GetWnd()
	{
		return m_pFieldWnd;
	}
private:
	bool m_bEnabled;
	bool m_bInvalid;
	CWnd* m_pFieldWnd;
};

template<class CTRLCLASS>
class CInputFieldCtrl : public CInputField
{
public:
	CInputFieldCtrl() : CInputField(&m_Ctrl)
	{
	}
	CTRLCLASS& GetCtrl()
	{
		return m_Ctrl;
	}
	CTRLCLASS* operator->()
	{
		return &m_Ctrl;
	}
private:
	CTRLCLASS m_Ctrl;

};



#endif