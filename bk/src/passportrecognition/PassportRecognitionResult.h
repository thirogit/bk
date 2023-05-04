#ifndef __PASSPORTRECOGNITIONRESULT_H__
#define __PASSPORTRECOGNITIONRESULT_H__

#include "PassportImage.h"
#include "PassportRecognizer.h"

class PassportRecognitionResult
{
	friend class PassportRecognitionFieldsConstructor;
	friend class PassportRecognizer;
private:
	
	void SetPassDate(const std::wstring& sValue);
	void SetPassNo(const std::wstring& sValue);
	void SetCowNoBC(const std::wstring& sValue);
	void SetCowNo(const std::wstring& sValue);
	void SetFarmNoBC(const std::wstring& sValue);
	void SetBirthDatePlace(const std::wstring& sValue);
	void SetCowStock(const std::wstring& sValue);
	void SetMotherNo(const std::wstring& sValue);
	void SetFstOwner(const std::wstring& sValue);
	void SetCowSex(const std::wstring& sValue);

	void CopyFrom(const PassportRecognitionResult& src);

public:
	PassportRecognitionResult();
	PassportRecognitionResult(const PassportRecognitionResult &src);
	PassportRecognitionResult& operator=(const PassportRecognitionResult &right);


	const std::wstring& GetPassDate() const;
	const std::wstring& GetPassNo() const;
	const std::wstring& GetCowNoBC() const;
	const std::wstring& GetCowNo() const;
	const std::wstring& GetFarmNoBC() const;
	const std::wstring& GetBirthDatePlace() const;
	const std::wstring& GetCowStock() const;
	const std::wstring& GetMotherNo() const;
	const std::wstring& GetFstOwner() const;
	const std::wstring& GetCowSex() const;
	
	

private:
	std::wstring m_passDate;
	std::wstring m_passNo;
	std::wstring m_cowNoBC;
	std::wstring m_cowNo;
	std::wstring m_farmNoBC;
	std::wstring m_birthDatePlace;
	std::wstring m_cowSex;
	std::wstring m_cowStock;
	std::wstring m_motherNo;
	std::wstring m_fstOwner;

	
	
};



#endif 
