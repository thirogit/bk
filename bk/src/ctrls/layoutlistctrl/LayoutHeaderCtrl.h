#ifndef __LAYOUTHEADERCTRL_H__
#define __LAYOUTHEADERCTRL_H__

class CLayoutHeaderCtrl : public CHeaderCtrl
{
public:
     CLayoutHeaderCtrl();
     virtual ~CLayoutHeaderCtrl();
protected:
     afx_msg LRESULT OnHeaderLayout(WPARAM wp, LPARAM lp);
	 afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
     DECLARE_MESSAGE_MAP()
};


#endif