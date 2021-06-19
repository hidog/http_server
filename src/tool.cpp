#include "tool.h"

#include <boost/lexical_cast.hpp>
#include <stdio.h>


using namespace std;


int g_port;
char g_ip[100];
char g_ext[100];
char g_root[100];




string int_to_string( int n )
{
	/*stringstream	ss;
	ss << input;
	string	result	=	ss.str();	// ���T�w�O�_���ͩR�P�������D  �O�I�_���h��@�h �t�׷|�ܺC 
	return	result;*/
	return	boost::lexical_cast<string>(n);
}




void seperate_filename_extension( const string& source, string& filename, string& extension )
{
	size_t	i	=	source.find_last_of(".");

	if( i == source.npos )		// �L���ɦW
	{
		filename	=	source;
		extension	=	"";
		return;
	}

	filename	=	source.substr( 0, i );
	extension	=	source.substr( i+1, source.length() );
}



void load_config()
{
    FILE *fp = fopen("config.txt", "r" );
    if( fp == NULL )
        printf( "error !! \n" );

    fscanf( fp, "%s", &g_ip );
    fscanf( fp, "%d", &g_port );
    fscanf( fp, "%s", &g_root );
    fscanf( fp, "%s", &g_ext );
}
