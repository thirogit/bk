#ifndef __PASSPOERPROFILEFIELDSLISTCTRL_H__
#define __PASSPOERPROFILEFIELDSLISTCTRL_H__

#include "../../../ctrls/ownrdrwlistctrl/OwnrDrwListCtrl.h"
#include "../../profileeditor/PassportRecognitionProfileEditor.h"
#include <types/NullDouble.h>
#include <functional>

typedef std::function<void (const IPassportField*,FIELD_METRIC,int)> MetricListener;

class PassportProfileFieldsListCtrl : public COwnrDrwListCtrl
{
	enum COLUMN_IDS
	{
		COL_FIELDTYPE,
		COL_FIELDNAME,
		COL_X,
		COL_Y,
		COL_WIDTH,
		COL_HEIGHT
	};

	class FieldItem
	{
	public:
		FieldItem(const std::wstring&,const IPassportField*);
		const IPassportField* field;
		std::wstring fieldname;
	};

public:
	PassportProfileFieldsListCtrl(PassportRecognitionProfileEditor *pProfileEditor);
	~PassportProfileFieldsListCtrl();
	void SetMetricListener(MetricListener metricListener);
	
private:
	PassportRecognitionProfileEditor *m_pProfileEditor;
	NewPtrFlushArray<FieldItem> m_Items;
	CImageList m_FieldTypeImagesList;
	MetricListener m_MetricListener;
protected:
	
	std::wstring GetCellText(int row, uint32_t colId);
	Color GetCellColor(int row, uint32_t colId);
	int GetCellImage(int row, uint32_t colId);
	void OnCellDblClick(int row, uint32_t colId, CellDescriptor& cell);

	virtual void SortByCol(int Column, bool bAsc);
	virtual void PostCreate();


	void NotifyMetric(const IPassportField* pField,FIELD_METRIC metric,int value);
	
	void EditFieldMetric(const IPassportField* pField, FIELD_METRIC metric, const CRect& cellRect);
	void OnSetMetricValue(const IPassportField* pField, FIELD_METRIC metric, const NullDouble& value);
	
	void UpdateItemCount();
			
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
 
	
	DECLARE_MESSAGE_MAP();
};


#endif

