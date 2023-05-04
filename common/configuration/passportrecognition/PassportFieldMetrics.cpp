#include "stdafx.h"
#include "PassportFieldMetrics.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

PassportFieldMetrics::PassportFieldMetrics()
{

}

PassportFieldMetrics::PassportFieldMetrics(int x,int y,int w,int h)
{
	m_xyOrd.set(x,y);
	m_sSize.set(w,h);
}

PassportFieldMetrics::PassportFieldMetrics(const IntPoint &ord,const IntSize &size) : m_xyOrd(ord),m_sSize(size)
{
}

PassportFieldMetrics::PassportFieldMetrics(const PassportFieldMetrics &copy)
{
	operator=(copy);
}

PassportFieldMetrics& PassportFieldMetrics::operator=(const PassportFieldMetrics &right)
{
	m_sSize = right.m_sSize;
	m_xyOrd = right.m_xyOrd;
	return *this;
}

int PassportFieldMetrics::GetW() const
{
	return m_sSize.cx;
}

int PassportFieldMetrics::GetH() const
{
	return m_sSize.cy;
}

int PassportFieldMetrics::GetX() const
{
	return m_xyOrd.x;
}

int PassportFieldMetrics::GetY() const
{
	return m_xyOrd.y;
}

const IntPoint& PassportFieldMetrics::GetOrd() const
{
	return m_xyOrd;
}

const IntSize& PassportFieldMetrics::GetSize() const
{
	return m_sSize;
}

void PassportFieldMetrics::SetW(int w)
{
	m_sSize.cx = w;
}

void PassportFieldMetrics::SetH(int h)
{
	m_sSize.cy = h;
}

void PassportFieldMetrics::SetX(int x)
{
	m_xyOrd.x = x;
}

void PassportFieldMetrics::SetY(int y)
{
	m_xyOrd.y = y;
}

void PassportFieldMetrics::SetOrd(const IntPoint& ord)
{
	m_xyOrd = ord;
}

void PassportFieldMetrics::SetSize(const IntSize& size)
{
	m_sSize = size;
}

void PassportFieldMetrics::SetMetric(FIELD_METRIC metric, int value)
{
	switch (metric)
	{
	case Y_METRIC:
		SetY(value);
		break;
	case X_METRIC:
		SetX(value);
		break;
	case W_METRIC:
		SetW(value);
		break;
	case H_METRIC:
		SetH(value);
		break;
	}
}

int PassportFieldMetrics::GetMetric(FIELD_METRIC metric) const
{
	switch (metric)
	{
	case Y_METRIC:
		return GetY();
	case X_METRIC:
		return GetX();
	case W_METRIC:
		return GetW();
	case H_METRIC:
		return GetH();
	}

	return -1;	
}