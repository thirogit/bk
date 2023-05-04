#ifndef __ORDERSPECIFIER_H__
#define __ORDERSPECIFIER_H__

#include <string>

class OrderSpecifier
{
public:
	OrderSpecifier(const OrderSpecifier& src);
	OrderSpecifier(const std::wstring& sFieldCode);
	OrderSpecifier(const std::wstring& sFieldCode,bool bAsc);

	const std::wstring& fieldCode() const;	
	bool asc() const;	
	void asc(bool bAsc);	
private:
	std::wstring m_FieldCode;
	bool m_Asc;
};

#endif