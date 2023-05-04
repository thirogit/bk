#include "stdafx.h"
#include "BuyDocDlg.h"
#include <data\datalimits.h>
#include "../resource.h"
#include "../utils/StringLoader.h"
#include <string\TextUtils.h>
#include "../text/DocNoText.h"
#include <boost/format.hpp>
#include "../context/transaction/TransactionNewCowEditor.h"
#include "../context/transaction/TransactionCowEditor.h"
#include "../cow/CowDlg.h"
#include "../ui/waitdlg\WaitDialog.h"
#include "../context/transaction/TransactionException.h"
#include "../hent/HentSearchDlg.h"
#include <utils/SafeGet.h>
#include "../ui/ExceptionMsgBox.h"
#include "../content/AgentsComboContent.h"
#include "../ui/cell/DecimalCellBuilder.h"
#include "../uiconstants/PrecisionContstants.h"
#include "../content/ClassesComboContent.h"
#include "../ui/cell/ComboCellBuilder.h"
#include "../passportsegregation/PassportSearchDlg.h"
#include "TransactionPassportCollection.h"
#include "../cow/ScanCowBCdsWizard.h"
#include "../hent/HentEditor.h"
#include "../hent/HentDlg.h"
#include <data\types\countries\Countries.h>
#include "../ui/menu/MenuBuilder.h"
#include "../context/transaction/DefaultValuesApplier.h"
#include "../settings/DefaultValuesEditor.h"

BEGIN_MESSAGE_MAP(BuyDocDlg, CSessionAwareDialog )
	
			ON_BN_CLICKED(IDC_ADDCOW, OnAddCow)			
			ON_WM_DESTROY()
			ON_BN_CLICKED(IDC_HENTBTN,OnHent)
			ON_BN_CLICKED(IDC_COWDELETE,OnDeleteCow)
			ON_BN_CLICKED(IDC_SETPLATENOMENUBTN, OnSetPlateNo)
						
			//ON_BN_CLICKED(IDC_GETCOWS,OnGetCows)			
			//ON_NOTIFY(NM_RCLICK, IDC_COWLIST, OnRClick)			
			//ON_COMMAND(IDM_COWLIST_EDITCOW,OnMenuEditCow)
			//ON_COMMAND(IDM_COWLIST_SHOWPASSIMG,OnMenuShowPass)
			ON_BN_CLICKED(IDC_FINDPASSPORTS,OnFindPassports)
			//ON_COMMAND_RANGE(IDM_OUTGRPMENU_BASE,IDM_OUTGRPMENU_MAX,OnGrpChange)
			//ON_COMMAND_RANGE(IDM_OUTGRPMENU_NOGRP,IDM_OUTGRPMENU_NOGRP,OnGrpChange)

			ON_BN_CLICKED(IDC_SETLOADTIMESTART,OnSetLoadTimeStart)
			ON_BN_CLICKED(IDC_SETLOADTIMEEND,OnSetLoadTimeEnd)
			ON_BN_CLICKED(IDC_SETPLATENO,OnSetPlateNo)
			ON_BN_CLICKED(IDC_FINDPASSPORTS,OnFindPassports)

			ON_BN_CLICKED(IDC_SCANCOW, OnScanCow)			
			ON_BN_CLICKED(IDC_CLEARAGENT,OnClearAgent)
			ON_NOTIFY(BN_EDITHENT, IDC_HENTBTN, OnEditHent)

END_MESSAGE_MAP()

BuyDocDlg::BuyDocDlg(BuyDocTransactionPtr transaction,CWnd* pParent) : CSessionAwareDialog(BuyDocDlg::IDD,pParent),
	m_extras(MAXEXTRAS),
	m_itemCount(transaction->GetFacade())
{
	m_transaction = transaction;
}

BuyDocDlg::~BuyDocDlg()
{
}


void BuyDocDlg::DoDataExchange(CDataExchange* pDX)
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
	DDX_Control(pDX, IDC_SCANCOW, m_ScanBtn);
	
	
}

