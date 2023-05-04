#include "stdafx.h"
#include "LonlyCowEditor.h"


LonlyCowEditor::LonlyCowEditor(SeasonSession* session) : m_pSession(session),m_CowId(NULL_ID)
{
}

void LonlyCowEditor::Edit(uint32_t cowId)
{
	m_CowId = cowId;
	CowClientObject* pCowCO = m_pSession->GetCow(m_CowId);
	ICow* pCow = pCowCO->Object();
	m_Details.CopyFrom(pCow);
}

bool LonlyCowEditor::IsEditing() const
{
	return true;
}

void LonlyCowEditor::Save()
{
	m_pSession->UpdateCow(m_CowId,m_Details);						
}