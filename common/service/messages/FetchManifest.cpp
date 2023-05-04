#include "stdafx.h"
#include "FetchManifest.h"

FetchManifest::FetchManifest()
{
	m_hent = 0;
	m_cow = 0;
	m_indoc = 0;
	m_buydoc = 0;
	m_movedoc = 0;
	m_selldoc = 0;
	m_outdoc = 0;
	m_buyinvoice = 0;
	m_sellinvoice = 0;
}

FetchManifest::FetchManifest(const FetchManifest& src)
{
	CopyFrom(src);
}

void FetchManifest::CopyFrom(const FetchManifest& src)
{
	m_hent = src.m_hent;
	m_cow = src.m_cow;
	m_indoc = src.m_indoc;
	m_buydoc = src.m_buydoc;
	m_movedoc = src.m_movedoc;
	m_selldoc = src.m_selldoc;
	m_outdoc = src.m_outdoc;
	m_buyinvoice = src.m_buyinvoice;
	m_sellinvoice = src.m_sellinvoice;
}

FetchManifest& FetchManifest::operator=(const FetchManifest& src)
{
	CopyFrom(src);
	return *this;
}

count_t FetchManifest::HentCount() const
{
return m_hent;
}

count_t& FetchManifest::HentCount()
{	
	return m_hent;
}

count_t FetchManifest::CowCount() const
{
	return m_cow;
}

count_t& FetchManifest::CowCount()
{
	return m_cow;
}


count_t FetchManifest::InDocCount() const
{
	return m_indoc;
}

count_t& FetchManifest::InDocCount()
{
	return m_indoc;
}


count_t FetchManifest::BuyDocCount() const
{
	return m_buydoc;
}

count_t& FetchManifest::BuyDocCount()
{
	return m_buydoc;
}


count_t FetchManifest::MoveDocCount() const
{
	return m_movedoc;
}

count_t& FetchManifest::MoveDocCount()
{
	return m_movedoc;
}

count_t FetchManifest::SellDocCount() const
{
	return m_selldoc;
}

count_t& FetchManifest::SellDocCount()
{
	return m_selldoc;
}

count_t FetchManifest::OutDocCount() const
{
	return m_outdoc;
}

count_t& FetchManifest::OutDocCount()
{
	return m_outdoc;
}
	
count_t FetchManifest::BuyInvoicesCount() const
{
	return m_buyinvoice;
}

count_t& FetchManifest::BuyInvoicesDocCount()
{
	return m_buyinvoice;
}

count_t FetchManifest::SellInvoicesCount() const
{
	return m_sellinvoice;
}

count_t& FetchManifest::SellInvoicesDocCount()
{
	return m_sellinvoice;
}

void FetchManifest::serialize(SF::Archive &archive)
{
	archive& m_hent;
	archive& m_cow;
	archive& m_indoc;
	archive& m_buydoc;
	archive& m_movedoc;
	archive& m_selldoc;
	archive& m_outdoc;
	archive& m_buyinvoice;
	archive& m_sellinvoice;
}