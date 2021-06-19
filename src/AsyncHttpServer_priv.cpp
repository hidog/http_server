#define ASYNC_HTTP_SERVER_PRIV_CPP
#include "AsyncHttpServer_priv.h"


#include <string>
#include <fstream>
#include <stdio.h>

#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "tool.h"


using namespace std;


extern char g_ext[100];
extern char g_root[100];






// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 實際上處理檔案傳輸的部分.
struct connection_handler : boost::enable_shared_from_this<connection_handler> 
{
	// --------------------------------------------------------------------------------------------------------------------------
	// constructor
	explicit connection_handler() {}

	// --------------------------------------------------------------------------------------------------------------------------
	// 配合cpp netlib的設計  先這樣寫  (留意boost::enable_shared_from_this<connection_handler>)  
	// 之前修改的時候發生crash
    void operator() (	std::string const &destination, 
						file_server::connection_ptr connection, 
						__int64 begin, __int64 end ) 
	{
        // 移除斜線
        std::string filename = destination.substr( 1 );

		string	full_filename = std::string(g_root) + filename + std::string(".") + std::string(g_ext);
            //"D:\\1.avi";

		// 開啟檔案
		int		fd	=	_open( full_filename.c_str(), _O_BINARY | _O_RDONLY );
		if( fd == -1 )
		{
			not_found(full_filename, connection);
			return;
		}

		// 傳輸檔案
		__int64		file_size	=	get_file_size(fd);
		//size_t	file_size		=	get_file_size(full_filename);
		if( end == FILE_POS_DEFAULT )	
			end		=	file_size;
		send_headers( connection , begin, end, file_size, "1.avi" );
		send_file( fd, begin, end, file_size, connection );
		 
		//_close(fd);		// 檢查這邊close的時機是否正確
	}


	// --------------------------------------------------------------------------------------------------------------------------
	void not_found( string const &path, 
					file_server::connection_ptr connection ) 
	{
		static file_server::response_header headers[] = {
				{"Connection","close"}
				,{"Content-Type", "text/plain"}
			};

		connection->set_status(file_server::connection::not_found);
		connection->set_headers(boost::make_iterator_range(headers, headers + 2));
		connection->write("File Not Found!");
	}

	// --------------------------------------------------------------------------------------------------------------------------
	void send_headers( file_server::connection_ptr connection , __int64 begin, __int64 end, __int64 file_size, string filename_extension ) 
	{
		string	file_name,	extension;
		seperate_filename_extension( filename_extension, file_name, extension );

		file_server::response_header common_headers[] = {
				 { "Content-Length",		boost::lexical_cast<std::string>(file_size) }
				,{ "Content-Disposition",	string("inline; filename=") + filename_extension }		// attachment
				,{ "Content-Type",			mime_types::extension_to_type( extension ) }
				,{ "Accept-Ranges",			"bytes" }
				//,{ "Content-Range",			string("bytes ") + boost::lexical_cast<std::string>(begin) + string("-") + boost::lexical_cast<std::string>(end) + string("/") + boost::lexical_cast<std::string>(file_size) }
				,{ "Content-Range",			string("bytes ") + int_to_string(begin) + string("-") + int_to_string(end) + string("/") + int_to_string(file_size) }
			};

		connection->set_status( file_server::connection::partial_content );
		connection->set_headers( boost::make_iterator_range(common_headers, common_headers + 5) );
	}


	// --------------------------------------------------------------------------------------------------------------------------
	void send_file( int fd, __int64 begin, __int64 end, __int64 file_size, file_server::connection_ptr connection )
	{
		_lseeki64( fd, begin, SEEK_SET );

		if( begin > end )
		{
			_close(fd);		// 檢查這邊close的時機是否正確
			return;
		}

		const int	buf_size	=	5000;

		char	buf[buf_size];
		//infile.read( buf, buf_size );
		if( _read( fd, buf, buf_size ) <= 0 )
		{
			_close(fd);
			return;
		}

		// 這邊要小心 之前不照cpp netlib的做法 會發生crash.
		connection->write	(	boost::asio::const_buffers_1( static_cast<char const *>(buf), buf_size )  ,		// 參數1
								boost::bind( &connection_handler::handle_chunk,									// 整串是參數2
										connection_handler::shared_from_this(),	// 似乎只能用shared from this的樣子....
										fd, begin + buf_size, end, file_size, connection, _1 ) 
							);
	}


	// --------------------------------------------------------------------------------------------------------------------------
	void handle_chunk( int fd, __int64 begin, __int64 end, __int64 file_size, file_server::connection_ptr connection, boost::system::error_code const &ec ) 
	{
		if (!ec)
			send_file( fd, begin, end, file_size, connection );
		else
			_close(fd);		// 檢查這邊close的時機是否正確
	}
};















