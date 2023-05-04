#include "stdafx.h"
#include "NewItemEditor.h"


NewItemEditor::NewItemEditor(PassportRecognitionSession* session, PassportImagePtr passportImage) : m_pSession(session), m_passportImage(passportImage)
{
}

bool NewItemEditor::IsEditing() const
{
	return true;
}

void NewItemEditor::Save()
{
	m_pSession->AddCow(m_Details, m_passportImage);
}