BOOL  BuyDocDlg::OnInitDialog()
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
	m_MotiveCombo.InitCombo(MotiveDirection_In);

	if (m_defaultValues.bSetInMotive)
	{
		m_MotiveCombo.SelectMotive(m_defaultValues.inmotive);
	}

	if(m_transaction->IsUpdating())
	{
		
		SetWindowText((boost::wformat(StringLoader(IDS_EDITBUYDOCDLGCAPTION)) % DocNoText(m_transaction->GetDocKey()).ToString()).str().c_str());

		const DocDetails& docDetails = m_transaction->GetDetails();		
		m_loadEndTm.SetTime(docDetails.GetLoadEndDtTm());
		m_loadStartTm.SetTime(docDetails.GetLoadStartDtTm());
		m_AgentCb.Select(docDetails.GetAgentId());
		m_extras.SetText(docDetails.GetExtras());
		m_plateNo = docDetails.GetPlateNo();
		m_TransportDate.SetDate(docDetails.GetLoadDate());
		m_DocDate.SetDate(docDetails.GetDocDate());
		m_MotiveCombo.SelectMotive(docDetails.GetMotive());
		m_hentbtn.SetHent(m_transaction->GetHentId());
	}
	else
	{
		uint32_t herdId = m_transaction->GetHerdId();
		IHerd* pHerd = session->GetHerd(herdId);
		std::wstring herdName = SafeGet(&IHerd::GetHerdName,pHerd,L"");
		 
		SetWindowText((boost::wformat(StringLoader(IDS_NEWBUYDOCDLGCAPTION).c_str()) % herdName).str().c_str());		
	}

	m_TransactionContent.Create(session,m_transaction->GetFacade());
	m_cowlist.SetContent(&m_TransactionContent);	

	std::function<void (int,uint32_t,CellDescriptor& cell)> dblClickHandler = 
		[this](int row,uint32_t colId,CellDescriptor& cell)
		{
			OnCellDblClick(row,colId,cell.GetRect());			
		};

	m_cowlist.SetCellDblClickHandler(dblClickHandler);

	m_ScanBtn.SetImage(IDPNG_YESBARCODE);
	
	

	UpdateData(FALSE);
	
	return TRUE;
}

void BuyDocDlg::OnDestroy()
{
	CSessionAwareDialog::OnDestroy();
	m_TransactionContent.Destroy();
}

void BuyDocDlg::OnOK()
{
	UpdateData();

	if(m_transaction->GetItemCount() == 0)
	{
		AfxMessageBox(IDS_EMPTYCOWLIST,MB_OK | MB_ICONERROR,0);
		return;
	}

	if(m_hentbtn.GetHent() == NULL_ID)
	{
		AfxMessageBox(IDS_ERRMISSINGHENT,MB_OK | MB_ICONERROR,0);
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
		AfxMessageBox(e.wwhat());
	}
	
	
}

void BuyDocDlg::SaveDocDetails(DocDetails& details)
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

void BuyDocDlg::OnHent()
{
	HentSearchDlg hentSearchDlg(this);
	uint32_t hentId = hentSearchDlg.FindHent();
	if (hentId != NULL_ID)
	{
		m_hentbtn.SetHent(hentId);
	}
}

void BuyDocDlg::OnCellDblClick(int row,uint32_t colId,const CRect& rect)
{
	const DocItem* item = m_transaction->GetItemAt(row);

	switch(colId)
	{
	case DocItemViewMember::DocItemView_Weight:
		{
			DecimalCellBuilder weightCell;		
			weightCell.precision(WEIGHT_PREC)
				.receiver(std::bind(&BuyDocDlg::OnItemWeight,this,row,std::placeholders::_1))
				.value(item->GetCowEntry().GetWeight())
				.show(&m_cowlist,rect);
			break;
		}
	case DocItemViewMember::DocItemView_Class:
		{
			ClassesComboContent content;			
			content.CreateContent(GetSeasonSession());
			ComboCellBuilder classCell;
			classCell.content(content)
					 .receiver(std::bind(&BuyDocDlg::OnItemClass,this,row,std::placeholders::_1))
					 .select(item->GetCowEntry().GetClassId())
					 .show(&m_cowlist,rect);
		}
		break;

	case DocItemViewMember::DocItemView_Stock:
		break;
	default:
		DocItemRow* pRow = m_TransactionContent.GetItemRowAt(row);			
		EditCow(pRow->Content()->GetItemId());
		break;

	}
}

