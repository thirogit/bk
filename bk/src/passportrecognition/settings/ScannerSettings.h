#ifndef __SCANNERSETTINGS_H__
#define __SCANNERSETTINGS_H__

#define DEFAULT_BRIGHTNESS 0
#define DEFAULT_CONTRAST 0
#define DEFAULT_RESOLUTION 300

class ScannerSettings 
{
public:
	ScannerSettings();
	
	int GetBrightness() const;
	int GetResolution() const;
	int GetContrast() const;
	
	void SetBrightness(int brightness);
	void SetResolution(int resolution);
	void SetContrast(int contrast);
		
private:
	int m_Brightness;
	int m_Resolution;
	int m_Contrast;
};


#endif