#include "stdafx.h"
#include "PassportRecognitionProfileEditor.h"
#include "../../context/ContextException.h"
#include <string/TextUtils.h>

PassportRecognitionProfileEditor::PassportFieldEditorImpl::PassportFieldEditorImpl(PassportRecognitionProfile* profile, 
																				   const std::wstring& fieldName) :
																				   m_profile(profile), m_fieldName(fieldName)
{
}

IPassportField* PassportRecognitionProfileEditor::PassportFieldEditorImpl::GetField() const
{
	return m_profile->GetFieldByName(m_fieldName);
}

void PassportRecognitionProfileEditor::PassportFieldEditorImpl::SetW(int w)
{
	m_profile->SetFieldMetric(m_fieldName, W_METRIC, w);
}

void PassportRecognitionProfileEditor::PassportFieldEditorImpl::SetH(int h)
{
	m_profile->SetFieldMetric(m_fieldName, H_METRIC, h);
}

void PassportRecognitionProfileEditor::PassportFieldEditorImpl::SetX(int x)
{
	m_profile->SetFieldMetric(m_fieldName, X_METRIC, x);
}

void PassportRecognitionProfileEditor::PassportFieldEditorImpl::SetY(int y)
{
	m_profile->SetFieldMetric(m_fieldName, Y_METRIC, y);
}

//-------------------------------------------------------------------------------------------------

PassportRecognitionProfileEditor::PassportRecognitionProfileEditor(XContext* context) : m_pWorkingCopy(NULL),m_context(context)
{	
}

PassportRecognitionProfileEditor::~PassportRecognitionProfileEditor()
{
	if (m_pWorkingCopy)
		delete m_pWorkingCopy;

	DeepDelete(m_EditorsMap);
}

IPassportRecognitionProfile* PassportRecognitionProfileEditor::GetProfile() const
{
	ThrowIfNotInitialized();
	return m_pWorkingCopy;
}

void PassportRecognitionProfileEditor::SetXDimention(int xdim)
{
	ThrowIfNotInitialized();
	m_pWorkingCopy->SetXDimention(xdim);
}

void PassportRecognitionProfileEditor::SetYDimention(int ydim)
{
	ThrowIfNotInitialized();
	m_pWorkingCopy->SetYDimention(ydim);
}

void PassportRecognitionProfileEditor::SetProfileName(const std::wstring& sProfileName)
{
	ThrowIfNotInitialized();
	m_pWorkingCopy->SetProfileName(sProfileName);
}

PassportFieldEditor* PassportRecognitionProfileEditor::EditField(const std::wstring& sFieldName)
{
	ThrowIfNotInitialized();
	auto findIt = m_EditorsMap.find(sFieldName);
	if(m_EditorsMap.end() != findIt)
		return findIt->second;

	return NULL;
}
	
void PassportRecognitionProfileEditor::New()
{
	ThrowIfAlreadyInitialized();
	m_pWorkingCopy = new PassportRecognitionProfile(NULL_ID);
	CreateFieldEditors();
	
}

void PassportRecognitionProfileEditor::CreateFieldEditors()
{
	PtrEnumerator<IPassportField> fieldEnum = m_pWorkingCopy->EnumFields();
		
	IPassportField* field;
	std::wstring fieldName;
	while (fieldEnum.hasNext())
	{
		field = *fieldEnum;
		fieldName = field->GetDescriptor()->GetFieldName();
		m_EditorsMap[fieldName] = new PassportFieldEditorImpl(m_pWorkingCopy, fieldName);
		fieldEnum.advance();
	}
}

void PassportRecognitionProfileEditor::Edit(uint32_t profileId)
{
	ThrowIfAlreadyInitialized();

	IPassportRecognitionProfile* profileToEdit = m_context->GetPassportRecognitionProfile(profileId);
	if (!profileToEdit)
		throw PassportRecognitionProfileEditorException(TextUtils::Format(L"profile with id=%d not found in user's context", profileId));

	m_pWorkingCopy = new PassportRecognitionProfile(profileToEdit);
	CreateFieldEditors();
}

void PassportRecognitionProfileEditor::ThrowIfAlreadyInitialized() const
{
	if (m_pWorkingCopy)
		throw PassportRecognitionProfileEditorException(L"editor already initialized");
}

void PassportRecognitionProfileEditor::ThrowIfNotInitialized() const
{
	if (!m_pWorkingCopy)
		throw PassportRecognitionProfileEditorException(L"editor not initialized");
}

bool PassportRecognitionProfileEditor::IsEditing() const
{
	ThrowIfNotInitialized();
	return m_pWorkingCopy->GetId() != NULL_ID;
}

void PassportRecognitionProfileEditor::Save()
{
	try
	{
		if (IsEditing())
		{
			m_context->UpdatePassportRecognitionProfile(m_pWorkingCopy);
		}
		else
		{
			m_context->SaveNewPassportRecognitionProfile(m_pWorkingCopy);
		}
	}
	catch (ContextException& e)
	{
		throw PassportRecognitionProfileEditorException(e.wwhat());
	}
}