void BuyDocDlg::OnItemClass(int row,uint32_t classId)
{
	DocItemRow* pRow = m_TransactionContent.GetItemRowAt(row);			
	m_transaction->GetFacade()->UpdateItemClass(pRow->Content()->GetItemId(),classId);
}


void BuyDocDlg::OnItemWeight(int row,const NullDecimal& weight)
{
	DocItemRow* pRow = m_TransactionContent.GetItemRowAt(row);			
	m_transaction->GetFacade()->UpdateItemWeight(pRow->Content()->GetItemId(),weight);
}

void BuyDocDlg::OnDeleteCow()
{
	int row = m_cowlist.GetCurSel();
	if(row >= 0)
	{
		DocItemRow* pItemRow = m_TransactionContent.GetItemRowAt(row);

		ExceptionMsgBox().TryCatchAny([pItemRow,this]()
		{
			m_transaction->DeleteCow(pItemRow->Content()->GetItemId());
		});
	}
}

//
//void BuyDocDlg::OnRClick(NMHDR* pNMHDR, LRESULT* pResult)
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
//void BuyDocDlg::RightClick(CPoint &ptCursor)
//{
//	CMenu* popup = m_listMenu.GetSubMenu(0);
//	popup->TrackPopupMenu(TPM_LEFTALIGN,ptCursor.x,ptCursor.y,this,NULL);
//}
//
//
//void BuyDocDlg::OnMenuEditCow()
//{
//	int item = m_cowlist->GetCurSel();
//	if(item >= 0)
//	{
//		EditCow(item);
//		m_cowlist->Invalidate();
//	}	
//}
//
//void BuyDocDlg::OnMenuShowPass()
//{
//	int item = m_cowlist->GetCurSel();
//	if(item >= 0)
//	{
//		ShowPass(item);
//	}
//	
//}
//
//void BuyDocDlg::OnFindPass()
//{
//	FindPass();
//}
//
//NullDoc* BuyDocDlg::CreateNewDoc()
//{
//	NullDoc *pDoc = new NullDoc();
//	pDoc->SetPlateNo(m_plateNo);
//
//	CDateTime docDt;
//	m_DocDate.GetDate(docDt);
//	pDoc->SetDocDate(docDt);
//
//	CDateTime transportDt;
//	m_TransportDate.GetDate(transportDt);
//	pDoc->SetLoadDate(transportDt);
//	
//	CString sExtras;
//	m_extras.GetWindowText(sExtras);
//	pDoc->SetExtras(sExtras);
//
//	pDoc->SetHent(m_hentCombo.GetSelectedHent());
//	pDoc->SetMotive(m_MotiveCombo.GetSelectedMotive());
//
//	pDoc->SetLoadStartTime(m_loadStartTm.GetTime());
//	pDoc->SetLoadEndTime(m_loadEndTm.GetTime());
//
//	pDoc->SetAgent(m_AgentCb.GetSelItemData());
//	return pDoc;
//
//}
//
//
//
//void BuyDocDlg::UpdateSummary()
//{
//	m_GrpSummaryList.UpdateSummary();
//}
//int BuyDocDlg::GetActiveGroup()
//{
//	return m_activeGrpCombo.GetSelOutGroup();	
//}
//void BuyDocDlg::ModifyListMenu(CMenu &listMenu)
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
//void BuyDocDlg::OnGrpChange(UINT grpItemId)
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

void BuyDocDlg::EditCow(ItemId cowItemId)
{
	TransactionCowEditor editor(m_transaction->GetFacade());
	editor.Edit(cowItemId);
	CowDlg cowDlg(&editor,this);
	cowDlg.DoModal();
}

void BuyDocDlg::OnAddCow()
{
	TransactionNewCowEditor editor(m_transaction->GetFacade());	

	DefaultValuesApplier applier(m_defaultValues);
	applier.ApplyTo(&editor);

	CowDlg cowDlg(&editor,this);
	cowDlg.DoModal();
	
}

