#include "stdafx.h"
#include "ExcelExportSettingsDlg.h"

#include <string/TempStringLoader.h>


ExcelExportSettingsDlg::ExcelExportSettingsDlg(ExcelExportSettings& settings,CWnd* pParent /*=NULL*/)
	: CDialog(ExcelExportSettingsDlg::IDD, pParent),m_Settings(settings)
{
}

ExcelExportSettingsDlg::~ExcelExportSettingsDlg()
{
	
}

void ExcelExportSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX,IDC_OPTIONSLIST,m_checkList);
	DDX_Check(pDX,IDC_EACHDOCPERSHEET,m_bPutDocsInSeperateSheets);
}


BEGIN_MESSAGE_MAP(ExcelExportSettingsDlg, CDialog)
	ON_CLBN_CHKCHANGE(IDC_OPTIONSLIST,OnCheckChange)
	ON_BN_CLICKED(IDC_MOVEUP,OnMoveUp)
	ON_BN_CLICKED(IDC_MOVEDOWN,OnMoveDown)
END_MESSAGE_MAP()


BOOL ExcelExportSettingsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	const struct OPTIONDESCRIPTOR
	{
		UINT optionDescResId;
		UINT optionId;
	} fieldOptions[] =
	{
		{IDS_HEADERSTATE,			COWS_SORT_BY_STATE},
		{IDS_HEADERCOWNO,			COWS_SORT_BY_EAN},
		{IDS_HEADERPASSNO,			COWS_SORT_BY_PASSNO},
		{IDS_HEADERDELIVER,			COWS_SORT_BY_DELIVER},
		{IDS_HEADERFSTOWNER,		COWS_SORT_BY_FSTOWNER},
		{IDS_HEADERBUYER,			COWS_SORT_BY_BUYER},
		{IDS_HEADERINDATE,			COWS_SORT_BY_INDATE},
		{IDS_HEADEROUTDATE,			COWS_SORT_BY_OUT_DATE},
		{IDS_HEADERWEIGHT,			COWS_SORT_BY_WEIGHT},
		{IDS_HEADERSEX,				COWS_SORT_BY_SEX},
		{IDS_HEADERSTOCK,			COWS_SORT_BY_STOCK},
		{IDS_HEADERINDOCNO,			COWS_SORT_BY_INDOCNO},
		{IDS_HEADEROUTDOCNO,		COWS_SORT_BY_OUTDOCNO},
		{IDS_HEADERMYPRICE,			COWS_SORT_BY_MYPRICE},
		{IDS_HEADERRRINV,			COWS_SORT_BY_RRINV},
		{IDS_HEADERBUYPRICE,		COWS_SORT_BY_BUYPRICE},
		{IDS_HEADERSELLPRICE,		COWS_SORT_BY_SELLPRICE},
		{IDS_HEADERTERMBUYSTOCK,	COWS_SORT_BY_TERMBUYSTOCK},
		{IDS_HEADERTERMBUYWEIGHT,	COWS_SORT_BY_TERMBUYWEIGHT},
		{IDS_HEADERTERMBUYPRICE,	COWS_SORT_BY_TERMBUYPRICE},
		{IDS_HEADERTERMSELLSTOCK,	COWS_SORT_BY_TERMSELLSTOCK},
		{IDS_HEADERTERMSELLWEIGHT,	COWS_SORT_BY_TERMSELLWEIGHT},
		{IDS_HEADERTERMSELLPRICE,	COWS_SORT_BY_TERMSELLPRICE},
		{IDS_HEADEREXTRAS,			COWS_SORT_BY_EXTRAS},
		{0,0}
	};
	

	CCheckOption* pCheckOption = NULL;
	const OPTIONDESCRIPTOR* pOptionDescriptor = fieldOptions;	
	while(pOptionDescriptor->optionId)
	{
		m_checkList.AddOption(pOptionDescriptor->optionId,CTempStringLoader(pOptionDescriptor->optionDescResId),true);		
		pOptionDescriptor++;
	}

	return TRUE;
}

bool ExcelExportSettingsDlg::Configure()
{
	return (DoModal() == IDOK);
}

void ExcelExportSettingsDlg::OnOK()
{
	UpdateData();
	m_Settings.RemoveAllFields();
	m_Settings.SetPutDocsInSeperateSheet(m_bPutDocsInSeperateSheets == TRUE);
	for(int i = 0, count = m_checkList.GetCount();i < count;i++)
	{
		CCheckOption* pCheckOption = (CCheckOption*)m_checkList.GetItemData(i);
		if(pCheckOption->GetOptionValue())
			m_Settings.AddField((COWDATA_MEMBERS)pCheckOption->GetOptionId());		
	}
	EndDialog(IDOK);

}

void ExcelExportSettingsDlg::OnCheckChange()
{
	m_checkList.UpdateOptions();
}


void ExcelExportSettingsDlg::OnMoveUp()
{
	m_checkList.MoveItemUp(m_checkList.GetCurSel());
}

void ExcelExportSettingsDlg::OnMoveDown()
{
	m_checkList.MoveItemDown(m_checkList.GetCurSel());
}