#ifndef __CHECKBOXITEM_H__
#define __CHECKBOXITEM_H__

#include <cstdint>
#include <string>

class CheckBoxItem
{
public:
	CheckBoxItem(uint32_t optionId,const std::wstring& optionDesc,bool bOptionValue);
	const std::wstring& GetOptionDesc() const;
	bool GetOptionValue() const;
	void SetOptionDesc(const std::wstring& sOptDesc);
	void SetOptionValue(bool bValue);
	uint32_t GetOptionId() const;
private:
	std::wstring m_sOptionDesc;
	bool m_bOptionValue;
	uint32_t m_OptionId;
};

#endif 