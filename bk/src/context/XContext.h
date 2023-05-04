#ifndef __XCONTEXT_H__
#define __XCONTEXT_H__

#include <service\IBKService.h>
#include <arrays/NewPtrFlushArray.h>
#include <data\impl\Farm.h>
#include <data\impl\Season.h>
#include <data\impl\Herd.h>
#include "SeasonSessionImpl.h"
#include <configuration\print\PrintProfiles.h>
#include <configuration\passportrecognition\PassportRecognitionProfiles.h>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <configuration/deductions/DeductionDefinitions.h>
#include <boost/type_traits.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/throw_exception.hpp>
#include <data\runtime\SimpleUser.h>

class XContextInitialiser;
class XContextDestructor;

typedef RcfClient<IBKService> BKServiceClient;

class XContext 
{	
	struct index_tag_id{};
	
	typedef boost::multi_index_container
		<
		PrintProfile*,
		boost::multi_index::indexed_by<
		boost::multi_index::ordered_unique < boost::multi_index::tag<index_tag_id>, boost::multi_index::const_mem_fun<PrintProfile, uint32_t, &PrintProfile::GetProfileId> >
		>
		> PrintProfileIndex;


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


	typedef boost::multi_index_container
		<
		Season*,
		boost::multi_index::indexed_by<
		boost::multi_index::ordered_unique < boost::multi_index::tag<index_tag_id>, boost::multi_index::const_mem_fun<Season, uint32_t, &Season::GetId> >
		>
		> SeasonIndex;

	typedef boost::multi_index_container
		<
		Herd*,
		boost::multi_index::indexed_by<
		boost::multi_index::ordered_unique < boost::multi_index::tag<index_tag_id>, boost::multi_index::const_mem_fun<Herd, uint32_t, &Herd::GetId> >
		>
		> HerdsIndex;

	typedef boost::multi_index_container
		<
		Agent*,
		boost::multi_index::indexed_by<
		boost::multi_index::ordered_unique < boost::multi_index::tag<index_tag_id>, boost::multi_index::const_mem_fun<Agent, uint32_t, &Agent::GetId> >
		>
		> AgentsIndex;

	typedef boost::multi_index_container
		<
		BankAccount*,
		boost::multi_index::indexed_by<
		boost::multi_index::ordered_unique < boost::multi_index::tag<index_tag_id>, boost::multi_index::const_mem_fun<BankAccount, uint32_t, &BankAccount::GetId> >
		>
		> BankAccountsIndex;

	

public:	
	XContext();		
	PtrEnumerator<ISeason> EnumSeasons() const;
	PtrEnumerator<IHerd> EnumHerds() const;
	PtrEnumerator<IAgent> EnumAgents();
	PtrEnumerator<IBankAccount> EnumBankAccounts();
	size_t GetHerdCount() const;
	IHerd* GetHerd(uint32_t herdId);
	IHerd* GetHerdByIndex(int herdIndex);
	ISeason* GetSeason(uint32_t seasonId);
	IAgent* GetAgent(uint32_t agentId);
	SeasonSession* OpenSeason(uint32_t seasonId);	
	IFarm* GetFarm() const;
	IBankAccount* GetBankAccount(uint32_t accountId);
	IClass* GetClass(uint32_t classId);	
	IClass* GetClassByCd(const std::wstring& classCd);
	IStock* GetStock(uint32_t stockId);
	PtrEnumerator<IStock> EnumStocks();
	PtrEnumerator<IClass> EnumClasses();
	ISimpleUser* GetUser() const;

	PrintProfiles GetPrintProfiles(const std::wstring& sDocumentCode);
	IPrintProfile* GetPrintProfile(uint32_t profileId);
	uint32_t SaveNewPrintProfile(const IPrintProfile* pProfile);
	void UpdatePrintProfile(const IPrintProfile* pProfile);
	void DeletePrintProfile(uint32_t profileId);

	PassportRecognitionProfiles GetPassportRecognitionProfiles();
	IPassportRecognitionProfile* GetPassportRecognitionProfile(uint32_t profileId);
	uint32_t SaveNewPassportRecognitionProfile(const IPassportRecognitionProfile* pProfile);
	void UpdatePassportRecognitionProfile(const IPassportRecognitionProfile* pProfile);
	void DeletePassportRecognitionProfile(uint32_t profileId);
	
	DeductionDefinitions GetDeductionDefinitions();
	IDeductionDefinition* GetDeductionDefinition(uint32_t definitionId);
	uint32_t SaveNewDeductionDefinition(const IDeductionDefinition* definition);
	void UpdateDeductionDefinition(const IDeductionDefinition* definition);
	void DeleteDeductionDefinition(uint32_t definitionId);
		
