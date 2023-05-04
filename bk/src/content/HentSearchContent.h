#pragma once

#include "../ctrls\contentlistctrl\ListContent.h"
#include "../text/HentText.h"
#include "../view/HentView.h"
#include "../context/ForwardSeasonObserver.h"
#include "HentRow.h"
#include <data\IHent.h>
#include "../context/SeasonSession.h"

class HentSearchContent : public ListContent,protected ForwardSeasonObserver
{
public:
	HentSearchContent();
	~HentSearchContent();
	virtual int GetRowCount() const;
	virtual ListRow* GetRow(int row);
	HentRow* GetHentAtRow(int row);
	virtual void SortContent(const SortAttributes& attributes);
	
	void CreateContent(SeasonSession* pSession,const std::wstring& sQueryPatern);
	void DestroyContent();
	
protected:
	void RefreshHentsRow(uint32_t hentId);
	bool IsHentMatching(const IHent* pHent);
	void AddRow(HentRow* row);
	
	virtual void  OnHentInserted(uint32_t hentId);
	virtual void  OnHentUpdated(uint32_t hentId);
	virtual void  OnHentDeleted(uint32_t hentId);
	virtual void  OnHentLocked(uint32_t hentId);
	virtual void  OnHentUnlocked(uint32_t hentId);	
	
private:	
	std::vector<HentRow*> m_Rows;
	std::map<uint32_t,HentRow*> m_RowToHentId;
	SeasonSession* m_pSession;
	std::wstring m_sQueryPattern;
};


