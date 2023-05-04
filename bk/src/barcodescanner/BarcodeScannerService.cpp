#include "stdafx.h"
#include "BarcodeScannerService.h"
#include <algorithm>
#include <functional>
#include <future>

BarcodeScannerService::BarcodeScannerService() : 
		//m_service(1), 
		//m_work(m_service), 
		m_connectionStatus(ScannerConnectionStatus_Disconnected)		
{
}

void BarcodeScannerService::Init(ServiceRegistry* registry)
{

}

void BarcodeScannerService::Start()
{
	if (!::IsWindow(m_scanner.m_hWnd))
	{
		if (m_scanner.Create(this))
		{
			//m_bridge.Create();
			//m_service.reset();
			//m_worker = boost::thread(boost::bind(&boost::asio::io_service::run, &m_service));
			RequestConnect();
		}
	}	
}

void BarcodeScannerService::Stop()
{
	if (::IsWindow(m_scanner.m_hWnd))
	{
		m_scanner.Disconnect();
		m_scanner.DestroyWindow();
		//m_bridge.Destroy();

		//m_service.stop();
		//m_worker.join();
	}
}

void BarcodeScannerService::DoAsync(std::function<void()>& f)
{
	/*typedef std::packaged_task<void()> task_t;
	std::shared_ptr< task_t > copyable_task = std::make_shared< task_t >(f);
	std::future<void> future = copyable_task->get_future();
	m_service.post(std::bind(&task_t::operator(), copyable_task));*/
}


void BarcodeScannerService::RequestConnect()
{
	//std::function<void()> f = std::bind(&BarcodeScanner::Connect, &m_scanner);
	//DoAsync(f);
	m_scanner.Connect();
}

void BarcodeScannerService::AddListener(ScannerEventListener *pListener)
{
	m_listeners.insert(pListener);
}

void BarcodeScannerService::RemoveListener(ScannerEventListener *pListener)
{
	m_listeners.erase(pListener);
}

void BarcodeScannerService::OnBarcode(const std::wstring& sBarcode)
{
	std::for_each(m_listeners.begin(), m_listeners.end(), [&sBarcode](ScannerEventListener* listener)
	{
		listener->OnBarcode(sBarcode);
	});
}

void BarcodeScannerService::OnScannerConnected()
{
	HandleScannerConnected();
	//m_bridge.InvokeAny(std::bind(&BarcodeScannerService::HandleScannerConnected, this));
}

void BarcodeScannerService::OnScannerDisconnected()
{
	HandleScannerDisconnected();
	//m_bridge.InvokeAny(std::bind(&BarcodeScannerService::HandleScannerDisconnected, this));
}

void BarcodeScannerService::OnScannerConnecing()
{
	HandleScannerConnecing();
	//m_bridge.InvokeAny(std::bind(&BarcodeScannerService::HandleScannerConnecing, this));
	
}

void BarcodeScannerService::OnScannerConnectionFailed()
{
	HandleScannerConnectionFailed();
	//m_bridge.InvokeAny(std::bind(&BarcodeScannerService::HandleScannerConnectionFailed, this));
}

void BarcodeScannerService::HandleScannerConnected()
{
	m_connectionStatus = ScannerConnectionStatus_Connected;
	std::for_each(m_listeners.begin(), m_listeners.end(), [](ScannerEventListener* listener)
	{
		listener->OnScannerConnected();
	});
}

void BarcodeScannerService::HandleScannerConnecing()
{
	m_connectionStatus = ScannerConnectionStatus_Connecting;
}

void BarcodeScannerService::HandleScannerConnectionFailed()
{
	m_connectionStatus = ScannerConnectionStatus_Disconnected;
}

void BarcodeScannerService::HandleScannerDisconnected()
{	
	m_connectionStatus = ScannerConnectionStatus_Disconnected;
	std::for_each(m_listeners.begin(), m_listeners.end(), [](ScannerEventListener* listener)
	{
		listener->OnScannerDisconnected();
	});
}

ScannerConnectionStatus BarcodeScannerService::GetConnectionStatus()
{
	return m_connectionStatus;
}