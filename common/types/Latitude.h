#ifndef __LATITUDE_H__
#define __LATITUDE_H__

#include "GeoCoordinate.h"

class Latitude : public GeoCoordinate
{
public:
	Latitude();
	Latitude(const Latitude& copy);
	Latitude(const NullDouble& copy);
	explicit Latitude(double dLatitudeAngle);
	virtual Hemisphere GetHemisphere() const;	
	virtual double GetMinAngle();
	virtual double GetMaxAngle();	
	static Latitude FromDouble(const NullDouble& d);
	
};


bool operator>=(const Latitude& left, const Latitude& right);
bool operator<=(const Latitude& left, const Latitude& right);
bool operator>(const Latitude& left, const Latitude& right);
bool operator<(const Latitude& left, const Latitude& right);
bool operator==(const Latitude& left, const Latitude& right);
bool operator!=(const Latitude& left, const Latitude& right);

#endif
