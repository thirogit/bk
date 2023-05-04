#include "stdafx.h"
#include "sagejson.h"
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <codecvt>
#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <boost/foreach.hpp>
#include "../../../text/InvoiceNoText.h"
#include "../../../text/PayWayString.h"
#include <string/IntString.h>
#include "../../calculator/InvoiceCalculator.h"
#include <string/TextUtils.h>

using namespace boost::assign;

namespace sagejson
{

serializer::serializer(std::wofstream& output, int ident/* = 0*/) : m_output(output), m_identLevel(ident)
{
}

void serializer::operator()(const std::wstring& s) const
{
	//m_output << ;
	m_output << L"=";
	m_output << s;
	m_output << std::endl; 
}

void serializer::operator()(const std::list<object> & objs) const
{
	m_output << L"{" << std::endl;

	auto it = objs.begin();
	while (it != objs.end())
	{
		std::wstring name = it->name();
		m_output << name;
		boost::apply_visitor(serializer(m_output), it->value());
		it++;
	}

	m_output << L"}" << std::endl;
}

//-------------------

object::object(const std::wstring& name, const std::wstring& value)
{
	m_name = name;
	m_value = value;
}

object::object(const std::wstring& name, const std::list<object>& value)
{
	m_name = name;
	m_value = value;
}

object::object(const std::wstring& name, int value)
{
	m_name = name;
	m_value = boost::lexical_cast<std::wstring>(value);
}

object::object(const std::wstring& name, const Decimal& value)
{
	m_name = name;
	m_value = value.ToString(5);
}

object::object(const std::wstring& name, double value)
{
	m_name = name;
	m_value = boost::lexical_cast<std::wstring>(value);
}

const std::wstring& object::name() const
{
	return m_name;
}

const boost::variant< std::wstring, std::list<object> > object::value() const
{
	return m_value;
}

void object::operator+=(const object& obj)
{
	if (typeid(std::list<object>) == m_value.type())
	{
		std::list<object>& list = boost::get<std::list<object>>(m_value);
		list.insert(list.end(), obj);
	}
}

void object::operator+=(const std::list<object>& objs)
{
	if (typeid(std::list<object>) == m_value.type())
	{
		std::list<object>& list = boost::get<std::list<object>>(m_value);
		auto it = objs.begin();
		while (it != objs.end())
		{
			list.insert(list.end(), *it);
			it++;
		}
	}
}

}
