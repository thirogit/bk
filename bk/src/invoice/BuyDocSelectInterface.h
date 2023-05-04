#ifndef __BUYDOCSELECTINTERFACE_H__
#define __BUYDOCSELECTINTERFACE_H__

#include "content\BuyDocsCandidatesListContent.h"
#include "../doc/DocSelectInterface.h"

class BuyDocSelectInterface : public DocSelectInterface
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
	BuyDocsCandidatesListContent m_candidates;
	InvoiceDocsTransactionPtr m_transaction;
	std::set<uint32_t> m_picked;
};

#endif