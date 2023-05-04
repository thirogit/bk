#pragma once

#include "TemplateVisitor.h"

namespace bkprint
{
	class Template
	{
	public:
		virtual void Visit(TemplateVisitor* visitor) = 0;		
		virtual ~Template() = 0;
	};
}