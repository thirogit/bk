#include "stdafx.h"
#include "PassportProfileFieldsListCtrl.h"
#include <configuration/passportrecognition/PassportFieldDescriptor.h>
#include <configuration/passportrecognition/PassportFields.h>
#include <boost/assign/list_of.hpp>
#include "../utils/PassportMetricUtils.h"
#include <list>
#include <boost/tuple/tuple.hpp>
#include "../../../resource.h"
#include "../../../utils/StringLoader.h"
#include "../../../ui/cell/NumericCellBuilder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace PassportMetricUtils;


///////////////////////////////////////////////////////////////////////////////////////////////////
PassportProfileFieldsListCtrl::FieldItem::FieldItem(const std::wstring& name,const IPassportField* f)
{
	field = f;
	fieldname = name;
}
	
///////////////////////////////////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(PassportProfileFieldsListCtrl, COwnrDrwListCtrl)
	
END_MESSAGE_MAP()

PassportProfileFieldsListCtrl::PassportProfileFieldsListCtrl(PassportRecognitionProfileEditor *pProfileEditor) : m_pProfileEditor(pProfileEditor)
{
	
}

PassportProfileFieldsListCtrl::~PassportProfileFieldsListCtrl()
{
	m_Items.FreeAndFlush();
}



void PassportProfileFieldsListCtrl::UpdateItemCount()
{
	SetItemCount(m_Items.size());
}


std::wstring PassportProfileFieldsListCtrl::GetCellText(int row, uint32_t colId)
{
	FieldItem *pItem = m_Items.GetAt(row);
	const IPassportField* pField = pItem->field;
	PassportFieldMetrics metrics = pField->GetFieldMetrics();
	switch (colId)
	{
		case COL_X:
			return FormatPassportMetric(metrics.GetX());
			
		case COL_Y:		
			return FormatPassportMetric(metrics.GetY());
		
		case COL_WIDTH:
			return FormatPassportMetric(metrics.GetW());
		
		case COL_HEIGHT:
			return FormatPassportMetric(metrics.GetH());		
		case COL_FIELDNAME:
			return pItem->fieldname;		
		case COL_FIELDTYPE:			
			break;

	}
	return L"";
}

