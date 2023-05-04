#ifndef __WAITDIALOG_H__
#define __WAITDIALOG_H__

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/exception_ptr.hpp>
#include <string>
#include <afxwin.h>

class CWaitDialog : public CDialog
{

public:
    CWaitDialog (CWnd* pParentWnd, const std::wstring& sWaitText);
    virtual ~CWaitDialog ();
	void WaitFor(boost::function<void()> &f);

	template<typename F> void WaitForTask(F& task)
    {
		boost::function<void()> f = task;
        WaitFor(f);
    }
private:
    BOOL Pump(); 
	void Show();
	void Hide();   
private:
	CWnd* m_pParentWnd;
	std::wstring m_sWaitText;
};


#endif