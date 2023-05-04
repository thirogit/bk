#ifndef __TEMPLATEUNMARSHALEXCEPTION_H__
#define __TEMPLATEUNMARSHALEXCEPTION_H__

#include <exception/TagException.h>

namespace bkprint
{
	struct tag_TemplateUnmarshalException {};
	typedef TagException<tag_TemplateUnmarshalException> TemplateUnmarshalException;
}
#endif