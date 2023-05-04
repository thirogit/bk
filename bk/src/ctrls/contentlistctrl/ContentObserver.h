#ifndef __CONTENTOBSERVER_H__
#define __CONTENTOBSERVER_H__

class ContentObserver
{
public:
	virtual void OnBeforeUpdate() = 0;
	virtual void OnCountChanged() = 0;
	virtual void OnContentChanged() = 0;
	virtual void OnAfterUpdate() = 0;
};


#endif