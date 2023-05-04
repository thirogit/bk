#include "stdafx.h"
#include "LongitudeEdit.h"
#include <types/Longitude.h>
#include <boost/format.hpp>
#include "../utils/StringLoader.h"
#include "../resource.h"

CLongitudeEdit::CLongitudeEdit()
{
}
bool CLongitudeEdit::IsValidGeoCoordinate()
{
	if(IsInputEmpty())
		return true;

	if(CGeoCoordinateEdit::IsValidGeoCoordinate())
	{
		Longitude longitude;
		unsigned short degrees;
		unsigned short minutes;
		unsigned short seconds;
		Hemisphere hemisphere = NullHemisphere();
		if(GetGeoCoordinate(degrees,minutes,seconds,hemisphere))
		{
			return longitude.Set(degrees,minutes,seconds,hemisphere);
		}
	}
	return false;
}

void CLongitudeEdit::SetLongitude(const  Longitude& longitude)
{
	if(longitude.IsNull())
	{
		EmptyData(TRUE);
	}
	else
	{
		SetGeoCoordinate(longitude);
	}
}

Longitude CLongitudeEdit::GetLongitude() const
{
	Longitude longitude;
	if(!IsInputEmpty() && IsFullPrompt())
	{	
		unsigned short degrees;
		unsigned short minutes;
		unsigned short seconds;
		Hemisphere hemisphere = NullHemisphere();
		if(GetGeoCoordinate(degrees,minutes,seconds,hemisphere))
		{
			longitude.Set(degrees,minutes,seconds,hemisphere);
		}
	}
	return longitude;
}

std::wstring CLongitudeEdit::GetValidationErrorMessage() const
{
	Longitude minLong,maxLong;
	minLong.SetAngle(minLong.GetMinAngle());
	maxLong.SetAngle(maxLong.GetMaxAngle());


	return (boost::wformat(StringLoader(IDS_MINMAX_VALIDATION_MSG)) %  minLong.ToString() % maxLong.ToString()).str();

}

std::wstring CLongitudeEdit::GetGeoCoordHint() const
{
	return L"180W/E";
}
