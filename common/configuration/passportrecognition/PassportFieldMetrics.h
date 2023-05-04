#ifndef __PASSPORTFIELDMETRICS_H__
#define __PASSPORTFIELDMETRICS_H__

#include "Point.h"

typedef Point<int> IntPoint;
typedef Size<int> IntSize;

enum FIELD_METRIC
{
	X_METRIC = 'x',
	Y_METRIC = 'y',
	W_METRIC = 'w',
	H_METRIC = 'h'
};

class PassportFieldMetrics
{
public:
	PassportFieldMetrics();
	PassportFieldMetrics(int x,int y,int w,int h);
	PassportFieldMetrics(const IntPoint &ord, const IntSize &size);
	PassportFieldMetrics(const PassportFieldMetrics &copy);
	PassportFieldMetrics& operator=(const PassportFieldMetrics &right);

	int GetW() const;
	int GetH() const;
	int GetX() const;
	int GetY() const;

	const IntPoint& GetOrd() const;
	const IntSize& GetSize() const;

	void SetW(int w);
	void SetH(int h);
	void SetX(int x);
	void SetY(int y);

	void SetMetric(FIELD_METRIC metric, int value);
	int GetMetric(FIELD_METRIC metric) const;
	
	void SetOrd(const IntPoint& ord);
	void SetSize(const IntSize& size);
	
public:
	IntPoint m_xyOrd; //in mm
	IntSize  m_sSize; //in mm

};

#endif 