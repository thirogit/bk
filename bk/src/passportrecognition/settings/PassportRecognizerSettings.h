#ifndef __PASSPORTRECOGNIZERSETTINGS_H__
#define __PASSPORTRECOGNIZERSETTINGS_H__

#include "DataSettings.h"
#include "EngineSettings.h"
#include "ImageSettings.h"
#include "ScannerSettings.h"

class PassportRecognizerSettings 
{
public:
	DataSettings& GetDataSettings();
	EngineSettings& GetEngineSettings();
	ImageSettings& GetImageSettings();
	ScannerSettings& GetScannerSettings();
protected:
	DataSettings m_DataSettings;
	EngineSettings m_EngineSettings;
	ImageSettings m_ImageSettings;
	ScannerSettings m_ScannerSettings;
};


#endif