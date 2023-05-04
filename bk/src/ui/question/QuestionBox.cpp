#include "stdafx.h"
#include "QuestionBox.h"
#include "QuestionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


Answer::Answer(UINT answer, bool bRemember/* = false*/) : m_bRemember(bRemember), m_uiAnswer(answer)
{
}

Answer::Answer(const Answer& src)
{
	operator=(src);
}

Answer& Answer::operator=(const Answer& right)
{
	m_bRemember = right.m_bRemember;
	m_uiAnswer = right.m_uiAnswer;
	return *this;
}

bool Answer::IsRemember() const
{
	return m_bRemember;
}

UINT Answer::GetAnswer() const
{
	return m_uiAnswer;
}


//---------------------------------------------------------------------------------------------------------------------
QuestionBox::QuestionBox()
{
}

QuestionBox::~QuestionBox()
{
	
}

QuestionBox& QuestionBox::AddAnswer(const std::wstring& sAnswer, UINT uiAnswerId)
{
	m_DlgTemplate.AddAnswerButton(sAnswer,uiAnswerId);
	return *this;
}

QuestionBox& QuestionBox::Caption(const std::wstring& sCaption)
{
	m_DlgTemplate.SetCaption(sCaption);
	return *this;
}

QuestionBox& QuestionBox::Question(const std::wstring& sQuestion)
{
	m_DlgTemplate.SetQuestion(sQuestion);
	return *this;
}

QuestionBox& QuestionBox::Remember(bool bShowRemember)
{
	m_DlgTemplate.ShowRemember(bShowRemember);
	return *this;
}

QuestionBox& QuestionBox::CancelId(UINT uiCancelId)
{
	m_DlgTemplate.SetCancelId(uiCancelId);
	return *this;
}

QuestionBox& QuestionBox::DefaultAnswer(UINT uiAnswerId)
{
	m_DlgTemplate.SeDefaultAnswer(uiAnswerId);
	return *this;
}

Answer QuestionBox::Ask(CWnd* pParentWnd)
{
	QuestionDlg dlg(&m_DlgTemplate, pParentWnd);
	if (dlg.DoModal() == IDOK)
	{
		return Answer(dlg.GetAnswer(), false);
	}

	return Answer(INVALID_ANSWER_ID, false);	
}
		