#ifndef __DOCUMENTINSTANCE_H__
#define __DOCUMENTINSTANCE_H__

#include <stdint.h>
#include <string>
#include <vector>
#include "../bkprint/renderer/PrintOut.h"
#include "options\DocumentCfgDescription.h"
#include <configuration\print\IPrintProfile.h>

class DocumentInstance
{
public:
	virtual bkprint::PrintOut Print(IPrintProfile* profile) = 0;
	virtual const std::wstring& GetName() const = 0;
	virtual const std::wstring& GetCode() const = 0;
	virtual ~DocumentInstance() = 0;
	virtual const DocumentCfgDescription* DescribeConfiguration() = 0;	
};



#endif
