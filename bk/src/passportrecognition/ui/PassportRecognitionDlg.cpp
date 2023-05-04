#include "stdafx.h"
#include "PassportRecognitionDlg.h"
#include <configuration/passportrecognition/PassportRecognitionProfiles.h>
#include <configuration/passportrecognition/PassportFields.h>
#include <configuration/passportrecognition/PassportFieldDescriptor.h>
#include "../../ContextAccess.h"
#include <boost/foreach.hpp>
#include <data\datalimits.h>
#include "../../ui/ExceptionMsgBox.h"
#include "../../context/ContextException.h"
#include "../profileeditor/PassportRecognitionProfileEditor.h"
#include "PassportProfileEditorDlg.h"
#include "../../resource.h"
#include "../../utils/StringLoader.h"
#include <boost/format.hpp>
#include "../engine/abbyy80/Abbyy80RecognitionEngine.h"
#include "../runtime/RecognitionException.h"
#include <string/TextUtils.h>
#include <boost/foreach.hpp>
#include "../../ui/waitdlg/WaitDialog.h"
#include "../../hent/HentDlg.h"
#include "../../hent/HentEditor.h"
#include "../PassportRecognitionResult.h"
#include "../../content/StocksComboContent.h"
#include <boost/xpressive/xpressive.hpp>
#include <validation/CowNoValidator.h>
#include <validation/HentNoValidator.h>
#include <utils\SafeGet.h>
#include "InputWeightDlg.h"
#include "../operations/LoadFromDIBOperation.h"
#include "../operations/LoadFromFileOperation.h"
#include "../operations/FindPassportFrameOperation.h"
#include "../operations/TresholdOperation.h"
#include "../../ui/question/QuestionBox.h"
#include "../session/NewItemEditor.h"
#include "../session/ExistingItemEditor.h"
#include "../../cow/CowDlg.h"
#include "ChooseCowDlg.h"

using namespace boost::xpressive;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define LOAD_FROM_FILE

#ifdef LOAD_FROM_FILE
#include <afxdlgs.h>
#endif

#if (!defined(_DEBUG) && defined(LOAD_FROM_FILE))
#error LOAD_FROM_FILE is defined in release version
#endif


PassportRecognitionDlg::PassportRecognitionDlg(PassportRecognitionSession* pSession, PassportRecognizerSettings* pSettings,	 CWnd* pParent /*=NULL*/) : 
CLazyDlg<CSessionAwareDialog>(PassportRecognitionDlg::IDD, pParent), 
	m_foundHentId(NULL_ID),
	m_foundStockId(NULL_ID),	
	m_TwainPP("bkocr",&m_TraceFile),
	m_bNoSource(FALSE),
	m_pPassRecognizer(NULL),
	m_pSettings(pSettings),
	m_pRecognitionEngine(NULL),
	m_pSession(pSession),
	m_content(NULL)
{

	DataSettings& dataSettings = m_pSettings->GetDataSettings();

	m_passDate.SetEnabled(dataSettings.IsFieldEnabled(DataSettings::PASSPORT_DATE));
	m_passNo.SetEnabled(dataSettings.IsFieldEnabled(DataSettings::PASSPORT_NO));
	m_hentNo.SetEnabled(dataSettings.IsFieldEnabled(DataSettings::FIRST_OWNER));
	m_cowNo.SetEnabled(true);
	m_birthDate.SetEnabled(dataSettings.IsFieldEnabled(DataSettings::BIRTH));
	m_birthPlace.SetEnabled(dataSettings.IsFieldEnabled(DataSettings::BIRTH));	
	m_stockCombo.SetEnabled(dataSettings.IsFieldEnabled(DataSettings::STOCK));
	m_motherNo.SetEnabled(dataSettings.IsFieldEnabled(DataSettings::MOTHER_NO));
	m_cowSexCombo.SetEnabled(dataSettings.IsFieldEnabled(DataSettings::SEX));
	m_hentName.SetEnabled(dataSettings.IsFieldEnabled(DataSettings::FIRST_OWNER));
	m_hentStreet.SetEnabled(dataSettings.IsFieldEnabled(DataSettings::FIRST_OWNER));
	m_hentPobox.SetEnabled(dataSettings.IsFieldEnabled(DataSettings::FIRST_OWNER));
	m_hentCity.SetEnabled(dataSettings.IsFieldEnabled(DataSettings::FIRST_OWNER));
	m_hentZip.SetEnabled(dataSettings.IsFieldEnabled(DataSettings::FIRST_OWNER));
	

	m_allInputFields.Add(&m_passDate);
	m_allInputFields.Add(&m_passNo);
	m_allInputFields.Add(&m_hentNo);
	m_allInputFields.Add(&m_cowNo),
	m_allInputFields.Add(&m_birthDate);
	m_allInputFields.Add(&m_birthPlace);	
	m_allInputFields.Add(&m_stockCombo);
	m_allInputFields.Add(&m_motherNo);
	m_allInputFields.Add(&m_cowSexCombo);
	m_allInputFields.Add(&m_hentName);
	m_allInputFields.Add(&m_hentStreet);
	m_allInputFields.Add(&m_hentPobox);
	m_allInputFields.Add(&m_hentCity);
	m_allInputFields.Add(&m_hentZip);
	
}

PassportRecognitionDlg::~PassportRecognitionDlg()
{
	

}

void PassportRecognitionDlg::EnableControls(bool bEnable)
{
	BOOST_FOREACH(CInputField* pField, m_allInputFields)
	{
		pField->GetWnd()->EnableWindow(bEnable);
	}
	
	GetDlgItem(IDC_ADDHENT)->EnableWindow(bEnable);
}


void PassportRecognitionDlg::EnableHentSection(bool bEnable)
{
	BOOST_FOREACH(CWnd* pCtrl, m_hentSection)
	{
		pCtrl->EnableWindow(bEnable);
	}

	GetDlgItem(IDC_ADDHENT)->EnableWindow(bEnable);
}


