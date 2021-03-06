// see https://code.google.com/p/cpp-netlib/source/browse/libs/network/example/http/fileserver.cpp?r=685810c885e6d0c03b39b61c0ac6201632eb323c

#pragma once

#if !(defined(ASYNC_HTTP_SERVER_CPP)|defined(ASYNC_HTTP_SERVER_PRIV_CPP))		// 為了保證include此檔案的只有async http server.
#error
#endif

#include <boost/network/include/http/server.hpp>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <map>
#include <vector>

#include "mime_type.hpp"

using namespace std;


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ struct & typedef ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 這邊必須照cpp netlib的格式來寫
struct		async_http_handler_struct;
typedef		boost::network::http::async_server<async_http_handler_struct>		file_server;


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ define ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define ASYNC_OK			1
#define ASYNC_DEFAULT		0
#define ASYNC_ERR			-1
#define ASYNC_ERR_METHOD	-2

#define FILE_POS_DEFAULT	-1


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
enum	ASYNC_METHOD
{
	ASYNC_HTTP_HEAD,
	ASYNC_HTTP_GET,
};



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
size_t				get_file_size( string filename );
__int64				get_file_size( int fh );

void				seperate_cmd_para( string destination, string& cmd, string& para );
void				seperate_filename_path( string para, string &filename, string &path );
bool				is_file_exist( string full_filename );
vector<string>		get_range( file_server::request const & request );
int					parse_method( string method );
int					handle_request( file_server::request const &request, 
									file_server::connection_ptr connection );
int					handle_get( file_server::request const &request, 
								file_server::connection_ptr connection );
void				url_decode( string& str );
map<string,string>	parse_parameter( string para );



// --------------------------------------------------------------------------------------------------------------------------
// async_http_handler_struct
struct	async_http_handler_struct 
{
	// --------------------------------------------------------------------------------------------------------------------------
	// constructor
	explicit async_http_handler_struct() 
	{}

	// destructor
	~async_http_handler_struct() {}

	// --------------------------------------------------------------------------------------------------------------------------
	void operator() (	file_server::request const &request, 
						file_server::connection_ptr connection ) 
	{
		int		res		=	handle_request( request, connection );
	}
};

