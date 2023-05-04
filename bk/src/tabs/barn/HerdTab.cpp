#include "stdafx.h"
#include "HerdTab.h"
#include "../../resource.h"
#include "../../utils/StringLoader.h"
#include "../../SeasonAccess.h"
#include "../../cow/CowDlg.h"
#include "../../cow/LonlyCowEditor.h"
#include "../../image/Png.h"
#include <boost\function.hpp>

#include "../../doc/InDocDlg.h"
#include "../../doc/FastInDocDlg.h"
#include "../../doc/BuyDocDlg.h"
#include "../../doc/MoveDocDlg.h"
#include "../../doc/SellDocDlg.h"
#include "../../doc/OutDocDlg.h"

#include "../../ui/waitdlg\WaitDialog.h"
#include "../../ui/menu/MenuBuilder.h"
#include "../../ContextAccess.h"
#include "../../invoice/BuyInvoiceDocScopeDlg.h"
#include "../../invoice/BuyInvoiceCowScopeDlg.h"
#include "../../invoice/SellInvoiceCowScopeDlg.h"
#include "../../invoice/SellInvoiceDocScopeDlg.h"
#include <boost/foreach.hpp>
#include "../../ui/ExceptionMsgBox.h"
#include "../../ui/YesNoBox.h"
#include <boost/format.hpp>
#include "../../view/CowPresenceView.h"
#include "../../cow/CowSelectDlg.h"
#include "../../header/CowListHeader.h"
#include "../../invoice/BuyInvoiceCowsSelectInterface.h"
#include "../../invoice/SellInvoiceCowsSelectInterface.h"
#include "../../print/DocumentManager.h"
#include "../../print/ui/DocumentPrintDlg.h"
#include "../../view/FatOwnDocViewCreator.h"
#include "../../print/DocumentTerritoryObj.h"
#include "../../print/context/DocumentContextBuilder.h"
#include "../../print/runtime/FeedException.h"
#include "../../AppServices.h"
#include <string/TextUtils.h>
#include "../../context/transaction/DefaultValuesApplier.h"
#include "../../excelexport/ExcelExportSettingsDlg.h"
#include "../../header/CowPresenceListHeader.h"
#include "../../header/MoveDocsListHeader.h"
#include "../../header/OwnDocsListHeader.h"
#include "../../header/HentsDocsListHeader.h"



#define ID_TAB_ID 1001
#define ID_STATUS_ID 1002

#define HERDNUMBERS_HEIGHT 20

#define ID_INCOWS_TAB_ID	2001
#define ID_ALLCOWS_TAB_ID	2002
#define ID_INDOCS_TAB_ID	2003
#define ID_BUYDOCS_TAB_ID	2004
#define ID_MOVEDOCS_TAB_ID	2005
#define ID_OUTDOCS_TAB_ID	2006
#define ID_SELLDOCS_TAB_ID	2007

namespace ph = std::placeholders;

HerdTab::HerdTab(IHerd* pHerd) : m_pHerd(pHerd), m_herdNumbers(pHerd->GetId())
{
	
}

HerdTab::~HerdTab()
{
	
}

BEGIN_MESSAGE_MAP(HerdTab, TabPage)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


void HerdTab::OnSize( UINT nType,  int cx,  int cy  )
{
	TabPage::OnSize(nType,cx,cy);		
	m_TabNavigator.SetWindowPos(NULL, 0, 0, cx, cy - HERDNUMBERS_HEIGHT, SWP_NOZORDER);
	m_herdNumbers.SetWindowPos(NULL, 0, cy - HERDNUMBERS_HEIGHT, cx, HERDNUMBERS_HEIGHT, SWP_NOZORDER);
}

BOOL HerdTab::Create(UINT dwStyle, const CRect & rect, CWnd * pParentWnd, UINT nID)
{
	return TabPage::Create( dwStyle, rect, pParentWnd, nID);
}

int HerdTab::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(TabPage::OnCreate(lpCreateStruct) != 0)
		return -1;
	

	if (!m_TabNavigator.Create(CMFCTabCtrl::STYLE_3D_ONENOTE, CRect(), this, ID_TAB_ID, CMFCTabCtrl::Location::LOCATION_TOP))
		return -1;

	if (!m_herdNumbers.Create(CRect(), this, ID_STATUS_ID))
		return -1;

	SeasonSession* session = GetSeasonSession();
		
	m_InCowsTab.Create(WS_CHILD  ,CRect(),&m_TabNavigator,ID_INCOWS_TAB_ID);
	m_AllCowsTab.Create(WS_CHILD  ,CRect(),&m_TabNavigator,ID_ALLCOWS_TAB_ID);
	m_InDocsTab.Create(WS_CHILD  ,CRect(),&m_TabNavigator,ID_INDOCS_TAB_ID);
	m_MoveDocsTab.Create(WS_CHILD  ,CRect(),&m_TabNavigator,ID_MOVEDOCS_TAB_ID);
	m_BuyDocsTab.Create(WS_CHILD,CRect(),&m_TabNavigator,ID_BUYDOCS_TAB_ID);
	m_OutDocsTab.Create(WS_CHILD,CRect(),&m_TabNavigator,ID_OUTDOCS_TAB_ID);
	m_SellDocsTab.Create(WS_CHILD,CRect(),&m_TabNavigator,ID_SELLDOCS_TAB_ID);

	int herdIndex = m_pHerd->GetHerdIndex();
	uint32_t seasonId = session->GetSeason()->GetId();
	m_InCowsTabListHdrLayout = new HeaderLayoutConfiguration(TextUtils::Format(L"INCOWS@%03d.%d", herdIndex, seasonId));
	m_AllCowsTabListHdrLayout = new HeaderLayoutConfiguration(TextUtils::Format(L"ALLCOWS@%03d.%d", herdIndex, seasonId));
	m_InDocsTabListHdrLayout = new HeaderLayoutConfiguration(TextUtils::Format(L"INDOCS@%03d.%d", herdIndex, seasonId));
	m_OutDocsTabListHdrLayout = new HeaderLayoutConfiguration(TextUtils::Format(L"OUTDOCS@%03d.%d", herdIndex, seasonId));
	m_BuyDocsTabListHdrLayout = new HeaderLayoutConfiguration(TextUtils::Format(L"BUYDOCS@%03d.%d", herdIndex, seasonId));
	m_MoveDocsTabListHdrLayout = new HeaderLayoutConfiguration(TextUtils::Format(L"MOVEDOCS@%03d.%d", herdIndex, seasonId));
	m_SellDocsTabListHdrLayout = new HeaderLayoutConfiguration(TextUtils::Format(L"SELLDOCS@%03d.%d", herdIndex, seasonId));

	
	m_InCows.Create(session,m_pHerd->GetId());
	m_InCowsTab.SetContent(&m_InCows);
	m_InCowsTab.SetOnDblClick(std::bind(&HerdTab::OnInCowsEditCow,this));
	m_InCowsTab.SetOnRightClick(std::bind(&HerdTab::OnInCowsRClick,this,ph::_1,ph::_2));
	m_InCowsTabListHdrLayout->Load(session);
	m_InCowsTab.SetHeaderLayout(m_InCowsTabListHdrLayout);
	
	m_AllCows.Create(session,m_pHerd->GetId());
	m_AllCowsTab.SetContent(&m_AllCows);
	m_AllCowsTab.SetOnDblClick(std::bind(&HerdTab::OnAllCowsEditCow,this));
	m_AllCowsTab.SetOnRightClick(std::bind(&HerdTab::OnAllCowsRClick,this,ph::_1,ph::_2));
	m_AllCowsTabListHdrLayout->Load(session);
	m_AllCowsTab.SetHeaderLayout(m_AllCowsTabListHdrLayout);

	m_InDocs.Create(session,m_pHerd->GetId());
	m_InDocsTab.SetContent(&m_InDocs);
	m_InDocsTab.SetOnRightClick(std::bind(&HerdTab::OnInDocsRClick,this,ph::_1,ph::_2));
	m_InDocsTab.SetOnDblClick(std::bind(&HerdTab::OnEditInDoc,this));
	m_InDocsTabListHdrLayout->Load(session);
	m_InDocsTab.SetHeaderLayout(m_InDocsTabListHdrLayout);
	
	m_BuyDocs.Create(session,m_pHerd->GetId());
	m_BuyDocsTab.SetContent(&m_BuyDocs);
	m_BuyDocsTab.SetOnRightClick(std::bind(&HerdTab::OnBuyDocsRClick,this,ph::_1,ph::_2));
	m_BuyDocsTab.SetOnDblClick(std::bind(&HerdTab::OnEditBuyDoc,this));
	m_BuyDocsTabListHdrLayout->Load(session);
	m_BuyDocsTab.SetHeaderLayout(m_BuyDocsTabListHdrLayout);

	m_MoveDocs.Create(session,m_pHerd->GetId());
	m_MoveDocsTab.SetContent(&m_MoveDocs);
	m_MoveDocsTab.SetOnRightClick(std::bind(&HerdTab::OnMoveDocsRClick,this,ph::_1,ph::_2));	
	m_MoveDocsTab.SetOnDblClick(std::bind(&HerdTab::OnEditMoveDoc,this));
	m_MoveDocsTabListHdrLayout->Load(session);
	m_MoveDocsTab.SetHeaderLayout(m_MoveDocsTabListHdrLayout);
	
	m_OutDocs.Create(session,m_pHerd->GetId());
	m_OutDocsTab.SetContent(&m_OutDocs);	
	m_OutDocsTab.SetOnRightClick(std::bind(&HerdTab::OnOutDocsRClick,this,ph::_1,ph::_2));
	m_OutDocsTab.SetOnDblClick(std::bind(&HerdTab::OnEditOutDoc,this));
	m_OutDocsTabListHdrLayout->Load(session);
	m_OutDocsTab.SetHeaderLayout(m_OutDocsTabListHdrLayout);

	m_SellDocs.Create(session,m_pHerd->GetId());
	m_SellDocsTab.SetContent(&m_SellDocs);	
	m_SellDocsTab.SetOnRightClick(std::bind(&HerdTab::OnSellDocsRClick,this,ph::_1,ph::_2));
	m_SellDocsTab.SetOnDblClick(std::bind(&HerdTab::OnEditSellDoc,this));
	m_SellDocsTabListHdrLayout->Load(session);
	m_SellDocsTab.SetHeaderLayout(m_SellDocsTabListHdrLayout);

	m_TabNavigator.AddTab(&m_InCowsTab,StringLoader(IDS_INCOWS_TABNAME).c_str());
	m_TabNavigator.AddTab(&m_AllCowsTab,StringLoader(IDS_ALLCOWS_TABNAME).c_str());
	m_TabNavigator.AddTab(&m_InDocsTab,StringLoader(IDS_INDOCS_TABNAME).c_str());
	m_TabNavigator.AddTab(&m_BuyDocsTab,StringLoader(IDS_BUYDOCS_TABNAME).c_str());
	m_TabNavigator.AddTab(&m_MoveDocsTab,StringLoader(IDS_MOVEDOCS_TABNAME).c_str());
	m_TabNavigator.AddTab(&m_SellDocsTab,StringLoader(IDS_SELLDOCS_TABNAME).c_str());
	m_TabNavigator.AddTab(&m_OutDocsTab,StringLoader(IDS_OUTDOCS_TABNAME).c_str());
		
	return 0;
}

