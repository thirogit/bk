#include "stdafx.h"
#include "UserSession.h"
#include <data/IIdable.h>
#include  <future>
#include <boost/foreach.hpp>
#include "dao/UserDAO.h"
#include "SessionException.h"
#include <errortoken/ErrorToken.h>


UserSession::UserSessionFacadeImpl::UserSessionFacadeImpl(UserSession* pDelegate) : m_pDelegate(pDelegate)
{
}

PtrEnumerator<IPrintProfile> UserSession::UserSessionFacadeImpl::EnumPrintProfiles() const
{
	return PtrEnumerator<IPrintProfile>(m_pDelegate->m_printProfiles);	
}

void UserSession::UserSessionFacadeImpl::UpdatePrintProfile(IPrintProfile* profile)
{
	const boost::multi_index::index<ProfilesIndex, index_tag_id>::type& indexById = boost::multi_index::get<index_tag_id>(m_pDelegate->m_printProfiles);
		
	auto profileIt = indexById.find(profile->GetProfileId());
	if (profileIt == indexById.end())
		ThrowPrintProfileNotFound(profile->GetProfileId());

	UserDAO dao;
	Transaction transaction = dao.OpenTransaction();
	dao.UpdatePrintProfile(transaction, profile);
	PrintProfile* profileToUpdate = (*profileIt);
	
	profileToUpdate->SetProfileName(profile->GetProfileName());
	profileToUpdate->CopyOptions(profile);
	profileToUpdate->CopyOrderSpecifiers(profile);

	transaction->Commit();	
}

uint32_t UserSession::UserSessionFacadeImpl::NewPrintProfile(IPrintProfile* profile)
{
	UserDAO dao;
	Transaction transaction = dao.OpenTransaction();
	PrintProfile* newProfile = dao.InsertPrintProfile(transaction, m_pDelegate->m_userId, profile);
	m_pDelegate->m_printProfiles.insert(newProfile);
	transaction->Commit();
	return newProfile->GetProfileId();
}

void UserSession::UserSessionFacadeImpl::ThrowPrintProfileNotFound(uint32_t profileId)
{
	BOOST_THROW_EXCEPTION(SessionException(ErrorToken(L"print_profile_not_found")(L"profile_id", profileId).str()));
}

void UserSession::UserSessionFacadeImpl::ThrowPassRecogntnProfileNotFound(uint32_t profileId)
{
	BOOST_THROW_EXCEPTION(SessionException(ErrorToken(L"passport_recognition_profile_not_found")(L"profile_id", profileId).str()));
}

void UserSession::UserSessionFacadeImpl::ThrowDeductionDefinitionNotFound(uint32_t deductionId)
{
	BOOST_THROW_EXCEPTION(SessionException(ErrorToken(L"deduction_definition_not_found")(L"deduction_id", deductionId).str()));
}

void UserSession::UserSessionFacadeImpl::DeletePrintProfile(uint32_t profileId)
{
	boost::multi_index::index<ProfilesIndex, index_tag_id>::type& indexById = boost::multi_index::get<index_tag_id>(m_pDelegate->m_printProfiles);

	auto profileIt = indexById.find(profileId);
	if (profileIt == indexById.end())
		ThrowPrintProfileNotFound(profileId);

	UserDAO dao;
	Transaction transaction = dao.OpenTransaction();
	dao.DeletePrintProfile(transaction, profileId);

	indexById.erase(profileIt);
	
	transaction->Commit();
}


PtrEnumerator<ISetting> UserSession::UserSessionFacadeImpl::EnumSettings()
{
	return m_pDelegate->m_settingsManager.EnumSettings();
}

void UserSession::UserSessionFacadeImpl::SaveSettings(const UserSettings& settings)
{
	m_pDelegate->m_settingsManager.MergeWith(settings);
	m_pDelegate->m_settingsManager.Save();	
}

PtrEnumerator<IPassportRecognitionProfile> UserSession::UserSessionFacadeImpl::EnumPassportRecognitionProfiles() const
{
	return PtrEnumerator<IPassportRecognitionProfile>(m_pDelegate->m_passRecgntnProfiles);
}

void UserSession::UserSessionFacadeImpl::UpdatePassportRecognitionProfile(IPassportRecognitionProfile* profile)
{
	const boost::multi_index::index<PassportRecognitionProfilesIndex, index_tag_id>::type& indexById = 
		boost::multi_index::get<index_tag_id>(m_pDelegate->m_passRecgntnProfiles);

	auto profileIt = indexById.find(profile->GetId());
	if (profileIt == indexById.end())
		ThrowPassRecogntnProfileNotFound(profile->GetId());

	UserDAO dao;
	Transaction transaction = dao.OpenTransaction();
	dao.UpdatePassRecgntnProfile(transaction, profile);
	PassportRecognitionProfile* profileToUpdate = (*profileIt);

	profileToUpdate->SetProfileName(profile->GetProfileName());
	profileToUpdate->CopyFrom(profile);

	transaction->Commit();
}

