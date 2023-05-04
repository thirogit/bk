#include "stdafx.h"
#include "URLBuilder.h"
#include "../string/IntString.h"


#define DEFAULT_HTTP_PORT 80

CURLBuilder::CURLBuilder(const CString& hostname,UINT port,const CString& resource) : m_hostName(hostname),m_uPort(port),m_sResource(resource)
{
}

CURLBuilder::CURLBuilder(const CString& hostname,const CString& resource) : m_hostName(hostname),m_uPort(DEFAULT_HTTP_PORT),m_sResource(resource)
{
}

void CURLBuilder::AddParameter(const CString& paramName,const CString& sParamValue)
{
	if(!paramName.IsEmpty())
		m_Parameters.SetAt(paramName,URLEscape(sParamValue));
}
void CURLBuilder::AddParameter(const CString& paramName,int iParamValue)
{
	if(!paramName.IsEmpty())
	{
		m_Parameters.SetAt(paramName,(CString)CIntString(iParamValue));
	}
}

CString CURLBuilder::URLEscape(const CString& s)
{
	CString sEscaped;

	for(int i = 0;i < s.GetLength();i++)
	{
		char c = s[i];
		if(IsEscapeCharacter(c))
		{
			sEscaped.AppendFormat("%%%02X",c);
		}
		else
		{
			sEscaped += c;
		}
	}
	return sEscaped;
}

bool CURLBuilder::IsEscapeCharacter(char c)
{
	const char _escapies[] = " <>#%{}|\\^~[]`;/?:@=&$}";
	int l = strlen(_escapies);
	for(int i = 0;i < l;i++)
		if(_escapies[i] == c)
			return true;

	return false;
}
		
CString CURLBuilder::GetURL() const
{
	if(!m_hostName.IsEmpty() && !m_sResource.IsEmpty())
	{
		CString url("http://");
		url += m_hostName;

		if(DEFAULT_HTTP_PORT != m_uPort)
		{
			url += ':';
			url += CIntString(m_uPort);
		}
		
		if(m_sResource.GetAt(0) != '/')
			url += '/';

		url += m_sResource;
		
		if(m_Parameters.GetSize() > 0)
		{
			url += '?';
			POSITION pos = m_Parameters.GetStartPosition();
			CString paramName,paramValue;

			while(pos)
			{
				m_Parameters.GetNextAssoc(pos,paramName,paramValue);
				url += paramName;
				url += '=';
				url += paramValue;	
				if(pos)
					url += '&';
			}
		}


		return url;
	}
	return CString();
	
}
