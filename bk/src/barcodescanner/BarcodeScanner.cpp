#include "stdafx.h"
#include "BarcodeScanner.h"
#include <dbt.h>
#include <setupapi.h>
#include "Opos.h"

#define ID_SCANNER 1001
#define CLASSNAME L"Barcode_Sink"

static const GUID GUID_DEVINTERFACE_USB_DEVICE = { 0xA5DCBF10, 0x6530, 0x11D2, { 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED } };
static const GUID GUID_DEVINTERFACE_HID = { 0x4D1E55B2, 0xF16F, 0x11CF, { 0x88, 0xCB, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30 } };
static const GUID GUID_SYMBOL_BARCODE_SCANNER = { 0x745A17A0, 0x74D3, 0x11D0, { 0xB6, 0xFE, 0x00, 0xA0, 0xC9, 0x0F, 0x57, 0xDA } };


#define WM_INVOKELATER (WM_APP+1)

typedef struct tagInvokeMsg
		{
			std::function<void ()> fn;
		} 
		InvokeMsg;

BEGIN_EVENTSINK_MAP(BarcodeScanner, CWnd)
  
	ON_EVENT(BarcodeScanner, ID_SCANNER, 1 /* DataEvent */, OnDataEventScanner, VTS_I4)
	
END_EVENTSINK_MAP()

BEGIN_MESSAGE_MAP(BarcodeScanner, CWnd)
	ON_MESSAGE(WM_DEVICECHANGE, OnUSBDeviceChange)
	ON_MESSAGE(WM_INVOKELATER, OnInvokeLater)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


BarcodeScanner::BarcodeScanner() : m_bOnline(false), m_pObserver(NULL)
{
	m_bScannerPluggedIn = IsUSBScannerPluggedIn();
}

bool BarcodeScanner::RegisterWndClass()
{
	
	WNDCLASS wc;
	wc.lpszClassName = CLASSNAME; 
	wc.hInstance = AfxGetInstanceHandle();
	wc.lpfnWndProc = ::DefWindowProc;
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = 0;
	wc.lpszMenuName = NULL;
	wc.hbrBackground = (HBRUSH) ::GetStockObject(COLOR_WINDOW);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_SAVEBITS;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;

	return (::RegisterClass(&wc) != 0);
}

bool BarcodeScanner::Create(BarcodeScannerObserver *pObserver)
{
	RegisterWndClass();

	if (!CWnd::Create(CLASSNAME, L"", WS_OVERLAPPED, CRect(), CWnd::GetDesktopWindow(), 0, NULL))
	{
//		LOG("FAILD TO CREATE BARCODE SINK WINDOW");
		return FALSE;
	}

	DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
	ZeroMemory( &NotificationFilter, sizeof(NotificationFilter) );
	NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
	NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;

	NotificationFilter.dbcc_classguid = GUID_DEVINTERFACE_USB_DEVICE;
	m_hDevNotify = RegisterDeviceNotification(this->GetSafeHwnd(),&NotificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE);
	if( !m_hDevNotify ) 
	{
//		LOG("FAILD TO REGISTER DEVICE NOTIFICATION.");
		DestroyWindow();
		return FALSE;
	}

	m_pObserver = pObserver;
	if(m_scanner.Create(L"",L"SCANNER",0,CRect(),this,ID_SCANNER) &&
		m_scanner.Open(L"STI_USBSCANNER") == 0)
	{
		return true;
	}
//	LOG("UNABLE TO SETUP OLE SCANNER.");
	DestroyWindow();
	return false;
}

void BarcodeScanner::OnDestroy()
{
	if(m_scanner.GetControlUnknown() != NULL)
	{
		m_scanner.Close();
	}
	if(m_hDevNotify) UnregisterDeviceNotification(m_hDevNotify);
}

bool BarcodeScanner::IsOnline()
{
	return m_bOnline;
}

