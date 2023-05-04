#ifndef __COLLECTORSERVER_H__
#define __COLLECTORSERVER_H__

#include <boost/asio.hpp>
#include <set>
#include <arrays/Enumerator.h>
#include "../context/Observerable.h"
#include "CollectorSession.h"
#include "CollectorServerObserver.h"
#include <client/rpcprotocolclient.h>
#include "../context/SeasonSession.h"
#include "data\DownloadSnapshot.h"
#include "data\UploadSnapshot.h"
#include <boost/thread.hpp>
#include "Collector.h"
#include <atomic>
#include "../services/AppService.h"

namespace ip = boost::asio::ip;


#define HEARTBEAT_RS_MAX_PREAMBULE 7
#define HEARTBEAT_RS_MAX_DEVICENAME 30
#define HEARTBEAT_RS_MAX_OSNAME 30
#define HEARTBEAT_RS_MAX_OSVERSION 30

class CollectorServer : public Observerable<CollectorServerObserver>, public AppService
{

#pragma pack(push,1)
	struct heartbeat_rq_frame
	{
		char preambule[HEARTBEAT_RS_MAX_PREAMBULE];
		uint16_t replay_port;
	};
	
	struct heartbeat_rs_frame
	{
		char preambule[HEARTBEAT_RS_MAX_PREAMBULE];
		uint16_t service_port;
		char device_name[HEARTBEAT_RS_MAX_DEVICENAME];
		char os_name[HEARTBEAT_RS_MAX_OSNAME];
		char os_version[HEARTBEAT_RS_MAX_OSVERSION];
		char sync_status[1];
	};
#pragma pack(pop)
	

	class HeartbeatResponse
	{
	public:
		HeartbeatResponse();
		HeartbeatResponse(const HeartbeatResponse& src);
		HeartbeatResponse& operator=(const HeartbeatResponse& right);

		std::wstring deviceName;
		std::wstring osName;
		std::wstring osVersion;
		CollectorDeviceStatus status;
		ip::tcp::endpoint endpoint;
	};

	class UDPSender
	{
	public:
		UDPSender(boost::asio::io_service& io_service);

		~UDPSender();

		void send(const void* msg, size_t size, const ip::udp::endpoint& endpoint);

	private:
		boost::asio::io_service& io_service_;
		ip::udp::socket socket_;
	};

	class UDPReceiver
	{
	public:
		UDPReceiver(boost::asio::io_service& io_service, CollectorServer* server);
	
		void start_receive();

		void handle_receive(const boost::system::error_code& error, std::size_t /*bytes_transferred*/);
	private:
		ip::udp::socket socket_;
		ip::udp::endpoint remote_endpoint_;
		boost::array<char, 1000> recv_buffer_;
		CollectorServer* m_server;
	};
	
	struct tag_collectorId {};
	struct tag_deviceAddress {};


	struct IdExtractor
	{
		typedef uint32_t result_type;
		result_type operator()(CollectorSession* session) const
		{
			return session->GetCollector()->GetId();
		}
	};

	struct AddressExtractor
	{
		typedef boost::asio::ip::address_v4 result_type;
		result_type operator()(CollectorSession* session) const
		{
			return session->GetCollector()->GetEndpoint().address().to_v4();
		}
	};


	class CollectorSessionImpl;

	typedef boost::multi_index_container
		<
		CollectorSessionImpl*,
		boost::multi_index::indexed_by<
		boost::multi_index::random_access<>,
		boost::multi_index::ordered_unique < boost::multi_index::tag<tag_collectorId>, IdExtractor >,
		boost::multi_index::ordered_unique < boost::multi_index::tag<tag_deviceAddress>, AddressExtractor >
		>
		> SessionIndex;
	

	class CollectorSessionImpl : public CollectorSession
	{
	public:
		CollectorSessionImpl(CollectorServer* parent, const Collector& collector);
		virtual ~CollectorSessionImpl();
		virtual const ICollector* GetCollector() const;
		void Connect();
		void Disconnect();


		
		void Erase();

		

		virtual void BeginUpload(int herdIndex,const std::wstring& userName,SimpleProgress* progress);
		virtual void UploadHents(const std::vector<UploadHent*>& hents, SimpleProgress* progress);
		virtual void UploadClasses(const std::vector<UploadCowClass*>& classes, SimpleProgress* progress);
		virtual void UploadCows(const std::vector<UploadCow*>& cows, SimpleProgress* progress);
		virtual void EndUpload(SimpleProgress* progress);

