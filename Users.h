#pragma once
#ifndef  USERS_H
#define  USERS_H
#include <fstream>
class Users {
public:
	int wincnt = 0;		/*胜利场数*/
	int failcnt = 0;	/*失败场数*/
	int drawcnt = 0;	/*平局场数*/
	int minsteps = 0;	/*胜利最短步数*/
	int restcnt = 10;	/*剩余悔棋步数*/
	void Read(void);	/*读取文档*/
	void Write(void);	/*存档*/
};
#endif // ! USERS_H