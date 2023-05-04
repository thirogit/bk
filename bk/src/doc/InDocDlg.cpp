#include "stdafx.h"
#include "InDocDlg.h"
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
#include <utils/SafeGet.h>
#include "../ui/ExceptionMsgBox.h"
#include "../context/transaction/TransactionCowEditor.h"
#include "../ui/cell/DecimalCellBuilder.h"
#include "../ui/cell/ComboCellBuilder.h"
#include "../uiconstants/PrecisionContstants.h"
#include "../content/AgentsComboContent.h"
#include "../content/ClassesComboContent.h"
#include "../passportsegregation/PassportSearchDlg.h"
#include "TransactionPassportCollection.h"
#include "../passportrecognition/settings/PassportRecognizerSettingsStorage.h"
#include "../passportrecognition/ui/settings/PassportRecognizerCfgDlg.h"
#include "../passportrecognition/ui/PassportRecognitionDlg.h"
#include "../passportrecognition/session/PlusDocPassportRecognitionSession.h"
#include "../cow/ScanCowBCdsWizard.h"
#include "../hent/HentEditor.h"
#include "../hent/HentDlg.h"
#include <data\types\countries\Countries.h>
#include "../context/transaction/DefaultValuesApplier.h"
#include "../settings/DefaultValuesEditor.h"


BEGIN_MESSAGE_MAP(InDocDlg, CSessionAwareDialog )
	
			ON_BN_CLICKED(IDC_ADDCOW, OnAddCow)			
			ON_WM_DESTROY()
			ON_BN_CLICKED(IDC_COWDELETE,OnDeleteCow)
			
			ON_BN_CLICKED(IDC_GETCOWS, OnPassRecgntn)
			ON_COMMAND(IDM_RECOGNIZEPASSPORTS_UPDATE, OnPassRecgntnUpdate)
			ON_COMMAND(IDM_RECOGNIZEPASSPORTS_CONFIGURE, OnPassRecgntnSettings )

			ON_BN_CLICKED(IDC_FINDPASSPORTS,OnFindPassports)
			ON_BN_CLICKED(IDC_SETLOADTIMESTART,OnSetLoadTimeStart)
			ON_BN_CLICKED(IDC_SETLOADTIMEEND,OnSetLoadTimeEnd)
			ON_BN_CLICKED(IDC_SETPLATENO,OnSetPlateNo)	
			ON_BN_CLICKED(IDC_SCANCOW, OnScanCow)			
			ON_BN_CLICKED(IDC_CLEARAGENT, OnClearAgent)
			
END_MESSAGE_MAP()

InDocDlg::InDocDlg(InDocTransactionPtr transaction,CWnd* pParent) : 
	CSessionAwareDialog(InDocDlg::IDD,pParent),
	m_extras(MAXEXTRAS),
	m_itemCount(transaction->GetFacade())
{
	m_transaction = transaction;
}

InDocDlg::~InDocDlg()
{
}


void InDocDlg::DoDataExchange(CDataExchange* pDX)
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
	DDX_Control(pDX,IDC_COWCOUNT,m_itemCount);
	DDX_Control(pDX, IDC_GETCOWS, m_GetCowsBtn);
	DDX_Control(pDX, IDC_SCANCOW, m_ScanBtn);

	
}

