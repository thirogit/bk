///////////////////////////////////////////////////////////////////////////////
//
//		CTwainPP
//		----------
//		C++ Twain interface (encapsulates TWAIN.H)
//
////Holger Kloos, 2003/////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TwainPP.h"

#include "twain.h"
#include "Tracer.h"
#include <stdlib.h>


//	CTwainPP::CTwainId
//	------------------

CTwainPP::CTwainId::CTwainId()
{
	Reset();
}

CTwainPP::CTwainId::CTwainId(const CTwainId& rOriginal)
{
	memcpy(&m_ID, &rOriginal.m_ID,  sizeof(TW_IDENTITY));
}

CTwainPP::CTwainId::~CTwainId()
{
}

void CTwainPP::CTwainId::Reset()
{
	memset(&m_ID, 0, sizeof(TW_IDENTITY));
}


void CTwainPP::CTwainId::FillStructure(const char* pAppName, TW_UINT16 nSupportedGroups, TW_UINT16 nCountryId)
{
	m_ID.Version.MajorNum = 1;
	m_ID.Version.Country = nCountryId;
	m_ID.ProtocolMajor = TWON_PROTOCOLMAJOR;
	m_ID.ProtocolMinor = TWON_PROTOCOLMINOR;
	m_ID.SupportedGroups = nSupportedGroups;
	strncpy_s(m_ID.ProductName, pAppName, sizeof(m_ID.ProductName));
}

//  CTwainPP::CSourceSelector
//	-------------------------

CTwainPP::CSourceSelector::CSourceSelector(const char* pSourceName) 
				: m_pSourceName(pSourceName) 
{ 
}

CTwainPP::CSourceSelector::~CSourceSelector()
{
}

BOOL CTwainPP::CSourceSelector::OnTwainEnum(const CTwainId& rSourceId)
{
	if (strcmp(rSourceId.GetName(), m_pSourceName) == 0)
	{
		m_SelectedSourceId = rSourceId;
		return TRUE;
	}
	return FALSE;
}

//	CTwainPP
//	--------

CTwainPP::CTracerDummy CTwainPP::m_TracerDummy;

CTwainPP::CTwainPP(const char* pLinkName, ITracer* pTracer)
		: m_pTracer(pTracer),m_bAutoFeedON(FALSE),m_bHasPendingXFERS(FALSE)
{
	ASSERT(m_pTracer);

	m_pTwainResult = NULL;
	DSMEntryProc = NULL;

	m_AppId.FillStructure(pLinkName ? pLinkName : "TwainPP", DG_IMAGE /*| DG_CONTROL*/, TWCY_GERMANY);
}

CTwainPP::~CTwainPP()
{
	Close();
}

BOOL CTwainPP::Create(HWND hParentWnd, ITwainResult* pTwainResult)
{
	m_pTwainResult = pTwainResult;

	if (!CDynDLL::Create(_T("TWAIN_32.DLL")) || !GetAddress((LPVOID*)&DSMEntryProc, MAKEINTRESOURCEA(1)))
	{
		SetError(eNoTwain);
		return FALSE;
	}

	m_EventHandler.SetWindowHandle(hParentWnd);
	return DSMManagerEntry(DG_CONTROL, DAT_PARENT, MSG_OPENDSM, (TW_MEMREF)m_EventHandler.GetWindowHandlePtr());
}

TW_UINT16 CTwainPP::CallDSMEntry(pTW_IDENTITY pDest, TW_UINT32 dg,TW_UINT16 dat, TW_UINT16 msg, TW_MEMREF pData)
{
	ASSERT(DSMEntryProc);

	TW_UINT16 nRes =DSMEntryProc(&m_AppId, pDest, dg, dat, msg, pData);

	m_pTracer->MakeTrace(L"CallDSMEntry %d, %d, %d -> %d", dg, dat, msg, nRes);

	return nRes;
}