void PassportRecognitionDlg::DoDataExchange(CDataExchange* pDX)
{
	CLazyDlg<CSessionAwareDialog>::DoDataExchange(pDX);

	DDX_Control(pDX,IDC_PASSDATE,m_passDate.GetCtrl());
	DDX_Control(pDX, IDC_PASSNO, m_passNo.GetCtrl());
	DDX_Control(pDX, IDC_EAN, m_cowNo.GetCtrl());
	DDX_Control(pDX, IDC_HENTNO, m_hentNo.GetCtrl());
	DDX_Control(pDX, IDC_BIRTHDATE, m_birthDate.GetCtrl());
	DDX_Control(pDX, IDC_BIRTHPLACE, m_birthPlace.GetCtrl());
	DDX_Control(pDX, IDC_STOCK, m_stockCombo.GetCtrl());
	DDX_Control(pDX, IDC_MOTHERNO, m_motherNo.GetCtrl());
	DDX_Control(pDX, IDC_SEX, m_cowSexCombo.GetCtrl());
	DDX_Control(pDX, IDC_HENTNAME, m_hentName.GetCtrl());
	DDX_Control(pDX, IDC_HENTSTREET, m_hentStreet.GetCtrl());
	DDX_Control(pDX, IDC_HENTPOBOX, m_hentPobox.GetCtrl());
	DDX_Control(pDX, IDC_HENTCITY, m_hentCity.GetCtrl());
	DDX_Control(pDX, IDC_HENTZIP, m_hentZip.GetCtrl());
	
	DDX_Control(pDX,IDC_SHOWTAB,m_showTab);
		
	DDX_Control(pDX,IDC_PASSPROFILESLIST,m_passProfiles);	
	DDX_Control(pDX,IDC_PROFILE_MENU,m_ProfileBtn);
}

BEGIN_MESSAGE_MAP(PassportRecognitionDlg, CLazyDlg<CSessionAwareDialog>)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_DOSCAN, OnDoScan)
	ON_BN_CLICKED(IDC_ADDCOW, OnAddCow)
	ON_NOTIFY(TCN_SELCHANGE,IDC_SHOWTAB, OnSelChange)
	ON_NOTIFY(TCN_SELCHANGING,IDC_SHOWTAB, OnSelChanging)
	ON_CBN_SELCHANGE(IDC_STOCK,OnStockCbnChange)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
		
	ON_BN_CLICKED(IDC_OCRAGAIN_UP, OnRepeatWithNextProfile)
	ON_BN_CLICKED(IDC_OCRAGAIN_DOWN, OnRepeatWithPrevProfile)
	ON_BN_CLICKED(IDC_ADDHENT, OnBnClickedAddHent)
	ON_BN_CLICKED(IDC_SCANNINGFINISHED, OnBnClickedScanningFinished)
//	ON_NOTIFY(NM_DBLCLK,IDC_COWLIST,OnDblClick)
	ON_BN_CLICKED(IDC_OCRAGAIN,OnOCRAgain)
	ON_BN_CLICKED(IDC_PROFILE_MENU,OnProfileMenuBtn)
	ON_COMMAND(IDM_DUPLICATE_PROFILE,OnDuplicateProfile)
	ON_COMMAND(IDM_DELETE_PROFILE,OnDeleteProfile)
	ON_COMMAND(IDM_EDIT_PROFILE,OnEditProfile)
	ON_COMMAND(IDM_ADD_NEW_PROFILE,OnAddNewProfile)
END_MESSAGE_MAP()


BOOL PassportRecognitionDlg::OnInitDialog()
{
	CLazyDlg<CSessionAwareDialog>::OnInitDialog();
	
	
	EnableControls(false);

	m_hentSection.Add(m_hentName.GetWnd());
	m_hentSection.Add(m_hentStreet.GetWnd());
	m_hentSection.Add(m_hentPobox.GetWnd());
	m_hentSection.Add(m_hentCity.GetWnd());
	m_hentSection.Add(m_hentZip.GetWnd());	
	m_hentSection.Add(m_hentNo.GetWnd());

	TCITEM item;
	item.mask = TCIF_TEXT|TCIF_PARAM;
	CRect tabItemRect;
	m_showTab.GetClientRect(&tabItemRect);

	tabItemRect.top += 28;
	tabItemRect.bottom -= 4;
	tabItemRect.left += 4;
	tabItemRect.right -= 8;

	m_cowsList.Create(LVS_REPORT | LVS_OWNERDATA | WS_BORDER | WS_TABSTOP,tabItemRect,&m_showTab,IDC_COWLIST);

	wchar_t tabName[50];
	wcscpy_s(tabName, StringLoader(IDS_OCRTABNAME_COWSLIST).c_str());

	
	item.lParam = (LPARAM)&m_cowsList;
	item.pszText = tabName;
	m_showTab.InsertItem(0, &item);	

	m_cowsList.SetWindowPos(NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOZORDER | SWP_NOMOVE);
	
	//m_scanList.ShowWindow(SW_HIDE);
	
	m_passportImageCtrl.Create(tabItemRect, &m_showTab);

	wcscpy_s(tabName, StringLoader(IDS_OCRTABNAME_IMAGE).c_str());
	item.lParam = (LPARAM)&m_passportImageCtrl;
	item.pszText = tabName;
	m_showTab.InsertItem(1, &item);
	
	m_passportImageCtrl.SetWindowPos(NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOZORDER | SWP_NOMOVE);
	
	m_passportImageCtrl.ShowWindow(SW_SHOW);
	m_iSelectedTab = 1;
	m_showTab.SetCurSel(m_iSelectedTab);
		
	InitProfilesList();
	m_passProfiles.SetCurSel(0);
		
	EnableRepeatButton(false);
	EnableAddCowButton(false);

	m_ProfileMenu.LoadMenu(IDR_PASSPORT_PROFILE_MENU);

	m_alert.Create(this);
	m_alert.SetStyle(TipWndStyle().margin(3).inner(Color(245,129,135)).outer(Color(237, 28, 36)));

	ListHeaderDescriptor header;
	header(RecognitionItemViewMembers::RecognitionItemView_CowNo, StringLoader(IDS_HEADER_COWNO))
		(RecognitionItemViewMembers::RecognitionItemView_PassNo, StringLoader(IDS_HEADER_PASSNO))
		(RecognitionItemViewMembers::RecognitionItemView_FirstOwner, StringLoader(IDS_HEADER_FSTOWNER))
		(RecognitionItemViewMembers::RecognitionItemView_BirthDate, StringLoader(IDS_HEADER_BIRTHDATE))
		(RecognitionItemViewMembers::RecognitionItemView_Weight, StringLoader(IDS_HEADER_WEIGHT))
		(RecognitionItemViewMembers::RecognitionItemView_Sex, StringLoader(IDS_HEADER_SEX))
		(RecognitionItemViewMembers::RecognitionItemView_Stock, StringLoader(IDS_HEADER_STOCK))
		(RecognitionItemViewMembers::RecognitionItemView_Class, StringLoader(IDS_HEADER_CLASS))
		;


	m_content = new RecognitionSessionContent(GetSeasonSession());
	m_content->Create(m_pSession);
	m_cowsList.SetHeaderDescriptor(header);
	m_cowsList.SetContent(m_content);
		
	return TRUE; 
}

