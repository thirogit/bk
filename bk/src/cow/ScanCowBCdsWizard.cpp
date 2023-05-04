#include "stdafx.h"
#include "ScanCowBCdsWizard.h"
#include "../utils/StringLoader.h"
#include "../resource.h"
#include <string\TextUtils.h>

ScanWizardResult::ScanWizardResult()
{
}

ScanWizardResult::ScanWizardResult(const ScanWizardResult& src)
{
	m_hentNo = src.m_hentNo;
	m_cowNo = src.m_cowNo;
}

ScanWizardResult::ScanWizardResult(ScanWizardResult&& src)
{
	m_hentNo = src.m_hentNo;
	m_cowNo = src.m_cowNo;

	src.m_hentNo.MakeNull();
	src.m_cowNo.MakeNull();
}

const HentNo& ScanWizardResult::GetHentNo() const
{
	return m_hentNo;
}

const CowNo& ScanWizardResult::GetCowNo() const
{
	return m_cowNo;
}

void ScanWizardResult::SetHentNo(const HentNo& hentNo)
{
	m_hentNo = hentNo;
}

void ScanWizardResult::SetCowNo(const CowNo& cowNo)
{
	m_cowNo = cowNo;
}

//#################################################################################################
CowNoBarcodeInputDialog::CowNoBarcodeInputDialog(CWnd* pParent) : CBarcodeInputDlg(pParent)
{

}

void CowNoBarcodeInputDialog::SetDefaultCountry(const Country& country)
{
	m_defaultCountry = country;
}

CowNo CowNoBarcodeInputDialog::InputCowNo()
{
	if (DoModal() == IDOK)
	{
		return m_input;
	}

	return CowNo();
}

void CowNoBarcodeInputDialog::ReceiveInput(const std::wstring& sBarcode)
{
	try
	{
		if (TextUtils::IsNumber(sBarcode) && !m_defaultCountry.IsNull())
		{
				int numPartLength = m_defaultCountry.GetCowNoLength()-2;
				std::wstring numberOnlyBarcode = TextUtils::PadBegin(sBarcode, '0', numPartLength);
				std::wstring cowNoBarcode = m_defaultCountry.GetCountryCode2A() + numberOnlyBarcode;
				m_input = CowNoFactory::FromString(cowNoBarcode);			
		}
		else
		{
			m_input = CowNoFactory::FromString(sBarcode);
		}		
	}
	catch (CowNoException& e)
	{
		throw BarcodeValidationException(StringLoader(IDS_ERR_NOT_A_COWNO));
	}

}

std::wstring CowNoBarcodeInputDialog::GetEntryText()
{
	return StringLoader(IDS_SCANCOWNO);
}

//#################################################################################################
HentNoBarcodeInputDialog::HentNoBarcodeInputDialog(CWnd* pParent) : CBarcodeInputDlg(pParent)
{

}

HentNo HentNoBarcodeInputDialog::InputHentNo()
{
	if (DoModal() == IDOK)
	{
		return m_input;
	}

	return HentNo();
}

void HentNoBarcodeInputDialog::ReceiveInput(const std::wstring& sBarcode)
{
	try
	{
		m_input = HentNoFactory::FromString(sBarcode);
	}
	catch (HentNoException& e)
	{
		throw BarcodeValidationException(StringLoader(IDS_ERR_NOT_A_HENTNO));
	}	
}

std::wstring HentNoBarcodeInputDialog::GetEntryText()
{
	return StringLoader(IDS_SCANHENTNO);
}

//#################################################################################################

ScanCowBCdsWizard::ScanCowBCdsWizard(CWnd *pParent/* = NULL*/) : m_pParent(pParent)
{

}

void ScanCowBCdsWizard::SetAssumedCountry(const Country& country)
{
	m_assumedCountry = country;
}

ScanWizardResult ScanCowBCdsWizard::DoMagic()
{
	ScanWizardResult result;
	CowNoBarcodeInputDialog cowNoInput(m_pParent);
	cowNoInput.SetDefaultCountry(m_assumedCountry);
	CowNo cowNo = cowNoInput.InputCowNo();
	if (!cowNo.IsNull())
	{
		result.SetCowNo(cowNo);

		HentNoBarcodeInputDialog hentNoInput(m_pParent);
		HentNo hentNo = hentNoInput.InputHentNo();
		result.SetHentNo(hentNo);		
	}
	return result;
}

