#include "stdafx.h"
#include "AllCowsTab.h"

namespace ph = std::placeholders;

AllCowsTab::AllCowsTab() : m_herdId(NULL_ID)
{
}


BEGIN_MESSAGE_MAP(AllCowsTab, CowsTab)
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


int AllCowsTab::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CowsTab::OnCreate(lpCreateStruct) != 0)
		return -1;
	
	SeasonSession* session = GetSeasonSession();

	m_AllCows.Create(session, m_herdId);
	SetContent(&m_AllCows);
	SetOnDblClick(std::bind(&AllCowsTab::OnEditCow, this));
	SetOnRightClick(std::bind(&AllCowsTab::OnRClick, this, ph::_1, ph::_2));


	return 0;

}

void AllCowsTab::EditCow(uint32_t cowId)
{
	SeasonSession* session = GetSeasonSession();
	Try()
		.Prepare([session, cowId]
	{
		session->LockCow(cowId);
	})
		.Do([session, cowId, this]
	{
		LonlyCowEditor editor(session);
		editor.Edit(cowId);
		CowDlg cowDlg(&editor, this);
		cowDlg.DoModal();
	})
		.TearDown([session, cowId]
	{
		session->UnlockCow(cowId);
	})
		.Catch([](wexception& e)
	{
		ExceptionMsgBox().Show(e);
	})
		.Run();
}

void AllCowsTab::OnEditCow()
{
	int row = GetSelectedRow();
	CowPresenceRow* pRow = m_AllCows.GetCowAtRow(row);
	CowPresenceView* pView = pRow->Content();
	EditCow(pView->GetKey().GetCowId());
}


void AllCowsTab::OnRClick(int row, int cow)
{
	MenuBuilder menu;
	DocumentSelection* pDocuments = NULL;

	if (row >= 0)
	{
		menu.add(StringLoader(IDS_EDIT), std::bind(&HerdTab::OnAllCowsEditCow, this));

		menu.addSeparator();

		MenuBuilder printMenu;
		DocumentManager* manager = AppServices::GetDocumentManager();

		pDocuments = new DocumentSelection(manager->FindDocuments(DocumentInput_Cow));

		PtrEnumerator<DocumentArchetype> archetypeEnum = pDocuments->Enum();

		DocumentArchetype* archetype;
		while (archetypeEnum.hasNext())
		{
			archetype = *archetypeEnum;
			printMenu.add(archetype->GetName(), std::bind(&HerdTab::OnPrintAllCows, this, *archetype));
			archetypeEnum.advance();
		}

		menu.addSubmenu(StringLoader(IDS_PRINT), printMenu);

		menu.addSeparator();
	}

	menu.add(StringLoader(IDS_NEWINDOC), std::bind(&HerdTab::OnNewInDoc, this));
	menu.add(StringLoader(IDS_NEWBUYDOC), std::bind(&HerdTab::OnNewBuyDoc, this));
	menu.add(StringLoader(IDS_NEWOUTDOC), std::bind(&HerdTab::OnNewOutDoc, this));
	menu.add(StringLoader(IDS_NEWSELLDOC), std::bind(&HerdTab::OnNewSellDoc, this));

	menu.addSeparator();
	menu.add(StringLoader(IDS_SELECTALL), std::bind(&ListTab::SelectAll, &m_AllCowsTab));
	menu.add(StringLoader(IDS_INVERTSELECTION), std::bind(&ListTab::InvertSelection, &m_AllCowsTab));

	menu.track(this);
	delete pDocuments;
}

void AllCowsTab::OnPrintCows(const DocumentArchetype& archetype)
{
	ListSelection selection = m_AllCowsTab.GetSelection();
	SeasonSession* session = GetSeasonSession();

	if (selection.GetCount() > 0)
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


		DocumentTerritoryObj territory(session->GetFarm(), m_pHerd);
		DocumentContext* context = DocumentContextBuilder().Create(&territory);
		DocumentInstance* instance = archetype.CreateDocument(session, context, cowKeys);
		DocumentPrintDlg printDlg(instance, this);
		printDlg.DoModal();
		delete instance;
		delete context;
		DeepDelete(cows);
	}
}
