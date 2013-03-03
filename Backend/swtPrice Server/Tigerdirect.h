#pragma once
#include "vendor.h"
#include "TigerdirectPrice.h"
#include "MySQL_Database.h"
#include <time.h>

class Tigerdirect :
	public Vendor{
public:
	static const char * ADDRESS;
	static const char * QUERY_PAGES;

	Tigerdirect(void);
	~Tigerdirect(void);

	void get_prices();
	string directory_path(string link,string current="");
	void url_decode(string & link);
	string remove_alpha(string s);


	void test();
	void update_price( long id, string name, float price );

private:
	MySQL_Database _db;
};

