#ifndef __SORTATTRIBUTES_H__
#define __SORTATTRIBUTES_H__

#include "SortAttribute.h"
#include <arrays\Enumerator.h>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/mem_fun.hpp>



class SortAttributes
{
	struct tag_MemberId{};

	typedef boost::multi_index_container
	<
		SortAttribute*,
		boost::multi_index::indexed_by< 
										boost::multi_index::random_access<>,
										boost::multi_index::ordered_unique < boost::multi_index::tag<tag_MemberId>, boost::multi_index::const_mem_fun<SortAttribute,uint32_t, &SortAttribute::MemberId> >
										>
	> AttributeIndex;



public:
	SortAttributes();
	~SortAttributes();
	SortAttributes(const SortAttributes& src);
	SortAttributes(SortAttributes&& src);

	void Clear();
	size_t Count() const;
	
	void AddAttribute(const SortAttribute& attribute);	
	void SetAttribute(const SortAttribute& attribute);	
	PtrEnumerator<const SortAttribute> EnumAttributes() const;
	void RemoveAttribute(uint32_t memberId);
	const SortAttribute* GetAttribute(uint32_t memberId) const;
	
	size_t GetattributeCount() const;
	const SortAttribute* GetAttributeAt(uint32_t index) const;

	const SortAttribute* operator[](uint32_t index) const;
	
protected:
	SortAttribute* FindAttribute(uint32_t memberId) const;
private:
	AttributeIndex m_attributes; 
	
};

#endif