void PassportProfileFieldsListCtrl::PostCreate()
{
	

	CLayoutListCtrl::PostCreate();

	std::function<std::wstring(int, uint32_t)> textresolver =
		std::bind(&PassportProfileFieldsListCtrl::GetCellText, this, std::placeholders::_1, std::placeholders::_2);

	std::function<Color(int, uint32_t)> clrresolver =
		std::bind(&PassportProfileFieldsListCtrl::GetCellColor, this, std::placeholders::_1, std::placeholders::_2);

	std::function<void(int, uint32_t, CellDescriptor&)> dblclickHandler =
		std::bind(&PassportProfileFieldsListCtrl::OnCellDblClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

	IPassportRecognitionProfile* pProfile = m_pProfileEditor->GetProfile();

	SetCellDblClickHandler(dblclickHandler);
	SetCellTextResolver(textresolver);
	SetCellColorResolver(clrresolver);


	ListHeaderDescriptor header;
	header	(COL_FIELDNAME, StringLoader(IDS_HEADERFIELDNAME))
			(COL_X,			StringLoader(IDS_HEADERFIELDXOFFSET))
			(COL_Y,			StringLoader(IDS_HEADERFIELDYOFFSET))
			(COL_WIDTH,		StringLoader(IDS_HEADERFIELDWIDTH))
			(COL_HEIGHT,	StringLoader(IDS_HEADERFIELDHEIGHT));

	SetHeaderDescriptor(header); 

	
	std::list<boost::tuple<PassportFieldDescriptor*, int>> fields = boost::assign::tuple_list_of
		(&PassportFields::BIRTHINFO_FIELD,		IDS_FIELDNAME_BIRTH)
		(&PassportFields::EAN_FIELD,			IDS_FIELDNAME_COWNO)
		(&PassportFields::EANBARCODE_FIELD,		IDS_FIELDNAME_COWNOBC)
		(&PassportFields::FARMNOBARCODE_FIELD,	IDS_FIELDNAME_FARMNOBC)
		(&PassportFields::FIRSTOWNER_FIELD,		IDS_FIELDNAME_FSTOWNRADDR)
		(&PassportFields::MOTHEREAN_FIELD,		IDS_FIELDNAME_MOTHEREAN)
		(&PassportFields::PASSDATE_FIELD,		IDS_FIELDNAME_PASSDATE)
		(&PassportFields::PASSNO_FIELD,			IDS_FIELDNAME_PASSNO)
		(&PassportFields::SEX_FIELD,			IDS_FIELDNAME_SEX)
		(&PassportFields::STOCK_FIELD,			IDS_FIELDNAME_STOCK);
	
	auto fieldIt = fields.begin();
	m_Items.FreeAndFlush();
	std::wstring name;
	while(fieldIt != fields.end())
	{
		name = StringLoader(boost::get<1>(*fieldIt));
		m_Items.Add(new FieldItem(name,pProfile->GetFieldByName(boost::get<0>(*fieldIt)->GetFieldName())));
		fieldIt++;
	}

	VERIFY(m_FieldTypeImagesList.Create(16,16,ILC_COLORDDB,2,1));
	CBitmap textAndBarcode;
	textAndBarcode.LoadBitmap(IDB_TXT_BC_IL);
	m_FieldTypeImagesList.Add(&textAndBarcode,RGB(1,1,1));

	SetImageList(&m_FieldTypeImagesList,LVSIL_SMALL);

	UpdateItemCount();
	SetItemHeight(21);
	
	
}

int PassportProfileFieldsListCtrl::GetCellImage(int iItem, uint32_t colId)
{
	if (colId  == COL_FIELDTYPE)
	{
		FieldItem *pItem =  m_Items.GetAt(iItem);
		const IPassportField* pField = pItem->field;
		const PassportFieldDescriptor* pDescriptor = pField->GetDescriptor();
		return pDescriptor->IsBarcode() ? 1 : 0;
	}
	return -1;
}

void PassportProfileFieldsListCtrl::SortByCol(int Column,bool bAsc)
{
}

void PassportProfileFieldsListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (GetFocus() != this) 
		SetFocus();
    
	COwnrDrwListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

void PassportProfileFieldsListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (GetFocus() != this) 
		SetFocus();
	COwnrDrwListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

void PassportProfileFieldsListCtrl::OnCellDblClick(int row, uint32_t colId, CellDescriptor& cell)
{	
	FieldItem *pItem =  m_Items.GetAt(row);
	const IPassportField* pField = pItem->field;
	PassportFieldMetrics metrics = pField->GetFieldMetrics();

	switch (colId)
	{
		case COL_X:
			EditFieldMetric(pField, X_METRIC, cell.GetRect());
		break;		
		case COL_Y:		
			EditFieldMetric(pField, Y_METRIC, cell.GetRect());
		break;
		case COL_WIDTH:
			EditFieldMetric(pField, W_METRIC, cell.GetRect());
		break;
		case COL_HEIGHT:
			EditFieldMetric(pField, H_METRIC, cell.GetRect());
		break;				

	}
}
void PassportProfileFieldsListCtrl::SetMetricListener(MetricListener metricListener)
{
	m_MetricListener = metricListener;
}

void PassportProfileFieldsListCtrl::NotifyMetric(const IPassportField* pField,FIELD_METRIC metric,int value)
{
	if(!m_MetricListener)
	{
		m_MetricListener(pField,metric,value);
	}
}

void PassportProfileFieldsListCtrl::EditFieldMetric(const IPassportField* pField, FIELD_METRIC metric, const CRect& cellRect)
{
	NumericCellBuilder cell;
	int metricValue = pField->GetFieldMetrics().GetMetric(metric);

	FloatReceiver floatreceiver = std::bind(&PassportProfileFieldsListCtrl::OnSetMetricValue, this, pField, metric, std::placeholders::_1);

	cell.precision(2)
		.value(NullDouble(metricValue / 100.0))
		.receiver(floatreceiver)
		.show(this,cellRect);
}

void PassportProfileFieldsListCtrl::OnSetMetricValue(const IPassportField* pField, FIELD_METRIC metric, const NullDouble& value)
{
	if (!value.IsNull())
	{
		NotifyMetric(pField, metric, (int)(value.GetDouble() * 100));
	}
}

Color PassportProfileFieldsListCtrl::GetCellColor(int row, uint32_t colId)
{
	return Color();
}