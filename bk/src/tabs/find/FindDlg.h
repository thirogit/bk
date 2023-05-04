#ifndef __FINDDLG_H__
#define __FINDDLG_H__


#include "../../SessionAwareDialog.h"

#define FN_FINDREQUESTED (WM_APP+10000)


struct NMFINDRQ : public NMHDR
{
	void* predicte;
};

class FindContainerDlg;

class FindDlg : public CSessionAwareDialog
{
public:
	virtual void Create(FindContainerDlg* container) = 0;

};

#endif