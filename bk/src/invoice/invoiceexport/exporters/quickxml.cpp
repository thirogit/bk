#include "stdafx.h"
#include "quickxml.h"
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <codecvt>
#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <boost/foreach.hpp>
#include <boost/range/adaptors.hpp>
#include <string/IntString.h>
#include <string/TextUtils.h>

using namespace boost::assign;

namespace quickxml
{

	serializer::serializer(std::wofstream& output) : m_output(output), m_state(NULL)
	{
	}

	std::wstring serializer::getprefix(const std::wstring& nmspace)
	{
		auto it = m_state->nsPrefixes.find(nmspace);
		if (it != m_state->nsPrefixes.end())
		{
			return it->second;
		}

		return L"";
	}


	void serializer::serialize_attributes(const std::list<attribute>& attributes)
	{
		auto it = attributes.begin();
		while(it != attributes.end())
		{
			m_output << it->name() << L"=\"" << it->value() << L"\"";
			it++;
			if (it != attributes.end())
			{
				m_output << L" ";
			}
		}
	}

	void serializer::serialize_value(const boost::variant< std::wstring, std::list<element> >&value)
	{
		element_visitor visitor(this);
		boost::apply_visitor(visitor, value);
	}

	void serializer::serialize_qname(const qname& name)
	{
		if (!name.nmspace().empty())
		{
			m_output << getprefix(name.nmspace()) << L":" << name.localpart();
		}
		else
		{
			m_output << name.localpart();
		}

	}
	
	void serializer::serialize(const element& elem)
	{		
		m_state = new serializer_state;
		m_state->m_identLevel = 0;
		
		namespace_element_visitor nsvisitor(m_state);
		nsvisitor(elem);
		boost::apply_visitor(nsvisitor, elem.value());
		
		m_output << L"<";

		serialize_qname(elem.name());

		if (!m_state->nsPrefixes.empty())
		{
			m_output << L" ";

			BOOST_FOREACH(std::wstring ns, m_state->nsPrefixes | boost::adaptors::map_keys)
			{
				m_output << L"xmlns:" << getprefix(ns) << "=\"" << ns << "\"";
			}
		}
		
		
		if (!elem.attributes().empty())
		{
			m_output << L" ";
			serialize_attributes(elem.attributes());
		}

		m_output << L">";

		serialize_value(elem.value());
		
		m_output << L"</";
		serialize_qname(elem.name());
		m_output << L">";
		
	}

	//-------------------

	element_visitor::element_visitor(serializer* parent) : m_parent(parent)
	{
	}

	void element_visitor::operator()(const std::list<element>& elements) const
	{
		auto it = elements.begin();
		while (it != elements.end())
		{			
			this->operator()(*it);
			it++;
		}
	}

	void element_visitor::operator()(const element& elem) const
	{
		m_parent->m_output << L"<";
		
		m_parent->serialize_qname(elem.name());

		if (!elem.attributes().empty())
		{
			m_parent->m_output << L" ";
			m_parent->serialize_attributes(elem.attributes());
		}

		m_parent->m_output << L">";

		m_parent->serialize_value(elem.value());

		m_parent->m_output << L"</";
		m_parent->serialize_qname(elem.name());
		m_parent->m_output << L">";

	}

	void element_visitor::operator()(const std::wstring& s) const
	{
		m_parent->m_output << s;
	}

	

	//-------------------

	namespace_element_visitor::namespace_element_visitor(serializer_state* state) : m_state(state)
	{
	}


	void namespace_element_visitor::operator()(const element& element) const
	{
		qname name = element.name();
		if (m_state->nsPrefixes.find(name.nmspace()) == m_state->nsPrefixes.end())
		{
			m_state->nsPrefixes[name.nmspace()] = TextUtils::Format(L"ns%d",m_state->nsPrefixes.size() + 1);
		}

		boost::apply_visitor(*this, element.value());
	}

	void namespace_element_visitor::operator()(const std::list<element>& elements) const
	{
		auto it = elements.begin();
		while (it != elements.end())
		{
			boost::apply_visitor(*this, it->value());
			it++;
		}
	}

	void namespace_element_visitor::operator()(const std::wstring& s) const
	{
	}

	
	//-------------------

	element::element(const qname& name, const std::wstring& value) : m_name(name)
	{		
		m_value = value;
	}

	element::element(const qname& name, const std::list<element>& value) : m_name(name)
	{
		m_value = value;
	}

	element::element(const qname& name, int value) : m_name(name)
	{		
		m_value = boost::lexical_cast<std::wstring>(value);
	}

	element::element(const qname& name, const Decimal& value) : m_name(name)
	{	
		m_value = value.ToString(5);
	}

	element::element(const qname& name, double value) : m_name(name)
	{		
		m_value = boost::lexical_cast<std::wstring>(value);
	}


	element::element(const qname& name, const std::list<attribute>& attrbutes, const std::wstring& value) : m_name(name)
	{
		m_value = value;
		m_attrbutes = attrbutes;
	}

	const qname& element::name() const
	{
		return m_name;
	}

	const boost::variant< std::wstring, std::list<element> > element::value() const
	{
		return m_value;
	}

	const std::list<attribute>& element::attributes() const
	{
		return m_attrbutes;
	}

	void element::operator+=(const element& e)
	{
		if (typeid(std::list<element>) == m_value.type())
		{
			std::list<element>& list = boost::get<std::list<element>>(m_value);
			list.insert(list.end(), e);
		}
	}

	void element::operator+=(const std::list<element>& elements)
	{
		if (typeid(std::list<element>) == m_value.type())
		{
			std::list<element>& list = boost::get<std::list<element>>(m_value);
			auto it = elements.begin();
			while (it != elements.end())
			{
				list.insert(list.end(), *it);
				it++;
			}
		}
	}

	//-------------------


	attribute::attribute(const std::wstring& name, const std::wstring& value) : m_name(name)
	{
		m_value = value;
	}

	attribute::attribute(const std::wstring& name, int value) : m_name(name)
	{
		m_value = boost::lexical_cast<std::wstring>(value);
	}

	attribute::attribute(const std::wstring& name, double value) : m_name(name)
	{
		m_value = boost::lexical_cast<std::wstring>(value);
	}

	attribute::attribute(const std::wstring& name, const Decimal& value) : m_name(name)
	{
		m_value = value.ToString(5);
	}

	const std::wstring& attribute::name() const
	{
		return m_name;
	}

	const std::wstring& attribute::value() const
	{
		return m_value;
	}
	
	//-------------------

	qname::qname(const std::wstring& nmspace, const std::wstring& localpart) : m_namespace(nmspace), m_localpart(localpart)
	{
	}

	qname::qname(const std::wstring& localpart) : m_localpart(localpart)
	{
	}

	const std::wstring& qname::nmspace() const
	{
		return m_namespace;
	}

	const std::wstring& qname::localpart() const
	{
		return m_localpart;
	}




}
