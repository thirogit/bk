#include "stdafx.h"
#include "InvoicesExportDlg.h"

#include <arrays\DeepDelete.h>
#include <boost/assign.hpp>
#include <boost\foreach.hpp>
#include <string\TextUtils.h>

#include "../../../utils/StringLoader.h"
#include "../../../ui/progressdlg/SimpleProgressDialog.h"


#include <fstream>
#include <boost/locale.hpp>
#include <boost/locale/encoding.hpp>

#include "../exporters/SmallBusinessSymfonia.h"
#include "../exporters/FullAccountingSymfonia.h"
#include "../exporters/JpkExporter.h"
#include "../exporters/SubiektEDI.h"
#include "../exporters/JsonExporter.h"

#include "../../../header/InvoiceListHeader.h"
#include "../../../AppServices.h"
#include "../../../ui/menu/MenuBuilder.h"
#include "../../../print/context/DocumentContextBuilder.h"
#include "../../../print/ui/DocumentPrintDlg.h"
#include "../../../ui/ExceptionMsgBox.h"
#include "../../../print/runtime/FeedException.h"
#include "../../../print/runtime/DocumentInputUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CInvoicesExportDlg::CInvoicesExportDlg(InvoiceInterface* interfacee, CWnd* pParent /*=NULL*/)
	: CSessionAwareDialog(CInvoicesExportDlg::IDD, pParent), m_Content(NULL),m_Interface(interfacee)
	
{
	InvoiceKind kind = m_Interface->GetInvoiceKind();
	if (kind.GetDirection() == InvoiceDirection_Buy)
	{
		m_Exporters = { new SmallBusinessSymfonia(), new FullAccountingSymfonia(),  new JpkExporter(), new SubiektEDI(), new JsonExporter() };
	}
	else
	{
		m_Exporters = { new JpkExporter() };
	}
				
}


CInvoicesExportDlg::~CInvoicesExportDlg()
{	
	DeepDelete(m_Exporters);
	
}

void CInvoicesExportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX,IDC_EXPORTERSCOMBO,m_ExportersCb);
	DDX_Control(pDX, IDC_LIST, m_List);
	DDX_Control(pDX, IDC_DATERANGE, m_DateRange);
	DDX_Control(pDX, IDC_PAYWAYCB, m_PayWayCb);
	DDX_Control(pDX, IDC_CLEARPAYWAY, m_ClearPayWayBtn);
	
}


BEGIN_MESSAGE_MAP(CInvoicesExportDlg, CDialog)
	
	ON_BN_CLICKED(IDC_DELETE,OnDeleteInvoice)
	ON_BN_CLICKED(IDC_FIND,OnFindInvoices)
	ON_BN_CLICKED(IDC_PRINT,OnPrint)
	ON_BN_CLICKED(IDC_CLEARPAYWAY,OnClearPayWay)
	ON_BN_CLICKED(IDC_EXPORTINVOICES,OnExport)
	ON_CBN_SELCHANGE(IDC_EXPORTERSCOMBO, OnExporterChange)
	
	ON_WM_DESTROY()
END_MESSAGE_MAP()


BOOL CInvoicesExportDlg::OnInitDialog()
{
	CDialog::OnInitDialog();	

	InitExportersCombo();
	
	DateTime nowMonthBegin = DateTime::now().ToMonthBegin();
	DateTime prevMonthBegin = nowMonthBegin.AddMonth(-1);
	DateTime prevMonthEnd = prevMonthBegin.ToMonthEnd();

	m_DateRange.SetRange(DateRange(prevMonthBegin, prevMonthEnd));

	m_List.SetHeaderDescriptor(InvoiceListHeader());
	m_PayWayCb.ReloadCombo();
	
	m_Content = new SimpleInvoicesListContent(GetSeasonSession());
	
	m_ClearPayWayBtn.SetImage(IDPNG_TRASHSMALL);

	UpdateTotals();
	UpdateBtns();

	return TRUE;
}

void CInvoicesExportDlg::OnDestory()
{
	delete m_Content;
	
	CSessionAwareDialog::OnDestroy();

}

void CInvoicesExportDlg::OnClearPayWay()
{
	m_PayWayCb.SetCurSel(-1);
}

void CInvoicesExportDlg::OnFindInvoices()
{
	m_Content->RemoveAll();
	m_Invoices.clear();
	SeasonSession* session = GetSeasonSession();
	PayWay payway = m_PayWayCb.GetSelectedPayWay();
	DateRange dateRange = m_DateRange.GetRange();
	UniqueIds invoiceIds = std::move(m_Interface->Search(dateRange, payway));

	BOOST_FOREACH(uint32_t invoiceId, invoiceIds)
	{		
		IInvoice* invoice = m_Interface->GetInvoice(invoiceId);
		if (invoice != NULL)
		{
			m_Content->AddRow(invoice);
			m_Invoices.add(invoice);
		}
	}

	m_List.SetContent(m_Content);
	UpdateTotals();
	UpdateBtns();
}


void CInvoicesExportDlg::InitExportersCombo()
{
	int iItem = -1;
	BOOST_FOREACH(InvoiceExporter* pExporter,m_Exporters)
	{
		iItem = m_ExportersCb.AddString(pExporter->GetExporterName().c_str());
		m_ExportersCb.SetItemData(iItem,(DWORD_PTR)pExporter);
	}
}

