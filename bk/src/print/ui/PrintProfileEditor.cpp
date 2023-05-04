#include "stdafx.h"
#include "PrintProfileEditor.h"
#include <string/TextUtils.h>
#include "../../context/ContextException.h"

PrintProfileEditor::PrintProfileEditor(XContext* context) : m_workingCopy(NULL), m_context(context)
{
}

PrintProfileEditor::~PrintProfileEditor()
{
	delete m_workingCopy;
}

void PrintProfileEditor::New(const std::wstring& documentCode)
{
	m_workingCopy = new PrintProfile(NULL_ID);
	m_workingCopy->SetDocumentCode(documentCode);
}

void PrintProfileEditor::Edit(uint32_t profileId)
{
	IPrintProfile* profileToEdit = m_context->GetPrintProfile(profileId);
	if (!profileToEdit)
		throw PrintProfileEditorExeption(TextUtils::Format(L"profile with id = %d does not exist in user context",profileId));
	m_workingCopy = new PrintProfile(profileToEdit);
}

void PrintProfileEditor::throwIfNotInitialized() const
{
	if (!m_workingCopy)
		throw PrintProfileEditorExeption(L"profile editor not initialized");
}

void PrintProfileEditor::Save()
{
	try
	{
		if (IsUpdating())
			m_context->UpdatePrintProfile(m_workingCopy);
		else
			m_context->SaveNewPrintProfile(m_workingCopy);
	}
	catch (ContextException& e)
	{
		throw PrintProfileEditorExeption(e.wwhat());
	}
}

bool PrintProfileEditor::IsUpdating() const
{	
	return m_workingCopy->GetProfileId() != NULL_ID;
}

const std::wstring& PrintProfileEditor::GetProfileName() const
{
	throwIfNotInitialized();
	return m_workingCopy->GetProfileName();

}

void PrintProfileEditor::SetProfileName(const std::wstring& profileName)
{
	throwIfNotInitialized();
	m_workingCopy->SetProfileName(profileName);
}

void PrintProfileEditor::SetOption(const PrintOption& option)
{
	throwIfNotInitialized();
	m_workingCopy->AddOption(option);
}

void PrintProfileEditor::RemoveOption(const std::wstring& optionName)
{
	throwIfNotInitialized();
	m_workingCopy->RemoveOption(optionName);
}

DubiousPrintOption PrintProfileEditor::GetOption(const std::wstring& optionName)
{
	throwIfNotInitialized();
	PrintOption* option = m_workingCopy->GetOption(optionName);
	if (option)
		return DubiousPrintOption(*option);

	return DubiousPrintOption();
}

OrderSpecifier* PrintProfileEditor::GetOrderSpecifierAt(int index)
{
	return m_workingCopy->GetOrderSpecifierAt(index);
}

void PrintProfileEditor::DeleteOrderSpecifierAt(int index)
{
	m_workingCopy->DeleteOrderSpecifierAt(index);
}

void PrintProfileEditor::AddOrderSpecifier(const OrderSpecifier& specifier)
{
	OrderSpecifier* addedSpecifier = m_workingCopy->AddOrderSpecifier(specifier.fieldCode());
	addedSpecifier->asc(specifier.asc());
}

int PrintProfileEditor::GetOrderSpecifiersCount() const
{
	return m_workingCopy->GetOrderSpecifiersCount();
}

void PrintProfileEditor::ClearOrderSpecifiers()
{
	m_workingCopy->ClearOrderSpecifiers();
}