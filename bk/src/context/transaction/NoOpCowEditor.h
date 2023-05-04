#ifndef __NOOPCOWEDITOR_H__
#define __NOOPCOWEDITOR_H__

#include "CowEditor.h"

class NoOpCowEditor : public CowEditor
{
public:
	virtual bool IsEditing() const;
	virtual void Save();
};

#endif