#pragma once
#include "mysql_database.h"
#include "Game.h"

using namespace swtDB;

class GameList{
public:
	GameList(void);
	~GameList(void);

	void update_game( Game & game );
	void print_games();

private:
	MySQL_Database _db;
	void filter_char( string & str );
	void unique_filter_char( string & str );
};

