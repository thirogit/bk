#ifndef __CURSOR_H__
#define __CURSOR_H__

#include <boost/shared_ptr.hpp>
#include <arrays/NewPtrFlushArray.h>

namespace repo 
{

class CursorFetchLimit
{
public:
	CursorFetchLimit(int minFetch,int maxFetch) : m_MinFetch(minFetch),m_MaxFetch(maxFetch) {};
	int GetMaxFetch() const { return m_MaxFetch; }
	int GetMinFetch() const { return m_MinFetch; } 
private:
	int m_MinFetch;
	int m_MaxFetch;
};

template<class ChunkType>
class Cursor
{
protected:
	Cursor(const std::wstring& id,int count,const CursorFetchLimit& limits) : m_Count(count),m_Limits(limits),m_Id(id) {};
public:
	inline int GetCount() const { return m_Count; };
	inline const std::wstring& GetId() const { return m_Id; };
	inline const CursorFetchLimit& GetLimits() const { return m_Limits; };	
	virtual ChunkType FetchMany(int howMany) = 0;
private:
	int m_Count;
	CursorFetchLimit m_Limits;
	std::wstring m_Id;
};

}

#endif
