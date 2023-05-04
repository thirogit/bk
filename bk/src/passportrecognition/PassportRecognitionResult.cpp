#include "stdafx.h"
#include "PassportRecognitionResult.h"


PassportRecognitionResult::PassportRecognitionResult() 
{
}

PassportRecognitionResult::PassportRecognitionResult(const PassportRecognitionResult &src) 
{
	CopyFrom(src);
}

PassportRecognitionResult& PassportRecognitionResult::operator=(const PassportRecognitionResult &right)
{
	CopyFrom(right);
	return *this;
}

void PassportRecognitionResult::CopyFrom(const PassportRecognitionResult& src)
{
	m_passDate = src.m_passDate;
	m_passNo = src.m_passNo;
	m_cowNoBC = src.m_cowNoBC;
	m_cowNo = src.m_cowNo;
	m_farmNoBC = src.m_farmNoBC;
	m_birthDatePlace = src.m_birthDatePlace;
	m_cowSex = src.m_cowSex;
	m_cowStock = src.m_cowStock;
	m_motherNo = src.m_motherNo;
	m_fstOwner = src.m_fstOwner;
}


void PassportRecognitionResult::SetPassDate(const std::wstring& sValue)
{
	m_passDate = sValue;
}

void PassportRecognitionResult::SetPassNo(const std::wstring& sValue)
{
	m_passNo = sValue;
}

void PassportRecognitionResult::SetCowNoBC(const std::wstring& sValue)
{
	m_cowNoBC = sValue;
}

void PassportRecognitionResult::SetCowNo(const std::wstring& sValue)
{
	m_cowNo = sValue;
}

void PassportRecognitionResult::SetFarmNoBC(const std::wstring& sValue)
{
	m_farmNoBC = sValue;
}

void PassportRecognitionResult::SetBirthDatePlace(const std::wstring& sValue)
{
	m_birthDatePlace = sValue;
}

void PassportRecognitionResult::SetCowStock(const std::wstring& sValue)
{
	m_cowStock  = sValue;
}

void PassportRecognitionResult::SetMotherNo(const std::wstring& sValue)
{
	m_motherNo = sValue;
}

void PassportRecognitionResult::SetFstOwner(const std::wstring& sValue)
{
	m_fstOwner = sValue;
}

void PassportRecognitionResult::SetCowSex(const std::wstring& sValue)
{
	m_cowSex = sValue;
}

const std::wstring& PassportRecognitionResult::GetPassDate() const
{
	return m_passDate;
}

const std::wstring& PassportRecognitionResult::GetPassNo() const
{
	return m_passNo;
}

const std::wstring& PassportRecognitionResult::GetCowNoBC() const
{
	return m_cowNoBC;
}

const std::wstring& PassportRecognitionResult::GetCowNo() const
{
	return m_cowNo;
}

const std::wstring& PassportRecognitionResult::GetFarmNoBC() const
{
	return m_farmNoBC;
}

const std::wstring& PassportRecognitionResult::GetBirthDatePlace() const
{
	return m_birthDatePlace;
}

const std::wstring& PassportRecognitionResult::GetCowStock() const
{
	return m_cowStock;
}

const std::wstring& PassportRecognitionResult::GetMotherNo() const
{
	return m_motherNo;
}

const std::wstring& PassportRecognitionResult::GetFstOwner() const
{
	return m_fstOwner;
}

const std::wstring& PassportRecognitionResult::GetCowSex() const
{
	return m_cowSex;
}

 