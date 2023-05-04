#ifndef __DEFAULTVALUESAPPLIER_H__
#define __DEFAULTVALUESAPPLIER_H__

#include "CowEditor.h"
#include "InvoiceTransactionFacade.h"
#include "../SeasonSession.h"
#include "../../settings/DefaultValues.h"

class DefaultValuesApplier 
{
public:
	DefaultValuesApplier(SeasonSession* session);	
	DefaultValuesApplier(const DefaultValues& values);	
	
	void ApplyTo(CowEditor* editor) const;
	void ApplyTo(InvoiceTransactionFacade* transaction) const;
	
private:

	DefaultValues m_values;
	
	
};

#endif