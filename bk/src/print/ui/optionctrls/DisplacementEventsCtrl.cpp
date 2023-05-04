#include "stdafx.h"
#include "DisplacementEventsCtrl.h"
#include "../../options/DisplacementEventType.h"
#include "../../../resource.h"
#include "../../../utils/StringLoader.h"
#include <arrays\DeepDelete.h>
#include <boost/foreach.hpp>
#include <string\IntString.h>
#include "../../options/PrintOptionSerializer.h"

#define ID_CHECKBOX_MIN_ID 1001

BEGIN_MESSAGE_MAP(DisplacementEventsCtrl, DocumentOptionCtrl)
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()


PrintOption DisplacementEventsCtrl::GetOptionValue()
{
	PrintOption option(GetOptionCode());
	PrintOptionSerializer seriazlier;
	seriazlier.SetValue(option, GetEvents());
	return option;
}

uint32_t DisplacementEventsCtrl::GetEvents()
{
	uint32_t events = DisplacementEventType_None;

	BOOST_FOREACH(EventButton* button, m_content)
	{
		if (button->checkbox->SendMessage(BM_GETCHECK))
		{
			events = events | button->eventType;
		}
	}
	return events;
}

void DisplacementEventsCtrl::SetEvents(uint32_t events)
{
	BOOST_FOREACH(EventButton* button, m_content)
	{
		button->checkbox->SendMessage(BM_SETCHECK, (events & button->eventType) ? BST_CHECKED : BST_UNCHECKED);
	}
}

void DisplacementEventsCtrl::SetOptionValue(const PrintOption& option)
{
	PrintOptionSerializer seriazlier;
	uint32_t events = seriazlier.GetDisplacementEventsValue(option);
	SetEvents(events);
}

void DisplacementEventsCtrl::OnDestroy()
{
	DocumentOptionCtrl::OnDestroy();
	DeepDelete(m_content);
}

void DisplacementEventsCtrl::CreateContent()
{
	const int buttonHeight = 17;

	struct LayoutControl
	{
		DisplacementEventType eventType;
		UINT caption;		
	} controls[] =
	{
		{ DisplacementEventType_Buy, IDS_DISPLACEMENT_BUY },
		{ DisplacementEventType_Sell, IDS_DISPLACEMENT_SELL },
		{ DisplacementEventType_BuySell, IDS_DISPACEMENT_BUYSELL },
		{ DisplacementEventType_ExportToEU, IDS_DISPLACEMENT_EXPORTTOE },
		{ DisplacementEventType_ExportNotToEU, IDS_DISPLAEMENT_EXPORTNOTTOEU },
		{ DisplacementEventType_Other, IDS_DISPLACEMENT_OTHER },
		{ DisplacementEventType_None, 0 }
	};
	
	CRect buttonPosition(0, 0, 0, buttonHeight);
	LayoutControl* control = controls;
	UINT id = ID_CHECKBOX_MIN_ID;
	while (control->eventType != DisplacementEventType_None)
	{
		EventButton* button = new EventButton();
		std::wstring caption = StringLoader(control->caption);
		button->checkbox = new CButton();
		button->checkbox->Create(caption.c_str(), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | WS_TABSTOP, buttonPosition, this, id);
		buttonPosition.top += buttonHeight;
		buttonPosition.bottom += buttonHeight;
		button->checkbox->SetFont(GetFont());
		button->eventType = control->eventType;
		m_content.push_back(button);
		control++;
		id++;
	}

	SetWindowPos(NULL, 0, 0, 0, buttonPosition.bottom, SWP_NOZORDER | SWP_NOMOVE);
				
}

void DisplacementEventsCtrl::OnSize(UINT nType, int cx, int cy)
{
	DocumentOptionCtrl::OnSize(nType, cx, cy);

	CRect buttonRect;
	BOOST_FOREACH(EventButton* button, m_content)
	{
		button->checkbox->GetClientRect(buttonRect);
		button->checkbox->SetWindowPos(NULL, 0, 0, cx, buttonRect.Height(),SWP_NOZORDER|SWP_NOMOVE);
	}

}