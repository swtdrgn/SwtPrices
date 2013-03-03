#include "StdAfx.h"
#include "Amazon.h"
#include <sstream>
#include <time.h>
#include <iomanip>
#include <wincrypt.h>
#include "hmacsha256.h"

#define ADDRESS "www.amazon.com"
//#define QUERY_PAGES "/s?rh=n%3A468642%2Cn%3A%2111846801%2Cn%3A229575%2Cn%3A12508701&page="
#define QUERY_PAGES "/s?rh=n%3A4924894011&page="
#define OPEN_DIV "<div"
#define CLOSE_DIV "</div>"
#define CLASS_DATA "class=\"data\""
#define OPEN_A "<a"
#define CLOSE_A "</a>"
#define CLASS_TITLE "class=\"title\""
#define CLASS_TP "tp"
#define CLASS_TP_TYPE "class=\"tpType\""
#define CLASS_LIST_PRICE "class=\"toeListPrice\""
#define CLASS_AMAZON_PRICE "class=\"toeOurPrice\""
#define START_STRIKE "<strike>"
#define END_STRIKE "</strike>"
#define PC_DOWNLOAD "PC Download"
#define PC "PC"
#define XBOX360 "Xbox 360"
#define PS3 "PLAYSTATION 3"
#define DL_LABEL "[Download]"
#define MACDL_LABEL "[Mac Download]"
#define NAME_TAG "name=\""
#define NEXT_PAGE "title=\"Next"


Amazon::Amazon(void){
	_output_path = "pages/amazon.html";
}


Amazon::~Amazon(void){
}

