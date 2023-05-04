#include "stdafx.h"
#include "DocumentPrintDlg.h"
#include "../../utils/StringLoader.h"
#include "../../ui/ExceptionMsgBox.h"
#include "../../resource.h"
#include "../../ui/waitdlg\WaitDialog.h"
#include <progress\SafeSimpleProgress.h>
#include <string/TextUtils.h>
#include "../../image/Png.h"
#include "PrintProfileDlg.h"
#include "PrintProfileEditor.h"
#include "../../ContextAccess.h"
#include <boost/foreach.hpp>
#include <utils\SafeGet.h>
#include "../runtime/WinPaperUtils.h"

#define WM_AFTERDLGSHOW (WM_APP+1001)

BEGIN_MESSAGE_MAP(DocumentPrintDlg, CSessionAwareDialog)
			ON_WM_WINDOWPOSCHANGED()
			ON_MESSAGE(WM_AFTERDLGSHOW,OnAfterDlgShow)
			ON_WM_DESTROY()
			ON_BN_CLICKED(IDC_DOPRINT,OnDoPrint)
			ON_BN_CLICKED(IDC_DOPRINT_2,OnDoPrint2)
			ON_BN_CLICKED(IDC_DOPRINT_3,OnDoPrint3)
			ON_WM_HSCROLL()
			ON_LBN_SELCHANGE(IDC_PROFILESLIST,OnProfileSelectChange)

END_MESSAGE_MAP()


DocumentPrintDlg::DocumentPrintDlg(DocumentInstance* instance,CWnd *pParent/* = NULL*/) : 
	CSessionAwareDialog(DocumentPrintDlg::IDD,pParent),m_instance(instance),m_pPrintOut(NULL)
{
}

void DocumentPrintDlg::DoDataExchange(CDataExchange* pDX)
{
	CSessionAwareDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_PAGESLIDER,m_pageSliderCtrl);
	DDX_Control(pDX, IDC_PRINTPROFILE_TOOLBAR, m_profilesToolbar);
	DDX_Control(pDX, IDC_PROFILESLIST, m_profilesList);
}

void DocumentPrintDlg::OnDestroy()
{
	if(m_pPrintOut)
		delete m_pPrintOut;

	CSessionAwareDialog::OnDestroy();	
}

BOOL DocumentPrintDlg::OnInitDialog()
{
	CSessionAwareDialog::OnInitDialog();	
	m_dlgshown = false;

	SetWindowText(TextUtils::Format(StringLoader(IDS_PRINTDLGCAPTIONFMT), m_instance->GetName()).c_str());

	CWnd* pPlaceholder = GetDlgItem(IDC_PRINTPREVIEW);
	CRect placeholderRect;
	pPlaceholder->GetClientRect(&placeholderRect);
	pPlaceholder->MapWindowPoints(this,&placeholderRect);
	pPlaceholder->DestroyWindow();

	m_previewCtrl.Create(WS_CHILD | WS_VISIBLE,placeholderRect,this,IDC_PRINTPREVIEW);

	CPng editImg, addImg, deleteImg;
	
	editImg.LoadPNG(IDPNG_EDIT);
	addImg.LoadPNG(IDPNG_ADD);
	deleteImg.LoadPNG(IDPNG_DELETE);
	
	//	boost::function<void()> onConnectFn = boost::bind(&CRepoAccountToolBar::OnConnect,this);
	//	boost::function<void()> onDisconnectFn = boost::bind(&CRepoAccountToolBar::OnDisconnect,this);
	boost::function<void()> onAddFn = boost::bind(&DocumentPrintDlg::OnProfileAdd, this);
	boost::function<void()> onDeleteFn = boost::bind(&DocumentPrintDlg::OnProfileDelete, this);
	boost::function<void()> onEditFn = boost::bind(&DocumentPrintDlg::OnProfileEdit, this);
	
	COLORREF bkgColor = ::GetSysColor(COLOR_3DFACE);
	//	AddButtonNoReToolBar(onConnectFn,connectImg.GetBitmap24(bkgColor),true);
	//	AddButtonNoReToolBar(onDisconnectFn,disconnectImg.GetBitmap24(bkgColor),true);
	m_profilesToolbar.AddButton(onAddFn, addImg.GetBitmap24(bkgColor), true);
	m_profilesToolbar.AddButton(onDeleteFn, deleteImg.GetBitmap24(bkgColor), true);
	m_profilesToolbar.AddButton(onEditFn, editImg.GetBitmap24(bkgColor), true);
	//m_profilesToolbar.AddButton(onEditFn, refreshImg.GetBitmap24(bkgColor), true);
		
	InitProfilesList();

	if (m_profilesList.GetCount() > 0)
	{
		m_profilesList.SetCurSel(0);		
	}

	return TRUE;
}

