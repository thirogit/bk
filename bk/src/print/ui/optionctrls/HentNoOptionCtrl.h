#ifndef __HENTNOOPTIONCTRL_H__
#define __HENTNOOPTIONCTRL_H__

#include "DocumentOptionCtrl.h"
#include "../../../ctrls/HentNoEdit.h"

class HentNoOptionCtrl : public DocumentOptionCtrl
{	
public:
	HentNoOptionCtrl();
	~HentNoOptionCtrl();

	virtual PrintOption GetOptionValue();
	virtual void SetOptionValue(const PrintOption& option);
	HentNo GetHentNo() const;
	void SetHentNo(const HentNo& hentNo);
protected:
	virtual void CreateContent();	
private:	
			
	CHentNoEdit m_edit;
protected:
	afx_msg void OnDestroy();
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};


#endif

