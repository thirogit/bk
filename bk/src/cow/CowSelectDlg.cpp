#include "stdafx.h"
#include "CowSelectDlg.h"
#include "../utils/StringLoader.h"
#include "../ui/ExceptionMsgBox.h"
#include <string\IntString.h>
#include <string\TextUtils.h>
#include "../AppServices.h"
#include "../ui/waitdlg\WaitDialog.h"
#include <boost/foreach.hpp>

#define HIDE_TOOLTIP_AFTER_MS 2000
#define TOOLTIP_FONTHEIGHT 45

BEGIN_MESSAGE_MAP(CowSelectDlg::PickContentListCtrl, CContentListCtrl)
	ON_WM_CHAR()	
END_MESSAGE_MAP()

CowSelectDlg::PickContentListCtrl::PickContentListCtrl(CowSelectInterface *pInterface) : m_pInterface(pInterface)
{
}

void CowSelectDlg::PickContentListCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch(nChar)
	{
	case '+':
		m_pInterface->PickRow(GetCurSel());
		break;
	case '-':
		m_pInterface->UnpickRow(GetCurSel());
		break;
	default:
		CContentListCtrl::OnChar(nChar,nRepCnt,nFlags);
	}
}

void CowSelectDlg::PickContentListCtrl::SortBy(const SortAttributes& attributes)
{
	m_pInterface->GetCandidates()->SortContent(attributes);
}

//-------------------------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CowSelectDlg, CDialog)
	ON_BN_CLICKED(IDC_SELECT, OnPickRow)
	ON_BN_CLICKED(IDC_UNSELECT, OnUnpickRow)
	ON_WM_DESTROY()
	ON_NOTIFY(EN_EDITCOMPLETE, IDC_COWNOWILDCARD, OnEditCowNoComplete)
END_MESSAGE_MAP()


CowSelectDlg::CowSelectDlg(CowSelectInterface *pInterface,CWnd *pParent/* = NULL*/) : 
CDialog(CowSelectDlg::IDD, pParent),
	m_pInterface(pInterface),
	m_CandidatesList(pInterface)	
{
}

void CowSelectDlg::SetHeader(const ListHeaderDescriptor& header)
{
	m_CandidatesList.SetHeaderDescriptor(header);
}

void CowSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_CANDIDATESLIST,m_CandidatesList);
	DDX_Control(pDX, IDC_SCANNERINDICATOR, m_indicator);
	DDX_Control(pDX, IDC_COWNOWILDCARD, m_CowNoWildcardEdit);
	
}


BOOL CowSelectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_CandidatesList.SetContent(m_pInterface->GetCandidates());

	std::function<void(int)> dblClick = [this](int row)
	{
		OnPickRow();
	};

	m_CandidatesList.SetRowDblClickHandler(dblClick);
	UpdateCount();

	AppServices::GetBCScannerService()->AddListener(this);
	UpdateBarcodeIndictor();
	/*TipWndStyle style;
	style.inner = Color();
	style.outer = Color();*/

	m_Msg.Create(this);
	m_Msg.SetShowCenteredInParent();
	
	//m_BadCowNoMsg.SetStyle(style);

	return TRUE;
}

void CowSelectDlg::OnDestroy()
{
	AppServices::GetBCScannerService()->RemoveListener(this);
	m_CandidatesList.SetContent(NULL);
	CDialog::OnDestroy();
}

void CowSelectDlg::PickRows(const std::vector<int>& rows)
{
	ExceptionMsgBox().TryCatchAny([this, &rows]()
	{
		CWaitDialog waitDlg(this, StringLoader(IDS_WAITSELECTING));

		waitDlg.WaitForTask([&rows, this]()
		{
			BOOST_FOREACH(int row, rows)
			{
				if (!m_pInterface->IsRowPicked(row))
				{
					m_pInterface->PickRow(row);
				}
			}
		});
	});
	UpdateCount();	
	
}