void HerdTab::OnSellCows()
{
	
}

void HerdTab::OnAllCowsEditCow()
{
	int row = m_AllCowsTab.GetSelectedRow();
	CowPresenceRow* pRow = m_AllCows.GetCowAtRow(row);
	CowPresenceView* pView = pRow->Content();
	EditCow(pView->GetKey().GetCowId());
}

void HerdTab::OnNewInDoc()
{
	SeasonSession* session = GetSeasonSession();
	InDocTransactionPtr inDocTransaction = session->NewInDoc(m_pHerd->GetId());
	InDocDlg inDocDlg(inDocTransaction,this);
	inDocDlg.DoModal();
}


void HerdTab::OnFastNewInDoc()
{
	SeasonSession* session = GetSeasonSession();
	InDocTransactionPtr inDocTransaction = session->NewInDoc(m_pHerd->GetId());
	FastInDocDlg inDocDlg(inDocTransaction, this);
	inDocDlg.DoModal();
}



void HerdTab::OnEditInDoc()
{
	int row = m_InDocsTab.GetSelectedRow();
	OwnDocRow* pRow = m_InDocs.GetDocAtRow(row);
	OwnDocView* pView = pRow->Content();
	EditInDoc(pView->GetDocId());
}

void HerdTab::OnEditBuyDoc()
{
	int row = m_BuyDocsTab.GetSelectedRow();
	HentsDocRow* pRow = m_BuyDocs.GetDocAtRow(row);
	HentsDocView* pView = pRow->Content();
	EditBuyDoc(pView->GetDocId());
}

void HerdTab::EditBuyDoc(uint32_t docId)
{
	ExceptionMsgBox box;
	box.TryCatchAny([this,docId]()
	{
		SeasonSession* session = GetSeasonSession();
		session->LockBuyDoc(docId);
		
		BuyDocTransactionPtr buyDocTransaction = session->EditBuyDoc(docId);
		BuyDocDlg buyDocDlg(buyDocTransaction,this);
		buyDocDlg.DoModal();

		session->UnlockBuyDoc(docId);
	});
}

void HerdTab::EditInDoc(uint32_t docId)
{
	ExceptionMsgBox box;
	box.TryCatchAny([this,docId]()
	{
		SeasonSession* session = GetSeasonSession();
		session->LockInDoc(docId);
		
		InDocTransactionPtr inDocTransaction = session->EditInDoc(docId);
		InDocDlg inDocDlg(inDocTransaction,this);
		inDocDlg.DoModal();

		session->UnlockInDoc(docId);
	});
}

void HerdTab::OnNewBuyDoc()
{
	SeasonSession* session = GetSeasonSession();
	BuyDocTransactionPtr transaction = session->NewBuyDoc(m_pHerd->GetId());
	BuyDocDlg buyDocDlg(transaction,this);
	buyDocDlg.DoModal();
}

void HerdTab::OnEditMoveDoc()
{
	int row = m_MoveDocsTab.GetSelectedRow();
	MoveDocRow* pRow = m_MoveDocs.GetMoveDocAtRow(row);
	MoveDocView* pView = pRow->Content();
	EditMoveDoc(pView->GetDocId());
}

void HerdTab::OnNewSellDoc()
{
	SeasonSession* session = GetSeasonSession();
	SellDocTransactionPtr transaction = session->NewSellDoc(m_pHerd->GetId());
	SellDocDlg sellDocDlg(transaction,this);
	sellDocDlg.DoModal();
}

void HerdTab::OnEditSellDoc()
{
	int row = m_SellDocsTab.GetSelectedRow();
	HentsDocRow* pRow = m_SellDocs.GetDocAtRow(row);
	HentsDocView* pView = pRow->Content();
	EditSellDoc(pView->GetDocId());
}

void HerdTab::EditSellDoc(uint32_t docId)
{
	ExceptionMsgBox box;
	box.TryCatchAny([this,docId]()
	{
		SeasonSession* session = GetSeasonSession();
		session->LockSellDoc(docId);

		SellDocTransactionPtr transaction = session->EditSellDoc(docId);
		SellDocDlg sellDocDlg(transaction,this);
		sellDocDlg.DoModal();

		session->UnlockSellDoc(docId);
	});
}


void HerdTab::OnNewOutDoc()
{
	SeasonSession* session = GetSeasonSession();
	OutDocTransactionPtr transaction = session->NewOutDoc(m_pHerd->GetId());
	OutDocDlg outDocDlg(transaction,this);
	outDocDlg.DoModal();
}

void HerdTab::OnEditOutDoc()
{
	int row = m_OutDocsTab.GetSelectedRow();
	OwnDocRow* pRow = m_OutDocs.GetDocAtRow(row);
	OwnDocView* pView = pRow->Content();
	EditOutDoc(pView->GetDocId());
}

void HerdTab::EditOutDoc(uint32_t docId)
{
	ExceptionMsgBox box;
	box.TryCatchAny([this,docId]()
	{
		SeasonSession* session = GetSeasonSession();
		session->LockOutDoc(docId);

		OutDocTransactionPtr transaction = session->EditOutDoc(docId);
		OutDocDlg outDocDlg(transaction,this);
		outDocDlg.DoModal();

		session->UnlockOutDoc(docId);
	});
}

