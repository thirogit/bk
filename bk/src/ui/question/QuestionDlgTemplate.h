#ifndef __QUESTIONDLGTEMPLATE_H__
#define __QUESTIONDLGTEMPLATE_H__

#include <vector>
#include "AnswerButton.h"

#define INVALID_ANSWER_ID ((UINT)-1)

class QuestionDlgTemplate 
{		

public:
	QuestionDlgTemplate();
	~QuestionDlgTemplate();
	
	
	void AddAnswerButton(const std::wstring& sAnswerBtnCaption, UINT uiAnswerId);
	int GetAnswerButtonCount() const;
	const AnswerButton* GetButton(int iBtn) const;
	void DeleteButton(int iBtn);
	void DeleteAllButtons();
	
	void SetCaption(const std::wstring& sCaption);
	void SetQuestion(const std::wstring& sQuestion);
	void ShowRemember(bool bShowRemember);
	void SetCancelId(UINT uiCancelId);
	void SeDefaultAnswer(UINT uiAnswerId);

	std::wstring GetCaption() const;
	std::wstring GetQuestion() const;
	bool IsShowRemebmer() const;
	UINT GetCancelId() const;
	UINT GetDefaultAnswer() const;	
		
private:	
	std::vector<AnswerButton*> m_AnswerButtons;
	std::wstring m_sCaption;
	std::wstring m_sQuestion;
	bool m_bShowRemember;
	UINT m_uiCancelId;
	UINT m_uiDefaultAnswerId;

};
	

#endif