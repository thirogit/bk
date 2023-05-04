#ifndef __HENTTYPECOMBO_H__
#define __HENTTYPECOMBO_H__

#include <data/HentType.h>


class CHentTypeCombo : public CComboBox
{
public:
	CHentTypeCombo();
	void ReloadCombo(HentType selection = HentType_None);
	HentType GetSelectedHentType();
};

#endif