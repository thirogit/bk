#include "stdafx.h"
#include "SellDocDlg.h"
#include <data\datalimits.h>
#include "../resource.h"
#include "../utils/StringLoader.h"
#include <string\TextUtils.h>
#include "../text/DocNoText.h"
#include <boost/format.hpp>
#include "../context/transaction/TransactionNewCowEditor.h"
#include "../cow/CowDlg.h"
#include "../ui/waitdlg\WaitDialog.h"
#include "../context/transaction/TransactionException.h"
#include "../header/CowInStockPresenceListHeader.h"
#include "../cow/CowSelectDlg.h"
#include "../ui/menu/MenuBuilder.h"
#include "../hent/HentSearchDlg.h"
#include "../view/CowPresenceViewCreator.h"
#include <arrays\DeepDelete.h>
#include <utils/SafeGet.h>
#include "../ui/ExceptionMsgBox.h"
#include "TransactionSelectInterface.h"
#include "../content/AgentsComboContent.h"
#include "../passportsegregation/PassportSearchDlg.h"
#include "TransactionPassportCollection.h"
#include "../settings/DefaultValuesEditor.h"
#include "../hent/HentEditor.h"
#include "../hent/HentDlg.h"


BEGIN_MESSAGE_MAP(SellDocDlg, CSessionAwareDialog )
	
			ON_BN_CLICKED(IDC_ADDCOW, OnAddCow)	
			ON_BN_CLICKED(IDC_COWDELETE,OnDeleteCow)
			ON_WM_DESTROY()
			ON_BN_CLICKED(IDC_SETPLATENOMENUBTN,OnSetPlateNo)
			//ON_NOTIFY(NM_DBLCLK , IDC_COWLIST, OnDblClick)
			//	
			//ON_BN_CLICKED(IDCANCEL,OnCancel)
			ON_BN_CLICKED(IDC_HENTBTN,OnHent)
			//ON_NOTIFY(NM_RCLICK, IDC_COWLIST, OnRClick)
			//ON_WM_CLOSE()
			//ON_COMMAND(IDM_COWLIST_EDITCOW,OnMenuEditCow)
			//ON_COMMAND(IDM_COWLIST_SHOWPASSIMG,OnMenuShowPass)
			ON_BN_CLICKED(IDC_FINDPASSPORTS,OnFindPassports)
			//ON_COMMAND_RANGE(IDM_OUTGRPMENU_BASE,IDM_OUTGRPMENU_MAX,OnGrpChange)
			//ON_COMMAND_RANGE(IDM_OUTGRPMENU_NOGRP,IDM_OUTGRPMENU_NOGRP,OnGrpChange)
			ON_BN_CLICKED(IDC_SETLOADTIMESTART,OnSetLoadTimeStart)
			ON_BN_CLICKED(IDC_SETLOADTIMEEND,OnSetLoadTimeEnd)
			ON_BN_CLICKED(IDC_SETPLATENO,OnSetPlateNo)
			ON_BN_CLICKED(IDC_CLEARAGENT, OnClearAgent)
			ON_NOTIFY(BN_EDITHENT, IDC_HENTBTN, OnEditHent)
END_MESSAGE_MAP()

SellDocDlg::SellDocDlg(SellDocTransactionPtr transaction,CWnd* pParent) : CSessionAwareDialog(SellDocDlg::IDD,pParent),
	m_extras(MAXEXTRAS),
	m_itemCount(transaction->GetFacade())
{
	m_transaction = transaction;
	
}

SellDocDlg::~SellDocDlg()
{
}


void SellDocDlg::DoDataExchange(CDataExchange* pDX)
{
	CSessionAwareDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COWLIST, m_cowlist);	
	
	DDX_Control (pDX, IDC_DOCDATE, m_DocDate ) ;
	DDX_Control (pDX, IDC_TRANSPORTDATE, m_TransportDate ) ;	
	DDX_StdText	(pDX,IDC_PLATENO,m_plateNo);
	DDX_Control (pDX,IDC_EXTRAS,m_extras);
	DDX_Control (pDX,IDC_MOTIVESCOMBO,m_MotiveCombo);	
	//DDX_Control(pDX,IDC_CURRENTGROUP,m_activeGrpCombo);
	DDX_Control(pDX,IDC_LOADTIMESTART,m_loadStartTm);
	DDX_Control(pDX,IDC_LOADTIMEEND,m_loadEndTm);
	DDX_Control(pDX,IDC_AGENTCOMBO,m_AgentCb);
	DDX_Control(pDX,IDC_HENTBTN,m_hentbtn);
	DDX_Control(pDX,IDC_COWCOUNT,m_itemCount);
	
}

