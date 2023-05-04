#ifndef __FATHENTSDOCVIEW_H__
#define __FATHENTSDOCVIEW_H__

#include "FatDocView.h"
#include "HentsDocView.h"

class FatHentsDocView : public FatDocView<HentsDocView>
{
public:
	FatHentsDocView(uint32_t docId) : FatDocView<HentsDocView>(docId)
	{	
	}

	FatHentsDocView(const FatDocView<HentsDocView>& src) : FatDocView<HentsDocView>(src)
	{	
		
	}

	~FatHentsDocView()
	{
		
	}
		
	
private:
	HentView* m_InvoiceHent;
	
};

#endif