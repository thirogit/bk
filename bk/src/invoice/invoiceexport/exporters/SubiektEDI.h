#ifndef __SUBIEKTEDI_H__
#define __SUBIEKTEDI_H__

#include "InvoiceExporter.h"
#include <utility>
#include <tuple>

namespace epp
{
	
	template<class CharT, class Traits>
	std::basic_ostream<CharT, Traits>& wendlrn(std::basic_ostream<CharT, Traits>& os)
	{
		os.put(os.widen('\r'));
		os.put(os.widen('\n'));
		os.flush();
		return (os);
	}


	template<class... Types>
	struct case_tuple_type_genrator
	{
		typedef std::tuple<typename std::_Unrefwrap_t<Types>...> type;
	};


	template<class R,class... Types> class choose_t;

	template<class R,class... Types>
	class case_forwarder_t
	{
	public:

		typedef typename case_tuple_type_genrator<Types...>::type case_tuple;

		case_forwarder_t(choose_t<R,Types...>& parent, const case_tuple & conditions) : m_parent(parent), m_conditions(conditions)
		{
		}

		choose_t<R,Types...>& give(const R& value)
		{
			m_parent.m_mapping.insert(std::make_pair(m_conditions, value));
			return m_parent;
		}


	private:
		choose_t<R,Types...>& m_parent;
		case_tuple m_conditions;

	};

	template<class R,class... Types>
	class choose_t
	{
	public:

		typedef typename case_forwarder_t<R,Types...>::case_tuple key_type;
		choose_t()
		{
		}

		choose_t(const R& defaultv) : m_defaultValue(defaultv)
		{
		}

		choose_t(const choose_t& src) : m_defaultValue(src.m_defaultValue)
		{
			m_mapping = src.m_mapping;
		}


		choose_t(choose_t&& src) : m_defaultValue(src.m_defaultValue)
		{
			m_defaultValue.clear();
			m_mapping.swap(src.m_mapping);
		}

		case_forwarder_t<R,Types...> when(Types&&... args)
		{
			case_forwarder_t<R,Types...> casefwd(*this, case_forwarder_t<R,Types...>::case_tuple(std::forward<Types>(args)...));
			return casefwd;
		}

		R operator()(Types&&... args)
		{
			key_type key(std::forward<Types>(args)...);
			auto it = m_mapping.find(key);
			if (it != m_mapping.end())
			{
				return it->second;
			}
			return m_defaultValue;
		}

	private:
		std::map<key_type, R> m_mapping;
		R m_defaultValue;

		friend class case_forwarder_t<R,Types...>;
	};


	template<class _R,class... _Types>
	_NODISCARD constexpr choose_t<_R,typename std::_Unrefwrap_t<_Types>...> choose()
	{
		typedef choose_t<_R,typename std::_Unrefwrap_t<_Types>...> choose_type;
		return choose_type();
	}


	
	class printable
	{
	public:
		virtual void print(std::wofstream& output) = 0;

	};

	class null_paceholder {};

	class section_row : public printable
	{
	public:
		section_row& operator|(const wchar_t* str);
		section_row& operator|(const std::wstring& str);
		section_row& operator|(int i);
		section_row& operator|(const DateTime& dt);
		section_row& operator|(const Decimal& dec);
		section_row& operator|(const null_paceholder& ignored);

		virtual void print(std::wofstream& output);

	private:
		std::vector<std::wstring> m_columns;
	};

	class section_row_forwarder
	{
	public:
		section_row operator|(const wchar_t* str) const;
		section_row operator|(int i) const;
		
	private:
		
	};

	section_row operator|(const section_row_forwarder&,const std::wstring& str);


	class section : public printable
	{
	public:

		section(const std::wstring& name);			
		section& operator+=(const section_row&);
		virtual void print(std::wofstream& output);

	protected:
		std::wstring section_name;
		std::vector<section_row> m_rows;
	};

	class info_section : public section
	{
	public:
		info_section();
	};


	


	namespace
	{
		const section_row_forwarder row = section_row_forwarder();		
		const null_paceholder null = null_paceholder();
	}

	class content_header : public section
	{
	public:
		content_header(const std::wstring& contentname);
	private:
		std::wstring m_contentName;
	};


	class content : public section
	{
	public:
		content();
		virtual void print(std::wofstream& output);
	private:

	};

	class invoice_header : public section
	{
	public:
		invoice_header();
		invoice_header& operator &= (const section_row& r);
	private:
		section::operator+=;
	};

	
}



class SubiektEDI : public InvoiceExporter
{	
public:
	virtual std::wstring GetExporterName();
	virtual std::wstring GetFileExtension();
	virtual std::string GetDefaultEncoding();

	virtual void Export(  IInvoiceRange invoices,
										InvoiceExporterOptions& options,
										std::wofstream& output,
										SeasonSession* session,
										SimpleProgress *progress);
	
};

#endif