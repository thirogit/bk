#ifndef __DECIMALFORMATCTRL_H__
#define __DECIMALFORMATCTRL_H__

#include "DocumentOptionCtrl.h"
#include <vector>
#include <types\Decimal.h>

class DecimalFormatCtrl : public DocumentOptionCtrl
{	
public:
	DecimalFormatCtrl();
	~DecimalFormatCtrl();
	virtual PrintOption GetOptionValue();
	virtual void SetOptionValue(const PrintOption& option);
	DecimalFormat GetFormat() const;
	void SetFormat(const DecimalFormat& fmt);
protected:
	virtual void CreateContent();
	void InitContent(const DecimalFormat& format);
private:	
	CStatic *m_pDecimalPtLabel;
	CStatic *m_pPrecisionLabel;
	CComboBox* m_pPrecisionCb;
	CComboBox* m_pDecimalPtCb;	
	
protected:
	afx_msg void OnDestroy();
	afx_msg void OnEnable(BOOL bEnable);
	DECLARE_MESSAGE_MAP()
};


#endif

