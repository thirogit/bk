#ifndef __PASSPORTIMAGE_H__
#define __PASSPORTIMAGE_H__

#include "../image/fipWinImageEx.h"
#include "types\NullPoint.h"
#include <types/NullInt.h>
#include <types/NullDouble.h>
#include <boost/shared_ptr.hpp>

class PassportImageOperation;

class PassportImageObject
{
public:
	fipWinImageEx m_RawPassportImage;
	fipWinImageEx m_PreparedPassportImage;
	
	NullDouble m_Skew;
	NullPoint<int> m_Org;
	NullInt m_dpiResolution;
};

class PassportImage
{
public:
	PassportImage();
	void Perform(PassportImageOperation* operation);
	const fipWinImageEx& GetRawPassportImage() const;
	const fipWinImageEx& GetPreparedPassportImage() const;
	const NullInt& GetResolution() const;
	const NullDouble& GetSkew() const;
	const NullPoint<int>& GetOrigin() const;
private:
	PassportImageObject m_object;

};

typedef boost::shared_ptr<PassportImage> PassportImagePtr;

#endif 
