#include "stdafx.h"
#include "HeaderLayoutConfiguration.h"

HeaderLayoutConfiguration::HeaderLayoutConfiguration(const std::wstring& layoutId) : m_layoutId(layoutId)
{
}

void HeaderLayoutConfiguration::Save(SeasonSession* session)
{
	std::map<int, int> layoutMap;
	PtrEnumerator<ColumnLayout> enumCol = EnumColumns();
	while (enumCol.hasNext())
	{
		ColumnLayout* columnLayout = *enumCol;
		layoutMap[columnLayout->GetColId()] = columnLayout->GetColWidth();
		enumCol.advance();
	}
	session->GetUserSettings()->SetSettingAny(GetSettingName(), layoutMap);
}

std::wstring HeaderLayoutConfiguration::GetSettingName() const
{
	return L"LAYOUT." + m_layoutId;
}

void HeaderLayoutConfiguration::Load(SeasonSession* session)
{
	boost::any layoutAny = session->GetUserSettings()->GetSettingAny(GetSettingName());

	if (!layoutAny.empty())
	{
		std::map<int, int> layoutMap = boost::any_cast<std::map<int, int>>(layoutAny);

		Clear();

		auto layoutIt = layoutMap.begin();
		while (layoutIt != layoutMap.end())
		{
			AddColumnLayout((uint32_t)layoutIt->first, layoutIt->second);
			layoutIt++;
		}
	}
}

	