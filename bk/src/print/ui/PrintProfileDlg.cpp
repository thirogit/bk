#include "stdafx.h"
#include "PrintProfileDlg.h"
#include "optionctrls\OptionCtrlFactory.h"
#include "../../utils/StringLoader.h"
#include <string/TextUtils.h>
#include <boost/foreach.hpp>
#include "../../ui/waitdlg/WaitDialog.h"
#include <arrays\DeepDelete.h>
#include "../../ui/ExceptionMsgBox.h"

BEGIN_MESSAGE_MAP(PrintProfileDlg, CDialog)

	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnTabSelChange)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnTabSelChanging)


END_MESSAGE_MAP()


PrintProfileDlg::PrintProfileDlg(const DocumentCfgDescription* configuration, PrintProfileEditor* pEditor, CWnd* pParent/* = NULL*/) :
CDialog(PrintProfileDlg::IDD, pParent), m_configuration(configuration), m_pEditor(pEditor), m_sortingOrderWnd(configuration, pEditor)
{
}

void PrintProfileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROFILENAME, m_profileName);
	DDX_Control(pDX, IDC_TAB, m_naviTab);
}


BOOL PrintProfileDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	std::wstring sDlgCaption;
	if (m_pEditor->IsUpdating())
		sDlgCaption = TextUtils::Format(StringLoader(IDS_EDITPRINTPROFILEDLGCAPTION), m_pEditor->GetProfileName());
	else
		sDlgCaption = StringLoader(IDS_NEWPRINTPROFILEDLGCAPTION);
	
	SetWindowText(sDlgCaption.c_str());
		
	CRect tabRect = GetTabRect();	

	m_sortingOrderWnd.Create(&m_naviTab);
	m_sortingOrderWnd.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_NOZORDER);
	
	m_verticalStackWnd.Create(WS_CHILD | WS_VISIBLE | SS_LEFT, tabRect, &m_naviTab, IDC_OPTIONS);
	CFont* dlgFont = GetFont();
	m_verticalStackWnd.SetFont(dlgFont);
	PtrEnumerator<DocumentOptionDefinition> enumOption = m_configuration->EnumOptionDefinitions();

	DocumentOptionCtrlFactory factory(&m_verticalStackWnd);
	UINT nStartId = 1001;
	while (enumOption.hasNext())
	{
		DocumentOptionDefinition* definition = *enumOption;
		DocumentOptionCtrl* ctrl = factory.CreateCtrl(*definition, nStartId++);

		DubiousPrintOption option = m_pEditor->GetOption(definition->GetCode());
		if (option.is_initialized())
		{
			ctrl->SetOptionValue(option.get());
		}
		else
		{
			PrintOption defaultOption(definition->GetCode());
			defaultOption.set(definition->GetDefaultValue());
			ctrl->SetOptionValue(defaultOption);
		}
		
		m_verticalStackWnd.AddControl(definition->GetName(), ctrl);
		m_optionCtrls.push_back(ctrl);

		enumOption.advance();
	}

	AddTab(&m_verticalStackWnd, StringLoader(IDS_PRINTOPTIONSTABCAPTION));
	AddTab(&m_sortingOrderWnd, StringLoader(IDS_PRINTSORTINGORDER));

	m_profileName.SetText(m_pEditor->GetProfileName());

	return TRUE;
}

void PrintProfileDlg::AddTab(CWnd* tabWnd, const std::wstring& sTabText)
{
	//CRect rect = GetTabRect();
	TCITEM item;
	item.mask = TCIF_TEXT | TCIF_PARAM;
	item.lParam = (LPARAM)tabWnd;
	item.pszText = const_cast<wchar_t*>(sTabText.c_str());
	m_naviTab.InsertItem(m_naviTab.GetItemCount(), &item);
	tabWnd->SetWindowPos(NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOZORDER | SWP_NOMOVE);
}


void PrintProfileDlg::OnOK()
{
	std::wstring sProfileName = m_profileName.GetText();

	if (sProfileName.empty() || TextUtils::IsAllWhitespace(sProfileName))
	{
		AfxMessageBox(IDS_EMPTYPROFILENAME, MB_OK | MB_ICONERROR);
		m_profileName.SetFocus();
		return;
	}

	m_pEditor->SetProfileName(sProfileName);

	BOOST_FOREACH(DocumentOptionCtrl* optionCtrl, m_optionCtrls)
	{
		m_pEditor->SetOption(optionCtrl->GetOptionValue());
	}

	m_sortingOrderWnd.Apply();

	ExceptionMsgBox msgBox;

	try
	{

		CWaitDialog waitForSave(this, StringLoader(IDS_SAVINGPROFILE));
		waitForSave.WaitForTask(std::bind(&PrintProfileEditor::Save, m_pEditor));
	}
	catch (PrintProfileEditorExeption& e)
	{
		msgBox.Show(e);
	}

	EndDialog(IDOK);

}

void PrintProfileDlg::OnDestroy()
{
	CDialog::OnDestroy();
	DeepDelete(m_optionCtrls);
}


CRect PrintProfileDlg::GetTabRect()
{
	CRect tabItemRect;
	m_naviTab.GetClientRect(&tabItemRect);
	tabItemRect.top += 28;
	tabItemRect.bottom -= 4;
	tabItemRect.left += 4;
	tabItemRect.right -= 8;
	return tabItemRect;
}

void PrintProfileDlg::OnTabSelChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	int iNewTab = m_naviTab.GetCurSel();

	TCITEM item;
	CWnd* pWnd;

	item.mask = TCIF_PARAM;

	if (m_prevSelectedTab >= 0)
	{
		//** hide the current tab ---------
		m_naviTab.GetItem(m_prevSelectedTab, &item);
		pWnd = reinterpret_cast<CWnd*> (item.lParam);
		ASSERT_VALID(pWnd);
		pWnd->ShowWindow(SW_HIDE);
	}

	//** show the selected tab --------
	m_naviTab.GetItem(iNewTab, &item);
	pWnd = reinterpret_cast<CWnd*> (item.lParam);
	ASSERT_VALID(pWnd);
	pWnd->ShowWindow(SW_SHOW);

	//OnTabChanged(m_prevSelectedTab,iNewTab);

	*pResult = 0;
}

void PrintProfileDlg::OnTabSelChanging(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_prevSelectedTab = m_naviTab.GetCurSel();

	*pResult = 0;
}
