#pragma once
#ifndef SHOW_H
#define SHOW_H
#include "Class.h"
#include "Users.h"
#include <graphics.h>
#include <time.h>
#include <string>
#include <stack>
#include <vector>
/*将坐标转化为实际位置*/
template<typename T> constexpr auto switch_xy(T x) { return x * 30 + 20; }

/*将实际位置转化为坐标*/
template<typename T> constexpr auto reswitch_xy(T x) { return (x - 20) / 30; }
class Show
{
public:
	void show(MyChess& mychs, Users& user);		/*绘制函数*/
	void setChess(int x, int y, int choice);	/*下棋函数，choice设置成0为电脑下，1为我方*/
	void Mouse(MyChess& mychs, Users& user);	/*控制响应鼠标*/
	void End(int who, Users& user, int steps);	/*游戏结束绘制页面*/
};

#endif // !SHOW_H