BOOL  SellDocDlg::OnInitDialog()
{
	CSessionAwareDialog::OnInitDialog();
	
	SendDlgItemMessage(IDC_PLATENO,EM_LIMITTEXT,MAXPLATE);
	
	//m_listMenu.LoadMenu(IDR_COWLISTPOPMENU);
	//ModifyListMenu(m_listMenu);

	//m_activeGrpCombo.InitCombo();

	CWnd* pCancelWnd = GetDlgItem(IDCANCEL);
	m_loadStartTm.SetAbortWnd(pCancelWnd);
	m_loadEndTm.SetAbortWnd(pCancelWnd);

	DateTime nowDt = DateTime::now();
	m_DocDate.SetDate(nowDt);
	m_TransportDate.SetDate(nowDt);

	SeasonSession* session = GetSeasonSession();

	DefaultValuesEditor editor(session->GetContext());
	editor.Load(m_defaultValues);

	AgentsComboContent agents;
	agents.CreateContent(session);

	m_AgentCb.SetContent(agents);
	m_MotiveCombo.InitCombo(MotiveDirection_Out);

	if (m_defaultValues.bSetOutMotive)
	{
		m_MotiveCombo.SelectMotive(m_defaultValues.outmotive);
	}

	if(m_transaction->IsUpdating())
	{
		
		SetWindowText((boost::wformat(StringLoader(IDS_EDITSELLDOCDLGCAPTION)) % DocNoText(m_transaction->GetDocKey()).ToString()).str().c_str());

		const DocDetails& docDetails = m_transaction->GetDetails();		
		m_loadEndTm.SetTime(docDetails.GetLoadEndDtTm());
		m_loadStartTm.SetTime(docDetails.GetLoadStartDtTm());
		m_AgentCb.Select(docDetails.GetAgentId());
		m_extras.SetText(docDetails.GetExtras());
		m_plateNo = docDetails.GetPlateNo();
		m_TransportDate.SetDate(docDetails.GetLoadDate());
		m_DocDate.SetDate(docDetails.GetDocDate());
		m_MotiveCombo.SelectMotive(docDetails.GetMotive());
		m_hentbtn.SetHent(m_transaction->GetHent());
	}
	else
	{
		uint32_t herdId = m_transaction->GetHerdId();
		IHerd* pHerd = session->GetHerd(herdId);
		std::wstring herdName = SafeGet(&IHerd::GetHerdName,pHerd,L"");
		SetWindowText((boost::wformat(StringLoader(IDS_NEWSELLDOCDLGCAPTION).c_str()) % herdName).str().c_str());	

	}

	m_TransactionContent.Create(session,m_transaction->GetFacade());
	m_cowlist.SetContent(&m_TransactionContent);	
		
	UpdateData(FALSE);
	return TRUE;
}

void SellDocDlg::OnDestroy()
{
	CSessionAwareDialog::OnDestroy();
	m_TransactionContent.Destroy();
}


void SellDocDlg::OnOK()
{
	UpdateData();

	if(m_transaction->GetItemCount() == 0)
	{
		AfxMessageBox(IDS_EMPTYCOWLIST,MB_OK | MB_ICONERROR,0);
		return;
	}

	if(m_DocDate.IsInputEmpty()) 
	{
			AfxMessageBox(IDS_ERRNODOCDATE,MB_OK | MB_ICONERROR,0);
			return;
	}

	if(m_TransportDate.IsInputEmpty()) 
	{
			AfxMessageBox(IDS_ERRNOTRANSAPORTDATE,MB_OK | MB_ICONERROR,0);
			return;
	}
	
	if(m_MotiveCombo.GetSelectedMotive() == Motive_None)
	{
		AfxMessageBox(IDS_ERRNOINMOTIVE,MB_OK | MB_ICONERROR,0);
		return;
	}

	if(m_hentbtn.GetHent() == NULL_ID)
	{
		AfxMessageBox(IDS_ERRNOHENT,MB_OK | MB_ICONERROR,0);
		return;
	}
	
	DocDetails details;
	SaveDocDetails(details);
	m_transaction->UpdateDetails(details);
	m_transaction->SetHent(m_hentbtn.GetHent());

	try
	{
		CWaitDialog waitDlg(this,StringLoader(IDS_WAITSAVING));

		waitDlg.WaitForTask([this]()
							{
								m_transaction->Commit();
							});
		
		EndDialog(IDOK);
	}
	catch(TransactionException& e)
	{
		AfxMessageBox(e.message().c_str(),MB_OK|MB_ICONERROR);
	}
	
	
}

