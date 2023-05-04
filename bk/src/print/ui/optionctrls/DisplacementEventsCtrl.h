#ifndef __DISPLACEMENTEVENTSCTRL_H__
#define __DISPLACEMENTEVENTSCTRL_H__

#include "DocumentOptionCtrl.h"
#include <vector>
#include "../../options/DisplacementEventType.h"

class DisplacementEventsCtrl : public DocumentOptionCtrl
{	
	struct EventButton
	{
		CButton* checkbox;
		DisplacementEventType eventType;
	};

public:
	
	virtual PrintOption GetOptionValue();
	virtual void SetOptionValue(const PrintOption& option);
	uint32_t GetEvents();
	void SetEvents(uint32_t events);
protected:
	virtual void CreateContent();	
private:
	
	
	std::vector<EventButton*> m_content;


protected:
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};


#endif

