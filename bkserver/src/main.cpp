#include "stdafx.h"
#include "BKServer.h"
#include "db/SQLSingleConnectionPool.h"
#include "Log.h"
#include <service/TypesRegistration.h>
#include <boost/program_options.hpp>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <filesystem/paths.h>
#include "repo/RepoClientFactory.h"
#include <activemq/library/ActiveMQCPP.h>
#include "db/SQLException.h"
#include <string/TextUtils.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string/classification.hpp>
//#include <aws/core/Aws.h>

#ifdef MSVC
#include <tchar.h>
#endif


namespace po = boost::program_options;
namespace fs = boost::filesystem;

#if !defined MSVC

#define DAEMON_NAME "bkserver"

static bool _terminate;

void signal_handler(int sig)
{
	if(sig == SIGTERM)
	{
		_terminate = true;
	}
}

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>

/*! Try to get lock. Return its file descriptor or -1 if failed.
 *
 *  @param lockName Name of file used as lock (i.e. '/var/lock/myLock').
 *  @return File descriptor of lock file, or -1 if failed.
 */
int obtain_lock( char const *lockName )
{
    mode_t m = umask( 0 );
    int fd = open( lockName, O_RDWR|O_CREAT, 0666 );
    umask( m );
    if( fd >= 0 && flock( fd, LOCK_EX | LOCK_NB ) < 0 )
    {
        close( fd );
        fd = -1;
    }
    return fd;
}

/*! Release the lock obtained with tryGetLock( lockName ).
 *
 *  @param fd File descriptor of lock returned by tryGetLock( lockName ).
 *  @param lockName Name of file used as lock (i.e. '/var/lock/myLock').
 */
void release_lock( int fd, char const *lockName )
{
    if( fd < 0 )
        return;
    remove( lockName );
    close( fd );
}


void daemonize()
{	
	pid_t pid = fork();
	if (pid < 0) {
		printf("unable to fork, errno = %d\n", errno);
		exit(1); /* fork error */
	}

	if (pid>0) 
	{
		printf("parent exiting...\n");
		exit(0); /* parent exits */
	}
	/* child (daemon) continues */
	setsid(); /* obtain a new process group */

	int i;
	for (i = getdtablesize(); i >= 0; --i) close(i); /* close all descriptors */
	i = open("/dev/null", O_RDWR); dup(i); dup(i); /* handle standart I/O */
	umask(027); /* set newly created file permissions */
	
	//fs::path tempDir = fs::temp_directory_path();
	//fs::path lockFilePath = tempDir / fs::path(DAEMON_NAME);

	//int lfp = open(lockFilePath.c_str(), O_RDWR | O_CREAT, 0640);
	//if (lfp < 0)
	//{
	//	printf("cannot open lock file\n");
	//	exit(1);
	//}

	//if (lockf(lfp, F_TLOCK, 0)<0)
	//{
	//	printf("cannot lock, another instance is running");
	//	exit(1); /* can not lock */
	//}

	/* first instance continues */
	//char pidstr[10];
	//sprintf(pidstr, "%d\n", getpid());
	//write(lfp, pidstr, strlen(pidstr)); /* record pid to lockfile */

	signal(SIGCHLD, SIG_IGN); /* ignore child */
	signal(SIGTSTP, SIG_IGN); /* ignore tty signals */
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGHUP, SIG_IGN);  /* ignore hangup signal */
	signal(SIGTERM, signal_handler); /* catch kill signal */
}
#endif

const char* HELP_OPTION_NAME = "help";
const char* DBHOST_OPTION_NAME = "dbhost"; 
const char* DBPORT_OPTION_NAME = "dbport";
const char* DBUSER_OPTION_NAME = "dbuser";
const char* DBPASS_OPTION_NAME = "dbpassword";
const char* REPOBROKER_OPTION_NAME = "repobroker";
const char* FARM_OPTION_NAME = "farm";
const char* SERVERPORT_OPTION_NAME = "port";
const char* DEAMON_OPTION_NAME = "demon";
const char* GACCOUNTFILE_OPTION_NAME = "gaccoutfile";
const char* REPOURL_OPTION_NAME = "repourl";
const char* AUTH0FILE_OPTION_NAME = "auth0file";

