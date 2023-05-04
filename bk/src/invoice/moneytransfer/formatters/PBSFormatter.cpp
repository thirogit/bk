#include "stdafx.h"
#include "PBSFormatter.h"
#include "fields/MoneyTransferTitleField.h"
#include "fields/MoneyFloatValueField.h"
#include "fields/InvoiceHentNameField.h"
#include "fields/InvoiceHentAddrField.h"
#include "fields/InvoiceHentAccountNoField.h"
#include "fields/CompanySourceAccountNo.h"
#include <format/FloatFormat.h>
#include "utils/FileFormatTarget.h"
#include "utils/XStr.h"
#include <boost\foreach.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

PBSFormatter::PBSFormatter() 
{
	
}

std::string PBSFormatter::GetFormatterName()
{
	return "PBS";
}

std::string PBSFormatter::GetFileExtension()
{
	return "XML";
}

void PBSFormatter::FormatTransfers(  MoneyTransferEntryRange moneyTransfers,
										MoneyTransferContext* pContext,
										CFile& outputFile,
										SimpleProgress *progress)
{
	try 
	{
		xercesc::XMLPlatformUtils::Initialize();
	}
	catch (const xercesc::XMLException& toCatch) 
	{
		// Do your failure processing here
		return;
	}


	xercesc::DOMImplementation    *pImplement = NULL;
	xercesc::DOMLSOutput         *pOutput    = NULL;
	FileFormatTarget			xmlTarget(&outputFile);

	/*
	Return the first registered implementation that has
	the desired features. In this case, we are after
	a DOM implementation that has the LS feature... or Load/Save.
	*/
	pImplement = xercesc::DOMImplementationRegistry::getDOMImplementation(L"LS");
		

	/*
	From the DOMImplementation, create a DOMWriter.
	DOMWriters are used to serialize a DOM tree [back] into an XML document.
	*/
	pOutput = ((xercesc::DOMImplementationLS*)pImplement)->createLSOutput();
		

	pOutput->setEncoding(X("iso-8859-2"));

	pOutput->setByteStream(&xmlTarget);
	
	//get a LSSerializer object
	xercesc::DOMLSSerializer* pSerializer = pImplement->createLSSerializer();

	if (pSerializer->getDomConfig()->canSetParameter(xercesc::XMLUni::fgDOMWRTFormatPrettyPrint, true))
		pSerializer->getDomConfig()->setParameter(xercesc::XMLUni::fgDOMWRTFormatPrettyPrint, true);
	
	xercesc::DOMDocument * pDOMDocument = NULL;
	pDOMDocument = pImplement->createDocument(0, L"przelewy", 0);

	xercesc::DOMElement * pRootElement = NULL;
	pRootElement = pDOMDocument->getDocumentElement();

	BOOST_FOREACH(MoneyTransferEntry* entry,moneyTransfers)
	{
		SerializeToXml(pContext,entry,pRootElement);
	}

	bool bSuccess = pSerializer->write(pDOMDocument,pOutput);

	xmlTarget.flush();
	
	pOutput->release();

	pDOMDocument->release();

	pSerializer->release();

	xercesc::XMLPlatformUtils::Terminate();
		
}

PBSFormatter::~PBSFormatter()
{
}

void PBSFormatter::SerializeToXml(MoneyTransferContext* pContext,MoneyTransferEntry* pEntry,xercesc::DOMElement * pParentElement)
{

	xercesc::DOMDocument* pDOMDoc = pParentElement->getOwnerDocument();
	
	xercesc::DOMElement* pEntryElement = pDOMDoc->createElement(X("przelew"));
	pParentElement->appendChild(pEntryElement);

	xercesc::DOMElement* pSrcAccountNoElement = pDOMDoc->createElement(X("rach_obc"));
	pEntryElement->appendChild(pSrcAccountNoElement);
	pSrcAccountNoElement->appendChild(pDOMDoc->createTextNode(X(CompanySourceAccountNo().GetFieldValue(pContext,pEntry))));

	//xercesc::DOMElement* pSrcBankNoElement = pDOMDoc->createElement(X("bank"));
	//pEntryElement->appendChild(pSrcBankNoElement);
	//pSrcBankNoElement->appendChild(pDOMDoc->createTextNode(X()));

	xercesc::DOMElement* pDestAccountNoElement = pDOMDoc->createElement(X("rachunek"));
	pEntryElement->appendChild(pDestAccountNoElement);
	pDestAccountNoElement->appendChild(pDOMDoc->createTextNode(X(InvoiceHentAccountNoField().GetFieldValue(pContext,pEntry))));

	xercesc::DOMElement* pDestNameElement = pDOMDoc->createElement(X("nazwa"));
	pEntryElement->appendChild(pDestNameElement);

	

	pDestNameElement->appendChild(pDOMDoc->createTextNode(
		X(
			InvoiceHentNameField().GetFieldValue(pContext,pEntry) + ", " + InvoiceHentAddrField().GetFieldValue(pContext,pEntry)
		 )));

	xercesc::DOMElement* pAmountElement = pDOMDoc->createElement(X("kwota"));
	pEntryElement->appendChild(pAmountElement);
	pAmountElement->appendChild(pDOMDoc->createTextNode(X(MoneyFloatValueField(DecimalFormat(2,'.')).GetFieldValue(pContext,pEntry))));

	xercesc::DOMElement* pTitleElement = pDOMDoc->createElement(X("tresc"));
	pEntryElement->appendChild(pTitleElement);
	pTitleElement->appendChild(pDOMDoc->createTextNode(X(MoneyTransferTitleField().GetFieldValue(pContext,pEntry))));
	
}