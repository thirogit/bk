#include "stdafx.h"
#include "PassportRecognizerSettingsStorage.h"
#include <filesystem\paths.h>
#include <boost/foreach.hpp>
#include <boost/range/adaptors.hpp>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define SETTINGS_FILE L"recognizer.xml"


const std::map<DataSettings::DATA_FIELD, std::wstring> _field_names = { 
	{ DataSettings::PASSPORT_DATE, L"passportdate" },
	{ DataSettings::BIRTH, L"birth" },
	{ DataSettings::MOTHER_NO, L"motherno" },
	{ DataSettings::FIRST_OWNER, L"firstowner" },
	{ DataSettings::PASSPORT_NO, L"passportno" },
	{ DataSettings::SEX, L"sex" },
	{ DataSettings::STOCK, L"stock" }
																	};

PassportRecognizerSettingsStorage::PassportRecognizerSettingsStorage()
{	
}

PassportRecognizerSettingsStorage::~PassportRecognizerSettingsStorage()
{
}

void PassportRecognizerSettingsStorage::Save(PassportRecognizerSettings& settings)
{

	pugi::xml_document doc;

	pugi::xml_node xmlrecognizer = doc.append_child(L"recognizer");
	
	
	pugi::xml_node xmldata = xmlrecognizer.append_child(L"data");
	SaveDataSettings(settings.GetDataSettings(), xmldata);

	pugi::xml_node xmlengine = xmlrecognizer.append_child(L"engine");
	SaveEngineSettings(settings.GetEngineSettings(), xmlengine);

	pugi::xml_node xmlimage = xmlrecognizer.append_child(L"image");
	SaveImageSettings(settings.GetImageSettings(), xmlimage);

	pugi::xml_node xmlscanner = xmlrecognizer.append_child(L"scanner");
	SaveScannerSettings(settings.GetScannerSettings(), xmlscanner);
	
	std::wstring storageFile = GetStorageFile(SETTINGS_FILE);
	doc.save_file(storageFile.c_str());

}

void PassportRecognizerSettingsStorage::Load(PassportRecognizerSettings& settings)
{

	pugi::xml_document doc;

	std::wstring storageFile = GetStorageFile(SETTINGS_FILE);

	if (paths_DoesPathExist(storageFile))
	{
		pugi::xml_parse_result result = doc.load_file(storageFile.c_str());

		pugi::xml_node xmlrecognizer = doc.child(L"recognizer");

		if (xmlrecognizer)
		{
			pugi::xml_node xmldata = xmlrecognizer.child(L"data");
			if (xmldata)
			{
				LoadDataSettings(xmldata,settings.GetDataSettings());
			}

			pugi::xml_node xmlengine = xmlrecognizer.child(L"engine");
			if (xmlengine)
			{
				LoadEngineSettings(xmlengine,settings.GetEngineSettings());
			}

			pugi::xml_node xmlimage = xmlrecognizer.child(L"image");
			if (xmlimage)
			{
				LoadImageSettings(xmlimage,settings.GetImageSettings());
			}

			pugi::xml_node xmlscanner = xmlrecognizer.child(L"scanner");
			if (xmlscanner)
			{
				LoadScannerSettings(xmlscanner,settings.GetScannerSettings());
			}
		}
	}
	
}

void PassportRecognizerSettingsStorage::SaveDataSettings(const DataSettings& dataSettings, pugi::xml_node datanode)
{
	datanode.append_child(L"uppercasehent").text().set(dataSettings.GetDoUppercaseHent());
	datanode.append_child(L"noupdateifpresent").text().set(dataSettings.GetDoNotUpdateValueIfPresent());
	
	pugi::xml_node fieldsnode = datanode.append_child(L"fields");
	pugi::xml_node fieldnode;
	auto it = _field_names.begin();
	while (it != _field_names.end())
	{
		fieldnode = fieldsnode.append_child(it->second.c_str());
		fieldnode.text().set(dataSettings.IsFieldEnabled(it->first));
		it++;
	}	
}

