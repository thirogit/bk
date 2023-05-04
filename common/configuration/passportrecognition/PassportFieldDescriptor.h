#if !defined(_PASSPORTFIELDDESCRIPTOR_H_)
#define _PASSPORTFIELDDESCRIPTOR_H_

#include <string>

class PassportFields;

class PassportFieldDescriptor
{
private:
	PassportFieldDescriptor(const std::wstring& sFieldName, bool bIsBarcode);
public:
	bool IsBarcode() const;
	const std::wstring& GetFieldName() const;
private:
	std::wstring m_sFieldName;			
	bool m_bIsBarcode;

	friend class PassportFields;
};

#endif