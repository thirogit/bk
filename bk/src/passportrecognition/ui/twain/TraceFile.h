///////////////////////////////////////////////////////////////////////////////
//
//		CTraceFile
//		----------
//		Implements ITrace as logging to file
//
////Holger Kloos, 2003/////////////////////////////////////////////////////////


#ifndef __TRACEFILE_H__
#define __TRACEFILE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Tracer.h"


class CTraceFile : public ITracer
{
public:
	CTraceFile();
	virtual ~CTraceFile();

	
	virtual void MakeTrace(const TCHAR* sTrace, ...);

private:
	
	BOOL	m_bEnabled;
};

#endif
