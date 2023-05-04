#include "stdafx.h"
#include "ContextInitialiser.h"
#include "ContextException.h"
#include "service\messages\LoginRequest.h"
#include "service\messages\LoginResponse.h"
#include "service\messages\FarmOrganisation.h"
#include <data\impl\Season.h>
#include <data\impl\Herd.h>
#include <data\impl\Farm.h>
#include <errortoken\ErrorToken.h>
#include <string\Utf8.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

XContextInitialiser::XContextInitialiser(XContext* pContext) : m_pContext(pContext)
{
}

void XContextInitialiser::Init()
{
	if(m_pContext->m_pClient != NULL)
		throw ContextException(L"context has been already initiated");

	 if(m_sAddress.length() == 0)
		 throw ContextException(L"Empty server address");


	 RCF::TcpEndpoint serverEndpoint(Utf8::ToUTF8(m_sAddress),m_Port);
	 RcfClient<IBKService>* pClient = new RcfClient<IBKService>(serverEndpoint);
	 	 
	 RCF::ClientStubPtr clientStub = pClient->getClientStubPtr();
	 clientStub->setRemoteCallTimeoutMs(10*60*1000);
	 clientStub->getTransport().setMaxIncomingMessageLength(5 * 1024 * 1024);
	 //clientStub->setEndpoint();
	 try
	 {
		clientStub->connect();
	 }
	 catch(RCF::Exception &e)
	 {		 
		 delete pClient;
		 throw ContextException(Utf8::FromUTF8(e.getErrorString()));
	 }

	 boost::shared_ptr<LoginRequest> ptrRequest(new LoginRequest());
	 ptrRequest->WithFarmNo(m_farmNo);
	 ptrRequest->WithLogin(m_sLogin);
	 ptrRequest->WithPassword(m_sPassword);

	 try
	 {
		LoginResponseObj response = pClient->Login(ptrRequest);

		if(response->GetResult() != LoginResult_Success)
		{
			throw ContextException(ErrorToken(L"login_failed").str());
		}


		const FarmOrganisation* pOrganization = response->GetFarmOrganisation();
		m_pContext->m_pFarm = new Farm(pOrganization->GetFarm());	
		
		PtrEnumerator<IHerd> herdsEnum = pOrganization->EnumHerds();
		while(herdsEnum.hasNext())
		{
			Herd* pHerd = new Herd(*herdsEnum);
			m_pContext->m_Herds.insert(pHerd);
			herdsEnum.advance() ;
		}
		
		PtrEnumerator<ISeason> seasonsEnum = pOrganization->EnumSeasons();
		while (seasonsEnum.hasNext())
		{
			m_pContext->m_Seasons.insert(new Season(*seasonsEnum));
			seasonsEnum.advance();
		}

		PtrEnumerator<IAgent> agentsEnum = pOrganization->EnumAgents();
		while (agentsEnum.hasNext())
		{
			Agent* agent = new Agent(*agentsEnum);
			m_pContext->m_Agents.insert(agent);
			agentsEnum.advance();
		}

		PtrEnumerator<IClass> classEnum = pOrganization->EnumClasses();
		while (classEnum.hasNext())
		{
			Class* cowClass = new Class(*classEnum);
			m_pContext->m_Classes.add(cowClass);
			classEnum.advance();
		}

		PtrEnumerator<IStock> stockEnum = pOrganization->EnumStocks();
		while (stockEnum.hasNext())
		{
			Stock* stock = new Stock(*stockEnum);
			m_pContext->m_Stocks.add(stock);
			stockEnum.advance();
		}

		PtrEnumerator<IBankAccount> accountsEnum = pOrganization->EnumAccounts();
		while (accountsEnum.hasNext())
		{
			BankAccount* account = new BankAccount(*accountsEnum);
			m_pContext->m_Accounts.insert(account);
			accountsEnum.advance();
		}

		IPrintProfile* printProfile;
		FetchPrintProfilesResponseObj profilesRs = pClient->FetchProfiles();
		PtrEnumerator<IPrintProfile> profilesEnum = profilesRs->GetProfiles();
		while (profilesEnum.hasNext())
		{
			printProfile = *profilesEnum;
			m_pContext->m_printProfiles.insert(new PrintProfile(printProfile));
			profilesEnum.advance();
		}	


		IPassportRecognitionProfile* passportRecognitionProfile;
		FetchPassportRecognitionProfilesResponseObj recgntnProfilesRs = pClient->FetchPassportRecognitionProfiles();
		PtrEnumerator<IPassportRecognitionProfile> recgntnProfilesEnum = recgntnProfilesRs->GetProfiles();
		while (recgntnProfilesEnum.hasNext())
		{
			passportRecognitionProfile = *recgntnProfilesEnum;
			m_pContext->m_passRecgntnProfiles.insert(new PassportRecognitionProfile(passportRecognitionProfile));
			recgntnProfilesEnum.advance();
		}


		IDeductionDefinition* deductionDefinition;
		FetchDeductionDefinitionsResponseObj deductionDefsRs = pClient->FetchDeductionDefinitions();
		PtrEnumerator<IDeductionDefinition> deductionDefsEnum = deductionDefsRs->GetDefinitions();
		while (deductionDefsEnum.hasNext())
		{
			deductionDefinition = *deductionDefsEnum;
			m_pContext->m_deductionDefinitions.insert(new DeductionDefinition(deductionDefinition));
			deductionDefsEnum.advance();
		}

		SettingsObj settings = pClient->FetchSettings(0);
		PtrEnumerator<ISetting> settingEnum = settings->EnumSettings();

		ISetting* setting;
		while (settingEnum.hasNext())
		{
			setting = *settingEnum;
			m_pContext->m_settings.insert(std::make_pair(setting->GetName(), new Setting(setting)));
			settingEnum.advance();
		}

		m_pContext->m_User = new SimpleUser(response->GetUser());
		
	 }
	 catch(RCF::Exception &e)
	 {
		 throw ContextException(Utf8::FromUTF8(e.getErrorString()));
	 }

	 m_pContext->m_pSettingsFacade = new UserSettingsImpl(m_pContext);

	 m_pContext->m_pClient = pClient;
	 m_pContext->m_ServerEndpoint = serverEndpoint;
}



void XContextInitialiser::SetServer(const std::wstring& sAddress,uint16_t port)
{
	m_sAddress = sAddress;
	m_Port = port;
}

void XContextInitialiser::SetLoginCredentials(const std::wstring& sLogin,const std::wstring& sPassword)
{
	m_sLogin = sLogin;
	m_sPassword = sPassword;
}

void XContextInitialiser::SetFarm(const FarmNo& farmNo)
{
	m_farmNo = farmNo;
}


