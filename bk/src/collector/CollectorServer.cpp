#include "stdafx.h"
#include "CollectorServer.h"
#include <boost\range\algorithm\copy.hpp>
#include <string\TextUtils.h>
#include <future>
#include <boost\range\adaptors.hpp>
#include <utils\SafeGet.h>
#include "../text/DocNoText.h"
#include <arrays\DeepDelete.h>
#include <codecvt>
#include "ConnectionException.h"
#include "CollectorException.h"
#include <boost/foreach.hpp>
#include <errortoken/ErrorToken.h>
#include <data\doctypes\DocTypes.h>
#include <string/IntString.h>
#include <boost/scope_exit.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <string>
#include <utils\JsonUtils.h>
#include "../context/XContext.h"

using boost::asio::ip::tcp;
namespace pt = boost::posix_time;

const short announce_rq_port = 996;
const short announce_rs_port = 997;
const short server_port = 998;
const char* listen_address = "0.0.0.0";
const char* preambule = "BKMOTO";


CollectorServer::HeartbeatResponse::HeartbeatResponse() : status(CollectorDeviceStatus_None)
{
}

CollectorServer::HeartbeatResponse::HeartbeatResponse(const CollectorServer::HeartbeatResponse& src)
{
	operator=(src);
}

CollectorServer::HeartbeatResponse& CollectorServer::HeartbeatResponse::operator=(const CollectorServer::HeartbeatResponse& right)
{
	deviceName = right.deviceName;
	osName = right.osName;
	osVersion = right.osVersion;
	status = right.status;
	endpoint = right.endpoint;
	return *this;
}
///////////////////////////////////////////////////////////////////////////////////////////////////

CollectorServer::UDPSender::UDPSender(boost::asio::io_service& io_service) : io_service_(io_service), socket_(io_service, ip::udp::endpoint(ip::udp::v4(), 0))
{		
}

CollectorServer::UDPSender::~UDPSender()
{
	socket_.close();
}

