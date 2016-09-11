/**********************************************************
 * \file logic/tool/File.cpp
 * \brief 
 * \note 
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#include "common_file.h"

#include "boost/regex.hpp"
#include "common_tool.h"

namespace Universal{
	string readFile( const string &path ){
		ifstream infile;
		infile.open( path.c_str() );
		if( infile ){
			ostringstream ostr;
			ostr << infile.rdbuf( );
			infile.close( );
			string strTmp = ostr.str( );

			return strTmp;
		}
		else{
			DEBUG_I( "%s: open aciton is failed.Please check arg of path: %s.", _RUN_FUNC_, path.c_str() );
			return _STRINGFALSE;
		}
	}
	bool writeFile( const string &path, const string &content ){
		std::ofstream outfile( path.c_str() );
		if( outfile ){
			outfile << content;
			outfile.close( );
			return true;
		}
		else{
			DEBUG_I( "%s: Can't open file.Please check arg of path: %s.", _RUN_FUNC_, path.c_str() );
		}
		return false;
	}
	bool addContentToFile( const string &path, const string &content ){
		std::ofstream outfile( path.c_str(), ios::app );
		if( outfile ){
			outfile << content;
			outfile.close( );
			return true;
		}
		else{
			DEBUG_I( "%s: Can't open file.Please check arg of path: %s.", _RUN_FUNC_, path.c_str() );
			return false;
		}
	}
	bool createFolder( string path, mode_t mode ){
		if( path.size() == 0 )  return true;
		string::size_type index( 0);
		string curFolder;
		if( path.at(path.length()-1) != '/' )  path += "/";
		while( 1){
			index = path.find( '/', index );
			if( index == string::npos )
				break;
			curFolder = path.substr( 0, index+1 );
			if( access(curFolder.c_str(), F_OK ) != 0 ){
				if( mkdir( curFolder.c_str(), mode ) != 0){
					DEBUG_I( "%s: Can't create this folder.full path is %s,current index is %d. folder name is %s", _RUN_FUNC_, path.c_str(), index, curFolder.c_str() );
					return false;
				}
			}
			++index;
		}

		return true;
	}


	bool clearFolder( string path ){
		return execShellCmd( "rm -rf " + completePath(path) + "*" );
	}


	bool deleteFile( const string &filePath ){
		return execShellCmd( "rm -f " + completePath(filePath) );
	}


	bool findFileFromDir( string path, vector<string> &fileNames ){
		fileNames.clear( );
		if( path.size() == 0 )  return true;
		if( path.at(path.length()-1) != '/' )  path += "/";
		
		DIR *pDir = opendir( path.c_str() );
		struct dirent *dirent_p;
		if( pDir != NULL ){
			while(1){
				dirent_p = readdir( pDir );
				if( dirent_p == NULL )  break;
				if( dirent_p->d_type != DT_REG )  continue;
				fileNames.push_back( dirent_p->d_name );
			}
		}
		else DEBUG_I( "%s: Can't find direction. Dir path is %s.", _RUN_FUNC_, path.c_str() );
		closedir( pDir);  pDir = NULL;
		return true;
	}


	bool findFileFromDir( string path, const string &filePattern, vector<string> &fileNames ){
		if( !findFileFromDir( path, fileNames ) ){ return false; }
		
		boost::regex reg( filePattern );
		vector<string>::iterator iterFile = fileNames.begin();
		while( iterFile != fileNames.end() ){
			if( !boost::regex_match( *iterFile, reg ) ){ 
				iterFile = fileNames.erase( iterFile );
			}
			else{
				++iterFile;
			}
		}

		return true;
	}
} // namespace

