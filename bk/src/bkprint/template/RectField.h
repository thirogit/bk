#ifndef __RECTFIELD_H__
#define __RECTFIELD_H__

#include "Field.h"
#include "../../uitypes/Color.h"
#include "Border.h"

namespace bkprint
{

	class RectField : public Field
	{
	public:
		RectField();
		virtual ~RectField();
	
		virtual void Unmarshal(pugi::xml_node & node);
		virtual void Visit(FieldVisitor* visitor);	
		virtual Field* Clone();
		void CopyFrom(const RectField& src);

		const Border& GetBorder() const;
		void SetBorder(const Border& border);

		const Color& GetFillColor() const;
		void SetFillColor(const Color& color);

	protected:	
		Border m_border;
		Color m_fillClr;
	};

}

#endif 
