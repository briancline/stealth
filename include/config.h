#ifndef __CONFIG_H__
#define __CONFIG_H__ "$Id: config.h,v 1.12 2002/06/23 22:21:17 bcline Exp $"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <map>
#include <ctime>

using namespace std;


typedef map<string, string> ConfigMapType;

class Config
{
	public:
		
		Config();
		Config( string filename );
		~Config();
		
		bool LoadConfiguration();
		bool SaveConfiguration();
		string RemoveBreaks( string strLine );
		string RemoveWhitespace( string strLine );
		string GetParam( string paramName );
		void SetParam( string paramName, string newValue );
		bool IsComment( string line );

		bool LoadUsers();
		bool LoadJupes();
		bool LoadClones();
		bool LoadChans();
		bool LoadExceptions();
		bool LoadCommands();

		bool SaveUsers();
		bool SaveJupes();
		bool SaveClones();
		bool SaveChans();
		bool SaveExceptions();
		bool SaveCommands();


	private:
		
		ConfigMapType c_options;
		time_t c_lastLoaded;
		string c_filename;
	
};


#endif // __CONFIG_H__
