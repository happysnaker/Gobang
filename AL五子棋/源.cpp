#include <time.h>
#include <iostream>
#include <stack>
#include <vector>
#include <string.h>
using namespace std;				

char* Hello(){
	char s = '1';
	return &s;
} 
int main() {
	int t;
	char s[64];
	sscanf("12341/home.html", "%d%s", &t, s);
	cout << t << " " << s << endl;
	char *ph = s;
	//strcpy(ph, s);
	cout << ph[2];
	return 0;
}