BOOL  InDocDlg::OnInitDialog()
{
	CSessionAwareDialog::OnInitDialog();
//	m_dlgshown = false;

	
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
		
		SetWindowText((boost::wformat(StringLoader(IDS_EDITINDOCDLGCAPTION)) % DocNoText(m_transaction->GetDocKey()).ToString()).str().c_str());

		const DocDetails& docDetails = m_transaction->GetDetails();		
		m_loadEndTm.SetTime(docDetails.GetLoadEndDtTm());
		m_loadStartTm.SetTime(docDetails.GetLoadStartDtTm());
		m_AgentCb.Select(docDetails.GetAgentId());
		m_extras.SetText(docDetails.GetExtras());
		m_plateNo = docDetails.GetPlateNo();
		m_TransportDate.SetDate(docDetails.GetLoadDate());
		m_DocDate.SetDate(docDetails.GetDocDate());
		m_MotiveCombo.SelectMotive(docDetails.GetMotive());
	}
	else
	{
		uint32_t herdId = m_transaction->GetHerdId();
		IHerd* pHerd = session->GetHerd(herdId);
		std::wstring herdName = SafeGet(&IHerd::GetHerdName,pHerd,L"");
		SetWindowText((boost::wformat(StringLoader(IDS_NEWINDOCDLGCAPTION).c_str()) % herdName).str().c_str());
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

void InDocDlg::OnCellDblClick(int row,uint32_t colId,const CRect& rect)
{
	const DocItem* item = m_transaction->GetItemAt(row);

	switch(colId)
	{
	case DocItemViewMember::DocItemView_Weight:
		{
			
			DecimalCellBuilder weightCell;		
			weightCell.precision(WEIGHT_PREC)
					  .receiver(std::bind(&InDocDlg::OnItemWeight,this,row,std::placeholders::_1))
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
					 .receiver(std::bind(&InDocDlg::OnItemClass,this,row,std::placeholders::_1))
					 .select(item->GetCowEntry().GetClassId())
					 .show(&m_cowlist,rect);
		}
		break;

	// DocItemViewMember::DocItemView_Stock:
	//	break;
	default:
		DocItemRow* pRow = m_TransactionContent.GetItemRowAt(row);			
		EditCow(pRow->Content()->GetItemId());
		break;

	}
}

void InDocDlg::OnItemClass(int row,uint32_t classId)
{
	DocItemRow* pRow = m_TransactionContent.GetItemRowAt(row);			
	m_transaction->GetFacade()->UpdateItemClass(pRow->Content()->GetItemId(),classId);
}

void InDocDlg::OnItemWeight(int row,const NullDecimal& weight)
{
	DocItemRow* pRow = m_TransactionContent.GetItemRowAt(row);			
	m_transaction->GetFacade()->UpdateItemWeight(pRow->Content()->GetItemId(),weight);
}

void InDocDlg::OnDestroy()
{
	CSessionAwareDialog::OnDestroy();
	m_TransactionContent.Destroy();
}

void InDocDlg::OnOK()
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
	
	DocDetails details;
	SaveDocDetails(details);
	m_transaction->UpdateDetails(details);

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

void InDocDlg::SaveDocDetails(DocDetails& details)
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

void InDocDlg::OnDeleteCow()
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

void InDocDlg::EditCow(ItemId cowItemId)
{
	TransactionCowEditor editor(m_transaction->GetFacade());
	editor.Edit(cowItemId);
	CowDlg cowDlg(&editor,this);
	cowDlg.DoModal();
}

//
//void InDocDlg::UpdateCowCount()
//{
//	m_cowlist->UpdateItemCount();	
//	SetDlgItemInt(IDC_COWCOUNT,m_cows.GetSize());
//}
//
void InDocDlg::OnPassRecgntn()
{
	PassportRecognizerSettings recognizerSettings;
	PassportRecognizerSettingsStorage settingsStorage;
	settingsStorage.Load(recognizerSettings);


	PlusDocPassportRecognitionSession session(&m_passImgCache, m_transaction->GetFacade());

	PassportRecognitionDlg passportRecognitionDlg(&session, &recognizerSettings, this);
	passportRecognitionDlg.DoModal();
}

void InDocDlg::OnPassRecgntnSettings()
{
	PassportRecognizerSettings recognizerSettings;
	PassportRecognizerSettingsStorage settingsStorage;
	settingsStorage.Load(recognizerSettings);
	CPassportRecognizerCfgDlg recognizerCfgDlg(&recognizerSettings, this);
	if (recognizerCfgDlg.DoModal() == IDOK)
	{
		settingsStorage.Save(recognizerSettings);
	}
}

void InDocDlg::OnPassRecgntnUpdate()
{

}
//
//
//void InDocDlg::OnRClick(NMHDR* pNMHDR, LRESULT* pResult)
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
//
//
//void InDocDlg::OnMenuEditCow()
//{
//	int item = m_cowlist->GetCurSel();
//	if(item >= 0)
//	{
//		EditCow(item);
//		m_cowlist->Invalidate();
//	}	
//}
//
//void InDocDlg::OnMenuShowPass()
//{
//	int item = m_cowlist->GetCurSel();
//	if(item >= 0)
//	{
//		ShowPass(item);
//	}
//	
//}
//
//void InDocDlg::OnFindPass()
//{
//	FindPass();
//}
//
//
//
//void InDocDlg::UpdateSummary()
//{
//	m_GrpSummaryList.UpdateSummary();
//}
//int InDocDlg::GetActiveGroup()
//{
//	return m_activeGrpCombo.GetSelOutGroup();	
//}
//void InDocDlg::ModifyListMenu(CMenu &listMenu)
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
//void InDocDlg::OnGrpChange(UINT grpItemId)
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

void InDocDlg::OnAddCow()
{
	TransactionNewCowEditor editor(m_transaction->GetFacade());	

	DefaultValuesApplier applier(GetSeasonSession());
	applier.ApplyTo(&editor);

	CowDlg cowDlg(&editor,this);
	cowDlg.DoModal();	
}

void InDocDlg::OnSetLoadTimeStart()
{
	m_loadStartTm.SetTime(Time::now());
}

void InDocDlg::OnSetLoadTimeEnd()
{
	m_loadEndTm.SetTime(Time::now());
}

void InDocDlg::OnSetPlateNo()
{
	uint32_t agentId = m_AgentCb.GetSelectedItemId();
	if(agentId != NULL_ID)
	{
		SeasonSession* session = GetSeasonSession();		
		IAgent* pAgent = session->GetAgent(agentId);
		if(pAgent != NULL)
		{			
			std::wstring sPlateNo = pAgent->GetPlateNo();
			if(!sPlateNo.empty())
			{
				m_plateNo = sPlateNo;
				UpdateData(FALSE);
			}
		}		
	}	
}

void InDocDlg::OnFindPassports()
{
	TransactionPassportCollection collection(m_transaction->GetFacade());
	CPassportSearchDlg passSearchDlg(&collection,this);
	passSearchDlg.DoModal();
}

void InDocDlg::OnScanCow()
{
	ScanCow();
}

void InDocDlg::ScanCow()
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

void InDocDlg::OnClearAgent()
{
	m_AgentCb.SetCurSel(-1);
}