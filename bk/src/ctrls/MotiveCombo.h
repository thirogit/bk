#ifndef __MOTIVECOMBO_H__
#define __MOTIVECOMBO_H__

#include <data/Motive.h>
#include <data\motives\Motives.h>

class CMotiveCombo : public CComboBox
{
public:
	CMotiveCombo();
	void InitCombo(Motive_Direction direction = MotiveDirection_None);
	void SelectMotive(Motive motive);
	
	Motive GetSelectedMotive();
};

#endif