void HerdTab::OnDestroy()
{
	SeasonSession* session = GetSeasonSession();
	
	m_BuyDocs.Destroy();
	m_SellDocs.Destroy();
	m_MoveDocs.Destroy();
	m_OutDocs.Destroy();
	m_InDocs.Destroy();
	m_InCows.Destroy();
	m_AllCows.Destroy();

	m_InCowsTabListHdrLayout->Save(session);
	m_AllCowsTabListHdrLayout->Save(session);
	m_InDocsTabListHdrLayout->Save(session);
	m_OutDocsTabListHdrLayout->Save(session);
	m_BuyDocsTabListHdrLayout->Save(session);
	m_MoveDocsTabListHdrLayout->Save(session);
	m_SellDocsTabListHdrLayout->Save(session);

	delete m_InCowsTabListHdrLayout;
	delete m_AllCowsTabListHdrLayout;
	delete m_InDocsTabListHdrLayout;
	delete m_OutDocsTabListHdrLayout;
	delete m_BuyDocsTabListHdrLayout;
	delete m_MoveDocsTabListHdrLayout;
	delete m_SellDocsTabListHdrLayout;

	TabPage::OnDestroy();
}

void HerdTab::EditCow(uint32_t cowId)
{
	SeasonSession* session = GetSeasonSession();
	Try()
	.Prepare([session,cowId]
	{
		session->LockCow(cowId);
	})
	.Do([session,cowId,this]
	{
		LonlyCowEditor editor(session);
		editor.Edit(cowId);
		CowDlg cowDlg(&editor,this);
		cowDlg.DoModal();	
	})
	.TearDown([session,cowId]
	{
		session->UnlockCow(cowId);
	})
	.Catch([](wexception& e)
	{
		ExceptionMsgBox().Show(e);
	})
	.Run();		
}

void HerdTab::OnInCowsRClick(int row,int cow)
{
	XContext* pContext = ContextAccess::GetXContext();
	
	MenuBuilder menu;
	DocumentSelection* pDocuments = NULL;

	if(row >= 0)
	{
		menu.add(StringLoader(IDS_EDIT),std::bind(&HerdTab::OnInCowsEditCow,this));
	
		if(pContext->GetHerdCount() > 1)
		{
			MenuBuilder herdMenu;
			PtrEnumerator<IHerd> herdEnum = pContext->EnumHerds();
			IHerd* pHerd;
			while(herdEnum.hasNext())
			{
				pHerd = *herdEnum;

				if(pHerd->GetId() != m_pHerd->GetId())
				{
					herdMenu.add(pHerd->GetHerdName(),std::bind(&HerdTab::MoveSelectedToHerd,this,pHerd->GetId()));
				}		
				herdEnum.advance();		
			}

			menu.addSubmenu(StringLoader(IDS_MOVETOHERD),herdMenu);
		}

		menu.addSeparator();
	
		MenuBuilder printMenu;
		DocumentManager* manager = AppServices::GetDocumentManager();

		pDocuments = new DocumentSelection(manager->FindDocuments(DocumentInput_Cow));

		PtrEnumerator<DocumentArchetype> archetypeEnum = pDocuments->Enum();

		DocumentArchetype* archetype;
		while(archetypeEnum.hasNext())
		{
			archetype = *archetypeEnum;

			

			printMenu.add(archetype->GetName(),std::bind(&HerdTab::OnPrintInCows,this,*archetype));
			archetypeEnum.advance();
		}

		menu.addSubmenu(StringLoader(IDS_PRINT),printMenu);

		menu.addSeparator();
	}

	menu.add(StringLoader(IDS_NEWINDOC),std::bind(&HerdTab::OnNewInDoc,this));
	menu.add(StringLoader(IDS_NEWBUYDOC),std::bind(&HerdTab::OnNewBuyDoc,this));
	menu.add(StringLoader(IDS_NEWOUTDOC),std::bind(&HerdTab::OnNewOutDoc,this));
	menu.add(StringLoader(IDS_NEWSELLDOC),std::bind(&HerdTab::OnNewSellDoc,this));
	menu.add(StringLoader(IDS_SELLCOWS), std::bind(&HerdTab::OnSellCows, this));
	menu.add(StringLoader(IDS_SEARCH), std::bind(&HerdTab::OnShowFindInCows, this));
	menu.add(StringLoader(IDS_EXPORT), std::bind(&HerdTab::OnExportCows, this, std::ref(m_InCowsTab)));

	menu.addSeparator();
	menu.add(StringLoader(IDS_SELECTALL), std::bind(&ListTab::SelectAll, &m_InCowsTab));
	menu.add(StringLoader(IDS_INVERTSELECTION), std::bind(&ListTab::InvertSelection, &m_InCowsTab));
	
	menu.track(this);
	delete pDocuments;
}



void HerdTab::OnPrintInCows(const DocumentArchetype& archetype)
{
	ListSelection selection = m_InCowsTab.GetSelection();
	SeasonSession* session = GetSeasonSession();

	if(selection.GetCount() > 0)
	{
		UniquePresenceKeys cowKeys;
		CowPresenceView* rowContent;	
		
		BOOST_FOREACH(int row,selection.GetRange())
		{
			rowContent = m_InCows.GetCowAtRow(row)->Content();
			cowKeys.add(rowContent->GetKey());
		}

		DocumentTerritoryObj territory(session->GetFarm(),m_pHerd);
		DocumentContext* context = DocumentContextBuilder().Create(&territory);
		DocumentInstance* instance = archetype.CreateDocument(session, context, cowKeys);
		DocumentPrintDlg printDlg(instance,this);		
		printDlg.DoModal();
		delete instance;
		delete context;
	
	}
}

void HerdTab::OnAllCowsRClick(int row,int cow)
{
	MenuBuilder menu;
	DocumentSelection* pDocuments = NULL;

	if(row >= 0)
	{
		menu.add(StringLoader(IDS_EDIT),std::bind(&HerdTab::OnAllCowsEditCow,this));
	
		menu.addSeparator();
	
		MenuBuilder printMenu;
		DocumentManager* manager = AppServices::GetDocumentManager();

		pDocuments = new DocumentSelection(manager->FindDocuments(DocumentInput_Cow));

		PtrEnumerator<DocumentArchetype> archetypeEnum = pDocuments->Enum();

		DocumentArchetype* archetype;
		while(archetypeEnum.hasNext())
		{
			archetype = *archetypeEnum;
			printMenu.add(archetype->GetName(),std::bind(&HerdTab::OnPrintAllCows,this,*archetype));
			archetypeEnum.advance();
		}

		menu.addSubmenu(StringLoader(IDS_PRINT),printMenu);

		menu.addSeparator();
	}

	menu.add(StringLoader(IDS_NEWINDOC),std::bind(&HerdTab::OnNewInDoc,this));
	menu.add(StringLoader(IDS_NEWBUYDOC),std::bind(&HerdTab::OnNewBuyDoc,this));
	menu.add(StringLoader(IDS_NEWOUTDOC),std::bind(&HerdTab::OnNewOutDoc,this));
	menu.add(StringLoader(IDS_NEWSELLDOC),std::bind(&HerdTab::OnNewSellDoc,this));
	menu.add(StringLoader(IDS_SEARCH), std::bind(&HerdTab::OnShowFindAllCows, this));
	menu.add(StringLoader(IDS_EXPORT), std::bind(&HerdTab::OnExportCows, this, std::ref(m_AllCowsTab)));
	menu.addSeparator();
	menu.add(StringLoader(IDS_SELECTALL), std::bind(&ListTab::SelectAll, &m_AllCowsTab));
	menu.add(StringLoader(IDS_INVERTSELECTION), std::bind(&ListTab::InvertSelection, &m_AllCowsTab));
	
	


	menu.track(this);
	delete pDocuments;
}

void HerdTab::OnPrintAllCows(const DocumentArchetype& archetype)
{
	ListSelection selection = m_AllCowsTab.GetSelection();
	SeasonSession* session = GetSeasonSession();

	if(selection.GetCount() > 0)
	{
		std::vector<CowPresenceView*> cows;
		CowPresenceView* rowContent;
		CowPresenceViewCreator viewCreator(session);
		HerdRegistry* pRegistry = session->GetHerdRegistry(m_pHerd->GetId());
		UniquePresenceKeys cowKeys;		

		BOOST_FOREACH(int row, selection.GetRange())
		{
			rowContent = m_AllCows.GetCowAtRow(row)->Content();
			cowKeys.add(rowContent->GetKey());
		}


		DocumentTerritoryObj territory(session->GetFarm(),m_pHerd);
		DocumentContext* context = DocumentContextBuilder().Create(&territory);
		DocumentInstance* instance = archetype.CreateDocument(session, context, cowKeys);
		DocumentPrintDlg printDlg(instance,this);		
		printDlg.DoModal();
		delete instance;
		delete context;
		DeepDelete(cows);
	}
}