/**********************************************************
	get_file_size
***********************************************************/
__int64		get_file_size( int fd )
{
	__int64		size	=	0;
	_lseeki64( fd, 0, SEEK_END );
	size	=	_telli64(fd);
	_lseeki64( fd, 0, SEEK_SET );		
	return	size;
}



/**********************************************************
	get_file_size
***********************************************************/
size_t		get_file_size( string filename )
{
	ifstream	infile( filename );

	infile.seekg ( 0, infile.end );
	size_t	size	=	infile.tellg();

	return	size;
}



/**********************************************************
	seperate_cmd_para
***********************************************************/
void	seperate_cmd_para( string destination, string& cmd, string& para )
{
	char	cmd_c[1000],	para_c[1000];
	sscanf( destination.c_str() , "/%[^?]?%[^\n]", cmd_c, para_c );		// note: path會有斜線.

	cmd		=	string(cmd_c);
	para	=	string(para_c);
}


/**********************************************************
	seperate_filename_path
***********************************************************/
void	seperate_filename_path( string para, string &filename, string &path )
{
	char	file_name_c[1000],	path_c[1000];
	//sscanf( para.c_str() , "file_name=%[^&]&file_path=%s[^\n]", file_name_c, path_c );
	sscanf( para.c_str() , "file_path=%[^&]&file_name=%[^\n]", path_c, file_name_c );
	//string full_filename	=	string("D:\\" ) + string(path_c) + string(file_name_c);

	filename	=	string(file_name_c);
	path		=	string(path_c);
}


/**********************************************************
	is_file_exist
***********************************************************/
bool	is_file_exist( string full_filename )
{
	ifstream	infile( full_filename );

	if( infile.is_open() )
		return	true;
	else
		return	false;
}


/**********************************************************
	get_range
***********************************************************/
vector<string>	get_range( file_server::request const & request )
{
	vector<string>	range;
	int i;
	for( i = 0; i < request.headers.size(); i++ )
	{
		if( request.headers[i].name == "Range" )
		{
			//range = request.headers[i].value;
			printf( "%s\n", request.headers[i].value.c_str() );
			range.push_back( request.headers[i].value );
			break;
		}
	}

	return	range;
}



/**********************************************************
	parse_method
***********************************************************/
int		parse_method( string method )
{
	if( method == "HEAD" )
		return	ASYNC_HTTP_HEAD;
	else if( method == "GET" )
		return	ASYNC_HTTP_GET;
	else
		return	ASYNC_ERR_METHOD;
}




/**********************************************************
	handle_get
***********************************************************/
int		handle_get( file_server::request const &request, 
					file_server::connection_ptr connection )
{
	int		i;

	// analysis range
	std::vector<std::string>	range	=	get_range( request );
	__int64		begin	=	FILE_POS_DEFAULT, 
				end		=	FILE_POS_DEFAULT;
	if( range.size() == 0 )		// head no range.
		begin	=	0;
	else if( range.size() == 1 )
	{
		//connection->set_status(async_server_type::connection::partial_content);
		if( range[0].back() == '-' )
			sscanf( range[0].c_str(), "bytes=%I64d-" , &begin );	
		else
			sscanf( range[0].c_str(), "bytes=%I64d-%I64d" , &begin, &end );	
	}
	else
		BOOST_ASSERT(false);

	// note: 這邊之前改掉後會發生crash  要研究shared ptr以後再修改.
	boost::shared_ptr<connection_handler>	handler( new connection_handler() );
	(*handler)( request.destination, connection, begin, end );

	return	ASYNC_OK;
}



/**********************************************************
	handle_request
***********************************************************/
int		handle_request( file_server::request const &request, 
						file_server::connection_ptr connection )
{
	int		res	=	ASYNC_DEFAULT;

	switch( parse_method(request.method) )
	{
		case ASYNC_HTTP_HEAD:
			BOOST_ASSERT(false);
			//boost::shared_ptr<connection_handler> h(new connection_handler());
			//(*h)(request.destination, connection, false, 0, 0, root_path);
			break;
		case ASYNC_HTTP_GET:
			res		=	handle_get( request, connection );
			break;
		default:
			BOOST_ASSERT(false);
			/*static file_server::response_header error_headers[] = {
					{ "Connection", "close" }
				};	
			connection->set_status(file_server::connection::not_supported);
			connection->set_headers(boost::make_iterator_range(error_headers, error_headers+1));
			connection->write("Method not supported.");*/
	}

	return	res;
}


/**********************************************************
	parse_parameter
***********************************************************/
map<string,string>	parse_parameter( string para )
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
	url_decode
	also see http://cpp-netlib.org/0.11.0/examples/http/simple_wget.html  cpp netlib的做法 
***********************************************************/
void	url_decode( std::string& str )
{
	//QString		qstr( str.c_str() );
	//QUrl		qurl( qstr );

	//printf( "%s\n", qurl.path().toStdString().c_str() );

	//str		=	qurl.path().toStdString().c_str() ;
	//std::string		output( qurl.path().toStdString().c_str() );
	//return	outupt;
}