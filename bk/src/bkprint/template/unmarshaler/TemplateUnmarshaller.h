#ifndef __TEMPLATEUNMARSHALLER_H__
#define __TEMPLATEUNMARSHALLER_H__

#include <pugixml.hpp>
#include "../Template.h"
#include "TemplateUnmarshalException.h"
#include "../../../logo/Logo.h"

namespace bkprint
{	
	class ExpandableTemplate;
	class FixedTemplate;

	class TemplateUnmarshaller
	{		
	public:		
		Template* Unmarshal(pugi::xml_node & node);
		void SetLogo(const Logo& logo);

	private:
		Logo m_logo;
	};

}
#endif