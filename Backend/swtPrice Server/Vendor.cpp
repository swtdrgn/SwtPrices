#include "StdAfx.h"
#include "Vendor.h"
#include <map>
#include <limits>


Vendor::Vendor(void)
{
}


Vendor::~Vendor(void)
{
}

bool Vendor::connect( const char * hostname, int port ){
	WSADATA wsadata;
	int error = WSAStartup(0x0202,&wsadata);

	if (wsadata.wVersion != 0x0202){
		WSACleanup();
		cout << "Wrong Version of WSDATA" << endl;
		return false;
	}


	hostent * he = gethostbyname(hostname);
	if( he == NULL ){ cout << "he = 0" << endl; return false; }


	SOCKADDR_IN target;
	target.sin_family = AF_INET;
	target.sin_port = htons((unsigned short) port);
	target.sin_addr = *( (struct in_addr *)(he->h_addr_list[0]) );

	cout <<  inet_ntoa(*(struct in_addr *)(he->h_addr_list[0])) << endl;

	_s = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (_s == INVALID_SOCKET){ cout << "Invalid Socket" << endl; return false; }

	for( int i=1; ::connect(_s,(SOCKADDR*)&target,sizeof(target)) == SOCKET_ERROR; i++ ){
		if( he->h_addr_list[i] == NULL ){
			cout << "Fail to connect." << endl; 
			return false;
		}
		target.sin_addr = *( (struct in_addr *)(he->h_addr_list[i]) );
	}

	_hostname = hostname;

	return true;
}

bool Vendor::send_receive( string file_path ){

	string request = "GET " + file_path + " HTTP/1.1\r\n";
	request += "Host: " + _hostname + "\r\n";
	//request += "Content-Length: 10\r\n";
	//request += "Content-Type: text/plain;charset=UTF-8\r\n\r\n";
	request += "Content-Type: text/plain\r\n\r\n";

	//cout << request << endl;

	if (send(_s,request.c_str(),request.length(),0) != request.length()){
		cout << "Fail to send." << endl;
		return false;
	}

	char buffer[RECV_BUFFERSIZE+8], * bptr;
	int length=RECV_BUFFERSIZE;

	ofstream output_html (_output_path);

	output_html << file_path << endl << endl;

	bool header_end = false, chunked_read = false;
	int CRLF = 0, chunk_size, current_size=0, recv_size = RECV_BUFFERSIZE;
	map<string,string> fields;
	string field="",value="", *fptr=&field;
	char c;

	int j=0;
    while ( (length = recv(_s,&buffer[0], recv_size, 0)) > 0 ){
		bptr = buffer;
		if( !header_end ){
			for( int i=0; i<length; i++ ){
				if( !header_end ){
					//cout << buffer[i];
					if( CRLF%2 == 0 && buffer[i] == '\r' ){
						CRLF++;
					}
					else if( CRLF%2 == 1 && buffer[i] == '\n' ){
						CRLF++;
					}
					else if( buffer[i] == ':' ){
						fptr = &value;
					}
					else{
						if( 'A' <= buffer[i] && buffer[i] <= 'Z' ){
							c = buffer[i] + 'a'-'A';
						}
						else{
							c = buffer[i];
						}
						CRLF = 0;
						*fptr += c;
					}

					if( CRLF == 2 ){
						cout << field << " : " << value << endl;
						fields[field] = value;
						fptr = &field;
						field = "";
						value = "";
					}
					else if( CRLF == 4 ){
						map<string,string>::iterator it;

						header_end = true;
						CRLF = i+1;

						if( (it=fields.find("content-length")) == fields.end() ){
							chunked_read = true;
							bptr = &buffer[CRLF];
							length = length-CRLF;
							chunk_size = read_chunk_size(bptr,length);
						}
						else{
							chunk_size = atoi(fields["content-length"].c_str());
							if( chunk_size <= 0 ){ chunk_size = INT_MAX; }
							cout << "Size: " << chunk_size << endl;
							current_size = length-CRLF;
							output_html.write(&buffer[CRLF],current_size);
						}
					}
				}
			}
		}
		else if(!chunked_read){
			current_size += length;
			output_html.write(buffer,length);
		}

		if(chunked_read){
			while( chunk_size <= current_size+length ){

				CRLF = chunk_size-current_size;
				output_html.write(bptr,CRLF);
				current_size = 0;
				length -= CRLF;
				bptr = bptr+CRLF;
				chunk_size = 0;

				if( bptr == buffer+RECV_BUFFERSIZE ){
					cout << "Renewed Read" << endl;
					length = recv(_s,&buffer[0], recv_size, 0);
					if( length <= 0 ){ break; }
					bptr = &buffer[0];
					if( *bptr == '\r' ){
						bptr++;
						length--;
					}
					else{
						cout << "Read: " << *bptr << "|<-" << endl;
						cout << "Should be: \\r" << endl;
						exit(0);
					}
				}
				else if( *bptr == '\r' ){
					bptr++;
					length--;
				}
				else{
					cout << "Read: " << *bptr << "|<-" << endl;
					cout << "Should be: \\r" << endl;
					exit(0);
				}


				if( bptr == buffer+RECV_BUFFERSIZE ){
					cout << "Renewed Read" << endl;
					length = recv(_s,&buffer[0], recv_size, 0);
					if( length <= 0 ){ break; }
					bptr = &buffer[0];
					if( bptr != buffer+RECV_BUFFERSIZE && *bptr == '\n' ){
						bptr++;
						length--;
					}
					else{
						cout << "Read: " << *bptr << "|<-" << endl;
						cout << "Should be: \\n" << endl;
						exit(0);
					}
				}
				else if( bptr != buffer+RECV_BUFFERSIZE && *bptr == '\n' ){
					bptr++;
					length--;
				}
				else{
					cout << "Read: " << *bptr << "|<-" << endl;
					cout << "Should be: \\n" << endl;
					exit(0);
				}

				cout << "LENGTH: " << length << endl;

				chunk_size = read_chunk_size(bptr,length);

				while( length < 0 ){
					cout << "Chunk Size 0: " << chunk_size << endl;
					cout << "LENGTH < 0: " << length << endl;
					length = recv(_s,&buffer[0], recv_size, 0);
					cout << "LENGTH: " << length << endl;
					bptr = &buffer[0];
					chunk_size = read_chunk_size(bptr,length,chunk_size);
				}

				cout << "Chunk Size: " << chunk_size << endl;

				if( chunk_size == 0 ){
					return true;
				}
			}

			output_html.write(bptr,length);
			current_size += length;
		}
		else{
			if( current_size >= chunk_size ){
				cout << "Current Size: " << current_size << endl;
				cout << "Content-Length: " << chunk_size << endl;
				return true;
				//break;
			}
			else if( chunk_size-current_size < RECV_BUFFERSIZE ){
				recv_size = chunk_size-current_size;
			}
		}
	}
	//cout << "Fully received." << endl;

	//output_html << endl << endl;
	//output_html << request;

	//exit(0);
}

