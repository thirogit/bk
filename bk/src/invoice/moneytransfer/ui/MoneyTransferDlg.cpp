#include "stdafx.h"
#include "MoneyTransferDlg.h"

#include "../formatters/ElixirFormatter.h"
#include "../formatters/KBFormatter.h"
#include "../formatters/PolbankFormatter.h"
#include "../formatters/BZWBKFormatter.h"
#include "../formatters/AliorFormatter.h"
#include "../formatters/PBSFormatter.h"

#include <arrays\DeepDelete.h>
#include <boost/assign.hpp>
#include <boost\foreach.hpp>
#include <string\TextUtils.h>

#include "../../../utils/StringLoader.h"
#include "../../../view/FarmHentViewCreator.h"
#include "../../../ui/progressdlg/SimpleProgressDialog.h"
#include "../MoneyTransferEntryFactory.h"
#include "../../../content/BankAccountsComboContent.h"


#include <fstream>
//#include <locale>
//#include <codecvt>
#include <boost/locale.hpp>
#include <boost/locale/encoding.hpp>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CMoneyTransferDlg::CMoneyTransferDlg(UniqueIds& buyInvoiceIds,CWnd* pParent /*=NULL*/)
	: CLazyDlg<CSessionAwareDialog>(CMoneyTransferDlg::IDD, pParent)
	
{
	m_Formatters = {
		new KBFormatter(),
		new ElixirFormatter(),
		new PolbankFormatter(),
		new BZWBKFormatter(),
		new AliorFormatter() };
				
		m_buyInvoiceIds = buyInvoiceIds;
}


CMoneyTransferDlg::~CMoneyTransferDlg()
{	
	DeepDelete(m_Formatters);
}

void CMoneyTransferDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX,IDC_FORMATSCOMBO,m_FormatsCb);
	DDX_Control(pDX,IDC_MONEYTRANSERLIST,m_moneyTransferList);
	DDX_Control(pDX,IDC_TRANSFERDATE,m_TransferDate);

	DDX_Control(pDX, IDC_BANKACCOUNTSCOMBO, m_BankAccounts);
	DDX_Control(pDX, IDC_ENCODINGSCOMBO, m_Encodings);
}


BEGIN_MESSAGE_MAP(CMoneyTransferDlg, CDialog)
	
	ON_BN_CLICKED(IDC_DELETEENTRY,OnDeleteMoneyTransferEntry)
	ON_CBN_SELCHANGE(IDC_FORMATSCOMBO, OnFormaterChange)
	
	ON_WM_DESTROY()
END_MESSAGE_MAP()


BOOL CMoneyTransferDlg::OnInitDialog()
{
	CDialog::OnInitDialog();	

	InitFormatsCombo();	
	Update();

	ListHeaderDescriptor header;
	header(MoneyTransferViewMember::MoneyTransferViewMember_InvoiceNo, StringLoader(IDS_HEADER_INVOICENO))
		(MoneyTransferViewMember::MoneyTransferViewMember_HentAlias, StringLoader(IDS_HEADER_INVOICEHENT))
		(MoneyTransferViewMember::MoneyTransferViewMember_CustomInvoiceNo, StringLoader(IDS_HEADER_CUSTOMNUMBER))
		(MoneyTransferViewMember::MoneyTransferViewMember_TransferValue, StringLoader(IDS_HEADER_TRANSFERVALUE))
		(MoneyTransferViewMember::MoneyTransferViewMember_StatusDesc, StringLoader(IDS_HEADER_ERROR));

	m_moneyTransferList.SetHeaderDescriptor(header);
	m_TransferDate.SetDate(DateTime::now());

	BankAccountsComboContent bankAccountContent;
	bankAccountContent.CreateContent(GetSeasonSession());
	m_BankAccounts.SetContent(bankAccountContent);
	
	m_Encodings.InitCombo("");

	return TRUE;
}


void CMoneyTransferDlg::OnDestory()
{
	m_buyInvoiceIds.clear();
	BOOST_FOREACH(MoneyTransferEntry* entry, m_Entries)
	{
		m_buyInvoiceIds.add(entry->GetInvoiceId());
	}

	DeepDelete(m_Entries);
	m_Content.Clear();
	CLazyDlg<CSessionAwareDialog>::OnDestroy();

}

void CMoneyTransferDlg::OnAfterDlgShow()
{
	MoneyTransferEntryFactory factory(GetSeasonSession());
	MoneyTransferEntry* entry;
	BOOST_FOREACH(uint32_t invoiceId, m_buyInvoiceIds)
	{
		entry = factory.CreateEntryForInvoice(invoiceId);
		m_Entries.push_back(entry);
		m_Content.AddRow(entry);
	}
		
	m_moneyTransferList.SetContent(&m_Content);
	Update();	
}


void CMoneyTransferDlg::InitFormatsCombo()
{
	int iItem = -1;
	BOOST_FOREACH(Formatter* pFormatter,m_Formatters)
	{
		iItem = m_FormatsCb.AddString(pFormatter->GetFormatterName().c_str());
		m_FormatsCb.SetItemData(iItem,(DWORD_PTR)pFormatter);
	}
}

void CMoneyTransferDlg::Update()
{
	CheckIfAllOK();
	SetTotal();
}

