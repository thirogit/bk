#include "stdafx.h"
#include "DocSelectDlg.h"
#include "../utils/StringLoader.h"
#include "../ui/ExceptionMsgBox.h"
#include <string\IntString.h>

BEGIN_MESSAGE_MAP(DocSelectDlg, CSessionAwareDialog)
	ON_BN_CLICKED(IDC_SELECT,OnPickRow)
	ON_BN_CLICKED(IDC_UNSELECT,OnUnpickRow)
END_MESSAGE_MAP()


DocSelectDlg::DocSelectDlg(DocSelectInterface *pInterface,CWnd *pParent/* = NULL*/) : 
	CSessionAwareDialog(DocSelectDlg::IDD,pParent),m_pInterface(pInterface)
{
}

void DocSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CSessionAwareDialog::DoDataExchange(pDX);	
	DDX_Control(pDX,IDC_CANDIDATESLIST,m_CandidatesList);
}

void DocSelectDlg::SetHeader(const ListHeaderDescriptor& header)
{
	m_CandidatesList.SetHeaderDescriptor(header);
}

BOOL DocSelectDlg::OnInitDialog()
{
	CSessionAwareDialog::OnInitDialog();	
	
	m_CandidatesList.SetContent(m_pInterface->GetCandidates());

	std::function<void(int)> dblClick = [this](int row)
	{
		OnPickRow();
	};

	m_CandidatesList.SetRowDblClickHandler(dblClick);

	UpdateCount();
	
	return TRUE;
}

void DocSelectDlg::OnPickRow()
{
	int row = m_CandidatesList.GetCurSel();
	if(row >= 0)
	{
		if(!m_pInterface->IsRowPicked(row))
		{
			ExceptionMsgBox().TryCatchAny([this,row]()
			{
				m_pInterface->PickRow(row);
			});
			UpdateCount();
		}
	}
}

void DocSelectDlg::OnUnpickRow()
{
	int row = m_CandidatesList.GetCurSel();
	if(row >= 0)
	{
		if(m_pInterface->IsRowPicked(row))
		{
			ExceptionMsgBox().TryCatchAny([this,row]()
			{
				m_pInterface->UnpickRow(row);
			});
			UpdateCount();
		}
	}
}
void DocSelectDlg::UpdateCount()
{
	SetDlgItemText(IDC_SELECTEDCOUNT,IntString(m_pInterface->GetPickedCount()).c_str());
}