#pragma once

#include <string>
#include "../ctrls/layoutlistctrl/ListHeaderLayout.h"
#include "../context/SeasonSession.h"

class HeaderLayoutConfiguration : public ListHeaderLayout
{
public:
	HeaderLayoutConfiguration(const std::wstring& layoutId);
	void Save(SeasonSession* session);
	void Load(SeasonSession* session);
protected:
	std::wstring GetSettingName() const;
private:
	std::wstring m_layoutId;
};