#ifndef __COWPASSPORT_H__
#define __COWPASSPORT_H__

#include <types\NullInt.h>
#include <data\types\CowNo.h>

class CowPassport
{
public:
	CowPassport();
	CowPassport(const CowNo& cowNo,const NullInt& group,const std::wstring& passportNo);
	CowPassport(const CowPassport& src);
	CowPassport& operator=(const CowPassport& right);

	const NullInt& GetGroup() const;
	const CowNo&  GetCowNo() const;
	const std::wstring& GetPassportNo() const;

private:
	CowNo m_cowNo;
	NullInt m_group;
	std::wstring m_passportNo;
};


#endif