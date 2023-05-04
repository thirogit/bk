#ifndef __FIELD_H__
#define __FIELD_H__

#include <string>
#include <pugixml.hpp>
#include "FieldVisitor.h"

namespace bkprint
{
	class Field 
	{
	public:
		Field();
		virtual ~Field();	
		virtual Field* Clone() = 0;		
		virtual void Visit(FieldVisitor* visitor) = 0;	
		virtual void Unmarshal(pugi::xml_node& node);
			
		const ::std::wstring& GetLabel() const;
		const CPoint& GetPosition() const; 
		const CSize& GetSize() const;
	
		void SetLabel(const ::std::wstring& label);
		void SetPosition(const CPoint& position);
		void SetSize(const CSize& size);
			
	protected:

		void CopyFrom(const Field& src);
		

		CPoint m_position;
		CSize m_size;
		::std::wstring m_label;	
	};

}

#endif 
