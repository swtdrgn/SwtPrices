#include "StdAfx.h"
#include "Tigerdirect.h"
#include "XMLParser.h"

#include <sstream>


#define HOST "localhost"
#define PORT 3306
#define USER "INSERT USERNAME"
#define PASS "INSERT PASSWORD"
#define DB "swtprices"
#define TABLE "tigerdirect_history"
#define TABLE2 "tigerdirect_frontpage"
#define TABLE3 "tigerdirect"

#define COL_UNIQUENAME "uniquename"
#define COL_NAME "name"
#define COL_ID "EdpNo"
#define COL_PRICE "price"
//#define COL_AVAIL "availability"
#define COL_DATE "date"
#define COL_OLDPRICE "oldprice"
//#define COL_OLDAVAIL "oldav"


const char * Tigerdirect::ADDRESS = "www.tigerdirect.com";
const char * Tigerdirect::QUERY_PAGES = "/applications/Category/guidedSearch.asp?CatId=17";
/*const string
	Tigerdirect::search_game = "href=\"http://store.steampowered.com/app/",
	Tigerdirect::search_name = "class=\"col search_name ellipsis\"",
	Tigerdirect::search_price = "<div class=\"col search_price\">",
	Tigerdirect::search_original = "<strike>",
	Tigerdirect::search_currency = "&#36;",
	Tigerdirect::search_max_page = "class=\"search_pagination_right\">";*/

Tigerdirect::Tigerdirect(void){
	_output_path = "pages/tigerdirect_prices.html";
}

Tigerdirect::~Tigerdirect(void)
{
}


