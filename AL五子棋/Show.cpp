#include "Show.h"
#include "Chess.h"
#include <vector>
#pragma comment(lib,"Winmm.lib")
using namespace std;
extern int aiset;
extern stack<pair<int, int>> stk;
extern vector<vector<int>> map;
extern double myPos, aiPos;
static const char dir1[] = { "ABCDEFGHIJKLMNOPQR" };
static const char dir2[] = { "123456789ABCDEFGHI" };
void Show::show(MyChess& mychs, Users& user)
{
	BeginBatchDraw();
	setbkcolor(YELLOW);
	cleardevice();
	setcolor(BLACK);
	TCHAR row, col;
	for (int i = 20; i <= 530; i += 30) {
		line(20, i, 530, i);
		line(i, 20, i, 530);
		row = dir1[reswitch_xy(i)];
		col = dir2[reswitch_xy(i)];
		outtextxy(5, i - 7, row);
		outtextxy(i - 5, 545, col);
	}
	setfillcolor(BLACK);
	/*4个小黑点*/
	fillcircle(170, 170, 3);
	fillcircle(170, 380, 3);
	fillcircle(380, 170, 3);
	fillcircle(380, 380, 3);
	for (int i = 0; i < 18; i++) {
		for (int j = 0; j < 18; j++) {
			if (map[i][j] != -1) {
				setChess(switch_xy(i), switch_xy(j), map[i][j]);
			}
		}
	}
	TCHAR str[250];
	wchar_t* wstr = new wchar_t[100];
	//double nowTime = clock(), sec = 20 - ((nowTime - startTime) / CLOCKS_PER_SEC);
	mychs.get_grade(user, wstr);
	setcolor(BLACK);
	/*显示用户信息*/
	outtextxy(600, 5, _T("用户：小蘑菇"));
	_stprintf_s(str, _T("胜场：%d"), user.wincnt);
	outtextxy(600, 25, str);
	_stprintf_s(str, _T("负场：%d"), user.failcnt);
	outtextxy(600, 45, str);
	_stprintf_s(str, _T("平局：%d"), user.drawcnt);
	outtextxy(600, 65, str);
	_stprintf_s(str, _T("胜率：%.2f%%"), user.failcnt ? (double)user.wincnt / (double)(user.wincnt + user.failcnt) * 100: 0);
	outtextxy(600, 85, str);
	_stprintf_s(str, _T("段位：%s"), wstr);
	outtextxy(600, 105, str);
	_stprintf_s(str, _T("胜场最短步数: %d"), user.minsteps);
	outtextxy(600, 125, str);
	_stprintf_s(str, _T("剩余悔棋步数: %d"), user.restcnt);
	outtextxy(600, 145, str);

	setcolor(BLUE);
	line(580, 2, 580, 170);
	line(580, 170, 735, 170);
	line(735, 170, 735, 2);
	line(580, 2, 735, 2);
	setcolor(BLACK);

	_stprintf_s(str, _T("您当前走了 %d 步"), mychs.steps);
	outtextxy (600, 195, str);
	outtextxy(600, 225, _T("当前鼠标行列："));

	setcolor(RED);
	rectangle(620, 265, 680, 305);
	outtextxy(634, 276, _T("悔棋"));
	setcolor(BLACK);

	/*注意绘图时的坐标是逆的，它的y轴是向下的*/
	_stprintf_s(str, _T("电脑刚才所下位置：%c行 %c列"), dir1[aiset & 0x1f], dir2[aiset >> 5]);
	outtextxy(570, 330, str);
	
	_stprintf_s(str, _T("您的棋局得分: %.1f"), myPos);
	outtextxy(570, 370, str);
	_stprintf_s(str, _T("对方棋局得分: %.1f"), aiPos);
	outtextxy(570, 410, str);
	mychs.get_assuse(wstr);
	_stprintf_s(str, _T("当前您的棋局评估：%s"), wstr);
	outtextxy(570, 450, str);

	if (mychs.now == true) {
		outtextxy(180, 595, _T("现在是您的回合，请下棋..."));
	}
	else
		outtextxy(160, 595, _T("现在是对方的回合，请您等待..."));
	FlushBatchDraw();
	EndBatchDraw();
	return;
}

void Show::setChess(int x, int y, int choice)//0为电脑下棋，1为自己下棋
{
	setfillcolor(RED);
	if (!choice) 
		setfillcolor(BLACK);
	fillcircle(x, y, 14);
	return;
}

