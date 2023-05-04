#ifndef __SELLDOCSELECTINTERFACE_H__
#define __SELLDOCSELECTINTERFACE_H__

#include "content\SellDocsCandidatesListContent.h"
#include "../doc/DocSelectInterface.h"

class SellDocKamilWSelectInterface : public DocSelectInterface
{
public:
	void Create(SeasonSession* pSession,
				InvoiceDocsTransactionPtr transaction);

	void Destroy();

	virtual ListContent* GetCandidates();
	virtual void PickRow(int row);
	virtual void UnpickRow(int row);
	virtual bool IsRowPicked(int row);
	virtual size_t GetPickedCount() const;
private:
	SellDocsCandidatesListContent m_candidates;
	InvoiceDocsTransactionPtr m_transaction;
	std::set<uint32_t> m_picked;
};

#endif