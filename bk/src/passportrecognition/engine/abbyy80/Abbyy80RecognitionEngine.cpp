#include "stdafx.h"
#include "Abbyy80RecognitionEngine.h"
#include <filesystem/paths.h>

#include "BstrWrap.h"
#include "../../runtime/RecognitionException.h"
#include <format/StringFormat.h>
#include "AbbyyString.h"
#include "AutoDib.h"
#include <memory>
#include <string\TextUtils.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


Abbyy80RecognitionEngine::Abbyy80RecognitionEngine() : m_FRDllHandle(0),m_Engine(NULL)
{
	m_pGetEngineObjectFn = NULL;
	m_pGetEngineObjectFn = NULL;
}



void Abbyy80RecognitionEngine::ThrowIfFailed(HRESULT hResult)
{
	if( FAILED( hResult ) ) 
	{
		// try to obtain FREngine internal error description
		CSafePtr<IErrorInfo> errorInfo;
		if( GetErrorInfo( 0, errorInfo.GetBuffer() ) == S_OK ) 
		{
			CBstr description;
			errorInfo->GetDescription( &description );			
			throw RecognitionException(description);			
		} 
		else 
		{
			throw RecognitionException(TextUtils::Format(L"Failed with result %08X",hResult));
		}
	}

	
}


std::wstring Abbyy80RecognitionEngine::RecognizeText(const fipWinImage& img)
{
	CSafePtr<IImageDocument> imageDoc;
	CAutoDib dib(img.copyToHandle());

	HRESULT result = m_Engine->OpenDib((long)(HANDLE)dib,300,&imageDoc);

	ThrowIfFailed(result);
	CSafePtr<IBlock> ptrTxtBlock;
	
	result = m_Engine->CreateBlock(BT_Text,&ptrTxtBlock);
	ThrowIfFailed(result);

	ULONG refCount = ptrTxtBlock->AddRef();
	refCount = ptrTxtBlock->Release();
	//ITextBlockProperties* pTxtBlockProps = NULL;
	//result = pTxtBlock->get_TextBlockProperties(&pTxtBlockProps);		
	//ThrowIfFailed(result);
	
	CSafePtr<IRegion> ptrTxtBlockRegion;
	result = ptrTxtBlock->get_Region(&ptrTxtBlockRegion);
	ThrowIfFailed(result);
	
	result = ptrTxtBlockRegion->AddRect(0,0,img.getWidth()-1,img.getHeight()-1);
	ThrowIfFailed(result);
			
	CSafePtr<ILayout> ptrLayout;
	result = m_Engine->CreateLayout(&ptrLayout);
	ThrowIfFailed(result);
	
	CSafePtr<IBlocksCollection> ptrLayoutBlocks;
	result = ptrLayout->get_Blocks(&ptrLayoutBlocks);
	ThrowIfFailed(result);

	result = ptrLayoutBlocks->Add(ptrTxtBlock);
	ThrowIfFailed(result);	

		
	result = m_Engine->RecognizeBlocks(imageDoc,0,ptrLayout,ptrLayoutBlocks);
	
	ThrowIfFailed(result);

	//long lBlockCount = 0;
	//result = pLayoutBlocks->get_Count(&lBlockCount);

	BlockRecognitionStatusEnum recognitionStatus;
	CSafePtr<ITextBlockProperties> ptrTxtBlockProps;
	CSafePtr<IText> ptrText;

	result = ptrTxtBlock->get_TextBlockProperties(&ptrTxtBlockProps);
	ThrowIfFailed(result);

	result = ptrTxtBlock->get_RecognitionStatus(&recognitionStatus);
	ThrowIfFailed(result);

	result = ptrTxtBlockProps->get_Text(&ptrText);
	ThrowIfFailed(result);

	std::wstring sRecognizedText;

	if(!ptrText.IsNull())
	{
		sRecognizedText = GetPlainText(ptrText);	
	}
	
	return sRecognizedText;

}

std::wstring Abbyy80RecognitionEngine::GetPlainText(IText* pText)
{
	CSafePtr<IParagraphs> ptrParagraphs;
	CSafePtr<IParagraph> ptrParagraph;
	BSTR pParagraphText;
	std::wstring sPlainText;

	HRESULT result = pText->get_Paragraphs(&ptrParagraphs);
	ThrowIfFailed(result);

	long lParagraphCount = 0;
	result = ptrParagraphs->get_Count(&lParagraphCount);
	ThrowIfFailed(result);

	for(long iParagraph = 0;iParagraph < lParagraphCount;iParagraph++)
	{
		ptrParagraphs->Item(iParagraph,&ptrParagraph);
		ThrowIfFailed(result);

		pParagraphText = NULL;
		
		result = ptrParagraph->get_Text(&pParagraphText);
		ThrowIfFailed(result);

		sPlainText += AbbyyString(CBstr(pParagraphText));
		if(iParagraph+1 < lParagraphCount)
			sPlainText += L'\n';

		::SysFreeString(pParagraphText);
	}

	return sPlainText;
}

