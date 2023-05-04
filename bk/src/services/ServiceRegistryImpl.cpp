#include "stdafx.h"
#include "ServiceRegistryImpl.h"
#include <boost/foreach.hpp>


ServiceRegistryImpl::ServiceRegistryFacade::ServiceRegistryFacade(ServiceRegistryImpl* parent) : m_parent(parent)
{
}

AppService* ServiceRegistryImpl::ServiceRegistryFacade::FindByType(const std::type_info &ti)
{
	return m_parent->FindByTypeNoLock(ti);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
ServiceRegistryImpl::ServiceRegistryImpl() :
        mReadWriteMutex(RCF::WriterPriority), m_started(false)
{
}


AppService* ServiceRegistryImpl::FindByTypeNoLock(const std::type_info &ti)
{
	ServiceIndex::index<index_tag_type>::type& indexByType = m_index.get<index_tag_type>();

	auto it = indexByType.find(ti.name());
	if (it != indexByType.end())
	{
		return (*it)->instance;
	}
	return NULL;
}

AppService* ServiceRegistryImpl::FindByNameNoLock(const std::wstring& name)
{
	ServiceIndex::index<index_tag_name>::type& indexByName = m_index.get<index_tag_name>();

	auto it = indexByName.find(name);
	if (it != indexByName.end())
	{
		return (*it)->instance;
	}
	return NULL;
}



AppService* ServiceRegistryImpl::FindByType(const std::type_info &ti)
{
	RCF::ReadLock lock(mReadWriteMutex);
	return FindByTypeNoLock(ti);
	
}

AppService* ServiceRegistryImpl::FindByName(const std::wstring& name)
{
	RCF::ReadLock lock(mReadWriteMutex);
	return FindByNameNoLock(name);	
}


ServiceRegistryImpl::~ServiceRegistryImpl()
{
	Clear();
}


void ServiceRegistryImpl::Clear()
{
	RCF::WriteLock lock(mReadWriteMutex);

	auto it = m_index.begin();
	while (it != m_index.end())
	{
		ServiceRecord* record = *it;
		if (record->instance)
		{			
			delete record->instance;
			record->instance = NULL;
		}

		delete record;

		it++;
	}

	m_index.clear();
       
}

void ServiceRegistryImpl::Start()
{
	RCF::WriteLock lock(mReadWriteMutex);

	ServiceRegistryFacade facade(this);

	if (m_started)
		throw;

	auto it = m_index.begin();
	while (it != m_index.end())
	{
		ServiceRecord* record = *it;
		record->instance = record->constructor();
		record->instance->Init(&facade);
		record->instance->Start();

		it++;
	}

	m_started = true;
}

void ServiceRegistryImpl::Stop()
{
	RCF::WriteLock lock(mReadWriteMutex);
	
	if (!m_started)
		return;

	auto it = m_index.begin();
	while (it != m_index.end())
	{
		ServiceRecord* record = *it;
		if (record->instance)
		{
			record->instance->Stop();
			delete record->instance;
			record->instance = NULL;
		}
		it++;
	}

	m_started = false;
}




