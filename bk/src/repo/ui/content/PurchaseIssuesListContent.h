#pragma once
#include "..\..\..\ctrls\contentlistctrl\ListContent.h"
#include "../content/Row.h"
#include "../view/DownloadIssueViewCreator.h"
#include "../text/DownloadIssueViewText.h"
#include <vector>

class IssueRow : public Row<DownloadIssueView>
{
public:
	IssueRow(DownloadIssueView* pView);
	virtual std::wstring CellValue(uint32_t colId);
private:
	DownloadIssueViewText m_Text;
};

class PurchaseIssuesListContent : public ListContent
{	
public:
	PurchaseIssuesListContent();
	~PurchaseIssuesListContent();
	
	virtual int GetRowCount() const;
	virtual ListRow* GetRow(int row);

	virtual void SortContent(const SortAttributes& attributes);
			
	void AddRow(IPurchaseDownloadIssue* issue);
	IssueRow* GetIssueRowAt(int row);

	void Clear();
	
protected:
	std::vector<IssueRow*> m_Rows;
	DownloadIssueViewCreator m_viewCreator;
};


