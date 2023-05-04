#ifndef __IMAGEFIELD_H__
#define __IMAGEFIELD_H__

#include "RectField.h"
#include "FreeImagePlus.h"

namespace bkprint
{

	class ImageField : public RectField
	{
	public:
		ImageField();
		virtual ~ImageField();
	
		virtual void Unmarshal(pugi::xml_node & node);
		virtual void Visit(FieldVisitor* visitor);	
		virtual Field* Clone();
		void CopyFrom(const ImageField& src);
		const fipWinImage& GetImage() const;

	protected:	
		fipWinImage m_image;
	};

}

#endif 
