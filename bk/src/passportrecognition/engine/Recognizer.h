#ifndef __RECOGNIZER_H__
#define __RECOGNIZER_H__

#include <FreeImagePlus.h>
#include <string>

class Recognizer
{
public:
	virtual std::wstring RecognizeText(const fipWinImage& img) = 0;
	virtual std::wstring RecognizeBarcode(const fipWinImage& img) = 0;
};



#endif 
