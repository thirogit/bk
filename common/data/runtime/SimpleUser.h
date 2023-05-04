#ifndef __SIMPLEUSER_H__
#define __SIMPLEUSER_H__

#include "ISimpleUser.h"
#include <SF/Archive.hpp>

class SimpleUser : public ISimpleUser
{
public:
	SimpleUser(uint32_t id);
	SimpleUser(const SimpleUser& src);
	SimpleUser(const ISimpleUser* src);
	void CopyFrom(const ISimpleUser* pUser);
	
	virtual const std::wstring& GetLogin() const;
	virtual const std::wstring& GetFirstName() const;
	virtual const std::wstring& GetLastName() const;
	virtual uint32_t GetId() const;

	void SetLogin(const std::wstring& login);
	void SetFirstName(const std::wstring& firstname);
	void SetLastName(const std::wstring& lastname);

	void serialize(SF::Archive &archive);
private:
	uint32_t m_id;
	std::wstring m_login;
	std::wstring m_firstname;
	std::wstring m_lastname;
	
};

#endif