void PassportRecognitionDlg::InitProfilesList()
{
	PassportRecognitionProfiles profiles = ContextAccess::GetXContext()->GetPassportRecognitionProfiles();

	m_passProfiles.ResetContent();

	int item;
	BOOST_FOREACH(IPassportRecognitionProfile* profile, profiles)
	{
		item = m_passProfiles.AddString(profile->GetProfileName().c_str());
		m_passProfiles.SetItemData(item, profile->GetId());
	}
}

void PassportRecognitionDlg::OnAfterDlgShow()
{
	m_pRecognitionEngine = new Abbyy80RecognitionEngine();
	try
	{
		CWaitDialog waitForEngine(this, StringLoader(IDS_INITIALIZINGOCRENGINE));

		waitForEngine.WaitForTask([this]()
		{
			m_pRecognitionEngine->Create(m_pSettings->GetEngineSettings());
			m_pPassRecognizer = new PassportRecognizer(m_pRecognitionEngine);
		});		
	}
	catch(RecognitionException& exception)
	{
		std::wstring message = TextUtils::Format(StringLoader(IDS_FAILDTOINITRECOGNITIONENGINE), exception.message());		
		AfxMessageBox(message.c_str());		
		EndDialog(IDOK);
		return;
	}

	m_bNoSource = !(m_TwainPP.Create(m_hWnd, this) && m_TwainPP.OpenSource());
}

void PassportRecognitionDlg::OnPaint()
{
	

	CPaintDC dc(this); // device context for painting

	CPen redPen(PS_SOLID,2,RGB(255,0,0));
	CPen blackPen(PS_SOLID,2,RGB(0,0,0));

		
	CRect wndRect;

	BOOST_FOREACH(CInputField *pInputField,m_allInputFields)
	{		
		pInputField->GetWnd()->GetWindowRect(&wndRect);
		ScreenToClient(&wndRect);
		wndRect.top -= 2;
		wndRect.left -= 2;
		wndRect.bottom += 2;
		wndRect.right += 2;	

		if(pInputField->IsInvalid())
		{
			CPen* oldPen = dc.SelectObject(&redPen);			
			dc.Rectangle(wndRect);
			dc.SelectObject(oldPen);
		}
		else if(!pInputField->IsFieldEnabledForInput())
		{
			CPen* oldPen = dc.SelectObject(&blackPen);			
			dc.Rectangle(wndRect);
			dc.SelectObject(oldPen);				
		}
	}

	CLazyDlg<CSessionAwareDialog>::OnPaint();
	
}

void PassportRecognitionDlg::OnDoScan()
{
	if(m_passProfiles.GetCurSel() < 0)
	{
		AfxMessageBox(IDS_NOPASSPORTPROFILESELECTED);
		return;
	}

#ifndef LOAD_FROM_FILE
	if (m_bNoSource)
	{
		AfxMessageBox(IDS_NOIMAGESOURCE, MB_OK, 0);
		return;
	}
#endif

	ClearFields();
	ClearInvalid();
	EnableControls(false);
	m_foundStockId = NULL_ID;
	m_foundHentId = NULL_ID;


#ifdef LOAD_FROM_FILE

	CFileDialog l_openimg(TRUE,
		NULL,
		NULL,
		OFN_OVERWRITEPROMPT,
		L"Windows Bitmap (*.bmp)|*.bmp|JPEG Compressed Image (*.jpg)|*.jpg|TrueVision Targa (*.tga)|*.tga|TIFF (*.tif)|*.tga||"
		);

	if (l_openimg.DoModal() != IDOK)
	{
		return;
	}

	LoadFromFileOperation loadOperation((LPCTSTR)l_openimg.GetPathName(), m_pSettings->GetScannerSettings().GetResolution());
	m_ptrImage.reset(new PassportImage());
	m_ptrImage->Perform(&loadOperation);
	DoFirstOCR(); 
	
#else
		
	if(!m_TwainPP.HasPendingTransfers())
	{
		CTwainPP::SBitmapOptions Options;
		Options.eImageType = CTwainPP::itGray;
		Options.nResolution = m_pSettings->GetScannerSettings().GetResolution();
		Options.nBrightness = m_pSettings->GetScannerSettings().GetBrightness();
		Options.nContrast = m_pSettings->GetScannerSettings().GetContrast();

		Options.fSizeX = A4_SizeX/Inch;
		Options.fSizeY = (A4_SizeY/2)/Inch;

		m_TwainPP.GetBitmap(FALSE, Options,FALSE);
	}
	else
		m_TwainPP.DoPendingTransfer();

#endif
}

DateTime PassportRecognitionDlg::GetDate(const std::wstring& sDt)
{
	return DateTime::ParseDate(DateTime::YYYYdashMMdashDD, sDt);
}

bool PassportRecognitionDlg::ValidateDate(const std::wstring& sDt)
{
	bool bValid = true;
	try
	{
		GetDate(sDt);
	}
	catch (...)
	{
		bValid = false;
	}
	return bValid;
}

bool PassportRecognitionDlg::ValidateCowNo(const std::wstring& sCowNo)
{
	bool bValid = true;
	try
	{
		CowNo cowNo = CowNoFactory::FromString(sCowNo);
		if (!CowNoValidator::IsValid(cowNo))
		{
			bValid = AfxMessageBox(IDS_YNBADCTRLDIGIT, MB_YESNO, 0) == IDYES;			
		}
	}
	catch (...)
	{
		bValid = false;
	}
	return bValid;
}

bool PassportRecognitionDlg::ValidateHentNo(const std::wstring& sHentNo)
{
	bool bValid = true;
	try
	{
		HentNo hentNo = HentNoFactory::FromString(sHentNo);
		if (!HentNoValidator::IsValid(hentNo))
		{
			bValid = AfxMessageBox(IDS_YNBADCTRLDIGIT, MB_YESNO, 0) == IDYES;
		}
	}
	catch (...)
	{
		bValid = false;
	}
	return bValid;
}