void HerdTab::OnSellDocsRClick(int row,int cow)
{
	MenuBuilder menu;
	menu.add(StringLoader(IDS_ADD),std::bind(&HerdTab::OnNewSellDoc,this));

	if(row >= 0)
	{
		menu.addSeparator();
		menu.add(StringLoader(IDS_EDIT),std::bind(&HerdTab::OnEditSellDoc,this));

		menu.addSeparator();

		MenuBuilder printMenu;
		DocumentManager* manager = AppServices::GetDocumentManager();

		DocumentSelection selection(manager->FindDocuments(DocumentInput_SellDoc));
				
		PtrEnumerator<DocumentArchetype> archetypeEnum = selection.Enum();

		DocumentArchetype* archetype;
		while(archetypeEnum.hasNext())
		{
			archetype = *archetypeEnum;
			printMenu.add(archetype->GetName(),std::bind(&HerdTab::OnPrintSellDoc,this,*archetype));
			archetypeEnum.advance();
		}

		menu.addSubmenu(StringLoader(IDS_PRINT),printMenu);
		
		menu.addSeparator();

		menu.add(StringLoader(IDS_CREATEINVOICE),std::bind(&HerdTab::OnCreateSellDocInvoice,this,InvoiceType_Regular));
		menu.add(StringLoader(IDS_DELETE),std::bind(&HerdTab::OnDeleteSellDoc,this));
		menu.addSeparator();

		MenuBuilder exportMenu;
		exportMenu.add(StringLoader(IDS_EXPORTCOWS), std::bind(&HerdTab::OnExportSellDocCows, this));
		exportMenu.add(StringLoader(IDS_EXPORTDOCS), std::bind(&HerdTab::OnExportSellDocs, this));
		
		menu.addSubmenu(StringLoader(IDS_EXPORT), exportMenu);

	}	

	
	menu.add(StringLoader(IDS_SELECTALL), std::bind(&ListTab::SelectAll, &m_SellDocsTab));
	menu.add(StringLoader(IDS_INVERTSELECTION), std::bind(&ListTab::InvertSelection, &m_SellDocsTab));
	
	menu.track(this);
}

void HerdTab::OnDeleteSellDoc()
{
	int row = m_SellDocsTab.GetSelectedRow();
	HentsDocRow* pRowContent = m_SellDocs.GetDocAtRow(row);
	
	DocView* pDocView = pRowContent->Content();
	uint32_t docId = pDocView->GetDocId();
	SeasonSession* session = GetSeasonSession();
	ExceptionMsgBox msgBox;
	
	if (YesNoBox().Ask((boost::wformat(StringLoader(IDS_ASKDELETEDOC)) % pDocView->GetDocNo()).str()))
	{

		Try()
			.Prepare([session, docId]
		{
			session->LockSellDoc(docId);
		})
			.Do([session, docId]
		{
			session->DeleteSellDoc(docId);
		})
			.Fallback([session, docId]
		{
			session->UnlockSellDoc(docId);
		})
			.Catch([&msgBox](wexception& e)
		{
			msgBox.Show(e);
		})
			.Run();
	}
}

void HerdTab::OnInDocsRClick(int row,int cow)
{
	MenuBuilder menu;

	menu.add(StringLoader(IDS_ADD),std::bind(&HerdTab::OnNewInDoc,this));

	menu.add(StringLoader(IDS_FASTADD), std::bind(&HerdTab::OnFastNewInDoc, this));

	if(row >= 0)
	{
		menu.addSeparator();
		menu.add(StringLoader(IDS_EDIT),std::bind(&HerdTab::OnEditInDoc,this));

		MenuBuilder invoiceMenu;
		invoiceMenu.add(StringLoader(IDS_REGULARINVOICE), std::bind(&HerdTab::OnCreateBuyCowsInvoice, this,InvoiceType_Regular));
		invoiceMenu.add(StringLoader(IDS_LUMPINVOICE), std::bind(&HerdTab::OnCreateBuyCowsInvoice, this, InvoiceType_Lump));

		menu.addSubmenu(StringLoader(IDS_CREATEINVOICE), invoiceMenu);
		menu.add(StringLoader(IDS_DELETE),std::bind(&HerdTab::OnDeleteInDoc,this));
		menu.addSeparator();
	
		MenuBuilder printMenu;
		DocumentManager* manager = AppServices::GetDocumentManager();

		DocumentSelection selection(manager->FindDocuments(DocumentInput_InDoc));

		if(!selection.IsEmpty())
		{
			PtrEnumerator<DocumentArchetype> archetypeEnum = selection.Enum();

			DocumentArchetype* archetype;
			while(archetypeEnum.hasNext())
			{
				archetype = *archetypeEnum;
				printMenu.add(archetype->GetName(),std::bind(&HerdTab::OnPrintInDoc,this,*archetype));
				archetypeEnum.advance();
			}

			menu.addSubmenu(StringLoader(IDS_PRINT),printMenu);
			//menu.addSeparator();
		}
		
		MenuBuilder exportMenu;
		exportMenu.add(StringLoader(IDS_EXPORTCOWS), std::bind(&HerdTab::OnExportInDocCows, this));
		exportMenu.add(StringLoader(IDS_EXPORTDOCS), std::bind(&HerdTab::OnExportInDocs, this));

		menu.addSubmenu(StringLoader(IDS_EXPORT), exportMenu);
		//menu.addSubmenu(StringLoader(IDS_PASSPORTSEARCH,std::bind(&HerdTab::OnInDocPassportSearch,this));
		menu.addSeparator();
	}

	
	menu.add(StringLoader(IDS_SELECTALL), std::bind(&ListTab::SelectAll, &m_InDocsTab));
	menu.add(StringLoader(IDS_INVERTSELECTION), std::bind(&ListTab::InvertSelection, &m_InDocsTab));

	menu.track(this);
}

void HerdTab::OnPrintInDoc(const DocumentArchetype& archetype)
{
	ListSelection selection = m_InDocsTab.GetSelection();
	SeasonSession* session = GetSeasonSession();

	if(selection.GetCount() > 0)
	{
		UniqueIds docIds;
		OwnDocView* rowContent;
		BOOST_FOREACH(int row,selection.GetRange())
		{
			rowContent = m_InDocs.GetDocAtRow(row)->Content();
			docIds.add(rowContent->GetDocId());
		}

		DocumentTerritoryObj territory(session->GetFarm(),m_pHerd);
		DocumentContext* context = DocumentContextBuilder().Create(&territory);
		DocumentInstance* instance = archetype.CreateDocument(session,context,docIds);
		DocumentPrintDlg printDlg(instance,this);		
		printDlg.DoModal();
		delete instance;
		delete context;
		
	}
}

void HerdTab::OnMoveDocsRClick(int row,int cow)
{
	MenuBuilder menu;

	if(row >= 0)
	{
		

		menu.add(StringLoader(IDS_EDIT),std::bind(&HerdTab::OnEditMoveDoc,this));
		menu.add(StringLoader(IDS_DELETE),std::bind(&HerdTab::OnDeleteMoveDoc,this));

		menu.addSeparator();
	
		MenuBuilder printMenu;
		DocumentManager* manager = AppServices::GetDocumentManager();

		DocumentSelection selection(manager->FindDocuments(DocumentInput_MoveDoc));

		if(!selection.IsEmpty())
		{
			PtrEnumerator<DocumentArchetype> archetypeEnum = selection.Enum();

			DocumentArchetype* archetype;
			while(archetypeEnum.hasNext())
			{
				archetype = *archetypeEnum;
				printMenu.add(archetype->GetName(),std::bind(&HerdTab::OnPrintMoveDoc,this,*archetype));
				archetypeEnum.advance();
			}

			menu.addSubmenu(StringLoader(IDS_PRINT),printMenu);
			//menu.addSeparator();
		}

		menu.add(StringLoader(IDS_EXPORT), std::bind(&HerdTab::OnExportMoveDocCows, this));
		MenuBuilder exportMenu;
		exportMenu.add(StringLoader(IDS_EXPORTCOWS), std::bind(&HerdTab::OnExportMoveDocCows, this));
		exportMenu.add(StringLoader(IDS_EXPORTDOCS), std::bind(&HerdTab::OnExportMoveDocs, this));

		
		menu.addSeparator();
	}


	menu.add(StringLoader(IDS_SELECTALL), std::bind(&ListTab::SelectAll, &m_InDocsTab));
	menu.add(StringLoader(IDS_INVERTSELECTION), std::bind(&ListTab::InvertSelection, &m_InDocsTab));
	menu.track(this);

	
}

