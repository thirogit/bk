#ifndef __LONLYCOWEDITOR_H__
#define __LONLYCOWEDITOR_H__

#include "../context/transaction/CowEditor.h"
#include "../context/SeasonSession.h"

class LonlyCowEditor : public CowEditor
{
public:
	LonlyCowEditor(SeasonSession* pSession);	
	void Edit(uint32_t cowId);
	virtual bool IsEditing() const;
	virtual void Save();
private:
	SeasonSession* m_pSession;
	uint32_t m_CowId;
};

#endif