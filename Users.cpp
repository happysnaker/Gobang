#include "Users.h"
using namespace std;
void Users::Read(void)
{
	ifstream in("users.dat", ios::binary | ios::in);
	if (!in.is_open())	return;
	in.read((char*)&wincnt, sizeof(int));
	in.read((char*)&failcnt, sizeof(int));
	in.read((char*)&drawcnt, sizeof(int));
	in.read((char*)&minsteps, sizeof(int));
	in.read((char*)&restcnt, sizeof(int));
	in.close();
}

void Users::Write(void)
{
	ofstream out("users.dat", ios::binary);
	if (!out.is_open()) exit(0);
	out.write((char*)&wincnt, sizeof(int));
	out.write((char*)&failcnt, sizeof(int));
	out.write((char*)&drawcnt, sizeof(int));
	out.write((char*)&minsteps, sizeof(int));
	out.write((char*)&restcnt, sizeof(int));
	out.close();
}
