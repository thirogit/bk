#include "stdafx.h"
#include "XStr.H"
#include <stdio.h>

XStr::XStr(const char* const toTranscode)
{
	fUnicodeForm = xercesc::XMLString::transcode(toTranscode);
}

XStr::XStr(const std::string& toTranscode)
{
	fUnicodeForm = xercesc::XMLString::transcode(toTranscode.c_str());
}

XStr::XStr(int toTranscode)
{
	char intBuffer[30];
	sprintf_s(intBuffer,sizeof(intBuffer),"%d",toTranscode);
	fUnicodeForm = xercesc::XMLString::transcode(intBuffer);
}


//XStr::XStr(COLORREF toTranscode)
//{
//	char rgbBuffer[30];
//	sprintf_s(rgbBuffer,sizeof(rgbBuffer),"#%02X%02X%02X",GetRValue(toTranscode),GetGValue(toTranscode),GetBValue(toTranscode));
//	fUnicodeForm = xercesc::XMLString::transcode(rgbBuffer);
//}

XStr::XStr(bool toTranscode)
{
	if(toTranscode)
		fUnicodeForm = xercesc::XMLString::transcode("true");
	else
		fUnicodeForm = xercesc::XMLString::transcode("false");

}

XStr::~XStr()
{
	xercesc::XMLString::release(&fUnicodeForm);
}


const XMLCh* XStr::unicodeForm() const
{
	return fUnicodeForm;
}


