#include "stdafx.h"
#include "Collector.h"


Collector::Collector(uint32_t id) : m_id(id), m_status(CollectorDeviceStatus_None)
{
	
}

Collector::Collector(const ICollector* src) : m_id(src->GetId())
{
	CopyFrom(src);
}

Collector::Collector(const Collector& src) : m_id(src.GetId())
{
	CopyFrom(&src);
}

void Collector::CopyFrom(const ICollector* src)
{
	m_deviceName = src->GetDeviceName();
	m_OS = src->GetOS();
	m_ClientVersion = src->GetClientVersion();
	m_endpoint = src->GetEndpoint();
	m_status = src->GetStatus();
}

Collector& Collector::operator=(const Collector& right) 
{
	m_id = right.GetId();
	CopyFrom(&right);
	return *this;
}

void Collector::SetEndpoint(const boost::asio::ip::tcp::endpoint& endpoint)
{
	m_endpoint = endpoint;
}

const boost::asio::ip::tcp::endpoint& Collector::GetEndpoint() const
{
	return m_endpoint;
}

uint32_t Collector::GetId() const
{
	return m_id;
}

std::wstring Collector::GetDeviceName() const
{
	return m_deviceName;
}

std::wstring Collector::GetOS() const
{
	return m_OS;
}

std::wstring Collector::GetClientVersion() const
{
	return m_ClientVersion;
}
	
void Collector::SetDeviceName(const std::wstring& deviceName)
{
	m_deviceName = deviceName;
}

void Collector::SetOS(const std::wstring& os)
{
	m_OS = os;
}

void Collector::SetClientVersion(const std::wstring& clientVersion)
{
	m_ClientVersion = clientVersion;
}

void Collector::SetStatus(CollectorDeviceStatus status)
{
	m_status = status;
}


CollectorDeviceStatus Collector::GetStatus() const
{
	return m_status;
}
