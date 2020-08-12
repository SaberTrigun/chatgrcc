#include <iostream>
#include <string>
using namespace std;

int main (){
	string msg;
	getline(cin, msg);
	cout << endl;
	int sizeMsg = 0;
	sizeMsg=msg.size();
	cout << sizeMsg << endl << msg << endl;
}
