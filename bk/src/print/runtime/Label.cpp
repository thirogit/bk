#include "stdafx.h"
#include "Label.h"
#include <string/TextUtils.h>
#include <string/IntString.h>

Label::Label(const std::wstring& label) : m_label(label)
{

}

Label::Label(int row)
{
	m_label = IntString(row);
}

Label::Label(const std::wstring& label,int row)
{
	m_label = label + IntString(row);
}

Label::operator const std::wstring&() const
{
	return  m_label;
}

const std::wstring& Label::ToString() const
{
	return  m_label;
};