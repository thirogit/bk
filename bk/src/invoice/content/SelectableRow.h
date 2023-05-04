#ifndef __SELECTABLEROW_H__
#define __SELECTABLEROW_H__

#include "../../uiconstants/ColorConstants.h"
#include "../../uitypes/Color.h"
#include <stdint.h>

template<class base_row>
class SelectableRow : public base_row
{
public:
	SelectableRow(typename base_row::content_type* pContent) : base_row(pContent),m_selected(false)
	{
	}

	bool IsSelected() const
	{
		return m_selected;
	}

	void SetSelected(bool selected)
	{
		m_selected = selected;
	}

	virtual Color CellColor(uint32_t colId)
	{
		if(IsSelected())
			return Color(ROW_SELECTED_CLR);

		return base_row::CellColor(colId);	
	}

private:
	bool m_selected;
};


#endif