#include "stdafx.h"
#include "InvoicesTab.h"
#include "../../view/InvoiceView.h"
#include "../../utils/StringLoader.h"
#include "../../ui/menu/MenuBuilder.h"
#include "../../resource.h"
#include "../../invoice/BuyInvoiceDocScopeDlg.h"
#include "../../invoice/BuyInvoiceCowScopeDlg.h"
#include "../../invoice/SellInvoiceDocScopeDlg.h"
#include "../../invoice/SellInvoiceCowScopeDlg.h"
#include "../../ui/waitdlg\WaitDialog.h"
#include "../../ui/ExceptionMsgBox.h"
#include <boost/foreach.hpp>
#include "../../runtime/UniqueIds.h"
#include "../../locking/LockerDlg.h"
#include "../../invoice/lock/BuyInvoiceLocker.h"
#include "../../invoice/lock/SellInvoiceLocker.h"
#include "../../AppServices.h"
#include "../../print/ui/DocumentPrintDlg.h"
#include "../../print/DocumentTerritoryObj.h"
#include <string\TextUtils.h>
#include "../../invoice/markpaid/BuyInvoiceMarkPaidDlg.h"
#include "../../invoice/markpaid/SellInvoiceMarkPaidDlg.h"
#include "../../header/InvoiceListHeader.h"
#include "../../invoice/moneytransfer/ui/MoneyTransferDlg.h"
#include "../../print/context/DocumentContextBuilder.h"
#include "../../print/runtime/FeedException.h"
#include "../../print/runtime/DocumentInputUtils.h"
#include "../../invoice/invoiceexport/ui/InvoicesExportDlg.h"
#include "../../excelexport/ExcelExportSettingsDlg.h"
#include "../../excelexport/ExcelExport.h"
#include "../../context/XContext.h"

#define ID_BUYREGULARINVOICES_TAB_ID 1001
#define ID_BUYLUMPINVOICES_TAB_ID 1002
#define ID_SELLREGULARINVOICES_TAB_ID 1003
#define ID_TOOLBAR_ID 1004
#define ID_TAB_ID 1005

#define TOOLBAR_HEIGHT_PX 0

namespace predicate_ops
{
	bool _and(IInvoice* invoice, std::function<bool(IInvoice*)>& oper1, std::function<bool(IInvoice*)>& oper2)
	{
		return oper1(invoice) && oper2(invoice);
	}
}


invoice_predicate and (const invoice_predicate& left, const invoice_predicate& right)
{
	invoice_predicate p;
	p.m_fn = std::bind(&predicate_ops::_and, std::placeholders::_1, left.m_fn, right.m_fn);
	return p;
}

bool invoice_predicate::operator()(IInvoice* invoice)
{
	return m_fn(invoice);
}

InvoicesTab::InvoiceTabInvoiceSearchInterfaceImpl::InvoiceTabInvoiceSearchInterfaceImpl(SeasonSession* session, const InvoiceKind& kind) : m_kind(kind), m_session(session)
{

}

IInvoice* InvoicesTab::InvoiceTabInvoiceSearchInterfaceImpl::GetBuyInvoice(uint32_t id)
{
	BuyInvoiceClientObject* buyInvoiceCO = m_session->GetBuyInvoice(id);
	if (buyInvoiceCO != NULL)
	{
		return buyInvoiceCO->Object();
	}
	return NULL;
}

IInvoice* InvoicesTab::InvoiceTabInvoiceSearchInterfaceImpl::GetSellInvoice(uint32_t id)
{
	SellInvoiceClientObject* sellInvoiceCO = m_session->GetSellInvoice(id);
	if (sellInvoiceCO != NULL)
	{
		return sellInvoiceCO->Object();
	}
	return NULL;
}

