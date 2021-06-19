#define HTTP_SERVER_PRIV_CPP
#include "HttpServer_priv.h"



#include <cstring>
#include <fstream>
#include <sstream>

#include "mime_type.hpp"




/**********************************************************
	constructor
***********************************************************/
HttpServerPriv::HttpServerPriv()
{}





/**********************************************************
	destructor
***********************************************************/
HttpServerPriv::~HttpServerPriv()
{}







/**********************************************************
	trans_cmd
***********************************************************/
int		HttpServerPriv::trans_get_cmd( string cmd )
{
	if( cmd == "validate_device" )
		return	HTTP_VALIDATE_DEVICE;
	else
		return	HTTP_SERVER_ERR_CMD;
}





/**********************************************************
	trans_method
***********************************************************/
int		HttpServerPriv::trans_method( string method )
{
	if( method == "GET" )
		return	HTTP_GET;
	else
		return	HTTP_SERVER_ERR_METHOD;
}





/**********************************************************
	parse_destination
***********************************************************/
map<string,string>	HttpServerPriv::parse_destination( string dest )
{
	char	cmd_c[100],		para_c[100];
	sscanf( dest.c_str() , "/%[^?]?%[^/ \\ \n]", cmd_c, para_c );

	map<string,string>	result_map;

	result_map["cmd"]	=	cmd_c;
	result_map["para"]	=	para_c;

	return	result_map;
}



/**********************************************************
	handle_get
***********************************************************/
int		HttpServerPriv::handle_get( http_server_type::request const &request, 
									http_server_type::response &response )
{
	map<string,string>	dest_map	=	parse_destination( request.destination );
	map<string,string>::iterator	cmd_itr,	para_itr;

	cmd_itr		=	dest_map.find("cmd");
	para_itr	=	dest_map.find("para");

	int		res		=	HTTP_SERVER_DEFAULT;

	if( cmd_itr == dest_map.end() || para_itr == dest_map.end() )
		return	HTTP_SERVER_ERR_PARAMETER;

	switch( trans_get_cmd(cmd_itr->second) )
	{
		case HTTP_VALIDATE_DEVICE:
			// validate func.
			break;
		default:
			BOOST_ASSERT(false);
	}

	return	res;
}





/**********************************************************
	handle_request
***********************************************************/
map<string,string>	HttpServerPriv::parse_parameter( string para )
{
	map<string,string>	result_map;

	//if( para.back() == '/' || para.back() == '\\' )	// 理論上在之前的parse就已經把最後面的斜線拿掉了.
		//para.pop_back();

	int		pos;
	char	key_c[1000],	value_c[1000];
	string	sub_str;

	
	//
	while( !para.empty() )
	{
		pos		=	para.find_last_of('&');

		if( pos == string::npos )
		{
			sub_str		=	para;
			para.erase( 0, para.length() );
		}
		else
		{
			sub_str		=	para.substr( pos+1, string::npos );
			para.erase( pos, para.length() );
		}

		sscanf( sub_str.c_str(), "%[^=]=%[^\n]", key_c, value_c );
		result_map[ key_c ]		=	value_c;	
	}

	return	result_map;
}





/**********************************************************
	handle_request
***********************************************************/
int		HttpServerPriv::handle_request( http_server_type::request const& request, 
										http_server_type::response & response )
{
	string	method	=	request.method;
	string	dest	=	request.destination;	

	vector<string>	range;

	int		res	=	HTTP_SERVER_DEFAULT;

	switch( trans_method(method) )
	{
		case HTTP_GET:
			res		=	handle_get( request, response );
			break;
		default:
			BOOST_ASSERT(false);
	}

	return	res;
}