void BuyDocDlg::OnSetLoadTimeStart()
{
	m_loadStartTm.SetTime(Time::now());
}

void BuyDocDlg::OnSetLoadTimeEnd()
{
	m_loadEndTm.SetTime(Time::now());
}


void BuyDocDlg::OnFindPassports()
{
	TransactionPassportCollection collection(m_transaction->GetFacade());
	CPassportSearchDlg passSearchDlg(&collection,this);
	passSearchDlg.DoModal();
}

void BuyDocDlg::OnScanCow()
{
	ScanCow();
}

void BuyDocDlg::ScanCow()
{
	SeasonSession* session = GetSeasonSession();
	TransactionNewCowEditor cowEditor(m_transaction->GetFacade());

	ScanCowBCdsWizard wizard(this);
	//wizard.SetAssumedCountry(country);
	ScanWizardResult result = wizard.DoMagic();

	CowNo cowNo = result.GetCowNo();
	if (!cowNo.IsNull())
	{
		cowEditor.SetCowNo(cowNo);

		HentNo firstOwnerNo = result.GetHentNo();
		if (!firstOwnerNo.IsNull())
		{
			HentClientObject* pFirstOwnerCO = session->GetHent(firstOwnerNo);
			if (!pFirstOwnerCO)
			{
				HentEditor hentEditor(session);
				hentEditor.New();
				hentEditor.SetHentNo(firstOwnerNo);

				HentDlg hendDlg(&hentEditor, this);
				if (hendDlg.DoModal() == IDOK)
				{
					cowEditor.SetFirstOwnerId(hentEditor.GetHentId());
				}
			}
			else
			{
				cowEditor.SetFirstOwnerId(pFirstOwnerCO->Object()->GetId());
			}
		}


		CowDlg cowDlg(&cowEditor, this);
		cowDlg.DoModal();
	}
}

void BuyDocDlg::OnSetPlateNo()
{
	MenuBuilder plateMenu;

	std::wstring sItemTextFormat = L"%s - %s";
	SeasonSession* session = GetSeasonSession();
	uint32_t hentId = m_hentbtn.GetHent();
	uint32_t agentId = m_AgentCb.GetSelectedItemId();
	if (hentId != NULL_ID)
	{

		HentClientObject* pHentCO = session->GetHent(hentId);
		if (pHentCO)
		{
			IHent* pHent = pHentCO->Object();
			std::wstring sPlateNo = pHent->GetPlateNo();
			if (!sPlateNo.empty())
			{
				std::wstring sItemText = (boost::wformat(sItemTextFormat) % pHent->GetAlias() % sPlateNo).str();
				plateMenu.add(sItemText, std::bind(&BuyDocDlg::SetPlateNo, this, sPlateNo));
			}
		}

	}


	if (agentId != NULL_ID)
	{
		IAgent* pAgent = session->GetAgent(agentId);
		if (pAgent)
		{
			std::wstring sPlateNo = pAgent->GetPlateNo();
			if (!sPlateNo.empty())
			{
				std::wstring sItemText = (boost::wformat(sItemTextFormat) % pAgent->GetAgentCode() % sPlateNo).str();
				plateMenu.add(sItemText, std::bind(&BuyDocDlg::SetPlateNo, this, sPlateNo));
			}
		}
	}


	if (plateMenu.itemCount() == 0)
	{
		plateMenu.add(StringLoader(IDS_NO_PLATE_NUMBERS), []{});
	}


	CRect rc;
	GetDlgItem(IDC_SETPLATENOMENUBTN)->GetWindowRect(rc);
	int x = rc.left;
	int y = rc.top;

	plateMenu.trackAt(x, y, this);

}

void BuyDocDlg::SetPlateNo(const std::wstring& sPlateNo)
{
	SetDlgItemText(IDC_PLATENO, sPlateNo.c_str());
}


void BuyDocDlg::OnClearAgent()
{
	m_AgentCb.SetCurSel(-1);
}

void BuyDocDlg::OnEditHent(NMHDR* pNMHDR, LRESULT* pResult)
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