#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <vector>
#include <arpa/inet.h>
#include "f_dump.hpp"
using namespace std;

string f_recv(int sockfd, int new_sockfd){
	char buf[1024];
	new_sockfd=recv(sockfd, buf, 30, 0);
	for (int i=0; i<=30; i++)
		return(buf);	
	cout << endl;
}

bool f_send(int sockfd, int new_sockfd, string message){
	bool flag;
	new_sockfd = send(sockfd, message.c_str(), message.size(), 0);
	if (new_sockfd > 0)
		return(flag=1);
	else
		return(flag=0);
}



int main(){
	int		sockfd, new_sockfd, yes=1, snd, rcv;
	const int 	PORT = 7890;
	char 		buf[1024];
	struct 		sockaddr_in host_addr, client_addr, dest_srv_addr;
	socklen_t 	sin_size;
	vector <char> 	send_msg={'F'};
	string 		message;
	bool 		flag_send;	

	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0))!=-1)
		cout << "sockfd create......" << sockfd << endl;

	client_addr.sin_family=AF_INET;
	client_addr.sin_port=htons(PORT);
	inet_aton("192.168.173.37", &client_addr.sin_addr);

	cout<<inet_ntoa(client_addr.sin_addr)<<endl;

	memset(&(host_addr.sin_zero), '\0', 8);
	
	new_sockfd=connect(sockfd, (struct sockaddr *)&client_addr, sizeof(client_addr));
	if (new_sockfd)
	{
		perror("error");
		exit(1);
	}


	cout << f_recv(sockfd, new_sockfd) << endl;

	while (1)
	{

		getline(cin, message);
		flag_send = f_send(sockfd, new_sockfd, message);
		if (flag_send > 0)
		{
			cout << "Message sent!" << endl;
		}	
		flag_send = 0;	
	}






		
	 
}
