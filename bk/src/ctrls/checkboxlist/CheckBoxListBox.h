#ifndef __CHECKBOXLISTBOX_H__
#define __CHECKBOXLISTBOX_H__

#include "CheckBoxItem.h"
#include <afxwin.h>
#include <cstdint>
#include <string>

class CCheckBoxListBox : public CCheckListBox
{
public:
	CCheckBoxListBox();
	virtual ~CCheckBoxListBox();
	void AddOption(uint32_t optionId,const std::wstring& sOptDesc,bool bValue);

	void MoveItemDown(int nItem);
	void MoveItemUp(int nItem);
	void MoveItem(int nSrcItemPos,int nDstItemPos);

	void UpdateOptions();

protected:
	
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
};

#endif 
