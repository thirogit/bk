#ifndef __USERDAO_H__
#define __USERDAO_H__

#include <arrays/NewPtrFlushArray.h>
#include <progress/SimpleProgress.h>
#include <progress/ComplexProgress.h>
#include <configuration/print/PrintProfile.h>
#include <configuration/passportrecognition/PassportRecognitionProfile.h>
#include <configuration/deductions/DeductionDefinition.h>
#include "../settings/SettingDefinition.h"
#include "../settings/SettingPart.h"
#include "../db/Transaction.h"
#include "BaseDAO.h"

class UserDAO : public BaseDAO
{
	class OrderSpecifierWithPosition : public OrderSpecifier
	{
	public:
		OrderSpecifierWithPosition(const std::wstring& sFieldCode,int position);
		int position() const;
	private:
		int m_position;
	};

	typedef std::multimap<uint32_t, PrintOption*> OptionsMap;
	typedef std::multimap<uint32_t, PassportField*> PassportFieldsMap;
	typedef std::multimap<uint32_t, OrderSpecifierWithPosition*> OrderSpecifiersMap;

public:
	bool LoadPrintProfiles(NewPtrFlushArray<PrintProfile>& profiles, uint32_t userId, ComplexProgress *progress = NULL);
	void UpdatePrintProfile(Transaction transaction,IPrintProfile* profile);
	bool DeletePrintProfile(Transaction transaction,uint32_t profileId);
	PrintProfile* InsertPrintProfile(Transaction transaction, uint32_t userId, IPrintProfile* profile);

	bool LoadSettingDefinitions(std::vector<SettingDefinition*>& result, SimpleProgress *progress = NULL);
	bool LoadUserSettings(NewPtrFlushArray<SettingPart>& result, uint32_t userId, SimpleProgress *progress = NULL);
	void InsertSettingPart(Transaction transaction, uint32_t userId, const SettingPart& part);
	bool UpdateSettingPart(Transaction transaction, uint32_t userId, const SettingPart& part);
	void DeleteSettingParts(Transaction transaction, uint32_t userId, const std::wstring& settingName);

	bool LoadPassRecgntnProfiles(NewPtrFlushArray<PassportRecognitionProfile>& profiles, uint32_t userId, ComplexProgress *progress = NULL);
	void UpdatePassRecgntnProfile(Transaction transaction, IPassportRecognitionProfile* profile);
	bool DeletePassRecgntnProfile(Transaction transaction, uint32_t profileId);
	PassportRecognitionProfile* InsertPassRecogntnProfile(Transaction transaction, uint32_t userId, IPassportRecognitionProfile* profile);

	bool LoadDeductionDefinitions(NewPtrFlushArray<DeductionDefinition>& profiles, uint32_t userId, SimpleProgress *progress = NULL);
	void UpdateDeductionDefinition(Transaction transaction, IDeductionDefinition* deductionDefinition);
	DeductionDefinition* InsertDeductionDefinition(Transaction transaction, uint32_t userId, IDeductionDefinition* deductionDefinition);
	bool DeleteDeductionDefinition(Transaction transaction, uint32_t profileId);

private:
	bool LoadFields(Transaction transaction, PassportFieldsMap& result, uint32_t userId, SimpleProgress* progress);
	bool DeleteFields(Transaction transaction, uint32_t profileId);
	void InsertField(Transaction transaction, uint32_t userId, uint32_t profileId, IPassportField* field);

	bool LoadOptions(Transaction transaction, OptionsMap& result, uint32_t userId, SimpleProgress* progress);
	bool DeleteOptions(Transaction transaction, uint32_t profileId);
	void InsertOption(Transaction transaction, uint32_t userId, uint32_t profileId, const PrintOption* option);
	
	bool LoadOrderSpecifiers(Transaction transaction, OrderSpecifiersMap& result, uint32_t userId, SimpleProgress* progress);
	bool DeleteOrderSpecifiers(Transaction transaction, uint32_t profileId);
	void InsertOrderSpecifier(Transaction transaction, uint32_t userId, uint32_t profileId, int position,const OrderSpecifier* specifier);
	

};

#endif
