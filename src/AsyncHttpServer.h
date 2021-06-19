#ifndef ASYNC_HTTP_SERVER_H
#define ASYNC_HTTP_SERVER_H

#include <string>



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
enum ASYNC_HTTP_GET_CMD
{
	ASYNC_HTTP_GET_FILE,
};



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/**********************************************************
	AsyncHttpServer
***********************************************************/
class AsyncHttpServer 
{
public:
	AsyncHttpServer();
	virtual ~AsyncHttpServer();

	//
	void	init();
	void	start();

	char	*root_path;

private:
	const static int	THREAD_MAX	=	20;

    int port;
    std::string ip;
};





#endif