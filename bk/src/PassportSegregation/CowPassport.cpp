#include "stdafx.h"
#include "CowPassport.h"


CowPassport::CowPassport()
{

}

CowPassport::CowPassport(const CowNo& cowNo,const NullInt& group,const std::wstring& passportNo) : m_cowNo(cowNo),m_group(group),m_passportNo(passportNo)
{
}

CowPassport::CowPassport(const CowPassport& src)
{
	operator=(src);
}

CowPassport& CowPassport::operator=(const CowPassport& right)
{
	m_cowNo = right.m_cowNo;
	m_group = right.m_group;
	m_passportNo = right.m_passportNo;

	return *this;
}

const NullInt& CowPassport::GetGroup() const
{
	return m_group;
}

const CowNo&  CowPassport::GetCowNo() const
{
	return m_cowNo;
}

const std::wstring& CowPassport::GetPassportNo() const
{
	return m_passportNo;
}
