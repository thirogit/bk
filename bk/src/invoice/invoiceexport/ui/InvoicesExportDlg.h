#ifndef __INVOICEEXPORTDLG_H__
#define __INVOICEEXPORTDLG_H__

#include <vector>
#include "../../../resource.h"
#include "../../../ui/lazydlg/LazyDlg.h"
#include "../../../SessionAwareDialog.h"
#include "../../../ctrls/contentlistctrl/ContentListCtrl.h"
#include "../../../ctrls/BaseCombo.h"
#include "../../../ctrls/daterange/DateRangeCtrl.h"
#include "../../../ctrls/paywaycombo/PayWayCombo.h"
#include "../../../ctrls/pngbutton/PngButton.h"
#include "../../../runtime/UniqueIds.h"
#include "../exporters/InvoiceExporter.h"
#include "../../../content/SimpleInvoicesListContent.h"
#include "../InvoiceInterface.h"
#include "../../../print/DocumentArchetype.h"

class CInvoicesExportDlg : public CSessionAwareDialog
{	
public:
	enum { IDD = IDD_INVOICEEXPORT };

	CInvoicesExportDlg(InvoiceInterface* interfacee,CWnd* pParent = NULL);
	virtual ~CInvoicesExportDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();

	void InitExportersCombo();
	void UpdateTotals();
	void UpdateBtns();
	void OnPrintInvoices(const DocumentArchetype& archetype);

	CPngButton m_ClearPayWayBtn;
	CComboBox m_ExportersCb;
	CDateRangeCtrl m_DateRange;
	CPaywayCombo m_PayWayCb;
	int m_FoundCount;
	CContentListCtrl m_List;
	std::vector<InvoiceExporter*> m_Exporters;
	InvoiceInterface* m_Interface;
	IdableIndex<IInvoice> m_Invoices;
	SimpleInvoicesListContent* m_Content;

	afx_msg void OnDestory();	
	afx_msg void OnDeleteInvoice();
	afx_msg void OnClearPayWay();
	afx_msg void OnFindInvoices();
	afx_msg void OnExporterChange();
	afx_msg void OnExport();
	afx_msg void OnPrint();
	
	DECLARE_MESSAGE_MAP()
};

#endif
