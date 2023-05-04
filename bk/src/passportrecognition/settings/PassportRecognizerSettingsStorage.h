#if !defined(__PASSPORTRECOGNIZERSETTINGSSTORAGE_H__)
#define __PASSPORTRECOGNIZERSETTINGSSTORAGE_H__

#include "PassportRecognizerSettings.h"
#include <pugixml.hpp>
#include "../../localstorage/LocalStorage.h"

class PassportRecognizerSettingsStorage : LocalStorage
{
public:
	PassportRecognizerSettingsStorage();
	~PassportRecognizerSettingsStorage();
	void Save(PassportRecognizerSettings& settings);
	void Load(PassportRecognizerSettings& settings);
private:
	void SaveDataSettings(const DataSettings& dataSettings, pugi::xml_node datanode);
	void SaveEngineSettings(const EngineSettings& engineSettings, pugi::xml_node datanode);
	void SaveImageSettings(const ImageSettings& imageSettings, pugi::xml_node datanode);
	void SaveScannerSettings(const ScannerSettings& scanerSettings, pugi::xml_node datanode);

	void LoadDataSettings(const pugi::xml_node datanode,DataSettings& dataSettings);
	void LoadEngineSettings(const pugi::xml_node enginenode, EngineSettings& engineSettings);
	void LoadImageSettings(const pugi::xml_node imagenode, ImageSettings& imageSettings);
	void LoadScannerSettings(const pugi::xml_node scannernode, ScannerSettings& scanerSettings);
private:

};



#endif

