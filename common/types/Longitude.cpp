#include "stdafx.h"
#include "Longitude.h"
#include <math.h>

Longitude::Longitude()
{
}

Longitude::Longitude(double dLongitudeAngle) : GeoCoordinate(dLongitudeAngle)
{
	
}

Longitude::Longitude(const Longitude& copy) 
{
	CopyFrom(copy);
}

Longitude::Longitude(const NullDouble& copy)
{
	CopyFrom(copy);
}

double Longitude::GetMinAngle()
{
	return -180.0;
}
double Longitude::GetMaxAngle()
{
	return 180.0;
}

Hemisphere Longitude::GetHemisphere() const
{
	if(IsNull())
		return NullHemisphere();

	if(GetAngle() < 0)
		return W();
	else
		return E();
}

Longitude Longitude::FromDouble(const NullDouble& d)
{
	return Longitude(d);
}


bool operator>=(const Longitude& left, const Longitude& right)
{
	if(left.IsNull() && right.IsNull()) return true;
	if(left.IsNull() || right.IsNull()) return false;

	return	left.GetAngle()   >= right.GetAngle();
	
}

bool operator<=(const Longitude& left, const Longitude& right)
{
	if(left.IsNull() && right.IsNull()) return true;
	if(left.IsNull() || right.IsNull()) return false;

	return left.GetAngle() <= right.GetAngle();
	
}

bool operator>(const Longitude& left, const Longitude& right)
{
	if(left.IsNull())
	{
		return false;
	}
	else
		if(right.IsNull()) return true;

	return left.GetAngle() > right.GetAngle();
		
}

bool operator<(const Longitude& left, const Longitude& right)
{
	return (right > left);
}


bool operator==(const Longitude& left, const Longitude& right)
{
	if(left.IsNull() && right.IsNull()) return true;
	if(right.IsNull() || left.IsNull()) return false;

	return left.GetAngle() == right.GetAngle();
}

bool operator!=(const Longitude& left, const Longitude& right)
{
	return !(left == right);
}

