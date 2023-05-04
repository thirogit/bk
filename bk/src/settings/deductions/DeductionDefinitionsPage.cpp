#include "stdafx.h"
#include "DeductionDefinitionsPage.h"
#include "../../ContextAccess.h"
#include <boost/foreach.hpp>
#include "../../ctrls/layoutlistctrl/ListHeaderDescriptor.h"
#include "DeductionDefinitionDlg.h"
#include "../../utils/StringLoader.h"
#include "../../ui/ExceptionMsgBox.h"
#include "../../context/ContextException.h"

IMPLEMENT_DYNAMIC(DeductionDefinitionsPage, SettingsPropPage)

DeductionDefinitionsPage::DeductionDefinitionsPage() : SettingsPropPage(DeductionDefinitionsPage::IDD)
{
}

DeductionDefinitionsPage::~DeductionDefinitionsPage()
{
}

void DeductionDefinitionsPage::DoDataExchange(CDataExchange* pDX)
{
	SettingsPropPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DEDUCTIONDEFSLIST, m_List);
}


BEGIN_MESSAGE_MAP(DeductionDefinitionsPage, SettingsPropPage)
	ON_BN_CLICKED(IDC_BUGTRAQREMOVEBUTTON, OnRemove)
	ON_BN_CLICKED(IDC_BUGTRAQEDITBUTTON, OnEdit)
	ON_BN_CLICKED(IDC_BUGTRAQADDBUTTON, OnAdd)
	ON_BN_CLICKED(IDC_BUGTRAQCOPYBUTTON, OnCopy)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DEDUCTIONDEFSLIST, OnLvnItemChanged)
	ON_NOTIFY(NM_DBLCLK, IDC_DEDUCTIONDEFSLIST, OnNMDblClk)
END_MESSAGE_MAP()

BOOL DeductionDefinitionsPage::OnInitDialog()
{
	SettingsPropPage::OnInitDialog();

	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER | LVS_EX_INFOTIP | LVS_EX_GRIDLINES);

	ListHeaderDescriptor header;
	
	header(DeductionDefinitionViewMember::Code, StringLoader(IDS_HEADER_DEDUCTIONCODE))
		(DeductionDefinitionViewMember::Fraction, StringLoader(IDS_HEADER_DEDUCTIONFRACTION))
		(DeductionDefinitionViewMember::Reason, StringLoader(IDS_HEADER_DEDUCTIONREASON))		
		;

	m_List.SetHeaderDescriptor(header);

	m_List.SetContent(&m_ListContent);

	ReloadDeductionsList();

	return TRUE;
}

void DeductionDefinitionsPage::OnRemove()
{
	// traversing from the end to the beginning so that the indices are not skipped
	int index = m_List.GetItemCount()-1;
	while (index >= 0)
	{
		if (m_List.GetItemState(index, LVIS_SELECTED) & LVIS_SELECTED)
		{
			DeductionDefinitionRow* row = (DeductionDefinitionRow*)m_ListContent.GetRow(index);
			ItemId id = row->Content()->GetItemId();
			m_ListContent.DeleteAt(index);

			CRUDDeductionDefinition* definition = m_definitions.find_item(id);

			if (definition->GetFlag() == CRUDFlag_New)
			{
				m_definitions.remove_item(id);
				delete definition;
			}
			else
			{
				definition->ChangeFlag(CRUDFlag_Deleted);
			}
			
			SetModified();
		}
		index--;
	}
}

void DeductionDefinitionsPage::OnEdit()
{
	if (m_List.GetSelectedCount() != 1)
		return;

	int index = m_List.GetNextItem(-1, LVNI_SELECTED);
	if (index == -1)
		return;

	
	DeductionDefinitionRow* row = (DeductionDefinitionRow*)m_ListContent.GetRow(index);
	ItemId itemId = row->Content()->GetItemId();

	CRUDDeductionDefinition* crudDefinition = m_definitions.find_item(itemId);


	CDeductionDefinitionDlg dlg(crudDefinition, this);
	if (dlg.DoModal() == IDOK)
	{
		m_ListContent.UpdateAt(index,crudDefinition);
		crudDefinition->ChangeFlag(CRUDFlag_Updated);
		SetModified();
	}
}

