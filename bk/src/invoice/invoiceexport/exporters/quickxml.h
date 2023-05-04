#ifndef __QUICKXML_H__
#define __QUICKXML_H__

#include <boost/variant.hpp>
#include <boost/assign/list_of.hpp>
#include <list>
#include <map>
#include <types/Decimal.h>

namespace quickxml
{
	
	class qname
	{
	public:
		qname(const std::wstring& nmspace, const std::wstring& localpart);
		qname(const std::wstring& localpart);
				
		const std::wstring& nmspace() const;
		const std::wstring& localpart() const;
		
	private:
		std::wstring m_namespace;
		std::wstring m_localpart;
	};
	
	

	class attribute
	{
	public:
		attribute(const std::wstring& name, const std::wstring& value);
		attribute(const std::wstring& name, int value);
		attribute(const std::wstring& name, double value);
		attribute(const std::wstring& name, const Decimal& value);
		
		const std::wstring& name() const;
		const std::wstring& value() const;
		

	private:
		std::wstring m_name;
		std::wstring m_value;
	};

	
	class element
	{
	public:
		element(const qname& name, const std::wstring& value);
		element(const qname& name, int value);
		element(const qname& name, double value);
		element(const qname& name, const Decimal& value);
		element(const qname& name, const std::list<element>& value);


		element(const qname& name, const std::list<attribute>& attrbutes, const std::wstring& value);

		const qname& name() const;
		const boost::variant< std::wstring, std::list<element> > value() const;
		const std::list<attribute>& attributes() const;

		void operator+=(const element& e);
		void operator+=(const std::list<element>& elements);
		
		void operator+=(const attribute& attrib);
		void operator+=(const std::list<attribute>& attributes);

	private:
		qname m_name;
		boost::variant< std::wstring, std::list<element> > m_value;
		std::list<attribute> m_attrbutes;
	};


	class serializer_state
	{
	public:
		int m_identLevel;
		std::map<std::wstring, std::wstring> nsPrefixes;
	};

	class namespace_element_visitor : public boost::static_visitor<void>
	{
	public:
		namespace_element_visitor(serializer_state* state);
		void operator()(const std::list<element>& elements) const;
		void operator()(const element& element) const;
		void operator()(const std::wstring& s) const;
	private:
		serializer_state * m_state;
	};

	class serializer;

	class element_visitor : public boost::static_visitor<void>
	{
	public:
		element_visitor(serializer* parent);
		void operator()(const std::list<element>& elements) const;
		void operator()(const element& element) const;
		void operator()(const std::wstring& s) const;
	private:
		serializer * m_parent;
	};

	class serializer : public boost::static_visitor<void>
	{
	public:
		serializer(std::wofstream& output);
		void serialize(const element& element);
	private:
		std::wstring getprefix(const std::wstring& nmspace);
		void serialize_attributes(const std::list<attribute>& attrbutes);
		void serialize_value(const boost::variant< std::wstring, std::list<element> >&value);
		void serialize_qname(const qname& name);
	private:
		std::wofstream& m_output;
		serializer_state* m_state;

		friend class element_visitor;
	};

};

#endif