void Amazon::get_game_prices(){
	int max_page = 1,cpage = 1,div,a_count,flag;
	stringstream ss;
	string file_path, in, in2;
	streampos stream_pos;
	int in_pos, in_endpos;
	while(cpage <= max_page && cpage < 300){
		if( connect(ADDRESS) ){
			ss.str("");
			ss << QUERY_PAGES << cpage;
			file_path = ss.str();

			cout << "GETTING: " << file_path << endl;

			send_receive(file_path);

			cout << "ENDED!" << endl;

			close();

			AmazonGamePrice p, * gptr;

			Game g("");
			//AmazonPrice p;
			string game_name, game_id;
			//float dl_price,pc_price,xbox360_price,ps3_price,dl_price_ret,pc_price_ret,xbox360_price_ret,ps3_price_ret,msrp;

			AmazonDLPrice dl_price;
			AmazonPCPrice pc_price;
			Amazon360Price xbox360_price;
			AmazonPS3Price ps3_price;
			bool dl,pc,x360,ps3;

			in = in2 = "";

			ifstream in_file (_output_path);
			for( in_file >> in; !in_file.eof(); in_file >> in ){
				if( (in_pos=in.find(NAME_TAG)) != string::npos ){
					in.erase(0,in_pos+sizeof(NAME_TAG)-1);
					in_pos=in.find("\"");
					game_id = in.substr(0,in_pos);
				}
				if( in.find(NEXT_PAGE) != string::npos ){
					max_page++;
				}
				//if( in.find(OPEN_DIV) != string::npos ){
				//	in_file >> in;
				//	if( (in_pos=in.find(CLASS_DATA)) == string::npos ){
				//		continue;
				//	}
				if( in.find(CLASS_DATA) != string::npos ){	
					//cout << "DIV!" << endl;
					div=0; a_count=0, flag=0;
					dl_price.reset();
					pc_price.reset();
					xbox360_price.reset();
					ps3_price.reset();
					dl=pc=x360=ps3=false;
					
					//in.erase(0,in_pos+sizeof(OPEN_DIV)-1);
					while(true){
						//cout << in << endl;
						//cout << div << endl;
						//cout << "TITLE!" << endl;

						/*while( (in_pos=in.find(CLOSE_DIV)) != string::npos ){
							div--;
							in.erase(in_pos,sizeof(CLOSE_DIV)-1);
						}*/

						/*if( (in_pos=in.find(OPEN_DIV)) != string::npos ){
							cout << "open DIV!!!!!!!!!!!" << endl;
							div++;
							in_file >> in;*/
							/*if( (in_pos=in.find(CLASS_TITLE)) != string::npos ){
								cout << "class title!!!!!!!!!!!" << endl;

								cout << in << endl;*/
								div = 1;
								if( (in_pos=in.find(OPEN_A)) != string::npos ){
									cout << "game_name!!!!!!!!!!!" << endl;
									/*if( div > 1 ){
										cout << "OH NOES!" << endl;
										exit(0);
									}*/
									in.erase(0,in_pos);

									

									while( (in_endpos = in.find(CLOSE_A)) == string::npos ){
										in_file >> in2;
										in += " " + in2;
									}
									in_pos=in.find(">");
									game_name = in.substr(in_pos+1,in_endpos-in_pos-1);
									in.erase(0,in_endpos+sizeof(CLOSE_A)-1);
								}
							//}
							else if( (in_pos=in.find(CLASS_TP)) != string::npos ){
								cout << "class TP!!!!!!!!!!!" << endl;
								in.erase(0,in_pos+sizeof(CLASS_TP)+1);
								gptr = NULL;
								do{
									if( (in_pos=in.find(CLASS_TP_TYPE)) != string::npos ){
										while( (in_endpos = in.find(CLOSE_A)) == string::npos ){
											in_file >> in2;
											in += " " + in2;
										}

										if( in.find(PC_DOWNLOAD) != string::npos ){
											//cout << "PCDOWNLOAD" << endl;
											//price = &dl_price;
											//retail_price = &dl_price_ret;
											gptr = &dl_price;
											dl = true;
										}
										else if( in.find(PC) != string::npos ){
											//cout << "PC" << endl;
											//price = &pc_price;
											//retail_price = &pc_price_ret;
											gptr = &pc_price;
											pc = true;
										}
										else if( in.find(XBOX360) != string::npos ){
											//cout << "XBOX" << endl;
											//price = &xbox360_price;
											//retail_price = &xbox360_price_ret;
											gptr = &xbox360_price;
											x360 = true;
										}
										else if( in.find(PS3) != string::npos ){
											//cout << "PS3" << endl;
											//price = &ps3_price;
											//retail_price = &ps3_price_ret;
											gptr = &ps3_price;
											ps3 = true;
										}
										else{
											//cout << "NONE" << endl;
										}
										in.erase(in_pos,in_endpos+sizeof(CLOSE_A)-in_pos);
									}
									if( (in_pos=in.find(CLASS_LIST_PRICE)) != string::npos ){
										//cout << in << endl;
										//cout << "I'm inSIDE" << endl;
										while( (in_endpos=in.find("</td>")) == string::npos ){
											in_file >> in2;
											in += " " + in2;
										}
										
										//exit(0);
										in2 = in.substr(in_pos,in_endpos); // string between class="toelistprice"...</td>
										in_pos=in2.find(">"); 
										in2.erase(0,in_pos+1); // string between >...</td>
										in_pos = in.find("<td");
										in.erase(in_pos,in_endpos+sizeof("</td>")-1);
										in_pos = in2.find("$");
										if( (in_endpos = in2.find(END_STRIKE)) && in_pos != string::npos ){
											in2 = in2.substr(in_pos+1,in_endpos-(in_pos+1));
										}
										//cout << in2 << endl;
										if( gptr != NULL ){
											//*retail_price = atof(in2.c_str());
											gptr->set_msrp(atof(in2.c_str()));
										}
										else{
											//cout << in2 << endl;
										}
									}
									if( (in_pos=in.find(CLASS_AMAZON_PRICE)) != string::npos ){
										//cout << in << endl;
										cout << "I'm inSIDE" << endl;
										while( (in_endpos=in.find("</td>")) == string::npos ){
											in_file >> in2;
											in += " " + in2;
										}
										
										//exit(0);
										in2 = in.substr(in_pos,in_endpos); // string between class="toeourprice"...</td>
										in_pos=in2.find(">"); 
										in2.erase(0,in_pos+1); // string between >...</td>
										in_pos = in.find("<td");
										in.erase(in_pos,in_endpos+sizeof("</td>")-1);
										in_pos = in2.find("$");
										if( (in_endpos = in2.find(END_STRIKE)) && in_pos != string::npos ){
											in2 = in2.substr(in_pos+1,in_endpos-(in_pos+1));
										}
										//cout << in2 << endl;
										if( gptr != NULL ){
											//*price = atof(in2.c_str());
											gptr->set_price(atof(in2.c_str()));
											if(gptr->price()>gptr->msrp()){
												gptr->set_msrp(gptr->price());
											}
										}
										else{
											//cout << in2 << endl;
										}
									}
									if( (in_pos=in.find("</td>")) != string::npos ){
										in.erase(0,in_pos+sizeof("</td>"));
									}
									if( (in_pos=in.find(OPEN_DIV)) != string::npos ){
										in.erase(0,in_pos+sizeof(OPEN_DIV));
										div++;
									}
									if( (in_endpos = in.find(CLOSE_DIV)) != string::npos ){
										div--;
										if( div<1){
											break;
										}
									}
									in_file >> in2;
									in += " " + in2;

								} while( true );
								if( (in_pos=game_name.find(DL_LABEL)) != string::npos ){
									game_name.erase(in_pos,sizeof(DL_LABEL));
								}
								else if( (in_pos=game_name.find(MACDL_LABEL)) != string::npos ){
									game_name.erase(in_pos,sizeof(MACDL_LABEL));
								}
								

								div--;
							}
							//in.erase(in_pos,sizeof(OPEN_DIV)-1);
						//}

						
						if( div < 0 ){
							//cout << "BREAK!" << endl;
							break;
						}
						
						in_file >> in;
					}

					cout << "GAME NAME: " << game_name << endl;

					g.set_name(game_name);
					//cout << g.name() << " (" << g.unique_name() << ")(" << game_id << ")" << endl;

					dl_price.set_game(&g);
					pc_price.set_game(&g);
					xbox360_price.set_game(&g);
					ps3_price.set_game(&g);
					dl_price.set_key(game_id);
					pc_price.set_key(game_id);
					xbox360_price.set_key(game_id);
					ps3_price.set_key(game_id);
					/*cout << "DL: " << dl_price.msrp() << " -> " << dl_price.price() << endl;
					cout << "PC: " << pc_price.msrp() << " -> " << pc_price.price() << endl;
					cout << "XBOX360: " << xbox360_price.msrp() << " -> " << xbox360_price.price() << endl;
					cout << "PS3: " << ps3_price.msrp() << " -> " << ps3_price.price() << endl;*/
					if(dl){
						dl_price.update_price();
					}
					if(pc){
						pc_price.update_price();
					}
					if(x360){
						xbox360_price.update_price();
					}
					if(ps3){
						ps3_price.update_price();
					}
				}
			}
		}
		cpage++;
	}
}


