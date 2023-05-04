#ifndef __DOCUMENTINTERFACE_H__
#define __DOCUMENTINTERFACE_H__

#include "DocumentTerritory.h"
#include "../bkprint/renderer/PrintOut.h"
#include "context/DocumentContext.h"
#include <configuration\print\IPrintProfile.h>

class DocumentInterface
{
public:
	virtual bool QueryInputInterface(const type_info& interfase_type,void** interfase) = 0;	
	virtual void SetContext(DocumentContext* context) {};	
	virtual bkprint::PrintOut Print(IPrintProfile* profile) = 0;
	virtual ~DocumentInterface() = 0;
};






#endif