bool PassportRecognitionDlg::ValidateFields()
{
	bool bValidationResult = true;
	
	DataSettings& dataSettings = m_pSettings->GetDataSettings();

	//PASSNO
	if(dataSettings.IsFieldEnabled(DataSettings::PASSPORT_NO))
	{			
		std::wstring sPassNo = m_passNo->GetText();
		m_passNo.SetInvalid(!ValidatePassNo(sPassNo));
		bValidationResult = bValidationResult && !m_passNo.IsInvalid();		
	}

	//COWNO
	if (ValidateCowNo(m_cowNo->GetText()))
	{			
		m_cowNo.SetInvalid(false);
	}
	else
	{
		m_cowNo.SetInvalid(true);		
	}

	bValidationResult = bValidationResult && !m_cowNo.IsInvalid();

	//PASSDATE	
	if(dataSettings.IsFieldEnabled(DataSettings::PASSPORT_DATE))
	{
		m_passDate.SetInvalid(!ValidateDate(m_passDate->GetText()));
		bValidationResult = bValidationResult && !m_passDate.IsInvalid();
	}
	
	//MOTHERNO
	if(dataSettings.IsFieldEnabled(DataSettings::MOTHER_NO))
	{
		if (ValidateCowNo(m_motherNo->GetText()))
		{					
			m_motherNo.SetInvalid(false);
		}
		else
		{
			m_motherNo.SetInvalid(true);
		}
		bValidationResult = bValidationResult && !m_motherNo.IsInvalid();
	}

	std::wstring stockCd = GetSelectedStock();
	IStock* stock = FindStockByCode(stockCd);
	m_stockCombo.SetInvalid(!stock);
	
	std::wstring sHentNo = m_hentNo->GetText();
	if (!(m_foundHentId == NULL_ID) && !sHentNo.empty() && ValidateHentNo(sHentNo))
	{
		IHent* hent = FindHentByHentNo(HentNoFactory::FromString(sHentNo));
		if (hent != NULL)
		{
			SetHentFields(hent);
			m_foundHentId = hent->GetId();

			m_hentNo.SetInvalid(false);
			m_hentName.SetInvalid(false);
			m_hentStreet.SetInvalid(false);
			m_hentPobox.SetInvalid(false);
			m_hentCity.SetInvalid(false);
			m_hentZip.SetInvalid(false);
			EnableHentSection(false);
		}
	}
	
	
	//FSTOWNR
	if(dataSettings.IsFieldEnabled(DataSettings::FIRST_OWNER) && m_foundHentId == NULL_ID)
	{
		ShowAlert(StringLoader(IDS_REQUIREFSTOWNER), m_hentNo.GetWnd());
		bValidationResult = false;	
	}	
	
	if(dataSettings.IsFieldEnabled(DataSettings::BIRTH))
	{
		//BIRTHDATE
		m_birthDate.SetInvalid(ValidateDate(m_birthDate->GetText()));
			
		//BIRTHPLACE		
		m_birthPlace.SetInvalid(!m_birthPlace->GetText().empty());

		bValidationResult = bValidationResult && !m_birthDate.IsInvalid() && !m_birthPlace.IsInvalid();
	}

	
	Invalidate();
	return bValidationResult;
}

IHent* PassportRecognitionDlg::FindHentByHentNo(const HentNo& hentNo)
{
	SeasonSession* session = GetSeasonSession();
	return SafeGet(&HentClientObject::Object, session->GetHent(hentNo), nullptr);
}

CowDetails PassportRecognitionDlg::CreateCowFromFields()
{
	DataSettings& dataSettings = m_pSettings->GetDataSettings();
	
	CowDetails details;
		
	details.SetStockId(m_foundStockId);		

	//COWNO
	details.SetCowNo(CowNoFactory::FromString(m_cowNo->GetText()));

	//PASSNO
	if(dataSettings.IsFieldEnabled(DataSettings::PASSPORT_NO))
	{			
		details.SetPassNo(m_passNo->GetText());
	}

	//PASSDATE	
	if(dataSettings.IsFieldEnabled(DataSettings::PASSPORT_DATE))
	{		
		details.SetPassDate(GetDate(m_passDate->GetText()));
	}

	//MOTHERNO
	if(dataSettings.IsFieldEnabled(DataSettings::MOTHER_NO))
	{
		details.SetMotherNo(CowNoFactory::FromString(m_motherNo->GetText()));
	}

	details.SetFirstOwnerId(m_foundHentId);
	
	if(dataSettings.IsFieldEnabled(DataSettings::BIRTH))
	{
		//BIRTHDATE
		details.SetBirthDate(GetDate(m_birthDate->GetText()));

		//BIRTHPLACE
		details.SetBirthPlace(m_birthPlace->GetText());
	}
	
	return details;

}


void PassportRecognitionDlg::OnAddCow()
{	
	if(ValidateFields())
	{
		CowDetails details = CreateCowFromFields();

		CInputWeightDlg inputWeight(this);			
		do
		{
			NullDecimal weight = inputWeight.ask();
			
			if(!weight.IsNull() && weight > NullDecimal(0L))
			{
				details.SetWeight(weight.ToDecimal(Decimal(0L)));				
				break;
			}				
			
			AfxMessageBox(IDS_ERRREQWEIGHT,MB_OK,0);
			
			return;
		} 
		while(FALSE);

		CWaitDialog wait(this, StringLoader(IDS_PLEASEWAIT));

		wait.WaitForTask(std::bind(&PassportRecognitionSession::AddCow, m_pSession, details, m_ptrImage));
							
		m_showTab.SetCurSel(0);	
	
		LRESULT lRes;
		OnSelChange(NULL,&lRes);
		OnSelChanging(NULL,&lRes);		
	
	}
	
}

void PassportRecognitionDlg::OnSelChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	int iNewTab = m_showTab.GetCurSel();

	TCITEM item;
	CWnd* pWnd;

	item.mask = TCIF_PARAM;
		
	//** hide the current tab ---------
	m_showTab.GetItem(m_iSelectedTab, &item);
	pWnd = reinterpret_cast<CWnd*> (item.lParam);
	ASSERT_VALID(pWnd);
	pWnd->ShowWindow(SW_HIDE);

	//** show the selected tab --------
	m_showTab.GetItem(iNewTab, &item);
	pWnd = reinterpret_cast<CWnd*> (item.lParam);
	ASSERT_VALID(pWnd);
	pWnd->ShowWindow(SW_SHOW);	

	*pResult = 0;
}

void PassportRecognitionDlg::OnSelChanging(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_iSelectedTab = m_showTab.GetCurSel();

	*pResult = 0;
}

void PassportRecognitionDlg::OnTwainImageInfo(int nSizeX, int nSizeY)
{
	
}

