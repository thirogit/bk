#ifndef __LABEL_H__
#define __LABEL_H__

#include <string>

class Label 
{
public:
	Label(const std::wstring& label);
	Label(const std::wstring& label,int row);
	explicit Label(int row);
	operator const std::wstring&() const;
	const std::wstring& ToString() const;	
private:
	std::wstring m_label;

};



#endif 
