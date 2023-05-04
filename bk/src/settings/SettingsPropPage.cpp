#include "stdafx.h"
#include "SettingsPropPage.h"

IMPLEMENT_DYNAMIC(SettingsPropPage, CPropertyPage)

SettingsPropPage::SettingsPropPage() : CPropertyPage()
{
}

SettingsPropPage::SettingsPropPage(LPCTSTR lpszTemplateName, UINT nIDCaption, UINT nIDHeaderTitle, UINT nIDHeaderSubTitle /* = 0 */, DWORD dwSize /* = sizeof */)
: CPropertyPage(lpszTemplateName, nIDCaption, nIDHeaderTitle, nIDHeaderSubTitle, dwSize)
{
}

SettingsPropPage::SettingsPropPage(UINT nIDTemplate, UINT nIDCaption, UINT nIDHeaderTitle, UINT nIDHeaderSubTitle /* = 0 */, DWORD dwSize /* = sizeof */)
: CPropertyPage(nIDTemplate, nIDCaption, nIDHeaderTitle, nIDHeaderSubTitle, dwSize)
{
}

SettingsPropPage::SettingsPropPage(LPCTSTR lpszTemplateName, UINT nIDCaption /* = 0 */, DWORD dwSize /* = sizeof */)
: CPropertyPage(lpszTemplateName, nIDCaption, dwSize)
{
}

SettingsPropPage::SettingsPropPage(UINT nIDTemplate, UINT nIDCaption /* = 0 */, DWORD dwSize /* = sizeof */)
: CPropertyPage(nIDTemplate, nIDCaption, dwSize)
{
}

SettingsPropPage::~SettingsPropPage()
{
}


void SettingsPropPage::AdjustControlSize(UINT nID)
{
	CWnd* pwndDlgItem = GetDlgItem(nID);
	if (!pwndDlgItem)
		return;
	// adjust the size of the control to fit its content
	CString sControlText;
	pwndDlgItem->GetWindowText(sControlText);
	// next step: find the rectangle the control text needs to
	// be displayed

	CDC* pDC = pwndDlgItem->GetWindowDC();
	RECT controlrect;
	RECT controlrectorig;
	pwndDlgItem->GetWindowRect(&controlrect);
	::MapWindowPoints(nullptr, GetSafeHwnd(), (LPPOINT)&controlrect, 2);
	controlrectorig = controlrect;
	if (pDC)
	{
		CFont* font = pwndDlgItem->GetFont();
		CFont* pOldFont = pDC->SelectObject(font);
		if (pDC->DrawText(sControlText, -1, &controlrect, DT_WORDBREAK | DT_EDITCONTROL | DT_EXPANDTABS | DT_LEFT | DT_CALCRECT))
		{
			// now we have the rectangle the control really needs
			if ((controlrectorig.right - controlrectorig.left) > (controlrect.right - controlrect.left))
			{
				// we're dealing with radio buttons and check boxes,
				// which means we have to add a little space for the checkbox
				// the value of 3 pixels added here is necessary in case certain visual styles have
				// been disabled. Without this, the width is calculated too short.
				const int checkWidth = GetSystemMetrics(SM_CXMENUCHECK) + 2 * GetSystemMetrics(SM_CXEDGE) + 3;
				controlrectorig.right = controlrectorig.left + (controlrect.right - controlrect.left) + checkWidth;
				pwndDlgItem->MoveWindow(&controlrectorig);
			}
		}
		pDC->SelectObject(pOldFont);
		ReleaseDC(pDC);
	}
}