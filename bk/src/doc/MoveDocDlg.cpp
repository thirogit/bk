#include "stdafx.h"
#include "MoveDocDlg.h"
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
#include "../ui/cell/DecimalCellBuilder.h"
#include "../ui/cell/ComboCellBuilder.h"
#include "../uiconstants/PrecisionContstants.h"
#include <utils/SafeGet.h>
#include "TransactionSelectInterface.h"
#include "../header/CowInStockPresenceListHeader.h"
#include "../cow/CowSelectDlg.h"
#include "../content/AgentsComboContent.h"
#include "../ui/ExceptionMsgBox.h"
#include "../passportsegregation/PassportSearchDlg.h"
#include "TransactionPassportCollection.h"

BEGIN_MESSAGE_MAP(MoveDocDlg, CSessionAwareDialog )
	
			ON_BN_CLICKED(IDC_ADDCOW, OnAddCow)			
			ON_WM_DESTROY()
			ON_BN_CLICKED(IDC_SETLOADTIMESTART,OnSetLoadTimeStart)
			ON_BN_CLICKED(IDC_SETLOADTIMEEND,OnSetLoadTimeEnd)
			ON_BN_CLICKED(IDC_SETPLATENO,OnSetPlateNo)
			ON_BN_CLICKED(IDC_COWDELETE,OnDeleteCow)
			ON_BN_CLICKED(IDC_FINDPASSPORTS,OnFindPassports)
			ON_BN_CLICKED(IDC_CLEARAGENT, OnClearAgent)
	
END_MESSAGE_MAP()

MoveDocDlg::MoveDocDlg(MoveDocTransactionPtr transaction,CWnd* pParent) : CSessionAwareDialog(MoveDocDlg::IDD,pParent),
	m_extras(MAXEXTRAS),
	m_itemCount(transaction->GetFacade())
{
	m_transaction = transaction;
}

MoveDocDlg::~MoveDocDlg()
{
}


void MoveDocDlg::DoDataExchange(CDataExchange* pDX)
{
	CSessionAwareDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COWLIST, m_cowlist);	
	
	DDX_Control (pDX, IDC_DOCDATE, m_DocDate ) ;
	DDX_Control (pDX, IDC_TRANSPORTDATE, m_TransportDate ) ;	
	DDX_StdText	(pDX,IDC_PLATENO,m_plateNo);
	DDX_Control (pDX,IDC_EXTRAS,m_extras);
	DDX_Control(pDX,IDC_LOADTIMESTART,m_loadStartTm);
	DDX_Control(pDX,IDC_LOADTIMEEND,m_loadEndTm);
	DDX_Control(pDX,IDC_AGENTCOMBO,m_AgentCb);
	DDX_Control(pDX,IDC_COWCOUNT,m_itemCount);
	
}

BOOL  MoveDocDlg::OnInitDialog()
{
	CSessionAwareDialog::OnInitDialog();

	SendDlgItemMessage(IDC_PLATENO,EM_LIMITTEXT,MAXPLATE);
	
	CWnd* pCancelWnd = GetDlgItem(IDCANCEL);
	m_loadStartTm.SetAbortWnd(pCancelWnd);
	m_loadEndTm.SetAbortWnd(pCancelWnd);

	DateTime nowDt = DateTime::now();
	m_DocDate.SetDate(nowDt);
	m_TransportDate.SetDate(nowDt);

	SeasonSession* session = GetSeasonSession();
	AgentsComboContent agents;
	agents.CreateContent(session);		

	m_AgentCb.SetContent(agents);
	
	if(m_transaction->IsUpdating())
	{

		SetWindowText((boost::wformat(StringLoader(IDS_EDITMOVEDOCDLGCAPTION)) % DocNoText(m_transaction->GetDocKey()).ToString()).str().c_str());

		const DocDetails& docDetails = m_transaction->GetDetails();		
		m_loadEndTm.SetTime(docDetails.GetLoadEndDtTm());
		m_loadStartTm.SetTime(docDetails.GetLoadStartDtTm());
		m_AgentCb.Select(docDetails.GetAgentId());
		m_extras.SetText(docDetails.GetExtras());
		m_plateNo = docDetails.GetPlateNo();
		m_TransportDate.SetDate(docDetails.GetLoadDate());
		m_DocDate.SetDate(docDetails.GetDocDate());
		
	}
	else
	{
		uint32_t srcHerdId = m_transaction->GetSrcHerdId();
		uint32_t dstHerdId = m_transaction->GetDstHerdId();
		IHerd* pSrcHerd = session->GetHerd(srcHerdId);
		IHerd* pDstHerd = session->GetHerd(dstHerdId);
		std::wstring sSrcHerdName = SafeGet(&IHerd::GetHerdName,pSrcHerd,L"");
		std::wstring sDstHerdName = SafeGet(&IHerd::GetHerdName,pDstHerd,L"");
		std::wstring sCaption = (boost::wformat(StringLoader(IDS_NEWMOVEDOCDLGCAPTION).c_str()) % sSrcHerdName % sDstHerdName).str();
																
		SetWindowText(sCaption.c_str());		
	}

	m_TransactionContent.Create(session,m_transaction->GetFacade());
	m_cowlist.SetContent(&m_TransactionContent);	

	std::function<void (int,uint32_t,CellDescriptor& cell)> dblHandler = 
		[this](int row,uint32_t colId,CellDescriptor& cell)
		{
			OnCellDblClick(row,colId,cell.GetRect());
		};

	m_cowlist.SetCellDblClickHandler(dblHandler);
	UpdateData(FALSE);
	return TRUE;
}

