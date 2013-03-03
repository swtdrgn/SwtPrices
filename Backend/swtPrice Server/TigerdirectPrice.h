#pragma once
#include "price.h"
#include <sstream>

class TigerdirectPrice :
	public Price{
public:
	TigerdirectPrice(void);
	~TigerdirectPrice(void);

	float msrp(){ return _msrp; }
	float price(){ return _price; }
	string vendor(){ return "Tigerdirect"; }
	string key(){ return "EdpNo"; }
	string key_value(){ ostringstream ss; ss << _id; return ss.str(); }

	void update_price(){}
	void set_id( int id ){ _id = id; }
	void set_msrp( float msrp ){ _msrp = msrp; }
	void set_price( float price ){ _price = price; };

private:
	float _msrp;
	float _price;
	int _id;

};