BOOL CTwainPP::SelectSource(const char* pSourceName)
{
	if (pSourceName)
	{
		CSourceSelector Selector(pSourceName);
		EnumSources(&Selector);

		const CTwainId& Source = Selector.GetSource();
		return Source.IsValid() 
			&& DSMManagerEntry(DG_CONTROL, DAT_IDENTITY, MSG_GETDEFAULT, (TW_MEMREF)&Source);
	}

	if (EnumSources(NULL) == 1)
		return DSMManagerEntry(DG_CONTROL, DAT_IDENTITY, MSG_GETDEFAULT, (TW_MEMREF)&m_SourceId);

	return DSMManagerEntry(DG_CONTROL, DAT_IDENTITY, MSG_USERSELECT, (TW_MEMREF)&m_SourceId);
}

BOOL CTwainPP::OpenSource(const char* pSourceName)
{
	return SelectSource(pSourceName)
		&& DSMManagerEntry(DG_CONTROL, DAT_IDENTITY, MSG_OPENDS, (TW_MEMREF)&m_SourceId)
		&& SelectImageCount(1);
}

BOOL CTwainPP::Close()
{
	if (!m_AppId.IsValid())
		return FALSE;

	CloseSource();

	DSMManagerEntry(DG_CONTROL, DAT_PARENT, MSG_CLOSEDSM, (TW_MEMREF)m_EventHandler.GetWindowHandlePtr());
	m_AppId.Reset();

	return CDynDLL::Free();
}

BOOL CTwainPP::CloseSource()
{
	if (m_SourceId.IsValid())
	{
		DSMManagerEntry(DG_CONTROL, DAT_IDENTITY, MSG_CLOSEDS, (TW_MEMREF)&m_SourceId);
		m_SourceId.Reset();
	}
	return TRUE;
}

void CTwainPP::CloseTransfer()
{
	if (!m_SourceId.IsValid())
		return;

	TW_USERINTERFACE ui;
	memset(&ui, 0, sizeof(ui));
	DSMSourceEntry(DG_CONTROL, DAT_USERINTERFACE, MSG_DISABLEDS, (TW_MEMREF)&ui);

	m_bAutoFeedON = FALSE;
	m_bHasPendingXFERS = FALSE;

	m_EventHandler.DisConnect();
}

void CTwainPP::SetError(ETWainError eError) const
{
	if (m_pTwainResult)
		m_pTwainResult->OnTwainError(eError);
}

BOOL CTwainPP::IsDeviceOnline()
{
	float fRes = 0.0f;
	if (!m_SourceId.IsValid())
		return FALSE;

	if (!GetCapabilityValue(CAP_DEVICEONLINE, fRes))
		return TRUE;							// Capability maybe not supported
	
	return fRes != 0.0;
}

BOOL CTwainPP::GetBitmap(BOOL bDlgModus, const SBitmapOptions& rOption,BOOL bUseADF/* = FALSE*/)
{
	ASSERT(rOption.nResolution > 0);

	if (!IsDeviceOnline())
	{
		SetError(eDeviceNotReady);
		return FALSE;
	}

	if(HasPendingTransfers()) return FALSE;

	TW_UINT32 nColor = TWPT_RGB;
	switch(rOption.eImageType)
	{
	case itGray:
		nColor = TWPT_GRAY;
		break;
	case itBlackWhite:
		nColor = TWPT_BW;
		break;
	}

	float fScanWidth, fScanHeight;
	if (!GetMaxImageSize(fScanWidth , fScanHeight))
	{
		SetError(eGetCapability);
		return FALSE;
	}

	if (rOption.fSizeX < fScanWidth)
		fScanWidth = rOption.fSizeX;
	if (rOption.fSizeY < fScanHeight)
		fScanHeight = rOption.fSizeY;
	
	if (	!SetImageLayout(0.0f, 0.0f, fScanWidth, fScanHeight)
//		||	!SelectScaling((TW_INT16)rOption.nScaling)
		||	!SelectImageType(nColor) 
		||	!SelectResolution((TW_INT16)rOption.nResolution)
		||	!SelectBrightness((TW_INT16)rOption.nBrightness)
		||	!SelectContrast((TW_INT16)rOption.nContrast))
	{
		SetError(eSetCapability);
		return FALSE;
	}

	if(bUseADF)
		m_bAutoFeedON = EnableADF();
	else
		m_bAutoFeedON = FALSE;

	return GetBitmapFromSource(bDlgModus);
}

