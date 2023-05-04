#ifndef __LOGO_H__
#define __LOGO_H__

#include <boost/shared_ptr.hpp>
#include "FreeImagePlus.h"

class LogoService;

class Logo
{
public:
	Logo();
	Logo(const Logo& src);
	Logo& operator=(const Logo& right);
	
	void Draw(HDC hDC, RECT& rcDest);
	const fipImage* GetImage() const;
	
private:
	boost::shared_ptr<fipImage> m_logoImgPtr;
	
	friend class LogoService;

};






#endif