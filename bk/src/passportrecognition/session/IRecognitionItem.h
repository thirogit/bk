#ifndef __IRECOGNITIONITEM_H__
#define __IRECOGNITIONITEM_H__

#include "../../image/fipWinImageEx.h"
#include <data\runtime\ItemId.h>
#include <data\impl\CowDetails.h>

class IRecognitionItem
{
public:
	virtual ItemId GetId() const = 0;
	virtual CowDetails GetCow() const = 0;
	virtual bool HasImage() const = 0;
	virtual fipWinImageEx GetImage() const = 0;
};

#endif