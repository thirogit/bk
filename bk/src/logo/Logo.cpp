#include "stdafx.h"
#include "Logo.h"


Logo::Logo()
{

}

Logo::Logo(const Logo& src)
{
	m_logoImgPtr = src.m_logoImgPtr;	
}

Logo& Logo::operator=(const Logo& right)
{
	m_logoImgPtr = right.m_logoImgPtr;
	return *this;
}
	
void Logo::Draw(HDC hDC, RECT& rcDest)
{

}

const fipImage* Logo::GetImage() const
{
	return m_logoImgPtr.get();
}
	
