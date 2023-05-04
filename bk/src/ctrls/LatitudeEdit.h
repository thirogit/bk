#ifndef __LATITUDEEDIT_H__
#define __LATITUDEEDIT_H__

#include "GeoCoordinateEdit.h"
#include <types/Latitude.h>

class CLatitudeEdit : public CGeoCoordinateEdit
{
public:
	CLatitudeEdit();
	void SetLatitude(const  Latitude& latitude);
	Latitude GetLatitude() const;
protected:
	bool IsValidGeoCoordinate();
	virtual std::wstring GetValidationErrorMessage() const;
	virtual std::wstring GetGeoCoordHint() const;

	
};


#endif