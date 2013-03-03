#include "StdAfx.h"
#include "SteamPrice.h"
#include <time.h>

#define HOST "localhost"
#define PORT 3306
#define USER "INSERT USERNAME"
#define PASS "INSERT PASSWORD"
#define DB "swtprices"
#define TABLE "Steam"
#define TABLE2 "steam_frontpage"

#define COL_UNIQUENAME "uniquename"
#define COL_NAME "name"
#define COL_APPID "appid"
#define COL_PRICE "price"
#define COL_MSRP "MSRP"
#define COL_DATE "date"
#define COL_OLDPRICE "old_price"
#define COL_OLDMSRP "old_msrp"

SteamPrice::SteamPrice(void){ _game=NULL; reset(); }


SteamPrice::~SteamPrice(void){}


void SteamPrice::reset(){
	_id = 0;
	if(_game!=NULL){ _game->remove_price(this); }
	_game = NULL;
}

void SteamPrice::update_price(){
	_db = MySQL_Database(DB);
	while( !_db.connect(HOST,PORT,USER,PASS) ){
		cout << "Database failed to connect." << endl;
		cout << "Connecting again in 10 seconds..." << endl;
		Sleep(1000*10);
		//exit(0);
	}

	MySQL_Result result;
	ostringstream ss;

	ss << "WHERE `" << COL_APPID << "` = " << _id << " ORDER BY  `" << COL_DATE << "` DESC LIMIT 0 , 1";

	_db.select(TABLE,ss.str(),result);

	string tunique_name,tname;
	int tid;
	float tprice,tmsrp;

	MySQL_Row row;
	while( result.more() ){
		result >> row;
		row >> tunique_name >> tname >> tid >> tprice >> tmsrp;
	}

	/*
	cout << "this game's appid: " << _id << endl;

	cout << tunique_name << endl;
	cout << tname << endl;
	cout << tid << endl;
	cout << tprice << endl;
	cout << tmsrp << endl;
	*/

	/*if( result.size() == 0 ){
		cout << _game->name() << endl;
	}*/

	if( result.size() > 0 && _price < tprice + 0.001 && _price > tprice - 0.001 && _msrp < tmsrp + 0.001 && _msrp > tmsrp - 0.001 ){
		_db.close();
		return;
	}

	_game->update();

	map<string,string> column_to_values;

	column_to_values[COL_UNIQUENAME] = _game->unique_name();
	column_to_values[COL_NAME] = _game->name();

	ss.str("");
	ss << _id;
	column_to_values[COL_APPID] = ss.str();

	ss.str("");
	ss << _price;
	column_to_values[COL_PRICE] = ss.str();

	ss.str("");
	ss << _msrp;
	column_to_values[COL_MSRP] = ss.str();

	time_t today;
	struct tm * timeinfo;
	time ( &today );
	timeinfo = localtime ( &today );
	string time_string = "";
	ss.str("");
	ss << timeinfo->tm_year+1900 << "-" << timeinfo->tm_mon+1 << "-" << timeinfo->tm_mday;
	column_to_values[COL_DATE] = ss.str();

	_db.insert(TABLE,column_to_values);

	if( result.size() > 0 ){
		ss.str("");
		ss << tprice;
		column_to_values[COL_OLDPRICE] = ss.str();
		ss.str("");
		ss << tmsrp;
		column_to_values[COL_OLDMSRP] = ss.str();
	}

	_db.replace(TABLE2,column_to_values);
	
	_db.close();
}