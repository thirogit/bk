#include "stdafx.h"
#include "AppServices.h"
#include "App.h"

namespace AppServices
{
	BarcodeScannerService* GetBCScannerService()
	{
		return ((CBKApp*)AfxGetApp())->GetServiceRegistry()->FindByType((BarcodeScannerService*)0);
	}

	DocumentManager* GetDocumentManager()
	{
		return ((CBKApp*)AfxGetApp())->GetServiceRegistry()->FindByType((DocumentManagerService*)0);
	}

	LogoService* GetLogoService()
	{
		return ((CBKApp*)AfxGetApp())->GetServiceRegistry()->FindByType((LogoService*)0);
	}
}