void Tigerdirect::get_prices(){
	_output_path = "pages/tigerdirect_prices.html";

	long id;
	string name;
	float price;
	bool avail;

	int max_page = 1,cpage = 1;
	stringstream ss;
	while(cpage <= max_page){
		if( connect(ADDRESS) ){
			string in, in2;
			streampos stream_pos;
			int in_pos, in_endpos;


			ss.str("");
			ss << QUERY_PAGES;
			//ss << QUERY_PAGES << cpage;

			string file_path = ss.str();

			send_receive(file_path);
			close();

			list<string> links;
			Element * element;

			XMLParser xml("pages/tigerdirect_prices.html");

			xml.next_tag("ul");
			while(xml.current()["id"].compare("header-nav") != 0){
				xml.next_tag("ul");
			}
			
			element = &xml.current();

			cout << xml.current().tag() << endl;
			cout << xml.current()["id"] << endl;

			int nested_depth = xml.depth();

			//int i = 0;
			string href;
			for( char rtag = xml.next_tag("a",element); !xml.eof() && rtag != XMLParser::CLOSE_TAG; rtag = xml.next_tag("a",element) ){
				href = xml.current()["href"];
				if( (in_pos=href.find("/applications/category/category_tlc.asp?CatId=")) != string::npos ){
					links.push_back(href);
					cout << href << endl;
				}
				else if( (in_pos=href.find("/applications/Category/guidedSearch.asp?CatId=")) != string::npos ){
					links.push_back(href);
					cout << href << endl;
				}
				//cout << i << endl;
				//i++;
			}


			cout << endl;
			cout << endl;
			cout << endl;
			cout << endl;
			cout << endl;
			cout << endl;
			cout << endl;
			cout << endl;
			cout << endl;


			_output_path = "pages/tigerdirect_page.html";
			list<string> links2;
			for( list<string>::iterator it=links.begin(); it!=links.end(); it++ ){
				links2.clear();

				if( (in_pos=(*it).find("/applications/category/category_tlc.asp?CatId=")) != string::npos ){
					(*it) += "&Lprice=0.01";
					in_pos = (*it).find("category_tlc");
					if( in_pos >= 0 ){
						(*it).replace(in_pos,12,"category_slc");
					}
					links2.push_back(*it);
				}
				else{
					while( !connect(ADDRESS) ){}
					ss.clear();
					ss.str("");
					ss << *it;
					//ss << QUERY_PAGES << cpage;

					cout << "IT: " << *it << endl;
					cout << ss.str() << endl << endl;
					string file_path2 = ss.str();

					send_receive(file_path2);
					close();

					string directory = directory_path(file_path2);


					cout << "Parsing tigerdirect_page.html..." << endl;

					Element * element2;

					XMLParser xml("pages/tigerdirect_page.html");

					xml.next_tag("div");
					while(xml.current()["class"].compare("sideNav") != 0){
						xml.next_tag("div");
					}
			
					element2 = &xml.current();

					cout << xml.current().tag() << endl;
					cout << xml.current()["id"] << endl;

					int nested_depth = xml.depth();


					//int i = 0;
					string href;
					for( char rtag = xml.next_tag("a",element2); !xml.eof() && rtag != XMLParser::CLOSE_TAG; rtag = xml.next_tag("a",element2) ){
						href = xml.current()["href"];

						if( (in_pos=href.find("sel=Price%3BPrice")) != string::npos ){
							if( href[0] != '/' ){
								links2.push_back(directory+"/"+href);
							}
							else{
								links2.push_back(href);
							}
							cout << href << endl;
						}
						/*else if( (in_pos=href.find("applications/category/category_slc.asp?")) != string::npos ){
							if( href[0] != '/' ){
								links2.push_back(directory+"/"+href);
							}
							else{
								links2.push_back(href);
							}
							cout << href << endl;
						}*/
					}
				}

				for( list<string>::iterator it2=links2.begin(); it2!=links2.end(); it2++ ){
					while( !connect(ADDRESS) ){}
					bool has_more = false;

					ss.clear();
					ss.str("");
					ss << *it2;
					//ss << QUERY_PAGES << cpage;

					cout << "IT: " << *it2 << endl;
						
					string file_path3 = ss.str();
					url_decode(file_path3);

					cout << file_path3 << endl << endl;

					send_receive(file_path3);
					close();

					string directory = directory_path(file_path3);

					cout << "DIRECTORY: " << directory << endl << endl;
					cout << "22222222222 Parsing tigerdirect_page.html..." << endl;

					Element * element2;

					XMLParser xml("pages/tigerdirect_page.html");

					/*xml.next_tag("div");
					while(xml.current()["class"].compare("resultsWrap listView") != 0){
						xml.next_tag("div");
					}*/

					xml.next_tag("form");
					while(xml.current()["name"].compare("frmCompare") != 0){
						xml.next_tag("form");
					}

						
			
					element2 = &xml.current();

					//cout << xml.current().tag() << endl;
					//cout << xml.current()["id"] << endl;

					int nested_depth = xml.depth();


					cout << endl;
					cout << endl;
					cout << endl;
					cout << endl;
					cout << endl;
					cout << endl;
					cout << endl;
					cout << endl;
					cout << endl;

					//int i = 0;
					string href;
					for( char rtag = xml.next_tag("div",element2); !xml.eof() && rtag != XMLParser::CLOSE_TAG; rtag = xml.next_tag("div",element2) ){
						//cout << "GOT PRODUCT 01" << endl;
						//cout << xml.current()["class"] << endl;
						if( xml.current()["class"].compare("product") == 0 ){
							price = 0;
							id = 0;
							avail = false;
							name = "";
							//cout << "GOT PRODUCT 02" << endl;

							Element * element3 = &xml.current();
							rtag = xml.next_tag("",element3);
							while( !xml.eof() && rtag != XMLParser::CLOSE_TAG ){
								//cout << "GOT PRODUCT 03" << endl;
								if( xml.current().tag().compare("h3") == 0 && xml.current()["class"].compare("itemName") == 0 ){
									rtag = xml.next_tag("a");
									Element * element4 = &xml.current();
									while( rtag != XMLParser::CLOSE_TAG ){
										rtag = xml.next_tag("a",element4);
									}
									cout << "TITLE: " << xml.content() << endl;

									name = xml.content();
									//break;
								}
								else if( xml.current().tag().compare("div") == 0 && xml.current()["class"].compare("salePrice") == 0 ){
									price = 0;
									Element * element4 = &xml.current();
									rtag = xml.next_tag("span",element4);
									while( xml.current()["class"].compare("priceDecimalMark") != 0 && rtag != XMLParser::CLOSE_TAG ){
										rtag = xml.next_tag("span",element4);
									}
									if( rtag == XMLParser::CLOSE_TAG ){ continue; }
									Element * element5 = &xml.current();
									rtag = xml.next_tag("",element5);
									while( rtag != XMLParser::CLOSE_TAG ){
										rtag = xml.next_tag("",element5);
									}

									element5 = &xml.current();
									rtag = xml.next_tag("",element5);
									while( rtag != XMLParser::CLOSE_TAG ){
										rtag = xml.next_tag("",element5);
									}

									//cout << "DECIMAL: " << xml.content() << endl;
									price = atof(xml.content().c_str())/100.;
									rtag = xml.next_tag("",element4);
									while( rtag != XMLParser::CLOSE_TAG ){
										rtag = xml.next_tag("",element4);
									}
									//cout << "DOLLAR: " << xml.content() << endl;
									price += atof(remove_alpha(xml.content()).c_str());
									cout << "PRICE: " << price << endl;
								}
								else if( xml.current().tag().compare("strong") == 0 && xml.current()["class"].compare("stockMesg1") == 0 ){
									Element * element4 = &xml.current();
									rtag = xml.next_tag("",element4);
									while( rtag != XMLParser::CLOSE_TAG ){
										rtag = xml.next_tag("",element4);
									}

									cout << "Availability: " << xml.content() << endl;
								}
								else if( xml.current().tag().compare("input") == 0 && xml.current()["name"].compare("cmp") == 0 ){
									//cout << atol(xml.current()["id"].c_str()) << endl;
									id = atol(xml.current()["id"].c_str());
								}
									

									
								rtag = xml.next_tag("",element3);
							}
							update_price(id,name,price);
							cout << endl << endl << endl << endl << endl << endl << endl;
						}

						else if( !has_more && xml.current()["class"].compare("itemsPagination") == 0 ){
							Element * element3 = &xml.current();

							while( xml.current()["class"].compare("paginatNext") != 0 && rtag != XMLParser::CLOSE_TAG ){
								rtag = xml.next_tag("a",element3);
							}
							if( rtag != XMLParser::CLOSE_TAG ){
								has_more = true;
								links2.push_back(xml.current()["href"]);

								cout << "ADDED NEXT PAGE: " << xml.current()["href"] << endl;
							}
						}
					}

						
				}
				//exit(0);

			}

			cpage++;
		}
		else{
			cout << "Failed to connect to Tigerdirect." << endl;
			cout << "Reconnecting in one minute." << endl;
			Sleep(1000*60*1);
		}
	}
}


