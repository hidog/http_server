#include "tool.h"

#include <boost/lexical_cast.hpp>

using namespace std;



string int_to_string( int n )
{
	/*stringstream	ss;
	ss << input;
	string	result	=	ss.str();	// 不確定是否有生命周期的問題  保險起見多轉一層 速度會變慢 
	return	result;*/
	return	boost::lexical_cast<string>(n);
}




void seperate_filename_extension( const string& source, string& filename, string& extension )
{
	size_t	i	=	source.find_last_of(".");

	if( i == source.npos )		// 無副檔名
	{
		filename	=	source;
		extension	=	"";
		return;
	}

	filename	=	source.substr( 0, i );
	extension	=	source.substr( i+1, source.length() );
}