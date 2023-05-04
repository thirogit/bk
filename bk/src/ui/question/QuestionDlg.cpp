#include "stdafx.h"
#include "QuestionDlg.h"
#include <arrays/DeepDelete.h>
#include "../../ctrls/xcolorstatic/XColorStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MESSAGE_PADDING_Y 14
#define MESSAGE_PADDING_X 14

#define BUTTON_WIDTH 80
#define BUTTON_HEIGHT 23

#define MAX_BTNS_IN_ROW 5

#define BUTTON_Y_DIVIDER_HEIGHT 5
#define BUTTON_X_DIVIDER_WIDTH 5
#define BUTTON_X_MARGIN 10
#define BUTTON_Y_MARGIN 3
		
#define CHECKBOX_CTRL_ID 10010
#define MIN_BTN_CTRL_ID 1001

#define MIN_MESSAGEAREA_HEIGHT 90
#define MIN_MESSAGEAREA_WIDTH 200

BEGIN_MESSAGE_MAP(QuestionDlg, CDialog)	
	ON_WM_DESTROY()	
	ON_WM_CLOSE()
END_MESSAGE_MAP()

QuestionDlg::QuestionDlg(const QuestionDlgTemplate* pTemplate, CWnd* pParent/* = NULL*/) : CDialog(IDD, pParent), m_pDlgTemplate(pTemplate), m_Answer(INVALID_ANSWER_ID)
{	
}

QuestionDlg::~QuestionDlg()
{
	
} 

UINT QuestionDlg::GetAnswer()
{
	return m_Answer;
}

CSize QuestionDlg::CalcSizeOfButtonArea()
{
	CSize sz(BUTTON_X_MARGIN, 0);
	
	const int btnRowHeight = BUTTON_Y_DIVIDER_HEIGHT + BUTTON_HEIGHT;
	int nBtnCount = m_pDlgTemplate->GetAnswerButtonCount();
	int rowCount = nBtnCount / MAX_BTNS_IN_ROW + (nBtnCount % MAX_BTNS_IN_ROW > 0 ? 1 : 0);
	int maxBtsInRow = nBtnCount > MAX_BTNS_IN_ROW ? MAX_BTNS_IN_ROW : nBtnCount;

	sz.cx = 2 * BUTTON_X_MARGIN + BUTTON_WIDTH*maxBtsInRow + BUTTON_X_DIVIDER_WIDTH*(maxBtsInRow - 1);
	sz.cy = 2 * BUTTON_Y_MARGIN + BUTTON_HEIGHT*rowCount + (rowCount - 1)*BUTTON_Y_DIVIDER_HEIGHT;
	
	
	return sz;
}

CSize QuestionDlg::CalcSizeOfQuestionArea()
{
	std::wstring sMsgText = m_pDlgTemplate->GetQuestion();
	int messageHeight = 2 * MESSAGE_PADDING_Y;
	int messageWidth = 2 * MESSAGE_PADDING_X;

	if (!sMsgText.empty())
	{
		CRect rect;
		HDC hdc = ::GetDC(m_hWnd);
		::DrawText(hdc, sMsgText.c_str(), sMsgText.length(), &rect, DT_CALCRECT);
		
		messageHeight = rect.Height();
		messageWidth = rect.Width();		
	} 

	return CSize(max(messageWidth, MIN_MESSAGEAREA_WIDTH), max(messageHeight, MIN_MESSAGEAREA_HEIGHT));
	
}

BOOL QuestionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(m_pDlgTemplate->GetCaption().c_str());

	CSize buttonsArea = CalcSizeOfButtonArea();
	CSize messageArea = CalcSizeOfQuestionArea();

	CSize clientSize(max(buttonsArea.cx, messageArea.cx), buttonsArea.cy + messageArea.cy);
	CSize windowSize(clientSize);
	windowSize.cx += 2 * ::GetSystemMetrics(SM_CXFIXEDFRAME);
	windowSize.cy += ::GetSystemMetrics(SM_CYCAPTION) + 2 * ::GetSystemMetrics(SM_CYFIXEDFRAME);

	SetWindowPos(NULL, 0, 0, windowSize.cx, windowSize.cy, SWP_NOMOVE | SWP_NOZORDER);
	
	UINT nBtnId = MIN_BTN_CTRL_ID;
	
	int btnX = BUTTON_X_MARGIN + (clientSize.cx - buttonsArea.cx)/2, btnY = messageArea.cy + BUTTON_Y_MARGIN;
	
	CButton* defaultButton = NULL;

	for (int iBtn = 0, nBtnCount = m_pDlgTemplate->GetAnswerButtonCount(); iBtn < nBtnCount; ++iBtn)
	{
		const AnswerButton* pAnswerBtn = m_pDlgTemplate->GetButton(iBtn);
		CButton* pBtn = new CButton();
		m_DlgItems.push_back(pBtn);
		
		pBtn->Create(pAnswerBtn->GetCaption().c_str(),
			WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_PUSHBUTTON,
			CRect(btnX, btnY, btnX + BUTTON_WIDTH, btnY + BUTTON_HEIGHT),
			this,
			nBtnId);


		m_BtnIdToAnswer[nBtnId] = pAnswerBtn->GetButtonId();

		pBtn->SetFont(GetFont(), FALSE);

		if (m_pDlgTemplate->GetDefaultAnswer() == pAnswerBtn->GetButtonId())
		{
			defaultButton = pBtn;
		}


		bool bLastInRow = iBtn > 0 && (iBtn + 1) % MAX_BTNS_IN_ROW == 0;
		btnX += BUTTON_WIDTH + (bLastInRow ? 0 : BUTTON_X_DIVIDER_WIDTH);
				

		if (bLastInRow)
		{
			btnY += BUTTON_HEIGHT+BUTTON_Y_DIVIDER_HEIGHT;
			btnX = BUTTON_X_MARGIN;
		}
		nBtnId++;
	}
	
	CXColorStatic* messageStatic = new CXColorStatic();	
	messageStatic->Create(m_pDlgTemplate->GetQuestion().c_str(), WS_VISIBLE | WS_CHILD | SS_CENTER| SS_CENTERIMAGE, CRect(0, 0, clientSize.cx, messageArea.cy), this);
	messageStatic->SetBackgroundColor(RGB(255, 255, 255));
	m_DlgItems.push_back(messageStatic);


	m_Answer = INVALID_ANSWER_ID;

	if (defaultButton)
		GotoDlgCtrl(defaultButton);
		

	return TRUE;
} 

BOOL QuestionDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if (HIWORD(wParam) == BN_CLICKED)
	{
		m_Answer = m_BtnIdToAnswer[LOWORD(wParam)];
		EndDialog(IDOK);
		return TRUE;
	}
	return CDialog::OnCommand(wParam, lParam);	
}

BOOL QuestionDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			return TRUE;
		case  VK_ESCAPE:
			EndDialog(IDCANCEL);
			return TRUE;          
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void QuestionDlg::OnDestroy()
{
	DeepDelete(m_DlgItems);
	m_DlgItems.clear();
	m_BtnIdToAnswer.clear();
}

void QuestionDlg::OnClose()
{
	EndDialog(IDCANCEL);
}