void PassportRecognitionDlg::ProcessRecognitionResult(const PassportRecognitionResult& recognitionResult)
{
	DataSettings& dataSettings = m_pSettings->GetDataSettings();
	
	ProcessCowNo(recognitionResult.GetCowNo(),recognitionResult.GetCowNoBC());					

	if(dataSettings.IsFieldEnabled(DataSettings::FIRST_OWNER))
	{
		ProcessFirstOwner(recognitionResult.GetFarmNoBC(),recognitionResult.GetFstOwner());
	}

	if(dataSettings.IsFieldEnabled(DataSettings::MOTHER_NO))
	{
		ProcessMotherNo(recognitionResult.GetMotherNo());
	}

	if(dataSettings.IsFieldEnabled(DataSettings::SEX))
	{
		ProcessCowSex(recognitionResult.GetCowSex());
	}

	if(dataSettings.IsFieldEnabled(DataSettings::PASSPORT_DATE))
	{
		ProcessPassDate(recognitionResult.GetPassDate());
	}

	if(dataSettings.IsFieldEnabled(DataSettings::PASSPORT_NO))
	{					
		ProcessPassNo(recognitionResult.GetPassNo());
	}

	if(dataSettings.IsFieldEnabled(DataSettings::STOCK))
	{
		ProcessCowStock(recognitionResult.GetCowStock());
	}
	
	if(dataSettings.IsFieldEnabled(DataSettings::BIRTH))
	{
		ProcessBirthDatePlace(recognitionResult.GetBirthDatePlace());
	}
}

void PassportRecognitionDlg::SetPassportImage(PassportImagePtr ptrImage, IPassportRecognitionProfile* pPassProfile)
{
	if(pPassProfile)
	{
		Size<int> size(pPassProfile->GetXDimention(),pPassProfile->GetYDimention());
		NullPoint<int> origin = ptrImage->GetOrigin();
		int originX = origin.x(0), originY = origin.y(0);
		int dpiResolution = ptrImage->GetResolution().ToInt(0);

		int right = originX + (size.cx*dpiResolution) / 254;
		int	bottom = originY + (size.cy*dpiResolution) / 254;
	
		ptrImage->GetPreparedPassportImage().copySubImage(m_passportImg, originX, originY, right, bottom);
		
		m_passportImageCtrl.SetImage(m_passportImg);
		m_passportImageCtrl.SetProfile(pPassProfile);
	}
	else
	{
		m_passportImg = ptrImage->GetRawPassportImage();
		m_passportImageCtrl.SetImage(m_passportImg);
		m_passportImageCtrl.SetProfile(NULL);
	}
}

uint32_t PassportRecognitionDlg::GetSelectedProfile()
{
	int nSelectedProfile = m_passProfiles.GetCurSel();
	if(nSelectedProfile >= 0)
		return (uint32_t)m_passProfiles.GetItemData(nSelectedProfile);

	return NULL_ID;
}

void PassportRecognitionDlg::OnRepeatWithPrevProfile()
{
	int currProfile = m_passProfiles.GetCurSel();
	if(currProfile >= 0)
	{
		int nextProfile = currProfile+1;
		if(nextProfile >= m_passProfiles.GetCount())
			nextProfile = 0;

		if (nextProfile != currProfile)
		{
			m_passProfiles.SetCurSel(nextProfile);			
			DoOCRWithProfile((uint32_t)m_passProfiles.GetItemData(nextProfile));
		}
	}
}

void PassportRecognitionDlg::OnRepeatWithNextProfile()
{
	int currProfile = m_passProfiles.GetCurSel();
	if(currProfile >= 0)
	{
		int prevProfile = currProfile-1;
		if(prevProfile < 0)
			prevProfile = m_passProfiles.GetCount()-1;

		if (prevProfile != currProfile)
		{
			m_passProfiles.SetCurSel(prevProfile);
			DoOCRWithProfile((uint32_t)m_passProfiles.GetItemData(prevProfile));
		}
	}	
}


BOOL PassportRecognitionDlg::OnTwainBitmap(HDIB hDIB)
{
	LoadFromDIBOperation loadOperation(hDIB, m_pSettings->GetScannerSettings().GetResolution());
	m_ptrImage.reset(new PassportImage());
	m_ptrImage->Perform(&loadOperation);
	DoFirstOCR();
	return TRUE;
}


void PassportRecognitionDlg::OnTwainError(CTwainPP::ETWainError eTwainError)
{
	switch (eTwainError)
	{
	case CTwainPP::eNoTwain:
		AfxMessageBox(IDS_SCANNERNOTFOUND,MB_OK,0);
		break;
	case CTwainPP::eCancel:
		AfxMessageBox(IDS_SCANCANCELED,MB_OK,0);
		break;
	case CTwainPP::eGetCapability:
		AfxMessageBox(IDS_ERRORGETOPTION,MB_OK,0);
		break;
	case CTwainPP::eSetCapability:
		AfxMessageBox(IDS_ERRORSETOPTION,MB_OK,0);
		break;
	case CTwainPP::eDeviceNotReady:
		AfxMessageBox(IDS_ERRORDEVICENOTREADY,MB_OK,0);
		break;
	case CTwainPP::eErrorReading:
		AfxMessageBox(IDS_ERRORREADING,MB_OK,0);
		break;
	}
}


void PassportRecognitionDlg::OnClose()
{

	CLazyDlg<CSessionAwareDialog>::OnClose();
}


void PassportRecognitionDlg::OnDestroy()
{
	m_TwainPP.Close();	

	if (m_pPassRecognizer)
	{
		delete m_pPassRecognizer;
		m_pPassRecognizer = NULL;
	}
	
	if(m_pRecognitionEngine)
	{
		m_pRecognitionEngine->Destroy();
		delete m_pRecognitionEngine;
		m_pRecognitionEngine = NULL;
	}

	if (m_content)
	{
		m_content->Destroy();
		delete m_content;
		m_content = NULL;
	}
}

void PassportRecognitionDlg::OnBnClickedAddHent()
{

	try
	{
		HentNo hentNo = HentNoFactory::FromString(m_hentNo->GetText());

	}
	catch (HentNoException& e)
	{

	}


	HentEditor editor(GetSeasonSession());
	editor.New();

	editor.SetName(m_hentName->GetText());
	editor.SetStreet(m_hentStreet->GetText());
	editor.SetPOBox(m_hentPobox->GetText());
	editor.SetCity(m_hentCity->GetText());
	editor.SetZip(m_hentZip->GetText());

		HentDlg hentDlg(&editor, this);

		if (hentDlg.DoModal() == IDOK)
		{
			m_foundHentId = editor.GetHentId();
			EnableHentSection(FALSE);
		}
}


bool PassportRecognitionDlg::ValidatePassNo(const std::wstring &passNo)
{
	wsregex passMatch = repeat<2, 2>(alnum) >> repeat<8, 8>(_d);
	return regex_match(passNo,passMatch);			
}

