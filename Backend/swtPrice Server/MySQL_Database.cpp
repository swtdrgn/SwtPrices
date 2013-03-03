#include "StdAfx.h"
#include "MySQL_Database.h"

#define HOST "localhost"
#define PORT 3306
#define USER "INSERT USERNAME"
#define PASS "INSERT PASSWORD"
#define DB "swtprices"


/*
	Implementation of MySQL_Row
*/

swtDB::MySQL_Row::MySQL_Row(){ reset(); }
void swtDB::MySQL_Row::reset(){ _column = 0; }

swtDB::Row & swtDB::MySQL_Row::operator[]( int col ){
	_column = col;
	return *(this);
}

swtDB::Row & swtDB::MySQL_Row::operator >> ( string & out ){
	out = _row[_column];
	_column++;
	return *this;
}

swtDB::Row & swtDB::MySQL_Row::operator >> ( int & out ){
	out = atoi(_row[_column]);
	_column++;
	return *this;
}

swtDB::Row & swtDB::MySQL_Row::operator >> ( long & out ){
	out = atol(_row[_column]);
	_column++;
	return *this;
}

swtDB::Row & swtDB::MySQL_Row::operator >> ( float & out ){
	out = atof(_row[_column]);
	_column++;
	return *this;
}


/*
	Implementation of MySQL_Result
*/

swtDB::MySQL_Result::MySQL_Result(){ _size = 0; _eof = true; }

swtDB::MySQL_Result::~MySQL_Result(){
	//delete _mysql_result;
}

unsigned int swtDB::MySQL_Result::size() { return _size; }
bool swtDB::MySQL_Result::more() { return !_eof; }

swtDB::MySQL_Result & swtDB::MySQL_Result::operator++(){
	if(_size==0||_eof){ return *this; }
	_row = mysql_fetch_row(_mysql_result);
	if( _row == NULL ){ _eof = true; }
	return *this;
}

swtDB::MySQL_Result & swtDB::MySQL_Result::operator >> ( MySQL_Row& row ){
	row.reset();
	row._row = _row;
	(*this)++;
	return *this;
}

swtDB::MySQL_Database::MySQL_Database(void)
{
}

swtDB::MySQL_Database::MySQL_Database( char * db ){
	_database = db;
}


swtDB::MySQL_Database::~MySQL_Database(void)
{
}

bool swtDB::MySQL_Database::connect( const char * hostname, int port, const char * user, const char * pass ){

	string host = hostname;
	//host += ":";
	//host += port;

	

	int query_state;

	mysql_init(&_mysql);
	_connection = mysql_real_connect(&_mysql,host.c_str(),user,pass,DB,0,0,0);

	if(!_connection){
		cout << "Failed to connect to database: Error: " << mysql_error(&_mysql) << endl;
		return false;
    }

	return true;
}

bool swtDB::MySQL_Database::insert( string table, map<string,string> &cols_values ){
	int size = cols_values.size();
	if( size == 0 ){ return true; }

	string insert_query = "INSERT INTO `";
	insert_query += _database;
	insert_query += "`.`";
	insert_query += table;
	insert_query += "` (";

	string cols="",values="";
	map<string,string>::iterator it=cols_values.begin();
	while(true){
		cols += "`";
		cols += it->first;
		cols += "`";
		values += "'";
		values += it->second;
		values += "'";
		it++;
		if( it!=cols_values.end() ){
			cols += ", ";
			values += ", ";
		}
		else{
			break;
		}
	}
	
	insert_query += cols;
	insert_query += ") VALUES (";
	insert_query += values;
	insert_query += ")";

	mysql_query(_connection,insert_query.c_str());
}

bool swtDB::MySQL_Database::insert_update( string table, map<string,string> &insert_cols_values, map<string,string> &update_cols_values ){
	int size = insert_cols_values.size();
	if( size == 0 ){ return true; }

	string insert_query = "INSERT INTO `";
	insert_query += _database;
	insert_query += "`.`";
	insert_query += table;
	insert_query += "` (";

	string cols="",values="";
	map<string,string>::iterator it=insert_cols_values.begin();
	while(true){
		cols += "`";
		cols += it->first;
		cols += "`";
		values += "'";
		values += it->second;
		values += "'";
		it++;
		if( it!=insert_cols_values.end() ){
			cols += ", ";
			values += ", ";
		}
		else{
			break;
		}
	}
	
	insert_query += cols;
	insert_query += ") VALUES (";
	insert_query += values;
	insert_query += ") ON DUPLICATE KEY UPDATE ";

	it=update_cols_values.begin();
	while(true){
		insert_query += "`";
		insert_query += it->first;
		insert_query += "`";
		insert_query += " = '";
		insert_query += it->second;
		insert_query += "'";
		it++;
		if( it!=update_cols_values.end() ){
			insert_query += ", ";
		}
		else{
			break;
		}
	}

	mysql_query(_connection,insert_query.c_str());
}

bool swtDB::MySQL_Database::replace( string table, map<string,string> &cols_values ){
	int size = cols_values.size();
	if( size == 0 ){ return true; }

	string insert_query = "REPLACE INTO `";
	insert_query += _database;
	insert_query += "`.`";
	insert_query += table;
	insert_query += "` (";

	string cols="",values="";
	map<string,string>::iterator it=cols_values.begin();
	while(true){
		cols += "`";
		cols += it->first;
		cols += "`";
		values += "'";
		values += it->second;
		values += "'";
		it++;
		if( it!=cols_values.end() ){
			cols += ", ";
			values += ", ";
		}
		else{
			break;
		}
	}
	
	insert_query += cols;
	insert_query += ") VALUES (";
	insert_query += values;
	insert_query += ")";


	//cout << insert_query << endl;
	mysql_query(_connection,insert_query.c_str());
}

bool swtDB::MySQL_Database::select( string table, string conditions, swtDB::MySQL_Result & result ){
	MYSQL_RES *res_set;
    MYSQL_ROW row;
	string select_query = "SELECT * FROM ";
	select_query += table + " " + conditions;

	mysql_query(_connection,select_query.c_str());
 
    res_set = mysql_store_result(_connection);

    unsigned int numrows = mysql_num_rows(res_set);

	result._size = numrows;
	result._mysql_result = res_set;
	if(numrows==0){ result._eof = true; } else{ result._eof = false; }

	result++;
	return true;
}

bool swtDB::MySQL_Database::select( string table, swtDB::MySQL_Result & result ){
	MYSQL_RES *res_set;
    MYSQL_ROW row;
	string select_query = "SELECT * FROM ";
	select_query += table;

	mysql_query(_connection,select_query.c_str());
 
    res_set = mysql_store_result(_connection);

    unsigned int numrows = mysql_num_rows(res_set);
 
    /*while ((row = mysql_fetch_row(res_set)) != NULL){
        printf("%s\n",row[i] != NULL ?
        row[i] : "NULL");
    }*/

	result._size = numrows;
	result._mysql_result = res_set;
	if(numrows==0){ result._eof = true; } else{ result._eof = false; }

	result++;
	return true;
}

bool swtDB::MySQL_Database::erase( string table, string conditions ){
	string delete_query = "DELETE FROM `" DB "`.`" + table + "` " + conditions;
	mysql_query(_connection,delete_query.c_str());

	return true;
}

void swtDB::MySQL_Database::close(){
	mysql_close(&_mysql);
	_connection = NULL;
}