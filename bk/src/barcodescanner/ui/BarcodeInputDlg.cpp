#include "stdafx.h"
#include "BarcodeInputDlg.h"
#include "../../AppServices.h"
#include "../../utils/StringLoader.h"
#include "../../resource.h"

#define DLG_FONT_SIZE 20
#define DLG_FONT_SIZE_STATUS 15
#define DLG_DELAY_BEFORE_CLOSE_MS 500
#define DLG_BC_BG_COLOR RGB(255,255,255)

IMPLEMENT_DYNAMIC(CBarcodeInputDlg, CDialog)

CBarcodeInputDlg::CBarcodeInputDlg(CWnd* pParent /*=NULL*/) : CDialog(CBarcodeInputDlg::IDD, pParent)
{
}

CBarcodeInputDlg::~CBarcodeInputDlg()
{
}

void CBarcodeInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_ENTRYTEXT,m_entryText);
	DDX_Control(pDX,IDC_BARCODE,m_barcodeValue);
	DDX_Control(pDX,IDC_STATUS,m_Status);
}


BEGIN_MESSAGE_MAP(CBarcodeInputDlg, CDialog)
	ON_WM_DESTROY()	
END_MESSAGE_MAP()


BOOL CBarcodeInputDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	BarcodeScannerService* bcService = AppServices::GetBCScannerService();
	bcService->AddListener(this);

	m_entryText.SetFont((LPCTSTR)NULL,DLG_FONT_SIZE);
	m_barcodeValue.SetBackgroundColor(DLG_BC_BG_COLOR);
	m_barcodeValue.SetFont((LPCTSTR)NULL,DLG_FONT_SIZE);
	
	m_entryText.SetWindowText(GetEntryText().c_str());
	m_Status.SetFont(NULL,DLG_FONT_SIZE_STATUS,FALSE);
	m_Status.SetTextColor(RGB(255, 0, 0), FALSE);

	ShowBarcodePrompt();

	if (bcService->GetConnectionStatus() != ScannerConnectionStatus_Connected)
	{
		OnScannerDisconnected();
	}
	else
	{
		ClearStatusText();
	}
	
	return TRUE;
}

void CBarcodeInputDlg::ShowBarcodePrompt()
{
	m_barcodeValue.SetWindowText(L"______________");
}

void CBarcodeInputDlg::SetStatusText(const std::wstring& status)
{
	m_Status.SetWindowText(status.c_str());
	m_Status.Invalidate();
}

void CBarcodeInputDlg::ClearStatusText()
{
	m_Status.SetWindowText(L"");
	m_Status.Invalidate();
}

void CBarcodeInputDlg::OnDestroy()
{	
	BarcodeScannerService* bcService = AppServices::GetBCScannerService();
	bcService->RemoveListener(this);
	CDialog::OnDestroy();
}

void CBarcodeInputDlg::OnBarcode(const std::wstring& sBarcode)
{
	m_barcodeValue.SetWindowText(sBarcode.c_str());
	m_barcodeValue.Invalidate();

	try
	{
		ReceiveInput(sBarcode);
		EndDialog(IDOK);
	}
	catch (BarcodeValidationException& e)
	{		
		SetStatusText(e.wwhat());
	}	
}


void CBarcodeInputDlg::OnScannerConnected()
{
	ClearStatusText();
	ShowBarcodePrompt();
}

void CBarcodeInputDlg::OnScannerDisconnected()
{
	SetStatusText(StringLoader(IDS_BCSCANNERNOTCONNECTED));
}
