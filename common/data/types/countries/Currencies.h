#ifndef __CURRENCIES_H__
#define __CURRENCIES_H__

//#include "Country.h"
//#include <boost/multi_index_container.hpp>
//#include <boost/multi_index/ordered_index.hpp>
//#include <boost/multi_index/mem_fun.hpp>

class Currencies
{		
	struct index_tag_isonumber{};
	struct index_tag_code2a{};
		
	typedef boost::multi_index_container
	<
	  Country*,
	  boost::multi_index::indexed_by< 
										boost::multi_index::ordered_unique < boost::multi_index::tag<index_tag_isonumber>, boost::multi_index::const_mem_fun<Country,unsigned int, &Country::GetCountryISONumber> >,
										boost::multi_index::ordered_unique < boost::multi_index::tag<index_tag_code2a>, boost::multi_index::const_mem_fun<Country,std::string, &Country::GetCountryCode2A> >
									>
	> CountriesIndex;

public:	
	class Enumerator : public std::iterator<std::output_iterator_tag, const Country*>
	{
		CountriesIndex::const_iterator m_It;
		CountriesIndex::const_iterator m_endIt;

		Enumerator(const CountriesIndex& countries) {m_It = countries.begin(); m_endIt = countries.end(); }		
	public:		
		Enumerator(const Enumerator& src) : m_endIt(src.m_endIt), m_It(src.m_It) {}
		Enumerator& operator++() {++m_It;return *this;}
		Enumerator operator++(int) {Enumerator tmp(*this); operator++(); return tmp;}
		bool operator==(const Enumerator& rhs) {return m_It==rhs.m_It;}
		bool operator!=(const Enumerator& rhs) {return m_It!=rhs.m_It;}
		const Country* operator*() {return m_It.get_node()->value();}
		bool hasNext() { return m_It != m_endIt; };
		const Country* operator->() { return m_It.get_node()->value(); }
		
		friend class Countries;
	};



private:
	Countries();
	~Countries();
public:
	static Enumerator EnumCountries();
	static const Country GetCountryByNumber(unsigned int number);
	static const Country GetCountryByCode2a(const std::string& code2a);	
	
private:
	CountriesIndex countriesIndex;	
	static Countries instance;
};



#endif