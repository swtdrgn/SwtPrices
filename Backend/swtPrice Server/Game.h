#pragma once
#include "item.h"
#include "price.h"
#include <set>

class Game :
	public Item{
public:
	Game( string name );
	~Game(void);

	// Title of the game
	string name(){ return _name; }
	void set_name( string name );
	float price(){ return _price; }

	void get_basic_info();

	void set_price( float price ){ _price = price; }
	void add_price( Price * price ){ _prices.insert(price); }
	void remove_price( Price * price ){ _prices.erase(price); }
	void set_vendor( string vendor ){ _cheapest_vendor = vendor; }
	string vendor(){ return _cheapest_vendor; }
	string unique_name(){ return _unique_name; }

	set<Price *>::iterator prices(){ return _prices.begin(); }
	bool more( set<Price *>::iterator it ){ return it != _prices.end(); }

	void update();

	static void clear_deals();

	//double msrp(){ return _price.msrp(); }
	//void set_steam_msrp( double price ){ _price = price; }

protected:
	string _name;
	string _unique_name;
	float _price;
	string _cheapest_vendor;
	bool _change;
	//Price * _price;
	set<Price *> _prices;
	MySQL_Database _db;

private:
	void filter_char( string & str );
	void unique_filter_char( string & str );
};

