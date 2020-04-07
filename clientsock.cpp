#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <vector>
using namespace std;

int main(){
	int sockfd, new_sockfd, yes=1, snd, rcv;
	const int PORT = 7890;
	char buf[6];
	struct sockaddr_in host_addr, client_addr;
	socklen_t sin_size;
	vector <char> send_msg={'F'};
	
	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0))!=-1)
		cout << "sockfd create......" << sockfd << endl;

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))!=-1)
		cout << "setsockopt accepted......" << setsockopt << endl;

	client_addr.sin_family=AF_INET;
	client_addr.sin_port=htons(PORT);
	client_addr.sin_addr.s_addr=0;

	memset(&(host_addr.sin_zero), '\0', 8);
	
	if (bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr))!=-1)
		cout << "bind addr_iface and num_port..." << bind << endl;
	

	

	if ((new_sockfd=connect(sockfd, (struct sockaddr *)&client_addr, sizeof(client_addr)))==0)
	{
		cout<<"Connected!"<<endl;
		for(int i=0; i<=5; i++)
		snd=send(new_sockfd, &send_msg[i], 1, 0);
	}
	rcv=recv(new_sockfd, buf, sizeof(buf), 0);
	if(rcv>0)
		printf("%c\n", buf[0]);
	else
		cout<<"fuck you and colese my socket..."<<rcv<<endl;
	close(new_sockfd);
}
