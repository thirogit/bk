#ifndef __LONGITUDEEDIT_H__
#define __LONGITUDEEDIT_H__

#include "GeoCoordinateEdit.h"
#include <types/Longitude.h>

class CLongitudeEdit : public CGeoCoordinateEdit
{
public:
	CLongitudeEdit();
	void SetLongitude(const  Longitude& longitude);
	Longitude GetLongitude() const;
	
protected:
	bool IsValidGeoCoordinate();
	virtual std::wstring GetValidationErrorMessage() const;
	virtual std::wstring GetGeoCoordHint() const;

	
};


#endif