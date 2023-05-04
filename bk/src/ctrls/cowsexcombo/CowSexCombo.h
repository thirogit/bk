#ifndef __SEXCOMBO_H__
#define __SEXCOMBO_H__

#include <data\types\CowSex.h>
#include <afxwin.h>

class CCowSexCombo : public CComboBox
{
public:
	void InitCombo(const CowSex& sex = SEX_UNK);
	CowSex GetSex();
	void SelectSex(const CowSex& sex);
	
};

#endif