void DocumentPrintDlg::InitProfilesList()
{
	PrintProfiles profiles = ContextAccess::GetXContext()->GetPrintProfiles(m_instance->GetCode());

	m_profilesList.ResetContent();
	int item;
	BOOST_FOREACH(PrintProfile* profile, profiles)
	{
		item = m_profilesList.AddString(profile->GetProfileName().c_str());
		m_profilesList.SetItemData(item, profile->GetProfileId());
	}
}

void DocumentPrintDlg::OnProfileSelectChange()
{
	WaitGeneratePrintOut();
}

IPrintProfile* DocumentPrintDlg::GetSelectedPrintProfile()
{
	int nSelected = m_profilesList.GetCurSel();

	if (nSelected >= 0)
	{
		uint32_t profileId = (uint32_t)m_profilesList.GetItemData(nSelected);
		return ContextAccess::GetXContext()->GetPrintProfile(profileId);
	}

	return NULL;
}

void DocumentPrintDlg::SelectProfile(uint32_t profileId)
{
	for (int i = 0, count = m_profilesList.GetCount(); i < count; i++)
	{
		if (m_profilesList.GetItemData(i) == profileId)
		{
			m_profilesList.SetCurSel(i);
			return;
		}
	}

	m_profilesList.SetCurSel(-1);

}


void DocumentPrintDlg::OnProfileAdd()
{	
	PrintProfileEditor editor(ContextAccess::GetXContext());
	editor.New(m_instance->GetCode());
	PrintProfileDlg profileDlg(m_instance->DescribeConfiguration(),&editor, this);
	if (profileDlg.DoModal() == IDOK)
	{		
		ReloadProfilesList();
	}
}

void DocumentPrintDlg::ReloadProfilesList()
{
	IPrintProfile* selectedProfile = GetSelectedPrintProfile();
	uint32_t selectedProfileId = SafeGet(&IPrintProfile::GetProfileId, selectedProfile, NULL_ID);
	InitProfilesList();
	SelectProfile(selectedProfileId);
}

void DocumentPrintDlg::OnProfileDelete()
{
	IPrintProfile* profile = GetSelectedPrintProfile();
	if (profile != NULL)
	{
		ContextAccess::GetXContext()->DeletePrintProfile(profile->GetProfileId());
		ReloadProfilesList();
	}
}

void DocumentPrintDlg::OnProfileEdit()
{
	IPrintProfile* profile = GetSelectedPrintProfile();
	if (profile != NULL)
	{
		PrintProfileEditor editor(ContextAccess::GetXContext());
		editor.Edit(profile->GetProfileId());
		PrintProfileDlg profileDlg(m_instance->DescribeConfiguration(), &editor, this);
		if (profileDlg.DoModal() == IDOK)
		{
			ReloadProfilesList();
			WaitGeneratePrintOut();
		}
	}
}

void DocumentPrintDlg::OnWindowPosChanged(WINDOWPOS* windowpos)
{
	if((windowpos->flags & SWP_SHOWWINDOW) && !m_dlgshown)
	{
		m_dlgshown = true;
		PostMessage(WM_AFTERDLGSHOW);
	}
	CSessionAwareDialog::OnWindowPosChanged(windowpos);
}

LRESULT DocumentPrintDlg::OnAfterDlgShow(WPARAM wParam,LPARAM lParam)
{	
	WaitGeneratePrintOut();
	return (LRESULT)0;
}

void DocumentPrintDlg::WaitGeneratePrintOut()
{
	CWaitDialog waitDlg(this, StringLoader(IDS_GENERATINGPRINTOUT));


	try
	{
		waitDlg.WaitForTask(boost::bind(&DocumentPrintDlg::GeneratePrintOut, this));
	}
	catch (wexception& e)
	{
		ExceptionMsgBox().Show(e);
	}
}

void DocumentPrintDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if(nSBCode != SB_ENDSCROLL)
	{
		int scrollToPage = m_pageSliderCtrl.GetPos()-1;
		if(m_previewCtrl.GetCurrentPage() != scrollToPage)
		{
			UpdatePagesIndicator();
			m_previewCtrl.SetCurrentPage(scrollToPage,TRUE);
		}
	}
}

void DocumentPrintDlg::GeneratePrintOut()
{
	if(m_pPrintOut)
		delete m_pPrintOut;

	IPrintProfile* currentProfile = GetSelectedPrintProfile();

	m_pPrintOut = new bkprint::PrintOut(std::move(m_instance->Print(currentProfile)));
	m_previewCtrl.SetPrintOut(m_pPrintOut);

	m_pageSliderCtrl.SetRange(1,m_pPrintOut->GetNumberOfPages(),TRUE);
	m_pageSliderCtrl.SetPos(1);
	UpdatePagesIndicator();
}

void DocumentPrintDlg::UpdatePagesIndicator()
{
	std::wstring sPagesIndicator = TextUtils::Format(L"%d/%d",m_pageSliderCtrl.GetPos(),m_pageSliderCtrl.GetRangeMax());
	SetDlgItemText(IDC_PAGEINDICATOR,sPagesIndicator.c_str());
}

