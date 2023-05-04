#include "stdafx.h"
#include "LatitudeEdit.h"
#include <types/Latitude.h>
#include <boost/format.hpp>
#include "../utils/StringLoader.h"
#include "../resource.h"

CLatitudeEdit::CLatitudeEdit()
{
}
bool CLatitudeEdit::IsValidGeoCoordinate()
{
	if(IsInputEmpty())
		return true;

	if(CGeoCoordinateEdit::IsValidGeoCoordinate())
	{
		Latitude latitue;
		unsigned short degrees;
		unsigned short minutes;
		unsigned short seconds;
		Hemisphere hemisphere = NullHemisphere();
		if(GetGeoCoordinate(degrees,minutes,seconds,hemisphere))
		{
			return latitue.Set(degrees,minutes,seconds,hemisphere);
		}
	}
	return false;
}
void CLatitudeEdit::SetLatitude(const  Latitude& latitude)
{
	if(latitude.IsNull())
	{
		EmptyData(TRUE);
	}
	else
	{
		SetGeoCoordinate(latitude);
	}
}
Latitude CLatitudeEdit::GetLatitude() const
{
	Latitude latitude;
	if(!IsInputEmpty() && IsFullPrompt())
	{	
		unsigned short degrees;
		unsigned short minutes;
		unsigned short seconds;
		Hemisphere hemisphere = NullHemisphere();
		if(GetGeoCoordinate(degrees,minutes,seconds,hemisphere))
		{
			latitude.Set(degrees,minutes,seconds,hemisphere);
		}
	}
	return latitude;
}

std::wstring CLatitudeEdit::GetValidationErrorMessage() const
{
	Latitude minLat,maxLat;
	minLat.SetAngle(minLat.GetMinAngle());
	maxLat.SetAngle(maxLat.GetMaxAngle());

	return (boost::wformat(StringLoader(IDS_MINMAX_VALIDATION_MSG)) % minLat.ToString() % maxLat.ToString()).str();

}
std::wstring CLatitudeEdit::GetGeoCoordHint() const
{
	return L"90N/S";
}
