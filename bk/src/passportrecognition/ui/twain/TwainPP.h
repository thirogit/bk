///////////////////////////////////////////////////////////////////////////////
//
//		CTwainPP
//		----------
//		C++ Twain interface (encapsulates TWAIN.H)
//
////Holger Kloos, 2003/////////////////////////////////////////////////////////


#ifndef __TWAINPP_H_
#define __TWAINPP_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "DynDll.h"
#include "twain.h"
#include "TwainEventHandler.h"

#include "Tracer.h"

typedef HGLOBAL HDIB;
const float A4_SizeX = 21.0f;
const float A4_SizeY = 29.7f;
const float Inch = 2.54f;


class CTwainPP : private CDynDLL, private IMsgLink 
{
public:
	enum ETWainError { eNoTwain, eCancel, eGetCapability, eSetCapability, eDeviceNotReady, eErrorReading };

	interface ITwainResult
	{
		virtual void OnTwainImageInfo(int nSizeX, int nSizeY) = 0;
		virtual BOOL OnTwainBitmap(HDIB hDIB) = 0;
		virtual void OnTwainError(ETWainError eTwainError) = 0;
	};

	enum EImageType { itColor, itGray, itBlackWhite };

	struct SBitmapOptions
	{
		SBitmapOptions() : 	eImageType(itGray), nResolution(360), nBrightness(0), nContrast(0), fSizeX(0.0f), fSizeY(0.0f) 
		{ 
		};

		EImageType	eImageType;
		int			nResolution;
		int			nBrightness;
		int			nContrast;
		float		fSizeX;
		float		fSizeY;
	};

	class CTracerDummy : public ITracer
	{
		virtual void MakeTrace(const TCHAR*, ...) {}
	};


	class CTwainId
	{
	public:
		CTwainId();
		CTwainId(const CTwainId& rOriginal);
		virtual ~CTwainId();
		void Reset();

		BOOL IsValid() const		{ return m_ID.Id != 0; }

		TW_IDENTITY* operator &()	{ return &m_ID; }

		const char* GetName() const { return m_ID.ProductName;}

		void FillStructure(const char* pAppName, TW_UINT16 nSupportedGroups, TW_UINT16 nCountryId);

	private:
		TW_IDENTITY m_ID;
	};

	interface ITwainSourceEnum
	{
		virtual BOOL OnTwainEnum(const CTwainId& rSourceId) = 0;
	};
private:
	class CSourceSelector : public ITwainSourceEnum
	{
	public:
		CSourceSelector(const char* pSourceName);
		~CSourceSelector();

		const CTwainId& GetSource() const { return m_SelectedSourceId;  }

	private:
		virtual BOOL OnTwainEnum(const CTwainId& rSourceId);

		CTwainId	m_SelectedSourceId;
		const char* m_pSourceName;
	};

public:
	CTwainPP(const char* pLinkName, ITracer* pTracer = &m_TracerDummy);
	virtual ~CTwainPP();

	BOOL Create(HWND hParentWnd, ITwainResult* pTwainResult);
	BOOL Close();

	BOOL OpenSource(const char* pSourceName = NULL);
	BOOL CloseSource();

	const char* GetSourceName() const	{ return m_SourceId.GetName(); }
	UINT EnumSources(ITwainSourceEnum* pSourceEnum);

	BOOL GetMaxImageSize(float& fWidth, float& fHeight);

	BOOL GetBitmap(BOOL bDlgModus, const SBitmapOptions& rOption,BOOL bUseADF = FALSE);

	BOOL IsDeviceOnline();

	BOOL HasPendingTransfers();

	BOOL DoPendingTransfer();
	

private:
	TW_UINT16 CallDSMEntry(pTW_IDENTITY pDest, TW_UINT32 dg,TW_UINT16 dat, TW_UINT16 msg, TW_MEMREF pData);
	inline BOOL DSMManagerEntry(TW_UINT32 dg,TW_UINT16 dat, TW_UINT16 msg, TW_MEMREF pData);
	inline BOOL DSMSourceEntry(TW_UINT32 dg,TW_UINT16 dat, TW_UINT16 msg, TW_MEMREF pData);

	BOOL SelectCapability(TW_UINT16 nCap, TW_UINT32 nVal);
	BOOL SelectImageCount(TW_UINT32 nCount);
	BOOL SelectImageType(TW_UINT32 nColor);

	BOOL SelectCapabilityValue(TW_UINT16 nCap, TW_INT16 nWhole, TW_UINT16 nFrac = 0);
	BOOL SelectCapabilityValue(TW_UINT16 nCap, float fVal);

	BOOL SelectResolution(TW_INT16 nResolution);
	BOOL SelectBrightness(TW_INT16 nBrightness);
	BOOL SelectContrast(TW_INT16 nContrast);
	BOOL SelectScaling(TW_INT16 nScaling);

	BOOL SelectSource(const char* pSourceName);

	BOOL SetImageLayout(const float fLeft, const float fTop, const float fRight, const float fBottom);
	BOOL GetDefaultImageSize(float& fWidth, float& fHeight);

	BOOL GetCurrentCapabilityValue(TW_UINT16 nCap, float& fRes);
	BOOL GetCapabilityValue(TW_UINT16 nCap, float& fRes);
	BOOL GetCapabilityValue(TW_UINT16 nMsg, TW_UINT16 nCap, float& fRes);

	void OnTransfer();
	void OnCloseSourceDlgRequest();

	void CloseTransfer();

	BOOL GetBitmapFromSource(BOOL bVisible);

	virtual BOOL OnWinMsg(const MSG* pMsg);

	void SetFloatToIntPair(const float fSource, TW_INT16& nWhole, TW_UINT16& nFrac) const;
	void SetFrame(TW_FRAME& frame, const float fLeft, const float fTop, const float fRight, const float fBottom) const;

	void SetFix32ToFloat(const TW_FIX32& Fix, float& fResult) const;

	void SetError(ETWainError eError) const;

	BOOL  EnableADF();

	DSMENTRYPROC		DSMEntryProc;

	BOOL				m_bAutoFeedON;
	BOOL				m_bHasPendingXFERS;
	CTwainId			m_AppId;
	CTwainId			m_SourceId;
	CTwainEventHandler	m_EventHandler;
	ITwainResult*		m_pTwainResult;

	ITracer*			m_pTracer;
	static CTracerDummy	m_TracerDummy;
};



BOOL CTwainPP::DSMManagerEntry(TW_UINT32 dg,TW_UINT16 dat, TW_UINT16 msg, TW_MEMREF pData)
{
	return CallDSMEntry(NULL, dg, dat, msg, pData) == TWRC_SUCCESS;
}

BOOL CTwainPP::DSMSourceEntry(TW_UINT32 dg,TW_UINT16 dat, TW_UINT16 msg, TW_MEMREF pData)
{
	ASSERT(m_SourceId.IsValid());
	return CallDSMEntry(&m_SourceId, dg, dat, msg, pData) == TWRC_SUCCESS;
}

#endif
