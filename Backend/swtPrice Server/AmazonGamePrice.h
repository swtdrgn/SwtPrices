#pragma once
#include "Price.h"
#include "Game.h"
class AmazonGamePrice :
	public Price
{
public:
	AmazonGamePrice(void);
	~AmazonGamePrice(void);

	void reset();
	virtual float msrp(){ return _msrp; }
	virtual string vendor(){ return "Amazon"; }
	virtual float price(){ return _price; }
	virtual string key(){ return "dp"; }
	virtual string key_value(){ return _key; }
	void update_price();
	virtual void set_game( Game * g ){ _game = g; }
	void set_platforum( string p ){ _platform = p; }

	/*void set_msrp( float msrp ){ _dl_msrp = msrp; }
	void set_price( float price ){ _dl_price = price; };
	void set_dl_price( float price ){ _dl_price = price; };
	void set_pc_price( float price ){ _pc_price = price; };
	void set_xbox360_price( float price ){ _xbox360_price = price; };
	void set_ps3_price( float price ){ _ps3_price = price; };
	void set_dl_msrp( float msrp ){ _dl_msrp = msrp; }
	void set_pc_msrp( float msrp ){ _pc_msrp = msrp; }
	void set_xbox360_msrp( float msrp ){ _xbox360_msrp = msrp; }
	void set_ps3_msrp( float msrp ){ _ps3_msrp = msrp; }
	void set_key( string key ){ _key = key; }*/

	void set_msrp( float msrp ){ _msrp = msrp; }
	void set_price( float price ){ _price = price; };
	void set_key( string key ){ _key = key; }

protected:
	string _platform;
	Game * _game;

	virtual void upload_record(){}

private:
	float _price,_msrp;
	//AmazonGamePrice * _dl,_pc,_xbox360,_ps3_price;
	//float _dl_msrp,_pc_msrp,_xbox360_msrp,_ps3_msrp;
	//float _dl_price,_pc_price,_xbox360_price,_ps3_price;
	string _key;
	MySQL_Database _db;
};

class AmazonDLPrice : public AmazonGamePrice{
public:
	AmazonDLPrice(){ _platform = "DL"; }
	~AmazonDLPrice(){}
	string vendor(){ return "AmazonDL"; }
	void set_game( Game * g ){ _game = g; g->add_price(this); }
protected:
	void upload_record(){ _game->update(); }
};

class AmazonPCPrice : public AmazonGamePrice{
public:
	AmazonPCPrice(){ _platform = "PC"; }
	~AmazonPCPrice(){}
	string vendor(){ return "AmazonPC"; }
	void set_game( Game * g ){ _game = g; g->add_price(this); }
protected:
	void upload_record(){ _game->update(); }
};

class Amazon360Price : public AmazonGamePrice{
public:
	Amazon360Price(){ _platform = "X360"; }
	~Amazon360Price(){}
	string vendor(){ return "AmazonXBOX360"; }
};

class AmazonPS3Price : public AmazonGamePrice{
public:
	AmazonPS3Price(){ _platform = "PS3"; }
	~AmazonPS3Price(){}
	string vendor(){ return "AmazonPS3"; }
};

