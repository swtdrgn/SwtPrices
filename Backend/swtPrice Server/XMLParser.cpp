#include "StdAfx.h"
#include "XMLParser.h"
#include <list>
#include <stack>

XMLParser::XMLParser(void){}


XMLParser::~XMLParser(void)
{
}

XMLParser::XMLParser( char * file_path, char parse_method ){
	_is_script = false;
	if( parse_method == STREAM ){
		_in.open(file_path);
		next_tag();
		//next_tag("html");
	}
}

XMLParser::TAG_TYPE XMLParser::next_tag( string search_tag, Element * close_element ){
	string in;
	string tag;
	bool start_tag = false, close = false;
	char c;
	int endpos;
	_in >> c;
	
	Element element;
	while( !_in.eof() ){
		/* Open bracket is found. Set to "Open Bracket State". */
		if( c == '<' ){
			start_tag = true;
			close = false;
			tag = "";
		}
		else if( start_tag ){
			if( tag.length() == 0 ){
				if( isalpha(c) || c == '!' ){
					if( 'A' <= c && c <= 'Z' ){
						c += 'a'-'A';
					}
					tag += c;
				}
				else if( c == '/' ){
					close = true;
				}
				else{
					start_tag = false;
					close = false;
					tag = "";
				}
			}
			else if( ( !_is_script || (_is_script && close && tag.compare("script") == 0) ) && c == ' ' ){

				element.clear();
				element.tag(tag);

				bool in_quote = false;
				char quote;
				string attribute_name, attribute_value;
				string * name_value_ptr = &attribute_name;
				_in >> c;
					
				while( !_in.eof() ){
					if( in_quote ){
						if( c == quote ){
							in_quote = false;
							if( name_value_ptr == &attribute_value ){
								element.add_attribute(attribute_name,attribute_value);
								attribute_value = "";
							}
							attribute_name = "";
							name_value_ptr = &attribute_name;
						}
						else{
							*name_value_ptr += c;
						}
					}
					else{
						if( c == '"' || c == '\'' || c == '`' ){
							in_quote = true;
							quote = c;

							if( name_value_ptr == &attribute_name ){
								element.add_attribute(attribute_name,"true");
							}
						}
						else if( c == ' ' || c == '\n' ){
							if( name_value_ptr == &attribute_value ){
								element.add_attribute(attribute_name,attribute_value);
								attribute_value = "";
							}
							else if( attribute_name.length() > 0 ){
								element.add_attribute(attribute_name,"true");
							}
							attribute_name = "";
							name_value_ptr = &attribute_name;
						}
						else if( c == '>' ){
							start_tag = false;
							if(close){
								if( tag.compare("script") == 0 ){
									_is_script = false;
								}

								for( list<Element>::iterator it = _tag_stack.begin(); it != _tag_stack.end(); it++ ){
									if( (*it).tag().compare(tag) == 0 ){
										if( &(*it) == close_element ){
											_content = (*it).content();
											_tag_stack.erase(it);
											return CLOSE_TAG;
										}
										else{
											_tag_stack.erase(it);
										}
										break;
									}
								}
								break;
							}
							else{
								if( tag.compare("script") == 0 ){
									_is_script = true;
								}

								_tag_stack.push_front(element);

								if( search_tag.length() == 0 ){
									return OPEN_TAG;
								} else if( tag.compare(search_tag) == 0 ){
									return OPEN_TAG;
								}
								else{
									break;
								}
							}
								
						}
						else if( c == '=' ){
							name_value_ptr = &attribute_value;
							_in >> c;
							continue;
						}
						else if( c == '\r' ){}
						else{
							*name_value_ptr += c;
						}
					}
					_in.get(c);
				}
			}
			else if( ( !_is_script || (_is_script && close && tag.compare("script") == 0) ) && c == '>' ){
				element.clear();
				element.tag(tag);
				start_tag = false;
				if(close){

					if( tag.compare("script") == 0 ){
						_is_script = false;
					}

					for( list<Element>::iterator it = _tag_stack.begin(); it != _tag_stack.end(); it++ ){
						if( (*it).tag().compare(tag) == 0 ){
							if( &(*it) == close_element ){
								_content = (*it).content();
								_tag_stack.erase(it);
								return CLOSE_TAG;
							}
							else{
								_tag_stack.erase(it);
							}
							break;
						}
					}
				}
				else{
					_tag_stack.push_front(element);
					
					if( tag.compare("script") == 0 ){
						_is_script = true;
					}

					if( search_tag.length() == 0 ){
						return OPEN_TAG;
					} else if( tag.compare(search_tag) == 0 ){
						return OPEN_TAG;
					}
				}
			}
			else{
				if( 'A' <= c && c <= 'Z' ){
					c += 'a'-'A';
				}
				tag += c;
				if( tag.compare("!--") == 0 ){
					bool end_comment = false;
					while( !_in.eof() ){
						if( c == '-' ){
							_in.get(c);
							while( !_in.eof() && c == '-' ){
								_in.get(c);
								if( c == '>' ){
									tag = "";
									start_tag = false;
									end_comment = true;
									break;
								}
							}
						}

						if( end_comment && c == '>' ){
							break;
						}
						else if( !_in.eof() ){
							_in.get(c);
						}
					}
				}
			}
		}
		else{
			if( _tag_stack.size() > 0 ){
				_tag_stack.front().add_content(c);
			}
		}
		_in.get(c);
	}

	return 0x00;
}