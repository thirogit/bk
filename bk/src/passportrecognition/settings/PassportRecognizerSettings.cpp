#include "stdafx.h"
#include "PassportRecognizerSettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


DataSettings& PassportRecognizerSettings::GetDataSettings()
{
	return m_DataSettings;
}

EngineSettings& PassportRecognizerSettings::GetEngineSettings()
{
	return m_EngineSettings;
}

ImageSettings& PassportRecognizerSettings::GetImageSettings()
{
	return m_ImageSettings;
}

ScannerSettings& PassportRecognizerSettings::GetScannerSettings()
{
	return m_ScannerSettings;
}
