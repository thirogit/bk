#include "stdafx.h"
#include "FieldFactory.h"
#include <boost/functional/factory.hpp>


#include "RectField.h"
#include "TextField.h"
#include "ImageField.h"
#include "LogoField.h"

namespace bkprint
{

	FieldFactory::FieldFactory()
	{
		m_factoryies[L"text"] = boost::factory<TextField*>();
		m_factoryies[L"rect"] = boost::factory<RectField*>();
		m_factoryies[L"img"] = boost::factory<ImageField*>();
		m_factoryies[L"logo"] = boost::factory<LogoField*>();

	}

	Field* FieldFactory::Create(const std::wstring& tagName)
	{
		auto factoryIt = m_factoryies.find(tagName);
		if (factoryIt != m_factoryies.end())
		{
			return factoryIt->second();
		}

		return NULL;

	}
}