BOOL CTwainPP::GetBitmapFromSource(BOOL bVisible)
{
	if (!m_EventHandler.Connect(this))
		return FALSE;

	TW_USERINTERFACE ui;
	ui.ShowUI = bVisible != FALSE;
	ui.ModalUI = TRUE;
	ui.hParent = m_EventHandler.GetWindowHandle();
	return DSMSourceEntry(DG_CONTROL, DAT_USERINTERFACE, MSG_ENABLEDS, (TW_MEMREF)&ui);
}

BOOL CTwainPP::SelectCapability(TW_UINT16 nCap, TW_UINT32 nVal)
{
	TW_CAPABILITY twCap;
	twCap.Cap = nCap;
	twCap.ConType = TWON_ONEVALUE;

	twCap.hContainer = GlobalAlloc(GHND, sizeof(TW_ONEVALUE));
	pTW_ONEVALUE pVal = (pTW_ONEVALUE)GlobalLock(twCap.hContainer);
	if (!pVal)
		return FALSE;

	pVal->ItemType = TWTY_INT16;
	pVal->Item = nVal;

	GlobalUnlock(twCap.hContainer);
	
	BOOL bRes = FALSE;
	
	TW_UINT16 nRes = CallDSMEntry(&m_SourceId, DG_CONTROL, DAT_CAPABILITY, MSG_SET, (TW_MEMREF)&twCap);
	GlobalFree(twCap.hContainer);

	switch (nRes)
	{
	case TWRC_SUCCESS:
		bRes = TRUE;
		break;

	case TWRC_FAILURE:
		bRes = FALSE;
		break;

	case TWRC_CHECKSTATUS:
	default:
		{
			m_pTracer->MakeTrace(L"Read capability again...");
			float fVal;
			bRes = GetCurrentCapabilityValue(nCap, fVal) && fVal == nVal;
		}
	}

	return bRes;
}

void CTwainPP::SetFloatToIntPair(const float fSource, TW_INT16& nWhole, TW_UINT16& nFrac) const
{
	const TW_INT32 temp = TW_INT32(fSource * 65536.0 + 0.5);

	nWhole = TW_INT16(temp >> 16);
	nFrac = TW_UINT16(temp & 0x0000FFFF);
}

BOOL CTwainPP::SelectCapabilityValue(TW_UINT16 nCap, float fVal)
{
	TW_INT16 nWhole;
	TW_UINT16 nFrac;
	SetFloatToIntPair(fVal, nWhole, nFrac);

	return SelectCapabilityValue(nCap, nWhole, nFrac);
}

BOOL CTwainPP::SelectCapabilityValue(TW_UINT16 nCap, TW_INT16 nWhole, TW_UINT16 nFrac)
{
	TW_CAPABILITY twCap;
	twCap.Cap = nCap;
	twCap.ConType = TWON_ONEVALUE;

	twCap.hContainer = GlobalAlloc(GHND, sizeof(TW_ONEVALUE));
	pTW_ONEVALUE pVal = (pTW_ONEVALUE)GlobalLock(twCap.hContainer);
	if (!pVal)
		return FALSE;

	pVal->ItemType = TWTY_FIX32;
	TW_FIX32* fixVal = (TW_FIX32*)&(pVal->Item);
	fixVal->Whole = nWhole;
	fixVal->Frac = nFrac;

	GlobalUnlock(twCap.hContainer);

	TW_UINT16 nRes = CallDSMEntry(&m_SourceId, DG_CONTROL, DAT_CAPABILITY, MSG_SET, (TW_MEMREF)&twCap);
	GlobalFree(twCap.hContainer);

	switch (nRes)
	{
	case TWRC_SUCCESS:
		return TRUE;

	case TWRC_CHECKSTATUS:
	default:
		{
			m_pTracer->MakeTrace(L"Read capability again...");
			float fVal;
			if (!GetCurrentCapabilityValue(nCap, fVal))
				return FALSE;

			m_pTracer->MakeTrace(L"Set %f instead of %d,%d", fVal, nWhole, nFrac);
	
			return TRUE;
		}
	}

	return FALSE;
}

