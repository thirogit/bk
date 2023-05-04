// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "SymbolScanner.h"

/////////////////////////////////////////////////////////////////////////////
// CSymbolScanner

IMPLEMENT_DYNCREATE(CSymbolScanner, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CSymbolScanner properties

CString CSymbolScanner::GetCheckHealthText()
{
	CString result;
	GetProperty(0x1, VT_BSTR, (void*)&result);
	return result;
}

void CSymbolScanner::SetCheckHealthText(LPCTSTR propVal)
{
	SetProperty(0x1, VT_BSTR, propVal);
}

BOOL CSymbolScanner::GetClaimed()
{
	BOOL result;
	GetProperty(0x2, VT_BOOL, (void*)&result);
	return result;
}

void CSymbolScanner::SetClaimed(BOOL propVal)
{
	SetProperty(0x2, VT_BOOL, propVal);
}

BOOL CSymbolScanner::GetDataEventEnabled()
{
	BOOL result;
	GetProperty(0x3, VT_BOOL, (void*)&result);
	return result;
}

void CSymbolScanner::SetDataEventEnabled(BOOL propVal)
{
	SetProperty(0x3, VT_BOOL, propVal);
}

BOOL CSymbolScanner::GetDeviceEnabled()
{
	BOOL result;
	GetProperty(0x4, VT_BOOL, (void*)&result);
	return result;
}

void CSymbolScanner::SetDeviceEnabled(BOOL propVal)
{
	SetProperty(0x4, VT_BOOL, propVal);
}

BOOL CSymbolScanner::GetFreezeEvents()
{
	BOOL result;
	GetProperty(0x5, VT_BOOL, (void*)&result);
	return result;
}

void CSymbolScanner::SetFreezeEvents(BOOL propVal)
{
	SetProperty(0x5, VT_BOOL, propVal);
}

long CSymbolScanner::GetOutputID()
{
	long result;
	GetProperty(0x6, VT_I4, (void*)&result);
	return result;
}

void CSymbolScanner::SetOutputID(long propVal)
{
	SetProperty(0x6, VT_I4, propVal);
}

long CSymbolScanner::GetResultCode()
{
	long result;
	GetProperty(0x7, VT_I4, (void*)&result);
	return result;
}

void CSymbolScanner::SetResultCode(long propVal)
{
	SetProperty(0x7, VT_I4, propVal);
}

long CSymbolScanner::GetResultCodeExtended()
{
	long result;
	GetProperty(0x8, VT_I4, (void*)&result);
	return result;
}

void CSymbolScanner::SetResultCodeExtended(long propVal)
{
	SetProperty(0x8, VT_I4, propVal);
}

long CSymbolScanner::GetState()
{
	long result;
	GetProperty(0x9, VT_I4, (void*)&result);
	return result;
}

void CSymbolScanner::SetState(long propVal)
{
	SetProperty(0x9, VT_I4, propVal);
}

CString CSymbolScanner::GetControlObjectDescription()
{
	CString result;
	GetProperty(0xa, VT_BSTR, (void*)&result);
	return result;
}

void CSymbolScanner::SetControlObjectDescription(LPCTSTR propVal)
{
	SetProperty(0xa, VT_BSTR, propVal);
}

long CSymbolScanner::GetControlObjectVersion()
{
	long result;
	GetProperty(0xb, VT_I4, (void*)&result);
	return result;
}

void CSymbolScanner::SetControlObjectVersion(long propVal)
{
	SetProperty(0xb, VT_I4, propVal);
}

CString CSymbolScanner::GetServiceObjectDescription()
{
	CString result;
	GetProperty(0xc, VT_BSTR, (void*)&result);
	return result;
}

void CSymbolScanner::SetServiceObjectDescription(LPCTSTR propVal)
{
	SetProperty(0xc, VT_BSTR, propVal);
}

long CSymbolScanner::GetServiceObjectVersion()
{
	long result;
	GetProperty(0xd, VT_I4, (void*)&result);
	return result;
}

void CSymbolScanner::SetServiceObjectVersion(long propVal)
{
	SetProperty(0xd, VT_I4, propVal);
}

CString CSymbolScanner::GetDeviceDescription()
{
	CString result;
	GetProperty(0xe, VT_BSTR, (void*)&result);
	return result;
}

void CSymbolScanner::SetDeviceDescription(LPCTSTR propVal)
{
	SetProperty(0xe, VT_BSTR, propVal);
}

CString CSymbolScanner::GetDeviceName()
{
	CString result;
	GetProperty(0xf, VT_BSTR, (void*)&result);
	return result;
}

void CSymbolScanner::SetDeviceName(LPCTSTR propVal)
{
	SetProperty(0xf, VT_BSTR, propVal);
}

CString CSymbolScanner::GetScanData()
{
	CString result;
	GetProperty(0x10, VT_BSTR, (void*)&result);
	return result;
}

void CSymbolScanner::SetScanData(LPCTSTR propVal)
{
	SetProperty(0x10, VT_BSTR, propVal);
}

BOOL CSymbolScanner::GetAutoDisable()
{
	BOOL result;
	GetProperty(0x11, VT_BOOL, (void*)&result);
	return result;
}

void CSymbolScanner::SetAutoDisable(BOOL propVal)
{
	SetProperty(0x11, VT_BOOL, propVal);
}

long CSymbolScanner::GetBinaryConversion()
{
	long result;
	GetProperty(0x12, VT_I4, (void*)&result);
	return result;
}

void CSymbolScanner::SetBinaryConversion(long propVal)
{
	SetProperty(0x12, VT_I4, propVal);
}

long CSymbolScanner::GetCapPowerReporting()
{
	long result;
	GetProperty(0x13, VT_I4, (void*)&result);
	return result;
}

void CSymbolScanner::SetCapPowerReporting(long propVal)
{
	SetProperty(0x13, VT_I4, propVal);
}

long CSymbolScanner::GetDataCount()
{
	long result;
	GetProperty(0x14, VT_I4, (void*)&result);
	return result;
}

void CSymbolScanner::SetDataCount(long propVal)
{
	SetProperty(0x14, VT_I4, propVal);
}

long CSymbolScanner::GetPowerNotify()
{
	long result;
	GetProperty(0x15, VT_I4, (void*)&result);
	return result;
}

void CSymbolScanner::SetPowerNotify(long propVal)
{
	SetProperty(0x15, VT_I4, propVal);
}

long CSymbolScanner::GetPowerState()
{
	long result;
	GetProperty(0x16, VT_I4, (void*)&result);
	return result;
}

void CSymbolScanner::SetPowerState(long propVal)
{
	SetProperty(0x16, VT_I4, propVal);
}

BOOL CSymbolScanner::GetDecodeData()
{
	BOOL result;
	GetProperty(0x17, VT_BOOL, (void*)&result);
	return result;
}

void CSymbolScanner::SetDecodeData(BOOL propVal)
{
	SetProperty(0x17, VT_BOOL, propVal);
}

CString CSymbolScanner::GetScanDataLabel()
{
	CString result;
	GetProperty(0x18, VT_BSTR, (void*)&result);
	return result;
}

void CSymbolScanner::SetScanDataLabel(LPCTSTR propVal)
{
	SetProperty(0x18, VT_BSTR, propVal);
}

long CSymbolScanner::GetScanDataType()
{
	long result;
	GetProperty(0x19, VT_I4, (void*)&result);
	return result;
}

void CSymbolScanner::SetScanDataType(long propVal)
{
	SetProperty(0x19, VT_I4, propVal);
}

long CSymbolScanner::GetOpenResult()
{
	long result;
	GetProperty(0x25, VT_I4, (void*)&result);
	return result;
}

void CSymbolScanner::SetOpenResult(long propVal)
{
	SetProperty(0x25, VT_I4, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// CSymbolScanner operations

long CSymbolScanner::Open(LPCTSTR lpszDeviceName)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1a, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lpszDeviceName);
	return result;
}

long CSymbolScanner::Close()
{
	long result;
	InvokeHelper(0x1b, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CSymbolScanner::Claim(long lTimeout)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1c, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lTimeout);
	return result;
}

long CSymbolScanner::Release()
{
	long result;
	InvokeHelper(0x1d, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CSymbolScanner::ClearInput()
{
	long result;
	InvokeHelper(0x1e, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CSymbolScanner::ClearOutput()
{
	long result;
	InvokeHelper(0x1f, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CSymbolScanner::DirectIO(long lCommand, long* plData, BSTR* pstrString)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_PI4 VTS_PBSTR;
	InvokeHelper(0x20, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lCommand, plData, pstrString);
	return result;
}

long CSymbolScanner::CheckHealth(long lLevel)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x21, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lLevel);
	return result;
}

long CSymbolScanner::ClaimDevice(long lTimeout)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x22, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lTimeout);
	return result;
}

long CSymbolScanner::ReleaseDevice()
{
	long result;
	InvokeHelper(0x23, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CSymbolScanner::CloseDevice()
{
	long result;
	InvokeHelper(0x24, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

void CSymbolScanner::AboutBox()
{
	InvokeHelper(0xfffffdd8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}