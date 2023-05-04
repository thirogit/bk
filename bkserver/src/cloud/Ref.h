#pragma once

#include <string>
#include <vector>

namespace gcp
{
	class Ref
	{
	public:
		Ref();
		Ref(const std::wstring& s);
		Ref(const Ref& src);
		Ref(Ref&& src);
		Ref& operator/=(const std::wstring& segment);
		Ref& operator/=(const Ref& path);
		std::wstring ToString() const;
		std::wstring LastSegment();
	private:
		std::vector<std::wstring> m_segments;
	};

	Ref operator/(const Ref& ref, const std::wstring& right);
	
};
