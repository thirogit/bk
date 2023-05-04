#ifndef __CLASSESCOMBOCONTENT_H__
#define __CLASSESCOMBOCONTENT_H__

#include "../ctrls/BaseCombo.h"
#include <data\IClass.h>
#include "../context/SeasonSession.h"
#include "../context/XContext.h"

class ClassesComboContent : public ComboContent
{
public:
	void CreateContent(SeasonSession* facade);
	void CreateContent(XContext* context);
	void DestroyContent();
private:
	void CreateContent(PtrEnumerator<IClass>& classesEnum);
};

#endif