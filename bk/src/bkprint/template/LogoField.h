#ifndef __LOGOFIELD_H__
#define __LOGOFIELD_H__

#include "RectField.h"
#include "FreeImagePlus.h"

namespace bkprint
{

	class LogoField : public RectField
	{
	public:
		LogoField();
		virtual ~LogoField();
	
		virtual void Unmarshal(pugi::xml_node & node);
		virtual void Visit(FieldVisitor* visitor);	
		virtual Field* Clone();
		void CopyFrom(const LogoField& src);
		const fipWinImage& GetImage() const;
		void SetImage(const fipImage& src);

	protected:	
		fipWinImage m_image;
	};

}

#endif 