void HerdTab::OnBuyDocsRClick(int row,int cow)
{
	MenuBuilder menu;
	menu.add(StringLoader(IDS_ADD),std::bind(&HerdTab::OnNewBuyDoc,this));

	if(row >= 0)
	{
		menu.addSeparator();
		menu.add(StringLoader(IDS_EDIT),std::bind(&HerdTab::OnEditBuyDoc,this));

		menu.addSeparator();

		MenuBuilder printMenu;
		DocumentManager* manager = AppServices::GetDocumentManager();

		DocumentSelection selection(manager->FindDocuments(DocumentInput_BuyDoc));

		PtrEnumerator<DocumentArchetype> archetypeEnum = selection.Enum();

		DocumentArchetype* archetype;
		while(archetypeEnum.hasNext())
		{
			archetype = *archetypeEnum;
			printMenu.add(archetype->GetName(),std::bind(&HerdTab::OnPrintBuyDoc,this,*archetype));
			archetypeEnum.advance();
		}

		menu.addSubmenu(StringLoader(IDS_PRINT),printMenu);
		
		menu.addSeparator();

		
		MenuBuilder invoiceMenu;
		invoiceMenu.add(StringLoader(IDS_REGULARINVOICE), std::bind(&HerdTab::OnCreateBuyDocInvoice, this, InvoiceType_Regular));
		invoiceMenu.add(StringLoader(IDS_LUMPINVOICE), std::bind(&HerdTab::OnCreateBuyDocInvoice, this, InvoiceType_Lump));

		menu.addSubmenu(StringLoader(IDS_CREATEINVOICE), invoiceMenu);
		
		MenuBuilder exportMenu;
		exportMenu.add(StringLoader(IDS_EXPORTCOWS), std::bind(&HerdTab::OnExportBuyDocCows, this));
		exportMenu.add(StringLoader(IDS_EXPORTDOCS), std::bind(&HerdTab::OnExportBuyDocs, this));

		menu.addSubmenu(StringLoader(IDS_EXPORT), exportMenu);

		menu.add(StringLoader(IDS_DELETE),std::bind(&HerdTab::OnDeleteBuyDoc,this));
		menu.addSeparator();
	}


	menu.add(StringLoader(IDS_SELECTALL), std::bind(&ListTab::SelectAll, &m_BuyDocsTab));
	menu.add(StringLoader(IDS_INVERTSELECTION), std::bind(&ListTab::InvertSelection, &m_BuyDocsTab));
		
	menu.track(this);
	
}

void HerdTab::OnDeleteInDoc()
{
	int row = m_InDocsTab.GetSelectedRow();
	OwnDocRow* pRowContent = m_InDocs.GetDocAtRow(row);
	OwnDocView* pDocView = pRowContent->Content();	

	if(YesNoBox().Ask((boost::wformat(StringLoader(IDS_ASKDELETEDOC)) % pDocView->GetDocNo()).str()))
	{
		uint32_t docId = pDocView->GetDocId();
		SeasonSession* session = GetSeasonSession();

		ExceptionMsgBox msgBox;

		Try()
		.Prepare([session,docId]	
		{
			session->LockInDoc(docId);
		})
		.Do([session,docId]
		{
			session->DeleteInDoc(docId);		
		})
		.Fallback([session,docId]
		{
			session->UnlockInDoc(docId);					
		})
		.Catch([&msgBox](wexception& e)
		{
			msgBox.Show(e);
		})
		.Run();
	}
}

void HerdTab::OnDeleteBuyDoc()
{
	int row = m_BuyDocsTab.GetSelectedRow();
	HentsDocRow* pRowContent = m_BuyDocs.GetDocAtRow(row);
	DocView* pDocView = pRowContent->Content();
	uint32_t docId = pDocView->GetDocId();
	SeasonSession* session = GetSeasonSession();
	
	ExceptionMsgBox msgBox;
	
	if (YesNoBox().Ask((boost::wformat(StringLoader(IDS_ASKDELETEDOC)) % pDocView->GetDocNo()).str()))
	{
		Try()
			.Prepare([session, docId]
		{
			session->LockBuyDoc(docId);
		})
			.Do([session, docId]
		{
			session->DeleteBuyDoc(docId);
		})
			.Fallback([session, docId]
		{
			session->UnlockBuyDoc(docId);
		})
			.Catch([&msgBox](wexception& e)
		{
			msgBox.Show(e);
		})
			.Run();
	}
}

void HerdTab::OnDeleteOutDoc()
{
	int row = m_OutDocsTab.GetSelectedRow();
	OwnDocRow* pRowContent = m_OutDocs.GetDocAtRow(row);
	DocView* pDocView = pRowContent->Content();
	uint32_t docId = pDocView->GetDocId();
	SeasonSession* session = GetSeasonSession();

	ExceptionMsgBox msgBox;
	
	if (YesNoBox().Ask((boost::wformat(StringLoader(IDS_ASKDELETEDOC)) % pDocView->GetDocNo()).str()))
	{

		Try()
			.Prepare([session, docId]
		{
			session->LockOutDoc(docId);
		})
			.Do([session, docId]
		{
			session->DeleteOutDoc(docId);
		})
			.Fallback([session, docId]
		{
			session->UnlockOutDoc(docId);
		})
			.Catch([&msgBox](wexception& e)
		{
			msgBox.Show(e);
		})
			.Run();
	}
}

void HerdTab::OnCreateBuyDocInvoice(InvoiceType type)
{
	SeasonSession* session = GetSeasonSession();
	DefaultValuesApplier applier(session);

	InvoiceDocsTransactionPtr transaction = session->NewBuyInvoiceDocs();
	transaction->SetInvoiceType(type);
	applier.ApplyTo(transaction->GetFacade());

	DateTime nowDt = DateTime::now();

	InvoiceDetails details = transaction->GetDetails();
	details.SetInvoiceDate(nowDt);
	details.SetTransactionDate(nowDt);
	transaction->UpdateDetails(details);

	int rowIndex = m_BuyDocsTab.GetSelectedRow();
	HentsDocRow* pRow = m_BuyDocs.GetDocAtRow(rowIndex);

	ExceptionMsgBox().TryCatchAny([this,transaction,pRow]
	{
		transaction->AddDoc(pRow->Content()->GetDocId());
		CBuyInvoiceDocScopeDlg invoiceDlg(transaction,this);
		invoiceDlg.DoModal();
	});
}

void HerdTab::OnCreateSellDocInvoice(InvoiceType type)
{
	SeasonSession* session = GetSeasonSession();
	DefaultValuesApplier applier(session);
	
	InvoiceDocsTransactionPtr transaction = session->NewSellInvoiceDocs();
	transaction->SetInvoiceType(type);
	applier.ApplyTo(transaction->GetFacade());

	DateTime nowDt = DateTime::now();
	InvoiceDetails details = transaction->GetDetails();
	details.SetInvoiceDate(nowDt);
	details.SetTransactionDate(nowDt);
	transaction->UpdateDetails(details);

	int rowIndex = m_SellDocsTab.GetSelectedRow();
	HentsDocRow* pRow = m_SellDocs.GetDocAtRow(rowIndex);

	ExceptionMsgBox().TryCatchAny([this,transaction,pRow]
	{
		transaction->AddDoc(pRow->Content()->GetDocId());
		CSellInvoiceDocScopeDlg invoiceDlg(transaction,this);
		invoiceDlg.DoModal();
	});
}

void HerdTab::OnCreateSellCowsInvoice(InvoiceType type)
{
	int rowIndex = m_OutDocsTab.GetSelectedRow();
	OwnDocRow* pRow = m_OutDocs.GetDocAtRow(rowIndex);	
	SeasonSession* session = GetSeasonSession();
	DefaultValuesApplier applier(session);

	try
	{
		InvoiceCowsTransactionPtr transaction = session->NewSellInvoiceCows();
		transaction->SetInvoiceType(type);
		applier.ApplyTo(transaction->GetFacade());

		DateTime nowDt = DateTime::now();
		InvoiceDetails details = transaction->GetDetails();
		details.SetInvoiceDate(nowDt);
		details.SetTransactionDate(nowDt);
		transaction->UpdateDetails(details);

		transaction->SetDocId(pRow->Content()->GetDocId());

		SellInvoiceCowsSelectInterface selectInterface;
		selectInterface.Create(session,transaction->GetFacade());

		CowSelectDlg cowSelectDlg(&selectInterface,this);
		cowSelectDlg.SetHeader(CowListHeader());
		cowSelectDlg.DoModal();

		if(transaction->GetItemCount() > 0)
		{
			CSellInvoiceCowScopeDlg sellInvoiceDlg(transaction,this);
			sellInvoiceDlg.DoModal();
		}
		else
		{
			transaction->Rollback();
		}
	}
	catch(wexception& e)
	{
		AfxMessageBox(e.wwhat(),MB_OK);
	}
}

