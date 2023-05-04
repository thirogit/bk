#include "stdafx.h"
#include "LockerDlg.h"
#include "../ui/waitdlg\WaitDialog.h"
#include "../utils/StringLoader.h"

BEGIN_MESSAGE_MAP(CLockerDlg, CLazyDlg<CDialog>)
	
	ON_BN_CLICKED(IDC_TRYLOCK,OnTryLock)

END_MESSAGE_MAP()

enum AttemptListColumn : uint32_t
{
	AttemptListColumn_Descriptor,
	AttemptListColumn_LockStatus
};


CLockerDlg::CLockerDlg(RecordLocker* locker,CWnd* pParent/* = NULL*/) : m_locker(locker), CLazyDlg<CDialog>(CLockerDlg::IDD,pParent)
{
}

CLockerDlg::~CLockerDlg()
{
}


std::wstring CLockerDlg::GetCellText(int row,uint32_t colId)
{
	const LockAttempt* attempt = m_locker->GetAttemptAt(row);

	switch (colId)
	{
		case AttemptListColumn_Descriptor:
			return attempt->GetDescriptor();		
		case AttemptListColumn_LockStatus:
			{
				switch(attempt->GetStatus())
				{
				case LockAttemptStatus_Failed:
					return attempt->GetErrorMsg();	
				case LockAttemptStatus_InProgress:
					return StringLoader(IDS_WAITLOCKING);
				case LockAttemptStatus_Locked:
					return StringLoader(IDS_INVOICELOCKED);
				}
			}
	}

	return std::wstring();
}

BOOL CLockerDlg::OnInitDialog()
{
	CLazyDlg<CDialog>::OnInitDialog();


	ListHeaderDescriptor header;
	header(AttemptListColumn_Descriptor,StringLoader(IDS_HEADER_RECORD));
	header(AttemptListColumn_LockStatus,StringLoader(IDS_HEADER_RESULT));

	m_attemptsList.SetHeaderDescriptor(header);
	

	std::function<std::wstring (int,uint32_t)> textresolver = 
		std::bind(&CLockerDlg::GetCellText,this,std::placeholders::_1,std::placeholders::_2);


	std::function<Color (int,uint32_t)> clrresolver = 
		std::bind(&CLockerDlg::GetCellColor,this,std::placeholders::_1,std::placeholders::_2);

	std::function<int (int,uint32_t)> imgresolver = 
		std::bind(&CLockerDlg::GetCellImage,this,std::placeholders::_1,std::placeholders::_2);

	m_attemptsList.SetCellTextResolver(textresolver);
	m_attemptsList.SetCellColorResolver(clrresolver);
//	m_attemptsList.SetCellImageResolver(imgresolver);	


	//BOOL b = m_statusIcons.Create(IDB_TASKSTATUS,16,16,1,RGB(255,0,255));
	//CBitmap question;
	//question.LoadBitmap(IDB_TASKSTATUS);
	//int r = m_statusIcons.Add(&question,RGB(255,0,255));

	//m_attemptsList.SetImageList(&m_statusIcons,LVSIL_NORMAL);
	m_attemptsList.SetItemCount(m_locker->Count());


	return TRUE;
}

int CLockerDlg::GetCellImage(int row,uint32_t colId)
{
	return (colId == AttemptListColumn_Descriptor) ? 0 : -1;
}

Color CLockerDlg::GetCellColor(int row,uint32_t colId)
{
	const LockAttempt* attempt = m_locker->GetAttemptAt(row);

	switch(attempt->GetStatus())
	{
		case LockAttemptStatus_Failed:
			return Color(255,0,0);		
		case LockAttemptStatus_Locked:
			return Color(34,177,76);		
		case LockAttemptStatus_InProgress:
			return Color(0,0,255);
	}

	return Color();
}

void CLockerDlg::DoDataExchange(CDataExchange* pDX)
{
	CLazyDlg<CDialog>::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_INVOICELIST,m_attemptsList);
}

void CLockerDlg::OnAfterDlgShow()
{
	m_locker->AddObserver((RecordLockerObserver*)this);

	CWaitDialog waitDlg(this,StringLoader(IDS_WAITLOCKING));

	waitDlg.WaitForTask(boost::bind(&CLockerDlg::LockRecords,this));

	m_locker->RemoveObserver((RecordLockerObserver*)this);
}

void CLockerDlg::OnLockAttempt(const LockAttempt* attempt)
{
	m_attemptsList.Invalidate(FALSE);
}

void CLockerDlg::LockRecords()
{
	for(int i = 0,count = m_locker->Count(); i < count;i++)
	{
		m_locker->AttemptLockAt(i);		
	}
}

void CLockerDlg::OnOK()
{
	for(int i = 0,count = m_locker->Count(); i < count;i++)
	{
		if(m_locker->GetAttemptAt(i)->GetStatus() != LockAttemptStatus_Locked)
		{
			AfxMessageBox(IDS_FAILEDTOLOCKALLRECORDS);
			return;
		}
	}
	EndDialog(IDOK);
}

void CLockerDlg::OnTryLock()
{
	int nsel = m_attemptsList.GetCurSel();
	if(nsel >= 0)
		m_locker->AttemptLockAt(nsel);
}