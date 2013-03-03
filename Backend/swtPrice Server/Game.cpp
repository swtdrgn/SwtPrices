#include "StdAfx.h"
#include "Game.h"
#include <sstream>
#include <time.h>

#define HOST "localhost"
#define PORT 3306
#define USER "INSERT USERNAME"
#define PASS "INSERT PASSWORD"
#define DB "swtprices"
#define TABLE "games"

#define COL_UNIQUENAME "uniquename"
#define COL_NAME "name"

Game::Game( string name ){
	set_name(name);
}


Game::~Game(void)
{
}

void Game::filter_char( string & str ){
	if( str.length() == 0 ){ return; }

	string::iterator it=str.begin();
	while( it!=str.end() ){
		if( (*it) < 0x20 || (*it) > 0x7E ){
			it = str.erase(it);
		}
		else if( (*it) == '\'' ){
			it = str.insert(it,'\\');
			it+=2;
		}
		else if( (*it) == '<' ){
			it = str.erase(it);
			while( it!=str.end() && (*it)!='>' ){
				it = str.erase(it);
			}
			if( it!=str.end() && (*it)=='>' ){
				it = str.erase(it);
			}
		}
		else{
			it++;
		}
	}

	it--;
	while( (*it) == ' ' ){
		it = str.erase(it);
		it--;
	}
}

void Game::unique_filter_char( string & str ){
	if( str.length() == 0 ){ return; }

	string::iterator it=str.begin(), it2;
	while( it!=str.end() ){
		if( (*it) == '&' ){
			it = str.erase(it);
			it2 = it;
			while( it2!=str.end() && (*it2)!=';' && (*it2)!='&' ){it2++;}
			if( it2!=str.end() && (*it2)==';' ){
				while( it!=str.end() && (*it)!=';' ){
					it = str.erase(it);
				}
				it = str.erase(it);
			}
		}
		else if( (*it) == ' ' || (*it) > 0x7A || (*it) < 0x30 || ( (*it) > 0x39 && (*it) < 0x41 ) || ( (*it) > 0x5A && (*it) < 0x61 ) ){
			it = str.erase(it);
		}
		else{
			it++;
		}
	}
}

void Game::set_name( string name ){
	_name = name;
	filter_char(_name);
	_unique_name = _name;
	unique_filter_char(_unique_name);
}


void Game::update(){
	_db = MySQL_Database(DB);
	if( !_db.connect(HOST,PORT,USER,PASS) ){
		cout << "Failed to connect." << endl;
		exit(0);
	}

	ostringstream ss;

	map<string,string> column_to_values;
	map<string,string> column_to_values2;

	
	column_to_values[COL_NAME] = _name;
	column_to_values[COL_UNIQUENAME] = _unique_name;

	for( set<Price *>::iterator it=_prices.begin(); it!=_prices.end(); it++ ){
		ss.str("");
		ss << (*it)->price();
		column_to_values[(*it)->vendor()] = ss.str();
		column_to_values2[(*it)->vendor()] = ss.str();
		column_to_values[(*it)->key()] = (*it)->key_value();
		column_to_values2[(*it)->key()] = (*it)->key_value();
	}

	/*time_t today;
	struct tm * timeinfo;
	time ( &today );
	timeinfo = localtime ( &today );
	string time_string = "";
	ss.str("");
	ss << timeinfo->tm_year+1900 << "-" << timeinfo->tm_mon+1 << "-" << timeinfo->tm_mday;
	column_to_values[COL_DATE] = ss.str();*/

	_db.insert_update(TABLE,column_to_values,column_to_values2);
	
	_db.close();
}

void Game::clear_deals(){
	time_t today;
	struct tm * timeinfo;
	time ( &today );
	today -= 7*24*60*60;
	timeinfo = localtime ( &today );
	string time_string = "";
	ostringstream ss;
	ss << timeinfo->tm_year+1900 << "-" << timeinfo->tm_mon+1 << "-" << timeinfo->tm_mday;
	
	string conditions = "WHERE `date` < '";
	conditions += ss.str();
	conditions += "'";

	MySQL_Database db = MySQL_Database(DB);
	if( !db.connect(HOST,PORT,USER,PASS) ){
		cout << "Failed to connect." << endl;
		exit(0);
	}
	db.erase( "steam_frontpage",conditions );
	db.close();
	//mysql_query("DELETE FROM `swtprices`.`steam_frontpage` WHERE `date` < '2012-01-22'");
}