#define ADDRESS "webservices.amazon.com"
#define QUERY_PAGES "/onca/xml"
#define QUERY_ID "AWSAccessKeyId"
#define QUERY_ASSOCIATE "AssociateTag"
#define SIGNATURE "Signature"

#define AWS_ACCESS_KEY_ID "AKIAIMFAUBXVE76NCUUQ"
#define SECRET_KEY "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"
#define ASSOCIATE_TAG "swtprices-20"

#define SERVICE "Service"
#define COMMERCE_SERVICE "AWSECommerceService"
#define OPERATION "Operation"
#define OP_ITEMSEARCH "ItemSearch"
#define SEARCH_INDEX "SearchIndex"
#define QUERY_KEYWORDS "Keywords"
#define QUERY_TIME "Timestamp"

void Amazon::get_game_prices2(){

	int max_page = 1,cpage = 1;
	stringstream ss;
	//while(cpage <= max_page){
		cout << ADDRESS << endl;
		if( connect(ADDRESS) ){
			map<string,string> parameters;
			string in, in2;
			streampos stream_pos;
			int in_pos, in_endpos;
			time_t ctime; time ( &ctime );
			struct tm * ptm; ptm = gmtime ( &ctime );
			map<string,string>::iterator it;

			parameters[QUERY_ID] = AWS_ACCESS_KEY_ID;
			parameters[QUERY_ASSOCIATE] = ASSOCIATE_TAG;
			parameters[SERVICE] = COMMERCE_SERVICE;
			parameters[OPERATION] = OP_ITEMSEARCH;
			parameters[SEARCH_INDEX] = "Apparel";
			parameters[QUERY_KEYWORDS] = "Shirt";

			ss.str("");
			ss << setfill('0') << setw(4) << ptm->tm_year+1900 << "-" << setw(2) << ptm->tm_mon+1 << "-" << setw(2) << ptm->tm_mday << "T" << setw(2) << ptm->tm_hour << "%3A" << setw(2) << ptm->tm_min << "%3A" << setw(2) << ptm->tm_sec << "Z";
			parameters[QUERY_TIME] = ss.str();

			ss.str("");
			it=parameters.begin();
			ss<<it->first<<"="<<it->second; it++;
			do{
				ss<<"&"<<it->first<<"="<<it->second;
			} while(++it!=parameters.end());

			string signature = "GET\nwebservices.amazon.com\n/onca/xml\n" + ss.str();
			//cout << signature << endl;
			signature = hmac::signature_sha256(SECRET_KEY,signature);
			if( *--signature.end() != '=' ){
				signature += "=";
			}

			//cout << signature << endl;
			url_encode(signature);

			//cout << signature << endl;
			

			ss << "&" << SIGNATURE << "=" << signature;

			string file_path = "/onca/xml?" + ss.str();
			//url_encode(file_path);
			send_receive(file_path);

			//ofstream output_html ("pages/amazon_test.txt");
			//output_html << "http://webservices.amazon.com/onca/xml?" << file_path << endl;
			//output_html.close();

			close();
		}
	//}
}


