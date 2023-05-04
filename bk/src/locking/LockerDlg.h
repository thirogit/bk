#ifndef __LOCKINGDLG_H__
#define __LOCKINGDLG_H__

#include "../resource.h"
#include <afxwin.h>
#include "LockAttempt.h"
#include "RecordLocker.h"
#include "../ctrls/layoutlistctrl/LayoutListCtrl.h"
#include "../ui/lazydlg/LazyDlg.h"

class CLockerDlg : public CLazyDlg<CDialog>, private RecordLockerObserver
{	
public:
	enum { IDD = IDD_LOCKER };	
	
	CLockerDlg(RecordLocker* locker,CWnd* pParent = NULL);
	virtual ~CLockerDlg();	
protected:
	virtual void OnAfterDlgShow();
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();

	void LockRecords();

	void OnLockAttempt(const LockAttempt* attempt);

	std::wstring GetCellText(int row,uint32_t colId);
	Color GetCellColor(int row,uint32_t colId);
	int GetCellImage(int row,uint32_t colId);
private:
	RecordLocker* m_locker;
	CLayoutListCtrl m_attemptsList;
		
private:
	afx_msg void OnTryLock();
	DECLARE_MESSAGE_MAP();
};

#endif