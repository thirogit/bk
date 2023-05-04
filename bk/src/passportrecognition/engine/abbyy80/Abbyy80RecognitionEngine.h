#ifndef __ABBYY80RECOGNITIONENGINE_H__
#define __ABBYY80RECOGNITIONENGINE_H__

#include "../RecognitionEngine.h"
#include "FREngine.h"
#include "SafePtr.h"

typedef HRESULT ( STDAPICALLTYPE* GetEngineObjectFunc )( BSTR, BSTR, BSTR, IEngine** );
typedef HRESULT ( STDAPICALLTYPE* DeinitializeEngineFunc )();

class Abbyy80RecognitionEngine : public RecognitionEngine
{
public:
	Abbyy80RecognitionEngine();
	virtual std::wstring RecognizeText(const fipWinImage& img);
	virtual std::wstring RecognizeBarcode(const fipWinImage& img);

	void Create(const EngineSettings& settings);
	void Destroy();

private:
	bool UnloadFREngine();
	bool LoadFREngine(const std::wstring& sFreDllPath);
	void ThrowIfFailed(HRESULT hResult);

	std::wstring GetPlainText(IText* pText);
	
private:
	HMODULE m_FRDllHandle;
	CSafePtr<IEngine> m_Engine;

	GetEngineObjectFunc m_pGetEngineObjectFn;
	DeinitializeEngineFunc m_pDeinitializeEngineFn;
};



#endif 