void CMoneyTransferDlg::CheckIfAllOK()
{
	BOOST_FOREACH(MoneyTransferEntry* entry,m_Entries)
	{		
		if(entry->GetStatus() != MoneyTransferEntryStatus_Valid)
		{
			GetDlgItem(IDOK)->EnableWindow(FALSE);
			return;
		}		
	}

	GetDlgItem(IDOK)->EnableWindow(m_Entries.size() > 0);
}

void CMoneyTransferDlg::SetTotal()
{
	 Decimal total(0L);

	BOOST_FOREACH(MoneyTransferEntry* entry,m_Entries)
	{		
		total += entry->GetTransferValue();			
	}

	SetDlgItemText(IDC_TOTAL,total.ToString(2).c_str());
}

void CMoneyTransferDlg::OnOK()
{
	int nsel = m_FormatsCb.GetCurSel();
	if(nsel < 0)
	{
		AfxMessageBox(IDS_SELECTMONEYTRANSFERFORMAT);
		m_FormatsCb.SetFocus();
		return;
	}

	

	if(!m_TransferDate.IsInputEmpty() && !m_TransferDate.IsDateValid())
	{
		AfxMessageBox(IDS_ERRBADDATE,MB_OK,0);
		m_TransferDate.SetFocus();
		return;
	}

	std::string encoding = m_Encodings.GetEncoding();
		
	if (m_BankAccounts.GetCurSel() < 0)
	{
		AfxMessageBox(IDS_SELECTSRCBANKACCOUNT);
		m_BankAccounts.SetFocus();
		return;
	}

	SeasonSession* session = GetSeasonSession();

	uint32_t srcAccountId = m_BankAccounts.GetSelectedItemId();
	IBankAccount* srcAccount = session->GetBankAccount(srcAccountId);
		
	FarmHentViewCreator m_farmViewCreator(session);
	
	Formatter* pFormatter = (Formatter*)m_FormatsCb.GetItemData(nsel);
	
	if (encoding.empty())
	{
		encoding = pFormatter->GetDefaultEncoding();
	}

	std::wstring extension(pFormatter->GetFileExtension());
	std::wstring filter = TextUtils::Format(L"%s  (*.%s)|*.%s||", extension, extension, extension);
	
	CFileDialog saveFileDlg(FALSE,extension.c_str(),NULL,OFN_OVERWRITEPROMPT,filter.c_str(),this);

	if(saveFileDlg.DoModal() == IDOK)
	{	
		std::wstring outputFilePathName = (LPCTSTR)saveFileDlg.GetPathName();
		
		CompanyView companyView;
		companyView.CopyFrom(session->GetFarm());

		MoneyTransferContext context;

		context.SetCompany(companyView);

		DateTime transferDate = m_TransferDate.GetDate();

		if(transferDate.IsNull())
			transferDate = DateTime::now();

		context.SetTransferDate(transferDate);
		context.SetSrcAccountNo(srcAccount->GetAccountNo());

		std::wofstream moneyTransferFile;			
		std::auto_ptr<boost::locale::util::base_converter> cvt;

		if ("utf8" == encoding)
		{
			cvt = boost::locale::util::create_utf8_converter();
		}
		else
		{
			cvt = boost::locale::util::create_simple_converter(encoding);
		}

		std::locale encodinglocale = boost::locale::util::create_codecvt(std::locale(), cvt, boost::locale::wchar_t_facet);	
		moneyTransferFile.imbue(encodinglocale);

		CSimpleProgressDialog progressDlg(StringLoader(IDS_CREATINGMONEYTRANSFERFILE));		

		progressDlg.Create(this);
		moneyTransferFile.open(outputFilePathName.c_str(), std::ofstream::out | std::ofstream::binary);
		if (moneyTransferFile.is_open())
		{
			pFormatter->FormatTransfers(m_Entries,&context,moneyTransferFile,&progressDlg);

			moneyTransferFile.flush();
			moneyTransferFile.close();
			if(progressDlg.WasCanceled())
			{
				moneyTransferFile.close();
				::DeleteFile(outputFilePathName.c_str());
				return;
			}

			if(AfxMessageBox(IDS_ASKMARKPAID,MB_YESNO) == IDYES)
			{
				EndDialog(IDCONTINUE);
				return;
			}
			EndDialog(IDOK);
		}
	}	
}

void CMoneyTransferDlg::OnCancel()
{
	EndDialog(IDCANCEL);
}


void CMoneyTransferDlg::OnDeleteMoneyTransferEntry()
{
	int nSelected = m_moneyTransferList.GetCurSel();
	if (nSelected >= 0)
	{
		MoneyTransferRow* pRowToDelete = m_Content.GetMoneyTransferRow(nSelected);
		uint32_t invoiceId = pRowToDelete->Content()->GetInvoiceId();
		m_Content.RemoveRow(invoiceId);
		
		boost::multi_index::index<EntryIndexType, tag_InoiceId>::type& indexById = m_Entries.get<tag_InoiceId>();
		indexById.erase(invoiceId);
		Update();
	}
	
}

UniqueIds CMoneyTransferDlg::GetInvoiceIds() const
{
	return std::move(m_buyInvoiceIds);
}

void CMoneyTransferDlg::OnFormaterChange()
{
	Formatter* pFormatter = (Formatter*)m_FormatsCb.GetItemData(m_FormatsCb.GetCurSel());
	std::string encoding = pFormatter->GetDefaultEncoding();

	if (!encoding.empty())
	{
		m_Encodings.SelectEncoding(encoding);
	}
}