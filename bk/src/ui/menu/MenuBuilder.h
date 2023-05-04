#ifndef __MENUBUILDER_H__
#define __MENUBUILDER_H__

#include <stdint.h>
#include <string>
#include <vector>
#include <functional>


class IdSequence
{
public:
	IdSequence(uint32_t start);	
	uint32_t next();
	uint32_t current() const;
private:
	uint32_t m_value;
};

class MenuBuilder
{
	enum ItemType
	{
		ItemType_None,
		ItemType_Text,
		ItemType_Separator,
		ItemType_Submenu		
	};
	
	class MenuItem
	{
	public:
		MenuItem();
		MenuItem(const MenuItem& src);
		~MenuItem();
		
	public:
		ItemType type;
		std::wstring text;
		std::function<void ()> handler;
		MenuBuilder* submenu;
	};

public:
	MenuBuilder();
	MenuBuilder(const MenuBuilder& src);
	MenuBuilder(MenuBuilder&& src);
	~MenuBuilder();

	template<class F>
	MenuBuilder& add(const std::wstring& sItemText,F& f)
	{
		std::function<void ()> handler = f;
		return add(sItemText,handler);
	}

	MenuBuilder& add(const std::wstring& sItemText,std::function<void ()>& handler);
	MenuBuilder& addSeparator();
	MenuBuilder& addSubmenu(const std::wstring& sItemText,const MenuBuilder& submenu);
	size_t itemCount() const;
	
	uint32_t trackAt(int x,int y,CWnd* pWnd);
	uint32_t track(CWnd* pWnd);
protected:
	HMENU createMenu(IdSequence& idSeq);
private:
	std::vector<MenuItem*> m_items;
};



#endif