void DeductionDefinitionsPage::OnAdd()
{	
	CRUDDeductionDefinition* crudDefinition = new CRUDDeductionDefinition(m_IdGenerator());	

	CDeductionDefinitionDlg dlg(crudDefinition, this);
	if (dlg.DoModal() == IDOK)
	{
		m_definitions.add_item(crudDefinition);
		m_ListContent.Add(crudDefinition);
		SetModified();
	}
}

void DeductionDefinitionsPage::OnLvnItemChanged(NMHDR * /*pNMHDR*/, LRESULT *pResult)
{
	UINT count = m_List.GetSelectedCount();
	GetDlgItem(IDC_BUGTRAQREMOVEBUTTON)->EnableWindow(count > 0);
	GetDlgItem(IDC_BUGTRAQEDITBUTTON)->EnableWindow(count == 1);
	GetDlgItem(IDC_BUGTRAQCOPYBUTTON)->EnableWindow(count == 1);
	*pResult = 0;
}

void DeductionDefinitionsPage::OnNMDblClk(NMHDR * /*pNMHDR*/, LRESULT *pResult)
{
	OnEdit();
	*pResult = 0;
}

BOOL DeductionDefinitionsPage::OnApply()
{
	UpdateData();

	try
	{
		SaveDeductions();
		ReloadDeductionsList();

		SetModified(FALSE);
		return SettingsPropPage::OnApply();
	}
	catch (ContextException& e)
	{
		ExceptionMsgBox().Show(e);
		return FALSE;
	}	
}

void DeductionDefinitionsPage::SaveDeductions()
{
	XContext* context = ContextAccess::GetXContext();

	BOOST_FOREACH(CRUDDeductionDefinition* definition, m_definitions)
	{
		switch (definition->GetFlag())
		{
		case CRUDFlag_Updated:
			context->UpdateDeductionDefinition(definition->Obj());
			break;
		case CRUDFlag_Deleted:
			context->DeleteDeductionDefinition(definition->Obj()->GetId());
			break;
		case CRUDFlag_New:
			context->SaveNewDeductionDefinition(definition->Obj());
			break;
		}
	}
}

void DeductionDefinitionsPage::ReloadDeductionsList()
{
	DeepDelete(m_definitions);
	m_definitions.clear();

	XContext* context = ContextAccess::GetXContext();
	DeductionDefinitions definitions = context->GetDeductionDefinitions();

	BOOST_FOREACH(DeductionDefinition* definition, definitions)
	{
		m_definitions.add_item(new CRUDDeductionDefinition(m_IdGenerator(), definition));
	}

	m_ListContent.DeleteAll();
	m_ListContent.AddAll(CRUDDeductionDefinitionRange(m_definitions));	

}

void DeductionDefinitionsPage::OnCopy()
{
	if (m_List.GetSelectedCount() != 1)
		return;

	int index = m_List.GetNextItem(-1, LVNI_SELECTED);
	if (index == -1)
		return;

	DeductionDefinitionRow* row = (DeductionDefinitionRow*)m_ListContent.GetRow(index);
	ItemId itemId = row->Content()->GetItemId();

	CRUDDeductionDefinition* crudDefinition = m_definitions.find_item(itemId);

	DeductionDefinition clone(crudDefinition->Obj());

	CRUDDeductionDefinition* crudClone = new CRUDDeductionDefinition(m_IdGenerator(), &clone);
		
	CDeductionDefinitionDlg dlg(crudClone, this);
	if (dlg.DoModal() == IDOK)
	{
		m_definitions.add_item(crudDefinition);
		m_ListContent.Add(crudDefinition);
		SetModified();
	}
	else
	{
		delete crudClone;
	}
}
