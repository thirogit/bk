#pragma once

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include "TextStyle.h"

namespace bkprint
{
	class TextStyleIndex
	{
		struct index_by_textStyleId{};

		struct TextStyleElement
		{
			std::wstring id;
			TextStyle style;
		};

		typedef boost::multi_index_container
			<
			TextStyleElement*,
			boost::multi_index::indexed_by<
			boost::multi_index::random_access<>,
			boost::multi_index::ordered_unique < boost::multi_index::tag<index_by_textStyleId>, boost::multi_index::member<TextStyleElement, std::wstring, &TextStyleElement::id> >
			>
			> IndexType;
	public:

		void remove_style(const std::wstring& styleId);
		TextStyle find_style(const std::wstring& id);

		void add_style(const std::wstring& styleId,const TextStyle& style);
		void clear();
		size_t size() const;
		TextStyle style_at(int index);

	private:
		IndexType m_index;

	};

}