UniqueIds InvoicesTab::InvoiceTabInvoiceSearchInterfaceImpl::Search(const DateRange& range, PayWay payway)
{
	UniqueIds result;

	invoice_predicate predicate = wrap([&range](IInvoice* invoice)
	{
		return range.IsDateWithin(invoice->GetInvoiceDate());
	});


	if (payway != PayWay_None)
	{
		predicate = and (predicate, wrap([payway](IInvoice* invoice) { return invoice->GetPayWay() == payway; }));
	}

	if (m_kind.GetInvoiceType() != InvoiceType_None)
	{
		predicate = and (predicate, wrap([this](IInvoice* invoice) {return invoice->GetInvoiceType() == m_kind.GetInvoiceType(); }));
	}

	switch (m_kind.GetDirection())
	{
		case InvoiceDirection_Buy:
		{
			PtrEnumerator<BuyInvoiceClientObject> buyInvoices = m_session->EnumBuyInvoices();
	
			while (buyInvoices.hasNext())
			{
				IInvoice* invoice = (*buyInvoices)->Object();
				if (predicate(invoice))
				{
					result.add(invoice->GetId());
				}
				buyInvoices.advance();
			}
			break;

		}
		case InvoiceDirection_Sell:
		{
			PtrEnumerator<SellInvoiceClientObject> sellInvoices = m_session->EnumSellInvoices();

			while (sellInvoices.hasNext())
			{
				IInvoice* invoice = (*sellInvoices)->Object();
				if (predicate(invoice))
				{
					result.add(invoice->GetId());
				}
				sellInvoices.advance();
			}
		}
	}	
	
	return std::move(result);
}

InvoiceKind InvoicesTab::InvoiceTabInvoiceSearchInterfaceImpl::GetInvoiceKind() const
{
	return m_kind;
}

IInvoice* InvoicesTab::InvoiceTabInvoiceSearchInterfaceImpl::GetInvoice(uint32_t id)
{
	switch (m_kind.GetDirection())
	{
		case InvoiceDirection_Buy:
		{
			BuyInvoiceClientObject* buyInvoiceCO = m_session->GetBuyInvoice(id);
			if (buyInvoiceCO != NULL)
			{
				return buyInvoiceCO->Object();
			}
			break;

		}
		case InvoiceDirection_Sell:
		{
			SellInvoiceClientObject* sellInvoiceCO = m_session->GetSellInvoice(id);
			if (sellInvoiceCO != NULL)
			{
				return sellInvoiceCO->Object();
			}
			break;
		}
	}
	return NULL;
}


//#####################################################################################################################

InvoicesTab::InvoicesTab()
{

}

InvoicesTab::~InvoicesTab()
{
}

BEGIN_MESSAGE_MAP(InvoicesTab, TabPage)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


void InvoicesTab::OnSize( UINT nType,  int cx,  int cy  )
{
	TabPage::OnSize(nType,cx,cy);
	m_Navigator.SetWindowPos(NULL,0,TOOLBAR_HEIGHT_PX,cx,cy-TOOLBAR_HEIGHT_PX,SWP_NOZORDER);
	m_toolbar.SetWindowPos(NULL,0,0,cx, TOOLBAR_HEIGHT_PX,SWP_NOZORDER);
//	m_List.SetWindowPos(NULL,TOOLBAR_WIDTH_PX,0,cx-TOOLBAR_WIDTH_PX,cy,SWP_NOZORDER);
}

