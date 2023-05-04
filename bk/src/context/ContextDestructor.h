#ifndef __CONTEXTDESTRUCTOR_H__
#define __CONTEXTDESTRUCTOR_H__

#include "XContext.h"


class XContextDestructor
{
public:
	XContextDestructor(XContext* pContext);
	void Destroy();
private:
	XContext* m_pContext;
	
};

#endif