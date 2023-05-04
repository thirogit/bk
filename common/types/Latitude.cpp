#include "stdafx.h"
#include "Latitude.h"
#include <math.h>

Latitude::Latitude()
{
}

Latitude::Latitude(double dLatitudeAngle) : GeoCoordinate(dLatitudeAngle)
{
	
}

Latitude::Latitude(const Latitude& copy) 
{
	CopyFrom(copy);
}

Latitude::Latitude(const NullDouble& copy)
{
	Latitude::CopyFrom(copy);
}

Hemisphere Latitude::GetHemisphere() const
{
	if(IsNull())
		return NullHemisphere();

	if(GetAngle() < 0)
		return S();
	else
		return N();
}


double Latitude::GetMinAngle()
{
	return -90.0;
}
double Latitude::GetMaxAngle()
{
	return 90.0;
}

Latitude Latitude::FromDouble(const NullDouble& d)
{
	return Latitude(d);
}


bool operator>=(const Latitude& left, const Latitude& right)
{
	if(left.IsNull() && right.IsNull()) return true;
	if(left.IsNull() || right.IsNull()) return false;

	return	left.GetAngle()   >= right.GetAngle();
	
}

bool operator<=(const Latitude& left, const Latitude& right)
{
	if(left.IsNull() && right.IsNull()) return true;
	if(left.IsNull() || right.IsNull()) return false;

	return left.GetAngle() <= right.GetAngle();
	
}

bool operator>(const Latitude& left, const Latitude& right)
{
	if(left.IsNull())
	{
		return false;
	}
	else
		if(right.IsNull()) return true;

	return left.GetAngle() > right.GetAngle();
		
}

bool operator<(const Latitude& left, const Latitude& right)
{
	return (right > left);
}


bool operator==(const Latitude& left, const Latitude& right)
{
	if(left.IsNull() && right.IsNull()) return true;
	if(right.IsNull() || left.IsNull()) return false;

	return left.Equals(right);
}

bool operator!=(const Latitude& left, const Latitude& right)
{
	return !(left == right);
}