void Show::Mouse(MyChess& mychs, Users& user)
{
	MOUSEMSG m;
	while (true) {
		TCHAR str[250];
		m = GetMouseMsg();
		int x = m.x, y = m.y;
		switch (m.uMsg)
		{
		case WM_MOUSEMOVE:	
			show(mychs, user);
			if (mychs.Get_xy(x, y) && map[reswitch_xy(x)][reswitch_xy(y)] == -1) {
				int x1 = reswitch_xy(x), y1 = reswitch_xy(y);
				_stprintf_s(str, _T("%c行 %c列"), dir1[y1], dir2[x1]);
				outtextxy(705, 225, str);
				circle(x, y, 14);		
			}
			else if (m.x > 620 && m.x < 680 && m.y > 265 && m.y < 305) {
				setcolor(BLUE);
				rectangle(620, 265, 680, 305);
				outtextxy(634, 276, _T("悔棋"));
				setcolor(BLACK); 
			}
			break;

		case WM_LBUTTONUP:
			/*下棋*/
			if (mychs.Get_xy(x, y) && map[reswitch_xy(x)][reswitch_xy(y)] == -1) {
				int x1 = reswitch_xy(x), y1 = reswitch_xy(y);
				stk.push({ aiset, (x1 << 5) | y1 });
				map[x1][y1] = 1;
				mychs.now = false;
				mychs.steps++;
				myPos = mychs.GetNowPoints(1);
				aiPos = mychs.GetNowPoints(0);
				mciSendString(_T("close chs"), NULL, 0, NULL);
				mciSendString(_T("open setChs.wav alias chs"), NULL, 0, NULL);
				mciSendString(_T("play chs"), NULL, 0, NULL);
				return;
			}
			/*悔棋*/
			else if (m.x > 620 && m.x < 680 && m.y > 265 && m.y < 305) {
				if (!user.restcnt){
					MessageBox(0, _T("您没有悔棋机会了，加油赢棋以获得更多机会吧！"), _T("NO"), 0);
					continue;
				}
				if (stk.empty()) {
					MessageBox(0, _T("现在无法悔棋！"), _T("NO"), 0);
					continue;
				}				
				auto it = stk.top();
				stk.pop();
				int ai = it.first, self = it.second;			
				map[self >> 5][self & 0x1f] = -1;
				map[aiset >> 5][aiset & 0x1f] = -1;
				aiset = ai;

				myPos = mychs.GetNowPoints(1);
				aiPos = mychs.GetNowPoints(0);
				user.restcnt--;
				user.Write();
				show(mychs, user);
				return Mouse(mychs, user);
			}
		}
	}
	return;
}

void Show::End(int who, Users& user, int steps){
	if (!who) return;
	LOGFONT f;
	gettextstyle(&f);						// 获取当前字体设置
	f.lfHeight = 20;						// 设置字体高度为 48
	wcscpy_s(f.lfFaceName, _T("黑体"));		// 设置字体为“黑体”(高版本 VC 推荐使用 _tcscpy_s 函数)
	f.lfQuality = ANTIALIASED_QUALITY;		// 设置输出效果为抗锯齿  
	settextstyle(&f);						// 设置字体样式
	outtextxy(615, 500, _T("游戏结束"));
	if (who == 1) {
		MessageBox(0, _T("恭喜您胜利！"), _T("YES"), 0);
		setbkcolor(GREEN);
		cleardevice();
		outtextxy(250, 200, _T("恭喜您胜利！"));	
		outtextxy(250, 300, _T("请按任意键继续游戏..."));
		user.wincnt++;
		user.restcnt += 10;
		if (user.minsteps == 0)
			user.minsteps = steps;
		else
			user.minsteps = min(user.minsteps, steps);
		mciSendString(_T("close win"), NULL, 0, NULL);
		mciSendString(_T("open win.wav alias win"), NULL, 0, NULL);
		mciSendString(_T("play win"), NULL, 0, NULL);
	}
	else {
		MessageBox(0, _T("您失败了，不要灰心下次再来吧！！"), _T("NO"), 0);
		setbkcolor(GREEN);
		cleardevice();
		outtextxy(250, 200, _T("您失败了，不要灰心下次再来吧！"));
		outtextxy(250, 300, _T("请按任意键继续游戏..."));
		mciSendString(_T("close det"), NULL, 0, NULL);
		mciSendString(_T("open defet.mp3 alias det"), NULL, 0, NULL);
		mciSendString(_T("play det"), NULL, 0, NULL);
		user.failcnt++;
	}
	user.Write();
	int ch = _gettch();
	while (!stk.empty())		stk.pop();
	aiPos = 0;
	myPos = 0;
	closegraph();
	return;
}