void PassportRecognizerSettingsStorage::SaveEngineSettings(const EngineSettings& engineSettings, pugi::xml_node enginenode)
{
	enginenode.append_child(L"enginepath").text().set(engineSettings.GetEnginePath().c_str());
	enginenode.append_child(L"licencefilepath").text().set(engineSettings.GetLicenseFilePath().c_str());
	enginenode.append_child(L"developersn").text().set(engineSettings.GetDeveloperSN().c_str());
	enginenode.append_child(L"projectid").text().set(engineSettings.GetProjectId().c_str());	
}

void PassportRecognizerSettingsStorage::SaveImageSettings(const ImageSettings& imageSettings, pugi::xml_node imagenode)
{

	imagenode.append_child(L"blacklevel").text().set(imageSettings.GetBlackLevel());
	imagenode.append_child(L"palettecenter").text().set(imageSettings.GetPaletteCenter());
	imagenode.append_child(L"palettesigma").text().set(imageSettings.GetPaletteSigma());
	imagenode.append_child(L"paletteenhance").text().set(imageSettings.GetDoPaletteEnhance());
}

void PassportRecognizerSettingsStorage::SaveScannerSettings(const ScannerSettings& scanerSettings, pugi::xml_node scannernode)
{
	scannernode.append_child(L"brightness").text().set(scanerSettings.GetBrightness());
	scannernode.append_child(L"contrast").text().set(scanerSettings.GetContrast());
	scannernode.append_child(L"resolution").text().set(scanerSettings.GetResolution());
}


void PassportRecognizerSettingsStorage::LoadDataSettings(const pugi::xml_node datanode, DataSettings& dataSettings)
{	
	dataSettings.SetDoUppercaseHent(datanode.child(L"uppercasehent").text().as_bool());
	dataSettings.SetDoNotUpdateValueIfPresent(datanode.child(L"noupdateifpresent").text().as_bool());

	const pugi::xml_node fieldsnode = datanode.child(L"fields");
	if (fieldsnode)
	{

		pugi::xml_node fieldnode;
		auto it = _field_names.begin();
		while (it != _field_names.end())
		{
			fieldnode = fieldsnode.child(it->second.c_str());
			if (fieldnode)
			{
				dataSettings.SetOptionalFieldEnabled(it->first, fieldnode.text().as_bool());
			}
			it++;
		}
	}
}

void PassportRecognizerSettingsStorage::LoadEngineSettings(const pugi::xml_node enginenode, EngineSettings& engineSettings)
{
	engineSettings.SetEnginePath(enginenode.child(L"enginepath").text().as_string());
	engineSettings.SetLicenseFilePath(enginenode.child(L"licencefilepath").text().as_string());
	engineSettings.SetDeveloperSN(enginenode.child(L"developersn").text().as_string());
	engineSettings.SetProjectId(enginenode.child(L"projectid").text().as_string());
}

void PassportRecognizerSettingsStorage::LoadImageSettings(const pugi::xml_node imagenode, ImageSettings& imageSettings)
{
	imageSettings.SetBlackLevel(imagenode.child(L"blacklevel").text().as_int());
	imageSettings.SetPaletteCenter(imagenode.child(L"palettecenter").text().as_int());
	imageSettings.SetPaletteSigma(imagenode.child(L"palettesigma").text().as_int());
	imageSettings.SetDoPaletteEnhance(imagenode.child(L"paletteenhance").text().as_bool());
}

void PassportRecognizerSettingsStorage::LoadScannerSettings(const pugi::xml_node scannernode, ScannerSettings& scanerSettings)
{
	scanerSettings.SetBrightness(scannernode.child(L"brightness").text().as_int());
	scanerSettings.SetContrast(scannernode.child(L"contrast").text().as_int());
	scanerSettings.SetResolution(scannernode.child(L"resolution").text().as_int());
}
