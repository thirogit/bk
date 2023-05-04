#include "stdafx.h"
#include "CowSex.h"

CowSex::CowSex(COWSEX sex) 
{
	if(IsCowSex(sex))
		m_sex = sex;
	else
		m_sex = SEX_UNK;
}

std::wstring CowSex::ToString() const
{
	switch(m_sex)
	{
		case SEX_XX:
			return L"XX";
		case SEX_XY:
			return L"XY";
		case SEX_UNK:
			return L"--";
	}
	
	return L"";
}

void CowSex::MakeXX()
{
	m_sex = SEX_XX;
}

void CowSex::MakeXY()
{
	m_sex = SEX_XY;
}

void CowSex::MakeUNK()
{
	m_sex = SEX_UNK;
}

void CowSex::Make(COWSEX eSex)
{
	if(!IsCowSex(eSex))
		MakeUNK();
	else m_sex = eSex;
}

CowSex& CowSex::operator=(const CowSex &right)
{
	m_sex = right.m_sex;
	return *this;
}

CowSex& CowSex::operator=(COWSEX right)
{
	m_sex = right;
	return *this;
}

bool CowSex::IsCowSex(COWSEX eSex) const
{
	return !(eSex != SEX_XY && eSex != SEX_XX && eSex != SEX_UNK);
}
 
COWSEX CowSex::getCOWSEX() const
{
	return m_sex;
}

bool CowSex::IsXX() const
{
	return m_sex == SEX_XX;
}
	
bool CowSex::IsXY() const
{
	return m_sex == SEX_XY;
}
	
bool CowSex::IsUNK() const
{
	return m_sex == SEX_UNK;
}


bool operator>=(const CowSex& left,const CowSex& right)
{
	return left.m_sex >= right.m_sex;
}
bool operator<=(const CowSex& left,const CowSex& right)
{
	return left.m_sex <= right.m_sex;
}
bool operator>(const CowSex& left,const CowSex& right)
{
	return left.m_sex > right.m_sex;
}
bool operator<(const CowSex& left,const CowSex& right)
{
	return left.m_sex < right.m_sex;
}
bool operator==(const CowSex& left,const CowSex& right)
{
	return left.m_sex == right.m_sex;
}
