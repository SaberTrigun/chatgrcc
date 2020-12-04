#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <vector>
#include "f_dump.hpp"
#include <arpa/inet.h>
#include <fstream>
#include "/usr/include/linux/if.h"
#include <netdb.h>
#include <unistd.h>
#include <limits.h>
using namespace std; // так лучше не делать

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
void transfer_f() {
	int		filefd, interimBuf;
	const char	infile[] = "chat_history.log", *pointerInFile = infile;
	char		buf[1024], *pointerBuf = buf;
	filefd = open(pointerInFile, O_RDONLY);
	if (filefd == -1)
		perror("open");
	interimBuf = read(filefd, pointerBuf, 1024);
	if (close(filefd) == -1)
		perror("close");	

}

int main () {
	
	int			sockfd, new_sockfd, yes=1, snd, rcv;
	const int		PORT = 7890;
	char			buf[1024], hostname[HOST_NAME_MAX];
	struct	sockaddr_in	host_addr, client_addr;
	socklen_t		sin_size;
	struct hostent		*srvName;	

//Получаем ip аддресс по имени 
	gethostname(hostname, HOST_NAME_MAX);
	srvName = gethostbyname(hostname);
	if(srvName == NULL) {
		herror("gethostbyname");
		exit(1);
	}

	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) != -1) //Получаем tcp сокет типа Internet
		cout << "sockfd create......" << sockfd << endl;
	else
	{
		perror ("Error call socket");
		exit (1);
	}
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) != -1) // Установка параметров сокета 
		cout << "setsockopt accepted......" << setsockopt << endl;
	else
	{
		perror ("Error setsockopt");
		exit (1);
	}

//Вывод отладочной информации	
	cout << "Name:         " << srvName -> h_name << endl;
	cout << "IP address:   " << inet_ntoa(*(struct in_addr*)srvName -> h_addr) << endl;
	cout << "Type address: " << srvName -> h_addrtype << endl;
//Приведение адреса и порта к нужному виду
	host_addr.sin_family = srvName -> h_addrtype;
	host_addr.sin_port   = htons(PORT);
	inet_aton(inet_ntoa(*(struct in_addr*)srvName -> h_addr), &host_addr.sin_addr);
	memset (&(host_addr.sin_zero), '\0', sizeof(host_addr.sin_zero));


	if (bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr))!=-1) //Биндим сокет
		cout << "bind addr_iface and num_port..." << bind << endl;
	else
	{
		perror ("Error bind");
		exit (1);
	}
	if (listen(sockfd, 5)!=-1) // Прослушиваем
		cout << "waiting connections......" << listen << endl;
	else
	{
		perror ("Error listen");
		exit (1);
	}

	while (1)
	{
		sin_size = sizeof(struct sockaddr_in);
		new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size); //Принимаем входящее соединение
		if (new_sockfd != -1)
			cout << "Connecting......" << endl << "incoming connection" << endl;
		send(new_sockfd, "Hy, welcome to datagram.com!!! 0_0", 34, 0); //Отправляем приветственное сообщене клиенту
		rcv = recv(new_sockfd, &buf, 1024, 0);
		while (rcv > 0)
		{
			printf("RECV: %d байтов\n", rcv);
			dump(buf, rcv);
			f_writeFile(buf);//Запись истории в файл
			rcv = recv(new_sockfd, buf, 1024, 0);
		}	
		close (new_sockfd);

	}



}

