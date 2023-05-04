#ifndef __IPRINTPROFILE_H__
#define __IPRINTPROFILE_H__

#include <stdint.h>
#include <string>
#include <map>
#include "../../arrays/Enumerator.h"
#include "PrintOption.h"
#include <CommonTypes.h>
#include "OrderSpecifier.h"

class IPrintProfile
{
public:	
	virtual const std::wstring& GetDocumentCode() const = 0;
	virtual uint32_t GetProfileId() const = 0;
	virtual const std::wstring& GetProfileName() const = 0;
	virtual PrintOption* GetOption(const std::wstring& optName) const = 0;
	virtual PtrEnumerator<PrintOption> EnumOptions() const = 0;
	virtual PtrEnumerator<OrderSpecifier> EnumOrderSpecifiers() const = 0;
	virtual count_t GetOptionsCount() const = 0;
	virtual count_t GetOrderSpecifiersCount() const = 0;
};

#endif