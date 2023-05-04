#include "stdafx.h"
#include "DocumentCfgDescription.h"
#include <arrays\DeepDelete.h>
#include <boost\foreach.hpp>
#include <boost\range\adaptors.hpp>

DocumentCfgDescription::DocumentCfgDescription()
{

}

DocumentCfgDescription::~DocumentCfgDescription()
{
	DeepDelete(m_options);
	DeepDelete(m_comparableFields);
}

DocumentCfgDescription::DocumentCfgDescription(const DocumentCfgDescription& src)
{
	BOOST_FOREACH(DocumentOptionDefinition* optionDefinition, src.m_options | boost::adaptors::map_values)
	{
		AddOptionDefinition(*optionDefinition);
	}

	BOOST_FOREACH(ComparableFieldDefinition* cmpFieldDefinition, src.m_comparableFields)
	{
		AddComparableFieldDefinition(*cmpFieldDefinition);
	}
}

DocumentCfgDescription::DocumentCfgDescription(DocumentCfgDescription&& src)
{
	BOOST_FOREACH(DocumentOptionDefinition* definition, src.m_options | boost::adaptors::map_values)
	{
		m_options[definition->GetName()] = definition;
	}

	BOOST_FOREACH(ComparableFieldDefinition* cmpFieldDefinition, src.m_comparableFields)
	{
		m_comparableFields.push_back(cmpFieldDefinition);
	}

	src.m_options.clear();
	src.m_comparableFields.clear();
}

void DocumentCfgDescription::AddOptionDefinition(const DocumentOptionDefinition& definition)
{
	auto it = m_options.find(definition.GetName());
	if (it != m_options.end())
	{
		delete it->second;
		m_options.erase(it);
	}	

	m_options[definition.GetName()] = new DocumentOptionDefinition(definition);
}


void DocumentCfgDescription::CopyOptionDefinitionsFrom(const DocumentCfgDescription& definitions)
{
	DeepDelete(m_options);
	m_options.clear();
	PtrEnumerator<DocumentOptionDefinition> enumDefinitions = definitions.EnumOptionDefinitions();
	while (enumDefinitions.hasNext())
	{
		AddOptionDefinition(**enumDefinitions);
		enumDefinitions.advance();
	}
}

void DocumentCfgDescription::CopyComparableFieldsFrom(const DocumentCfgDescription& description)
{
	m_comparableFields.clear();
	BOOST_FOREACH(ComparableFieldDefinition* comparableField, description.m_comparableFields)
	{
		m_comparableFields.push_back(new ComparableFieldDefinition(*comparableField));
	}
}

void DocumentCfgDescription::AddComparableFieldDefinition(const ComparableFieldDefinition& definition)
{
	m_comparableFields.push_back(new ComparableFieldDefinition(definition));
}

PtrEnumerator<ComparableFieldDefinition> DocumentCfgDescription::EnumComparableFieldDefinitions() const
{
	return PtrEnumerator<ComparableFieldDefinition>(m_comparableFields);
}

DocumentOptionDefinition* DocumentCfgDescription::GetOptionDefinition(const std::wstring& optionCode)
{
	auto it = m_options.find(optionCode);
	if (it != m_options.end())
		return it->second;

	return NULL;
}

PtrEnumerator<DocumentOptionDefinition> DocumentCfgDescription::EnumOptionDefinitions() const
{
	return PtrEnumerator<DocumentOptionDefinition>(m_options | boost::adaptors::map_values);
}

