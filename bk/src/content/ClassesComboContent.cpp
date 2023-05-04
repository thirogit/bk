#include "stdafx.h"
#include "ClassesComboContent.h"


void ClassesComboContent::CreateContent(SeasonSession* facade)
{
	PtrEnumerator<IClass> classEnum = facade->EnumClasses();
	CreateContent(classEnum);
}

void ClassesComboContent::DestroyContent()
{
	ComboContent::Destroy();
}

void ClassesComboContent::CreateContent(XContext* context)
{
	PtrEnumerator<IClass> classesEnum = context->EnumClasses();
	CreateContent(classesEnum);
}

void ClassesComboContent::CreateContent(PtrEnumerator<IClass>& classesEnum)
{
	IClass* pClass = NULL;
	while (classesEnum.hasNext())
	{
		pClass = *classesEnum;
		AddItem(pClass->GetId(), pClass->GetClassCode());
		classesEnum.advance();
	}
}
