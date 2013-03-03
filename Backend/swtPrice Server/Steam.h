#pragma once
#include "Vendor.h"
#include "Game.h"

class Steam :
	public Vendor
{
public:
	Steam(void);
	~Steam(void);

	void get_prices();

private:
	const static char * ADDRESS;
	const static char * QUERY_PAGES;
	static const string search_game, search_name, search_price, search_original, search_currency, search_max_page;

	DWORD WINAPI process_line( LPVOID param ){ return NULL; }
	string read_game_name( ifstream & in_file );
	void read_price( ifstream &in_file, string &in, int in_pos, float & price, float & msrp );
	int read_number_of_pages( ifstream &in_file, string & in );
};

