#pragma once

#include <sstream>
#include <iostream>
#include <fstream>
#include <map>
#include <list>

using namespace std;

class Element{
public:
	Element(void){}
	~Element(void){}
	void clear(){ _tag = ""; _attribute.clear(); }
	void tag( string t ){ _tag = t; }
	string tag(){ return _tag; }
	string operator []( string name ){ map<string,string>::iterator it = _attribute.find(name); if( it != _attribute.end() ){ return it->second; } else return ""; }
	void add_attribute( string name, string value ){ if( value.length() == 0 ){ value = true; } if( name.length() > 0 ){ _attribute[name] = value; } }
	map<string,string> & attributes(){ return _attribute; }
	string content(){ return _content; }
	void add_content( string s ){ _content += s; }
	void add_content( char c ){ _content += c; }

private:
	map<string,string> _attribute;
	string _tag;
	string _content;
};

class XMLParser{
public:
	static const char STREAM = 0x01;
	typedef char TAG_TYPE;
	static const TAG_TYPE CLOSE_TAG = 0x01, OPEN_TAG = 0x02;
public:
	XMLParser(void);
	XMLParser( char * file_path, char parse_method = STREAM );
	~XMLParser(void);

	void next();
	TAG_TYPE next_tag( string search_tag = "", Element * e = NULL );
	Element & current(){ return _tag_stack.front(); }

	int depth(){ return _tag_stack.size(); }

	bool eof(){ return _in.eof(); }
	list<Element> _tag_stack;
	string content(){ return _content; }

private:
	ifstream _in;
	bool _is_script;
	string _content;
};



