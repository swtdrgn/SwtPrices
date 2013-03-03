#include "StdAfx.h"
#include "GameList.h"
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
#define COL_PRICE "price"
#define COL_DATE "date"
#define COL_VENDOR "vendor"

GameList::GameList(void)
{
}


GameList::~GameList(void)
{
}

void GameList::update_game( Game & game ){
	_db = MySQL_Database(DB);
	if( !_db.connect(HOST,PORT,USER,PASS) ){
		cout << "Failed to connect." << endl;
		exit(0);
	}

	ostringstream ss;

	map<string,string> column_to_values;

	
	column_to_values[COL_NAME] = game.name();
	column_to_values[COL_UNIQUENAME] = game.unique_name();

	ss << game.price();
	column_to_values[COL_PRICE] = ss.str();

	time_t today;
	struct tm * timeinfo;
	time ( &today );
	timeinfo = localtime ( &today );
	string time_string = "";
	ss.str("");
	ss << timeinfo->tm_year+1900 << "-" << timeinfo->tm_mon+1 << "-" << timeinfo->tm_mday;
	column_to_values[COL_DATE] = ss.str();

	_db.replace(TABLE,column_to_values);
	
	_db.close();
}


void GameList::print_games(){
	_db = MySQL_Database(DB);
	if( !_db.connect(HOST,PORT,USER,PASS) ){
		cout << "Failed to connect." << endl;
		exit(0);
	}

	MySQL_Result result;
	_db.select(TABLE,result);
	string game_name,date;
	float price;
	MySQL_Row row;
	while( result.more() ){
		game_name = "";
		result >> row;
		row >> game_name >> price >> date;
		cout << game_name  << endl << "$" << price << endl << date << endl << endl;
	}
	_db.close();
}

void GameList::filter_char( string & str ){
	if( str.length() == 0 ){ return; }

	string::iterator it=str.begin();
	while( it!=str.end() ){
		if( (*it) < 0x20 || (*it) > 0x7E ){
			str.erase(it);
		}
		else if( (*it) == '\'' ){
			str.insert(it,'\\');
			it+=2;
		}
		else{
			it++;
		}
	}

	it--;
	while( (*it) == ' ' ){
		str.erase(it);
		it--;
	}
}

void GameList::unique_filter_char( string & str ){
	//cout << "string: " << str << endl;
	if( str.length() == 0 ){ return; }

	string::iterator it=str.begin();
	while( it!=str.end() ){
		//cout << *it << endl;
		if( (*it) == '&' ){
			//cout << "Vroom." << endl;
			str.erase(it);
			while( it!=str.end() && (*it)!=';' ){
				//cout << *it << endl;
				str.erase(it);
			}
			if( (*it)==';' ){
				//cout << *it << endl;
				str.erase(it);
			}
		}
		else if( (*it) == ' ' || (*it) > 0x7A || (*it) < 0x30 || ( (*it) > 0x39 && (*it) < 0x41 ) || ( (*it) > 0x5A && (*it) < 0x61 ) ){
			//cout << "deleted." << endl;
			str.erase(it);
		}
		else{
			//cout << "++" << endl;
			it++;
		}
	}
}