void CollectorServer::UDPSender::send(const void* msg, size_t size, const ip::udp::endpoint& endpoint)
{
	socket_.send_to(boost::asio::buffer(msg, size), endpoint);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
CollectorServer::UDPReceiver::UDPReceiver(boost::asio::io_service& io_service, CollectorServer* server) : socket_(io_service, ip::udp::endpoint(ip::udp::v4(), announce_rs_port)), m_server(server)
{

}

void CollectorServer::UDPReceiver::start_receive()
{
	socket_.async_receive_from(
		boost::asio::buffer(recv_buffer_), remote_endpoint_,
		boost::bind(&UDPReceiver::handle_receive, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
}

void CollectorServer::UDPReceiver::handle_receive(const boost::system::error_code& error, std::size_t /*bytes_transferred*/)
{
	if (!error || error == boost::asio::error::message_size)
	{
		heartbeat_rs_frame* rs = (heartbeat_rs_frame*)recv_buffer_.data();

		if (strncmp(rs->preambule, preambule,6) == 0)
		{
			if (TextUtils::IsOneOf(rs->sync_status[0], CollectorDeviceStatus_Busy, CollectorDeviceStatus_Syncing, CollectorDeviceStatus_ReadyToSync, CollectorDeviceStatus_Idle))
			{
				HeartbeatResponse heartbeat;

				heartbeat.endpoint = ip::tcp::endpoint(remote_endpoint_.address(),rs->service_port);
				
				char szDeviceName[HEARTBEAT_RS_MAX_DEVICENAME + 1];
				strncpy_s(szDeviceName, rs->device_name, HEARTBEAT_RS_MAX_DEVICENAME);
				heartbeat.deviceName = TextUtils::FromUTF8(szDeviceName);

				char szOSName[HEARTBEAT_RS_MAX_OSNAME + 1];
				strncpy_s(szOSName, rs->os_name, HEARTBEAT_RS_MAX_OSNAME);
				heartbeat.osName = TextUtils::FromUTF8(szOSName);

				char szOSVersion[HEARTBEAT_RS_MAX_OSVERSION + 1];
				strncpy_s(szOSVersion, rs->os_version, HEARTBEAT_RS_MAX_OSVERSION);
				heartbeat.osVersion = TextUtils::FromUTF8(szOSVersion);


				heartbeat.status = (CollectorDeviceStatus)rs->sync_status[0];

				m_server->HandleHeartbeat(heartbeat);
			}			
		}

		start_receive();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
CollectorServer::CollectorSessionImpl::CollectorSessionImpl(CollectorServer* parent, const Collector& collector) :
	m_parent(parent), m_collector(collector), m_socket(parent->m_serverioservice)
{
}

CollectorServer::CollectorSessionImpl::~CollectorSessionImpl()
{
	
}

const ICollector* CollectorServer::CollectorSessionImpl::GetCollector() const
{
	return &m_collector;
}

void CollectorServer::HandleHeartbeat(const HeartbeatResponse& heartbeat)
{
	PostTask(boost::bind(&CollectorServer::HandleHeartbeatAsync, this, heartbeat));
}

void CollectorServer::HandleHeartbeatAsync(const HeartbeatResponse heartbeat)
{
	CollectorSessionImpl* session = FindSession(heartbeat.endpoint.address().to_v4());
	
	Collector collector(ObtainNextCollectorId());
	collector.SetClientVersion(L"BKMOTO");
	collector.SetDeviceName(heartbeat.deviceName);
	collector.SetOS(heartbeat.osName + L" " + heartbeat.osVersion);
	collector.SetStatus(heartbeat.status);
	collector.SetEndpoint(heartbeat.endpoint);
	
	if (session)
	{
		session->m_collector.CopyFrom(&collector);
		session->m_lastHeartbeatTime = pt::microsec_clock::local_time();	
		VisitObservers(&CollectorServerObserver::CollectorUpdated, session->GetCollector()->GetId());
	}
	else
	{
		session = new CollectorSessionImpl(this, collector);
		session->m_lastHeartbeatTime = pt::microsec_clock::local_time();
		AddSession(session);		
	}	
}


void CollectorServer::CollectorSessionImpl::Connect()
{
	boost::system::error_code error = boost::asio::error::host_not_found;
	m_socket.close();
	
	m_socket.connect(m_collector.GetEndpoint(), error);
	
	if (error)
		BOOST_THROW_EXCEPTION( ConnectionFailedException(L"connection_failed") );
}

void CollectorServer::CollectorSessionImpl::Disconnect()
{
	m_socket.close();
}

void CollectorServer::CollectorSessionImpl::Erase()
{
	std::wstring rq = m_jsonRpcProtocol.BuildRequest(L"1",std::wstring(L"erase"), json::value(), false);
	boost::system::error_code werror;
	boost::asio::write(m_socket, boost::asio::buffer(TextUtils::ToUTF8(rq)), boost::asio::transfer_all(), werror);
	if (!werror)
	{
		json::value response = ReceiveResponse();
	}
}

void CollectorServer::CollectorSessionImpl::BeginUpload(int herdIndex, const std::wstring& userName,SimpleProgress* progress)
{
	progress->SetSteps(1);
	json::value parameters = json::value::array(json::value(herdIndex), json::value(userName), json::value(json_DayDate(DateTime::now())));
	
	std::wstring rq = m_jsonRpcProtocol.BuildRequest(L"1",std::wstring(L"beginupload"), parameters, false);

	boost::system::error_code werror;
	boost::asio::write(m_socket, boost::asio::buffer(TextUtils::ToUTF8(rq)), boost::asio::transfer_all(), werror);
	if (!werror)
	{
		try
		{
			json::value response = ReceiveResponse();
			progress->Step();
		}
		catch (jsonrpc::JsonRpcException& e)
		{
			BOOST_THROW_EXCEPTION( CollectorException(e.msg()));
		}
	}
}
void CollectorServer::CollectorSessionImpl::UploadHents(const std::vector<UploadHent*>& hents, SimpleProgress* progress)
{
	progress->SetSteps(hents.size());
	BOOST_FOREACH(UploadHent* hent, hents)
	{

		json::value parameter = hent->ToJson();
		std::vector<json::value> parameters = { parameter };
		std::wstring rq = m_jsonRpcProtocol.BuildRequest(L"1",std::wstring(L"uploadhent"), json::value::array(parameters), false);

		boost::system::error_code werror;
		boost::asio::write(m_socket, boost::asio::buffer(TextUtils::ToUTF8(rq)), boost::asio::transfer_all(), werror);
		if (!werror)
		{
			try
			{
				json::value response = ReceiveResponse();
			}
			catch (jsonrpc::JsonRpcException& e)
			{
				BOOST_THROW_EXCEPTION( CollectorException(e.msg()) );
			}
		}

		progress->Step();
	}	
}

void CollectorServer::CollectorSessionImpl::UploadClasses(const std::vector<UploadCowClass*>& classes, SimpleProgress* progress)
{
	progress->SetSteps(classes.size());
	BOOST_FOREACH(UploadCowClass* cowClass, classes)
	{

		json::value parameter = cowClass->ToJson();
		std::vector<json::value> parameters = { parameter };
		std::wstring rq = m_jsonRpcProtocol.BuildRequest(L"1",std::wstring(L"uploadclass"), json::value::array(parameters), false);

		boost::system::error_code werror;
		boost::asio::write(m_socket, boost::asio::buffer(TextUtils::ToUTF8(rq)), boost::asio::transfer_all(), werror);
		if (!werror)
		{
			try
			{
				json::value response = ReceiveResponse();
			}
			catch (jsonrpc::JsonRpcException& e)
			{
				BOOST_THROW_EXCEPTION( CollectorException(e.msg()) );
			}
		}

		progress->Step();
	}
}
void CollectorServer::CollectorSessionImpl::UploadCows(const std::vector<UploadCow*>& cows, SimpleProgress* progress)
{
	progress->SetSteps(cows.size());
	BOOST_FOREACH(UploadCow* cow, cows)
	{

		json::value parameter = cow->ToJson();
		std::vector<json::value> parameters = { parameter };
		std::wstring rq = m_jsonRpcProtocol.BuildRequest(L"1",std::wstring(L"uploadcow"), json::value::array(parameters), false);

		boost::system::error_code werror;
		boost::asio::write(m_socket, boost::asio::buffer(TextUtils::ToUTF8(rq)), boost::asio::transfer_all(), werror);
		if (!werror)
		{
			try
			{
				json::value response = ReceiveResponse();
			}
			catch (jsonrpc::JsonRpcException& e)
			{
				BOOST_THROW_EXCEPTION( CollectorException(e.msg()) );
			}
		}

		progress->Step();
	}
}
void CollectorServer::CollectorSessionImpl::EndUpload(SimpleProgress* progress)
{
	progress->SetSteps(1);
	std::wstring rq = m_jsonRpcProtocol.BuildRequest(L"1",std::wstring(L"endupload"), json::value(), false);
	boost::system::error_code werror;
	boost::asio::write(m_socket, boost::asio::buffer(TextUtils::ToUTF8(rq)), boost::asio::transfer_all(), werror);
	if (!werror)
	{
		json::value response = ReceiveResponse();
	}
	progress->Step();
}


UploadProperties CollectorServer::CollectorSessionImpl::BeginDownload(SimpleProgress* progress)
{
	progress->SetSteps(1);
	std::wstring rq = m_jsonRpcProtocol.BuildRequest(L"1",std::wstring(L"begindownload"), json::value(), false);
	boost::system::error_code werror;
	boost::asio::write(m_socket, boost::asio::buffer(TextUtils::ToUTF8(rq)), boost::asio::transfer_all(), werror);
	if (!werror)
	{
		json::value response = ReceiveResponse();
		UploadProperties properties = properties.FromJson(response);
		progress->Step();
		return properties;
	}	
	ThrowCollectorDisconnected();

	return UploadProperties();
}

json::value	CollectorServer::CollectorSessionImpl::DoRemoteCall(const std::wstring& procedureName, const json::value& parameters)
{
	std::wstring rq = m_jsonRpcProtocol.BuildRequest(L"1",procedureName, parameters, false);
	boost::system::error_code werror;
	boost::asio::write(m_socket, boost::asio::buffer(TextUtils::ToUTF8(rq)), boost::asio::transfer_all(), werror);
	if (!werror)
	{
		json::value response = ReceiveResponse();
		return response;
	}
	ThrowCollectorDisconnected();

	return json::value::null();
}


std::vector<DownloadCow*> CollectorServer::CollectorSessionImpl::DownloadCows(SimpleProgress* progress)
{
	std::vector<DownloadCow*> cows;

	int cowCount = GetCowCount();

	progress->SetSteps(cowCount);

	json::value jsonCow;
	do
	{
		jsonCow = DoRemoteCall(L"downloadcow", json::value::null());
		if (!jsonCow.is_null())
		{
			DownloadCow cow = DownloadCow::FromJson(jsonCow);
			cows.push_back(new DownloadCow(cow));
			progress->Step();
		}
	} while (!jsonCow.is_null());

	return std::move(cows);
}

int	CollectorServer::CollectorSessionImpl::GetCowCount()
{
	json::value result = DoRemoteCall(L"getcowcount", json::value::array());
	return result.as_integer();
}

std::vector<DownloadDoc*> CollectorServer::CollectorSessionImpl::DownloadInDocs(SimpleProgress* progress)
{
	std::vector<DownloadDoc*> docs;

	int count = GetInDocCount();

	progress->SetSteps(count);

	json::value jsonDoc;
	do
	{
		jsonDoc = DoRemoteCall(L"downloadindoc", json::value::null());
		if (!jsonDoc.is_null())
		{
			DownloadDoc doc = DownloadDoc::FromJson(jsonDoc);
			docs.push_back(new DownloadDoc(doc));
			progress->Step();
		}
	} while (!jsonDoc.is_null());

	return std::move(docs);
}

int	CollectorServer::CollectorSessionImpl::GetInDocCount()
{
	json::value result = DoRemoteCall(L"getindoccount", json::value::array());
	return result.as_integer();
}

std::vector<DownloadDoc*> CollectorServer::CollectorSessionImpl::DownloadOutDocs(SimpleProgress* progress)
{
	std::vector<DownloadDoc*> docs;

	int count = GetOutDocCount();

	progress->SetSteps(count);

	json::value jsonDoc;
	do
	{
		jsonDoc = DoRemoteCall(L"downloadoutdoc", json::value::null());
		if (!jsonDoc.is_null())
		{
			DownloadDoc doc = DownloadDoc::FromJson(jsonDoc);
			docs.push_back(new DownloadDoc(doc));
			progress->Step();
		}
	} while (!jsonDoc.is_null());


	return std::move(docs);
}

int	CollectorServer::CollectorSessionImpl::GetOutDocCount()
{
	json::value result = DoRemoteCall(L"getoutdoccount", json::value::array());
	return result.as_integer();
}


void CollectorServer::CollectorSessionImpl::EndDownload(SimpleProgress* progress)
{

}

void CollectorServer::CollectorSessionImpl::ThrowCollectorDisconnected()
{
	BOOST_THROW_EXCEPTION(ConnectionAbortedException(ErrorToken(L"collector_disconnected").str()));
}


json::value CollectorServer::CollectorSessionImpl::ReceiveResponse()
{
	boost::system::error_code rerror;

	boost::asio::streambuf buffer;
	boost::asio::read_until(m_socket, buffer, '\0', rerror);

	if (!rerror)
	{
		std::wbuffer_convert<std::codecvt_utf8<wchar_t>> conv_in(&buffer);
		std::wistream ucsbuf(&conv_in);

		std::wstring rq;
		rq.reserve(buffer.size());
		for (wchar_t c; ucsbuf.get(c) && c != 0; )
		{
			rq.append(1, c);
		}

		json::value response;
		m_jsonRpcProtocol.HandleResponse(rq, response);

		return std::move(response);
	}
	ThrowCollectorDisconnected();
	return json::value::null();	
}


//---------------------------------------------------------------------------------------

CollectorServer::CollectorServer() : 
	m_serverioservice(1),
	m_serverwork(m_serverioservice),
	m_acceptor(m_serverioservice),
	m_collectorIdGenerator(1000),
	m_heartbeatSender(m_serverioservice),
	m_heartbeatReceiver(m_serverioservice,this),
	m_heartbeatTimer(m_serverioservice),
	m_sessionTimer(m_serverioservice)
{	
	
}

uint32_t CollectorServer::ObtainNextCollectorId()
{
	return m_collectorIdGenerator.fetch_add(1, std::memory_order_relaxed);
}

void CollectorServer::BurstToNetwork(const boost::asio::ip::address_v4& ip)
{
	std::array<unsigned char, 4> targetIpBytes = ip.to_bytes();
	int this_octet = targetIpBytes[3];

	heartbeat_rq_frame frame;
	strncpy_s(frame.preambule, preambule, 6);
	frame.replay_port = announce_rs_port;

	for (unsigned char i = 1; i < 255; i++)
	{
		//if (i != this_octet)
		{
			targetIpBytes[3] = i;
			boost::asio::ip::address_v4 target_adr(targetIpBytes);

			if (!m_serverioservice.stopped())
			{
				int len = sizeof(heartbeat_rq_frame);
				unsigned char* buffer = (unsigned char*)&frame;
				m_heartbeatSender.send(buffer, len , ip::udp::endpoint(target_adr, announce_rq_port));
			}
		}
	}
}

void CollectorServer::DoUDPBurst()
{	
	boost::asio::ip::tcp::resolver resolver(m_serverioservice);
	boost::asio::ip::tcp::resolver::query query(boost::asio::ip::host_name(), "");
	boost::asio::ip::tcp::resolver::iterator it = resolver.resolve(query);

	boost::asio::ip::address_v4 localIp;
	while (it != boost::asio::ip::tcp::resolver::iterator())
	{
		boost::asio::ip::tcp::endpoint this_endpoint = *it;
		auto this_address = this_endpoint.address();
		if (this_address.is_v4() && !this_address.is_loopback() && !this_address.is_unspecified())
		{
			localIp = this_endpoint.address().to_v4();
			BurstToNetwork(localIp);

		}
		it++;
	}
			
	m_heartbeatTimer.expires_from_now(boost::posix_time::seconds(3));
	m_heartbeatTimer.async_wait(boost::bind(&CollectorServer::DoUDPBurst, this));
}

void CollectorServer::Start()
{		
	m_heartbeatReceiver.start_receive();
	m_serverworker = boost::thread(boost::bind(&boost::asio::io_service::run, &m_serverioservice));
	m_serverioservice.post(boost::bind(&CollectorServer::DoUDPBurst, this));
	ScheduleSessionCheck();
}

void CollectorServer::Stop()
{
	m_serverioservice.stop();
	CloseAllSessions();
}

void CollectorServer::ScheduleSessionCheck()
{
	m_sessionTimer.expires_from_now(pt::seconds(10));
	m_sessionTimer.async_wait(boost::bind(&CollectorServer::SessionIdleCheck, this));
}

void CollectorServer::SessionIdleCheck()
{
	std::set<uint32_t> destroyedSessions;
	pt::ptime now = pt::microsec_clock::local_time();
	auto it = m_sessions.begin();
	while(it != m_sessions.end())
	{
		CollectorSessionImpl* session = *it;
		uint32_t collectorId = session->GetCollector()->GetId();

		if (!session->m_socket.is_open())
		{
			pt::time_duration idleTime = now - session->m_lastHeartbeatTime;
			if (idleTime > pt::seconds(5))
			{			
				destroyedSessions.insert(collectorId);
				it = m_sessions.erase(it);
				delete session;
				continue;
			}
		}		
		it++;
	}

	
	BOOST_FOREACH(uint32_t collectorId, destroyedSessions)
	{
		VisitObservers(&CollectorServerObserver::CollectorDisconnected, collectorId);
	}

	ScheduleSessionCheck();
}

void CollectorServer::Init(ServiceRegistry* registry)
{

}

void CollectorServer::CloseAllSessions()
{
	while(m_sessions.size() > 0)
	{
		DestroySession(m_sessions[0]);
	}
}

void CollectorServer::Do(std::function<void(CollectorServer*)>& action)
{
	typedef std::packaged_task<void()> task_t;

	if (boost::this_thread::get_id() == m_serverworker.get_id())
	{
		action(this);
	}
	else
	{
		std::function<void()> f = std::bind(action, this);
		std::shared_ptr< task_t > copyable_task = std::make_shared< task_t >(f);
		std::future<void> future = copyable_task->get_future();
		m_serverioservice.post(std::bind(&task_t::operator(), copyable_task));
		future.wait();
			
		future.get(); //trigger throwing an exception	
		
	}
}




void CollectorServer::AddSession(CollectorSessionImpl* session)
{
	m_sessions.push_back(session);
	VisitObservers(&CollectorServerObserver::CollectorConnected, session->GetCollector()->GetId());
}

void CollectorServer::DestroySession(CollectorSessionImpl* session)
{
	SessionIndex::index<tag_collectorId>::type& idindex = m_sessions.get<tag_collectorId>();
	idindex.erase(session->GetCollector()->GetId());	
	delete session;
}
void CollectorServer::CloseSession(CollectorSessionImpl* session)
{
	uint32_t collectorId = session->GetCollector()->GetId();
	DestroySession(session);
	VisitObservers(&CollectorServerObserver::CollectorDisconnected, collectorId);
}

CollectorServer::CollectorSessionImpl* CollectorServer::FindSession(uint32_t sessionId)
{
	SessionIndex::index<tag_collectorId>::type& idindex = m_sessions.get<tag_collectorId>();
	auto it = idindex.find(sessionId);
	if (it != idindex.end())
	{
		return *it;
	}

	return NULL;
}

CollectorServer::CollectorSessionImpl* CollectorServer::FindSession(const boost::asio::ip::address_v4& address)
{
	SessionIndex::index<tag_deviceAddress>::type& idindex = m_sessions.get<tag_deviceAddress>();
	auto it = idindex.find(address);
	if (it != idindex.end())
	{
		return *it;
	}

	return NULL;
}


boost::optional<Collector> CollectorServer::GetCollector(uint32_t id)
{
	boost::optional<Collector> result;
	CollectorSession* session = NULL;

	auto get = [this,id](CollectorServer* server) -> CollectorSession*
	{
		return server->FindSession(id);
	};


	if (boost::this_thread::get_id() == m_serverworker.get_id())
	{
		session = get(this);
	}
	else
	{
		session = EvalAny<CollectorSession*>(get);
	}

	if (session != NULL)
	{
		result = session->GetCollector();
	}
	return result;
}

std::vector<Collector> CollectorServer::GetCollectors()
{
	std::vector<Collector> collectors;
	DoAny([&collectors](CollectorServer* server)
	{
		BOOST_FOREACH(CollectorSessionImpl* session, server->m_sessions)
		{
			collectors.push_back(Collector(session->GetCollector()));
		}
	});

	return std::move(collectors);
}

void CollectorServer::Upload(uint32_t collectorId, SeasonSession* session, uint32_t herdId, ComplexProgress* progress)
{
	std::map<uint32_t, UploadHent*> syncHents;
	std::vector<UploadCow*> syncCows;
	std::vector<UploadCowClass*> syncClasses;
	
	SimpleProgress* taskProgress = progress->BeginTask(L"packaging_hents");
	taskProgress->SetSteps(1);
	PtrEnumerator<HentClientObject> hentsEnum = session->EnumHents();
	while (hentsEnum.hasNext())
	{
		HentClientObject* hentCO = *hentsEnum;
		Hent* hent = hentCO->Object();

		if (hent->GetSyncIt())
		{
			syncHents[hent->GetId()] = (CreateHentFrom(hent));
		}
		hentsEnum.advance();
	}
	taskProgress->Step();
	
	taskProgress = progress->BeginTask(L"packaging_classes");
	taskProgress->SetSteps(1);
	PtrEnumerator<IClass> classEnum = session->EnumClasses();
	while (classEnum.hasNext())
	{
		syncClasses.push_back(CreateCowClassFrom(*classEnum));
		classEnum.advance();
	}
	taskProgress->Step();
	
	IHerd* herd = session->GetHerd(herdId);
	int herdIndex = herd->GetHerdIndex();
	HerdRegistry* registry = session->GetHerdRegistry(herdId);
	
	taskProgress = progress->BeginTask(L"packaging_cows");
	taskProgress->SetSteps(registry->GetCountOfInStockCows());
	PtrEnumerator<ICowPresence> cowPresenceEnum = registry->EnumCows();
	while (cowPresenceEnum.hasNext())
	{
		ICowPresence* presence = *cowPresenceEnum;

		if (presence->IsInStock())
		{
			syncCows.push_back(CreateCowFrom(presence,session));
			taskProgress->Step();
		}
		cowPresenceEnum.advance();
	}
	
	ISimpleUser* user = session->GetContext()->GetUser();

	UploadSnapshot* snapshot = new UploadSnapshot;
	snapshot->herdIndex = herd->GetHerdIndex();	
	snapshot->cows.swap(syncCows);
	boost::range::copy(syncHents | boost::adaptors::map_values, std::back_inserter(snapshot->hents));	
	snapshot->classes.swap(syncClasses);

	BOOST_SCOPE_EXIT(&snapshot) {
		DeepDelete(snapshot->cows);
		DeepDelete(snapshot->hents);
		DeepDelete(snapshot->classes);
		delete snapshot;
	} BOOST_SCOPE_EXIT_END
		
	DoAny([snapshot, collectorId, progress, herdIndex, user](CollectorServer* server)
	{
		CollectorSessionImpl* session = server->FindSession(collectorId);
		if (!session) BOOST_THROW_EXCEPTION( CollectorNotFoundException(ErrorToken(L"collector_not_found")(L"collectorid", collectorId).str()) );

		try
		{		
			session->Connect();
			session->BeginUpload(herdIndex, user->GetLogin(), progress->BeginTask(L"starting_upload"));
			session->UploadClasses(snapshot->classes, progress->BeginTask(L"uploading_classes"));
			session->UploadHents(snapshot->hents, progress->BeginTask(L"uploading_hents"));
			session->UploadCows(snapshot->cows, progress->BeginTask(L"uploading_cows"));
			session->EndUpload(progress->BeginTask(L"saving_upload"));
			session->Disconnect();
		}
		catch (ConnectionFailedException& e)
		{
			session->Disconnect();
			BOOST_THROW_EXCEPTION( CollectorException(L"failed_to_connect_to_collector") );
		}
		catch (ConnectionAbortedException& e)
		{
			session->Disconnect();
			BOOST_THROW_EXCEPTION( CollectorException(L"connection_with_collector_aborted") );
		}
		catch (...)
		{
			session->Disconnect();
			boost::rethrow_exception(boost::current_exception());
		}
	});	
}

std::unique_ptr<DownloadSnapshot> CollectorServer::Download(uint32_t collectorId, ComplexProgress* progress)
{
	std::unique_ptr<DownloadSnapshot> result = std::make_unique<DownloadSnapshot>();
	DoAny([&result, collectorId, progress](CollectorServer* server)
	{
		CollectorSessionImpl* session = server->FindSession(collectorId);
		if (!session) BOOST_THROW_EXCEPTION(CollectorNotFoundException(ErrorToken(L"collector_not_found")(L"collectorid", collectorId).str()));

		try
		{
			std::vector<DownloadDoc*> indocs;
			std::vector<DownloadDoc*> outdocs;
			std::vector<DownloadCow*> cows;

			/*BOOST_SCOPE_EXIT(&indocs,&outdocs,&cows) {
				DeepDelete(indocs);
				DeepDelete(outdocs);
				DeepDelete(cows);				
			} BOOST_SCOPE_EXIT_END*/


			session->Connect();
			UploadProperties properties = session->BeginDownload(progress->BeginTask(L"starting_download"));
			indocs = std::move(session->DownloadInDocs(progress->BeginTask(L"downloading_indocs")));
			outdocs = std::move(session->DownloadOutDocs(progress->BeginTask(L"downloading_outdocs")));
			cows = std::move(session->DownloadCows(progress->BeginTask(L"downloading_cows")));
			session->EndDownload(progress->BeginTask(L"ending_download"));
			session->Disconnect();


			BOOST_FOREACH(DownloadDoc* doc, indocs)
			{
				result->AddInDoc(*doc);
			}

			BOOST_FOREACH(DownloadDoc* doc, outdocs)
			{
				result->AddOutDoc(*doc);
			}

			BOOST_FOREACH(DownloadCow* cow, cows)
			{
				result->AddCow(*cow);
			}

			result->SetDownloadDtTm(DateTime::now());
			result->SetHerdIndex(properties.herdIndex);
			result->SetUploadUser(properties.userName);
			result->SetUploadDtTm(properties.uploadDtTm);
			
		}
		catch (ConnectionFailedException& e)
		{
			session->Disconnect();
			BOOST_THROW_EXCEPTION(CollectorException(L"failed_to_connect_to_collector"));
		}
		catch (ConnectionAbortedException& e)
		{
			session->Disconnect();
			BOOST_THROW_EXCEPTION(CollectorException(L"connection_with_collector_aborted"));
		}
		catch (jsonrpc::JsonRpcException& e)
		{
			session->Disconnect();
			BOOST_THROW_EXCEPTION(CollectorException(L"collector_comm_error"));
		}
		catch (...)
		{
			session->Disconnect();
			boost::rethrow_exception(boost::current_exception());
		}
	});

	return result;
}

UploadCow* CollectorServer::CreateCowFrom(const ICowPresence* presence,SeasonSession* session)
{
	UploadCow* result = new UploadCow();
	result->Id = presence->GetCowId();
	result->ExternalId = result->Id;
	result->CowNo = presence->GetCowNo().ToString();
	result->Sex = presence->GetSex();
	result->Weight = presence->GetOwnWeight();

	IClass* owncowclass = session->GetClass(presence->GetOwnClassId());
	result->CowClass = SafeGet(&IClass::GetClassCode, owncowclass, L"");

	result->OutGroup = presence->GetHerdOutGrp();
	result->InGroup = presence->GetHerdInGrp();

	result->TermBuyPrice = presence->GetTermBuyPrice();
	result->TermBuyWeight = presence->GetTermBuyWeight();
	
	IClass* buycowclass = session->GetClass(presence->GetTermBuyClassId());
	result->TermBuyClass = SafeGet(&IClass::GetClassCode, buycowclass, L"");
		
	result->TermSellPrice = presence->GetTermSellPrice();
	result->TermSellWeight = presence->GetTermSellWeight();

	IClass* sellcowclass = session->GetClass(presence->GetTermSellClassId());
	result->TermSellClass = SafeGet(&IClass::GetClassCode, sellcowclass, L"");
		
	const IDoc* plusDoc = presence->GetHerdInDoc();
	result->PlusDocId = CreateDocId(plusDoc);

	result->PlusDocNumber = DocNoText(plusDoc).ToString();
	result->LoadDate = plusDoc->GetLoadDate();
	
	if (plusDoc->GetDocType() == DocType_Buy)
	{
		HentClientObject* hentCO = session->GetHent(((BuyDoc*)plusDoc)->GetHentId());
		Hent* hent = hentCO->Object();	
		result->DeliverHentName = hent->GetName();
	}

	result->DeliverPlateNo = plusDoc->GetPlateNo();
	result->PlusMotive = plusDoc->GetMotive();

	ICow* cow = session->GetCow(presence->GetCowId())->Object();

	uint32_t buyInvoiceId = NULL_ID;
	switch (plusDoc->GetDocType())
	{
		case DocType_Buy:
			buyInvoiceId = ((BuyDoc*)plusDoc)->GetInvoiceId();
			break;
		case DocType_In:
		   buyInvoiceId = cow->GetBuyInvoiceId();
			break;
	}
	

	if (buyInvoiceId != NULL_ID)
	{
		BuyInvoiceClientObject* pBuyInvoiceCO = session->GetBuyInvoice(buyInvoiceId);
		BuyInvoice* pBuyInvoice = pBuyInvoiceCO->Object();
		const IInvoiceHent* pHent = pBuyInvoice->GetInvoiceHent();
		result->BuyFromHentName = pHent->GetName();
	}

	return result;
}


UploadHent* CollectorServer::CreateHentFrom(const IHent* hent)
{
	UploadHent* result = new UploadHent;
	result->Id = hent->GetId();
	result->Alias = hent->GetAlias();
	result->Name = hent->GetName();
	result->Zip = hent->GetZip();
	result->City = hent->GetCity();
	result->Street = hent->GetStreet();
	result->POBox = hent->GetPOBox();
	result->HentNo = hent->GetHentNo().ToString();
	result->Plate = hent->GetPlateNo();
	result->Type = hent->GetHentType();

	return result;
}

std::wstring CollectorServer::CreateDocId(const IDoc* doc)
{
	return DocTypes::ToString(doc->GetDocType()) + L"/" + IntString(doc->GetId());
}

UploadCowClass* CollectorServer::CreateCowClassFrom(const IClass* cowClass)
{
	UploadCowClass* result = new UploadCowClass();
	result->Code = cowClass->GetClassCode();
	result->Sex = cowClass->GetPredefSex();
	return result;
}