#if defined MSVC
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char* argv[])
#endif
{
	po::options_description options("server options");

	options.add_options()
		(HELP_OPTION_NAME, "produce help message")
		(DEAMON_OPTION_NAME, "run as a demon")
		(DBHOST_OPTION_NAME, po::value<std::string>(), "db host")
		(DBPORT_OPTION_NAME, po::value<uint16_t>(), "db port")
		(DBUSER_OPTION_NAME, po::value<std::string>(), "db user")
		(DBPASS_OPTION_NAME, po::value<std::string>(), "db password")
		(REPOBROKER_OPTION_NAME, po::value<std::string>(), "repo broker uri")
		(FARM_OPTION_NAME, po::value<std::string>(), "farm number")
		(SERVERPORT_OPTION_NAME, po::value<uint16_t>(), "server listen port")
		(GACCOUNTFILE_OPTION_NAME, po::value<std::string>(), "path to google cloud service account json file")
		(REPOURL_OPTION_NAME, po::value<std::string>(), "url to purchase repository")
		(AUTH0FILE_OPTION_NAME, po::value<std::string>(), "path to auth0 account json file")
		;

	po::variables_map vm;
	bool demon = false;

	po::store(po::parse_command_line(argc, argv, options), vm);

	fs::path cfgFileName = ".bkserver.cfg";
	fs::path homeCfg = fs::path(paths_GetApplicationDir()) / cfgFileName;
	if (fs::exists(fs::path(homeCfg)))
	{		
		po::store(po::parse_config_file<char>(homeCfg.string().c_str(), options), vm);
	}

	po::notify(vm);
	
	if (vm.count(HELP_OPTION_NAME)) {
		cout << options << endl;
		return 1;
	}
	
	if (vm.count(DEAMON_OPTION_NAME)) {
		demon = true;
	}

	if (!vm.count(DBHOST_OPTION_NAME))
	{
		cout << "Unable to determine database host property: " << DBHOST_OPTION_NAME << endl;
		return 2;
	}
		
	if (!vm.count(DBPORT_OPTION_NAME))
	{
		cout << "Unable to determine database port property: " << DBPORT_OPTION_NAME << endl;
		return 2;
	}

	if (!vm.count(DBUSER_OPTION_NAME))
	{
		cout << "Unable to determine database user property: " << DBUSER_OPTION_NAME << endl;
		return 2;
	}
	if (!vm.count(DBPASS_OPTION_NAME))
	{
		cout << "Unable to determine database password property: " << DBPASS_OPTION_NAME << endl;
		return 2;
	}

	if (!vm.count(REPOURL_OPTION_NAME))
	{
		cout << "Unable to determine purchase repository url property: " << REPOURL_OPTION_NAME << endl;
		return 2;
	}

	if (!vm.count(FARM_OPTION_NAME))
	{
		cout << "Unable to determine farm number property: " << FARM_OPTION_NAME << endl;
		return 2;
	}

	if (!vm.count(SERVERPORT_OPTION_NAME))
	{
		cout << "Unable to determine server listen port property: " << SERVERPORT_OPTION_NAME << endl;
		return 2;
	}

	if (!vm.count(AUTH0FILE_OPTION_NAME))
	{
		cout << "Unable to determine Auth0 account file path property: " << AUTH0FILE_OPTION_NAME << endl;
		return 2;
	}


	

#ifndef MSVC
	_terminate = false;
	
	if(demon)
	{
		daemonize();
	}
#endif

	std::string dbHost = vm[DBHOST_OPTION_NAME].as<std::string>();
	uint16_t dbPort = vm[DBPORT_OPTION_NAME].as<uint16_t>();
	std::string dbUser = vm[DBUSER_OPTION_NAME].as<std::string>();
	std::string dbPassword = vm[DBPASS_OPTION_NAME].as<std::string>();
	std::string repoUrl = vm[REPOURL_OPTION_NAME].as<std::string>();
	std::string sFarmNums = vm[FARM_OPTION_NAME].as<std::string>();
	uint16_t serverPort = vm[SERVERPORT_OPTION_NAME].as<uint16_t>();
	std::string auth0JsonFile = vm[AUTH0FILE_OPTION_NAME].as<std::string>();
	
	std::string sGAccountJsonFile = "";

	if (vm.count(GACCOUNTFILE_OPTION_NAME))
	{
		sGAccountJsonFile = vm[GACCOUNTFILE_OPTION_NAME].as<std::string>();
	}

	gcp::GoogleServiceAccount gaccount;
	if (!sGAccountJsonFile.empty())
	{
		gcp::GoogleServiceAccountFactory gAccountFactory;
		std::ifstream ifs(sGAccountJsonFile, std::ifstream::in);

		if (!ifs.is_open())
		{
			wcout << L"cannot open gcloud service json file: " << TextUtils::FromUTF8(sGAccountJsonFile) << endl;
			return 3;
		}

		gaccount = gAccountFactory.Create(ifs);
		ifs.close();		
		
	}
	else
	{
		wcout << L"gcloud service json file not specified - publishing to gcloud id disabled." << endl;
	}

	std::vector<FarmNo> farmNums;
	try
	{
		typedef vector< string > split_vector_type;

		std::vector<std::string> farmNumTokens;
		boost::algorithm::split(farmNumTokens, sFarmNums, boost::algorithm::is_any_of(","), boost::token_compress_on); 
		
		BOOST_FOREACH(std::string token, farmNumTokens)
		{
			farmNums.push_back(FarmNoFactory::FromString(TextUtils::FromUTF8(token)));
		}
	}
	catch (FarmNoException& e)
	{
		wcout << e.message() << endl;
		return 2;
	}

	Log::Configure();
	RegisterTypes();	

	DbConnectionOptions dbOptions;
	dbOptions.Host(dbHost).Port(dbPort);
	dbOptions.User(dbUser).Password(dbPassword);

	try
	{
		SQLSingleConnectionPool::GetInstance()->CreatePool(dbOptions, 1, 5);
	}
	catch (SQLException& e)
	{
		std::wcout << e.wwhat() << endl;
		return 1;
	}
	
	activemq::library::ActiveMQCPP::initializeLibrary();



	std::string sAuth0JsonFile = vm[AUTH0FILE_OPTION_NAME].as<std::string>();
	Auth0AccountFactory auth0AccountFactory;
	std::ifstream ifs(sAuth0JsonFile, std::ifstream::in);

	if (!ifs.is_open())
	{
		wcout << L"cannot open auth0 json file: " << TextUtils::FromUTF8(sAuth0JsonFile) << endl;
		return 4;
	}

	Auth0Account account = auth0AccountFactory.Create(ifs);
	ifs.close();


	repo::RepoConnectionSettings repoConnSettings;
	repoConnSettings.Url(repoUrl);
	repoConnSettings.Auth0(account);

//	Aws::SDKOptions options;
//	Aws::InitAPI(options);

		
	BKServer server;
	std::cout << "Initializing server..." << std::endl;
	server.Initialize(serverPort, farmNums, gaccount,repoConnSettings);
		
#ifndef MSVC	
	if(demon)
	{
		while (!_terminate)
		{
			sleep(1);
		}
	}
	else
#endif
	{
		
	    std::cout << "Press Enter to exit..." << std::endl;
		std::cin.get();
		std::cout << "Stopping server..." << std::endl;	
	}

//	Aws::ShutdownAPI(options);		
	server.Finalize();	
	activemq::library::ActiveMQCPP::shutdownLibrary();
	SQLSingleConnectionPool::GetInstance()->DestroyPool();

	return 0;
}