void CowSelectDlg::OnPickRow()
{
	POSITION pos = m_CandidatesList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		std::vector<int> selectedItems;
		while (pos)
		{
			int nItem = m_CandidatesList.GetNextSelectedItem(pos);
			selectedItems.push_back(nItem);
		}

		PickRows(selectedItems);

	}
}

void CowSelectDlg::OnUnpickRow()
{
	int row = m_CandidatesList.GetCurSel();
	if(row >= 0)
	{
		ExceptionMsgBox().TryCatchAny([this,row]()
		{
			m_pInterface->UnpickRow(row);
		});
		UpdateCount();
	}
}

void CowSelectDlg::UpdateCount()
{
	SetDlgItemText(IDC_SELECTEDCOUNT,IntString(m_pInterface->GetPickedCount()).c_str());
}

void CowSelectDlg::OnEditCowNoComplete(NMHDR* pNMHDR, LRESULT* pResult)
{
	std::wstring cowNoWC = m_CowNoWildcardEdit.GetText();
	
	SelectResult result = m_pInterface->SelectCows(cowNoWC);

	Color red(255, 0, 0);
	Color yellow(255, 0, 0);
	Color green(0, 255, 0);

	if (result.matched == 0)
	{
		m_Msg.SetText(StringLoader(IDS_NOTFOUND));
		m_Msg.SetStyle(TipWndStyle().inner(red).outer(red).textsize(TOOLTIP_FONTHEIGHT));
		m_Msg.Show(HIDE_TOOLTIP_AFTER_MS);
		MessageBeep(MB_ICONERROR);
	}
	else if (result.matched != result.picked)
	{
		m_Msg.SetText(TextUtils::Format(StringLoader(IDS_PICKEDOUTOFMATCHED), result.picked, result.matched));
		m_Msg.SetStyle(TipWndStyle().inner(red).outer(red).textsize(TOOLTIP_FONTHEIGHT));
		m_Msg.Show(HIDE_TOOLTIP_AFTER_MS);
		MessageBeep(MB_ICONWARNING);
	}
	else
	{
		m_Msg.SetText(TextUtils::Format(StringLoader(IDS_COWSPICKED),result.picked));
		m_Msg.SetStyle(TipWndStyle().inner(green).outer(green).textsize(TOOLTIP_FONTHEIGHT));
		m_Msg.Show(HIDE_TOOLTIP_AFTER_MS);
		MessageBeep(MB_ICONWARNING);
	}
	
	m_CandidatesList.Invalidate();
	UpdateCount();
	m_CowNoWildcardEdit.SetSel(0, -1);
	*pResult = (LRESULT)0;
}

void CowSelectDlg::OnBarcode(const std::wstring& sBarcode)
{
	Color red(255, 0, 0);
	try
	{
		CowNo cowNo = CowNoFactory::FromString(sBarcode);
		if (!m_pInterface->SelectCow(cowNo))
		{
			m_Msg.SetText(StringLoader(IDS_NOTFOUND));
			m_Msg.SetStyle(TipWndStyle().inner(red).outer(red).textsize(TOOLTIP_FONTHEIGHT));
			m_Msg.Show(HIDE_TOOLTIP_AFTER_MS);
			MessageBeep(MB_ICONERROR);
		}
		m_CandidatesList.Invalidate();
		UpdateCount();
	}
	catch (...)
	{
		m_Msg.SetText(StringLoader(IDS_BADCOWNOBARCODE));
		m_Msg.Show(HIDE_TOOLTIP_AFTER_MS);
		MessageBeep(MB_ICONWARNING);
	}
}

void CowSelectDlg::OnScannerConnected()
{
	UpdateBarcodeIndictor();
}

void CowSelectDlg::OnScannerDisconnected()
{
	UpdateBarcodeIndictor();
}

void CowSelectDlg::UpdateBarcodeIndictor()
{
	m_indicator.SetStatus(AppServices::GetBCScannerService()->GetConnectionStatus());
}

