#ifndef __PASSPORTPROFILEEDITORDLG_H__
#define __PASSPORTPROFILEEDITORDLG_H__

#include "../../resource.h"
#include <configuration/passportrecognition/PassportRecognitionProfile.h>
#include "ctrls\PassportProfileFieldsListCtrl.h"
//#include "ctrls\FloatEnterEdit.h"
#include "../profileeditor/PassportRecognitionProfileEditor.h"
#include "ctrls\FloorPlanCtrl.h"
#include "..\..\ctrls\numericedit\NumericEdit.h"
#include "../../ctrls/enteredit/EnterEdit.h"

class PassportProfileEditorDlg : public CDialog
{
	class MetricEdit : public CNumericEdit
	{
	public:
		MetricEdit();
	};

public:
	PassportProfileEditorDlg(PassportRecognitionProfileEditor* pProfileEditor,CWnd* pParent = NULL);  
	virtual ~PassportProfileEditorDlg();
	void SetCaption(const std::wstring& sCaption);

	enum { IDD = IDD_PASSPORT_PROFILE };
private:
	PassportRecognitionProfileEditor* m_pProfileEditor;
	PassportProfileFieldsListCtrl m_FieldList;
	CWrapperEnterEdit<MetricEdit> m_DimXEdit;
	CWrapperEnterEdit<MetricEdit> m_DimYEdit;
	std::wstring m_sProfileName;

	FloorPlanCtrl m_Floor;
	std::wstring  m_sDlgCaption;
	
	
protected:

	void OnOK();
	virtual BOOL OnInitDialog();
	void UpdateFloor();
	void UpdateDims();
	void OnFieldMetric(const IPassportField* pField,FIELD_METRIC metric,int value);
	void UpdateTiles();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    


	afx_msg void OnDimY(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDimX(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	
	DECLARE_MESSAGE_MAP();
};

#endif