void CInvoicesExportDlg::UpdateBtns()
{
	bool bHaveContent = m_Content->GetRowCount() > 0;
	bool bExporterIsSelected = m_ExportersCb.GetCurSel() != -1;
	GetDlgItem(IDC_PRINT)->EnableWindow(bHaveContent);
	GetDlgItem(IDC_EXPORTINVOICES)->EnableWindow(bHaveContent && bExporterIsSelected);
	GetDlgItem(IDC_DELETE)->EnableWindow(bHaveContent);

}

void CInvoicesExportDlg::UpdateTotals()
{	
	 Decimal total(0L);

	BOOST_FOREACH(InvoiceRow* row,*m_Content)
	{		
		InvoiceView* view = row->Content();
		total += view->GetTotalGrossValue();
	}

	SetDlgItemInt(IDC_FOUNDCOUNT,m_Content->GetRowCount());
	SetDlgItemText(IDC_TOTALGROSS, total.ToString(2).c_str());
}

void CInvoicesExportDlg::OnPrint()
{
	MenuBuilder printMenu;
	DocumentManager* manager = AppServices::GetDocumentManager();

	DocumentSelection selection(manager->FindDocuments(DocumentInputUtils::GetDocumentInputMaskForInvoice(m_Interface->GetInvoiceKind())));

	if (!selection.IsEmpty())
	{
		PtrEnumerator<DocumentArchetype> archetypeEnum = selection.Enum();

		DocumentArchetype* archetype;

		while (archetypeEnum.hasNext())
		{
			archetype = *archetypeEnum;
			printMenu.add(archetype->GetName(), std::bind(&CInvoicesExportDlg::OnPrintInvoices, this, *archetype));
			archetypeEnum.advance();
		}
	}

	CWnd* printBtn = GetDlgItem(IDC_PRINT);
	CRect windowRect;
	printBtn->GetWindowRect(&windowRect);
	printMenu.trackAt( windowRect.left, windowRect.top, this);

}

void CInvoicesExportDlg::OnPrintInvoices(const DocumentArchetype& archetype)
{
	SeasonSession* session = GetSeasonSession();

	if (m_Content->GetRowCount() > 0)
	{
		UniqueIds ids;
		InvoiceView* view;
		DocumentPredicate* predicate = archetype.GetPredicate();
		
		BOOST_FOREACH(InvoiceRow* row, *m_Content)
		{
			view = row->Content();

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



void CInvoicesExportDlg::OnExport()
{
	int nsel = m_ExportersCb.GetCurSel();
	if(nsel < 0)
	{
		AfxMessageBox(IDS_SELECTEXPORTER);
		m_ExportersCb.SetFocus();
		return;
	}

	SeasonSession* session = GetSeasonSession();

	
	InvoiceExporter* pExporter = (InvoiceExporter*)m_ExportersCb.GetItemData(nsel);	

	std::wstring extension(pExporter->GetFileExtension());
	std::wstring filter = TextUtils::Format(L"%s  (*.%s)|*.%s||", extension, extension, extension);
	
	CFileDialog saveFileDlg(FALSE,extension.c_str(),NULL,OFN_OVERWRITEPROMPT,filter.c_str(),this);

	if(saveFileDlg.DoModal() == IDOK)
	{	
		std::wstring outputFilePathName = (LPCTSTR)saveFileDlg.GetPathName();
		std::auto_ptr<boost::locale::util::base_converter> cvt;
		
		std::wofstream invoiceExportFile;
		
		std::string encoding = pExporter->GetDefaultEncoding();

		if (encoding.empty() || "utf8" == encoding || "utf-8" == encoding)
		{
			cvt = boost::locale::util::create_utf8_converter();
		}
		else
		{
			cvt = boost::locale::util::create_simple_converter(encoding);
		}

		std::locale encodinglocale = boost::locale::util::create_codecvt(std::locale(), cvt, boost::locale::wchar_t_facet);	

		invoiceExportFile.imbue(encodinglocale);

		CSimpleProgressDialog progressDlg(StringLoader(IDS_EXPORTINGINVOICES));		

		progressDlg.Create(this);
		invoiceExportFile.open(outputFilePathName.c_str(), std::ofstream::out | std::ofstream::binary);
		if (invoiceExportFile.is_open())
		{
			pExporter->Export(IInvoiceRange(m_Invoices),InvoiceExporterOptions(), invoiceExportFile,session,&progressDlg);

			invoiceExportFile.flush();
			invoiceExportFile.close();
			if (progressDlg.WasCanceled())
			{
				invoiceExportFile.close();
				::DeleteFile(outputFilePathName.c_str());
				return;
			}
		}
	}
}


void CInvoicesExportDlg::OnDeleteInvoice()
{
	int nSelected = m_List.GetCurSel();
	if (nSelected >= 0)
	{
		InvoiceRow* pRowToDelete = m_Content->GetInvoiceRowAt(nSelected);
		uint32_t invoiceId = pRowToDelete->Content()->GetInvoiceId();
		m_Content->RemoveRow(invoiceId);
		
		m_Invoices.remove(invoiceId);
		
		UpdateTotals();
		UpdateBtns();
	}
	
}

void CInvoicesExportDlg::OnExporterChange()
{
	UpdateBtns();
}