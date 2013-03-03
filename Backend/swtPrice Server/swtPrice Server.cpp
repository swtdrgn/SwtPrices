// swtPrice Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Amazon.h"
#include "Steam.h"
#include "Tigerdirect.h"
#include "Database.h"
#include "GameList.h"
#include "SHA1.h"
#include "HMAC_SHA1.h"
#include "hmacsha256.h"
#include "XMLParser.h"
//#include "hmac-sha256.c"
//#include "hmac.h"
//#include "sha.h"

//#include "dll.h"
//#include "hmac.h"
//#include "sha.h"

//using namespace CryptoPP;

//#include "sha.h"
//#include "hmac.h"


//char base64_set[];
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
		//cout << "int: " << (int) b[i] << endl;
		//cout << "char: " << b[i] << endl;
	}
	cout << "DONE!" << endl;
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
	cout << endl;
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

void url_encode( string & signature ){
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
}

int _tmain(int argc, _TCHAR* argv[]){

	/*string s = "helloworld";
	uint8_t * out;
	int size = hmac::char_base64((char*)s.c_str(),out);
	cout << "hex: ";
	hmac::print_hex(out,size);
	cout << endl << "base64: " << hmac::base64_char(out,size) << endl;*/

	
	/*int in_pos;
	string request = "GET\nwebservices.amazon.com\n/onca/xml\nAWSAccessKeyId=00000000000000000000&ItemId=0679722769&Operation=ItemLookup&ResponseGroup=ItemAttributes%2COffers%2CImages%2CReviews&Service=AWSECommerceService&Timestamp=2009-01-01T12%3A00%3A00Z&Version=2009-01-06";
	string key = "1234567890";
	//string request = "...";
	//string key = "mykey";
	string sign = hmac::signature_sha256(key,request);
	url_encode(sign);
	if((in_pos=sign.find("%3D"))==string::npos||in_pos<sign.length()-3){
		sign += "%3D";
	}

	cout << "signature: " << sign << endl;

	if( !sign.compare("cF3UtjbJb1%2BxDh387C%2FEmS1BCtS%2FZ01taykBCGemvUU%3D") ){
		cout << "EQUAL!" << endl;
	}*/

	/*string sign = "p/j5jKQjJ7O+/k/4/P9zAt9470aRBB3WTUKEpAv6RiE";
	url_encode(sign);
	cout << sign << endl;*/

	//Amazon * v = new Amazon();
	//v->get_game_prices();

	//Amazon * v = new Amazon();
	//v->get_game_prices2();

	Vendor * v = new Steam();
	v->get_prices();
	Game::clear_deals();

	//Vendor * v = new Tigerdirect();
	//v->get_prices();
	//((Tigerdirect *)v)->test();

	/*stringstream ss;
	ss << "Testing this";

	char c;
	for( ss.get(c); !ss.eof(); ss.get(c) ){
		cout << c;
	}
	cout << endl;*/

	/*string s = "!--";
	if( s.rfind("--",0) == string::npos ){
		cout << "Failed." << endl;
	}
	else{
		cout << "Win: " << s.rfind("--",0) << endl;
	}

	return 0;*/

	//XMLParser xml("pages/tigerdirect_prices.html");

	/*cout << "<" << xml.current().tag();
	map<string,string> & attribute = xml.current().attributes();
	for( map<string,string>::iterator it = attribute.begin(); it != attribute.end(); it++ ){
		cout << " " << it->first << "=\"" << it->second << "\"";
	}
	cout << ">" << endl;*/
	
	//while( !xml.eof() ){
		//xml.next_tag();

		/*if( !xml.eof() ){
			for( int i=1; i<xml.depth() ; i++ ){
				cout << "\t";
			}
			cout << "<" << xml.current().tag();

			map<string,string> & attribute = xml.current().attributes();
			for( map<string,string>::iterator it = attribute.begin(); it != attribute.end(); it++ ){
				cout << " " << it->first << "=\"" << it->second << "\"";
			}
			cout << ">" << endl;
		}*/
	//}
	
	/*for( list<Element>::iterator it = xml._tag_stack.begin(); it != xml._tag_stack.end(); it++ ){
		cout << (*it).tag() << endl;
	}

	cout << "Size of Tag Stack: " << xml._tag_stack.size() << endl;*/
}
