#include "stdafx.h"
#include "PrintRecordOrderDlg.h"
#include <boost/foreach.hpp>
#include "../../resource.h"
#include "../../utils/StringLoader.h"
#include "../../ctrls/layoutlistctrl/ListHeaderDescriptor.h"
#include "../../ui/cell/ComboCellBuilder.h"


#define ASC_ID ((uint32_t)1)
#define DESC_ID ((uint32_t)(-1));


PrintRecordOrderDlg::SortingOrderListRow::SortingOrderListRow(const OrderSpecifier* specifier, const std::wstring& fieldName) : m_fieldName(fieldName)
{
	m_Specifier = new OrderSpecifier(*specifier);
}

PrintRecordOrderDlg::SortingOrderListRow::~SortingOrderListRow()
{
	delete m_Specifier;
}

std::wstring PrintRecordOrderDlg::SortingOrderListRow::CellValue(uint32_t colId)
{
	switch (colId)
	{
	case SortintOrderListColumn_FieldName:
		return m_fieldName;
	case SortintOrderListColumn_Collation:
		if (m_Specifier->asc())
			return StringLoader(IDS_COLATION_ASC);
		else
			return StringLoader(IDS_COLATION_DESC);
	}

	return L"";
}

const OrderSpecifier* PrintRecordOrderDlg::SortingOrderListRow::Specifier() const
{
	return m_Specifier;
}


void PrintRecordOrderDlg::SortingOrderListRow::SetCollation(bool bAsc)
{
	m_Specifier->asc(bAsc);
}

PrintRecordOrderDlg::SortingOrderListContent::~SortingOrderListContent()
{
	DeleteAllRows();
}

PrintRecordOrderDlg::SortingOrderListRow* PrintRecordOrderDlg::SortingOrderListContent::AddRow(const OrderSpecifier* specifier,const std::wstring& fieldName)
{
	PrintRecordOrderDlg::SortingOrderListRow* row = new PrintRecordOrderDlg::SortingOrderListRow(new OrderSpecifier(*specifier),fieldName);
	m_content.push_back(row);
	NotifyCountChanged();
	return row;
}

void PrintRecordOrderDlg::SortingOrderListContent::DeleteRow(int row)
{
	m_content.erase(m_content.begin() + row);
	NotifyCountChanged();
}

void PrintRecordOrderDlg::SortingOrderListContent::MoveRow(int rowIndex, int positions)
{
	int newRow = rowIndex + positions;
	newRow = min(newRow, m_content.size() - 1);
	newRow = max(0, newRow);

	auto srcIt = m_content.begin() + rowIndex;
	SortingOrderListRow* row = *srcIt;

	m_content.erase(srcIt);
	m_content.insert(m_content.begin() + newRow, row);
	NotifyContentUpdated();
}

void PrintRecordOrderDlg::SortingOrderListContent::DeleteAllRows()
{
	DeepDelete(m_content);
	m_content.clear();
	NotifyCountChanged();
}

int PrintRecordOrderDlg::SortingOrderListContent::GetRowCount() const
{
	return m_content.size();
}

ListRow* PrintRecordOrderDlg::SortingOrderListContent::GetRow(int row)
{
	return m_content.at(row);
}

void PrintRecordOrderDlg::SortingOrderListContent::SortContent(const SortAttributes& attributes)
{
}

void PrintRecordOrderDlg::SortingOrderListContent::SetCollation(int row, bool bAsc)
{
	m_content.at(row)->SetCollation(bAsc);
	NotifyContentUpdated();
}

const PrintRecordOrderDlg::SortingOrderListRow* PrintRecordOrderDlg::SortingOrderListContent::GetSortingOrderRowAt(int row) const
{
	return m_content.at(row);
}

const PrintRecordOrderDlg::SortingOrderListRow* PrintRecordOrderDlg::SortingOrderListContent::GetRowForField(const std::wstring& fieldCd) const
{
	const boost::multi_index::index<ContentIndex, index_by_code>::type& indexByFieldCode = m_content.get<index_by_code>();
	
	auto fieldIt = indexByFieldCode.find(fieldCd);
	if (indexByFieldCode.end() != fieldIt)
	{
		return *fieldIt;
	}
	return NULL;
}

//#######################################################################################

BEGIN_MESSAGE_MAP(PrintRecordOrderDlg, CDialog)

	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_ADD,OnAdd)
	ON_BN_CLICKED(IDC_MOVEUP, OnMoveUp)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_MOVEDOWN, OnMoveDown)


END_MESSAGE_MAP()


PrintRecordOrderDlg::PrintRecordOrderDlg(const DocumentCfgDescription* configuration, PrintProfileEditor* pEditor, CWnd* pParent/* = NULL*/) :
CDialog(PrintRecordOrderDlg::IDD, pParent), m_configuration(configuration), m_pEditor(pEditor)
{
}

void PrintRecordOrderDlg::Create(CWnd* pParent)
{
	CDialog::Create(PrintRecordOrderDlg::IDD, pParent);
}

void PrintRecordOrderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMPARABLEFIELDS, m_ComparableFieldsCb);
	DDX_Control(pDX, IDC_LIST, m_sortingOrderList);
	DDX_Control(pDX, IDC_MOVEUP, m_moveUpBtn);
	DDX_Control(pDX, IDC_MOVEDOWN, m_moveDownBtn);
}


