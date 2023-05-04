#pragma once

#include "../../ctrls/contentlistctrl/ContentListCtrl.h"
#include "DeductionDefinitionRow.h"
#include "CRUDDeductionDefinition.h"
#include "DeductionDefinitionViewCreator.h"
	
class DeductionDefsContent : public ListContent
{
public:
	DeductionDefsContent();
	~DeductionDefsContent();
	
	void Add(CRUDDeductionDefinition* definition);
	void AddAll(CRUDDeductionDefinitionRange range);
	void DeleteAt(int row);
	void UpdateAt(int row, CRUDDeductionDefinition* definition);
	
	void DeleteAll();

	virtual int GetRowCount() const;
	virtual ListRow* GetRow(int row);
	virtual void SortContent(const SortAttributes& attributes);
	
protected:
	void QuietAdd(CRUDDeductionDefinition* definition);

	DeductionDefinitionViewCreator m_viewCreator;
private:
	std::vector<DeductionDefinitionRow*> m_rows;
	
};
	
