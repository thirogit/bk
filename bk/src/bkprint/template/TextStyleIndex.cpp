#include "stdafx.h"
#include "TextStyleIndex.h"
#include <arrays/DeepDelete.h>

namespace bkprint
{

	void TextStyleIndex::remove_style(const std::wstring& styleId)
	{
		boost::multi_index::index<IndexType, index_by_textStyleId>::type&  index = m_index.get<index_by_textStyleId>();
		auto rowIt = index.find(styleId);
		if (rowIt != index.end())
		{
			TextStyleElement* pStyle = *rowIt;
			index.erase(rowIt);
			delete pStyle;
		}
	}

	TextStyle TextStyleIndex::find_style(const std::wstring& id)
	{
		boost::multi_index::index<IndexType, index_by_textStyleId>::type&  index = m_index.get<index_by_textStyleId>();
		auto rowIt = index.find(id);
		if (rowIt != index.end())
		{
			return (*rowIt)->style;
		}
		return TextStyle();
	}

	void TextStyleIndex::add_style(const std::wstring& styleId, const TextStyle& style)
	{
		remove_style(styleId);

		TextStyleElement* element = new TextStyleElement();
		element->id = styleId;
		element->style = style;
		m_index.push_back(element);
	}

	void TextStyleIndex::clear()
	{
		DeepDelete(m_index);
		m_index.clear();
	}

	size_t TextStyleIndex::size() const
	{
		return m_index.size();
	}

	TextStyle TextStyleIndex::style_at(int index)
	{
		return m_index[index]->style;
	}
}