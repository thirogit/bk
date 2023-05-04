#pragma once

#include "../resource.h"
#include "../ctrls/contentlistctrl\ContentListCtrl.h"
#include "../ctrls/contentlistctrl\NullListRow.h"
#include <data/ISeason.h>
#include "../text\SeasonText.h"
#include "../context/XContext.h"


class SeasonListContent : public ListContent
{
	class SeasonListRow : public NullListRow
	{
	public:
		SeasonListRow(ISeason* pSeason);
		virtual std::wstring CellValue(uint32_t colId);
		ISeason* Content() const;
	private:
		SeasonText m_SeasonText;
		ISeason* m_pSeason;
	};

public:
	SeasonListContent();
	~SeasonListContent();
	void AddSeason(ISeason* season);
	void DestroyContent();
	virtual void AddContentObserver(ContentObserver* pObserver);
	virtual int GetRowCount() const;
	virtual ListRow* GetRow(int row);
	virtual void SortContent(const SortAttributes& attributes);
	ISeason* GetSeasonAt(int iIndex);

private:
	std::vector<SeasonListRow*> m_Content;
};

class OpenSeasonDlg : public CDialogEx
{
public:
	OpenSeasonDlg(CWnd* pParent = NULL);   
	virtual ~OpenSeasonDlg();
	enum { IDD = IDD_OPENSEASON };
	ISeason* ChooseSeason(XContext* pContext);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//INT_PTR CDialogEx::DoModal;
private:
	CContentListCtrl m_SeasonList;
	SeasonListContent m_ListContent;
	ISeason* m_pSelectedSeason;
	
	DECLARE_MESSAGE_MAP()
};
