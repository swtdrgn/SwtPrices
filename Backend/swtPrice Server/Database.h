#pragma once

#include <string>

using namespace std;

namespace swtDB{
	class Row{
	public:
		Row();
		virtual Row & operator[]( int col ) = 0;
		virtual Row & operator >> ( string& ) = 0;
		virtual Row & operator >> ( int& ) = 0;
		virtual Row & operator >> ( float& ) = 0;
	};

	class Result{
	public:
	};

	class Database{
	public:
		Database(void);
		~Database(void);

		virtual bool connect( const char * hostname, int port, const char * user, const char * pass ) = 0;
		virtual void close() = 0;
	};
}