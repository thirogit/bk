#include "stdafx.h"
#include "DataSettingsPage.h"
#include "../../../utils/StringLoader.h"
#include "../../../resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CDataSettingsPage, CPropertyPage)

CDataSettingsPage::CDataSettingsPage(DataSettings *pDataSettings) 	: CPropertyPage(CDataSettingsPage::IDD)
{
	m_pDataSettings = pDataSettings;	
}

CDataSettingsPage::~CDataSettingsPage()
{
}

void CDataSettingsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Check(pDX,IDC_UPPERCASEHENTDATA,m_bUppercaseHent);
	DDX_Control(pDX, IDC_FIELDSELECTION, m_FieldSelection);
	DDX_Check(pDX,IDC_NOUPDATEIFPRESENT,m_bDoNotUpdateValueIfPresent);
	
}


BEGIN_MESSAGE_MAP(CDataSettingsPage, CPropertyPage)
	
END_MESSAGE_MAP()


BOOL CDataSettingsPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();


	const struct
	{
		DataSettings::DATA_FIELD fieldId;
		UINT nFieldNameRsrcId;
	}
	optionalFieldNames[] =
	{
		{ DataSettings::PASSPORT_DATE,		IDS_FIELDNAME_PASSDATE },
		{ DataSettings::PASSPORT_NO,		IDS_FIELDNAME_PASSNO },
		{ DataSettings::BIRTH,				IDS_FIELDNAME_BIRTH },
		{ DataSettings::SEX,				IDS_FIELDNAME_SEX },
		{ DataSettings::STOCK,				IDS_FIELDNAME_STOCK },
		{ DataSettings::MOTHER_NO,			IDS_FIELDNAME_MOTHEREAN },
		{ DataSettings::FIRST_OWNER,		IDS_FIELDNAME_FSTOWNRADDR },
		{ (DataSettings::DATA_FIELD)0, 0 }
	};

	int i = 0,item;
	while (optionalFieldNames[i].nFieldNameRsrcId || optionalFieldNames[i].fieldId)
	{

		item = m_FieldSelection.AddString(StringLoader(optionalFieldNames[i].nFieldNameRsrcId).c_str());
		m_FieldSelection.SetCheck(item, m_pDataSettings->IsFieldEnabled(optionalFieldNames[i].fieldId) ? 1 : 0);
		m_FieldSelection.SetItemData(item,optionalFieldNames[i].fieldId);
		i++;
	}


	m_bUppercaseHent = m_pDataSettings->GetDoUppercaseHent();
	m_bDoNotUpdateValueIfPresent = m_pDataSettings->GetDoNotUpdateValueIfPresent();
	
	UpdateData(FALSE);
	return TRUE;
}

void CDataSettingsPage::OnOK()
{
	UpdateData();

	m_pDataSettings->SetDoNotUpdateValueIfPresent(m_bDoNotUpdateValueIfPresent == TRUE);
	m_pDataSettings->SetDoUppercaseHent(m_bUppercaseHent == TRUE); 	
	

	DataSettings::DATA_FIELD field;

	for (int i = 0, count = m_FieldSelection.GetCount(); i < count; i++)
	{
		field = (DataSettings::DATA_FIELD)m_FieldSelection.GetItemData(i);
		m_pDataSettings->SetOptionalFieldEnabled(field, m_FieldSelection.GetCheck(i));
	}

	
}