void PassportRecognitionDlg::OnStockCbnChange()
{	
	m_foundStockId = (uint32_t)m_stockCombo->GetItemData(m_stockCombo->GetCurSel());	
}

bool PassportRecognitionDlg::DoManualInputAdd()
{
	NewItemEditor editor(m_pSession, m_ptrImage);
	CowDlg dlg(&editor,this);
	return (dlg.DoModal() == IDOK);
}

bool PassportRecognitionDlg::DoManualInputUpdate()
{
	CChooseCowDlg chooseDlg(m_pSession, this);
	ItemId item = chooseDlg.ChooseCow();
	if (!item.is_nil())
	{
		ExistingItemEditor editor(m_pSession, m_ptrImage);
		editor.Edit(item);
		CowDlg dlg(&editor, this);
		return (dlg.DoModal() == IDOK);
	}
	return false;
}

void PassportRecognitionDlg::OnBnClickedScanningFinished()
{
	EndDialog(IDOK);
}

void PassportRecognitionDlg::OnUpdateCow()
{

}

void PassportRecognitionDlg::OnOK()
{

}

/*
void PassportRecognitionDlg::OnDblClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = -1;
	int nSubItem = -1;
	if (pNMIA)
	{
		
		nItem = pNMIA->iItem;

		if(nItem >= 0)
		{
			if(EditCow(m_outArray[nItem]))
			m_scanList.Invalidate();
		}
	}

	*pResult = 0;
}


bool PassportRecognitionDlg::EditCow(Cow *pCow)
{
	CEditCowDlg editCowDlg(pCow,this);
	editCowDlg.LockEANEdit();
	if(editCowDlg.DoModal() == IDOK)
	{
		pCow->CommitChanges();
		return true;
	}
	return false;
}
*/

void PassportRecognitionDlg::ClearFields()
{
	m_passDate->Clear();
	m_passNo->Clear();	
	m_cowNo->Clear();
	m_birthDate->Clear();
	m_birthPlace->Clear();	
	m_stockCombo->ResetContent();
	m_motherNo->Clear();
	m_cowSexCombo->ResetContent();
	ClearFirstOwnerFields(true);
	
}

void PassportRecognitionDlg::ClearFirstOwnerFields(bool bClearFarmNo)
{
	if(bClearFarmNo)
		m_hentNo->Clear();

	m_hentName->Clear();
	m_hentStreet->Clear();
	m_hentPobox->Clear();
	m_hentCity->Clear();
	m_hentZip->Clear();
}


void PassportRecognitionDlg::SetHentFields(IHent* pHent)
{
	m_hentName->SetText(pHent->GetName());
	m_hentStreet->SetText(pHent->GetStreet());
	m_hentPobox->SetText(pHent->GetPOBox());
	m_hentZip->SetText(pHent->GetZip());
	m_hentCity->SetText(pHent->GetCity());
}


void PassportRecognitionDlg::ClearInvalid()
{
	BOOST_FOREACH(CInputField* pField, m_allInputFields)
	{
		pField->SetInvalid(false);
	}
}


void PassportRecognitionDlg::DoFirstOCR()
{
	TresholdOperation threshold(m_pSettings->GetImageSettings().GetBlackLevel());
	m_ptrImage->Perform(&threshold);
	OnOCRAgain();
}

void PassportRecognitionDlg::OnOCRAgain()
{
	uint32_t profileId = GetSelectedProfile();
	if (profileId != NULL_ID)
	{
		m_foundStockId = NULL_ID;
		m_foundHentId = NULL_ID;
		ClearInvalid();
		ClearFields();
		InitStockSelection();

		EnableRepeatButton(false);
		EnableAddCowButton(false);

		NullPoint<int> origin = m_ptrImage->GetOrigin();

		if (origin.IsNull())
		{
			FindPassportFrameOperation findFrameOperation;
			m_ptrImage->Perform(&findFrameOperation);

			if (origin.IsNull())
			{
				SetPassportImage(m_ptrImage, NULL);
				LRESULT lRes;
				m_showTab.SetCurSel(1);
				OnSelChange(NULL, &lRes);
				OnSelChanging(NULL, &lRes);

				Answer manualInputDecision = QuestionBox().Question(StringLoader(IDS_ASKMANUALINPUT))
					.AddAnswer(StringLoader(IDS_ANSWERYESADD), 1)
					.AddAnswer(StringLoader(IDS_ANSWERYESUPDATE), 2)
					.AddAnswer(StringLoader(IDS_ANSWERNO), 3)
					.CancelId(3)
					.Ask(this);

				switch (manualInputDecision.GetAnswer())
				{
				case 1:
					DoManualInputAdd();
					break;
				case 2:
					DoManualInputUpdate();
					break;
				}
					
			}
			
		}
		else
		{
			DoOCRWithProfile(profileId);
		}
	}	
}

void PassportRecognitionDlg::DoOCRWithProfile(uint32_t profileId)
{	
	HCURSOR hCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));

	XContext* context = ContextAccess::GetXContext();
	IPassportRecognitionProfile* profile = context->GetPassportRecognitionProfile(profileId);

	PassportRecognitionResult result = m_pPassRecognizer->Recognize(m_ptrImage,profile);

	ProcessRecognitionResult(result);		
	SetPassportImage(m_ptrImage, profile);
		
	Invalidate();
	LRESULT lRes;
	m_showTab.SetCurSel(1);					
	OnSelChange(NULL,&lRes);
	OnSelChanging(NULL,&lRes);	
	
}

void PassportRecognitionDlg::EnableRepeatButton(bool bEnable)
{
	GetDlgItem(IDC_OCRAGAIN)->EnableWindow(bEnable);
	GetDlgItem(IDC_OCRAGAIN_UP)->EnableWindow(bEnable);
	GetDlgItem(IDC_OCRAGAIN_DOWN)->EnableWindow(bEnable);

}
void PassportRecognitionDlg::EnableAddCowButton(bool bEnable)
{	
	GetDlgItem(IDC_ADDCOW)->EnableWindow(bEnable);
}

void PassportRecognitionDlg::OnProfileMenuBtn()
{
	const UINT profileWiseMenuItems[] = 
	{
		IDM_DUPLICATE_PROFILE,
		IDM_DELETE_PROFILE,
		IDM_EDIT_PROFILE,
		0
	};

	uint32_t profileId = GetSelectedProfile();
	CMenu* pPopupMenu = m_ProfileMenu.GetSubMenu(0);
	ASSERT(pPopupMenu); 

	UINT menuItemFlags = 0;
	if (profileId == NULL_ID)
		menuItemFlags = menuItemFlags | MF_GRAYED | MF_DISABLED;

	const UINT* pMenuItemId = profileWiseMenuItems;
	while(*pMenuItemId)
	{
		pPopupMenu->EnableMenuItem(*pMenuItemId,MF_BYCOMMAND | menuItemFlags);
		pMenuItemId++;
	}
	
	CRect rc;
	GetDlgItem(IDC_PROFILE_MENU)->GetWindowRect(rc);

	int x = rc.left;
	int y = rc.bottom;

	
	pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,x,y,this);

}

