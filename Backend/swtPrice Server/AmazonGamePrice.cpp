#include "StdAfx.h"
#include "AmazonGamePrice.h"
#include <sstream>
#include <time.h>

#define HOST "localhost"
#define PORT 3306
#define USER "INSERT USERNAME"
#define PASS "INSERT PASSWORD"
#define DB "swtprices"
#define TABLE "Amazon"

#define COL_UNIQUENAME "uniquename"
#define COL_NAME "name"
#define COL_DP "dp"
#define COL_PRICE "price"
#define COL_PLATFORM "platform"
#define COL_MSRP "MSRP"
#define COL_DATE "date"

AmazonGamePrice::AmazonGamePrice(void){ reset(); }


AmazonGamePrice::~AmazonGamePrice(void)
{
}

void AmazonGamePrice::reset(){
	_msrp = 0;
	_price = 0;
	/*_vendor = "Amazon";
	_dl_price = 0;
	_pc_price = 0;
	_xbox360_price = 0;
	_ps3_price = 0;
	_dl_msrp = 0;
	_pc_msrp = 0;
	_xbox360_msrp = 0;
	_ps3_msrp = 0;*/
	_game = NULL;
}

void AmazonGamePrice::update_price(){
	_db = MySQL_Database(DB);
	while( !_db.connect(HOST,PORT,USER,PASS) ){
		cout << "Database failed to connect." << endl;
		cout << "Connecting again in 10 seconds..." << endl;
		Sleep(1000*10);
		//exit(0);
	}

	MySQL_Result result;
	ostringstream ss;

	ss << "WHERE  `" COL_DP "` =  '" << _key << "' AND  `" COL_PLATFORM "` =  '" << _platform << "' ORDER BY  `" << COL_DATE << "` DESC LIMIT 0 , 1";
	
	_db.select(TABLE,ss.str(),result);

	string tunique_name,tname;
	string tid,platform;
	float tprice,tmsrp;
	
	MySQL_Row row;
	while( result.more() ){
		result >> row;
		row >> tunique_name >> tname >> tid >> platform >> tmsrp >> tprice;
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

	//cout << "Inserting new row..." << endl;

	cout << "NAME: " << _game->name() << endl;
	cout << "UNIQUE NAME: " << _game->unique_name() << endl;;

	upload_record();

	map<string,string> column_to_values;
	
	column_to_values[COL_UNIQUENAME] = _game->unique_name();
	column_to_values[COL_NAME] = _game->name();

	column_to_values[COL_DP] = _key;
	column_to_values[COL_PLATFORM] = _platform;

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
	_db.close();
}
