#include "stdafx.h"

#include "PassportRecognizer.h"

#include <filesystem/paths.h>
#include <format/StringFormat.h>
#include <types/DateTime.h>
#include "PassportRecognitionResult.h"
#include "PassportImage.h"
#include <boost/assign/list_of.hpp> 
#include <list>
#include "PassportRecognitionFieldsConstructor.h"
#include <errortoken/ErrorToken.h>
#include <configuration/passportrecognition/PassportFields.h>
#include <string\TextUtils.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if _DEBUG
#define SAVEDEBUGIMGS
#endif

PassportRecognizer::PassportRecognizer(Recognizer* pRecognizer) : m_pRecognizer(pRecognizer)
{	
}

PassportRecognizer::~PassportRecognizer() 
{	
}

PassportRecognitionResult PassportRecognizer::Recognize(PassportImagePtr ptrImage, IPassportRecognitionProfile* passProfile)
{	
	PassportImage* pContext = ptrImage.get();
	
	PassportRecognitionResult result;

	PassportRecognitionFieldsConstructor fieldsConstructor(&result);
	
	PassportFieldDescriptor* pField = NULL;

	PassportFields allFields;
	PtrEnumerator<PassportFieldDescriptor> fieldEnum = allFields.EnumFields();
	while (fieldEnum.hasNext())
	{
		pField = *fieldEnum;
		fieldsConstructor.SetField(pField,RecognizeField(pContext,
														pField->GetFieldName().c_str(),
														passProfile->GetFieldByName(pField->GetFieldName())));
		fieldEnum.advance();
	}	
		
	return result;
}


void PassportRecognizer::SaveDebugImage(const std::wstring& imgFileName,fipImage &image)
{
#ifdef SAVEDEBUGIMGS
	std::wstring imgPath = paths_ConcatPath(paths_GetApplicationDir(),L"OCRIMGDBG");
	paths_EnsurePathExist(imgPath);
	std::wstring imgFilePath = paths_ConcatPath(imgPath, TextUtils::Format(L"%s.BMP", imgFileName));
	image.saveU(imgFilePath.c_str());
#endif
}


std::wstring PassportRecognizer::RecognizeField(const PassportImage* pImg,
												 const std::wstring& fieldName, 
												 const IPassportField* passField)
{
	fipWinImage fieldImg;

	int dpiResolution = pImg->GetResolution().ToInt();
	NullPoint<int> origin = pImg->GetOrigin();
	int originX = origin.x(0), originY = origin.y(0);

	PassportFieldMetrics metrics = passField->GetFieldMetrics();
	const PassportFieldDescriptor* descriptor = passField->GetDescriptor();

	int l = originX + (metrics.GetX()*dpiResolution) / 254;
	int t = originY + (metrics.GetY()*dpiResolution) / 254;
	int w = (metrics.GetW()*dpiResolution) / 254;
	int h = (metrics.GetH()*dpiResolution) / 254;
	int r = l+w;
	int b = t+h;

	pImg->GetPreparedPassportImage().copySubImage(fieldImg, l, t, l + w, t + h);
	
	SaveDebugImage(fieldName,fieldImg);

	if (descriptor->IsBarcode())
		return RecognizeBarcodeField(fieldImg);
	else
		return RecognizeTextField(fieldImg);

}

std::wstring PassportRecognizer::RecognizeTextField(fipWinImage& fieldImg)
{	
	return m_pRecognizer->RecognizeText(fieldImg);
}

std::wstring PassportRecognizer::RecognizeBarcodeField(fipWinImage& fieldImg)
{
	return m_pRecognizer->RecognizeBarcode(fieldImg);
}


  
