#include "stdafx.h"
#include "EncodingCombo.h"
#include <list>
#include <boost/foreach.hpp>

CEncodingCombo::EncodingItem CEncodingCombo::_encodings[] =
{
	{ "windows1250", L"Windows 1250"},
	{ "iso88592", L"ISO 8859-2" },
	{ "utf8", L"UTF-8" },
	{NULL,NULL}
};


void CEncodingCombo::InitCombo(const std::string& encoding)
{
	ResetContent();

	int index, nSel = -1;
	EncodingItem* item = _encodings;

	while (item->name != NULL)
	{
		index = AddString(item->alias);
		SetItemData(index, (DWORD_PTR)item->name);
		if (encoding == item->name) nSel = index;
		item++;
	}	
	SetCurSel(nSel);

}

std::string CEncodingCombo::GetEncoding()
{
	int selected = GetCurSel();
	if (selected >= 0)
	{
		return (char*)GetItemData(selected);
	}

	return "";
}

void CEncodingCombo::SelectEncoding(const std::string& encoding)
{
	for(int i = 0,s = GetCount(); i < s;i++)
	{
		if (encoding == (char*)GetItemData(i))
		{
			SetCurSel(i);
			break;
		}
	}
}