		virtual UploadProperties BeginDownload(SimpleProgress* progress);
		virtual std::vector<DownloadCow*> DownloadCows(SimpleProgress* progress);
				int	GetCowCount();
				
		virtual std::vector<DownloadDoc*> DownloadInDocs(SimpleProgress* progress);
				int	GetInDocCount();

		virtual std::vector<DownloadDoc*> DownloadOutDocs(SimpleProgress* progress);
				int	GetOutDocCount();

		virtual void EndDownload(SimpleProgress* progress);

		json::value ReceiveResponse();

		json::value	DoRemoteCall(const std::wstring& procedureName, const json::value& parameters);

		void ThrowCollectorDisconnected();

		
		jsonrpc::RpcProtocolClient m_jsonRpcProtocol;
		Collector m_collector;
		CollectorServer* m_parent;
		boost::asio::ip::tcp::socket m_socket;	
		boost::posix_time::ptime m_lastHeartbeatTime;
	};


public:
	CollectorServer();
	virtual void Start();
	virtual void Stop();		
	virtual void Init(ServiceRegistry* registry);
	
	boost::optional<Collector> GetCollector(uint32_t id);
	std::vector<Collector> GetCollectors();
	void Upload(uint32_t collectorId, SeasonSession* session,uint32_t herdId, ComplexProgress* progress);
	std::unique_ptr<DownloadSnapshot> Download(uint32_t collectorId, ComplexProgress* progress);
	
protected:
	void HandleHeartbeat(const HeartbeatResponse& heartbeat);
	void HandleHeartbeatAsync(const HeartbeatResponse heartbeat);

	void ReceiveCallback(CollectorSessionImpl* session, boost::system::error_code const& error, size_t bytes_transferred);
	void CloseAllSessions();
	void DoUDPBurst();
	void BurstToNetwork(const boost::asio::ip::address_v4& ip);
	void SessionIdleCheck();

	void ScheduleSessionCheck();


	void AddSession(CollectorSessionImpl* session);
	void CloseSession(CollectorSessionImpl* session);
	CollectorSessionImpl* FindSession(uint32_t sessionId);
	CollectorSessionImpl* FindSession(const boost::asio::ip::address_v4& address);
	void DestroySession(CollectorSessionImpl* session);
	UploadCow* CreateCowFrom(const ICowPresence* cow, SeasonSession* session);
	UploadHent* CreateHentFrom(const IHent* hent);
	UploadCowClass* CreateCowClassFrom(const IClass* cowClass);
	std::wstring CreateDocId(const IDoc* doc);
	std::wstring CowSexToJson(const CowSex& sex);

	void Do(std::function<void(CollectorServer*)>& action);

	template<typename F>
	void DoAny(const F& f)
	{
		std::function<void(CollectorServer*)> job = f;
		Do(job);
	}

	template<typename R>
	R Eval(std::function< R(CollectorServer*)> & f)
	{
		typedef std::packaged_task<R(CollectorServer*)> eval_t;
		eval_t evaluator(f);
		std::function<void(CollectorServer*)> evalFunctor = std::bind(&eval_t::operator(), &evaluator, std::placeholders::_1);
		Do(evalFunctor);
		return evaluator.get_future().get();
	}

	template<typename R, typename F>
	R EvalAny(const F& functor)
	{
		std::function< R(CollectorServer*)>  f = functor;
		return Eval<R>(f);
	}

	template < typename Task > 	void PostTask(Task task)
	{
		// Post a wrapped task into the queue.
		m_serverioservice.post(boost::function< void() >(task));
	}

	uint32_t ObtainNextCollectorId();
	

	friend class CollectorSessionImpl;
	friend class ServerFacade;
	friend class SessionMonitor;
	
private:
  boost::asio::io_service m_serverioservice;
  boost::asio::io_service::work m_serverwork;
  boost::thread m_serverworker;
  std::atomic<uint32_t> m_collectorIdGenerator;

  boost::asio::ip::tcp::acceptor m_acceptor;

  boost::asio::deadline_timer m_sessionTimer;
  boost::asio::deadline_timer m_heartbeatTimer;
  UDPSender m_heartbeatSender;
  UDPReceiver m_heartbeatReceiver;

  SessionIndex m_sessions;
 
 
  char m_onebyte[1];
};

#endif
