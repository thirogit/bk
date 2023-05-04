#pragma once

#include "..\ctrls\contentlistctrl\ListContent.h"
#include "../text/HentText.h"
#include "../view/HentView.h"
#include "../context/SeasonSession.h"
#include "../context/ForwardSeasonObserver.h"
#include "../content/HentRow.h"
#include "../content/FindResult.h"
#include "../content/predicate/Predicate.h"


class HentsListContent : public ListContent,protected ForwardSeasonObserver
{
public:
	HentsListContent();
	~HentsListContent();
	virtual int GetRowCount() const;
	virtual ListRow* GetRow(int row);
	virtual void SortContent(const SortAttributes& attributes);
	HentRow* GetHentAtRow(int row);
	void CreateContent(SeasonSession* pSession);
	void DestroyContent();
	FindResult FindHents(Predicate<HentView>* predicate);
	void ClearFound();
	void SortFound();
protected:
	void RefreshHentsRow(uint32_t hentId);

	virtual void  BeforeHentChange();
	virtual void  OnHentInserted(uint32_t hentId);
	virtual void  OnHentUpdated(uint32_t hentId);
	virtual void  OnHentDeleted(uint32_t hentId);
	virtual void  OnHentLocked(uint32_t hentId);
	virtual void  OnHentUnlocked(uint32_t hentId);	
	virtual void  AfterHentChange();	
private:	
	std::vector<HentRow*> m_Rows;
	std::map<uint32_t,HentRow*> m_RowToHentId;
	SeasonSession* m_pSession;
};
