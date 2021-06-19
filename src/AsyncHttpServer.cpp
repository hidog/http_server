#include "AsyncHttpServer.h"

#define ASYNC_HTTP_SERVER_CPP
#include "AsyncHttpServer_priv.h"		// �קK��L�ɮ�include priv

#include <boost/thread.hpp>
#include <fstream>


using namespace std;





// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**********************************************************
	constructor
***********************************************************/
AsyncHttpServer::AsyncHttpServer()
{
	root_path	=	NULL;
    ip = "0.0.0.0";
    port = 8888;
}




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
	ss << port;
	string	str_port = ss.str();

	try	{
		async_http_handler_struct handler;
		// �o�䤧��A��rename
		file_server::options options(handler);

		file_server instance(
				options.thread_pool(boost::make_shared<boost::network::utils::thread_pool>(THREAD_MAX)).address(ip).port(str_port)	
			);

		root_path	=	handler.root_path;
		instance.run();
	} catch ( exception &e ) {
		printf("error");
	}
}




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


