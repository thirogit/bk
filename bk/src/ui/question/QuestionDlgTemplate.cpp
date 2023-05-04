#include "stdafx.h"
#include "QuestionDlgTemplate.h"
#include <arrays/DeepDelete.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


QuestionDlgTemplate::QuestionDlgTemplate() : m_uiCancelId(INVALID_ANSWER_ID),
											 m_uiDefaultAnswerId(INVALID_ANSWER_ID),
											 m_bShowRemember(false)
{
}

QuestionDlgTemplate::~QuestionDlgTemplate()
{
	DeleteAllButtons();
}


void QuestionDlgTemplate::AddAnswerButton(const std::wstring& sAnswerBtnCaption, UINT uiAnswerId)
{
	m_AnswerButtons.push_back(new AnswerButton(sAnswerBtnCaption,uiAnswerId));
}

int QuestionDlgTemplate::GetAnswerButtonCount() const
{
	return m_AnswerButtons.size();
}

const AnswerButton* QuestionDlgTemplate::GetButton(int iBtn) const
{
	return const_cast<QuestionDlgTemplate*>(this)->m_AnswerButtons.at(iBtn);
}

void QuestionDlgTemplate::DeleteButton(int iBtn)
{
	m_AnswerButtons.erase(m_AnswerButtons.begin()+iBtn);
}

void QuestionDlgTemplate::DeleteAllButtons()
{
	DeepDelete(m_AnswerButtons);
	m_AnswerButtons.clear();
}

void QuestionDlgTemplate::SetCaption(const std::wstring& sCaption)
{
	m_sCaption = sCaption;
}

void QuestionDlgTemplate::SetQuestion(const std::wstring& sQuestion)
{
	m_sQuestion = sQuestion;
}

void QuestionDlgTemplate::ShowRemember(bool bShowRemember)
{
	m_bShowRemember = bShowRemember;
}

void QuestionDlgTemplate::SetCancelId(UINT uiCancelId)
{
	m_uiCancelId = uiCancelId;
}

void QuestionDlgTemplate::SeDefaultAnswer(UINT uiAnswerId)
{
	m_uiDefaultAnswerId = uiAnswerId;
}

std::wstring QuestionDlgTemplate::GetCaption() const
{
	return m_sCaption;
}

std::wstring QuestionDlgTemplate::GetQuestion() const
{
	return m_sQuestion;
}

bool QuestionDlgTemplate::IsShowRemebmer() const
{
	return m_bShowRemember;
}

UINT QuestionDlgTemplate::GetCancelId() const
{
	return m_uiCancelId;
}

UINT QuestionDlgTemplate::GetDefaultAnswer() const
{
	return m_uiDefaultAnswerId;
}
	
	
	