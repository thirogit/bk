#ifndef __EXCELEXPORTSETTINGS_H__
#define __EXCELEXPORTSETTINGS_H__

#include "../view/CowPresenceView.h"
#include "../context/UserSettings.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/identity.hpp>

class ExcelExportSettings
{

	typedef boost::multi_index_container
		<
		uint32_t,
		boost::multi_index::indexed_by<	
		boost::multi_index::random_access<>,
		boost::multi_index::ordered_unique<	boost::multi_index::identity<uint32_t> >	
		>
		> FieldIndex;

public:
	ExcelExportSettings();
	ExcelExportSettings(const ExcelExportSettings& src);
	ExcelExportSettings(ExcelExportSettings&& src);
	ExcelExportSettings& operator=(const ExcelExportSettings& right);	

	int GetFieldCount() const;	
	uint32_t GetField(int nIndex) const;
	void AddField(uint32_t fieldId);
	bool HasField(uint32_t fieldId) const;
	void RemoveField(int nIndex);
	void RemoveAllFields();

	static ExcelExportSettings Load(UserSettings* settings, const std::wstring& settingName);
	void Save(UserSettings* settings, const std::wstring& settingName);
private:
	FieldIndex m_FieldOrder;

};

#endif