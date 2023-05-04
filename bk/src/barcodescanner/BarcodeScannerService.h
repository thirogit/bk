#ifndef __BARCODESCANNERSERVICE__
#define __BARCODESCANNERSERVICE__

#include <string>
#include <set>
#include "BarcodeScannerObserver.h"
#include <functional>
//#include <boost/asio.hpp>
//#include <boost/thread.hpp>
#include "ScannerEventListener.h"
#include "BarcodeScanner.h"
//#include "../context/UIBridge.h"
#include "ScannerConnectionStatus.h"
#include "../services/AppService.h"

class BarcodeScannerService : private BarcodeScannerObserver, public AppService
{
public:

	BarcodeScannerService();

	virtual void Start();
	virtual void Stop();
	virtual void Init(ServiceRegistry* registry);

	void AddListener(ScannerEventListener *pListener);
	void RemoveListener(ScannerEventListener *pListener);

	ScannerConnectionStatus GetConnectionStatus();

protected:
	void DoAsync(std::function<void()>& f);

	virtual void OnBarcode(const std::wstring& sBarcode);
	virtual void OnScannerConnected();
	virtual void OnScannerConnecing();
	virtual void OnScannerConnectionFailed();
	virtual void OnScannerDisconnected();

	void HandleScannerConnected();
	void HandleScannerConnecing();
	void HandleScannerConnectionFailed();
	void HandleScannerDisconnected();

	void RequestConnect();

	std::set<ScannerEventListener*> m_listeners;
	BarcodeScanner m_scanner;
	ScannerConnectionStatus m_connectionStatus;

	//UIBridge m_bridge;
	//boost::thread m_worker;
	//boost::asio::io_service m_service;
	//boost::asio::io_service::work m_work;
	
};

#endif