	UserSettings* GetUserSettings();

protected:
	void CloseSession(SeasonSessionImpl* pSession);
	
	template<typename A1>
	void InvokeClient(RCF::FutureImpl<RCF::Void> (BKServiceClient::*fn)( RCF::RemoveOut<uint32_t>::type,typename RCF::RemoveOut<A1>::type),uint32_t sessionToken,A1 a1)
	{
		try
		{
			(m_pClient->*fn)(sessionToken,a1);
		}
		catch(RCF::Exception& e)
		{
			BOOST_THROW_EXCEPTION(ContextException(TextUtils::FromUTF8(e.getWhat())));
		}
	}

	template<typename A1>
	void InvokeClient(RCF::FutureImpl<RCF::Void>(BKServiceClient::*fn)(typename RCF::RemoveOut<A1>::type), A1 a1)
	{
		try
		{
			(m_pClient->*fn)(a1);
		}
		catch (RCF::Exception& e)
		{
			BOOST_THROW_EXCEPTION(ContextException(TextUtils::FromUTF8(e.getWhat())));
		}
	}


	template<typename A1, typename R = RCF::Void>
	R InvokeClientR(RCF::FutureImpl<R>(BKServiceClient::*fn)(typename RCF::RemoveOut<A1>::type), A1 a1, typename boost::disable_if_c<boost::is_same<uint32_t,A1>::value,A1>::type* dummy = 0)
	{
		try
		{
			return (m_pClient->*fn)(a1);
		}
		catch (RCF::Exception& e)
		{
			BOOST_THROW_EXCEPTION(ContextException(TextUtils::FromUTF8(e.getWhat())));
		}
	}


	template<typename R = RCF::Void>
	R InvokeClientR(RCF::FutureImpl<R>(BKServiceClient::*fn)(RCF::RemoveOut<uint32_t>::type), uint32_t sessionToken)
	{
		try
		{
			return (m_pClient->*fn)(sessionToken);
		}
		catch (RCF::Exception& e)
		{
			BOOST_THROW_EXCEPTION(ContextException(TextUtils::FromUTF8(e.getWhat())));
		}
	}



	template<typename A1, typename R = RCF::Void>
	R InvokeClientR(RCF::FutureImpl<R>(BKServiceClient::*fn)(RCF::RemoveOut<uint32_t>::type, typename RCF::RemoveOut<A1>::type), uint32_t sessionToken, A1 a1)
	{
		try
		{
			return (m_pClient->*fn)(sessionToken,a1);
		}
		catch (RCF::Exception& e)
		{
			BOOST_THROW_EXCEPTION(ContextException(TextUtils::FromUTF8(e.getWhat())));
		}
	}

	template<typename A1, typename A2, typename R = RCF::Void>
	R InvokeClientR(RCF::FutureImpl<R>(BKServiceClient::*fn)(RCF::RemoveOut<uint32_t>::type, typename RCF::RemoveOut<A1>::type, typename RCF::RemoveOut<A2>::type), uint32_t sessionToken, A1 a1, A2 a2)
	{
		try
		{
			return (m_pClient->*fn)(sessionToken, a1, a2);
		}
		catch (RCF::Exception& e)
		{
			BOOST_THROW_EXCEPTION(ContextException(TextUtils::FromUTF8(e.getWhat())));
		}
	}

	boost::any GetSetting(const std::wstring& settingName);
	void SetSetting(const std::wstring& settingName, const boost::any& settingValue);
	void SaveSettings();


	Farm* m_pFarm;
	SeasonIndex m_Seasons;
	HerdsIndex	m_Herds;	
	AgentsIndex		m_Agents;
	BankAccountsIndex m_Accounts;
	ClassesIndex m_Classes;
	StocksIndex m_Stocks;
	SimpleUser* m_User;

	BKServiceClient* m_pClient;
	RCF::TcpEndpoint m_ServerEndpoint;
	std::map<uint32_t,SeasonSessionImpl*> m_Sessions;
	PrintProfileIndex m_printProfiles;
	PassportRecognitionProfilesIndex m_passRecgntnProfiles;
	DeductionDefinitionsIndex m_deductionDefinitions;
	std::unordered_map<std::wstring, Setting*> m_settings;
	UserSettingsImpl* m_pSettingsFacade;
	
	friend class XContextInitialiser;
	friend class XContextDestructor;
	friend class SeasonSessionImpl;
	friend class UserSettingsImpl;
};

#endif