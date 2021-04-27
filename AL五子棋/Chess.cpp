#include "Chess.h"
#include "Show.h"
#include <vector>
#pragma comment(lib,"Winmm.lib")
extern vector<vector<int>> map;
extern int aiset;
extern double aiPos, myPos;
int MyChess::Get_xy(int& x, int& y)//判断鼠标是否落入空交点内
{
	for (int i = 0; i < 18; i++) {
		for (int j = 0; j < 18; j++) {
			if ((x - switch_xy(i)) * (x - switch_xy(i)) + (y - switch_xy(j)) * (y - switch_xy(j)) <= 36) {
				x = switch_xy(i);
				y = switch_xy(j);
				return 1;
			}
		}
	}
	return 0;
}


void AiChess::Get_key_Setchess(MyChess& mychs)
{
	if (mychs.steps == 0) {
		map[8][8] = 0;
		aiset = 0x108;
	}
	else {
		int points, maxpoints = -1, maxx = 0, maxy = 0;
		int book = 0, cnt = 324 - 2 * mychs.steps;
		vector<vector<int>> visit(map);
		/*记录当前需要遍历的次数*/
		while (book != cnt) {
			int i, j;
			/*使得电脑下棋尽可能随机化*/
			srand((unsigned)time(0));
			do {
				i = rand() % 18;
				j = rand() % 18;
			} while (visit[i][j] != -1);
			visit[i][j] = 0;
			points = Get_Points(i, j, 1);
			/*获取最大的分点*/
			if (points > maxpoints) {
				maxpoints = points;
				maxx = i;
				maxy = j;
			}
			book++;
		}
		aiset = (maxx << 5) | maxy;
		map[maxx][maxy] = 0;
	}
	myPos = mychs.GetNowPoints(1);
	aiPos = mychs.GetNowPoints(0);
	Sleep(500);	/*为了让上一次的音效播放完，必须让他停顿一会*/
	mciSendString(_T("close chs"), NULL, 0, NULL);
	mciSendString(_T("open setChs.wav alias chs"), NULL, 0, NULL);
	mciSendString(_T("play chs"), NULL, 0, NULL);
	return ;
}

int Chess::Get_Points(int x, int y, int ch)
{
	int ret = 0;
	int x1(x), y1(y), x2(x), y2(y), x3(x), y3(y), x4(x), y4(y);
	for (int i = 0; i < 5; i++) {
		if (is_Ok(x1, y1)) {
			ret += x_Points(Get_Nums1(x1, y1), ch);
			y1++;
		}
		if (is_Ok(x2, y2)) {
			ret += x_Points(Get_Nums2(x2, y2), ch);
			x2++;
		}
		if (is_Ok(x3, y3)) {
			ret += x_Points(Get_Nums3(x3, y3), ch);
			x3--, y3++;
		}
		if (is_Ok(x4, y4)) {
			ret += x_Points(Get_Nums4(x4, y4), ch);
			x4++, y4++;
		}
	}
	return ret;
}

vector<int> Chess::Get_Nums1(int x, int y)
{
	int k = 0, t = 0, cnt = 5;
	while (cnt-- && is_Ok(x,y)) {
		if (map[x][y] == 0) k++;
		if (map[x][y] == 1)	t++;
		y--;
	}
	if (cnt >= 0)	return {};
	return vector<int>({ k, t });
}

vector<int> Chess::Get_Nums2(int x, int y)
{
	int k = 0, t = 0, cnt = 5;
	while (cnt-- && is_Ok(x, y)) {
		if (map[x][y] == 0) k++;
		if (map[x][y] == 1)	t++;
		x--;
	}
	if (cnt >= 0 )	return {};
	return vector<int>({ k, t });
}

vector<int> Chess::Get_Nums3(int x, int y)
{
	int k = 0, t = 0, cnt = 5;
	while (cnt-- && is_Ok(x, y)) {
		if (map[x][y] == 0) k++;
		if (map[x][y] == 1)	t++;
		x++, y--;
	}
	if (cnt >= 0)	return {};
	return vector<int>({k,t});
}

vector<int> Chess::Get_Nums4(int x, int y) {
	int k = 0, t = 0, cnt = 5;
	while (cnt-- && is_Ok(x, y)) {
		if (map[x][y] == 0) k++;
		if (map[x][y] == 1)	t++;
		x--, y--;
	}
	if (cnt >= 0)	return {};
	return vector<int>({ k,t });
}

