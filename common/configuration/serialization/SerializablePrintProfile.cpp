#include "stdafx.h"
#include "SerializablePrintProfile.h"
//#include "../../types/serialization/TypesSerialization.h"


SerializablePrintProfile::SerializablePrintProfile(PrintProfile* pProfile) : m_pProfile(pProfile)
{
}

void SerializablePrintProfile::serialize(SF::Archive &archive)
{
	PrintOption* option;
	OrderSpecifier* orderSpecifier;

	archive & Id();
	archive & ProfileName();
	archive & DocumentCode();

	count_t optionCount;
	count_t orderSpecifiersCount;

	if (archive.isRead())
	{
		std::wstring optionName, optionValue, fieldCode;
		bool ascending;

		archive & optionCount;

		for (count_t i = 0; i < optionCount; i++)
		{
			archive & optionName;
			archive & optionValue;

			m_pProfile->AddOption(optionName)->set(optionValue);
		}

		archive & orderSpecifiersCount;

		for (count_t i = 0; i < orderSpecifiersCount; i++)
		{
			archive & fieldCode;
			archive & ascending;

			m_pProfile->AddOrderSpecifier(fieldCode)->asc(ascending);
		}
	}
	else
	{
		optionCount = m_pProfile->GetOptionsCount();
		archive & optionCount;

		PtrEnumerator<PrintOption> enumOptions = m_pProfile->EnumOptions();
		while (enumOptions.hasNext())
		{
			option = *enumOptions;

			archive & option->name();
			archive & option->get();

			enumOptions.advance();
		}

		orderSpecifiersCount = m_pProfile->GetOrderSpecifiersCount();
		archive & orderSpecifiersCount;

		PtrEnumerator<OrderSpecifier> enumOrderSpecifiers = m_pProfile->EnumOrderSpecifiers();
		while (enumOrderSpecifiers.hasNext())
		{
			orderSpecifier = *enumOrderSpecifiers;

			archive & orderSpecifier->fieldCode();
			archive & orderSpecifier->asc();

			enumOrderSpecifiers.advance();
		}
	}
}

uint32_t& SerializablePrintProfile::Id()
{
	return m_pProfile->m_profileId;
}

std::wstring& SerializablePrintProfile::ProfileName()
{
	return m_pProfile->m_profileName;
}

std::wstring& SerializablePrintProfile::DocumentCode()
{
	return m_pProfile->m_documentCode;
}

