#pragma once
#pragma comment(lib, "ws2_32.lib")

#define RECV_BUFFERSIZE 1024*8
#define MAX_THREADS 1024

#include <string>
#include <iostream>
#include <fstream>
#include <winsock.h>
#include <list>
//#include <strsafe.h>
//#include <sqlext.h>

#include <mysql.h>

using namespace std;

class Vendor
{
public:
	Vendor(void);
	~Vendor(void);

	virtual void get_prices() = 0;

protected:
	SOCKET _s;
	string _hostname;
	char * _output_path; // path of the page

	virtual bool connect( const char * hostname, int port = 80 );
	bool send_receive( string file_path );
	void close();

	void read_line( ifstream & in_file, string & in );
	int read_chunk_size( char * &source, int & size, int init=0 );

	//void connect_db();

private:
	
	list<char *> _test;

	void create_thread( char * buffer );
};

DWORD WINAPI steam_process_line( LPVOID param );