void HerdTab::OnOutDocsRClick(int row,int cow)
{
	MenuBuilder menu;
	menu.add(StringLoader(IDS_ADD),std::bind(&HerdTab::OnNewOutDoc,this));

	if(row >= 0)
	{
		menu.addSeparator();
		menu.add(StringLoader(IDS_EDIT),std::bind(&HerdTab::OnEditOutDoc,this));
	
		menu.addSeparator();
	
		MenuBuilder printMenu;
		DocumentManager* manager = AppServices::GetDocumentManager();

		DocumentSelection selection(manager->FindDocuments(DocumentInput_OutDoc));

		PtrEnumerator<DocumentArchetype> archetypeEnum = selection.Enum();

		DocumentArchetype* archetype;
		while(archetypeEnum.hasNext())
		{
			archetype = *archetypeEnum;
			printMenu.add(archetype->GetName(),std::bind(&HerdTab::OnPrintOutDoc,this,*archetype));
			archetypeEnum.advance();
		}

		menu.addSubmenu(StringLoader(IDS_PRINT),printMenu);

		menu.addSeparator();

		menu.add(StringLoader(IDS_CREATEINVOICE),std::bind(&HerdTab::OnCreateSellCowsInvoice,this,InvoiceType_Regular));

		MenuBuilder exportMenu;
		exportMenu.add(StringLoader(IDS_EXPORTCOWS), std::bind(&HerdTab::OnExportOutDocCows, this));
		exportMenu.add(StringLoader(IDS_EXPORTDOCS), std::bind(&HerdTab::OnExportOutDocs, this));

		menu.addSubmenu(StringLoader(IDS_EXPORT), exportMenu);

		
		menu.add(StringLoader(IDS_DELETE),std::bind(&HerdTab::OnDeleteOutDoc,this));
		menu.addSeparator();
	}

	menu.add(StringLoader(IDS_SELECTALL), std::bind(&ListTab::SelectAll, &m_OutDocsTab));
	menu.add(StringLoader(IDS_INVERTSELECTION), std::bind(&ListTab::InvertSelection, &m_OutDocsTab));

	menu.track(this);
	
}

void HerdTab::OnPrintOutDoc(const DocumentArchetype& archetype)
{
	ListSelection selection = m_OutDocsTab.GetSelection();
	SeasonSession* session = GetSeasonSession();

	if(selection.GetCount() > 0)
	{
		UniqueIds docIds;
		OwnDocView* rowContent;
		
		BOOST_FOREACH(int row,selection.GetRange())
		{
			rowContent = m_OutDocs.GetDocAtRow(row)->Content();
			docIds.add(rowContent->GetDocId());
		}

		DocumentTerritoryObj territory(session->GetFarm(),m_pHerd);
		DocumentContext* context = DocumentContextBuilder().Create(&territory);
		DocumentInstance* instance = archetype.CreateDocument(session,context,docIds);
		DocumentPrintDlg printDlg(instance,this);		
		printDlg.DoModal();
		delete instance;
		delete context;
	}
}

void HerdTab::OnPrintBuyDoc(const DocumentArchetype& archetype)
{
	ListSelection selection = m_BuyDocsTab.GetSelection();
	SeasonSession* session = GetSeasonSession();

	if(selection.GetCount() > 0)
	{
		UniqueIds buyDocs;
		HentsDocView* rowContent;
		BOOST_FOREACH(int row,selection.GetRange())
		{
			rowContent = m_BuyDocs.GetDocAtRow(row)->Content();
			buyDocs.add(rowContent->GetDocId());
		}

		 
		DocumentTerritoryObj territory(session->GetFarm(),m_pHerd);
		boost::scoped_ptr<DocumentContext> context(DocumentContextBuilder().Create(&territory));

		try
		{
			DocumentInstance* instance = archetype.CreateDocument(session, context.get(), buyDocs);
			DocumentPrintDlg printDlg(instance, this);
			printDlg.DoModal();
			delete instance;
		}
		catch (FeedException& e)
		{
			ExceptionMsgBox().Show(e);
		}
		
		
	}
}

void HerdTab::OnPrintSellDoc(const DocumentArchetype& archetype)
{
	ListSelection selection = m_SellDocsTab.GetSelection();
	SeasonSession* session = GetSeasonSession();

	if(selection.GetCount() > 0)
	{
		UniqueIds sellDocs;
		HentsDocView* rowContent;
		BOOST_FOREACH(int row,selection.GetRange())
		{
			rowContent = m_SellDocs.GetDocAtRow(row)->Content();
			sellDocs.add(rowContent->GetDocId());
		}

		DocumentTerritoryObj territory(session->GetFarm(),m_pHerd);
		DocumentContext* context = DocumentContextBuilder().Create(&territory);
		DocumentInstance* instance = archetype.CreateDocument(session,context,sellDocs);
		DocumentPrintDlg printDlg(instance,this);		
		printDlg.DoModal();
		delete instance;		
		delete context;
	}
}

void HerdTab::OnPrintMoveDoc(const DocumentArchetype& archetype)
{
	ListSelection selection = m_MoveDocsTab.GetSelection();
	SeasonSession* session = GetSeasonSession();

	if(selection.GetCount() > 0)
	{
		UniqueIds moveDocIds;
		MoveDocView* rowContent;
		BOOST_FOREACH(int row,selection.GetRange())
		{
			rowContent = m_MoveDocs.GetMoveDocAtRow(row)->Content();
			moveDocIds.add(rowContent->GetDocId());
		}

		DocumentTerritoryObj territory(session->GetFarm(),m_pHerd);
		DocumentContext* context = DocumentContextBuilder().Create(&territory);
		DocumentInstance* instance = archetype.CreateDocument(session,context,moveDocIds);
		DocumentPrintDlg printDlg(instance,this);		
		printDlg.DoModal();
		delete instance;		
		delete context;
	}
}

void HerdTab::OnDeleteMoveDoc()
{
	int row = m_MoveDocsTab.GetSelectedRow();
	MoveDocRow* pRowContent = m_MoveDocs.GetMoveDocAtRow(row);
	DocView* pDocView = pRowContent->Content();
	uint32_t docId = pDocView->GetDocId();
	SeasonSession* session = GetSeasonSession();

	ExceptionMsgBox msgBox;
	
	if (YesNoBox().Ask((boost::wformat(StringLoader(IDS_ASKDELETEDOC)) % pDocView->GetDocNo()).str()))
	{

		Try()
			.Prepare([session, docId]
		{
			session->LockMoveDoc(docId);
		})
			.Do([session, docId]
		{
			session->DeleteMoveDoc(docId);
		})
			.Fallback([session, docId]
		{
			session->UnlockMoveDoc(docId);
		})
			.Catch([&msgBox](wexception& e)
		{
			msgBox.Show(e);
		})
			.Run();
	}
}

void HerdTab::MoveSelectedToHerd(uint32_t herdId)
{
	ListSelection selection = m_InCowsTab.GetSelection();

	SeasonSession* session = GetSeasonSession();
	MoveDocTransactionPtr movTransaction = session->NewMoveDoc(m_pHerd->GetId(),herdId);
	SelectionRange selRange = selection.GetRange();

	BOOST_FOREACH(int row,selRange)
	{
		CowPresenceRow* pRow = m_InCows.GetCowAtRow(row);
		uint32_t cowId = pRow->Content()->GetKey().GetCowId();
		movTransaction->AddCow(cowId);
	}

	MoveDocDlg moveDlg(movTransaction,this);
	moveDlg.DoModal();
}

void HerdTab::OnInCowsEditCow()
{
	int nRow = m_InCowsTab.GetSelectedRow();
	if(nRow >= 0)
	{
		CowPresenceRow* pRow = m_InCows.GetCowAtRow(nRow);
		CowPresenceView* pView = pRow->Content();
		EditCow(pView->GetKey().GetCowId());	
	}
}

