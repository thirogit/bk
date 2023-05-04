#ifndef __EXTRASMEMO_H__
#define __EXTRASMEMO_H__

#include "dimedit/DimEditCtrl.h"

#define DIMCOLOR RGB( 0xFF, 0x80, 0x80 )

class CExtrasMemo : public CDimEditCtrl
{
public:
	CExtrasMemo(int charsLimit);
protected:
	virtual void PreSubclassWindow();
	virtual void SetExtrasMemo();
private:
	static const char m_szMax255[];
	int m_CharsLimit;
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

};


#endif