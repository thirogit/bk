#ifndef __BASECOMBO_H__
#define __BASECOMBO_H__

#include <string>
#include <cstdint>
#include <arrays\Enumerator.h>
#include <vector>
#include <afxwin.h>

class ComboItem
{
public:
	ComboItem(uint32_t itemId,const std::wstring& itemText);
	ComboItem(const ComboItem& src);
	void CopyFrom(const ComboItem& src);
	ComboItem& operator=(const ComboItem& right);
	
	uint32_t GetItemId() const;
	const std::wstring& GetItemText() const;

private:
	uint32_t m_itemId;
	std::wstring m_itemText;
};

class ComboContent
{
public:
	ComboContent();
	~ComboContent();
	ComboContent(const ComboContent& src);
	void CopyFrom(const ComboContent& src);

	void AddItem(const ComboItem& item);
	void AddItem(uint32_t itemId,const std::wstring& itemText);
	ComboContent& operator()(uint32_t itemId,const std::wstring& itemText);
	PtrEnumerator<ComboItem> EnumItems();		
	ComboContent& operator=(const ComboContent& right);
	void Destroy();
private:
	std::vector<ComboItem*> m_Items;	
};


class CBaseCombo : public CComboBox
{
public:

	void Select(uint32_t itemId);	
	void SetContent(ComboContent& content);	
	uint32_t GetSelectedItemId();	

protected:
	
};




#endif