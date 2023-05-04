#ifndef __FIELDVISITOR_H__
#define __FIELDVISITOR_H__


namespace bkprint
{

	class RectField;
	class TextField;
	class ImageField;
	class LogoField;

	class FieldVisitor
	{
	public:
		virtual void Accept(RectField* field) = 0;
		virtual void Accept(TextField* field) = 0;
		virtual void Accept(ImageField* field) = 0;
		virtual void Accept(LogoField* field) = 0;

	};

}

#endif 
