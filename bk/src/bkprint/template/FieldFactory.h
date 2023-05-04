#ifndef __FIELDFACTORY_H__
#define __FIELDFACTORY_H__

#include "Field.h"
#include <boost\function.hpp>
#include <map>

namespace bkprint
{
	class FieldFactory
	{
		typedef boost::function< Field*() > doc_object_factory;
	public:
		FieldFactory();
		Field* Create(const std::wstring& tagName);

	private:
		std::map<std::wstring, doc_object_factory> m_factoryies;

	};
}

#endif