void SellDocDlg::SaveDocDetails(DocDetails& details)
{
	details.SetDocDate(m_DocDate.GetDate());
	details.SetLoadDate(m_TransportDate.GetDate());
	details.SetExtras(m_extras.GetText());
	details.SetPlateNo(m_plateNo);
	details.SetMotive(m_MotiveCombo.GetSelectedMotive());	
	details.SetLoadStartDtTm(m_loadStartTm.GetTime());
	details.SetLoadEndDtTm(m_loadEndTm.GetTime());	
	details.SetAgentId(m_AgentCb.GetSelectedItemId());
}

//void SellDocDlg::OnDblClick(NMHDR* pNMHDR, LRESULT* pResult)
//{
//	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//	
//	if (pNMIA && pNMIA->iItem >= 0)
//	{
//		EditCow(pNMIA->iItem);	
//		m_cowlist->Invalidate();
//	}
//	*pResult = 0;
//}
//
//void SellDocDlg::OnDeleteCow()
//{
//	int item = m_cowlist->GetCurSel();
//	if(item >= 0)
//	{
//		m_cowlist->SetRedraw(FALSE);
//		DeleteCow(item);
//		m_cowlist->SetRedraw(TRUE);
//		UpdateCowCount();
//	}
//}
//
//void SellDocDlg::UpdateCowCount()
//{
//	m_cowlist->UpdateItemCount();	
//	SetDlgItemInt(IDC_COWCOUNT,m_cows.GetSize());
//}
//
void SellDocDlg::SetPlateNo(const std::wstring& sPlateNo)
{
	SetDlgItemText(IDC_PLATENO,sPlateNo.c_str());
}
//
//void SellDocDlg::OnGetCows()
//{
//	GetCows();
//}
//
// void SellDocDlg::OnCancel()
// {
//	 CancelDoc();
//	 CDialog::OnCancel();
// }
//
//void SellDocDlg::OnRClick(NMHDR* pNMHDR, LRESULT* pResult)
//{
//	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//
//	if (pNMIA && pNMIA->iItem >= 0)
//	{
//		CPoint ptCursor;
//		GetCursorPos(&ptCursor);
//		RightClick(ptCursor);				
//	}
//	*pResult = 0;
//}
//
//void SellDocDlg::RightClick(CPoint &ptCursor)
//{
//	CMenu* popup = m_listMenu.GetSubMenu(0);
//	popup->TrackPopupMenu(TPM_LEFTALIGN,ptCursor.x,ptCursor.y,this,NULL);
//}
//
//
//void SellDocDlg::OnMenuEditCow()
//{
//	int item = m_cowlist->GetCurSel();
//	if(item >= 0)
//	{
//		EditCow(item);
//		m_cowlist->Invalidate();
//	}	
//}
//
//void SellDocDlg::OnMenuShowPass()
//{
//	int item = m_cowlist->GetCurSel();
//	if(item >= 0)
//	{
//		ShowPass(item);
//	}
//	
//}
//
//void SellDocDlg::OnFindPass()
//{
//	FindPass();
//}
//
//
//
//
//void SellDocDlg::UpdateSummary()
//{
//	m_GrpSummaryList.UpdateSummary();
//}
//int SellDocDlg::GetActiveGroup()
//{
//	return m_activeGrpCombo.GetSelOutGroup();	
//}
//void SellDocDlg::ModifyListMenu(CMenu &listMenu)
//{
//	CMenu grpMenu;
//	grpMenu.CreatePopupMenu();
//	CString menuStr;
//	for(int i = MAX_INOUT_GRP;i >= 0 ;i--)
//	{
//		menuStr.Format("%d",i);
//		grpMenu.InsertMenu(0, MF_BYPOSITION,IDM_OUTGRPMENU_BASE+i ,(LPCSTR)menuStr);	
//	}
//
//	menuStr.LoadStringA(IDS_NOINOUTGRP);
//	grpMenu.InsertMenu(0, MF_BYPOSITION,IDM_OUTGRPMENU_NOGRP ,(LPCSTR)menuStr);
//
//	menuStr.LoadString(IDS_OUTDOC_CHNGGRP_MENUTEXT);
//	CMenu* pSubMenu = m_listMenu.GetSubMenu(0);
//	pSubMenu->InsertMenu(-1,MF_BYPOSITION | MF_POPUP, (UINT)grpMenu.GetSafeHmenu(), (LPCSTR)menuStr);
//}
//
//void SellDocDlg::OnGrpChange(UINT grpItemId)
//{
//	
//	POSITION pos;
//	int selItem;
//	Cow *pCow;
//	
//		pos = m_cowlist->GetFirstSelectedItemPosition();
//		while(pos)
//		{
//			selItem = m_cowlist->GetNextSelectedItem(pos);
//			pCow = m_cows[selItem];
//
//			if(grpItemId == IDM_OUTGRPMENU_NOGRP)
//				ChangeGrp4Cow(pCow,NULL_INOUT_GRP);
//			else if((grpItemId-IDM_OUTGRPMENU_BASE) <= MAX_INOUT_GRP)
//			{	
//				ChangeGrp4Cow(pCow,(grpItemId-IDM_OUTGRPMENU_BASE));
//			}
//		}	
//		m_cowlist->Invalidate();
//	
//	UpdateSummary();
//
//}
//

