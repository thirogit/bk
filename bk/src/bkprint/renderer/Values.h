#ifndef __VALUES_H__
#define __VALUES_H__

#include <string>
#include <map>
namespace bkprint
{
	class Values
	{
	public:
		Values();
		Values(const Values& src);
		Values(Values&& src);
		Values& operator=(const Values& right);
		std::wstring Get(const std::wstring& name) const;
		void Set(const std::wstring& name, const std::wstring& value);
		Values& operator+=(const Values& values);
	private:
		std::map<std::wstring, std::wstring> m_values;
	};
}
#endif