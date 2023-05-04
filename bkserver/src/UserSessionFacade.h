#pragma once

#include <functional>
#include <stdint.h>
#include <configuration/print/IPrintProfile.h>
#include <configuration/passportrecognition/IPassportRecognitionProfile.h>
#include <configuration/deductions/IDeductionDefinition.h>
#include <configuration/ISetting.h>
#include "settings/UserSettings.h"


class UserSessionFacade
{
public:
	virtual PtrEnumerator<IPrintProfile> EnumPrintProfiles() const = 0;
	virtual void UpdatePrintProfile(IPrintProfile* profile) = 0;
	virtual uint32_t NewPrintProfile(IPrintProfile* profile) = 0;
	virtual void DeletePrintProfile(uint32_t profileId) = 0;
	virtual PtrEnumerator<ISetting> EnumSettings() = 0;
	virtual void SaveSettings(const UserSettings& settings) = 0;

	virtual PtrEnumerator<IPassportRecognitionProfile> EnumPassportRecognitionProfiles() const = 0;
	virtual void UpdatePassportRecognitionProfile(IPassportRecognitionProfile* profile) = 0;
	virtual uint32_t NewPassportRecognitionProfile(IPassportRecognitionProfile* profile) = 0;
	virtual void DeletePassportRecognitionProfile(uint32_t profileId) = 0;

	virtual PtrEnumerator<IDeductionDefinition> EnumDeductionDefinitions() const = 0;
	virtual void UpdateDeductionDefinition(IDeductionDefinition* definition) = 0;
	virtual uint32_t NewDeductionDefinition(IDeductionDefinition* definition) = 0;
	virtual void DeleteDeductionDefinition(uint32_t deductionId) = 0;
};

