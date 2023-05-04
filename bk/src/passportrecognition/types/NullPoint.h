#ifndef __NULLPOINT_H__
#define __NULLPOINT_H__

#include "Point.h"
#include <boost/optional.hpp>

template<typename pointT>
class NullPoint : public Nullable
{
public:
	NullPoint()
	{
	}
		
	NullPoint(const Point<pointT>& point)
	{
		m_point = point;
	}

	NullPoint(const pointT& initX,const pointT& initY)
	{
		m_point = Point<pointT>(initX, initY);
	}
	
	NullPoint(const NullPoint<pointT>& src)
	{
		operator=(src);
	}
	
	NullPoint<pointT>& operator=(const NullPoint<pointT>& right)
	{
		m_point = right.m_point;
		return *this;
	}

	NullPoint<pointT>& operator=(const Point<pointT>& right)
	{
		m_point = right;
		return *this;
	}

	virtual void MakeNull()
	{
		m_point.reset();
	}

	virtual bool IsNull() const
	{
		return !m_point.is_initialized();
	}

	pointT x(pointT ifNull) const
	{
		if (IsNull()) return ifNull;
		return m_point.get().x;
	}

	pointT y(pointT ifNull) const
	{
		if (IsNull()) return ifNull;
		return m_point.get().y;
	}

private:
	boost::optional<Point<pointT>> m_point;	
	
};

#endif