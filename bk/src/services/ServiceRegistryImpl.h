#ifndef __SERVICEREGISTRYIMPL_H__
#define __SERVICEREGISTRYMPL_H__

#include <string>
#include <map>
#include  <boost/core/noncopyable.hpp>
#include <RCF/ThreadLibrary.hpp>
#include <boost/function.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/member.hpp>
#include "AppService.h"
#include <boost/functional/factory.hpp>
#include <string/TextUtils.h>
#include "ServiceRegistry.h"

class ServiceRegistryImpl : boost::noncopyable, public ServiceRegistry
{
	typedef std::string                         Rtti;

	struct ServiceRecord
	{
		Rtti serviceTypeName;
		boost::function<AppService* ()> constructor;
		std::wstring serviceName;
		AppService* instance;
	};

	struct index_tag_name {};
	struct index_tag_type {};

	typedef boost::multi_index_container
		<
		ServiceRecord*,
		boost::multi_index::indexed_by<
		boost::multi_index::random_access<>,
		boost::multi_index::ordered_unique < boost::multi_index::tag<index_tag_name>, boost::multi_index::member<ServiceRecord, std::wstring, &ServiceRecord::serviceName> >,
		boost::multi_index::ordered_unique < boost::multi_index::tag<index_tag_type>, boost::multi_index::member<ServiceRecord, std::string, &ServiceRecord::serviceTypeName> >
		>
		> ServiceIndex;	
		  
       
		ServiceIndex m_index;
		bool m_started;
        RCF::ReadWriteMutex                         mReadWriteMutex;

    public:
		
		ServiceRegistryImpl();
		~ServiceRegistryImpl();

		class ServiceRegistryFacade : public ServiceRegistry
		{
		public:
			ServiceRegistryFacade(ServiceRegistryImpl* parent);
			virtual AppService* FindByType(const std::type_info &ti);
			ServiceRegistryImpl* m_parent;
		};

		template<typename Type> void RegisterService(Type *)
		{
			RegisterService<Type>((Type*)0, TextUtils::FromUTF8(typeid(Type).name()));
		}


		template<typename Type>
		void RegisterService(Type *, const std::wstring &serviceName)
		{
			RCF::WriteLock lock(mReadWriteMutex);

			Rtti typeRtti = typeid(Type).name();

			ServiceRecord* record = new ServiceRecord();
			record->serviceTypeName = typeRtti;
			record->constructor = boost::factory<Type*>();
			record->serviceName = serviceName;
			record->instance = NULL;

			m_index.push_back(record);

		}
     
		AppService* FindByType(const std::type_info &ti);

		AppService* FindByName(const std::wstring& name);

        

		void Clear();
		void Start();
		void Stop();

	protected:
		AppService* FindByTypeNoLock(const std::type_info &ti);

		AppService* FindByNameNoLock(const std::wstring& name);
    };

#endif