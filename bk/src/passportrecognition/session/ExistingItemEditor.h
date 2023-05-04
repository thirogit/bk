#ifndef __EXISTINGITEMEDITOR_H__
#define __EXISTINGITEMEDITOR_H__

#include "../../context/transaction/CowEditor.h"
#include "PassportRecognitionSession.h"
#include "../PassportImage.h"

class ExistingItemEditor : public CowEditor
{
public:
	ExistingItemEditor(PassportRecognitionSession* pSession, PassportImagePtr passportImage);
	void Edit(ItemId id);
	virtual bool IsEditing() const;
	virtual void Save();
private:
	PassportRecognitionSession* m_pSession;
	ItemId m_Id;
	PassportImagePtr m_passportImage;
};

#endif