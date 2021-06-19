#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H


#include <string>



using namespace std;




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/**********************************************************
	HttpServer
***********************************************************/
class HttpServer
{
public:
	HttpServer();
	virtual ~HttpServer();

	//
	void	init();
	void	start();

private:

	const static int	THREAD_MAX	=	10;
	int port;
    std::string ip;
};





#endif