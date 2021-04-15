#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include "f_dump.hpp"
#include <arpa/inet.h>
#include <fstream>
#include "/usr/include/linux/if.h"
#include <netdb.h>
#include <unistd.h>
#include <limits.h>
using namespace std; // так лучше не делать

	class netConf {
		private:
			int			sockfd, new_sockfd, yes, setsockoptfd, bindfd, listenfd;
			const int		Port;
			vector <char>		buf;
			char			hostname[HOST_NAME_MAX];//Переменная HOST_NAME_MAX массива hostname определена в какой то либе!
			struct sockaddr_in	host_addr, client_addr;
			socklen_t		sin_size;
			struct hostent		*srvName;
		public:
			netConf() : Port(7890), yes(1)
			{}

			int get_netPort() 
			{ return Port; }

			void getHostent(){
				gethostname(hostname, HOST_NAME_MAX);
				srvName = gethostbyname(hostname);
				cout << "Name:          " << srvName -> h_name << endl;
				cout << "IP address:    " << inet_ntoa(*(struct in_addr*)srvName -> h_addr_list[0])<< endl;
				cout << "Type address:  " << srvName -> h_addrtype << endl;
			}
			int netSockInit(){
				if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0 )
				{
					perror ("Error call socket");
					exit (1);
				}
				if ((setsockoptfd = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))) == -1)
				{
					perror ("Error setsockopt");
					exit (1);
				}
				host_addr.sin_family = srvName -> h_addrtype;
				host_addr.sin_port   = htons(get_netPort());
				inet_aton(inet_ntoa(*(struct in_addr*)srvName -> h_addr_list[0]), &host_addr.sin_addr);
				memset (&(host_addr.sin_zero), '\0', sizeof(host_addr.sin_zero));
				if ((bindfd = bind(sockfd,(struct sockaddr*)&host_addr, sizeof(struct sockaddr))) == -1 )
				{
					perror ("Error bind");
					exit (1);
				}
				return 0;
			}
			void listenStart(){
				if ((listenfd = listen(sockfd, 5)) < 0)
				{
					perror ("Error listen");
					exit (1);
				}
			}
			int acceptConnect(){
				sin_size = sizeof(struct sockaddr_in);
				if ((new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size)) != -1)
				{
					cout << "Connecting......" << endl << "incoming connection" << endl;
					return new_sockfd;
				}
				else
				{
					perror ("Error accept");
					exit (1);
				}
					
			}
			void getSockInfo(){
				cout << "Socketfd create........" 		<< sockfd 	<< endl;  
				cout << "Setsockopt accepted...."	  	<< setsockoptfd << endl;
				cout << "Bind addr_iface and num port...."	<< bindfd	<< endl;
				cout << "Waiting connections............."	<< listenfd	<< endl;

			}
			void closeSock(){
				close (new_sockfd);
			}
	};



//Функция записи истории в файл
//Эту функцию со временем переписать для ухода от библиотеки fstream
	void f_writeFile(const char *data_buffer) {
		setlocale(LC_ALL, "Russian");
		fstream	fs;
		fs.open("chat_history.log", ofstream::app);
		if(!fs.is_open())
			cout << "Error open" << endl;
		fs << data_buffer << "\n";
		fs.close();
	}

//Функция чтения из лог файла. Пока не используется
//Со временем переписать её для ухода от библиотеки fstream
	string f_readFile(string *cmd_fistory) {
		string history;
		setlocale(LC_ALL, "Russian");
		ifstream fs;
		fs.open("chat_history.log", ofstream::app);
		if(!fs.is_open())
			cout << "Error_open" << endl;
		return history;
	}

//Функция для передачи файла по сети
	void transferFileOut(int new_sockfd) {
		int		filefd, interimBuf;
		const char	infile[] = "chat_history.log", *pointerInFile = infile;
		char		buf[1024], *pointerBuf = buf;
		filefd = open(pointerInFile, O_RDONLY);//Открываем файл и получаем файловый дескриптор
		if (filefd == -1)
			perror("open");
		interimBuf = read(filefd, pointerBuf, 1024); //Читаем файл и записываем в буфер
		if (close(filefd) == -1)
			perror("close");	
		send(new_sockfd, pointerBuf, 1024, 0); 
		
	}

int main () {
	
	int			rcv, newConnect;
	char			buf[1024]; 
	netConf			confServer;

	confServer.getHostent();
	confServer.netSockInit();
	confServer.getSockInfo();
	confServer.listenStart();

	while (1)
	{
		newConnect = confServer.acceptConnect();
		send(newConnect, "Hy, welcome to datagramchat.com!!! 0_0", 38, 0); //Отправляем приветственное сообщене клиенту
		rcv = recv(newConnect, &buf, 1024, 0);
		while (rcv > 0)
		{
			printf("RECV: %d байтов\n", rcv);
			dump(buf, rcv);
			rcv = recv(newConnect, buf, 1024, 0);
			//f_writeFile(buf);//Запись истории в файл
		}
		
		confServer.closeSock();

	}

	return 0;

}

