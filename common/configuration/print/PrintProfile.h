#ifndef __PRINTPROFILE_H__
#define __PRINTPROFILE_H__

#include "IPrintProfile.h"
#include <vector>

class SerializablePrintProfile;

class PrintProfile : public IPrintProfile
{
public:
	PrintProfile(uint32_t id);
	PrintProfile(const PrintProfile& src);
	PrintProfile(const IPrintProfile* src);
	~PrintProfile();
	void CopyFrom(const IPrintProfile* src);

	PrintProfile& operator=(const PrintProfile& src);

	void SetDocumentCode(const std::wstring& sDocCode);
	const std::wstring& GetDocumentCode() const;
	
	void SetProfileName(const std::wstring& sName);
	uint32_t GetProfileId() const;
	const std::wstring& GetProfileName() const;

	PrintOption* GetOption(const std::wstring& optName) const;
	PrintOption* AddOption(const std::wstring& optName);
	void AddOption(const PrintOption& option);
	void RemoveOption(const std::wstring& optName);
	PtrEnumerator<PrintOption> EnumOptions() const;
	count_t GetOptionsCount() const;
	void CopyOptions(const IPrintProfile* src);
	
	OrderSpecifier* AddOrderSpecifier(const std::wstring& fieldCode);
	virtual PtrEnumerator<OrderSpecifier> EnumOrderSpecifiers() const;
	count_t GetOrderSpecifiersCount() const;
	OrderSpecifier* GetOrderSpecifierAt(int index);
	void DeleteOrderSpecifierAt(int index);
	void ClearOrderSpecifiers();
	void CopyOrderSpecifiers(const IPrintProfile* src);
	
protected:
	void AddOption(PrintOption* option);
	
private:
	uint32_t m_profileId;
	std::wstring m_profileName;
	std::wstring m_documentCode;
	std::map<std::wstring,PrintOption*> m_options;	
	std::vector<OrderSpecifier*> m_orderSpecifiers;

	friend class SerializablePrintProfile;
};

#endif