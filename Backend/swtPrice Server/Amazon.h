#pragma once
#include "Vendor.h"
#include "Game.h"
#include "AmazonGamePrice.h"
#include <WinCrypt.h>
#include <iostream>

using namespace std;

class Amazon :
	public Vendor{
public:
	Amazon(void);
	~Amazon(void);

	void get_prices(){}
	void get_game_prices();
	void get_game_prices2();

	LPSTR  encrypt( LPSTR message );

protected:
	bool send_receive( string file_path );

private:
	string& url_encode( string & signature );
};

