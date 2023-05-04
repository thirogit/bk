#pragma once

#include <boost/shared_ptr.hpp>
#include <SF/Archive.hpp>
#include "../../CommonTypes.h"

class FetchManifest
{
public:
	FetchManifest();
	FetchManifest(const FetchManifest& src);
	void CopyFrom(const FetchManifest& src);
	FetchManifest& operator=(const FetchManifest& src);

	count_t HentCount() const;
	count_t& HentCount();

	count_t CowCount() const;
	count_t& CowCount();

	count_t InDocCount() const;
	count_t& InDocCount();

	count_t BuyDocCount() const;
	count_t& BuyDocCount();

	count_t MoveDocCount() const;
	count_t& MoveDocCount();

	count_t SellDocCount() const;
	count_t& SellDocCount();

	count_t OutDocCount() const;
	count_t& OutDocCount();
	
	count_t BuyInvoicesCount() const;
	count_t& BuyInvoicesDocCount();
	
	count_t SellInvoicesCount() const;
	count_t& SellInvoicesDocCount();
	
	void serialize(SF::Archive &archive);

private:
	count_t m_hent;
	count_t m_cow;
	count_t m_indoc;
	count_t m_buydoc;
	count_t m_movedoc;
	count_t m_selldoc;
	count_t m_outdoc;
	count_t m_buyinvoice;
	count_t m_sellinvoice;
};

typedef boost::shared_ptr<FetchManifest> FetchManifestObj;
