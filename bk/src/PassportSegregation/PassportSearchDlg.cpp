#include "stdafx.h"
#include "PassportSearchDlg.h"
#include <boost/foreach.hpp>
#include "../AppServices.h"
#include "../utils/StringLoader.h"

BEGIN_MESSAGE_MAP(CPassportSearchDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()

CPassportSearchDlg::CPassportSearchDlg(const PassportCollection* pCollection,CWnd* pParent/* = NULL*/) : CDialog(IDD,pParent)
{
	BOOST_FOREACH(CowPassport passport,pCollection->Enum())
	{
		m_bucket.add(passport);
	}
}

CPassportSearchDlg::~CPassportSearchDlg()
{
}

void CPassportSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	DDX_Control(pDX,IDC_SEARCHSUMMARY,m_Summary);
	DDX_Control(pDX,IDC_BARCODE,m_Barcode);
	DDX_Control(pDX,IDC_SEARCHSTATUS,m_SearchStatus);
	DDX_Control(pDX,IDC_SEARCHBUCKET,m_bucketList);
}


BOOL CPassportSearchDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_bucketList.SetBucket(&m_bucket);
	m_Summary.UpdateFromBucket(m_bucket);

	m_SearchStatus.SetFont((LPCTSTR)NULL,-20);
	m_SearchStatus.SetWindowText(StringLoader(IDS_WAITINGFORBARCODE).c_str());
	m_SearchStatus.SetBackgroundColor(RGB(255,255,255),TRUE);


	m_Barcode.SetFont((LPCTSTR)NULL,-20);

	AppServices::GetBCScannerService()->AddListener(this);

	return TRUE;
}

void CPassportSearchDlg::OnDestroy()
{
	AppServices::GetBCScannerService()->RemoveListener(this);
	CDialog::OnDestroy();
}

void CPassportSearchDlg::OnBarcode(const std::wstring& sBarcode)
{
	m_Barcode.SetWindowText(sBarcode.c_str());

	try
	{
		CowNo cowNo = CowNoFactory::FromString(sBarcode);
		BucketEntry* entry = m_bucket.find(cowNo);
		if(entry)
		{
			entry->SetFound(true);

			m_SearchStatus.SetWindowText(StringLoader(IDS_FOUND).c_str());
			m_SearchStatus.SetBackgroundColor(RGB(0,255,0),TRUE);

			m_Summary.UpdateFromBucket(m_bucket);
		}
		else
		{
			m_SearchStatus.SetWindowText(StringLoader(IDS_NOTFOUND).c_str());
			m_SearchStatus.SetBackgroundColor(RGB(255,0,0),TRUE);
		}

	}
	catch(...)
	{
		m_SearchStatus.SetWindowText(StringLoader(IDS_BADCOWNOBARCODE).c_str());
		m_SearchStatus.SetBackgroundColor(RGB(205,201,14),TRUE);
	}
}


void CPassportSearchDlg::OnScannerConnected()
{
}

void CPassportSearchDlg::OnScannerDisconnected()
{
}