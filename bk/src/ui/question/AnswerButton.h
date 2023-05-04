#ifndef __ANSWERBUTTON_H__
#define __ANSWERBUTTON_H__

#include <string>

class AnswerButton
{
public:
	AnswerButton(const std::wstring& sBtnCaption,UINT uiBtnId);
	const std::wstring& GetCaption() const;
	UINT GetButtonId() const;
private:
	std::wstring m_sBtnCaption;
	UINT m_uiBtnId;
};

#endif