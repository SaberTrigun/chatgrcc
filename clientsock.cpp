#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <vector>
#include <arpa/inet.h>
#include "f_dump.hpp"
using namespace std;

int main(){
	int sockfd, new_sockfd, yes=1, snd, rcv;
	const int PORT = 7890;
	char buf[1024];
	struct sockaddr_in host_addr, client_addr, dest_srv_addr;
	socklen_t sin_size;
	vector <char> send_msg={'F'};
		
	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0))!=-1)
		cout << "sockfd create......" << sockfd << endl;

	//if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))!=-1)
	//	cout << "setsockopt accepted......" << setsockopt << endl;

	client_addr.sin_family=AF_INET;
	client_addr.sin_port=htons(PORT);
	inet_aton("192.168.1.4", &client_addr.sin_addr);

	cout<<inet_ntoa(client_addr.sin_addr)<<endl;

	memset(&(host_addr.sin_zero), '\0', 8);
	

	
	while (1)	
	{
		new_sockfd=connect(sockfd, (struct sockaddr *)&client_addr, sizeof(client_addr));
		if (new_sockfd==0)
			cout<<"Connected!"<<endl;
		rcv=read(new_sockfd, buf, 1024);
		for(int i=0; i<=rcv; i++)
		cout<<rcv;
		send(new_sockfd, "Hy", 2, 0);
		while(rcv>0)
		{
			printf("RECV: %d байтов\n", rcv);
			dump(buf, rcv);
			rcv=recv(new_sockfd, &buf, 1024, 0);
		}
		close(new_sockfd);
	}
		
		
	 
}