void DocumentPrintDlg::DoPrint(int nCopies)
{
	if(!m_pPrintOut)
		return;

	CPrintInfo printInfo;
	CWinApp* pApp = AfxGetApp();

	printInfo.SetMinPage(1);
	printInfo.SetMaxPage(m_pPrintOut->GetNumberOfPages());
			
	printInfo.m_pPD->m_pd.nFromPage = (WORD)printInfo.GetMinPage();
	printInfo.m_pPD->m_pd.nToPage = (WORD)printInfo.GetMaxPage();

	if (pApp->GetPrinterDeviceDefaults(&printInfo.m_pPD->m_pd))
	{
		LPDEVMODE pDevMode=printInfo.m_pPD->GetDevMode();

		if (printInfo.m_pPD->m_pd.Flags & PD_USEDEVMODECOPIES)
		{			
			pDevMode->dmCopies = nCopies;
		}
		else
		{
			printInfo.m_pPD->m_pd.nCopies = nCopies;
		}

		bkprint::Paper paper = m_pPrintOut->GetPaper();
		pDevMode->dmOrientation = WinPaperUtils::GetOrientation(paper.GetOrientation());
		pDevMode->dmPaperSize = WinPaperUtils::GetPaperSize(paper.GetType());

	}

	if (pApp->DoPrintDialog(printInfo.m_pPD) == IDOK)
	{
		if (printInfo.m_pPD->m_pd.hDC == NULL)
			return;

		SafeSimpleProgress progress(NULL);
		PerformPrinting(&printInfo,&progress);
	}	
}

void DocumentPrintDlg::PerformPrinting(CPrintInfo* pInfo,SimpleProgress* progress)
{
	CDC dcPrint;
	dcPrint.Attach(pInfo->m_pPD->m_pd.hDC);  // attach printer dc
	dcPrint.m_bPrinting = TRUE;
		
	DOCINFO docInfo;
	memset(&docInfo, 0, sizeof(DOCINFO));
	docInfo.cbSize = sizeof(DOCINFO);
	docInfo.lpszDocName = m_instance->GetName().c_str();
	
	// start document printing process
	pInfo->m_nJobNumber = dcPrint.StartDoc(&docInfo);
	if (pInfo->m_nJobNumber == SP_ERROR)
	{
		dcPrint.Detach();   // will be cleaned up by CPrintInfo destructor
		AfxMessageBox(AFX_IDP_FAILED_TO_START_PRINT);
		return;
	}

	// Guarantee values are in the valid range
	UINT nEndPage = pInfo->GetToPage();
	UINT nStartPage = pInfo->GetFromPage();

	if (nEndPage < pInfo->GetMinPage())
		nEndPage = pInfo->GetMinPage();
	if (nEndPage > pInfo->GetMaxPage())
		nEndPage = pInfo->GetMaxPage();

	if (nStartPage < pInfo->GetMinPage())
		nStartPage = pInfo->GetMinPage();
	if (nStartPage > pInfo->GetMaxPage())
		nStartPage = pInfo->GetMaxPage();

	int nStep = (nEndPage >= nStartPage) ? 1 : -1;
	nEndPage = (nEndPage == 0xffff) ? 0xffff : nEndPage + nStep;
	
	progress->SetSteps((nStartPage<nEndPage ? nEndPage-nStartPage : nStartPage-nEndPage)+1);		

	bool bAbort = false;

	// begin page printing loop
	for (pInfo->m_nCurPage = nStartPage; pInfo->m_nCurPage != nEndPage; pInfo->m_nCurPage += nStep)
	{
		// check for end of print
		if (!pInfo->m_bContinuePrinting)
			break;

		// attempt to start the current page
		if (dcPrint.StartPage() < 0)
		{
			bAbort = true;
			break;
		}

		dcPrint.SetMapMode(MM_ANISOTROPIC);
		dcPrint.SetViewportExt(dcPrint.GetDeviceCaps(LOGPIXELSX),dcPrint.GetDeviceCaps(LOGPIXELSY));
		dcPrint.SetWindowExt(254, 254);

		//CRect rectDraw(0, 0,dcPrint.GetDeviceCaps(HORZRES),dcPrint.GetDeviceCaps(VERTRES));
		//dcPrint.DPtoLP(&rectDraw);

		//dcPrint.SetWindowOrg( -rectDraw.Width()/2, -rectDraw.Height()/2 );

		m_pPrintOut->DrawPage(&dcPrint,pInfo->m_nCurPage-1);
				
		dcPrint.EndPage();

		if(!progress->Step())
		{
			bAbort = true;
			break;
		}
	}
		
		if (!bAbort)
			dcPrint.EndDoc();
		else
			dcPrint.AbortDoc();

	dcPrint.Detach(); 
}



void DocumentPrintDlg::OnDoPrint()
{
	DoPrint(1);
}

void DocumentPrintDlg::OnDoPrint2()
{
	DoPrint(2);
}

void DocumentPrintDlg::OnDoPrint3()
{
	DoPrint(3);
}