void BarcodeScanner::Connect()
{
	if(m_scanner.GetControlUnknown() != NULL)
	{
		long lClaimResult = m_scanner.ClaimDevice(0);
		
		m_bOnline = lClaimResult == OPOS_SUCCESS;
		if(m_bOnline)
		{
			m_scanner.SetDeviceEnabled(TRUE);
			m_scanner.SetDataEventEnabled(TRUE);
			m_scanner.SetDecodeData(1);
			m_scanner.SetFreezeEvents(FALSE);
			if (m_pObserver)
				m_pObserver->OnScannerConnected();
		}		
	}	
}
void BarcodeScanner::Disconnect()
{
	if(m_bOnline)
	{
		m_bOnline = false;
		//m_scanner.SetDataEventEnabled(FALSE);
		m_scanner.SetDeviceEnabled(FALSE);
		//m_scanner.CloseDevice();
		m_scanner.ReleaseDevice();
		//m_scanner.Close();
		//m_scanner.Release();
		if (m_pObserver)
			m_pObserver->OnScannerDisconnected();
	}
	
}

void BarcodeScanner::OnDataEventScanner(long lStatus)
{
	if (m_pObserver)
	{
		m_pObserver->OnBarcode((LPCTSTR)m_scanner.GetScanDataLabel());
	}

	m_scanner.SetDataEventEnabled(TRUE);
}

void BarcodeScanner::OnUSBScannerPluggedIn()
{	
	Connect();
}
void BarcodeScanner::OnUSBScannerPluggedOut()
{
	Disconnect();
}


bool BarcodeScanner::IsUSBScannerPluggedIn()
{
	DWORD dwFlag = (DIGCF_PRESENT);
	HDEVINFO hDevInfo = SetupDiGetClassDevs(&GUID_SYMBOL_BARCODE_SCANNER,L"USB",NULL,dwFlag);
	SP_DEVINFO_DATA spDevInfoData;
	spDevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	for(int i=0; SetupDiEnumDeviceInfo(hDevInfo, i, &spDevInfoData); i++) 
	{
		DWORD nSize=0 ;
		TCHAR buf[MAX_PATH];

		if (SetupDiGetDeviceInstanceId(hDevInfo, &spDevInfoData, buf, sizeof(buf), &nSize) ) 
		{
			if(CompareGUID(&spDevInfoData.ClassGuid,&GUID_SYMBOL_BARCODE_SCANNER))
			{
				return true;
			}
		} 

	}
	return false;

}


bool BarcodeScanner::CompareGUID(const GUID *pGUID1,const GUID *pGUID2)
{
	if(pGUID1->Data1 == pGUID2->Data1 &&
		pGUID1->Data2 == pGUID2->Data2 &&
		pGUID1->Data3 == pGUID2->Data3)
	{
		for(int i = 0;i < sizeof(pGUID1->Data4);i++)
		{
			if(pGUID1->Data4[i] != pGUID2->Data4[i])
				return false;
		}
		return true;
	}
	return false;
}

void BarcodeScanner::InvokeLater(std::function<void ()>& fn)
{
	InvokeMsg* msg = new InvokeMsg();
	msg->fn = fn;
	this->PostMessage(WM_INVOKELATER,0,(LPARAM)msg);
}

LRESULT BarcodeScanner::OnInvokeLater(WPARAM wParam, LPARAM lParam)
{
	InvokeMsg* msg = (InvokeMsg*)lParam;
	msg->fn();
	delete msg;
	return (LRESULT)0;
}

LRESULT BarcodeScanner::OnUSBDeviceChange(WPARAM wParam, LPARAM lParam)
{
    PDEV_BROADCAST_HDR pHdr = (PDEV_BROADCAST_HDR)lParam;

	switch(wParam)
	{
		case DBT_DEVICEARRIVAL:
			if(pHdr->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
			{
				if(IsUSBScannerPluggedIn() && !m_bScannerPluggedIn)
				{
					m_bScannerPluggedIn = true;
					InvokeLater(std::bind(&BarcodeScanner::OnUSBScannerPluggedIn,this));
					//OnUSBScannerPluggedIn();					
				}
			}
			break;
		case DBT_DEVICEREMOVECOMPLETE:
			if(pHdr->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
			{
				if(!IsUSBScannerPluggedIn() && m_bScannerPluggedIn)
				{
					m_bScannerPluggedIn = false;
					InvokeLater(std::bind(&BarcodeScanner::OnUSBScannerPluggedOut, this));
					//OnUSBScannerPluggedOut();					
				}
			}
			break;
	}
	
    return (LRESULT)TRUE;
}

