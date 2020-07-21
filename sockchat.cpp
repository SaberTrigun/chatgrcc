#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <vector>
#include "f_dump.hpp"
#include <arpa/inet.h>
#include <fstream>
using namespace std;

void f_writeFile(const char *data_buffer){
	setlocale(LC_ALL, "Russian");
	ofstream fs;
	fs.open("chat_history.log", ofstream::app);
	if(!fs.is_open())
		cout<<"Error open"<<endl;	
	fs << data_buffer<<"\n" ;
	int test=5;
	fs.close();

}

string f_readFlile(string *cmd_history){
	string history;
	setlocale(LC_ALL, "Russian");
        ifstream fs;
        fs.open("chat_history.log", ofstream::app);
	if(!fs.is_open())
                cout<<"Error open"<<endl;
	
	return history;

}

its a test
int main(){
	int sockfd, new_sockfd, yes=1, snd, rcv;
	const int PORT = 7890;
	char buf[1024];
	struct sockaddr_in host_addr, client_addr;
	socklen_t sin_size;
	vector <char> send_msg={'X'};



	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0))!=-1)
		cout << "sockfd create......" << sockfd << endl;

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))!=-1)
		cout << "setsockopt accepted......" << setsockopt << endl;

	host_addr.sin_family=AF_INET;
	host_addr.sin_port=htons(PORT);
	inet_aton("192.168.1.4", &host_addr.sin_addr);
	
	cout<<inet_ntoa(host_addr.sin_addr)<<endl;
	memset(&(host_addr.sin_zero), '\0', 8);
	
	if (bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr))!=-1)
		cout << "bind addr_iface and num_port..." << bind << endl;

	if (listen(sockfd, 5)!=-1)
		cout << "waiting connections......" << listen << endl;

	while(1)
	{
		sin_size=sizeof(struct sockaddr_in);
		new_sockfd=accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);
		if (new_sockfd!=-1)
			cout << "connecting......" << endl << "incoming connection" << endl;
		send(new_sockfd, "Hy, welcome to grcc", 19, 0); cout<<send<<endl;
		rcv=recv(new_sockfd, &buf, 1024, 0);
		while (rcv>0)
		{
			printf("RECV: %d байтов\n", rcv);
			dump(buf, rcv);
			f_writeFile(buf); //write history in file
			rcv=recv(new_sockfd, &buf, 1024, 0);
		}



		close (new_sockfd);
	}
}
