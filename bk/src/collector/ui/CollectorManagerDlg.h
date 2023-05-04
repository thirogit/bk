#ifndef __COLLECTORMANAGERDLG_H__
#define __COLLECTORMANAGERDLG_H__

#include <afxwin.h>
#include "../../resource.h"
#include "../../SessionAwareDialog.h"
#include "../CollectorServer.h"
#include "../../context/UIBridge.h"
#include "content/CollectorListContent.h"
#include "../../ctrls/xtoolbar/XToolBar.h"
#include "../../ui/lazydlg/LazyDlg.h"
#include "CollectorServerStatusBarCtrl.h"
#include "../../ctrls/tipwnd/TimerTipWnd.h"
#include "../snapshotstorage/CollectorSnapshotStorage.h"
#include "content/SnapshotListContent.h"
#include "../../ctrls/sectionheader/SectionHeader.h"

class CollectorManagerDlg : public CLazyDlg<CSessionAwareDialog>, protected CollectorServerObserver
{
public:
	CollectorManagerDlg(CWnd* pParent);
	enum { IDD = IDD_COLLECTORSELECT };
protected:
	virtual void CollectorConnected(uint32_t id);
	virtual void CollectorDisconnected(uint32_t id);
	virtual void CollectorUpdated(uint32_t id);
	virtual void OnAfterDlgShow();
	void AddItem(const ICollector* collector);
	void RemoveItem(uint32_t collectorId);
	void OnCollectorConnected(const Collector& collector);
	void OnCollectorDisconnected(uint32_t id);
	void OnCollectorUpdated(const Collector& collector);
	
	uint32_t GetSelectedCollector();
	uint32_t SelectHerd();
	uint32_t GetSelectedSnapshot();
private:
	CollectorListContent m_CollectorListContent;
	CContentListCtrl m_CollectorList;
	CContentListCtrl m_SnapshotList;
	CollectorServer* m_server;
	UIBridge m_bridge;
	XToolBar m_MainToolbar;
	XToolBar m_ExitToolbar;
	XToolBar m_BufferToolbar;
	CSectionHeader m_divider;
	CTimerTipWnd m_Msg;
	CollectorSnapshotStorage m_storage;
	SnapshotListContent m_SnapshotListContent;

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    
	virtual BOOL	OnInitDialog() ;
	virtual void	OnOK();
	void ShowCollectorLostMsg();
	void Download(boost::function<void(const DownloadSnapshot*)>& handler);
	void SaveDownload(const DownloadSnapshot* download);
	void ImportSnapshot(const DownloadSnapshot* download);
	void AddSavedSnapshot(uint32_t id);

	afx_msg void OnDestroy();
	afx_msg void OnUpload();
	afx_msg void OnDownload();	
	afx_msg void OnSaveDownload();

	afx_msg void OnImportSavedSnapshot();
	afx_msg void OnDeleteShapshot();

	
	DECLARE_MESSAGE_MAP()
};

#endif