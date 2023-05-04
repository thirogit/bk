///////////////////////////////////////////////////////////////////////////////
//
//		CTraceFile
//		----------
//		Implements ITrace as logging to file
//
////Holger Kloos, 2003/////////////////////////////////////////////////////////


#include "stdafx.h"
#include "TraceFile.h"


//  CTraceFile
//	----------


CTraceFile::CTraceFile()
{
	m_bEnabled = FALSE;
}

CTraceFile::~CTraceFile()
{
	m_bEnabled = FALSE;
}



void CTraceFile::MakeTrace(const TCHAR* sTrace, ...)
{
	CString str;

	va_list argList;
	va_start(argList, sTrace);
	str.FormatV(sTrace, argList);
	va_end(argList);

	str += _T("\r\n");

	

	TRACE(str);
}
