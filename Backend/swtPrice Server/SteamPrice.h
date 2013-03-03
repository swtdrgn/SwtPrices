#pragma once
#include "Price.h"
#include "Game.h"
#include <sstream>

class SteamPrice :
	public Price
{
public:
	SteamPrice(void);
	~SteamPrice(void);

	void reset();
	float msrp(){ return _msrp; }
	float price(){ return _price; }
	string vendor(){ return "Steam"; }
	string key(){ return "appid"; }
	string key_value(){ ostringstream ss; ss << _id; return ss.str(); }

	void update_price();
	void fetch_price();

	void set_appid( int id ){ _id = id; }
	void set_msrp( float msrp ){ _msrp = msrp; }
	void set_price( float price ){ _price = price; };
	void set_game( Game * g ){ _game = g; g->add_price(this); }

private:
	MySQL_Database _db;
	Game * _game;
	int _id;
	string _name;
	float _price;
	float _msrp;
};