void MoveDocDlg::OnItemWeight(int row,const NullDecimal& weight)
{
	DocItemRow* pRow = m_TransactionContent.GetItemRowAt(row);
	m_transaction->GetFacade()->UpdateItemWeight(pRow->Content()->GetItemId(),weight);
}

void MoveDocDlg::OnCellDblClick(int row,uint32_t colId,const CRect& rect)
{
	switch(colId)
	{
	case DocItemViewMember::DocItemView_Weight:
		{
			const DocItem* item = m_transaction->GetItemAt(row);
			DecimalCellBuilder weightCell;		
			weightCell.precision(WEIGHT_PREC)
				.receiver(std::bind(&MoveDocDlg::OnItemWeight,this,row,std::placeholders::_1))
				.value(item->GetCowEntry().GetWeight())
				.show(&m_cowlist,rect);
			break;
		}
	case DocItemViewMember::DocItemView_Class:
		{

		}
		break;

	case DocItemViewMember::DocItemView_Stock:
		break;

	}
}

void MoveDocDlg::OnDestroy()
{
	CSessionAwareDialog::OnDestroy();
	m_TransactionContent.Destroy();
}

void MoveDocDlg::OnOK()
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

void MoveDocDlg::SaveDocDetails(DocDetails& details)
{
	details.SetDocDate(m_DocDate.GetDate());
	details.SetLoadDate(m_TransportDate.GetDate());
	details.SetExtras(m_extras.GetText());
	details.SetPlateNo(m_plateNo);
	details.SetLoadStartDtTm(m_loadStartTm.GetTime());
	details.SetLoadEndDtTm(m_loadEndTm.GetTime());	
	details.SetAgentId(m_AgentCb.GetSelectedItemId());
}

void MoveDocDlg::OnAddCow()
{
	TransactionSelectInterface selectInterface;
	SeasonSession* session = GetSeasonSession();	
	selectInterface.Create(session,m_transaction->GetSrcHerdId(),m_transaction->GetFacade());

	CowSelectDlg cowSelectDlg(&selectInterface,this);
	cowSelectDlg.SetHeader(CowInStockPresenceListHeader());
	cowSelectDlg.DoModal();

	selectInterface.Destroy();
}

void MoveDocDlg::OnSetLoadTimeStart()
{
	m_loadStartTm.SetTime(Time::now());
}

void MoveDocDlg::OnSetLoadTimeEnd()
{
	m_loadEndTm.SetTime(Time::now());
}

void MoveDocDlg::OnSetPlateNo()
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

void MoveDocDlg::OnDeleteCow()
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

void MoveDocDlg::OnFindPassports()
{
	TransactionPassportCollection collection(m_transaction->GetFacade());
	CPassportSearchDlg passSearchDlg(&collection,this);
	passSearchDlg.DoModal();
}

void MoveDocDlg::OnClearAgent()
{
	m_AgentCb.SetCurSel(-1);
}