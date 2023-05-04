#ifndef __ICOLLECTOR_H__
#define __ICOLLECTOR_H__

#include <string>
#include <cstdint>
#include <boost/asio.hpp>
#include "CollectorDeviceStatus.h"

class ICollector
{
public:
	virtual uint32_t GetId() const = 0;
	virtual std::wstring GetDeviceName() const = 0;
	virtual std::wstring GetOS() const = 0;
	virtual std::wstring GetClientVersion() const = 0;
	virtual const boost::asio::ip::tcp::endpoint& GetEndpoint() const = 0;
	virtual CollectorDeviceStatus GetStatus() const = 0;
};

#endif
