#include "stdafx.h"
#include "FastInDocDlg.h"
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
#include "../cow/ScanCowBCdsWizard.h"
#include "../hent/HentEditor.h"
#include "../hent/HentDlg.h"
#include <data\types\countries\Countries.h>
#include "../context/transaction/DefaultValuesApplier.h"
#include "../settings/DefaultValuesEditor.h"
#include "../AppServices.h"
#include <validation\CowNoValidator.h>
#include "../context/transaction/NoOpCowEditor.h"

#define HIDE_TOOLTIP_AFTER_MS 2000
#define TOOLTIP_FONTHEIGHT 45


BEGIN_MESSAGE_MAP(FastInDocDlg, CSessionAwareDialog )
	
			ON_WM_DESTROY()
			ON_BN_CLICKED(IDC_COWDELETE,OnDeleteCow)
			
			
			ON_BN_CLICKED(IDC_SETLOADTIMESTART,OnSetLoadTimeStart)
			ON_BN_CLICKED(IDC_SETLOADTIMEEND,OnSetLoadTimeEnd)
			ON_BN_CLICKED(IDC_SETPLATENO,OnSetPlateNo)	
			ON_BN_CLICKED(IDC_CLEARAGENT, OnClearAgent)
			ON_BN_CLICKED(IDC_ADDCOW, OnAddCow)
			
END_MESSAGE_MAP()

FastInDocDlg::FastInDocDlg(InDocTransactionPtr transaction,CWnd* pParent) : 
	CSessionAwareDialog(FastInDocDlg::IDD,pParent),
	m_extras(MAXEXTRAS),
	m_itemCount(transaction->GetFacade())
{
	m_transaction = transaction;
}

FastInDocDlg::~FastInDocDlg()
{
}


void FastInDocDlg::DoDataExchange(CDataExchange* pDX)
{
	CSessionAwareDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COWLIST, m_cowlist);	
	
	DDX_Control (pDX, IDC_DOCDATE, m_DocDate ) ;
	DDX_Control (pDX, IDC_TRANSPORTDATE, m_TransportDate ) ;	
	DDX_StdText	(pDX,IDC_PLATENO,m_plateNo);
	DDX_Control (pDX,IDC_EXTRAS,m_extras);
	DDX_Control (pDX,IDC_MOTIVESCOMBO,m_MotiveCombo);	
	DDX_Control(pDX,IDC_LOADTIMESTART,m_loadStartTm);
	DDX_Control(pDX,IDC_LOADTIMEEND,m_loadEndTm);
	DDX_Control(pDX,IDC_AGENTCOMBO,m_AgentCb);
	DDX_Control(pDX,IDC_COWCOUNT,m_itemCount);	
	DDX_Control(pDX, IDC_COWNO, m_cowno);
	DDX_Control(pDX, IDC_SEXBTNS, m_sexbtns);
	DDX_Control(pDX, IDC_WEIGHT, m_weight);
	DDX_Control(pDX, IDC_CLASSCOMBO, m_class);
	DDX_Control(pDX, IDC_SCANNERINDICATOR, m_indicator);
}

BOOL  FastInDocDlg::OnInitDialog()
{
	CSessionAwareDialog::OnInitDialog();

	BarcodeScannerService* bcService = AppServices::GetBCScannerService();
	bcService->AddListener(this);
	
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
		

	m_Msg.Create(this);
	m_Msg.SetShowCenteredInParent();

	
	ClassesComboContent classContent;
	classContent.CreateContent(session);
	m_class.SetContent(classContent);


	NoOpCowEditor noOpEditor;	
	DefaultValuesApplier applier(GetSeasonSession());
	applier.ApplyTo(&noOpEditor);

	m_class.Select(noOpEditor.GetClassId());
	

	Country defaultCountry = Countries::GetCountryByNumber(616);
	m_cowno.SetCountry(defaultCountry);
	


	UpdateBarcodeIndictor();

	m_cowno.EnableButton();

	UpdateData(FALSE);
	
	return TRUE;
}