int Chess::x_Points(vector<int> nums, int ch)
{
	if (nums.empty())	return 0;
	int g, b;
	if (ch)		 g = nums[0], b = nums[1];/*g是我方，b是电脑，ch = 1则获取电脑得分*/
	else         g = nums[1], b = nums[0];/*g是电脑，b是我方，ch = 0则获取我方得分*/
	if (g == 0 && b == 0)		return 10;
	else if (g == 1 && b == 0)	return 35;
	else if (g == 2 && b == 0)	return 1500;
	else if (g == 3 && b == 0)	return 18000;
	else if (g == 4 && b == 0)	return 1000000;
	else if (g == 0 && b == 1)	return 15;
	else if (g == 0 && b == 2)	return 400;
	else if (g == 0 && b == 3)	return 6000;
	else if (g == 0 && b == 4)	return 150000;
	else if (g != 0 && b != 0)	return 0;
	return 0;
}

double Chess::GetNowPoints(int ch)
{
	double ret = 0;
	for (int i = 0; i < 18; i++) {
		for (int j = 0; j < 18; j++) {
			if (map[i][j] == ch) {
				ret += Get_Points(i, j, ch) / 100.0;
			}
		}
	}
	return ret;
}

bool AiChess::gameOver(Show s, Users& user)
{
	for (int i = 0; i < 18; i++) {
		for (int j = 0; j < 18; j++) {
			if (map[i][j] == 0 && win(i, j)) {
				s.End(-1, user, 0);
				return true;
			}
		}
	}
	return false;
}

bool MyChess::gameOver(Show s, Users& user)
{
	for (int i = 0; i < 18; i++) {
		for (int j = 0; j < 18; j++) {
			if (map[i][j] == 1 && win(i, j)) {
				s.End(1, user, steps);
				return true;
			}
		}
	}
	return false;
}

void MyChess::get_grade(Users user, wchar_t*& wstr)
{
	int w = user.wincnt, f = user.failcnt;
	if (w < 10) {
		if (f < 10)			wstr = const_cast<wchar_t*>(L"默默无闻");
		else if (f < 30)	wstr = const_cast<wchar_t*>(L"逆境之旅");
		else if (f < 60)	wstr = const_cast<wchar_t*>(L"饥寒交迫");
		else if (f < 100)	wstr = const_cast<wchar_t*>(L"露宿街头");
		else				wstr = const_cast<wchar_t*>(L"心如死灰");
	}
	else if (w < 30) {
		if (f < 10)			wstr = const_cast<wchar_t*>(L"小有成色");
		else if (f < 30)	wstr = const_cast<wchar_t*>(L"英勇之士");
		else if (f < 50)	wstr = const_cast<wchar_t*>(L"心怀希望");
		else				wstr = const_cast<wchar_t*>(L"不屈不饶");
	}
	else if (w < 50) {
		if (f < 10)			wstr = const_cast<wchar_t*>(L"黎明之日");
		else if (f < 50)	wstr = const_cast<wchar_t*>(L"年少有为");
		else				wstr = const_cast<wchar_t*>(L"探索者");
	}
	else if (w < 100) {
		if (f < 10)			wstr = const_cast<wchar_t*>(L"闪耀之星");
		else if (f < 50)	wstr = const_cast<wchar_t*>(L"大放光彩");
		else				wstr = const_cast<wchar_t*>(L"才华横溢");
	}
	else if (w < 180) {
		if (f < 10)			wstr = const_cast<wchar_t*>(L"常胜将军");
		else if (f < 50)	wstr = const_cast<wchar_t*>(L"登峰造极");
		else if (f < 200)	wstr = const_cast<wchar_t*>(L"春风得意");
		else				wstr = const_cast<wchar_t*>(L"万众瞩目");
	}
	else
		wstr = const_cast<wchar_t*>(L"群星之巅");
}

void MyChess::get_assuse(wchar_t*& wstr)
{
	double ai = aiPos;
	double my = myPos;
	if (ai > my + 8000) {
		wstr = const_cast<wchar_t*>(L"毫无胜算");
	}
	else if (ai > my + 5000) {
		wstr = const_cast<wchar_t*>(L"危在旦夕");
	}
	else if (ai > my + 2000) {
		wstr = const_cast<wchar_t*>(L"措不及防");
	}
	else if (ai > my + 600) {
		wstr = const_cast<wchar_t*>(L"略显被动");
	}
	else if (ai > my) {
		wstr = const_cast<wchar_t*>(L"均势");
	}
	else if (my > ai + 8000) {
		wstr = const_cast<wchar_t*>(L"锁定胜局");
	}
	else if (my > ai + 5000) {
		wstr = const_cast<wchar_t*>(L"胜券在握");
	}
	else if (my > ai + 2000) {
		wstr = const_cast<wchar_t*>(L"遥遥领先");
	}
	else if (my > ai + 600) {
		wstr = const_cast<wchar_t*>(L"小有优势");
	}
	else
		wstr = const_cast<wchar_t*>(L"均势");
	return;
}