void CTwainPP::SetFrame(TW_FRAME& frame, const float fLeft, const float fTop, const float fRight, const float fBottom) const
{
	SetFloatToIntPair(fLeft, frame.Left.Whole, frame.Left.Frac);
	SetFloatToIntPair(fTop, frame.Top.Whole, frame.Top.Frac);
	SetFloatToIntPair(fRight, frame.Right.Whole, frame.Right.Frac);
	SetFloatToIntPair(fBottom, frame.Bottom.Whole, frame.Bottom.Frac);
}

void CTwainPP::SetFix32ToFloat(const TW_FIX32& Fix, float& fResult) const
{
	fResult = float(Fix.Whole + Fix.Frac / 65536.0);
}

BOOL CTwainPP::SetImageLayout(const float fLeft, const float fTop, const float fRight, const float fBottom)
{
	TW_IMAGELAYOUT layout;

	SetFrame(layout.Frame, fLeft, fTop, fRight, fBottom);

	layout.DocumentNumber = 1;
	layout.PageNumber = 1;
	layout.FrameNumber = 1;

	return SelectCapability(ICAP_UNITS, TWUN_INCHES)
		&& DSMSourceEntry(DG_IMAGE, DAT_IMAGELAYOUT, MSG_SET, (TW_MEMREF)&layout);
}

BOOL CTwainPP::SelectResolution(TW_INT16 nResolution)
{
	return	SelectCapability(ICAP_UNITS, TWUN_INCHES)
			&& SelectCapabilityValue(ICAP_XRESOLUTION, nResolution)
			&& SelectCapabilityValue(ICAP_YRESOLUTION, nResolution);
}

BOOL CTwainPP::SelectBrightness(TW_INT16 nBrightness)
{
	if (nBrightness < -1000)
		nBrightness = -1000;
	else
		if (nBrightness > 1000)
			nBrightness = 1000;

	return SelectCapabilityValue(ICAP_BRIGHTNESS, nBrightness);
}

BOOL CTwainPP::SelectContrast(TW_INT16 nContrast)
{
	if (nContrast < -1000)
		nContrast = -1000;
	else
		if (nContrast > 1000)
			nContrast = 1000;

	return SelectCapabilityValue(ICAP_CONTRAST, nContrast);
}

BOOL CTwainPP::SelectScaling(TW_INT16 nScaling)
{
	float fScale = (float)nScaling / 100.0f;
	return	SelectCapabilityValue(ICAP_XSCALING, fScale)
		&&	SelectCapabilityValue(ICAP_YSCALING, fScale);
}


BOOL CTwainPP::SelectImageCount(TW_UINT32 nCount)
{
	return SelectCapability(CAP_XFERCOUNT, nCount);
}

BOOL CTwainPP::SelectImageType(TW_UINT32 nColor)
{
	return SelectCapability(ICAP_PIXELTYPE, nColor);
}

