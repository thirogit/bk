#ifndef __NULLVISITOR_H__
#define __NULLVISITOR_H__

#include "FieldVisitor.h"

namespace bkprint
{	
	class NullVisitor : public FieldVisitor
	{
	public:
		virtual void Accept(RectField* field);
		virtual void Accept(TextField* field);
		virtual void Accept(ImageField* field);
		virtual void Accept(LogoField* field);

	};

}

#endif 