void HerdTab::EditMoveDoc(uint32_t docId)
{
	SeasonSession* session = GetSeasonSession();

	ExceptionMsgBox msgBox;
	
	Try()
	.Prepare([session,docId]	
	{
		session->LockMoveDoc(docId);
	})
	.Do([this,session,docId]
	{
		MoveDocTransactionPtr moveDocTransaction = session->EditMoveDoc(docId);
		MoveDocDlg movDocDlg(moveDocTransaction,this);
		movDocDlg.DoModal();
	})
	.TearDown([session,docId]
	{
		session->UnlockMoveDoc(docId);
	})
	.Catch([&msgBox](wexception& e)
	{
		msgBox.Show(e);
	})
	.Run();
}

void HerdTab::OnCreateBuyCowsInvoice(InvoiceType type)
{
	int rowIndex = m_InDocsTab.GetSelectedRow();
	OwnDocRow* pRow = m_InDocs.GetDocAtRow(rowIndex);	
	SeasonSession* session = GetSeasonSession();
	DefaultValuesApplier applier(session);
	try
	{
		InvoiceCowsTransactionPtr transaction = session->NewBuyInvoiceCows();
		transaction->SetInvoiceType(type);
		applier.ApplyTo(transaction->GetFacade());

		DateTime nowDt = DateTime::now();
		InvoiceDetails details = transaction->GetDetails();
		details.SetInvoiceDate(nowDt);
		details.SetTransactionDate(nowDt);
		transaction->UpdateDetails(details);
		
		transaction->SetDocId(pRow->Content()->GetDocId());

		BuyInvoiceCowsSelectInterface selectInterface;
		selectInterface.Create(session,transaction->GetFacade());

		CowSelectDlg cowSelectDlg(&selectInterface,this);
		cowSelectDlg.SetHeader(CowListHeader());
		cowSelectDlg.DoModal();

		if(transaction->GetItemCount() > 0)
		{
			CBuyInvoiceCowScopeDlg buyInvoiceDlg(transaction,this);
			buyInvoiceDlg.DoModal();
		}
		else
		{
			transaction->Rollback();
		}
	}
	catch(wexception& e)
	{
		AfxMessageBox(e.wwhat(),MB_OK);
	}
		
}

void HerdTab::OnShowFindInCows()
{
	m_InCowsTab.SendMessage(WM_COMMAND, MAKEWPARAM(ID_SHOWFIND,0));
}

void HerdTab::OnShowFindAllCows()
{
	m_AllCowsTab.SendMessage(WM_COMMAND, MAKEWPARAM(ID_SHOWFIND, 0));
}

void HerdTab::ExportCows(ExcelExport* exporter, const std::wstring& xlsFile, CowsTab& tab)
{
	SeasonSession* session = GetSeasonSession();

	std::vector<ICowPresence*> cows;
	CowPresenceView* rowContent;
	HerdRegistry* registry = session->GetHerdRegistry(m_pHerd->GetId());
	ListContent* content = tab.GetContent();

	ListSelection selection = tab.GetSelection();

	if (selection.GetCount() > 0)
	{

		BOOST_FOREACH(int row, selection.GetRange())
		{
			rowContent = ((CowPresenceRow*)content->GetRow(row))->Content();
			ICowPresence* presence = registry->GetCowPresence(rowContent->GetKey());
			if (presence != NULL)
			{
				cows.push_back(presence);
			}
		}
		exporter->Export(xlsFile,cows);
	}
}

void HerdTab::OnExportCows(CowsTab& tab)
{
	std::function<void(ExcelExport*, const std::wstring&)> exportCall = std::bind(&HerdTab::ExportCows, this, std::placeholders::_1, std::placeholders::_2, std::ref(tab));
	CowPresenceListHeader cowsHeader;
	Export(exportCall, cowsHeader, L"COWS_EXCELEXPORT");
}

boost::optional<ExcelExportSettings> HerdTab::ConfigureExcelExport(const ListHeaderDescriptor& headerFields, const std::wstring& settingName) {
	
	boost::optional<ExcelExportSettings> result;
	SeasonSession* session = GetSeasonSession();
	UserSettings* userSettings = session->GetContext()->GetUserSettings();
	ExcelExportSettings setting = ExcelExportSettings::Load(userSettings, settingName);
	
	ExcelExportSettingsDlg exportSettingsDlg(&setting, &headerFields, this);
	if (exportSettingsDlg.Configure())
	{
		setting.Save(userSettings, settingName);
		result = setting;
	}

	return result;
}


void HerdTab::Export(std::function<void (ExcelExport*,const std::wstring&)>& exportCall, const ListHeaderDescriptor& headerFields, const std::wstring& settingName)
{

	SeasonSession* session = GetSeasonSession();
	boost::optional<ExcelExportSettings> settings = ConfigureExcelExport(headerFields, settingName);
	if(settings)
	{		
		ExcelExport exporter(session, settings.get_ptr());		
		std::wstring extension(exporter.GetFileExtension());
		std::wstring filter = TextUtils::Format(L"%s  (*.%s)|*.%s||", extension, extension, extension);
		CFileDialog saveFileDlg(FALSE, extension.c_str(), NULL, OFN_OVERWRITEPROMPT, filter.c_str(), this);

		if (saveFileDlg.DoModal() == IDOK)
		{
			std::wstring outputFilePathName = (LPCTSTR)saveFileDlg.GetPathName();
			exportCall(&exporter, outputFilePathName);
		}
	}
}



void HerdTab::OnExportSellDocCows()
{
	std::function<void(ExcelExport*, const std::wstring&)> exportCall = std::bind(&HerdTab::ExportSellDocCows, this, std::placeholders::_1, std::placeholders::_2);
	CowPresenceListHeader header;
	Export(exportCall, header, L"SELLDOC_COWS_EXCELEXPORT");
}


void HerdTab::ExportSellDocCows(ExcelExport* exporter, const std::wstring& xlsFile)
{
	SeasonSession* session = GetSeasonSession();

	std::vector<ISellDoc*> sellDocs;	
	ListContent* content = m_SellDocsTab.GetContent();

	ListSelection selection = m_SellDocsTab.GetSelection();

	if (selection.GetCount() > 0)
	{

		BOOST_FOREACH(int row, selection.GetRange())
		{
			HentsDocView* rowContent = ((HentsDocRow*)content->GetRow(row))->Content();
			SellDocClientObject* sellDocCO = session->GetSellDoc(rowContent->GetDocId());
			if (sellDocCO != NULL)
			{
				sellDocs.push_back(sellDocCO->Object());
			}
		}
		exporter->Export(xlsFile, sellDocs);
	}
}

void HerdTab::ExportInDocCows(ExcelExport* exporter, const std::wstring& xlsFile)
{
	SeasonSession* session = GetSeasonSession();

	std::vector<IInDoc*> inDocs;
	ListContent* content = m_InDocsTab.GetContent();
	ListSelection selection = m_InDocsTab.GetSelection();

	if (selection.GetCount() > 0)
	{
		BOOST_FOREACH(int row, selection.GetRange())
		{
			OwnDocView* rowContent = ((OwnDocRow*)content->GetRow(row))->Content();
			InDocClientObject* inDocCO = session->GetInDoc(rowContent->GetDocId());
			if (inDocCO != NULL)
			{
				inDocs.push_back(inDocCO->Object());
			}
		}
		exporter->Export(xlsFile, inDocs);
	}
}

void HerdTab::OnExportInDocCows()
{
	std::function<void(ExcelExport*, const std::wstring&)> exportCall = std::bind(&HerdTab::ExportInDocCows, this, std::placeholders::_1, std::placeholders::_2);
	OwnDocsListHeader header;
	Export(exportCall, header, L"INDOC_COWS_EXCELEXPORT");

}

void HerdTab::ExportMoveDocCows(ExcelExport* exporter, const std::wstring& xlsFile)
{
	SeasonSession* session = GetSeasonSession();

	std::vector<IMoveDoc*> moveDocs;
	ListContent* content = m_MoveDocsTab.GetContent();
	ListSelection selection = m_MoveDocsTab.GetSelection();

	if (selection.GetCount() > 0)
	{

		BOOST_FOREACH(int row, selection.GetRange())
		{
			MoveDocView* rowContent = ((MoveDocRow*)content->GetRow(row))->Content();
			MoveDocClientObject* moveDocCO = session->GetMoveDoc(rowContent->GetDocId());
			if (moveDocCO != NULL)
			{
				moveDocs.push_back(moveDocCO->Object());
			}
		}
		exporter->Export(xlsFile, m_pHerd->GetId(), moveDocs);
	}
}


