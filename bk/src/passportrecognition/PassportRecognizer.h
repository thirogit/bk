#ifndef __PASSPORTRECOGNIZER_H__
#define __PASSPORTRECOGNIZER_H__

#include "../image/fipWinImageEx.h"
#include "engine/Recognizer.h"
#include "settings/ImageSettings.h"
#include <configuration\passportrecognition\PassportRecognitionProfile.h>
#include "PassportImage.h"

#define _USE_MATH_DEFINES
#include <math.h>

class PassportRecognitionResult;



class PassportRecognizer
{
public:
	PassportRecognizer(Recognizer* pRecognizer);
    ~PassportRecognizer();
	
	PassportRecognitionResult Recognize(PassportImagePtr ptrImage,	IPassportRecognitionProfile* passProfile);

protected:
	std::wstring RecognizeField(const PassportImage* pImage,
								const std::wstring& fieldName,
								const IPassportField* passField);

	std::wstring RecognizeBarcodeField(fipWinImage& fieldImg);
	std::wstring RecognizeTextField(fipWinImage& fieldImg);

	void CutFieldFromImage(IPassportField* passField,fipWinImage& outFieldImg);		

	void SaveDebugImage(const std::wstring& imgFileName, fipImage &image);

	Recognizer* m_pRecognizer;
};


#endif