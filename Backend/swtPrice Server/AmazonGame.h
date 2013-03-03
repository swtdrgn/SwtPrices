#pragma once
#include "game.h"
class AmazonGame :
	public Game
{
public:
	AmazonGame( string name );
	~AmazonGame(void);

	void record_price();

private:
	string _key;
};

