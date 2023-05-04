#ifndef __URLBUILDER_H__
#define __URLBUILDER_H__

#include "../types/MapEx.h"

class CURLBuilder
{
public:
	CURLBuilder(const CString& hostname,UINT port,const CString& resource);
	CURLBuilder(const CString& hostname,const CString& resource);
	void AddParameter(const CString& paramName,const CString& sParamValue);
	void AddParameter(const CString& paramName,int iParamValue);
	CString GetURL() const;
protected:
	CString URLEscape(const CString& s);
	bool IsEscapeCharacter(char c);

protected:
	CString m_hostName;
	CString m_sResource;
	UINT m_uPort;

	CMapEx<CString,const CString&,CString,const CString&> m_Parameters;

};


#endif //__TRANSFERSTREAM_H__