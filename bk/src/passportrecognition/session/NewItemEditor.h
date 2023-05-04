#ifndef __NEWITEMEDITOR_H__
#define __NEWITEMEDITOR_H__

#include "../../context/transaction/CowEditor.h"
#include "PassportRecognitionSession.h"
#include "../PassportImage.h"

class NewItemEditor : public CowEditor
{
public:
	NewItemEditor(PassportRecognitionSession* pSession, PassportImagePtr passportImage);
	virtual bool IsEditing() const;
	virtual void Save();
private:
	PassportRecognitionSession* m_pSession;	
	PassportImagePtr m_passportImage;
};

#endif