#include "stdafx.h"
#include "ExistingItemEditor.h"
#include "../runtime/RecognitionException.h"
#include <string/TextUtils.h>

ExistingItemEditor::ExistingItemEditor(PassportRecognitionSession* pSession, PassportImagePtr passportImage) : m_pSession(pSession), m_passportImage(passportImage)
{
}

void ExistingItemEditor::Edit(ItemId id)
{
	m_Id = id;
	IRecognitionItem* item = m_pSession->GetItem(id);
	if (item == NULL)
	{
		throw RecognitionException(TextUtils::Format(L"there is no item %s in session", ItemIdText::ToString(id)));
	}

	m_Details.CopyFrom(item->GetCow());
}

bool ExistingItemEditor::IsEditing() const
{
	return true;
}

void ExistingItemEditor::Save()
{
	m_pSession->UpdateCow(m_Id, m_Details, m_passportImage);
}