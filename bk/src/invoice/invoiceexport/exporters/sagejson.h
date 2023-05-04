#ifndef __SAGEJSON_H__
#define __SAGEJSON_H__

#include <boost/variant.hpp>
#include <boost/assign/list_of.hpp>
#include <list>
#include <types/Decimal.h>

namespace sagejson
{
	class object
	{
	public:
		object(const std::wstring& name, const std::wstring& value);
		object(const std::wstring& name, int value);
		object(const std::wstring& name, double value);
		object(const std::wstring& name, const Decimal& value);
		object(const std::wstring& name, const std::list<object>& value);

		const std::wstring& name() const;
		const boost::variant< std::wstring, std::list<object> > value() const;

		void operator+=(const object& obj);
		void operator+=(const std::list<object>& objs);

	private:
		std::wstring m_name;
		boost::variant< std::wstring, std::list<object> > m_value;
	};

	class serializer : public boost::static_visitor<void>
	{
	public:
		serializer(std::wofstream& output, int identLevel = 0);

		void operator()(const std::wstring& s) const;
		void operator()(const std::list<object> & objs) const;
	private:
		std::wofstream& m_output;
		int m_identLevel;

	};

};

#endif