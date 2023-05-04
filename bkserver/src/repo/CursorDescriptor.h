#ifndef __CURSORDESCRIPTOR_H__
#define __CURSORDESCRIPTOR_H__

#include <string>

namespace repo 
{

class CursorDescriptor
{
public:
	CursorDescriptor(const std::wstring& cursorId);
	CursorDescriptor(const CursorDescriptor& src);
	CursorDescriptor& operator=(const CursorDescriptor& right);
	void CopyFrom(const CursorDescriptor& src);

	int GetCount() const;
	int GetMinFetch() const;
	int GetMaxFetch() const;
	const std::wstring& GetCursorId() const;

	void SetCount(int count);
	void SetMinFetch(int minFetch);
	void SetMaxFetch(int maxFetch);
private:
	std::wstring m_CursorId;
	int m_Count;
	int m_MinFetch;
	int m_MaxFetch;
};

}

#endif