int InvoicesTab::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(TabPage::OnCreate(lpCreateStruct) != 0)
		return -1;

	if (!m_Navigator.Create(CMFCTabCtrl::STYLE_3D_ONENOTE, CRect(), this, ID_TAB_ID, CMFCTabCtrl::Location::LOCATION_TOP))
		return -1;


	m_toolbar.Create(this,CRect(), ID_TOOLBAR_ID);

	m_BuyLumpInvoicesTab.Create(WS_CHILD  ,CRect(),&m_Navigator,ID_BUYLUMPINVOICES_TAB_ID);
	m_BuyRegularInvoicesTab.Create(WS_CHILD, CRect(), &m_Navigator, ID_BUYREGULARINVOICES_TAB_ID);
	m_SellRegularInvoicesTab.Create(WS_CHILD, CRect(), &m_Navigator, ID_SELLREGULARINVOICES_TAB_ID);
	

	SeasonSession* session = GetSeasonSession();

	InvoiceListHeader invoiceHeader;

	m_BuyLumpInvoicesTab.SetHeaderDescriptor(invoiceHeader);
	m_BuyLumpInvoices.Create(session,InvoiceKind(InvoiceDirection_Buy,InvoiceType_Lump));
	m_BuyLumpInvoicesTab.SetContent(&m_BuyLumpInvoices);

	m_BuyRegularInvoicesTab.SetHeaderDescriptor(invoiceHeader);
	m_BuyRegularInvoices.Create(session, InvoiceKind(InvoiceDirection_Buy, InvoiceType_Regular));
	m_BuyRegularInvoicesTab.SetContent(&m_BuyRegularInvoices);

	m_SellRegularInvoicesTab.SetHeaderDescriptor(invoiceHeader);
	m_SellRegularInvoices.Create(session, InvoiceKind(InvoiceDirection_Sell, InvoiceType_Regular));
	m_SellRegularInvoicesTab.SetContent(&m_SellRegularInvoices);

	m_Navigator.AddTab(&m_BuyLumpInvoicesTab,StringLoader(IDS_BUYLUMPINVOICES_TABNAME).c_str());
	m_Navigator.AddTab(&m_BuyRegularInvoicesTab, StringLoader(IDS_BUYREGULARINVOICES_TABNAME).c_str());
	m_Navigator.AddTab(&m_SellRegularInvoicesTab,StringLoader(IDS_SELLREGULARINVOICES_TABNAME).c_str());
	
	
	m_BuyRegularInvoicesTab.SetOnRightClick(std::bind(&InvoicesTab::OnBuyInvoicesRClick,this,std::placeholders::_1,std::placeholders::_2,&m_BuyRegularInvoicesTab));
	m_BuyLumpInvoicesTab.SetOnRightClick(std::bind(&InvoicesTab::OnBuyInvoicesRClick, this, std::placeholders::_1, std::placeholders::_2, &m_BuyLumpInvoicesTab));
	m_SellRegularInvoicesTab.SetOnRightClick(std::bind(&InvoicesTab::OnSellInvoicesRClick, this, std::placeholders::_1, std::placeholders::_2, &m_SellRegularInvoicesTab));
		
	//m_toolbar.AddButton()


	return 0;
}

void InvoicesTab::OnBuyInvoicesRClick(int row,int col,ListTab* list)
{
	if(row >= 0)
	{
		MenuBuilder menu;
		
		menu.add(StringLoader(IDS_EDITINVOICE),std::bind(&InvoicesTab::OnEditBuyInvoice,this,list));
		menu.add(StringLoader(IDS_DELETE),std::bind(&InvoicesTab::OnDeleteBuyInvoice,this,list));
		menu.addSeparator();
		menu.add(StringLoader(IDS_MARKPAID),std::bind(&InvoicesTab::OnMarkPaidBuyInvoice,this,list));
		menu.add(StringLoader(IDS_MONEYTRANSFER), std::bind(&InvoicesTab::OnMoneyTransfer, this, list));
		menu.add(StringLoader(IDS_EXPORTINVOICES), std::bind(&InvoicesTab::OnExportInvoices, this, list));
		menu.add(StringLoader(IDS_EXPORT), std::bind(&InvoicesTab::OnExportToExcel, this, list));
		menu.addSeparator();

		MenuBuilder printMenu;
		DocumentManager* manager = AppServices::GetDocumentManager();

		InvoicesListContent* content = (InvoicesListContent*)list->GetContent();
		DocumentSelection selection(manager->FindDocuments(DocumentInputUtils::GetDocumentInputMaskForBuyInvoice(content->GetInvoiceType())));

		if(!selection.IsEmpty())
		{
			PtrEnumerator<DocumentArchetype> archetypeEnum = selection.Enum();

			DocumentArchetype* archetype;

			while(archetypeEnum.hasNext())
			{
				archetype = *archetypeEnum;
				printMenu.add(archetype->GetName(),std::bind(&InvoicesTab::OnPrintBuyInvoice,this,*archetype,list));
				archetypeEnum.advance();
			}

			menu.addSubmenu(StringLoader(IDS_PRINT),printMenu);
			
		}


		menu.track(this);
	}
}

