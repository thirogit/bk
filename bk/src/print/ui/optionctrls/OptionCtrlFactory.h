#ifndef __DOCUMENTOPTIONCTRLFACTORY_H__
#define __DOCUMENTOPTIONCTRLFACTORY_H__

#include "DocumentOptionCtrl.h"
#include "../../options/DocumentOptionDefinition.h"

class DocumentOptionCtrlFactory
{
public:
	DocumentOptionCtrlFactory(CWnd* pParent);

	DocumentOptionCtrl* CreateCtrl(const DocumentOptionDefinition& definition, UINT nId);
private:
	CWnd* m_pParent;
};


#endif

