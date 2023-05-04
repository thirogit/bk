#pragma once
#include "..\TabPage.h"
#include "CowsTab.h"
#include <data/IHerd.h>
#include "../../content/InCowsListContent.h"
#include "../../content/AllCowsListContent.h"
#include "../../content\InDocsListContent.h"
#include "../../content\OutDocsListContent.h"
#include "../../content\SellDocsListContent.h"
#include "../../content\BuyDocsListContent.h"
#include "../../ctrls/xtoolbar/XToolBar.h"
#include <boost/range/any_range.hpp>
#include "../../print/DocumentArchetype.h"
#include "../../header/HeaderLayoutConfiguration.h"
#include "HerdNumbersBar.h"
#include "OwnDocsTab.h"
#include "HentsDocsTab.h"
#include "MoveDocsTab.h"
#include "../../excelexport/ExcelExport.h"

class HerdTab : public TabPage
{
	typedef boost::any_range<uint32_t,boost::forward_traversal_tag, uint32_t, std::ptrdiff_t> CowIdRange;


public:
	HerdTab(IHerd* pHerd);
	BOOL Create(UINT dwStyle, const CRect & rect, CWnd * pParentWnd, UINT nID);
protected:
	CHerdNumbersBar m_herdNumbers;
	CMFCTabCtrl m_TabNavigator;
	CowsTab m_InCowsTab;
	HeaderLayoutConfiguration* m_InCowsTabListHdrLayout;

	CowsTab m_AllCowsTab;
	HeaderLayoutConfiguration* m_AllCowsTabListHdrLayout;

	OwnDocsTab m_InDocsTab;
	HeaderLayoutConfiguration* m_InDocsTabListHdrLayout;

	OwnDocsTab m_OutDocsTab;
	HeaderLayoutConfiguration* m_OutDocsTabListHdrLayout;

	HentsDocsTab m_BuyDocsTab;
	HeaderLayoutConfiguration* m_BuyDocsTabListHdrLayout;
	
	MoveDocsTab m_MoveDocsTab;
	HeaderLayoutConfiguration* m_MoveDocsTabListHdrLayout;

	HentsDocsTab m_SellDocsTab;
	HeaderLayoutConfiguration* m_SellDocsTabListHdrLayout;
		
	OutDocsListContent m_OutDocs;
	InDocsListContent m_InDocs;
	MoveDocsListContent m_MoveDocs;
	BuyDocsListContent m_BuyDocs;
	SellDocsListContent m_SellDocs;


	InCowsListContent m_InCows;
	AllCowsListContent m_AllCows;
	IHerd* m_pHerd;
protected:
	void OnInCowsEditCow();
	void OnAllCowsEditCow();
	void OnShowFindInCows();
	void OnShowFindAllCows();
	void EditCow(uint32_t cowId);
	void OnPrintInCows(const DocumentArchetype& archetype);
	void OnPrintAllCows(const DocumentArchetype& archetype);

	void OnNewInDoc();
	void OnFastNewInDoc();
	void OnEditInDoc();
	void EditInDoc(uint32_t docId);
	void OnPrintInDoc(const DocumentArchetype& archetype);

	void OnNewBuyDoc();
	void OnEditBuyDoc();
	void EditBuyDoc(uint32_t docId);
	void OnPrintBuyDoc(const DocumentArchetype& archetype);

	void OnEditMoveDoc();
	void EditMoveDoc(uint32_t docId);
	void OnPrintMoveDoc(const DocumentArchetype& archetype);

	void OnNewSellDoc();
	void OnEditSellDoc();
	void EditSellDoc(uint32_t docId);
	void OnPrintSellDoc(const DocumentArchetype& archetype);

	void OnNewOutDoc();
	void OnEditOutDoc();
	void EditOutDoc(uint32_t docId);
	void OnPrintOutDoc(const DocumentArchetype& archetype);

	void OnInCowsRClick(int row,int cow);
	void OnAllCowsRClick(int row,int cow);
	void OnSellDocsRClick(int row,int cow);
	void OnMoveDocsRClick(int row,int cow);
	void OnBuyDocsRClick(int row,int cow);
	void OnOutDocsRClick(int row,int cow);
	void OnInDocsRClick(int row,int cow);

	void OnCreateSellDocInvoice(InvoiceType type);
	void OnCreateBuyDocInvoice(InvoiceType type);
	
	void MoveSelectedToHerd(uint32_t herdId);

	void OnCreateBuyCowsInvoice(InvoiceType type);
	void OnCreateSellCowsInvoice(InvoiceType type);
	

	void OnDeleteSellDoc();
	void OnDeleteMoveDoc();
	void OnDeleteBuyDoc();
	void OnDeleteOutDoc();
	void OnDeleteInDoc();

	void OnSellCows();

	void Export(std::function<void(ExcelExport*, const std::wstring&)>& exportCall, const ListHeaderDescriptor& headerFields, const std::wstring& settingName);


	void OnExportCows(CowsTab& tab);
	void ExportCows(ExcelExport* exporter,  const std::wstring& xlsFile, CowsTab& tab);


	void ExportSellDocCows(ExcelExport* exporter,  const std::wstring& xlsFile);
	void OnExportSellDocCows();

	void ExportInDocCows(ExcelExport* exporter,  const std::wstring& xlsFile);
	void OnExportInDocCows();

	void ExportMoveDocCows(ExcelExport* exporter,  const std::wstring& xlsFile);
	void OnExportMoveDocCows();

	void ExportBuyDocCows(ExcelExport* exporter,  const std::wstring& xlsFile);
	void OnExportBuyDocCows();

	void ExportOutDocCows(ExcelExport* exporter,  const std::wstring& xlsFile);
	void OnExportOutDocCows();

	void ExportOutDocs(ExcelExport* exporter,  const std::wstring& xlsFile);
	void OnExportOutDocs();

	void ExportBuyDocs(ExcelExport* exporter,  const std::wstring& xlsFile);
	void OnExportBuyDocs();
	
	void ExportInDocs(ExcelExport* exporter,  const std::wstring& xlsFile);
	void OnExportInDocs();

	void ExportMoveDocs(ExcelExport* exporter,  const std::wstring& xlsFile);
	void OnExportMoveDocs();

	void ExportSellDocs(ExcelExport* exporter,  const std::wstring& xlsFile);
	void OnExportSellDocs();

	boost::optional<ExcelExportSettings> ConfigureExcelExport(const ListHeaderDescriptor& headerFields, const std::wstring& settingName);

public:
	virtual ~HerdTab();
	
protected:
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize( UINT nType,  int cx,  int cy  );
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
};

