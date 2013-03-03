#include "StdAfx.h"
#include "Steam.h"
#include "GameList.h"
#include "SteamPrice.h"
#include <sstream>


const char * Steam::ADDRESS = "store.steampowered.com";
const char * Steam::QUERY_PAGES = "/search/?category1=998&page=";
const string
	Steam::search_game = "href=\"http://store.steampowered.com/app/",
	Steam::search_name = "class=\"col search_name ellipsis\"",
	Steam::search_price = "<div class=\"col search_price\">",
	Steam::search_original = "<strike>",
	Steam::search_currency = "&#36;",
	Steam::search_max_page = "class=\"search_pagination_right\">";

Steam::Steam(void){
	_output_path = "pages/steam_prices.html";
}


Steam::~Steam(void){
}

void Steam::get_prices(){
	
	int max_page = 1,cpage = 1;
	stringstream ss;
	while(cpage <= max_page){
		if( connect(ADDRESS) ){
			string in, in2;
			streampos stream_pos;
			int in_pos, in_endpos;


			ss.str("");
			ss << QUERY_PAGES << cpage;

			string file_path = ss.str();

			send_receive(file_path);
			close();

			GameList gl;

			Game g("");
			SteamPrice p;
			int game_id;
			string game_name;
			float price, msrp;

			ifstream in_file (_output_path);
			for( in_file >> in; !in_file.eof(); in_file >> in ){
				if((in_pos = in.find(search_game)) != string::npos){
					in.erase(in_pos,search_game.length());
					in_endpos = in.find("/?");
					game_id = atoi(in.substr(in_pos,in_endpos).c_str());

					while( (in_pos = in.find("</a>")) == string::npos ){
						if( ( in_pos = in.find(search_name) ) != string::npos ){
							game_name = read_game_name(in_file);
						}
						else if( ( in_pos = in.find(search_price) ) != string::npos ){
							read_price(in_file,in,in_pos,price,msrp);
						}

						if( in_file.peek() == ' ' ){
							in_file >> in2;
							in += " " + in2;
						}
						else{
							in_file >> in;
						}
					}
					in.erase(in_pos,4);

					g.set_name(game_name);
					g.set_price(price);

					p.reset();
					p.set_game(&g);
					p.set_appid(game_id);
					p.set_price(price);
					p.set_msrp(msrp);

					p.update_price();
				}
				else if((in_pos = in.find(search_max_page)) != string::npos){
					in.erase(0,in_pos+search_max_page.length());
					max_page = read_number_of_pages(in_file,in);
				}
			}
			in_file.close();
			cout << "CURRENT PAGE: " << cpage << "/" << max_page << endl;
			cpage++;
		}
		else{
			cout << "Failed to connect to Steam's store page." << endl;
			cout << "Reconnecting in one minute." << endl;
			Sleep(1000*60*1);
		}
	}
}

int Steam::read_number_of_pages( ifstream &in_file, string & in ){
	int in_pos,end_pos,max_page=1,t;
	string int_string;
	while(true){
		if((in_pos = in.find("<a")) != string::npos){
			while((end_pos = in.find(">")) == string::npos){ in_file >> in; } //trap
			in.erase(0,end_pos+1);
			while((in_pos = in.find("</a>")) == string::npos){}
			int_string = in.substr(0,in_pos);
			t = atoi(int_string.c_str());
			if( t>max_page ){ max_page = t; }
			in.erase(0,in_pos+4);
		}
		if( (in_pos = in.find("</div>")) != string::npos ){
			break;
		}
		in_file >> in;
	}
	return max_page;
}


void Steam::read_price( ifstream &in_file, string &in, int in_pos, float & price, float & msrp ){
	string in2;
	int in_endpos;
	read_line(in_file,in);
	in.erase(in_pos,search_price.length());
						
	if( ( in_endpos = in.find("</div>") ) != string::npos ){
		in2 = in.substr(0,in_endpos);
	}
	else{
		in2 = in;
	}

	if( ( in_pos = in2.find("<br>") ) != string::npos ){
		in2 = in2.substr(in_pos+4);
	}

	if( ( in_pos = in2.find(search_currency) ) != string::npos ){
		in2.erase(in_pos,search_currency.length());
	}

	price = atof(in2.c_str());

	if( ( in_pos = in.find(search_original) ) != string::npos ){
		in2 = in.substr(in_pos+search_original.length());
		if( ( in_pos = in2.find("</strike>") ) != string::npos ){
			in2 = in2.substr(0,in_pos);
			if( ( in_pos = in2.find(search_currency) ) != string::npos ){
				in2.erase(in_pos,search_currency.length());
				msrp = atof(in2.c_str());
			}
		}
	}
	else{
		msrp = price;
	}

	in.erase(0,in_endpos+6);
}

string Steam::read_game_name( ifstream & in_file ){
	string game_name = "", in;
	int in_pos, in_endpos;

	in_file >> in;
	in_pos = in.find("<h4>");
	in.erase(in_pos,4);

	// Precondition check to avoid having extra spaces at the end of game end.
	if( (in_endpos = in.find("</h4>")) == string::npos ){
		game_name = in;
		in_file >> in;
	}
	else{ // Finish early to avoid having spaces.
		in.erase(in_endpos,5);
		return in;
	}

	while( in.length() < 5 || (in_endpos = in.find("</h4>")) == string::npos ){
		game_name += " " + in;
		in_file >> in;
	}

	in.erase(in_endpos,5);
	if( in.length() > 0 ){
		game_name += " " + in;
	}

	return game_name;
}

