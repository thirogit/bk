#ifndef __QUESTIONDLG_H__
#define __QUESTIONDLG_H__

#include "QuestionDlgTemplate.h"
#include <vector>
#include <map>
#include "../../resource.h"

class QuestionDlg : public CDialog
{
public:
	QuestionDlg(const QuestionDlgTemplate* pTemplate,CWnd* pParent = NULL);
	enum { IDD = IDD_EMPTYDLG };
	virtual ~QuestionDlg();
	UINT GetAnswer();

protected:
	CSize CalcSizeOfButtonArea();
	CSize CalcSizeOfQuestionArea();

	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	
private:
	const QuestionDlgTemplate* m_pDlgTemplate;
	std::vector<CWnd*> m_DlgItems;
	std::map<UINT, UINT> m_BtnIdToAnswer;
	UINT m_Answer;
	

	afx_msg void OnDestroy();
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
};

#endif