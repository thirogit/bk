#ifndef __EXCELEXPORTSETTINGS_H__
#define __EXCELEXPORTSETTINGS_H__

#include "../datatypes/CowDataComparator.h"
#include <vector>

class ExcelExportSettings
{
public:
	ExcelExportSettings();
	ExcelExportSettings(const ExcelExportSettings& src);
	ExcelExportSettings& operator=(const ExcelExportSettings& right);

	bool GetPutDocsInSeperateSheet() const;
	void SetPutDocsInSeperateSheet(bool bPutDocsInSeperateSheet);

	int GetFieldCount() const;
	COWDATA_MEMBERS GetField(int nIndex) const;
	void AddField(COWDATA_MEMBERS fieldId);
	void RemoveField(int nIndex);
	void RemoveAllFields();
private:
	std::vector<COWDATA_MEMBERS> m_FieldOrder;
	bool m_bPutDocsInSeperateSheet;
};

#endif