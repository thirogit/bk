#ifndef __PASSIMAGECTRL_H__
#define __PASSIMAGECTRL_H__

#include "../../../ctrls/freeimagectrl/FreeImageCtrl.h"
#include <configuration/passportrecognition/IPassportRecognitionProfile.h>
#include "../../types/Size.h"
#include "../../types/Point.h"
#include <vector>

class CPassImageCtrl : public CFreeImageCtrl
{
	struct Field
	{
		Point<int> xy;
		Size<int> wh;
	};

public:	
	~CPassImageCtrl();
	void SetProfile(const IPassportRecognitionProfile* profile);
	
protected:
	std::vector<Field*> m_fields;
	Size<int> m_passSizeMM;
protected:
	afx_msg void OnPaint();
	
	DECLARE_MESSAGE_MAP()
};


#endif