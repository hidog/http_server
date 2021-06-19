#include "AsyncHttpServer.h"

#define ASYNC_HTTP_SERVER_CPP
#include "AsyncHttpServer_priv.h"		// �קK��L�ɮ�include priv

#include <boost/thread.hpp>
#include <fstream>


using namespace std;

extern int g_port;
extern char g_ip[100];


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**********************************************************
	constructor
***********************************************************/
AsyncHttpServer::AsyncHttpServer()
{}




/**********************************************************
	destructor
***********************************************************/
AsyncHttpServer::~AsyncHttpServer()
{}




/**********************************************************
	init
***********************************************************/
void	AsyncHttpServer::init()
{}






/**********************************************************
	start

	�ثe�ĥΦh���覡
	�C��thread�|�A�}thread�h�B�zrequest.

***********************************************************/
void	AsyncHttpServer::start()
{
	stringstream ss;
	ss << g_port;
	string	str_port = ss.str();

	try	{
		async_http_handler_struct handler;
		// �o�䤧��A��rename
		file_server::options options(handler);

		file_server instance(
				options.thread_pool(boost::make_shared<boost::network::utils::thread_pool>(THREAD_MAX)).address(std::string(g_ip)).port(str_port)	
			);
		instance.run();
	} catch ( exception &e ) {
		printf("error");
	}
}




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


