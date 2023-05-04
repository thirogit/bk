#include "stdafx.h"
#include "OpenSeasonDlg.h"
#include "afxdialogex.h"
#include "../resource.h"
#include "../compare/SeasonComparator.h"
#include "../utils/StringLoader.h"
#include <arrays/DeepDelete.h>
#include "../compare/DataComparator.h"
#include "../compare/SeasonComparator.h"
#include "../content/compare/RowComparator.h"

SeasonListContent::SeasonListRow::SeasonListRow(ISeason* pSeason) : m_pSeason(pSeason)
{
}

std::wstring SeasonListContent::SeasonListRow::CellValue(uint32_t colId)
{
	return m_SeasonText.Text(m_pSeason,colId);
}

ISeason* SeasonListContent::SeasonListRow::Content() const
{
	return m_pSeason;
}

void SeasonListContent::SortContent(const SortAttributes& attributes)
{
	RowComparator< DataComparator<SeasonComparator> > comparator(attributes);
	std::sort(m_Content.begin(),m_Content.end(),comparator);
}


//#################################################################################################

SeasonListContent::SeasonListContent()
{
}

SeasonListContent::~SeasonListContent()
{
	DestroyContent();
}

void SeasonListContent::DestroyContent()
{
	DeepDelete(m_Content);
	m_Content.clear();
}

void SeasonListContent::AddSeason(ISeason* season)
{
	m_Content.push_back(new SeasonListRow(season));
}

void SeasonListContent::AddContentObserver(ContentObserver* pObserver)
{
}

int SeasonListContent::GetRowCount() const
{
	return m_Content.size();
}

ListRow* SeasonListContent::GetRow(int row)
{
	return m_Content[row];
}

ISeason* SeasonListContent::GetSeasonAt(int iIndex)
{
	return m_Content[iIndex]->Content();
}

//#################################################################################################

OpenSeasonDlg::OpenSeasonDlg(CWnd* pParent /*=NULL*/) : CDialogEx(OpenSeasonDlg::IDD, pParent),m_pSelectedSeason(NULL)
{
	ListHeaderDescriptor header;
	header(SeasonMember_Name,StringLoader(IDS_HEADER_SEASONNAME))
		  (SeasonMember_StartDt,StringLoader(IDS_HEADER_SEASONSTARTDT))
		  (SeasonMember_EndDt,StringLoader(IDS_HEADER_SEASONENDDT));

	m_SeasonList.SetHeaderDescriptor(header);
}

OpenSeasonDlg::~OpenSeasonDlg()
{
}

void OpenSeasonDlg::OnOK()
{
	int nSel = m_SeasonList.GetCurSel();
	if(nSel >= 0)
	{
		m_pSelectedSeason = m_ListContent.GetSeasonAt(nSel);
		EndDialog(IDOK);
	}
}

ISeason* OpenSeasonDlg::ChooseSeason(XContext* pContext)
{
	
	PtrEnumerator<ISeason> seasonsEnum = pContext->EnumSeasons();
	
	while(seasonsEnum.hasNext())
	{
		m_ListContent.AddSeason(*seasonsEnum);
		seasonsEnum.advance();
	}

	m_SeasonList.SetContent(&m_ListContent);

	if(DoModal() == IDOK)
	{
		m_ListContent.DestroyContent();
		return m_pSelectedSeason;
	}

	m_ListContent.DestroyContent();
	return NULL;
}

void OpenSeasonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_SEASONLIST,m_SeasonList);
}

BOOL OpenSeasonDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	
	return TRUE;
}

BEGIN_MESSAGE_MAP(OpenSeasonDlg, CDialogEx)
END_MESSAGE_MAP()


