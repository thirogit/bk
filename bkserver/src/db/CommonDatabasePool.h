#ifndef __COMMONDATABASEPOOL_H__
#define __COMMONDATABASEPOOL_H__

#include <map>
#include "DbConnection.h"
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/locks.hpp>
#include <boost/bind.hpp>
#include "DbConnectionOptions.h"

using namespace std;

enum{
	DBPOOL_POOLEDCON_FREE,
	DBPOOL_POOLEDCON_USED,
	DBPOOL_TEMPCON_USED
};

template <typename T>
class CommonDatabasePool{
public:
	virtual void CreatePool(const DbConnectionOptions&,int,int);
	virtual DbConnection* GetConnectionFromPool();
	virtual void ReleaseConnectionToPool(DbConnection*);
	virtual void DestroyPool();
	void    KeepPoolAlive();
	
	bool IsPoolValid();
	

protected:
	/*	These have to be implemented in the technology specific Pool 
		MySQL, Oracle, SQLServer etc
	 */
	virtual T* MyGetConcreteConnection()=0;
	virtual void MyReleaseConcreteConnection(T*)=0;
	virtual int MyPingConcreteConnection(T*)=0;
	virtual int MyGetPingTimeout()=0;

	const DbConnectionOptions& GetConnectionOptions() const;
	
private:
	unsigned int poolerrorno;
	bool exitkeepalivepool;
	bool ispoolvalid;
	map<T*, short> poolmap;
	int pooledconnections;
	unsigned int maxconnections;
	DbConnectionOptions m_ConnOptions;
	
	boost::condition_variable destroy_signal;
	boost::thread keepalive_thread;
	boost::mutex busy_mutex;
};

template <typename T>
const DbConnectionOptions& CommonDatabasePool<T>::GetConnectionOptions() const
{
	return m_ConnOptions;
}

template <typename T>
void CommonDatabasePool<T>::CreatePool(const DbConnectionOptions& connOptions,int pooled, int max){
	pooledconnections=pooled;
	maxconnections=max;
	ispoolvalid=false;
	m_ConnOptions = connOptions;

	T* t;
	for(int i=0 ; i < pooled ; i++){
		if((t = MyGetConcreteConnection())!=0){
			if(MyPingConcreteConnection(t)==0)
				poolmap.insert(make_pair(t, DBPOOL_POOLEDCON_FREE));
			else
				MyReleaseConcreteConnection(t);
		}
	}

	exitkeepalivepool = false;
	
	keepalive_thread = boost::thread(boost::bind(&CommonDatabasePool<T>::KeepPoolAlive,this));
	
}

template <typename T>
DbConnection* CommonDatabasePool<T>::GetConnectionFromPool(){
	
	boost::unique_lock<boost::mutex> lock(busy_mutex);
	T* t=0;
	typename map<T*, short>::iterator iter;
	for( iter = poolmap.begin(); iter != poolmap.end(); iter++ ) {
		if(iter->second == DBPOOL_POOLEDCON_FREE){
			iter->second = DBPOOL_POOLEDCON_USED;
			t = iter->first;
			break;
		}
	}
	if (t==0){
		if(poolmap.size()< maxconnections){
			if((t = MyGetConcreteConnection())!=0){
				MyPingConcreteConnection(t);
				poolmap.insert(make_pair(t, DBPOOL_TEMPCON_USED));
			}
		}
	}	
	return t;
}

template <typename T>
void CommonDatabasePool<T>::ReleaseConnectionToPool(DbConnection* t)
{
	
	boost::unique_lock<boost::mutex> lock(busy_mutex);
	typename map<T*, short>::iterator iter = poolmap.find((T*)t);
	
    	if( iter != poolmap.end() ) 
	{
		if(iter->second==DBPOOL_POOLEDCON_USED)
			iter->second=DBPOOL_POOLEDCON_FREE;
		if(iter->second==DBPOOL_TEMPCON_USED)
		{
			MyReleaseConcreteConnection(iter->first);
			poolmap.erase(iter);
		}
	}	
}

template <typename T>
void CommonDatabasePool<T>::DestroyPool()
{
	typename map<T*, short>::iterator iter;
	for( iter = poolmap.begin(); !poolmap.empty()&& iter != poolmap.end();  ) 
	{
		if(iter->second == DBPOOL_POOLEDCON_FREE)
		{
			MyReleaseConcreteConnection(iter->first);
			iter = poolmap.erase(iter);
			
		}
		else
		{
			iter++;
		}
	}
	
	exitkeepalivepool = true;

	destroy_signal.notify_one();	
	
	keepalive_thread.join();	
	
}

template <typename T>
void  CommonDatabasePool<T>::KeepPoolAlive()
{
	while(!exitkeepalivepool && ispoolvalid)
	{
	
		boost::unique_lock<boost::mutex> lock(busy_mutex);
		
		typename map<T*, short>::iterator iter;
		for( iter = poolmap.begin(); !poolmap.empty() && 
				iter != poolmap.end(); iter++ ) {
			if(iter->second == DBPOOL_POOLEDCON_FREE)
				if(MyPingConcreteConnection(iter->first)==-1){
					ReleaseConnectionToPool(iter->first);
				}
		}
		
		if(destroy_signal.wait_for(lock,
		boost::chrono::milliseconds(MyGetPingTimeout())) 
		== boost::cv_status::no_timeout)
		{

			ispoolvalid=false;
			break;
		}
	}
}

template <typename T>
bool CommonDatabasePool<T>::IsPoolValid(){
	return ispoolvalid;
}





#endif
