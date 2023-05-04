#ifndef __COLLECTOR_H__
#define __COLLECTOR_H__

#include "ICollector.h"


class Collector : public ICollector
{
public:
	explicit Collector(uint32_t id);
	Collector(const ICollector* src);
	Collector(const Collector& src);

	void CopyFrom(const ICollector* src);
	Collector& operator=(const Collector& right);

	virtual uint32_t GetId() const;
	virtual std::wstring GetDeviceName() const;
	virtual std::wstring GetOS() const;
	virtual std::wstring GetClientVersion() const;
	virtual const boost::asio::ip::tcp::endpoint& GetEndpoint() const;
	virtual CollectorDeviceStatus GetStatus() const;
	
	
	void SetDeviceName(const std::wstring& deviceName);
	void SetOS(const std::wstring& os);
	void SetClientVersion(const std::wstring& clientVersion);
	void SetEndpoint(const boost::asio::ip::tcp::endpoint& endpoint);
	void SetStatus(CollectorDeviceStatus status);
	
private:
	uint32_t m_id;
	std::wstring m_deviceName;
	std::wstring m_OS;
	std::wstring m_ClientVersion;
	boost::asio::ip::tcp::endpoint m_endpoint;
	CollectorDeviceStatus m_status;
	
};

#endif