string& Amazon::url_encode( string & signature ){
	for(string::iterator it=signature.begin();it!=signature.end();){
		if(*it=='+'){
			//*it = '%';
			//it++;
			it = signature.erase(it);
			it = signature.insert(it,'B');
			it = signature.insert(it,'2');
			it = signature.insert(it,'%');
			it += 3;
		}
		else if(*it=='='){
			it = signature.erase(it);
			it = signature.insert(it,'D');
			it = signature.insert(it,'3');
			it = signature.insert(it,'%');
			it += 3;
		}
		else if(*it==','){
			it = signature.erase(it);
			it = signature.insert(it,'C');
			it = signature.insert(it,'2');
			it = signature.insert(it,'%');
			it += 3;
		}
		else if(*it==':'){
			it = signature.erase(it);
			it = signature.insert(it,'A');
			it = signature.insert(it,'3');
			it = signature.insert(it,'%');
			it += 3;
		}
		else if(*it=='/'){
			it = signature.erase(it);
			it = signature.insert(it,'F');
			it = signature.insert(it,'2');
			it = signature.insert(it,'%');
			it += 3;
		}
		else{it++;}
	}
	return signature;
}

bool Amazon::send_receive( string file_path ){

	//string request = "GET " + file_path + "\r\n\r\n";
	string request = "GET " + file_path + " HTTP/1.1\r\n";
	request += "Host: " + _hostname + "\r\n";
	//request += "Content-Type: text/plain;charset=UTF-8\r\n\r\n";
	request += "Connection: close\r\n";
	request += "Accept-Charset: UTF-8;q=0.7,*;q=0.7\r\n";
	request += "Cache-Control: no-cache\r\n";
	request += "Accept-Language: en;q=0.7,en-us;q=0.3\r\n\r\n";

	cout << request << endl;

	ofstream output_html (_output_path);
	output_html << request << endl;

	if (send(_s,request.c_str(),request.length(),0) != request.length()){
		cout << "Fail to send." << endl;
		return false;
	}

	char buffer[RECV_BUFFERSIZE];
	int length=RECV_BUFFERSIZE;

	//ofstream output_html (_output_path);

	int i=0;
    while ( (length = recv(_s,&buffer[0], RECV_BUFFERSIZE, 0)) != 0 ){
		output_html.write(buffer,length);
	}

	output_html.close();
	//output_html << endl << endl;
	//output_html << request;
}

/*
char base64_set[] =	{	'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
						'a','b','c','d','e','f','g','h','i','j','k','l','n','m','o','p','q','r','s','t','u','v','w','x','y','z',
						'0','1','2','3','4','5','6','7','8','9','+','/' };

int base64_length( string s ){
	return ceil(s.length()*sizeof(char)*8./6.);
}

BYTE* to_base64( string s ){
	int size = base64_length(s);
	const char * cstr = s.c_str();
	BYTE * b = new BYTE [size];
	for( int i=0; i<size; i++ ){
		cout << "i: " << i << "/" << size << endl;
		cout << "ci: " << floor(i*6./8.) << "/" << s.length() << endl;
		if(i%4==0){
			b[i] = (cstr[i*6/8]&0xFC) >> 2;
		}
		else if(i%4==1){
			b[i] = ((cstr[(int)floor(i*6./8.)]&0x03)<<4) + ((cstr[(int)floor(i*6./8.)+1]&0xF0)>>4);
		}
		else if(i%4==2){
			b[i] = ((cstr[(int)floor(i*6./8.)]&0x0F)<<2) + ((cstr[(int)floor(i*6./8.)+1]&0xC0)>>6);
		}
		else if(i%4==3){
			b[i] = cstr[(int)floor(i*6./8.)]&0x3F;
		}
	}
	return b;
}

char* base64_char( BYTE * s, int size ){
	char * str = new char[size+1];
	for( int i=0; i<size; i++ ){
		if( ((char*)s)[i] > 63 ){
			cout << "AHHH!" << endl;
			exit(0);
		}
		str[i] = base64_set[((char*)s)[i]];
	}
	str[size] = 0;
	return str;
}

void print_hex( BYTE * b, int size ){
	for( int i=0 ; i<size ; i++ ){
	   printf("%2.2x ",b[i]);
	}
}

BYTE * char_base64( char * s ){
	int size = 0;
	while(s[size]!=0){ size++; }
	BYTE * b = new BYTE[size];
	for( int i=0; i<size; i++ ){
		if( 'A' <= s[i] && s[i] <= 'Z' ){
			b[i] = s[i] - 'A';
		}
		else if( 'a' <= s[i] && s[i] <= 'z' ){
			b[i] = s[i] - 'a' + 26;
		}
		else if( '0' <= s[i] && s[i] <= '9' ){
			b[i] = s[i] - '0' + 52;
		}
		else if( s[i] == '+' ){
			b[i] = 62;
		}
		else if( s[i] == '/' ){
			b[i] = 63;
		}
	}
	return b;
}*/
