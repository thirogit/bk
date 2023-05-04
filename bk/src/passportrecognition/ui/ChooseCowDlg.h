#ifndef __CHOOSECOWDLG_H__INCLUDED__
#define __CHOOSECOWDLG_H__INCLUDED__

#include "../session/PassportRecognitionSession.h"
#include "../../resource.h"
#include "../../ctrls/contentlistctrl/ContentListCtrl.h"
#include "../../SessionAwareDialog.h"
#include "content/RecognitionSessionContent.h"

class CChooseCowDlg : public CSessionAwareDialog
{	
public:
	CChooseCowDlg(PassportRecognitionSession* session,CWnd* pParent = NULL);   
	virtual ~CChooseCowDlg();
	enum { IDD = IDD_CHOOSECOWDLG };
	
	ItemId ChooseCow();
private:
	RecognitionSessionContent* m_content;
	PassportRecognitionSession* m_session;
	CContentListCtrl m_cowList;
protected:
	CDialog::DoModal;
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
protected:
	
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
};

#endif