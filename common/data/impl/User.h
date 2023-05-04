#ifndef __USER_H__
#define __USER_H__

#include "../IUser.h"

class User : public IUser
{
public:

	User();
	User(uint32_t Id,IFarm* pFarm);
	
	void CopyFrom(const User &copy);
	User(User &copy);
	User& operator=(const User& right);
	
	virtual const std::wstring&  GetLogin() const;
	virtual uint32_t GetId() const;	
	virtual const std::wstring& GetFirstName() const;
	virtual const std::wstring& GetLastName() const;
	virtual const std::wstring& GetPassword() const;
	virtual const DateTime& GetLastLogin() const;
	virtual const IFarm* GetFarm() const;

	void SetLogin(const std::wstring&);
	void SetFirstName(const std::wstring&);
	void SetLastName(const std::wstring&);
	void SetPassword(const std::wstring&);
	void SetLastLogin(const DateTime&);
	
protected:
	std::wstring m_UserName;
	int m_Id;
	std::wstring m_FirstName;
	std::wstring m_LastName;
	std::wstring m_Password;
	DateTime m_LastLogin;
	IFarm* m_pFarm;
		


};




#endif