void HerdTab::OnExportMoveDocCows()
{
	std::function<void(ExcelExport*, const std::wstring&)> exportCall = std::bind(&HerdTab::ExportMoveDocCows, this, std::placeholders::_1, std::placeholders::_2);
	MoveDocsListHeader header;
	Export(exportCall, header, L"MOVEDOC_COWS_EXCELEXPORT");

}


void HerdTab::ExportBuyDocCows(ExcelExport* exporter, const std::wstring& xlsFile)
{
	SeasonSession* session = GetSeasonSession();

	std::vector<IBuyDoc*> buyDocs;
	ListContent* content = m_BuyDocsTab.GetContent();
	ListSelection selection = m_BuyDocsTab.GetSelection();

	if (selection.GetCount() > 0)
	{

		BOOST_FOREACH(int row, selection.GetRange())
		{
			HentsDocView* rowContent = ((HentsDocRow*)content->GetRow(row))->Content();
			BuyDocClientObject* buyDocCO = session->GetBuyDoc(rowContent->GetDocId());
			if (buyDocCO != NULL)
			{
				buyDocs.push_back(buyDocCO->Object());
			}
		}
		exporter->Export(xlsFile, buyDocs);
	}
}

void HerdTab::OnExportBuyDocCows()
{
	std::function<void(ExcelExport*, const std::wstring&)> exportCall = std::bind(&HerdTab::ExportBuyDocCows, this, std::placeholders::_1, std::placeholders::_2);
	HentsDocsListHeader header;
	Export(exportCall, header, L"BUYDOC_COWS_EXCELEXPORT");

}

void HerdTab::ExportOutDocCows(ExcelExport* exporter, const std::wstring& xlsFile)
{
	SeasonSession* session = GetSeasonSession();

	std::vector<IOutDoc*> outDocs;
	ListContent* content = m_OutDocsTab.GetContent();
	ListSelection selection = m_OutDocsTab.GetSelection();

	if (selection.GetCount() > 0)
	{

		BOOST_FOREACH(int row, selection.GetRange())
		{
			OwnDocView* rowContent = ((OwnDocRow*)content->GetRow(row))->Content();
			OutDocClientObject* outDocCO = session->GetOutDoc(rowContent->GetDocId());
			if (outDocCO != NULL)
			{
				outDocs.push_back(outDocCO->Object());
			}
		}
		exporter->Export(xlsFile, outDocs);
	}
}

void HerdTab::OnExportOutDocCows()
{
	std::function<void(ExcelExport*, const std::wstring&)> exportCall = std::bind(&HerdTab::ExportOutDocCows, this, std::placeholders::_1, std::placeholders::_2);
	OwnDocsListHeader header;
	Export(exportCall, header, L"OUTDOC_COWS_EXCELEXPORT");
}



void HerdTab::ExportOutDocs(ExcelExport* exporter, const std::wstring& xlsFile)
{
	SeasonSession* session = GetSeasonSession();

	std::vector<IOutDoc*> outDocs;
	ListContent* content = m_OutDocsTab.GetContent();
	ListSelection selection = m_OutDocsTab.GetSelection();

	if (selection.GetCount() > 0)
	{

		BOOST_FOREACH(int row, selection.GetRange())
		{
			OwnDocView* rowContent = ((OwnDocRow*)content->GetRow(row))->Content();
			OutDocClientObject* outDocCO = session->GetOutDoc(rowContent->GetDocId());
			if (outDocCO != NULL)
			{
				outDocs.push_back(outDocCO->Object());
			}
		}
		exporter->ExportDocs(xlsFile, outDocs);
	}
}

void HerdTab::OnExportOutDocs()
{
	std::function<void(ExcelExport*, const std::wstring&)> exportCall = std::bind(&HerdTab::ExportOutDocs, this, std::placeholders::_1, std::placeholders::_2);
	OwnDocsListHeader docsHeader;
	Export(exportCall, docsHeader, L"OUTDOC_EXCELEXPORT");
}

void HerdTab::ExportBuyDocs(ExcelExport* exporter, const std::wstring& xlsFile)
{
	SeasonSession* session = GetSeasonSession();

	std::vector<IBuyDoc*> buyDocs;
	ListContent* content = m_BuyDocsTab.GetContent();
	ListSelection selection = m_BuyDocsTab.GetSelection();

	if (selection.GetCount() > 0)
	{

		BOOST_FOREACH(int row, selection.GetRange())
		{
			HentsDocView* rowContent = ((HentsDocRow*)content->GetRow(row))->Content();
			BuyDocClientObject* buyDocCO = session->GetBuyDoc(rowContent->GetDocId());
			if (buyDocCO != NULL)
			{
				buyDocs.push_back(buyDocCO->Object());
			}
		}
		exporter->ExportDocs(xlsFile, buyDocs);
	}
}

void HerdTab::OnExportBuyDocs()
{
	std::function<void(ExcelExport*, const std::wstring&)> exportCall = std::bind(&HerdTab::ExportBuyDocs, this, std::placeholders::_1, std::placeholders::_2);
	HentsDocsListHeader docsHeader;
	Export(exportCall, docsHeader, L"BUYDOC_EXCELEXPORT");
}

void HerdTab::ExportInDocs(ExcelExport* exporter, const std::wstring& xlsFile)
{
	SeasonSession* session = GetSeasonSession();

	std::vector<IInDoc*> inDocs;
	ListContent* content = m_InDocsTab.GetContent();
	ListSelection selection = m_InDocsTab.GetSelection();

	if (selection.GetCount() > 0)
	{
		BOOST_FOREACH(int row, selection.GetRange())
		{
			OwnDocView* rowContent = ((OwnDocRow*)content->GetRow(row))->Content();
			InDocClientObject* inDocCO = session->GetInDoc(rowContent->GetDocId());
			if (inDocCO != NULL)
			{
				inDocs.push_back(inDocCO->Object());
			}
		}
		exporter->ExportDocs(xlsFile, inDocs);
	}
}

void HerdTab::OnExportInDocs()
{
	std::function<void(ExcelExport*, const std::wstring&)> exportCall = std::bind(&HerdTab::ExportInDocs, this, std::placeholders::_1, std::placeholders::_2);
	OwnDocsListHeader docsHeader;
	Export(exportCall, docsHeader, L"INDOC_EXCELEXPORT");
}

void HerdTab::ExportMoveDocs(ExcelExport* exporter, const std::wstring& xlsFile)
{
	SeasonSession* session = GetSeasonSession();

	std::vector<IMoveDoc*> moveDocs;
	ListContent* content = m_MoveDocsTab.GetContent();
	ListSelection selection = m_MoveDocsTab.GetSelection();

	if (selection.GetCount() > 0)
	{

		BOOST_FOREACH(int row, selection.GetRange())
		{
			MoveDocView* rowContent = ((MoveDocRow*)content->GetRow(row))->Content();
			MoveDocClientObject* moveDocCO = session->GetMoveDoc(rowContent->GetDocId());
			if (moveDocCO != NULL)
			{
				moveDocs.push_back(moveDocCO->Object());
			}
		}
		exporter->ExportDocs(xlsFile, moveDocs);
	}
}

void HerdTab::OnExportMoveDocs()
{
	std::function<void(ExcelExport*, const std::wstring&)> exportCall = std::bind(&HerdTab::ExportMoveDocs, this, std::placeholders::_1, std::placeholders::_2);
	MoveDocsListHeader docsHeader;
	Export(exportCall, docsHeader, L"MOVEDOC_EXCELEXPORT");
}

void HerdTab::ExportSellDocs(ExcelExport* exporter, const std::wstring& xlsFile)
{
	SeasonSession* session = GetSeasonSession();

	std::vector<ISellDoc*> sellDocs;
	ListContent* content = m_SellDocsTab.GetContent();
	ListSelection selection = m_SellDocsTab.GetSelection();

	if (selection.GetCount() > 0)
	{

		BOOST_FOREACH(int row, selection.GetRange())
		{
			HentsDocView* rowContent = ((HentsDocRow*)content->GetRow(row))->Content();
			SellDocClientObject* sellDocCO = session->GetSellDoc(rowContent->GetDocId());
			if (sellDocCO != NULL)
			{
				sellDocs.push_back(sellDocCO->Object());
			}
		}
		exporter->ExportDocs(xlsFile, sellDocs);
	}
}

void HerdTab::OnExportSellDocs()
{
	std::function<void(ExcelExport*, const std::wstring&)> exportCall = std::bind(&HerdTab::ExportSellDocs, this, std::placeholders::_1, std::placeholders::_2);
	HentsDocsListHeader docsHeader;
	Export(exportCall, docsHeader, L"SELLDOC_EXCELEXPORT");
}