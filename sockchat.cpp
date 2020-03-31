#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <vector>
using namespace std;

int main(){
	int sockfd, new_sockfd, yes=1;
	const int PORT = 7890;
	char buf[1024];
	struct sockaddr_in host_addr, client_addr;
	socklen_t sin_size;


	if (sockfd = socket(AF_INET, SOCK_STREAM, 0)>0)
		cout << "sockfd create......" << endl;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))==-1)
		cout << "setsockopt accepted......" << endl;

	host_addr.sin_family=AF_INET;
	host_addr.sin_port=(PORT);
	host_addr.sin_addr.s_addr=0;



	if (bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr))==-1)
		cout << "bind addr_iface and num_port" << endl;

	if (listen(sockfd, 5)==-1)
		cout << "waiting connections......" << endl;

	while(1)
	{
		sin_size=sizeof(struct sockaddr_in);
		new_sockfd=accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);
		if (new_sockfd>1)
			cout << "anyone hello" << endl;
		close (new_sockfd);
	}
}