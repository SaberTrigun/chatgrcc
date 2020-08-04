void f_recv(int sockfd, int new_sockfd, char buf[1024]){
	new_sockfd=recv(sockfd, buf, 30, 0);
	for (int i=0; i<=30; i++)
		cout << buf[i];
	cout << endl;
}



void f_send(int sockfd, int new_sockfd){
	new_sockfd=send(sockfd, "Hello, i am client and i am work!", 33, 0);
	if (new_sockfd<=0)
		perror("error send");
}
