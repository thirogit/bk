#ifndef __SIZE_H__
#define __SIZE_H__

template<typename sizeT>
class Size
{
public:

	Size()
	{
	}

	Size(const sizeT& initCX,const sizeT& initCY) : cx(initCX),cy(initCY)
	{
	}
	
	Size(const Size<sizeT>& src)
	{
		operator=(src);
	}
	
	Size<sizeT>& operator=(const Size<sizeT>& right)
	{
		cx = right.cx;
		cy = right.cy;

		return *this;
	}
	
	Size<sizeT> operator*(double scale) const
	{
		return Size<sizeT>(cx*scale,cy*scale);
	}

	void set(int sizex, int sizey)
	{
		cx = sizex;
		cy = sizey;
	}

	sizeT cx;
	sizeT cy;

	
	
};
#endif