void InvoicesTab::OnPrintBuyInvoice(const DocumentArchetype& archetype,ListTab* list)
{
	ListSelection selection = list->GetSelection();
	SeasonSession* session = GetSeasonSession();

	if(selection.GetCount() > 0)
	{
		UniqueIds ids;
		InvoiceView* view;
		DocumentPredicate* predicate = archetype.GetPredicate();
		InvoicesListContent* content = (InvoicesListContent*)list->GetContent();

		BOOST_FOREACH(int row,selection.GetRange())
		{		
			view = content->GetInvoiceRowAt(row)->Content();

			if (predicate && !predicate->applies(view))
			{
				AfxMessageBox(TextUtils::Format(StringLoader(IDS_INVOICEISNOTVALIDFORDOCUMENT),view->GetInvoiceNo(),archetype.GetName()).c_str());
				return;
			}
			ids.add(view->GetInvoiceId());
		}
		
		DocumentContext* context = DocumentContextBuilder().Create(NULL);

		try
		{
			DocumentInstance* instance = archetype.CreateDocument(session, context, ids);
			DocumentPrintDlg printDlg(instance, this);
			printDlg.DoModal();
			delete instance;
			delete context;
		}
		catch (FeedException& e)
		{
			ExceptionMsgBox().Show(e);
		}
	
	}
}


void InvoicesTab::OnExportInvoices(ListTab* list)
{	
	InvoiceTabInvoiceSearchInterfaceImpl searchInterface(GetSeasonSession(),((InvoicesListContent*)(list->GetContent()))->GetInvoiceKind());
	CInvoicesExportDlg exportDlg(&searchInterface, this);
	exportDlg.DoModal();	
}


void InvoicesTab::OnExportToExcel(ListTab* list)
{
	const wchar_t* EXPORT_SETTING_NAME = L"INVOICE_COWS_EXCELEXPORT";
	SeasonSession* session = GetSeasonSession();
	UserSettings* userSettings = session->GetContext()->GetUserSettings();
	ExcelExportSettings setting = ExcelExportSettings::Load(userSettings, EXPORT_SETTING_NAME);
	InvoiceListHeader header;
	ExcelExportSettingsDlg exportSettingsDlg(&setting, &header, this);
	if (exportSettingsDlg.Configure())
	{
		setting.Save(userSettings, EXPORT_SETTING_NAME);
		ExcelExport exporter(session, &setting);
		std::wstring extension(exporter.GetFileExtension());
		std::wstring filter = TextUtils::Format(L"%s  (*.%s)|*.%s||", extension, extension, extension);
		CFileDialog saveFileDlg(FALSE, extension.c_str(), NULL, OFN_OVERWRITEPROMPT, filter.c_str(), this);

		if (saveFileDlg.DoModal() == IDOK)
		{
			std::wstring outputFilePathName = (LPCTSTR)saveFileDlg.GetPathName();
					   
			std::vector<IInvoice*> invoices;
			InvoicesListContent* content = (InvoicesListContent*)list->GetContent();
			ListSelection selection = list->GetSelection();
			InvoiceTabInvoiceSearchInterfaceImpl searchInterface(session, content->GetInvoiceKind());

			if (selection.GetCount() > 0)
			{

				BOOST_FOREACH(int row, selection.GetRange())
				{					
					InvoiceRow* pRow = content->GetInvoiceRowAt(row);
					InvoiceView* pView = pRow->Content();					
					IInvoice* invoice = searchInterface.GetInvoice(pView->GetInvoiceId());
					invoices.push_back(invoice);
				}
				exporter.Export(outputFilePathName, invoices);
			}			
		}
	}
}

