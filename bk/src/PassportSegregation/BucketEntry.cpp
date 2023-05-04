#include "stdafx.h"
#include "BucketEntry.h"


BucketEntry::BucketEntry(const CowPassport& passport)  : m_passport(passport),m_found(false)
{
}

BucketEntry::BucketEntry(const BucketEntry& src)
{
	operator=(src);
}

BucketEntry& BucketEntry::operator=(const BucketEntry& right)
{
	m_passport = right.m_passport;
	m_found = right.m_found;
	return *this;
}

const CowPassport& BucketEntry::GetPassport() const
{
	return m_passport;
}

bool BucketEntry::IsFound() const
{
	return m_found;
}

void BucketEntry::SetFound(bool bFound)
{
	m_found = bFound;
}

