#ifndef __COWSEXBTN_H__
#define __COWSEXBTN_H__

#include <data\types\CowSex.h>

class CCowSexBtn : public CStatic
{
public:
	CCowSexBtn();
	virtual ~CCowSexBtn();

	void SetSex(const CowSex& sex);
	const CowSex& GetSex() const;
	
protected:
	virtual void PreSubclassWindow();
	void CreateButtons();
	void CheckButtons();
	
	CowSex m_sex;
	CToolBarCtrl m_Btns;

	afx_msg void OnXX();
	afx_msg void OnXY();


	DECLARE_MESSAGE_MAP()
};


#endif