void InvoicesTab::OnMoneyTransfer(ListTab* list)
{	
	UniqueIds invoiceIds = GetSelectedInvoices(list);

	BuyInvoiceLocker locker(GetSeasonSession());
	locker.CreateAttempts(invoiceIds);

	CLockerDlg lockDlg(&locker);
	if (lockDlg.DoModal() == IDOK)
	{
		CMoneyTransferDlg moneyTransferDlg(invoiceIds, this);
		if(moneyTransferDlg.DoModal() == IDCONTINUE)
		{
			CBuyInvoiceMarkPaidDlg markPaidDlg(moneyTransferDlg.GetInvoiceIds(), this);
			markPaidDlg.DoModal();
		}
	}

	locker.UnlockAll();
}


void InvoicesTab::OnMarkPaidBuyInvoice(ListTab* list)
{
	UniqueIds invoiceIds = GetSelectedInvoices(list);

	BuyInvoiceLocker locker(GetSeasonSession());
	locker.CreateAttempts(invoiceIds);

	CLockerDlg lockDlg(&locker);
	if(lockDlg.DoModal() == IDOK)
	{
		CBuyInvoiceMarkPaidDlg markPaidDlg(invoiceIds,this);
		markPaidDlg.DoModal();
	}

	locker.UnlockAll();
}

uint32_t InvoicesTab::GetSelectedInvoice(ListTab* list)
{
	int nRow = list->GetSelectedRow();
	if(nRow >= 0)
	{
		InvoicesListContent* content = (InvoicesListContent*)list->GetContent();
		InvoiceRow* pRow = content->GetInvoiceRowAt(nRow);
		InvoiceView* pView = pRow->Content();
		return pView->GetInvoiceId();
	}
	return NULL_ID;
}



void InvoicesTab::EditBuyInvoice(uint32_t invoiceId)
{
	SeasonSession* session = GetSeasonSession();

	ExceptionMsgBox box;
	box.TryCatchAny([session,invoiceId]()
	{
		session->LockBuyInvoice(invoiceId);
	});

							

	BuyInvoiceClientObject* pBuyInvoiceCO = session->GetBuyInvoice(invoiceId);
	BuyInvoice* pBuyInvoice = pBuyInvoiceCO->Object();
	
	bool paid = !pBuyInvoice->GetPaidDate().IsNull();
											
	if(paid)
	{
		AfxMessageBox(IDS_INVOICEISPAID);
		//ViewInvoice();
	}
	else
	{
		InvoiceScope scope = pBuyInvoice->GetInvoiceScope();																

		switch(scope)
		{
		case InvoiceScope_Docs:
			{
				InvoiceDocsTransactionPtr transaction = session->EditBuyInvoiceDocs(invoiceId);
				CBuyInvoiceDocScopeDlg invoiceDlg(transaction,this);
				invoiceDlg.DoModal();
				break;
			}
		case InvoiceScope_Cows:
			{
				InvoiceCowsTransactionPtr transaction = session->EditBuyInvoiceCows(invoiceId);
				CBuyInvoiceCowScopeDlg invoiceDlg(transaction,this);
				invoiceDlg.DoModal();
				break;
			}
		}		
	}

	box.TryCatchAny([session, invoiceId]()
	{
		session->UnlockBuyInvoice(invoiceId);
	});

			
}

void InvoicesTab::OnEditBuyInvoice(ListTab* list)
{
	uint32_t invoiceId = GetSelectedInvoice(list);
	EditBuyInvoice(invoiceId);	
}

void InvoicesTab::DeleteBuyInvoice(uint32_t invoiceId)
{
	SeasonSession* session = GetSeasonSession();
	ExceptionMsgBox msgBox;

	Try()
		.Prepare([session,invoiceId]
			{
				session->LockBuyInvoice(invoiceId);
			})
		.Do([session,invoiceId]
			{
				session->DeleteBuyInvoice(invoiceId);					
			})
		.Fallback([session,invoiceId]
			{
				session->UnlockBuyInvoice(invoiceId);					
			})
		.Catch([&msgBox](wexception& e)
			{
				msgBox.Show(e);
			})
		.Run();
	
}