string Tigerdirect::directory_path(string link,string current){

	cout << "LINK: " << link << endl << endl;

	string ret = "";
	int pos = link.rfind("/");
	if( link[0] == '/' ){
		if( pos == -1 ){ return ""; }
		return link.substr(0,pos);
	}
	else{
		if( pos == -1 ){ return current; }
		ret = current + "/" + link.substr(0,pos);
	}
	return "";
}

void Tigerdirect::url_decode(string & link){
	int pos = link.find("&amp;");
	while( pos >= 0 ){
		link.replace(pos,5,"&");
		pos = link.find("&amp;");
	}
}

string Tigerdirect::remove_alpha(string s){
	string ret = "";
	for( string::iterator it=s.begin(); it!=s.end(); it++ ){
		if( '9' >= *it && *it >= '0' ){
			ret += *it;
		}
	}
	return ret;
}








void Tigerdirect::update_price( long id, string name, float price ){
	_db = MySQL_Database(DB);
	while( !_db.connect(HOST,PORT,USER,PASS) ){
		cout << "Database failed to connect." << endl;
		cout << "Connecting again in 10 seconds..." << endl;
		Sleep(1000*10);
		//exit(0);
	}

	MySQL_Result result;
	ostringstream ss;

	ss << "WHERE `" << COL_ID << "` = " << id << " ORDER BY  `" << COL_DATE << "` DESC LIMIT 0 , 1";

	_db.select(TABLE,ss.str(),result);

	string tunique_name,tname;
	long tid;
	float tprice=0,tmsrp;

	MySQL_Row row;
	while( result.more() ){
		result >> row;
		row >> tid >> tprice;
	}

	if( price < tprice + 0.001 && price > tprice - 0.001 ){
		_db.close();
		return;
	}

	//_game->update();

	map<string,string> column_to_values;

	//column_to_values[COL_UNIQUENAME] = _game->unique_name();
	//column_to_values[COL_NAME] = _game->name();

	ss.str("");
	ss << id;
	column_to_values[COL_ID] = ss.str();

	ss.str("");
	ss << price;
	column_to_values[COL_PRICE] = ss.str();

	//ss.str("");
	//ss << _msrp;
	//column_to_values[COL_MSRP] = ss.str();

	time_t today;
	struct tm * timeinfo;
	time ( &today );
	timeinfo = localtime ( &today );
	string time_string = "";
	ss.str("");
	ss << timeinfo->tm_year+1900 << "-" << timeinfo->tm_mon+1 << "-" << timeinfo->tm_mday;
	column_to_values[COL_DATE] = ss.str();

	_db.insert(TABLE,column_to_values);

	if( result.size() > 0 ){
		ss.str("");
		ss << tprice;
		column_to_values[COL_OLDPRICE] = ss.str();
	}

	column_to_values[COL_NAME] = name;

	_db.replace(TABLE2,column_to_values);
	
	_db.close();
}