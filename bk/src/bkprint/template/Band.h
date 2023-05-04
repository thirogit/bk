#ifndef __BAND_H__
#define __BAND_H__

#include "FieldsContainer.h"

namespace bkprint
{
	class Band
	{
	public:
		Band(const std::wstring& name);
		
		FieldsContainer& FooterFields();
		FieldsContainer& BodyFields();
		FieldsContainer& HeaderFields();
		const std::wstring& Name() const;

		void Unmarshal(pugi::xml_node & node);
	private:
		FieldsContainer m_header;
		FieldsContainer m_body;
		FieldsContainer m_footer;

		std::wstring m_name;
	};
}

#endif