void InvoicesTab::OnDeleteBuyInvoice(ListTab* list)
{
	if (ConfirmInvoiceDelete())
	{

		uint32_t invoiceId = GetSelectedInvoice(list);

		CWaitDialog deleteWait(this, StringLoader(IDS_DELETING));

		deleteWait.WaitForTask([invoiceId, this]
		{
			DeleteBuyInvoice(invoiceId);
		});
	}
}



bool InvoicesTab::ConfirmInvoiceDelete()
{
	if (AfxMessageBox(IDS_ASKTOCONFIRMINVOICEDELETE, MB_YESNO, 0) != IDYES)
	{
		return false;
	}
	return true;
}


void InvoicesTab::OnPrintSellInvoice(const DocumentArchetype& archetype, ListTab* list)
{
	ListSelection selection = list->GetSelection();
	SeasonSession* session = GetSeasonSession();

	if (selection.GetCount() > 0)
	{
		UniqueIds ids;
		InvoiceView* view;
		DocumentPredicate* predicate = archetype.GetPredicate();
		InvoicesListContent* content = (InvoicesListContent*)list->GetContent();

		BOOST_FOREACH(int row, selection.GetRange())
		{
			view = content->GetInvoiceRowAt(row)->Content();

			if (predicate && !predicate->applies(view))
			{
				AfxMessageBox(TextUtils::Format(StringLoader(IDS_INVOICEISNOTVALIDFORDOCUMENT), view->GetInvoiceNo(), archetype.GetName()).c_str());
				return;
			}
			ids.add(view->GetInvoiceId());
		}

		DocumentContext* context = DocumentContextBuilder().Create(NULL);

		try
		{
			DocumentInstance* instance = archetype.CreateDocument(session, context, ids);
			DocumentPrintDlg printDlg(instance, this);
			printDlg.DoModal();
			delete instance;
			delete context;
		}
		catch (FeedException& e)
		{
			ExceptionMsgBox().Show(e);
		}

	}
}


void InvoicesTab::OnSellInvoicesRClick(int row,int col, ListTab* list)
{
	if(row >= 0)
	{
		MenuBuilder menu;
		
		menu.add(StringLoader(IDS_EDITINVOICE),std::bind(&InvoicesTab::OnEditSellInvoice,this,list));
		menu.add(StringLoader(IDS_DELETE),std::bind(&InvoicesTab::OnDeleteSellInvoice,this,list));
		menu.addSeparator();		
		menu.add(StringLoader(IDS_MARKPAID), std::bind(&InvoicesTab::OnMarkPaidSellInvoice, this,list));
		menu.add(StringLoader(IDS_EXPORTINVOICES), std::bind(&InvoicesTab::OnExportInvoices, this, list));
		menu.add(StringLoader(IDS_EXPORT), std::bind(&InvoicesTab::OnExportToExcel, this, list));
		menu.addSeparator();

		MenuBuilder printMenu;
		DocumentManager* manager = AppServices::GetDocumentManager();

		InvoicesListContent* content = (InvoicesListContent*)list->GetContent();
		DocumentSelection selection(manager->FindDocuments(DocumentInputUtils::GetDocumentInputMaskForSellInvoice(content->GetInvoiceType())));

		if(!selection.IsEmpty())
		{
			PtrEnumerator<DocumentArchetype> archetypeEnum = selection.Enum();

			DocumentArchetype* archetype;
			while(archetypeEnum.hasNext())
			{
				archetype = *archetypeEnum;
				printMenu.add(archetype->GetName(),std::bind(&InvoicesTab::OnPrintBuyInvoice,this,*archetype,list));
				archetypeEnum.advance();
			}

			menu.addSubmenu(StringLoader(IDS_PRINT),printMenu);
			//menu.addSeparator();
		}



		menu.track(this);
	}
}

