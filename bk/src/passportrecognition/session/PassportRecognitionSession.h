#ifndef __PASSPORTRECOGNITIONSESSION_H__
#define __PASSPORTRECOGNITIONSESSION_H__

#include "../../image/fipWinImageEx.h"
#include <data\runtime\ItemId.h>
#include <data\impl\CowDetails.h>
#include "../../context/Observerable.h"
#include <CommonTypes.h>
#include "../PassportImage.h"
#include "IRecognitionItem.h"

class PassportRecognitionObserver
{
public:
	virtual void OnItemUpdated(ItemId id) = 0;
	virtual void OnItemAdded(ItemId id) = 0;
};

class PassportRecognitionSession : public Observerable<PassportRecognitionObserver>
{
public:
	virtual ItemId AddCow(const CowDetails& details, PassportImagePtr image) = 0;
	virtual void UpdateCow(ItemId id, const CowDetails& details, PassportImagePtr image) = 0;
	virtual void UpdateCow(ItemId id,const CowDetails& details) = 0;
	virtual IRecognitionItem* FindItem(const CowNo& cowNo) = 0;
	virtual IRecognitionItem* GetItem(ItemId id) const = 0;
	virtual IRecognitionItem* GetItem(CowNo& cowno) const = 0;
	virtual IRecognitionItem* GetItemAt(int index) const = 0;
	virtual count_t GetItemCount() const = 0;
};

#endif