#ifndef __PLUSDOCTRANSACTIONFACADE_H__
#define __PLUSDOCTRANSACTIONFACADE_H__

#include "DocTransactionFacade.h"

//class IPassportUploadSession
//{
//public:
//	virtual void End() = 0;
//	virtual void UploadChunk(const PassportChunk& chunk) = 0;
//};

class PlusDocTransactionFacade : public DocTransactionFacade
{
public:
	virtual void UpdateCow(ItemId id,const CowDetails& details) = 0;	
	virtual ItemId PlusCow(const CowDetails& details) = 0;
	//virtual IPassportUploadSession* StartPassportImgUpload() = 0;
};

#endif