void InvoicesTab::OnEditSellInvoice(ListTab* list)
{
	uint32_t invoiceId = GetSelectedInvoice(list);
	EditSellInvoice(invoiceId);	
}

void InvoicesTab::EditSellInvoice(uint32_t invoiceId)
{
	SeasonSession* session = GetSeasonSession();
	session->LockSellInvoice(invoiceId);							

	SellInvoiceClientObject* pSellInvoiceCO = session->GetSellInvoice(invoiceId);
	SellInvoice* pSellInvoice = pSellInvoiceCO->Object();
	
	bool paid = !pSellInvoice->GetPaidDate().IsNull();
											
	if(paid)
	{
		//ViewInvoice();
	}
	else
	{
		InvoiceScope scope = pSellInvoice->GetInvoiceScope();																

		switch(scope)
		{
		case InvoiceScope_Docs:
			{
				InvoiceDocsTransactionPtr transaction = session->EditSellInvoiceDocs(invoiceId);
				CSellInvoiceDocScopeDlg invoiceDlg(transaction,this);
				invoiceDlg.DoModal();
				break;
			}
		case InvoiceScope_Cows:
			{
				InvoiceCowsTransactionPtr transaction = session->EditSellInvoiceCows(invoiceId);
				CSellInvoiceCowScopeDlg invoiceDlg(transaction,this);
				invoiceDlg.DoModal();
				break;
			}
		}		
	}

	session->UnlockSellInvoice(invoiceId);			
}

void InvoicesTab::OnDeleteSellInvoice(ListTab* list)
{
	if (ConfirmInvoiceDelete())
	{
		uint32_t invoiceId = GetSelectedInvoice(list);

		CWaitDialog deleteWait(this, StringLoader(IDS_DELETING));

		deleteWait.WaitForTask([invoiceId, this]
		{
			DeleteSellInvoice(invoiceId);
		});
	}
}

void InvoicesTab::DeleteSellInvoice(uint32_t invoiceId)
{
	SeasonSession* session = GetSeasonSession();
	ExceptionMsgBox msgBox;

	Try()
		.Prepare([session,invoiceId]
			{
				session->LockSellInvoice(invoiceId);
			})
		.Do([session,invoiceId]
			{
				session->DeleteSellInvoice(invoiceId);					
			})
		.Fallback([session,invoiceId]
			{
				session->UnlockSellInvoice(invoiceId);					
			})
		.Catch([&msgBox](wexception& e)
			{
				msgBox.Show(e);
			})
		.Run();
}

void InvoicesTab::OnDestroy()
{
	TabPage::OnDestroy();
	m_BuyLumpInvoices.Destroy();
	m_BuyRegularInvoices.Destroy();
	m_SellRegularInvoices.Destroy();
}

UniqueIds InvoicesTab::GetSelectedInvoices(ListTab* list)
{
	ListSelection selection = list->GetSelection();

	UniqueIds invoiceIds;

	InvoicesListContent* content = (InvoicesListContent*)list->GetContent();
	BOOST_FOREACH(int nrow, selection.GetRange())
	{
		InvoiceRow* row = content->GetInvoiceRowAt(nrow);
		invoiceIds.add(row->Content()->GetInvoiceId());
	}

	return std::move(invoiceIds);
}

void InvoicesTab::OnMarkPaidSellInvoice(ListTab* list)
{
	UniqueIds invoiceIds = GetSelectedInvoices(list);

	SellInvoiceLocker locker(GetSeasonSession());
	locker.CreateAttempts(invoiceIds);

	CLockerDlg lockDlg(&locker);
	if (lockDlg.DoModal() == IDOK)
	{
		CSellInvoiceMarkPaidDlg markPaidDlg(invoiceIds, this);
		markPaidDlg.DoModal();
	}

	locker.UnlockAll();	
}