#include "stdafx.h"
#include "AnswerButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


AnswerButton::AnswerButton(const std::wstring& sBtnCaption, UINT uiBtnId) : m_sBtnCaption(sBtnCaption), m_uiBtnId(uiBtnId)
{
}

const std::wstring& AnswerButton::GetCaption() const
{
	return m_sBtnCaption;
}

UINT AnswerButton::GetButtonId() const
{
	return m_uiBtnId;
}