void SellDocDlg::OnAddCow()
{
	TransactionSelectInterface selectInterface;
	SeasonSession* session = GetSeasonSession();	
	selectInterface.Create(session,m_transaction->GetHerdId(),m_transaction->GetFacade());

	CowSelectDlg cowSelectDlg(&selectInterface,this);
	cowSelectDlg.SetHeader(CowInStockPresenceListHeader());
	cowSelectDlg.DoModal();

	selectInterface.Destroy();
}

void SellDocDlg::OnHent()
{
	HentSearchDlg hentSearchDlg(this);
	uint32_t hentId = hentSearchDlg.FindHent();
	if (hentId != NULL_ID)
	{
		m_hentbtn.SetHent(hentId);
	}
}

void SellDocDlg::OnSetLoadTimeStart()
{
	m_loadStartTm.SetTime(Time::now());
}

void SellDocDlg::OnSetLoadTimeEnd()
{
	m_loadEndTm.SetTime(Time::now());
}

void SellDocDlg::OnSetPlateNo()
{
	MenuBuilder plateMenu;

	std::wstring sItemTextFormat = L"%s - %s";
	SeasonSession* session = GetSeasonSession();
	uint32_t hentId = m_hentbtn.GetHent();
	uint32_t agentId = m_AgentCb.GetSelectedItemId();
	if(hentId != NULL_ID)
	{
		
		HentClientObject* pHentCO = session->GetHent(hentId);
		if(pHentCO)
		{
			IHent* pHent = pHentCO->Object();
			std::wstring sPlateNo = pHent->GetPlateNo();
			if(!sPlateNo.empty())
			{
				std::wstring sItemText = (boost::wformat(sItemTextFormat) % pHent->GetAlias() % sPlateNo).str();
				plateMenu.add(sItemText,std::bind(&SellDocDlg::SetPlateNo,this,sPlateNo));
			}
		}
					
	}

	
	if(agentId != NULL_ID)
	{		
		IAgent* pAgent = session->GetAgent(agentId);
		if(pAgent)
		{
			std::wstring sPlateNo = pAgent->GetPlateNo();
			if(!sPlateNo.empty())
			{
				std::wstring sItemText = (boost::wformat(sItemTextFormat) % pAgent->GetAgentCode() % sPlateNo).str();
				plateMenu.add(sItemText,std::bind(&SellDocDlg::SetPlateNo,this,sPlateNo));
			}
		}					
	}

	
	if(plateMenu.itemCount() == 0)
	{
		plateMenu.add(StringLoader(IDS_NO_PLATE_NUMBERS),[]{});	
	}


	CRect rc;
	GetDlgItem(IDC_SETPLATENOMENUBTN)->GetWindowRect(rc);
	int x = rc.left;
	int y = rc.top;

	plateMenu.trackAt(x,y,this);
	
}


void SellDocDlg::OnDeleteCow()
{
	int row = m_cowlist.GetCurSel();
	if(row >= 0)
	{
		const DocItemRow* item = m_TransactionContent.GetItemRowAt(row);
		ExceptionMsgBox().TryCatchAny([this,item]()
									  {										  
										  m_transaction->DeleteCow(item->Content()->GetItemId());
									  });
	}
}

void SellDocDlg::OnFindPassports()
{
	TransactionPassportCollection collection(m_transaction->GetFacade());
	CPassportSearchDlg passSearchDlg(&collection,this);
	passSearchDlg.DoModal();
}

void SellDocDlg::OnClearAgent()
{
	m_AgentCb.SetCurSel(-1);
}

void SellDocDlg::OnEditHent(NMHDR* pNMHDR, LRESULT* pResult)
{
	uint32_t hentId = m_hentbtn.GetHent();
	SeasonSession* session = GetSeasonSession();

	ExceptionMsgBox msgBox;

	Try()
		.Prepare([session, hentId]
	{
		session->LockHent(hentId);
	})
		.Do([this, session, hentId]
	{
		HentEditor editor(session);
		editor.Edit(hentId);
		HentDlg hendDlg(&editor, this);
		hendDlg.DoModal();
	})
		.TearDown([session, hentId]
	{
		session->UnlockHent(hentId);
	})
		.Catch([&msgBox](std::exception& e)
	{
		msgBox.Show(e);
	})
		.Run();
}