void FastInDocDlg::OnCellDblClick(int row,uint32_t colId,const CRect& rect)
{
	const DocItem* item = m_transaction->GetItemAt(row);

	switch(colId)
	{
	case DocItemViewMember::DocItemView_Weight:
		{
			
			DecimalCellBuilder weightCell;		
			weightCell.precision(WEIGHT_PREC)
					  .receiver(std::bind(&FastInDocDlg::OnItemWeight,this,row,std::placeholders::_1))
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
					 .receiver(std::bind(&FastInDocDlg::OnItemClass,this,row,std::placeholders::_1))
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

void FastInDocDlg::OnItemClass(int row,uint32_t classId)
{
	DocItemRow* pRow = m_TransactionContent.GetItemRowAt(row);			
	m_transaction->GetFacade()->UpdateItemClass(pRow->Content()->GetItemId(),classId);
}

void FastInDocDlg::OnItemWeight(int row,const NullDecimal& weight)
{
	DocItemRow* pRow = m_TransactionContent.GetItemRowAt(row);			
	m_transaction->GetFacade()->UpdateItemWeight(pRow->Content()->GetItemId(),weight);
}

void FastInDocDlg::OnDestroy()
{

	BarcodeScannerService* bcService = AppServices::GetBCScannerService();
	bcService->RemoveListener(this);


	CSessionAwareDialog::OnDestroy();
	m_TransactionContent.Destroy();
}

void FastInDocDlg::OnOK()
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

void FastInDocDlg::SaveDocDetails(DocDetails& details)
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

void FastInDocDlg::OnDeleteCow()
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

void FastInDocDlg::EditCow(ItemId cowItemId)
{
	TransactionCowEditor editor(m_transaction->GetFacade());
	editor.Edit(cowItemId);
	CowDlg cowDlg(&editor,this);
	cowDlg.DoModal();
}

void FastInDocDlg::OnSetLoadTimeStart()
{
	m_loadStartTm.SetTime(Time::now());
}

void FastInDocDlg::OnSetLoadTimeEnd()
{
	m_loadEndTm.SetTime(Time::now());
}

void FastInDocDlg::OnSetPlateNo()
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


void FastInDocDlg::OnClearAgent()
{
	m_AgentCb.SetCurSel(-1);
}

void FastInDocDlg::OnAddCow()
{
	CowNo cowNo = m_cowno.GetCowNo();	

	if (cowNo.IsNull())
	{
		AfxMessageBox(IDS_ERREMPTYCOWNO, MB_OK | MB_ICONERROR, 0);
		m_cowno.SetFocus();
		return;
	}

	if (!CowNoValidator::IsValid(cowNo))
	{
		if (AfxMessageBox(IDS_YNBADCTRLDIGIT, MB_YESNO, 0) == IDNO)
		{
			m_cowno.SetFocus();
			return;
		}
	}

	AddCowWithNumber(cowNo);
}

void FastInDocDlg::AddCowWithNumber(const CowNo& cowNo) 
{
	uint32_t classId = m_class.GetSelectedItemId();
	NullDecimal weight = m_weight.GetDecimal();
	CowSex cowsex = m_sexbtns.GetSex();
	

	CowEditor* editor = NULL;
	ItemId existingItemId = m_transaction->GetItemWithCowNo(cowNo);


	if (existingItemId.is_nil()) {
		TransactionNewCowEditor* cowEditor = new TransactionNewCowEditor(m_transaction->GetFacade());
		cowEditor->SetCowNo(cowNo);
		editor = cowEditor;
	}
	else {
		TransactionCowEditor* cowEditor = new TransactionCowEditor(m_transaction->GetFacade());
		cowEditor->Edit(existingItemId);
		editor = cowEditor;
	}

	DefaultValuesApplier applier(GetSeasonSession());
	applier.ApplyTo(editor);

	
	if (classId != NULL_ID) {
		editor->SetClassId(classId);
	}

	if (!cowsex.IsUNK()) {
		editor->SetSex(cowsex);
	}

	if (!weight.IsNull()) {
		editor->SetWeight(weight.ToDecimal(Decimal(1L)));
	} else {
		ShowErrorTooltip(StringLoader(IDS_ERRREQWEIGHT));
		return;
	}

	if (editor->GetClassId() == NULL_ID) {
		ShowErrorTooltip(StringLoader(IDS_ERRSELCLASS));
		return;
	}

	editor->Save();
	delete editor;

}

void FastInDocDlg::OnBarcode(const std::wstring& sBarcode)
{
	
	try
	{
		CowNo cowNo = CowNoFactory::FromString(sBarcode);
		AddCowWithNumber(cowNo);		
	}
	catch (...)
	{
		m_Msg.SetText(StringLoader(IDS_BADCOWNOBARCODE));
		m_Msg.Show(HIDE_TOOLTIP_AFTER_MS);
		MessageBeep(MB_ICONWARNING);
	}
}

void FastInDocDlg::ShowErrorTooltip(const std::wstring& text)
{
	Color red(255, 0, 0);
	m_Msg.SetText(text);
	m_Msg.SetStyle(TipWndStyle().inner(red).outer(red).textsize(TOOLTIP_FONTHEIGHT));
	m_Msg.Show(HIDE_TOOLTIP_AFTER_MS);
	MessageBeep(MB_ICONERROR);
}

void FastInDocDlg::OnScannerConnected()
{
	UpdateBarcodeIndictor();
}

void FastInDocDlg::OnScannerDisconnected()
{

	UpdateBarcodeIndictor();
}
void FastInDocDlg::UpdateBarcodeIndictor()
{
	m_indicator.SetStatus(AppServices::GetBCScannerService()->GetConnectionStatus());
}