#ifndef __XSTR_H__
#define __XSTR_H__

#include <xercesc/dom/DOM.hpp>
#include <string>

class XStr
{
public :
	XStr(const std::string& toTranscode);
	XStr(const char* const toTranscode);
	XStr(int toTranscode);
	//XStr(COLORREF toTranscode);
	XStr(bool toTranscode);
	
	
    ~XStr();

    const XMLCh* unicodeForm() const;

private :
    XMLCh*   fUnicodeForm;
};

#define X(str) XStr(str).unicodeForm()

#endif