int Vendor::read_chunk_size( char * &source, int & size, int init ){
	int CRLF = 0;
	int chunk_size = init;
	for( int i=0; i<size; i++ ){
		cout << source[i];
		if( source[i] == '\r' ){
			CRLF++;
		}
		else if( CRLF == 1 && source[i] == '\n' ){
			cout << endl;
			CRLF++;
			size -= i+1;
			source = &source[i+1];
			return chunk_size;
		}
		else{
			if( '0' <= source[i] && source[i] <= '9' ){
				chunk_size <<= 4;
				chunk_size += source[i] - '0';
			}
			else if( 'a' <= source[i] && source[i] <= 'f' ){
				chunk_size <<= 4;
				chunk_size += 10 + source[i] - 'a';
			}
			else if( 'A' <= source[i] && source[i] <= 'F' ){
				chunk_size <<= 4;
				chunk_size += 10 + source[i] - 'A';
			}
			else{
				cout << endl;
				size -= i+1;
				source = &source[i];
				return chunk_size;
			}
			CRLF = 0;
		}
	}
	size = -1;
	return chunk_size;
}

void Vendor::close(){
	if (_s){ closesocket(_s); }
    WSACleanup();
}


void Vendor::create_thread( char * buffer ){
	DWORD dwThreadIdArray;	
	HANDLE hThreadArray = CreateThread( 
        NULL,                   // default security attributes
        0,                      // use default stack size  
        steam_process_line,			// thread function name
        buffer,          // argument to thread function 
        0,                      // use default creation flags 
        &dwThreadIdArray);   // returns the thread identifier 
}

DWORD WINAPI steam_process_line( LPVOID param ){ return NULL; }

void Vendor::read_line( ifstream & in_file, string & in ){
	string temp;
	while(in_file.peek() == ' '){
		in_file >> temp;
		in += " " + temp;
	}
}

/*void show_error(unsigned int handletype, const SQLHANDLE& handle){
	SQLWCHAR sqlstate[1024];
	SQLWCHAR message[1024];
	if(SQL_SUCCESS == SQLGetDiagRec(handletype, handle, 1, sqlstate, NULL, message, 1024, NULL))
		cout<<"Message: "<<message<<"\nSQLSTATE: "<<sqlstate<<endl;
}*/
