#include "stdafx.h"
#include "DeductionDefinitionViewCreator.h"
#include <string/TextUtils.h>



DeductionDefinitionView* DeductionDefinitionViewCreator::CreateView(ItemId id,const DeductionDefinition* definition)
{
	DeductionDefinitionView* pView = new DeductionDefinitionView(id);
	RefreshView(definition, pView);
	return pView;
}

void DeductionDefinitionViewCreator::RefreshView(const DeductionDefinition* definition, DeductionDefinitionView* pView)
{
	
	pView->SetCode(definition->GetCode());
	pView->SetFraction(definition->GetFraction());
	pView->SetIsEnabled(definition->GetIsEnabledByDefault());
	pView->SetReason(TextUtils::JoinLines(definition->GetReason(), L';'));

}

