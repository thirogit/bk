#include "stdafx.h"
#include "ScannerSettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


ScannerSettings::ScannerSettings()
{
	m_Brightness = DEFAULT_BRIGHTNESS;
	m_Contrast = DEFAULT_CONTRAST;
	m_Resolution = DEFAULT_RESOLUTION;
}

int ScannerSettings::GetBrightness() const
{
	return m_Brightness;
}

int ScannerSettings::GetResolution() const
{
	return m_Resolution;
}

int ScannerSettings::GetContrast() const
{
	return m_Contrast;
}

void ScannerSettings::SetBrightness(int brightness)
{
	m_Brightness = brightness;
}

void ScannerSettings::SetResolution(int resolution)
{
	m_Resolution = resolution;
}

void ScannerSettings::SetContrast(int contrast)
{
	m_Contrast = contrast;
}
	

