#pragma once
#ifndef CHESS_H
#define CHESS_H
#include <graphics.h>
#include <time.h>
#include <vector>
#include "Class.h"
using namespace std;
class Chess {
public:
	/*四个接口，用于过去包含x,y点四个方向的五元组情况，返回敌我棋的数量*/
	vector<int> Get_Nums1(int x, int y);
	vector<int> Get_Nums2(int x, int y);
	vector<int> Get_Nums3(int x, int y);
	vector<int> Get_Nums4(int x, int y);
	int  Get_Points(int x, int y, int ch);				/*获取该点的得分*/
	int  x_Points(vector<int> nums, int ch);			/*根据敌我棋的数量获取得分*/
	bool is_Ok(int x, int y) { return x >= 0 && y >= 0 && x < 18 && y < 18; }
	virtual bool win(int x, int y) = 0;
	virtual bool gameOver(Show s, Users& user) = 0;		/*判断是否结束游戏*/
	double GetNowPoints(int ch);
};
class MyChess : public Chess {
public:
	MyChess() : steps(0), now(false) {};
	int Get_xy(int& x, int& y);		/*判断该点是否处于空交点范围内*/
	int steps;						/*走的步数*/
	bool now;						/*是否轮到我们下棋*/
	bool win(int x, int y)override {
		return (!Get_Nums1(x, y).empty() && Get_Nums1(x, y)[1] == 5) || (!Get_Nums2(x, y).empty() && Get_Nums2(x, y)[1] == 5) || (!Get_Nums3(x, y).empty() && Get_Nums3(x, y)[1] == 5) || (!Get_Nums4(x, y).empty() && Get_Nums4(x, y)[1] == 5);
	}
	bool gameOver(Show s, Users& user)override;
	void get_grade(Users user, wchar_t*& wstr);
	void get_assuse(wchar_t*& wstr);
};
class AiChess : public Chess {
public:
	bool is_Ok(int x, int y) { return x >= 0 && y >= 0 && x < 18 && y < 18; }
	void Get_key_Setchess(MyChess& mychs);/*获取得分最高的位置，并下棋*/
	bool win(int x, int y) override {
		return (!Get_Nums1(x, y).empty() && Get_Nums1(x, y)[0] == 5) || (!Get_Nums2(x, y).empty() && Get_Nums2(x, y)[0] == 5) || (!Get_Nums3(x, y).empty() && Get_Nums3(x, y)[0] == 5) || (!Get_Nums4(x, y).empty() && Get_Nums4(x, y)[0] == 5);
	}
	bool gameOver(Show s, Users& user)override;
};


#endif // !CHESS_H