void PassportRecognitionDlg::OnDuplicateProfile()
{
	XContext* context = ContextAccess::GetXContext();
	uint32_t selectedProfileId = GetSelectedProfile();
	if (selectedProfileId != NULL_ID)
	{
		IPassportRecognitionProfile* selectedProfile = context->GetPassportRecognitionProfile(selectedProfileId);
		std::wstring sDuplicateSuffix = L" 2";

		std::wstring newProfileName;
		std::wstring sProfileName = selectedProfile->GetProfileName();

		if (sProfileName.length() + sDuplicateSuffix.length() > MAXPROFILENAME)
		{
			newProfileName = sProfileName.substr(0, MAXPROFILENAME - sDuplicateSuffix.length()) + sDuplicateSuffix;
		}
		else
		{
			newProfileName = sProfileName + sDuplicateSuffix;
		}

		PassportRecognitionProfile duplicatedProfile(selectedProfile);
		duplicatedProfile.SetProfileName(newProfileName);

		try
		{
			context->SaveNewPassportRecognitionProfile(&duplicatedProfile);
			int nSelectedProfile = m_passProfiles.GetCurSel();
			InitProfilesList();
			m_passProfiles.SetCurSel(nSelectedProfile);
		}
		catch (ContextException& e)
		{
			ExceptionMsgBox().Show(e);
		}
	}
}

void PassportRecognitionDlg::OnDeleteProfile()
{
	uint32_t profileId = GetSelectedProfile();
	
	if (profileId != NULL_ID)
	{
		if(AfxMessageBox(IDS_CONFIRMPROFILEDELETE,MB_YESNO) == IDYES)
		{
			ExceptionMsgBox msgBox;

			msgBox.TryCatchAny([profileId,this]()
			{
				ContextAccess::GetXContext()->DeletePassportRecognitionProfile(profileId);
				InitProfilesList();
			});
		}
	}
}

void PassportRecognitionDlg::OnEditProfile()
{
	uint32_t profileId = GetSelectedProfile();

	PassportRecognitionProfileEditor editor(ContextAccess::GetXContext());
	editor.Edit(profileId);
	PassportProfileEditorDlg profileEditDlg(&editor,this);
	profileEditDlg.SetCaption(StringLoader(IDS_EDITPROFILEDLGCAPTION));

	if(profileEditDlg.DoModal() == IDOK)
	{
		int nSelectedProfile = m_passProfiles.GetCurSel();
		InitProfilesList();
		m_passProfiles.SetCurSel(nSelectedProfile);		
	}
}

void PassportRecognitionDlg::OnAddNewProfile()
{
	const int defaultDimX = 1780,defaultDimY = 1310;
	const struct DEFAULT_FIELD
	{
		PassportFieldDescriptor* field;
		int x,y,h,w;		
	}
	defaultProfileFields[] = 
	{
		{ &PassportFields::BIRTHINFO_FIELD,		381, 390, 90, 440 },
		{ &PassportFields::EAN_FIELD,			550, 160, 60, 500 },
		{ &PassportFields::EANBARCODE_FIELD,	1130, 105, 120, 610 },
		{ &PassportFields::FARMNOBARCODE_FIELD, 1130, 220, 120, 610 },
		{ &PassportFields::FIRSTOWNER_FIELD,	1000, 400, 300, 760 },
		{ &PassportFields::MOTHEREAN_FIELD,		381, 560, 40, 440 },
		{ &PassportFields::PASSDATE_FIELD,		400, 50, 48, 320 },
		{ &PassportFields::PASSNO_FIELD,		1005, 60, 40, 170 },
		{ &PassportFields::SEX_FIELD,			381, 480, 40, 440 },
		{ &PassportFields::STOCK_FIELD,			381, 520, 40, 440 },
		{NULL,									0, 0, 0, 0}
	};

	std::wstring newProfileNamePattern = StringLoader(IDS_NEW_PROFILE_NAME);
	int profileNumber = 1;
	std::wstring newProfileName;
	boost::wformat newProfileNameFmter(newProfileNamePattern.c_str());

	XContext* context = ContextAccess::GetXContext();
	PassportRecognitionProfiles recognitionProfiles = context->GetPassportRecognitionProfiles();

	do
	{
		newProfileName = boost::str(newProfileNameFmter % profileNumber);
		profileNumber++;
	}
	while(!std::none_of(recognitionProfiles.begin(),recognitionProfiles.end(),
						[newProfileName](PassportRecognitionProfile* pProfile) 
						{ 
							return pProfile->GetProfileName() == newProfileName; 
						}));
	
	PassportFieldEditor* fieldEditor;
	PassportRecognitionProfileEditor editor(context);
	editor.New();
	editor.SetProfileName(newProfileName);
	editor.SetXDimention(defaultDimX);
	editor.SetYDimention(defaultDimY);

	const DEFAULT_FIELD* pDefaultField = defaultProfileFields;
	while(pDefaultField->field)
	{
		fieldEditor = editor.EditField(pDefaultField->field->GetFieldName());
		fieldEditor->SetX(pDefaultField->x);
		fieldEditor->SetY(pDefaultField->y);
		fieldEditor->SetW(pDefaultField->w);
		fieldEditor->SetH(pDefaultField->h);										
		pDefaultField++;
	}
		
	PassportProfileEditorDlg newProfileDlg(&editor,this);
	newProfileDlg.SetCaption(StringLoader(IDS_NEWPROFILEDLGCAPTION));
	if(newProfileDlg.DoModal() == IDOK)
	{
		int nSelectedProfile = m_passProfiles.GetCurSel();
		InitProfilesList();
		m_passProfiles.SetCurSel(nSelectedProfile);
	}
	
}

void PassportRecognitionDlg::ShowAlert(const std::wstring& message, CWnd* pAnchorWnd)
{
	CRect wndRect;
	pAnchorWnd->GetWindowRect(wndRect);
	m_alert.SetPosition(CPoint(wndRect.left, wndRect.bottom));
	m_alert.SetText(message.c_str());
	m_alert.Show(3000);
}



