#ifndef __HEADERCOLUMN_H__
#define __HEADERCOLUMN_H__

#include <string>
#include <cstdint>

class HeaderColumn
{
public:
	HeaderColumn(uint32_t colId,const std::wstring& colText);
	HeaderColumn(const HeaderColumn& src);
	HeaderColumn& operator=(const HeaderColumn& right);
	uint32_t GetColId() const;
	const std::wstring& GetColText() const;
private:
	uint32_t m_ColId;
	std::wstring m_ColText;
};


#endif