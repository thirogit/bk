#ifndef __FORWARDCONTENTOBSERVER_H__
#define __FORWARDCONTENTOBSERVER_H__

#include "ContentObserver.h"

class ForwardContentObserver : public ContentObserver
{
public:
	virtual void OnBeforeUpdate() {};
	virtual void OnCountChanged() {};
	virtual void OnContentChanged() {};
	virtual void OnAfterUpdate() {};
};


#endif