BOOL CTwainPP::GetDefaultImageSize(float& fWidth, float& fHeight)
{
	TW_IMAGELAYOUT layout;

	BOOL bRes = SelectCapability(ICAP_UNITS, TWUN_INCHES)
		&& DSMSourceEntry(DG_IMAGE, DAT_IMAGELAYOUT, MSG_GETDEFAULT, (TW_MEMREF)&layout);

	SetFix32ToFloat(layout.Frame.Right, fWidth);
	SetFix32ToFloat(layout.Frame.Bottom, fHeight);

	return bRes;
}

BOOL CTwainPP::GetMaxImageSize(float& fWidth, float& fHeight)
{
	BOOL bRes = SelectCapability(ICAP_UNITS, TWUN_INCHES)
		&& GetCapabilityValue(ICAP_PHYSICALWIDTH, fWidth)
		&& GetCapabilityValue(ICAP_PHYSICALHEIGHT, fHeight);

	return bRes;
}

BOOL CTwainPP::GetCurrentCapabilityValue(TW_UINT16 nCap, float& fRes)
{
	return GetCapabilityValue(MSG_GETCURRENT, nCap, fRes);
}

BOOL CTwainPP::GetCapabilityValue(TW_UINT16 nCap, float& fRes)
{
	return GetCapabilityValue(MSG_GET, nCap, fRes);
}

BOOL CTwainPP::GetCapabilityValue(TW_UINT16 nMsg, TW_UINT16 nCap, float& fRes)
{
	TW_CAPABILITY twCap;
	twCap.Cap = nCap;
	twCap.ConType = TWON_ONEVALUE;
	twCap.hContainer = NULL;

	m_pTracer->MakeTrace(L"GetCapabilityValue %d", nCap);

	if (!DSMSourceEntry(DG_CONTROL, DAT_CAPABILITY, nMsg, (TW_MEMREF)&twCap))
	{
		m_pTracer->MakeTrace(L" -> not supported capability");
		return FALSE;
	}

	pTW_ONEVALUE pVal = (pTW_ONEVALUE)GlobalLock(twCap.hContainer);
	if (!pVal)
		return FALSE;

	switch (pVal->ItemType)
	{
	case TWTY_INT8:
		fRes = (char)pVal->Item;
		break;

	case TWTY_INT16:
		fRes = (short)pVal->Item;
		break;

	case TWTY_INT32:
		fRes = (float)(int)pVal->Item;ASSERT(0);
		break;

	case TWTY_UINT8:
		fRes = (BYTE)pVal->Item;
		break;

	case TWTY_BOOL:
	case TWTY_UINT16:
		fRes = (WORD)pVal->Item;
		break;

	case TWTY_UINT32:
		fRes = (float)(UINT)pVal->Item;ASSERT(0);
		break;

	case TWTY_FIX32:
		SetFix32ToFloat(*(TW_FIX32*)(&pVal->Item), fRes);
		break;

	default:
		m_pTracer->MakeTrace(L" -> unsupported type");
		fRes = 0.0;ASSERT(0);
	}

	m_pTracer->MakeTrace(L" %d -> %f", pVal->ItemType, fRes);

	GlobalUnlock(twCap.hContainer);
	GlobalFree(twCap.hContainer);

	return TRUE;
}

void CTwainPP::OnTransfer()
{
	TW_IMAGEINFO imageInfo;
	if (!DSMSourceEntry(DG_IMAGE, DAT_IMAGEINFO, MSG_GET, (TW_MEMREF)&imageInfo))
	{
		SetError(eErrorReading);
		return;
	}

	if (m_pTwainResult)
			m_pTwainResult->OnTwainImageInfo(imageInfo.ImageWidth, imageInfo.ImageLength);

	HBITMAP hBitmap = NULL;
	if (CallDSMEntry(&m_SourceId, DG_IMAGE, DAT_IMAGENATIVEXFER, MSG_GET, (TW_MEMREF)&hBitmap) == TWRC_XFERDONE)
	{
		BOOL bAppReceiveBMP = m_pTwainResult 
							&& m_pTwainResult->OnTwainBitmap(hBitmap);
		if (!bAppReceiveBMP)
			DeleteObject(hBitmap);
	}
	else
	{
		if (hBitmap)
			DeleteObject(hBitmap);

		SetError(eCancel);
	}

	TW_PENDINGXFERS pendingXfers;
	if (DSMSourceEntry(DG_CONTROL, DAT_PENDINGXFERS, MSG_ENDXFER, (TW_MEMREF)&pendingXfers))
	{
		m_pTracer->MakeTrace(L"Close Transfer with %d images remaining", pendingXfers.Count);
	}

	if(!m_bAutoFeedON || pendingXfers.Count == 0)
		CloseTransfer();
	else
	{
		m_bHasPendingXFERS = TRUE;
	}

	m_pTracer->MakeTrace(L"Transfer finished");
}

