#pragma once
#include "..\ctrls\contentlistctrl\ListContent.h"
#include "../content/InvoiceRow.h"
#include "InvoiceRowsIndex.h"
#include "../view/InvoiceViewCreator.h"
#include "../context/SeasonSession.h"

class SimpleInvoicesListContent : public ListContent
{
	typedef InvoiceRowsIndex<InvoiceRow> RowIndex;	
	
public:

	typedef RowIndex::IndexType::iterator iterator;

	SimpleInvoicesListContent(SeasonSession* session);
	~SimpleInvoicesListContent();
	
	virtual int GetRowCount() const;
	virtual ListRow* GetRow(int row);

	virtual void SortContent(const SortAttributes& attributes);
			
	void AddRow(IInvoice* invoice);
	void RemoveRow(uint32_t invoiceId);
	void RemoveAll();
	void UpdateRow(IInvoice* invoice);
	
	InvoiceRow* GetInvoiceRowAt(int row);	
	InvoiceRow* FindRow(uint32_t invoiceId);


	iterator begin();

	iterator end();
	
protected:
	RowIndex m_Rows;	
	InvoiceViewCreator m_viewCreator;
};


