#pragma once

#include <string>
#include <iostream>
#include "MySQL_Database.h"

using namespace std;
using namespace swtDB;

class Price
{
public:
	Price(void);
	~Price(void);

	virtual float msrp() = 0;
	virtual string vendor() = 0;
	virtual float price() = 0;
	virtual void update_price() = 0;
	virtual string key() = 0;
	virtual string key_value() = 0;

	virtual void set_msrp( float msrp ) = 0;
	virtual void set_price( float price ) = 0;
};

