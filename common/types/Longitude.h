#ifndef __LONGITUDE_H__
#define __LONGITUDE_H__

#include "GeoCoordinate.h"

class Longitude : public GeoCoordinate
{
public:
	Longitude();
	Longitude(const Longitude& copy);
	Longitude(const NullDouble& copy);
	explicit Longitude(double dLongitudeAngle);
	virtual Hemisphere GetHemisphere() const;
	virtual double GetMinAngle();
	virtual double GetMaxAngle();
	static Longitude FromDouble(const NullDouble& d);
};



bool operator>=(const Longitude& left, const Longitude& right);
bool operator<=(const Longitude& left, const Longitude& right);
bool operator>(const Longitude& left, const Longitude& right);
bool operator<(const Longitude& left, const Longitude& right);
bool operator==(const Longitude& left, const Longitude& right);
bool operator!=(const Longitude& left, const Longitude& right);



#endif
