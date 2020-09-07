#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <vector>
#include <arpa/inet.h>
#include "f_dump.hpp"
#include <netdb.h>
#include <unistd.h>
#include <limits.h>
using namespace std;

//Функция приёма сообщений
string f_recv(int sockfd, int new_sockfd){
	char buf[1024];
	new_sockfd=recv(sockfd, buf, 30, 0);
	for (int i=0; i<=30; i++)
		return(buf);	
	cout << endl;
}

//Функция отправки сообщений
bool f_send(int sockfd, int new_sockfd, string message){
	bool flag;
	new_sockfd = send(sockfd, message.c_str(), message.size(), 0);
	if (new_sockfd > 0)
		return(flag=1);
	else
		return(flag=0);
}



int main(){
	int			sockfd, new_sockfd, yes=1, snd, rcv;
	const int 		PORT = 7890;
	char 			buf[1024];
	struct sockaddr_in 	host_addr, client_addr, dest_srv_addr;
	socklen_t 		sin_size;
	string 			message;
	bool 			flag_send;	
	struct hostent		*srvName;

	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0))!=-1)
		cout << "sockfd create......" << sockfd << endl;



	srvName=gethostbyname("datagram.ru");
	if (srvName==NULL) {
		herror("gethostbyname");
		exit(1);
	}
	
	cout << "Name:         " << srvName -> h_name << endl;
	cout << "IP address:   " << inet_ntoa(*(struct in_addr*)srvName -> h_addr) << endl;
	cout << "Type address: " << srvName -> h_addrtype << endl;


	client_addr.sin_family = srvName -> h_addrtype;
	client_addr.sin_port   = htons(PORT);
	inet_aton(inet_ntoa(*(struct in_addr*)srvName -> h_addr), &client_addr.sin_addr);
	memset(&(host_addr.sin_zero), '\0', sizeof(host_addr.sin_zero));
	



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
