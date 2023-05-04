#pragma once

#include "barcodescanner\BarcodeScannerService.h"
#include "print\DocumentManagerService.h"
#include "logo\LogoService.h"

namespace AppServices
{
	BarcodeScannerService* GetBCScannerService();
	DocumentManager* GetDocumentManager();
	LogoService* GetLogoService();
}



