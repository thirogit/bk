#ifndef __SCANCOWBCDSWIZARD_H__
#define __SCANCOWBCDSWIZARD_H__

#include <data\types\HentNo.h>
#include <data\types\CowNo.h>
#include "../barcodescanner/ui/BarcodeInputDlg.h"

class ScanWizardResult
{
public:
	ScanWizardResult();
	ScanWizardResult(const ScanWizardResult& src);
	ScanWizardResult(ScanWizardResult&& src);

	const HentNo& GetHentNo() const;
	const CowNo& GetCowNo() const;

	void SetHentNo(const HentNo& hentNo);
	void SetCowNo(const CowNo& cowNo);;

private:
	HentNo m_hentNo;
	CowNo m_cowNo;
};

class CowNoBarcodeInputDialog : public CBarcodeInputDlg
{
public:
	CowNoBarcodeInputDialog(CWnd* pParent);
	void SetDefaultCountry(const Country& country);
	CowNo InputCowNo();
protected:
	CBarcodeInputDlg::DoModal;

	virtual void ReceiveInput(const std::wstring& sBarcode);
	virtual std::wstring GetEntryText();
private:
	CowNo m_input;
	Country m_defaultCountry;
};

class HentNoBarcodeInputDialog : public CBarcodeInputDlg
{
public:
	HentNoBarcodeInputDialog(CWnd* pParent);
	HentNo InputHentNo();
protected:
	CBarcodeInputDlg::DoModal;

	virtual void ReceiveInput(const std::wstring& sBarcode);
	virtual std::wstring GetEntryText();
private:
	HentNo m_input;
};

class ScanCowBCdsWizard 
{
public:
	ScanCowBCdsWizard(CWnd *pParent = NULL);
	ScanWizardResult DoMagic();
	void SetAssumedCountry(const Country& country);
	
protected:
	CWnd* m_pParent;
	Country m_assumedCountry;
};

#endif