BOOL PrintRecordOrderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ListHeaderDescriptor header;
	header(SortintOrderListColumn_FieldName, StringLoader(IDS_PROPERTY));
	header(SortintOrderListColumn_Collation, StringLoader(IDS_COLLATION));
	m_sortingOrderList.SetHeaderDescriptor(header);

	std::map<std::wstring, ComparableFieldDefinition*> comparableFieldsMap;

	ComparableFieldDefinition* cmpFieldDefinition;
	PtrEnumerator<ComparableFieldDefinition> cmpFieldEnum = m_configuration->EnumComparableFieldDefinitions();
	while (cmpFieldEnum.hasNext())
	{
		cmpFieldDefinition = *cmpFieldEnum;
		comparableFieldsMap[cmpFieldDefinition->GetCode()] = cmpFieldDefinition;
		cmpFieldEnum.advance();
	}
		
	std::wstring fieldCode;
	OrderSpecifier* orderSpecifier;

	for (int i = 0, count = m_pEditor->GetOrderSpecifiersCount(); i < count; i++)
	{
		orderSpecifier = m_pEditor->GetOrderSpecifierAt(i);
		fieldCode = orderSpecifier->fieldCode();
		auto cmpFieldIt = comparableFieldsMap.find(fieldCode);

		if (cmpFieldIt != comparableFieldsMap.end())
		{
			m_Content.AddRow(orderSpecifier, cmpFieldIt->second->GetName());
		}
	}
	
	ReloadComparableFieldsCombo();



	std::function<void(int, uint32_t, CellDescriptor&)> handler = [this](int row, uint32_t colId, CellDescriptor& cell)
	{
		OnCellDblClick(row, colId, cell);
	};
	m_sortingOrderList.SetCellDblClickHandler(handler);

	m_sortingOrderList.SetItemHeight(20);
	m_sortingOrderList.SetContent(&m_Content);
	m_moveDownBtn.SetArrowStyle(ABS_ARROW_DOWN);

	return TRUE;
}

void PrintRecordOrderDlg::OnCellDblClick(int row, uint32_t colId, CellDescriptor& cell)
{
	const uint32_t ascId = ASC_ID;
	const uint32_t descId = DESC_ID;

	const PrintRecordOrderDlg::SortingOrderListRow* rowContent = m_Content.GetSortingOrderRowAt(row);
	bool bAsc = rowContent->Specifier()->asc();
	
	if (colId == SortintOrderListColumn_Collation)
	{
		ComboCellBuilder::SelectReceiver receiver = std::bind(&PrintRecordOrderDlg::OnCollationChanged, this, row, std::placeholders::_1);

		ComboCellBuilder cbBuilder;
		cbBuilder.add(ascId, StringLoader(IDS_COLATION_ASC))
			.add(descId, StringLoader(IDS_COLATION_DESC))
			.select(bAsc ? ascId : descId)
			.receiver(receiver)
			.show(&m_sortingOrderList, cell.GetRect());

	}
}

void PrintRecordOrderDlg::OnCollationChanged(int row, uint32_t collationId)
{
	m_Content.SetCollation(row, collationId == ASC_ID);
}

void PrintRecordOrderDlg::ReloadComparableFieldsCombo()
{
	m_ComparableFieldsCb.ResetContent();
		
	int item;
	ComparableFieldDefinition* cmpFieldDefinition;
	PtrEnumerator<ComparableFieldDefinition> cmpFieldEnum = m_configuration->EnumComparableFieldDefinitions();
	while (cmpFieldEnum.hasNext())
	{
		cmpFieldDefinition = *cmpFieldEnum;
		
		if (m_Content.GetRowForField(cmpFieldDefinition->GetCode()) == NULL) {
			item = m_ComparableFieldsCb.AddString(cmpFieldDefinition->GetName().c_str());
			m_ComparableFieldsCb.SetItemDataPtr(item, cmpFieldDefinition);
		}
		
		cmpFieldEnum.advance();
	}
}

void PrintRecordOrderDlg::OnDestroy()
{
	CDialog::OnDestroy();
	
}

void PrintRecordOrderDlg::OnAdd()
{
	int item = m_ComparableFieldsCb.GetCurSel();
	if (item >= 0)
	{
		ComparableFieldDefinition* definition = (ComparableFieldDefinition*)m_ComparableFieldsCb.GetItemDataPtr(item);
		OrderSpecifier specifier(definition->GetCode());
		m_Content.AddRow(&specifier, definition->GetName());
		ReloadComparableFieldsCombo();
	}
	
}

void PrintRecordOrderDlg::OnMoveUp()
{
	int row = m_sortingOrderList.GetCurSel();
	if (row > 0)
	{
		m_Content.MoveRow(row, 1);
	}
}

void PrintRecordOrderDlg::OnDelete()
{
	int row = m_sortingOrderList.GetCurSel();
	if (row >= 0)
	{
		m_Content.DeleteRow(row);
		ReloadComparableFieldsCombo();
	}
}

void PrintRecordOrderDlg::OnMoveDown()
{
	int row = m_sortingOrderList.GetCurSel();
	if (row < m_Content.GetRowCount()-1)
	{
		m_Content.MoveRow(row,-1);
	}
}

void PrintRecordOrderDlg::OnOK()
{
}

void PrintRecordOrderDlg::OnCancel()
{
}

void PrintRecordOrderDlg::Apply()
{
	m_pEditor->ClearOrderSpecifiers();
	for (int i = 0, count = m_Content.GetRowCount(); i < count;i++)
	{
		const SortingOrderListRow* row = m_Content.GetSortingOrderRowAt(i);
		m_pEditor->AddOrderSpecifier(*row->Specifier());
	}

}