uint32_t UserSession::UserSessionFacadeImpl::NewPassportRecognitionProfile(IPassportRecognitionProfile* profile)
{
	UserDAO dao;
	Transaction transaction = dao.OpenTransaction();
	PassportRecognitionProfile* newProfile = dao.InsertPassRecogntnProfile(transaction, m_pDelegate->m_userId, profile);
	m_pDelegate->m_passRecgntnProfiles.insert(newProfile);
	transaction->Commit();
	return newProfile->GetId();
}

void UserSession::UserSessionFacadeImpl::DeletePassportRecognitionProfile(uint32_t profileId)
{
	boost::multi_index::index<PassportRecognitionProfilesIndex, index_tag_id>::type& indexById = 
		boost::multi_index::get<index_tag_id>(m_pDelegate->m_passRecgntnProfiles);

	auto profileIt = indexById.find(profileId);
	if (profileIt == indexById.end())
		ThrowPassRecogntnProfileNotFound(profileId);

	UserDAO dao;
	Transaction transaction = dao.OpenTransaction();
	dao.DeletePassRecgntnProfile(transaction, profileId);

	indexById.erase(profileIt);

	transaction->Commit();
}


PtrEnumerator<IDeductionDefinition> UserSession::UserSessionFacadeImpl::EnumDeductionDefinitions() const
{
	return PtrEnumerator<IDeductionDefinition>(m_pDelegate->m_deductionDefinitions);
}

void UserSession::UserSessionFacadeImpl::UpdateDeductionDefinition(IDeductionDefinition* definition)
{
	const boost::multi_index::index<DeductionDefinitionsIndex, index_tag_id>::type& indexById =
		boost::multi_index::get<index_tag_id>(m_pDelegate->m_deductionDefinitions);

	auto deductionIt = indexById.find(definition->GetId());
	if (deductionIt == indexById.end())
		ThrowDeductionDefinitionNotFound(definition->GetId());

	UserDAO dao;
	Transaction transaction = dao.OpenTransaction();
	dao.UpdateDeductionDefinition(transaction, definition);
	DeductionDefinition* deductionToUpdate = (*deductionIt);

	deductionToUpdate->CopyFrom(definition);
	
	transaction->Commit();
}

uint32_t UserSession::UserSessionFacadeImpl::NewDeductionDefinition(IDeductionDefinition* definition)
{
	UserDAO dao;
	Transaction transaction = dao.OpenTransaction();
	DeductionDefinition* newDefinition = dao.InsertDeductionDefinition(transaction, m_pDelegate->m_userId, definition);
	m_pDelegate->m_deductionDefinitions.insert(newDefinition);
	transaction->Commit();
	return newDefinition->GetId();
}

void UserSession::UserSessionFacadeImpl::DeleteDeductionDefinition(uint32_t deductionId)
{
	boost::multi_index::index<DeductionDefinitionsIndex, index_tag_id>::type& indexById =
		boost::multi_index::get<index_tag_id>(m_pDelegate->m_deductionDefinitions);

	auto deductionIt = indexById.find(deductionId);
	if (deductionIt == indexById.end())
		ThrowDeductionDefinitionNotFound(deductionId);

	UserDAO dao;
	Transaction transaction = dao.OpenTransaction();
	dao.DeleteDeductionDefinition(transaction, deductionId);

	indexById.erase(deductionIt);

	transaction->Commit();
}


//#################################################################################################

UserSession::UserSession(uint32_t userId) : m_userId(userId), m_service(1), m_work(NULL), m_settingsManager(userId)
{

}

void UserSession::Do(std::function<void(UserSessionFacade*)>& job)
{
	typedef std::packaged_task<void()> task_t;

	UserSessionFacadeImpl facade(this);
	std::function<void()> f = std::bind(job, &facade);

	std::shared_ptr< task_t > copyable_task = std::make_shared< task_t >(f);
	std::future<void> future = copyable_task->get_future();
	m_service.post(std::bind(&task_t::operator(), copyable_task));
	future.wait();
	future.get(); //trigger throwing an exception	
}

void UserSession::Begin()
{
	if (!m_work)
	{
		m_work = new boost::asio::io_service::work(m_service);
		m_worker = boost::thread(boost::bind(&boost::asio::io_service::run, &m_service));
	}
}

void UserSession::End()
{
	if (m_work)
	{
		delete m_work;
		m_work = NULL;
		m_worker.join();
	}
}


