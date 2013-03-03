#pragma once
#include "database.h"
#include <winsock.h>
#include <mysql.h>
#include <iostream>
#include <map>

#define ACSCENDING "ACSC"
#define DESCENDING "DESC"

namespace swtDB{
	class MySQL_Row : public Row{
	public:
		friend class MySQL_Result;
		MySQL_Row();
		Row & operator[]( int col );
		Row & operator >> ( string& );
		Row & operator >> ( int& );
		Row & operator >> ( long& );
		Row & operator >> ( float& );
	private:
		MYSQL_ROW _row;
		int _column;
		void reset();
	};

	class MySQL_Result : 
		public Result{

	public:
		friend class MySQL_Database;
		MySQL_Result();
		~MySQL_Result();
		MySQL_Result & operator++();
		MySQL_Row & operator[]( int row );
		MySQL_Result & operator >> ( MySQL_Row& row );
		unsigned int size();
		bool more();

	private:
		int _size;
		MYSQL_RES *_mysql_result;
		bool _eof;
		MYSQL_ROW _row;
	};

	class MySQL_Database :
		public Database{

	public:
		MySQL_Database(void);
		MySQL_Database( char * db );
		~MySQL_Database(void);

		bool connect( const char *hostname, int port, const char *user, const char *pass );
		bool insert( string table, map<string,string> &cols_values );
		bool insert_update( string table, map<string,string> &insert_cols_values, map<string,string> &update_cols_values );
		bool replace( string table, map<string,string> &cols_values );
		bool select( string table, MySQL_Result & result );
		bool select( string table, string conditions, MySQL_Result & result );
		bool erase( string table, string conditions );
		//bool perform( string query );
		//string WHERE( ... );
		//string ORDER( ... );
		//string LIMIT( int min, int max );
		void close();

	private:
		MYSQL *_connection, _mysql;
		MYSQL_RES *_result;
		char * _database;
	};
}