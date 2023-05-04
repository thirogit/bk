#include "stdafx.h"
#include "TemplateUnmarshaller.h"

#include "../ExpandableTemplate.h"
#include "../FixedTemplate.h"
#include "..\util\PaperTypeUtil.h"
#include "..\util\PaperOrientationUtil.h"
#include "TypesUnmarshaler.h"
#include "LogoSettingVisitor.h"

namespace bkprint
{
	Template* TemplateUnmarshaller::Unmarshal(pugi::xml_node & node)
	{
		Template* templ = NULL;
		std::wstring rootNodeName = node.name();
		if (rootNodeName == L"expandabletemplate")
		{
			ExpandableTemplate* expandableTempl = new ExpandableTemplate();
			expandableTempl->Unmarshal(node);
			
			templ = expandableTempl;
		}
		else if (rootNodeName == L"fixedtemplate")
		{
			FixedTemplate* fixedTempl = new FixedTemplate();
			fixedTempl->Unmarshal(node);
			templ = fixedTempl;
		}

		const fipImage* logoImg = m_logo.GetImage();
		if (logoImg != NULL && templ != NULL)
		{
			LogoSettingVisitor visitor(*logoImg);
			templ->Visit(&visitor);
		}
		return templ;
	}

	void TemplateUnmarshaller::SetLogo(const Logo& logo)
	{
		m_logo = logo;
	}
}