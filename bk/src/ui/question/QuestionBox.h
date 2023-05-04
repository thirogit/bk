#ifndef __QUESTIONBOX_H__
#define __QUESTIONBOX_H__

#include <afxwin.h>
#include "QuestionDlgTemplate.h"
#include <string>

class Answer
{
public:
	Answer(UINT answer, bool bRemember = false);
	Answer(const Answer& src);
	Answer& operator=(const Answer& right);

	bool IsRemember() const;
	UINT GetAnswer() const;

private:
	bool m_bRemember;
	UINT m_uiAnswer;
};


class QuestionBox 
{	
public:
	QuestionBox();
	~QuestionBox();

	QuestionBox& Caption(const std::wstring& sCaption);	
	QuestionBox& AddAnswer(const std::wstring& sAnswer, UINT uiAnswerId);	
	QuestionBox& Question(const std::wstring& sQuestion);
	QuestionBox& Remember(bool bShowRemember);
	QuestionBox& CancelId(UINT uiCancelId);
	QuestionBox& DefaultAnswer(UINT uiAnswerId);
		
	Answer Ask(CWnd* pParentWnd);
		
private:
	QuestionDlgTemplate m_DlgTemplate;	

};
	

#endif