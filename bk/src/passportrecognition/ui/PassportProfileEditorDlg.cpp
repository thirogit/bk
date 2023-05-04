#include "stdafx.h"
#include "PassportProfileEditorDlg.h"
#include <string/TextUtils.h>
#include "../../ctrls/stdedit/StdEdit.h"
#include <data\datalimits.h>
#include "utils\PassportMetricUtils.h"
#undef min
#undef max
#include "../../ui/ExceptionMsgBox.h"

#define MAX_DIMX_METRIC 2100
#define MIN_DIMX_METRIC 1500
#define MAX_DIMY_METRIC 1500
#define MIN_DIMY_METRIC 1000

#define MIN_FIELD_W 40
#define MIN_FIELD_H 40


PassportProfileEditorDlg::MetricEdit::MetricEdit() : CNumericEdit(2)
{
}


//--------------------------------------------------------------------------------------------=----

BEGIN_MESSAGE_MAP(PassportProfileEditorDlg, CDialog)
	ON_NOTIFY(EN_EDITCOMPLETE, IDC_PASSPORT_DIMX, OnDimX)
	ON_NOTIFY(EN_EDITCOMPLETE, IDC_PASSPORT_DIMY, OnDimY)
END_MESSAGE_MAP()

PassportProfileEditorDlg::PassportProfileEditorDlg(PassportRecognitionProfileEditor* pProfileEditor,CWnd* pParent /*=NULL*/)
	:	CDialog(PassportProfileEditorDlg::IDD, pParent),
		m_pProfileEditor(pProfileEditor),
		m_FieldList(pProfileEditor)
{	
	MetricListener metricListener = std::bind(&PassportProfileEditorDlg::OnFieldMetric, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	m_FieldList.SetMetricListener(metricListener);
}

PassportProfileEditorDlg::~PassportProfileEditorDlg()
{
}

void PassportProfileEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_FIELDS_LIST,m_FieldList);
	DDX_StdText(pDX,IDC_PASSPORT_PROFILE_NAME, m_sProfileName);
	DDX_Control(pDX,IDC_PASSPORT_DIMY,m_DimYEdit);
	DDX_Control(pDX,IDC_PASSPORT_DIMX,m_DimXEdit);
	DDX_Control(pDX,IDC_PASSPORT_METRICS,m_Floor);
	
}

void PassportProfileEditorDlg::SetCaption(const std::wstring& sCaption)
{
	m_sDlgCaption = sCaption;
}

BOOL PassportProfileEditorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	IPassportRecognitionProfile* pProfile = m_pProfileEditor->GetProfile();

	m_sProfileName = pProfile->GetProfileName();
	SendDlgItemMessage(IDC_PASSPORT_PROFILE_NAME,EM_LIMITTEXT,MAXPROFILENAME);

	UpdateDims();
	
	UpdateData(FALSE);
	
	UpdateFloor();

	PassportFieldMetrics metrics;
	PtrEnumerator<IPassportField> fieldEnum = pProfile->EnumFields();
	while(fieldEnum.hasNext())
	{
		metrics = fieldEnum->GetFieldMetrics();
		m_Floor.AddTile(fieldEnum->GetDescriptor()->GetFieldName(),
			FloatPoint(metrics.GetX() / 10.0, metrics.GetY() / 10.0),
			FloatSize(metrics.GetW() / 10.0, metrics.GetH() / 10.0));
		fieldEnum.advance();
	}


	SetWindowText(m_sDlgCaption.c_str());

	return TRUE;
}

void PassportProfileEditorDlg::OnOK()
{
	UpdateData();

	m_pProfileEditor->SetProfileName(m_sProfileName.c_str());

	ExceptionMsgBox msgBox;

	msgBox.TryCatchAny([this]()
	{
		m_pProfileEditor->Save();
		EndDialog(IDOK);
	});
			
}

void PassportProfileEditorDlg::UpdateDims()
{
	IPassportRecognitionProfile* pProfile = m_pProfileEditor->GetProfile();
	m_DimYEdit.SetCommitedText(PassportMetricUtils::FormatPassportMetric(pProfile->GetYDimention()).c_str());
	m_DimXEdit.SetCommitedText(PassportMetricUtils::FormatPassportMetric(pProfile->GetXDimention()).c_str());
}

void PassportProfileEditorDlg::UpdateFloor()
{
	IPassportRecognitionProfile* pProfile = m_pProfileEditor->GetProfile();
	m_Floor.SetFloorSize(FloatSize(pProfile->GetXDimention()/10.0,pProfile->GetYDimention()/10.0));
	m_Floor.Redraw();	
}

void PassportProfileEditorDlg::OnFieldMetric(const IPassportField* pField, FIELD_METRIC metric, int value)
{
	PassportFieldEditor* pFieldEditor = m_pProfileEditor->EditField(pField->GetDescriptor()->GetFieldName());
	switch(metric)
	{
		case X_METRIC:
			pFieldEditor->SetX(std::min(value,MAX_DIMX_METRIC-MIN_FIELD_W));
			break;

		case Y_METRIC:
			pFieldEditor->SetY(std::min(value,MAX_DIMY_METRIC-MIN_FIELD_H));
			break;

		case W_METRIC:
			pFieldEditor->SetW(std::min(std::max(value,MIN_FIELD_W),MAX_DIMX_METRIC));
			break;
		case H_METRIC:
			pFieldEditor->SetH(std::min(std::max(value,MIN_FIELD_H),MAX_DIMY_METRIC));
			break;
	}
	UpdateTiles();
}

void PassportProfileEditorDlg::UpdateTiles()
{
	IPassportRecognitionProfile* pProfile = m_pProfileEditor->GetProfile();
	PtrEnumerator<IPassportField> fieldEnum = pProfile->EnumFields();
	while(fieldEnum.hasNext())
	{
		Tile* pFieldTile = m_Floor.FindTile(fieldEnum->GetDescriptor()->GetFieldName());
		PassportFieldMetrics metrics = fieldEnum->GetFieldMetrics();
		pFieldTile->SetOrd(FloatPoint(metrics.GetX() / 10.0, metrics.GetY() / 10.0));
		pFieldTile->SetSize(FloatSize(metrics.GetW() / 10.0, metrics.GetH() / 10.0));
		fieldEnum.advance();
	}
	m_Floor.Redraw();

}

void PassportProfileEditorDlg::OnDimY(NMHDR* pNMHDR, LRESULT* pResult)
{
	std::wstring s = m_DimYEdit.GetText();
	double dimY = TextUtils::AlphaToFloat(s);
	int dimYMetric = (int)(dimY*100.0);
	dimYMetric = std::min(dimYMetric,MAX_DIMY_METRIC);
	dimYMetric = std::max(dimYMetric,MIN_DIMY_METRIC);

	m_pProfileEditor->SetYDimention(dimYMetric);
	UpdateFloor();
	UpdateDims();
	*pResult = (LRESULT)0;
}

void PassportProfileEditorDlg::OnDimX(NMHDR* pNMHDR, LRESULT* pResult) 
{
	std::wstring s = m_DimXEdit.GetText();

	double dimX = TextUtils::AlphaToFloat(s);
	int dimXMetric = (int)(dimX*100.0);
	dimXMetric = std::min(dimXMetric,MAX_DIMX_METRIC);
	dimXMetric = std::max(dimXMetric,MIN_DIMX_METRIC);
	
	m_pProfileEditor->SetXDimention(dimXMetric);
	UpdateFloor();
	UpdateDims();
	*pResult = (LRESULT)0;
}