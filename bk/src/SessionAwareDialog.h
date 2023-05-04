#pragma once

#include <afxcmn.h>
#include "context\SeasonSession.h"
#include "SessionAwareWnd.h"

class CSessionAwareDialog : public CDialog
{
public:
	CSessionAwareDialog();
	explicit CSessionAwareDialog(UINT nIDTemplate, CWnd* pParentWnd);
	SeasonSession* GetSeasonSession();
protected:
	afx_msg LRESULT OnGetSeasonSession(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