void CTwainPP::OnCloseSourceDlgRequest()
{
	CloseTransfer();
}

BOOL CTwainPP::OnWinMsg(const MSG* pMsg)
{
	TW_EVENT twEvent;
	twEvent.pEvent = (TW_MEMREF)pMsg;
	twEvent.TWMessage = MSG_NULL;

	if (CallDSMEntry(&m_SourceId, DG_CONTROL, DAT_EVENT, MSG_PROCESSEVENT, (TW_MEMREF)&twEvent) == TWRC_NOTDSEVENT)
		return FALSE;

	switch (twEvent.TWMessage)
	{
	case MSG_XFERREADY:
		OnTransfer();
		return TRUE;

	case MSG_CLOSEDSREQ:
		OnCloseSourceDlgRequest();
		return TRUE;

	case MSG_CLOSEDSOK:
	case MSG_DEVICEEVENT:
		return TRUE;

	default:
		return FALSE;
	}
}

UINT CTwainPP::EnumSources(ITwainSourceEnum* pSourceEnum)
{
	UINT nRes = 0;

	CTwainId source;
	if (!DSMManagerEntry(DG_CONTROL, DAT_IDENTITY, MSG_GETFIRST, (TW_MEMREF)&source))
		return FALSE;

	do
	{
		if (pSourceEnum && pSourceEnum->OnTwainEnum(source))
			break;
		nRes++;
	}
	while (DSMManagerEntry(DG_CONTROL, DAT_IDENTITY, MSG_GETNEXT, (TW_MEMREF)&source));

	return nRes;
}