void PassportRecognitionDlg::ProcessCowNo(const std::wstring& sCowNoText, const std::wstring& sCowNoBC)
{
	std::wstring sCowNo = sCowNoBC;

	if (sCowNo.empty())
	{
		sCowNo = sCowNoText; //if empty use other number
	}

	m_cowNo.SetInvalid(!ValidateCowNo(sCowNo));
	m_cowNo->SetText(sCowNo);
}

void PassportRecognitionDlg::ProcessFirstOwner(const std::wstring& sFirstOwnerFarmNo, const std::wstring& sFirstOwnerNameAndAddress)
{
	IHent *pHent = NULL;
	
	if (!sFirstOwnerFarmNo.empty())
	{
		m_hentNo.SetInvalid(!ValidateHentNo(sFirstOwnerFarmNo));
		m_hentNo->SetText(sFirstOwnerFarmNo);

		if (!m_hentNo.IsInvalid())
		{
			pHent = FindHentByHentNo(HentNoFactory::FromString(sFirstOwnerFarmNo));
		}
	}
	else
	{
		m_hentNo.SetInvalid(true);
		m_hentNo->EnableWindow(TRUE);
	}

	if (pHent)
	{
		SetHentFields(pHent);
		EnableHentSection(FALSE);
		m_foundHentId = pHent->GetId();		
	}
	else
	{
		/*CString sHentNameAddress(sOCRFirstOwnerNameAddress);

		CStringList addr;
		ParseLines(addr, sHentNameAddress);

		if (!ParseHent(addr))
		{
			m_hentName.SetInvalid(true);
			m_hentStreet.SetInvalid(true);
			m_hentPobox.SetInvalid(true);
			m_hentCity.SetInvalid(true);
			m_hentZip.SetInvalid(true);
			ClearFirstOwnerFields(false);
		}*/
	}
}

void PassportRecognitionDlg::ProcessMotherNo(const std::wstring& sMotherNo)
{
	m_motherNo.SetInvalid(!ValidateCowNo(sMotherNo));
	m_motherNo->SetText(sMotherNo);	
}

void PassportRecognitionDlg::ProcessCowSex(const std::wstring& sCowSex)
{
	COWSEX cowSex = SEX_UNK;
	
	if (sCowSex.find(L"XX") != std::string::npos)
		cowSex = SEX_XX;
	else if (sCowSex.find(L"XY") != std::string::npos)
		cowSex = SEX_XY;
	else
		m_cowSexCombo.SetInvalid(true);

	m_cowSexCombo->InitCombo(cowSex);

}

void PassportRecognitionDlg::InitStockSelection()
{
	m_stockCombo->ResetContent();
	SeasonSession* session = GetSeasonSession();

	PtrEnumerator<IStock> stockEnum = session->EnumStocks();
	while (stockEnum.hasNext())
	{
		m_stockCombo->AddString(stockEnum->GetStockCode().c_str());
		stockEnum.advance();
	}
}

void PassportRecognitionDlg::AddStockToSelection(const std::wstring& stockCode)
{
	if (!stockCode.empty() && !TextUtils::IsAllWhitespace(stockCode))
	{
		m_stockCombo->AddString(stockCode.c_str());
	}
}

void PassportRecognitionDlg::SelectStock(const std::wstring& stockCode)
{
	m_stockCombo->SelectString(-1, stockCode.c_str());
}

void PassportRecognitionDlg::ProcessPassDate(const std::wstring& sPassDate)
{
	m_passDate.SetInvalid(!ValidateDate(sPassDate));
	m_passDate->SetText(sPassDate);
}

void PassportRecognitionDlg::ProcessPassNo(const std::wstring& sPassNo)
{	
	m_passNo.SetInvalid(!ValidatePassNo(sPassNo));
	m_passNo->SetText(sPassNo);	
}

IStock* PassportRecognitionDlg::FindStockByCode(const std::wstring& stockCd)
{
	SeasonSession* session = GetSeasonSession();

	IStock* stock;
	PtrEnumerator<IStock> stockEnum = session->EnumStocks();
	while (stockEnum.hasNext())
	{
		stock = *stockEnum;
		if (stock->GetStockCode() == stockCd)
		{
			return stock;
		}
		stockEnum.advance();
	}

	return NULL;
}

std::wstring PassportRecognitionDlg::GetSelectedStock()
{
	int txtLen = m_stockCombo->GetWindowTextLength();
	int bufferSize = txtLen + 1;
	wchar_t* text = new wchar_t[bufferSize];
	m_stockCombo->GetWindowText(text, bufferSize);
	std::wstring result = text;
	delete text;
	return result;
}

void PassportRecognitionDlg::ProcessCowStock(const std::wstring& sCowStock)
{
	IStock *pStock = FindStockByCode(sCowStock);
	if (pStock)
	{
		m_foundStockId = pStock->GetId();
		SelectStock(sCowStock);
	}
	else
	{
		AddStockToSelection(sCowStock);
		m_stockCombo.SetInvalid(true);
	}
}


void PassportRecognitionDlg::ProcessBirthDatePlace(const std::wstring& sBirthDatePlace)
{
	//CString birthDatePlace = sOCRBirthDatePlace;
	//birthDatePlace.Remove('\r');

	//int lfPos = birthDatePlace.Find('\n'), lfPos2 = -1;
	//if (lfPos >= 0)
	//{
	//	CString sBirthDate(birthDatePlace.Left(lfPos));
	//	lfPos += 1; //skip LF
	//	lfPos2 = birthDatePlace.Find('\n', lfPos);
	//	if (lfPos2 < 0)
	//		lfPos2 = birthDatePlace.GetLength();
	//	else
	//		lfPos2 = min(lfPos2, birthDatePlace.GetLength() - lfPos);

	//	CString sBirthPlace(birthDatePlace.Mid(lfPos, lfPos2));
	//	sBirthPlace = RemoveWhitespace(sBirthPlace);

	//	LOG("birthDate = '%s'", (LPCSTR)sBirthDate);
	//	sBirthDate = RepairDate(sBirthDate);

	//	LOG("repaired birthDate = '%s'", (LPCSTR)sBirthDate);

	//	CDateTime birthDate;
	//	m_birthDate.SetInvalid(!birthDate.ParseDate(CDateTime::YYYYdashMMdashDD, (LPCSTR)sBirthDate, false));
	//	m_birthDate->SetText(sBirthDate);

	//	LOG("m_birthDateErr = %d", m_birthDate.IsInvalid());

	//	LOG("birthPlace = '%s'", (LPCSTR)sBirthPlace);
	//	m_birthPlace->SetText(sBirthPlace);
	//}
}
