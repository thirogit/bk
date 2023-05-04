#ifndef __POINT_H__
#define __POINT_H__

#include "Size.h"

template<typename pointT>
class Point
{
public:
	Point()
	{
	}

	Point(const pointT& initX,const pointT& initY) : x(initX),y(initY)
	{
	}
	
	Point(const Point<pointT>& src)
	{
		operator=(src);
	}
	
	Point<pointT>& operator=(const Point<pointT>& right)
	{
		x = right.x;
		y = right.y;
		return *this;
	}

	Point<pointT> operator+(const Size<pointT> &s) const
	{
		return Point<pointT>(x+s.cx,y+s.cy);
	}
	
   // friend Point<pointT> operator+(const Point<pointT> &p, const Size<pointT> &s);
    Point<pointT> operator*(double scale) const
	{
		return Point<pointT>(x*scale,y*scale);
	}
	
	pointT x;
	pointT y;
	
};
/*
template<typename pointT>
Point<pointT> operator+(const Point<pointT> &p, const Size<pointT> &s)
{
	return Point<pointT>(p.x+s.cx,p.y+s.cy);
}

template<typename pointT>
Point<pointT> operator*(const Point<pointT> &p, double scale)
{
	return Point<pointT>(p.x*scale,p.y*scale);
}
	
*/
#endif