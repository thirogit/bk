#pragma once

class ClientSeasonSession;

enum LockType : wchar_t
{
	LockType_Shallow = L'S',
	LockType_Deep = L'D',
	LockType_None = L'\0'
};


class LockableObject
{
public:
	LockableObject();
	bool Lock(ClientSeasonSession* pClient,LockType lockType);
	bool Unlock(ClientSeasonSession* pClient);
	bool OwnsLock(ClientSeasonSession* pClient);
	ClientSeasonSession* GetLockHolder() const;
	bool IsLocked();
	LockType GetLockType() const;
private:
	ClientSeasonSession* m_pClient;
	LockType m_lockType;
};