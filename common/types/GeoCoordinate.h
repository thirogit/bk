#ifndef __GEOCOORDINATE_H__
#define __GEOCOORDINATE_H__

#include "NullDouble.h"


class Hemisphere
{
public:
	Hemisphere(int sign,wchar_t suffix);
	Hemisphere(const Hemisphere& copy);
	Hemisphere& operator=(const Hemisphere& right);
	void CopyFrom(const Hemisphere& right);
	int GetSign() const;
	wchar_t GetSuffix() const;
private:
	int m_sign;
	wchar_t m_suffix;
};


class HemisphereFactory
{
public:
	static Hemisphere GetHemisphere(wchar_t cHemisphere);
};

class N : public Hemisphere
{
public:
	N();
};

class S : public Hemisphere
{
public:
	S();
};

class W : public Hemisphere
{
public:
	W();
};

class E : public Hemisphere
{
public:
	E();
};

class NullHemisphere : public Hemisphere
{
public:
	NullHemisphere();
};


double operator*(const Hemisphere& hemi,double angle);
double operator*(double angle,const Hemisphere& hemi);

class GeoCoordinate : public NullDouble
{
public:
	GeoCoordinate();
	GeoCoordinate(double dAngle);
	GeoCoordinate(const GeoCoordinate& copy);
	virtual void CopyFrom(const NullDouble& copy);

	unsigned short GetDegrees() const;
	unsigned short GetMinutes() const;
	unsigned short GetSeconds() const;
	virtual Hemisphere GetHemisphere() const = 0;

	bool	Set(unsigned short degrees,unsigned short minutes,unsigned short seconds,const Hemisphere& hemisphere); 
	void	SetAngle(double dAngle);
	double  GetAngle(double dIfNull = 0.0) const;

	virtual std::wstring ToString(unsigned int precision = NULLDOUBLE_DEFAULT_PREC) const;
	
	virtual double GetMinAngle() = 0;
	virtual double GetMaxAngle() = 0;

	bool Equals(const GeoCoordinate& geoCoord) const;

	NullDouble AsDouble() const;
	
protected:
	bool Validate();
	
};




#endif
