#ifndef __BASEDAO_H__
#define __BASEDAO_H__

#include "../db/Transaction.h"

class BaseDAO 
{
public:
	Transaction OpenTransaction();
	uint32_t NextSeqValue(Transaction transaction, const std::wstring& sSequenceName);
};


#endif