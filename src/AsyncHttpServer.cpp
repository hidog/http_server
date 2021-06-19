#include "AsyncHttpServer.h"

#define ASYNC_HTTP_SERVER_CPP
#include "AsyncHttpServer_priv.h"		// 避免其他檔案include priv

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

	目前採用多緒方式
	每個thread會再開thread去處理request.

***********************************************************/
void	AsyncHttpServer::start()
{
	stringstream ss;
	ss << port;
	string	str_port = ss.str();

	try	{
		async_http_handler_struct handler;
		// 這邊之後再來rename
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


