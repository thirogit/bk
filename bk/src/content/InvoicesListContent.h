#pragma once
#include "..\ctrls\contentlistctrl\ListContent.h"
#include "../context/SeasonSession.h"
#include "../context/ForwardSeasonObserver.h"
#include "../content/InvoiceRow.h"
#include "InvoiceRowsIndex.h"
#include <data\InvoiceKind.h>

class InvoicesListContent : public ListContent,public ForwardSeasonObserver
{
	typedef InvoiceRowsIndex<InvoiceRow> RowIndex;	
	
public:
	InvoicesListContent();
	~InvoicesListContent();
	virtual int GetRowCount() const;
	virtual ListRow* GetRow(int row);	
	InvoiceRow* GetInvoiceRowAt(int row);	
	InvoiceRow* GetInvoiceRowWithId(uint32_t id);
	virtual void SortContent(const SortAttributes& attributes);
	void Create(SeasonSession* pSession,const InvoiceKind& kind);
	void Destroy();	
	InvoiceType GetInvoiceType() const;
	const InvoiceKind& GetInvoiceKind() const;
protected:
	bool IsValidForThisContent(IInvoice* invoice);
	void AddRow(InvoiceRow* row);
	void RemoveRow(uint32_t invoiceId);
	InvoiceRow* FindRow(uint32_t invoiceId);
	virtual void CreateContent() = 0;
	
protected:
	RowIndex m_Rows;
	SeasonSession* m_pSession;
	InvoiceKind m_kind;
	
};


