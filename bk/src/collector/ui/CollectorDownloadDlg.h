#ifndef __COLLECTORDOWNLOADDLG_H__
#define __COLLECTORDOWNLOADDLG_H__

#include <afxwin.h>
#include "../../resource.h"
#include "../../SessionAwareDialog.h"
#include "../../ui/lazydlg/LazyDlg.h"
#include "../../ctrls/tipwnd/TimerTipWnd.h"
#include "../../ctrls/vnavigation/VNavigation.h"
#include "../data/DownloadSnapshot.h"

class CollectorDownloadDlg : public CLazyDlg<CSessionAwareDialog>
{
public:
	CollectorDownloadDlg(const DownloadSnapshot* snapshot,CWnd* pParent);
	~CollectorDownloadDlg();
	enum { IDD = IDD_COLLECTORDOWNLOADDLG };
protected:
	
	virtual void OnAfterDlgShow();
	void ShowCollectorLostMsg();
	
private:
	DownloadSnapshot* m_Snapshot;
	CTimerTipWnd m_Msg;
	CVNavigation m_Nav;
protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    
	virtual BOOL	OnInitDialog() ;
	virtual void	OnOK();
	
	afx_msg void OnDestroy();
	
	DECLARE_MESSAGE_MAP()
};

#endif