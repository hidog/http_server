#include "HttpServer.h"

#define HTTP_SERVER_CPP
#include "HttpServer_priv.h"		// �n���w�q HTTP_SERVER_CPP �~��ϥ�.

#include <boost/thread.hpp>
#include "tool.h"

using namespace std;



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**********************************************************
	constructor
***********************************************************/
HttpServer::HttpServer()
{
	port = 1111;
    ip = "0.0.0.0";
}




/**********************************************************
	destructor
***********************************************************/
HttpServer::~HttpServer()
{}





/**********************************************************
	init
***********************************************************/
void	HttpServer::init()
{}




/**********************************************************
	start

	�ثe�ĥΦh���覡
	�C��thread�|�A�}thread�h�B�zrequest.

***********************************************************/
void	HttpServer::start()
{
	string	str_port	=	int_to_string(port);
	boost::thread	*thr[THREAD_MAX];
	HttpServerPriv	http_server_priv;
	int		i;

	try	{
		// create server  (�榡�Ѧ�library.)  ����w�q�b priv��.
		http_handler_struct			handler;
		http_server_type::options	options(handler);
		// 
		http_server_type			http_server( options.address(ip).port(str_port) );
		// bind handle_request
		handler.handle_request	=	boost::bind( &HttpServerPriv::handle_request, &http_server_priv, _1, _2 );

		// create threads.
		for( i = 0; i < THREAD_MAX; i++ )
			thr[i]	=	new boost::thread( boost::bind(&http_server_type::run,&http_server) );

		http_server.run();

	} catch ( std::exception &e ) {
        std::cerr << e.what() << std::endl;
    }

	// delete
	for( i = 0; i < THREAD_MAX; i++ )
	{
		delete thr[i];
		thr[i]	=	NULL;
	}
}