std::wstring Abbyy80RecognitionEngine::RecognizeBarcode(const fipWinImage& img)
{
	CSafePtr<IImageDocument> imageDoc;
	
	CAutoDib dib(img.copyToHandle());
	HRESULT result = m_Engine->OpenDib((long)(HANDLE)dib,300,&imageDoc);
	
	ThrowIfFailed(result);

	CSafePtr<IBlock> ptrBCBlock;
	result = m_Engine->CreateBlock(BT_Barcode,&ptrBCBlock);
	ThrowIfFailed(result);
		
	CSafePtr<IRegion> ptrBCBlockRegion = NULL;
	ptrBCBlock->get_Region(&ptrBCBlockRegion);
		
	ptrBCBlockRegion->AddRect(0,0,img.getWidth()-1,img.getHeight()-1);
	
	CSafePtr<IBlocksCollection> ptrLayoutBlocks = NULL;		
	CSafePtr<ILayout> ptrLayout;
	result = m_Engine->CreateLayout(&ptrLayout);
	ThrowIfFailed(result);
	result = ptrLayout->get_Blocks(&ptrLayoutBlocks);
	ThrowIfFailed(result);
	result = ptrLayoutBlocks->Add(ptrBCBlock);			
	ThrowIfFailed(result);

	result = m_Engine->RecognizeBlocks(imageDoc,0,ptrLayout,ptrLayoutBlocks);

	ThrowIfFailed(result);
		
	CSafePtr<IBarcodeBlockProperties> ptrBCBlockProps = NULL;
	result = ptrBCBlock->get_BarcodeBlockProperties(&ptrBCBlockProps);
	ThrowIfFailed(result);

	CSafePtr<IText> ptrText;

	std::wstring sRecognizedBCText;

	if(!ptrBCBlockProps.IsNull())
	{
		 ptrBCBlockProps->get_BarcodeText(&ptrText);
		 if(!ptrText.IsNull())
		 {
			sRecognizedBCText = GetPlainText(ptrText);
		}
	}
	
	return sRecognizedBCText;
}

void Abbyy80RecognitionEngine::Create(const EngineSettings& settings)
{
	std::wstring sFREngingDllPath = paths_ConcatPath(settings.GetEnginePath(),L"FREngine.dll");

	if(!LoadFREngine(sFREngingDllPath))
	{
		BOOST_THROW_EXCEPTION(RecognitionException(L"Failed to load engine from " + sFREngingDllPath));
	}

	CBstr developerSN(settings.GetDeveloperSN());
	CBstr licence(settings.GetLicenseFilePath());
	CBstr projectId(settings.GetProjectId());

	HRESULT result = m_pGetEngineObjectFn( developerSN, licence, projectId, &m_Engine ) ;

	ThrowIfFailed(result);
	m_Engine->put_MessagesLanguage(ML_Polish);
	//m_Engine->
		
}

void Abbyy80RecognitionEngine::Destroy()
{
	if(m_Engine != NULL)
	{
		m_Engine = NULL;

		// Deinitialize FineReader Engine
		if( m_pDeinitializeEngineFn != NULL ) 
		{
			HRESULT hResult = m_pDeinitializeEngineFn();
			ThrowIfFailed(hResult);
		}
	}
	
	UnloadFREngine();
	
}

bool Abbyy80RecognitionEngine::UnloadFREngine()
{
	if( m_FRDllHandle == 0 ) 
	{
		return false;
	}
		
	// Now it's safe to free the FREngine.dll library
	FreeLibrary( m_FRDllHandle );
	m_FRDllHandle = 0;
	return true;
}

bool Abbyy80RecognitionEngine::LoadFREngine(const std::wstring& sFreDllPath)
{
	if( m_Engine != NULL ) {
		// Already loaded
		return false;
	}

	if( m_FRDllHandle == 0 ) 
	{
		m_FRDllHandle = LoadLibraryEx( sFreDllPath.c_str(), 0, LOAD_WITH_ALTERED_SEARCH_PATH );
		if( m_FRDllHandle == 0 )
		{			
			return false;
		}
	}	

	m_pGetEngineObjectFn = ( GetEngineObjectFunc )GetProcAddress( m_FRDllHandle, "GetEngineObject" );

	if( m_pGetEngineObjectFn == NULL ) 
	{
		UnloadFREngine();		
		return false;
	}

	m_pDeinitializeEngineFn = ( DeinitializeEngineFunc )GetProcAddress( m_FRDllHandle, "DeinitializeEngine" );
	if(m_pDeinitializeEngineFn == NULL)
	{
		UnloadFREngine();		
		return false;
	}

	return true;

}