BOOL  CTwainPP::EnableADF()
{
	TW_CAPABILITY   cap;
	pTW_ONEVALUE    pval = NULL;
	TW_INT16        status = TWRC_SUCCESS;
	

	memset(&cap, 0, sizeof(TW_CAPABILITY));
	

	cap.Cap = CAP_FEEDERENABLED;
	cap.ConType = TWON_ONEVALUE;

	status = CallDSMEntry(&m_SourceId,
		DG_CONTROL, 
		DAT_CAPABILITY, 
		MSG_GET,
		(TW_MEMREF)&cap);

	if (status != TWRC_SUCCESS)
	{
		GlobalFree(cap.hContainer);
		status = TWRC_FAILURE;
	}
	else
	{   
		pval = (pTW_ONEVALUE)GlobalLock(cap.hContainer);
		if (pval->Item == TRUE)
		{   
			/*
			* Feeder is enabled no need to negotiate
			*/
			GlobalUnlock(cap.hContainer);
			GlobalFree((HANDLE)cap.hContainer); 
		}
		else     
		{
			/*
			* Negotiate with the source
			* Try to turn on CAP_FEEDERENABLED
			*/
			pval->ItemType = TWTY_BOOL;
			pval->Item = TRUE;
			GlobalUnlock(cap.hContainer);

			status = CallDSMEntry(&m_SourceId,
				DG_CONTROL, 
				DAT_CAPABILITY, 
				MSG_SET,
				(TW_MEMREF)&cap);

			/*
			* free here because the GET call will allocate a new container
			*/
			GlobalFree(cap.hContainer);

			if (status == TWRC_SUCCESS)
			{   
				/*
				* Verify that CAP_FEEDERENABLED is now TRUE
				*/
				status = CallDSMEntry(&m_SourceId,
					DG_CONTROL, 
					DAT_CAPABILITY, 
					MSG_GET,
					(TW_MEMREF)&cap);

				if (status == TWRC_SUCCESS)
				{
					pval = (pTW_ONEVALUE)GlobalLock(cap.hContainer);
					if (pval->Item != TRUE) 
					{
						/*
						* Verification failed -- CAP_FEEDERENABLED is FALSE even after successful set to TRUE
						*/
						status = TWRC_FAILURE;						
					}
					GlobalUnlock(cap.hContainer);
					GlobalFree((HANDLE)cap.hContainer);    
				}
				else
				{
					status = TWRC_FAILURE;                  					
				}               
			}
			else
			{    
				/*
				* MSG_SET of CAP_FEEDERENABLED to TRUE did not succeed
				*/
				status = TWRC_FAILURE;
			}
		}

		if (status == TWRC_SUCCESS)
		{
			/*
			* CAP_AUTOFEED
			*/
			cap.Cap = CAP_AUTOFEED;
			cap.ConType = TWON_ONEVALUE;

			status = CallDSMEntry(&m_SourceId,
				DG_CONTROL, 
				DAT_CAPABILITY, 
				MSG_GET,
				(TW_MEMREF)&cap);

			if (status != TWRC_SUCCESS)
			{
				/*
				* MSG_GET on CAP_AUTOFEED did not succeed
				*/
				status = TWRC_FAILURE;				
			}

			/*
			* MSG_GET on CAP_AUTOFEED returned success
			*/
			pval = (pTW_ONEVALUE)GlobalLock(cap.hContainer);
			if (pval->Item == TRUE)
			{
				/*
				* CAP_AUTOFEED is on
				*/
				GlobalUnlock(cap.hContainer);
				GlobalFree((HANDLE)cap.hContainer);         
			}
			else
			{
				/*
				* Try to set CAP_AUTOFEED to TRUE
				*/
				pval->ItemType = TWTY_BOOL;
				pval->Item = TRUE;
				GlobalUnlock(cap.hContainer);

				status = CallDSMEntry(&m_SourceId,
					DG_CONTROL, 
					DAT_CAPABILITY, 
					MSG_SET,
					(TW_MEMREF)&cap);

				GlobalFree((HANDLE)cap.hContainer);     

				if (status == TWRC_SUCCESS)
				{   
					/*
					* Verify that CAP_AUTOFEED is on
					*/
					status = CallDSMEntry(&m_SourceId,
						DG_CONTROL, 
						DAT_CAPABILITY, 
						MSG_GET,
						(TW_MEMREF)&cap);

					if (status == TWRC_SUCCESS)
					{
						pval = (pTW_ONEVALUE)GlobalLock(cap.hContainer);
						if (pval->Item != TRUE)
						{
							status = TWRC_FAILURE;
							/*
							* CAP_AUTOFEED returns FALSE even after succesful set to TRUE
							*/
						}
						GlobalUnlock(cap.hContainer);
						GlobalFree((HANDLE)cap.hContainer); 
					}
					else
					{
						status = TWRC_FAILURE;
					}
				}
				else
				{
					/*
					* MSG_SET of CAP_AUTOFEED to TRUE did not succeed
					*/
					status = TWRC_FAILURE;					
				}
			}   
		}   
	}   

	
	return status == TWRC_SUCCESS;
}  

BOOL CTwainPP::HasPendingTransfers()
{	
	return m_bAutoFeedON && m_bHasPendingXFERS;
}


BOOL CTwainPP::DoPendingTransfer()
{
	if(HasPendingTransfers())
	{
		OnTransfer();
		return TRUE;
	}
	return FALSE;
}
