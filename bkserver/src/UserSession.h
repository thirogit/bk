#pragma once

#include <functional>
#include <stdint.h>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <configuration/print/PrintProfiles.h>
#include <configuration/passportrecognition/PassportRecognitionProfile.h>
#include "UserSessionFacade.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/asio/io_service.hpp>
#include "settings/UserSettingsManager.h"
#include <configuration/deductions/DeductionDefinition.h>

class UserSessionBuilder;

class UserSession
{	

	struct index_tag_id{};

	typedef boost::multi_index_container
		<
		PrintProfile*,
		boost::multi_index::indexed_by<
		boost::multi_index::ordered_unique < boost::multi_index::tag<index_tag_id>, boost::multi_index::const_mem_fun<PrintProfile, uint32_t, &PrintProfile::GetProfileId> >
		>
		> ProfilesIndex;


	typedef boost::multi_index_container
		<
		PassportRecognitionProfile*,
		boost::multi_index::indexed_by<
		boost::multi_index::ordered_unique < boost::multi_index::tag<index_tag_id>, boost::multi_index::const_mem_fun<PassportRecognitionProfile, uint32_t, &PassportRecognitionProfile::GetId> >
		>
		> PassportRecognitionProfilesIndex;

	typedef boost::multi_index_container
		<
		DeductionDefinition*,
		boost::multi_index::indexed_by<
		boost::multi_index::ordered_unique < boost::multi_index::tag<index_tag_id>, boost::multi_index::const_mem_fun<DeductionDefinition, uint32_t, &DeductionDefinition::GetId> >
		>
		> DeductionDefinitionsIndex;



	class UserSessionFacadeImpl : public UserSessionFacade
	{
	public:
		UserSessionFacadeImpl(UserSession* pDelegate);

		virtual PtrEnumerator<IPrintProfile> EnumPrintProfiles() const;
		virtual void UpdatePrintProfile(IPrintProfile* profile);
		virtual uint32_t NewPrintProfile(IPrintProfile* profile);
		virtual void DeletePrintProfile(uint32_t profileId);

		virtual PtrEnumerator<ISetting> EnumSettings();
		virtual void SaveSettings(const UserSettings& settings);

		virtual PtrEnumerator<IPassportRecognitionProfile> EnumPassportRecognitionProfiles() const;
		virtual void UpdatePassportRecognitionProfile(IPassportRecognitionProfile* profile);
		virtual uint32_t NewPassportRecognitionProfile(IPassportRecognitionProfile* profile);
		virtual void DeletePassportRecognitionProfile(uint32_t profileId);

		virtual PtrEnumerator<IDeductionDefinition> EnumDeductionDefinitions() const;
		virtual void UpdateDeductionDefinition(IDeductionDefinition* definition);
		virtual uint32_t NewDeductionDefinition(IDeductionDefinition* definition);
		virtual void DeleteDeductionDefinition(uint32_t deductionId);

	protected:
		void ThrowPrintProfileNotFound(uint32_t profileId);
		void ThrowPassRecogntnProfileNotFound(uint32_t profileId);
		void ThrowDeductionDefinitionNotFound(uint32_t deductionId);
	private:
		UserSession* m_pDelegate;
	};

	UserSession(uint32_t userId);
public:
	void Do(std::function<void (UserSessionFacade*)>& job);	
	void Begin();
	void End();
private:	
	uint32_t m_userId;
	ProfilesIndex m_printProfiles;
	PassportRecognitionProfilesIndex m_passRecgntnProfiles;
	DeductionDefinitionsIndex m_deductionDefinitions;
	UserSettingsManager m_settingsManager;	
	
	boost::thread m_worker;
	boost::asio::io_service m_service;
	boost::asio::io_service::work* m_work;

	friend class UserSessionBuilder;
	friend class UserSessionFacadeImpl;
};
