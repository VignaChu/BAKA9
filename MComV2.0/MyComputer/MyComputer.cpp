#include <iostream>
#include <tchar.h>
#include <queue>
#include <cmath>
#include <ctime>
#include <string>
#include <time.h>
#include <graphics.h>
#include <stdlib.h>
#include <list>
#include <vector>
#include <cstdlib>
#include <conio.h>

using namespace std;
#pragma comment(lib,"MSIMG32.LIB")
#pragma warning(disable:4996)；
void putimage_new(int x, int y, IMAGE* img);

short FLBdesk[4][4] = { 8 };
short FLBcolor[8] = { 0 };
RECT FLBColorRECT[4][4];
bool is_move_up = 0;
bool is_move_down = 0;
bool is_move_left = 0;
bool is_move_right = 0;
short v = 3;

constexpr int FLBblock = 90 * 2;
constexpr int FLBframe = 20 * 2;
constexpr int FLBtable = FLBblock * 4 + FLBframe * 2;
constexpr auto swidth = 1100;
constexpr auto sheight = 800;
constexpr int HeroWidth = 60;
constexpr int HeroHeight = 90;
constexpr int icon_gap = 100;
constexpr unsigned int SHP = 3;
constexpr unsigned int ICON = 32;
constexpr unsigned int bullet_height = 64;
constexpr unsigned int bullet_weight = 32;
constexpr auto hurttime = 1000;

enum
{
	MCPbk_weight = 128 * 8,
	MCPbk_height = 96 * 8,
	MCPbk_gap = 16 * 1,
	WBI_gp = 2 * 1,
	LoadingSleepTime = 128,
	icon_size = 16 * 4,
};

struct FLBtemp_message
{
	POINT point;
	short color_type;
};
class BK
{
public:
	BK(IMAGE& img)
		:img(img), y(-sheight)
	{

	}
	void Show()
	{
		if (y == 0) 
		{ 
			y = -sheight;
		}
		y += 4;
		putimage(112, y, &img);
	}

private:
	IMAGE& img;
	int y;

};
class Hero
{
public:
	Hero(IMAGE& img)
		:img(img), HP(SHP)
	{
		rect.left = swidth / 2 + 10;
		rect.top = sheight - HeroHeight + 35;
		rect.right = rect.left + HeroWidth - 40;
		rect.bottom = sheight - 35;
	}
	void Show()
	{
		setlinecolor(RED);
		setlinestyle(PS_SOLID, 4);
		putimage_new(rect.left - 15, rect.top, &img);
		line(rect.left, rect.top - 5, rect.left + (HeroWidth / SHP * HP), rect.top - 5);
		setlinecolor(MAGENTA);
		rectangle(rect.left, rect.top, rect.right, rect.bottom);
	}
	void HeroControl()
	{
		ExMessage msg;
		while (peekmessage(&msg))
		{
			if (msg.message == WM_KEYDOWN)
			{
				switch (msg.vkcode)
				{
				case VK_UP:
					is_move_up = 1;
					break;
				case VK_DOWN:
					is_move_down = 1;
					break;
				case VK_LEFT:
					is_move_left = 1;
					break;
				case VK_RIGHT:
					is_move_right = 1;
					break;
				default:
					break;
				}
			}
			if (msg.message == WM_KEYUP)
			{
				switch (msg.vkcode)
				{
				case VK_UP:
					is_move_up = 0;
					break;
				case VK_DOWN:
					is_move_down = 0;
					break;
				case VK_LEFT:
					is_move_left = 0;
					break;
				case VK_RIGHT:
					is_move_right = 0;
					break;
				default:
					break;
				}
			}
		}

		if (is_move_up && rect.top > 0)
		{
			rect.top -= v;
			rect.bottom -= v;
		}
		if (is_move_down && rect.top < 800 - HeroHeight * 2 / 3)
		{
			rect.top += v;
			rect.bottom += v;
		}
		if (is_move_left && rect.left > 110)
		{
			rect.left -= v;
			rect.right -= v;
		}
		if (is_move_right && rect.left < 660)
		{
			rect.left += v;
			rect.right += v;
		}
	}

	bool hurt()
	{
		HP--;
		return (HP == 0) ? false : true;
	}


	RECT& GetRect()
	{
		return rect;
	}
private:
	IMAGE& img;
	RECT rect;
	POINT point;
	POINT draw_point;
	unsigned int HP;
	unsigned short HP_piece = 0;

};
class Bullet
{
public:
	Bullet(IMAGE& img, POINT xy)
		:img(img)
	{
		rect.left = xy.x + (HeroWidth) / 2 - img.getwidth() / 2;
		rect.right = xy.x + img.getwidth();
		rect.top = xy.y - img.getheight();
		rect.bottom = xy.y + img.getheight() - 10;
		cout << "bullet loading\n";
	}
	Bullet(IMAGE& img, RECT rect)
		:img(img), rect(rect)
	{
		cout << "bullet loading\n";
	}
	bool Show()
	{
		if (rect.bottom <= 0)
		{
			return false;
		}
		rect.top -= 3;
		rect.bottom -= 3;
		putimage_new(rect.left, rect.top, &img);

		return true;
	}
	RECT& GetRect() { return rect; }

protected:
	IMAGE& img;
	RECT rect;
};
class HPEnemy
{
public:
	HPEnemy(IMAGE& img, int x)
		:img(img), isdie(false), boomsum(0)
	{
		rect.left = x;
		rect.right = rect.left + img.getwidth();
		rect.top = -img.getheight();
		rect.bottom = 0;
	}
	bool Show()
	{

		if (isdie)
		{
			if (boomsum == 3)
			{
				return false;
			}
			putimage_new(rect.left, rect.top, selfboom + boomsum);
			boomsum++;

			return true;
		}

		if (rect.top >= sheight)
		{
			return false;
		}
		setlinecolor(YELLOW);
		circle(rect.left, rect.top, 5);
		rect.top += 4;
		rect.bottom += 4;
		putimage_new(rect.left, rect.top, &img);

		return true;
	}

	void Isdie()
	{
		isdie = true;
	}

	RECT& GetRect() { return rect; }

private:
	IMAGE& img;
	RECT rect;
	IMAGE selfboom[3];
	short health = 10;
	bool isdie;
	int boomsum;
};
class EBullet : public Bullet
{
public:
	EBullet(IMAGE& img, RECT pr)
		:Bullet(img, pr)
	{
		rect.left = pr.left + (pr.right - pr.left) / 2 - img.getwidth() / 2;
		rect.right = rect.left + img.getwidth();
		rect.top = pr.bottom;
		rect.bottom = rect.top + img.getheight();
		cout << "bullet loading\n";
	}
	bool Show()
	{
		if (rect.top >= sheight)
		{
			return false;
		}
		rect.top += 5;
		rect.bottom += 5;
		putimage_new(rect.left, rect.top, &img);

		return true;
	}
};


bool MyCPmain();
void MyCPclose();
void FLBBegining();
bool FLBPointInRect(int x, int y, RECT& r);
bool FLBWelcome();
void FLBBlockPrepare();
void FLBDraw();
bool FLBPlay();
bool FLBPointsame(POINT a, POINT b);
void FLBColorGameMain();
bool PointInRect(int x, int y, RECT& r);
bool RectDuangRect(RECT& r1, RECT& r2);
bool HWelcome();
void Over(unsigned long long& kill);
bool AddEnemy(vector<HPEnemy*>& es, IMAGE& enemyimg);
bool Play();
bool STPlay();

int main()
{
    MyCPmain();
    MyCPclose();
    system("pause");
}

void putimage_new(int x, int y, IMAGE* img)
{
    int w = img->getwidth();
    int h = img->getheight();
    AlphaBlend(GetImageHDC(NULL), x, y, w, h, GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}
bool MyCPmain()
{
    initgraph(MCPbk_weight, MCPbk_height, EX_NOCLOSE| EX_NOMINIMIZE);
    setbkcolor(RGB(50,79,252));
    
    cleardevice();
    RECT IsgoingloadingRect = { 0,0,MCPbk_weight,MCPbk_height};
    settextstyle(96, 64, _T("宋体"));
    for (int qwq = 0; qwq < 3; qwq++)
    {
        drawtext(_T("loading..."), &IsgoingloadingRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        Sleep(LoadingSleepTime);
        cleardevice();
        drawtext(_T("loading.."), &IsgoingloadingRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        Sleep(LoadingSleepTime);
        cleardevice();
        drawtext(_T("loading."), &IsgoingloadingRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        Sleep(LoadingSleepTime);
        cleardevice();
        drawtext(_T("loading.."), &IsgoingloadingRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        Sleep(LoadingSleepTime);
        cleardevice();
        drawtext(_T("loading..."), &IsgoingloadingRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        Sleep(LoadingSleepTime);
        cleardevice();
        drawtext(_T("loading.."), &IsgoingloadingRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        Sleep(LoadingSleepTime);
        cleardevice();
        drawtext(_T("loading."), &IsgoingloadingRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        Sleep(LoadingSleepTime);
        cleardevice();
        drawtext(_T("loading"), &IsgoingloadingRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        Sleep(10);
    }
    drawtext(_T("欢迎登录"), &IsgoingloadingRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    Sleep(LoadingSleepTime / 2);
    settextstyle(128, 96, _T("宋体"));
    drawtext(_T("欢迎登录"), &IsgoingloadingRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    Sleep(LoadingSleepTime * 3 / 2);
    cleardevice();
    IMAGE MCPbkimg;
    setbkmode(TRANSPARENT);
    loadimage(&MCPbkimg, _T("source/MCPBK_main.png"),MCPbk_weight,MCPbk_height);
    RECT QUT_choice = { MCPbk_gap,MCPbk_height-MCPbk_gap*3/2-icon_size,MCPbk_gap+icon_size,MCPbk_height - MCPbk_gap*3/2 };
    RECT SHT_choice = { MCPbk_gap,MCPbk_height / 7 * 1,MCPbk_gap + icon_size,MCPbk_height / 7 * 1 + icon_size };
    RECT VGP_choice = { MCPbk_gap,MCPbk_height / 7 * 2,MCPbk_gap + icon_size,MCPbk_height / 7 * 2 + icon_size };
    RECT CCL_choice = { MCPbk_gap,MCPbk_height / 7 * 3,MCPbk_gap + icon_size,MCPbk_height / 7 * 3 + icon_size };
    RECT DPT_choice = { MCPbk_gap,MCPbk_height / 7 * 4,MCPbk_gap + icon_size,MCPbk_height / 7 * 4 + icon_size };
    RECT FLB_choice = { MCPbk_gap,MCPbk_height / 7 * 5,MCPbk_gap + icon_size,MCPbk_height / 7 * 5 + icon_size };
    IMAGE QUT_icon;
    IMAGE SHT_icon;
    IMAGE VGP_icon;
    IMAGE CCL_icon;
    IMAGE DPT_icon;
    IMAGE FLB_icon;
    loadimage(&QUT_icon, _T("source/QUTicon.png"), icon_size, icon_size);
    loadimage(&SHT_icon, _T("source/SHTicon.png"), icon_size, icon_size);
    loadimage(&VGP_icon, _T("source/VGPicon.png"), icon_size, icon_size);
    loadimage(&DPT_icon, _T("source/DPTicon.png"), icon_size, icon_size);
    loadimage(&CCL_icon, _T("source/CCLicon.png"), icon_size, icon_size);
    loadimage(&FLB_icon, _T("source/FLBicon.png"), icon_size, icon_size);
    TCHAR QUT_char[] = _T("关机");
    TCHAR SHT_char[] = _T("躲避子弹");
    TCHAR VGP_char[] = _T("生存挑战");
    TCHAR CCL_char[] = _T("简单计算器");
    TCHAR DPT_char[] = _T("绘图");
    TCHAR FLB_char[] = _T("找找方块");
    TCHAR Time_char[20];
    BeginBatchDraw();
    while (true)
    {
        putimage_new(0, 0, &MCPbkimg);
        settextstyle(32, 16, _T("宋体"));
        drawtext(_T("Baka_9系统"), &IsgoingloadingRect, DT_BOTTOM| DT_SINGLELINE| DT_RIGHT);
        putimage_new(QUT_choice.left, QUT_choice.top,&QUT_icon);
        putimage_new(SHT_choice.left, SHT_choice.top, &SHT_icon);
        putimage_new(VGP_choice.left, VGP_choice.top, &VGP_icon);
        putimage_new(DPT_choice.left, DPT_choice.top, &DPT_icon);
        putimage_new(CCL_choice.left, CCL_choice.top, &CCL_icon);
        putimage_new(FLB_choice.left, FLB_choice.top, &FLB_icon);
        settextstyle(16, 0, _T("宋体"));
        outtextxy(QUT_choice.left, QUT_choice.top+icon_size+WBI_gp, QUT_char);
        outtextxy(SHT_choice.left, SHT_choice.top + icon_size + WBI_gp, SHT_char);
        outtextxy(VGP_choice.left, VGP_choice.top + icon_size + WBI_gp, VGP_char);
        outtextxy(CCL_choice.left, CCL_choice.top + icon_size + WBI_gp, CCL_char);
        outtextxy(DPT_choice.left, DPT_choice.top + icon_size + WBI_gp, DPT_char);
        outtextxy(FLB_choice.left, FLB_choice.top + icon_size + WBI_gp, FLB_char);
        time_t timep;
        struct tm* tp;
        time(&timep);
        tp = localtime(&timep);
        settextstyle(24, 12, _T("楷体"));
        _stprintf(Time_char, _T("%d/%d/%d %02d:%02d:%02d\n"), 1900+tp->tm_year,1+tp->tm_mon,tp->tm_mday,tp->tm_hour,tp->tm_min,tp->tm_sec);
        outtextxy(MCPbk_weight-MCPbk_gap-icon_size*13/4, MCPbk_height-icon_size, Time_char);
        ExMessage mess;
        getmessage(&mess, EM_MOUSE);
        POINT choice;
        choice.x = mess.x;
        choice.y = mess.y;
        if (mess.lbutton)
        {
            if (PtInRect(&QUT_choice, choice))
            {
                break;
            }
            else if (PtInRect(&FLB_choice, choice))
            {
				FLBColorGameMain();
				initgraph(MCPbk_weight, MCPbk_height, EX_NOCLOSE | EX_NOMINIMIZE);
				setbkmode(TRANSPARENT);
				
            }
            else if (PtInRect(&SHT_choice, choice))
            {
				STPlay();
				initgraph(MCPbk_weight, MCPbk_height, EX_NOCLOSE | EX_NOMINIMIZE);
				setbkmode(TRANSPARENT);
            }
			else
			{
				cout << "a" << endl;
			}
           //else if (PtInRect(&tsetar, choice))
            {
                
            }
        }
        FlushBatchDraw();
    }
    Sleep(100);
    EndBatchDraw();
    return 0;
}
void MyCPclose()
{
    cout << "aaa" << endl;
    RECT IsgoingloadingRect = { 0,0,MCPbk_weight,MCPbk_height };
    cleardevice();
    drawtext(_T("正在关机..."), &IsgoingloadingRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    Sleep(3*LoadingSleepTime);
    setbkcolor(BLACK);
    cleardevice();
}
void FLBBegining()
{
	fill(FLBcolor, FLBcolor + 7, 0);
	initgraph(FLBtable, FLBtable, EX_NOCLOSE | EX_NOMINIMIZE);
	for (short z = 0; z < 4; z++)
	{
		for (short c = 0; c < 4; c++)
		{
			FLBColorRECT[z][c].left = FLBframe + z * FLBblock;
			FLBColorRECT[z][c].right = FLBColorRECT[z][c].left + FLBblock;
			FLBColorRECT[z][c].top = FLBframe + c * FLBblock;
			FLBColorRECT[z][c].left = FLBColorRECT[z][c].top + FLBblock;
			cout << FLBColorRECT[z][c].top << endl;;
		}
	}
}
bool FLBPointInRect(int x, int y, RECT& r)
{
	return (r.left <= x && x <= r.right && r.top <= y && y <= r.bottom);
}
bool FLBWelcome()
{
	LPCTSTR title = _T("找找方块");
	LPCTSTR tplay = _T("开始游戏");
	LPCTSTR texit = _T("退出游戏");

	RECT tplayr = { 0,0,0,0 }, texitr = { 0,0,0,0 };
	BeginBatchDraw();
	setbkcolor(RGB(95, 163, 255));
	cleardevice();
	settextstyle(60, 0, _T("黑体"));
	settextcolor(BLACK);
	outtextxy(FLBtable / 2 - textwidth(title) / 2, FLBtable / 5, title);

	settextstyle(40, 0, _T("黑体"));
	tplayr.left = FLBtable / 2 - textwidth(tplay) / 2;
	tplayr.right = tplayr.left + textwidth(tplay);
	tplayr.top = FLBtable / 5 * 2.5;
	tplayr.bottom = tplayr.top + textheight(tplay);

	texitr.left = FLBtable / 2 - textwidth(texit) / 2;
	texitr.right = texitr.left + textwidth(texit);
	texitr.top = FLBtable / 5 * 3;
	texitr.bottom = texitr.top + textheight(texit);

	outtextxy(tplayr.left, tplayr.top, tplay);
	outtextxy(texitr.left, texitr.top, texit);

	EndBatchDraw();

	while (true)
	{
		ExMessage mess;
		getmessage(&mess, EM_MOUSE);
		if (mess.lbutton)
		{
			if (FLBPointInRect(mess.x, mess.y, tplayr))
			{
				return 1;
			}
			else if (FLBPointInRect(mess.x, mess.y, texitr))
			{
				return 0;
			}
		}
	}
}
void FLBBlockPrepare()
{
	for (int i = 0; i < 8; i++)
	{
		FLBcolor[i] = 0;
	}
	short temp_1 = 0;
	srand(time(NULL));
	for (short q = 0; q < 4; q++)
	{
		for (short w = 0; w < 4; w++)
		{
			FLBflag_1:
			short temp_1 = rand() % 8;
			if (FLBcolor[temp_1] < 2)
			{
				FLBdesk[q][w] = temp_1;
				FLBcolor[temp_1]++;
				cout << "h";
			}
			else
			{
				goto FLBflag_1;
			}
		}
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			cout << FLBdesk[i][j] << " ";
		}
		cout << endl;
	}
}
void FLBDraw()
{
	setbkcolor(LIGHTGRAY);
	cleardevice();
	setlinecolor(BLACK);
	rectangle(FLBframe, FLBframe, FLBtable - FLBframe, FLBtable - FLBframe);
	for (short i = 1; i <= 3; i++)
	{
		line(FLBframe + FLBblock * i, FLBframe, FLBframe + FLBblock * i, FLBtable - FLBframe);
		line(FLBframe, FLBframe + FLBblock * i, FLBtable - FLBframe, FLBframe + FLBblock * i);
	}
	for (short x = 0; x < 4; x++)
	{
		for (short y = 0; y < 4; y++)
		{
			switch (FLBdesk[x][y])
			{
			case 0:
				setfillcolor(RGB(179, 63, 76));  //醒酒色179 63 76  我推的发色
				fillrectangle(FLBframe + x * FLBblock, FLBframe + y * FLBblock, FLBframe + (x + 1) * FLBblock, FLBframe + (y + 1) * FLBblock);
				setfillcolor(LIGHTGRAY);
				break;
			case 1:
				setfillcolor(MAGENTA);  //品红色
				fillrectangle(FLBframe + x * FLBblock, FLBframe + y * FLBblock, FLBframe + (x + 1) * FLBblock, FLBframe + (y + 1) * FLBblock);
				setfillcolor(LIGHTGRAY);
				break;
			case 2:
				setfillcolor(RGB(36, 132, 189));  //窗外蓝24 84 BD
				fillrectangle(FLBframe + x * FLBblock, FLBframe + y * FLBblock, FLBframe + (x + 1) * FLBblock, FLBframe + (y + 1) * FLBblock);
				setfillcolor(LIGHTGRAY);
				break;
			case 3:
				setfillcolor(RGB(149, 46, 99));  //波森梅95 2E 63
				fillrectangle(FLBframe + x * FLBblock, FLBframe + y * FLBblock, FLBframe + (x + 1) * FLBblock, FLBframe + (y + 1) * FLBblock);
				setfillcolor(LIGHTGRAY);
				break;
			case 4:
				setfillcolor(RGB(73, 166, 144));  //浅海薄荷49 A6 90
				fillrectangle(FLBframe + x * FLBblock, FLBframe + y * FLBblock, FLBframe + (x + 1) * FLBblock, FLBframe + (y + 1) * FLBblock);
				setfillcolor(LIGHTGRAY);
				break;
			case 5:
				setfillcolor(RGB(219, 188, 81));  //姜黄色DB BC 51
				fillrectangle(FLBframe + x * FLBblock, FLBframe + y * FLBblock, FLBframe + (x + 1) * FLBblock, FLBframe + (y + 1) * FLBblock);
				setfillcolor(LIGHTGRAY);
				break;
			case 6:
				setfillcolor(RGB(249, 224, 218));  //淡粉色F9 E0 DA    我推的肤色
				fillrectangle(FLBframe + x * FLBblock, FLBframe + y * FLBblock, FLBframe + (x + 1) * FLBblock, FLBframe + (y + 1) * FLBblock);
				setfillcolor(LIGHTGRAY);
				break;
			case 7:
				setfillcolor(RGB(213, 120, 90));  //兵马俑D5 78 5A
				fillrectangle(FLBframe + x * FLBblock, FLBframe + y * FLBblock, FLBframe + (x + 1) * FLBblock, FLBframe + (y + 1) * FLBblock);
				setfillcolor(LIGHTGRAY);
				break;
			defalut:
				cout << "ERROR";
				break;
			}
		}
	}
}
bool FLBPlay()
{
	DWORD FLBbegin_t = GetTickCount();
	FLBtemp_message temp_empty = { {0,0},8 };
	FLBtemp_message FLBtemp1 = temp_empty;
	ExMessage msg;
	settextstyle(10, 10, _T("黑体"));
	short FLBscore = 0;
	bool cho = 0;
	POINT choice_POINT = { 0,0 };
	short choice_col = 8;
	RECT choice_RECT = { 0,0,0,0 };
	while (FLBscore != 8)
	{
		char FBarr[514];
		DWORD FLBstart_time = GetTickCount();
		BeginBatchDraw();
		outtextxy(10, 10, L"当前分数为：");
		outtextxy(114, 9, char(FLBscore + '0'));
		outtextxy(233, 10, L"当前耗时为：");
		_itoa_s(FLBstart_time - FLBbegin_t + 4, FBarr, 10);
		outtextxy(340, 9, *FBarr);
		while (peekmessage(&msg))
		{
			if (msg.message == WM_LBUTTONDOWN)
			{
				POINT check = { msg.x,msg.y };
				POINT FLBxy = { (check.x - FLBframe) / FLBblock ,(check.y - FLBframe) / FLBblock };
				cout << FLBxy.x << " " << FLBxy.y << ' ' << FLBdesk[FLBxy.x][FLBxy.y] << endl;
				if (cho == 0)
				{
					FLBtemp1.point = FLBxy;
					FLBtemp1.color_type = FLBdesk[FLBxy.x][FLBxy.y];
					cho = 1;
				}
				else if (cho == 1)
				{
					if (FLBdesk[FLBxy.x][FLBxy.y] == FLBtemp1.color_type && !FLBPointsame(FLBtemp1.point, FLBxy))
					{
						fillrectangle(FLBxy.x * FLBblock + FLBframe, FLBxy.y * FLBblock + FLBframe, FLBframe + FLBxy.x * FLBblock + FLBblock, FLBxy.y * FLBblock + FLBblock + FLBframe);
						fillrectangle(FLBtemp1.point.x * FLBblock + FLBframe, FLBtemp1.point.y * FLBblock + FLBframe, FLBframe + FLBtemp1.point.x * FLBblock + FLBblock, FLBtemp1.point.y * FLBblock + FLBblock + FLBframe);
						FLBscore++;
						FLBtemp1 = temp_empty;
					}
					else
					{
						FLBtemp1 = temp_empty;
					}
					cho = 0;
				}
			}
		}
		EndBatchDraw();
		DWORD FLBfinish_time = GetTickCount();
		DWORD FLBdtime = FLBfinish_time - FLBstart_time;
		if (FLBdtime < 1000 / 60)
		{
			Sleep(1000 / 60 - FLBdtime);
		}
	}
	return 1;
}
bool FLBPointsame(POINT a, POINT b)
{
	if (a.x == b.x && a.y == b.y)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
void FLBColorGameMain()
{
	system("cls 找相同颜色的方块并点击");
	FLBBegining();
	if (FLBWelcome())
	{
		FLBBlockPrepare();
		FLBDraw();
		FLBPlay();
		outtextxy(10, 10, L"当前分数为：");
		outtextxy(114, 9, '8');
		cout << "niyingli";
		Sleep(495);
		MessageBox(GetHWnd(), _T("您成功了"), _T("游戏结束,您可真行"), MB_OK);
	}
}
bool PointInRect(int x, int y, RECT& r)
{
	return (r.left <= x && x <= r.right && r.top <= y && y <= r.bottom);
}
bool RectDuangRect(RECT& r1, RECT& r2)
{
	RECT r;
	r.left = r1.left - (r2.right - r2.left);
	r.right = r1.right;
	r.top = r1.top - (r2.bottom - r2.top);
	r.bottom = r1.bottom;

	return (r.left < r2.left && r2.left <= r.right && r.top <= r2.top && r2.top <= r.bottom);
}
bool HWelcome()
{
	LPCTSTR title = _T("NULL");
	LPCTSTR tplay = _T("开始游戏");
	LPCTSTR texit = _T("退出游戏");

	RECT tplayr, texitr;
	BeginBatchDraw();
	setbkcolor(WHITE);
	cleardevice();
	setbkmode(TRANSPARENT);
	IMAGE LBK;
	loadimage(&LBK, _T("source/tempfirst.png"));
	putimage(0, 0, &LBK);
	settextstyle(60, 0, _T("黑体"));
	settextcolor(BLACK);
	outtextxy(swidth / 2 - textwidth(title) / 2, sheight / 5, title);

	settextstyle(40, 0, _T("黑体"));
	tplayr.left = swidth / 2 - textwidth(tplay) / 2;
	tplayr.right = tplayr.left + textwidth(tplay);
	tplayr.top = sheight / 5 * 2.5;
	tplayr.bottom = tplayr.top + textheight(tplay);

	texitr.left = swidth / 2 - textwidth(texit) / 2;
	texitr.right = texitr.left + textwidth(texit);
	texitr.top = sheight / 5 * 3;
	texitr.bottom = texitr.top + textheight(texit);

	outtextxy(tplayr.left, tplayr.top, tplay);
	outtextxy(texitr.left, texitr.top, texit);

	EndBatchDraw();

	while (true)
	{
		ExMessage mess;
		getmessage(&mess, EM_MOUSE);
		if (mess.lbutton)
		{
			if (PointInRect(mess.x, mess.y, tplayr))
			{
				cout << "hng" << endl;
				return 1;
			}
			else if (PointInRect(mess.x, mess.y, texitr))
			{ 
				cout << "ha" << endl;
				return 0;//得改
			}
		}
	}
}
void Over(unsigned long long& kill)
{
	printf_s("o");
	TCHAR* str = new TCHAR[128];
	_stprintf_s(str, 128, _T("击杀数：%llu"), kill);

	settextcolor(RED);
	outtextxy(swidth / 2 - textwidth(str) / 2, sheight / 5, str);

	// 键盘事件 （按Enter返回）
	LPCTSTR info = _T("按Enter返回");
	settextstyle(20, 0, _T("黑体"));
	outtextxy(swidth - textwidth(info), sheight - textheight(info), info);

	while (true)
	{
		ExMessage mess;
		getmessage(&mess, EM_KEY);
		if (mess.vkcode == 0x0D)
		{
			return;
		}
	}
}
bool AddEnemy(vector<HPEnemy*>& es, IMAGE& enemyimg)
{
	HPEnemy* e = new HPEnemy(enemyimg, abs(rand()) % (swidth - enemyimg.getwidth() - 112 - 288 - icon_gap) + 112);
	for (auto& i : es)
	{
		if (RectDuangRect(i->GetRect(), e->GetRect()))
		{
			delete e;
			return false;
		}
	}
	es.push_back(e);
	return true;

}
bool Play()
{
	setbkcolor(WHITE);
	cleardevice();
	bool is_play = true;

	IMAGE heroimg, enemyimg, bkimg1, mybulletimg, bkimg2, enemybuttle;

	loadimage(&heroimg, _T("source/myself.png"), HeroWidth, HeroHeight);
	loadimage(&enemyimg, _T("source/enemy.png"), HeroWidth, HeroWidth);
	loadimage(&bkimg1, _T("source/temp_main.png"));
	loadimage(&bkimg2, _T("source/temp_background.png"));
	loadimage(&mybulletimg, _T("source/my_buttle.png"), HeroWidth / 2, HeroHeight / 2);
	loadimage(&enemybuttle, _T("source/enemy_buttle3.png"), bullet_weight, bullet_height);


	BK bk = BK(bkimg2);
	Hero hp = Hero(heroimg);

	vector<HPEnemy*> es;
	vector<Bullet*> bs;
	vector<EBullet*> ebs;
	int bsing = 0;

	clock_t hurtlast = clock();

	unsigned long long kill = 0;
	BeginBatchDraw();

	while (is_play)
	{
		DWORD begin_time = GetTickCount();
		bsing++;
		if (bsing % 10 == 0)
		{
			POINT temp_point = { hp.GetRect().left, hp.GetRect().top };
			bs.push_back(new Bullet(mybulletimg, temp_point));
		}
		if (bsing == 96)
		{
			for (int i = 0; i < 7; i++)
			{
				AddEnemy(es, enemyimg);
			}
			bsing = 0;
		}


		putimage(0, 0, &bkimg1);
		bk.Show();
		hp.HeroControl();
		flushmessage();
		hp.Show();
		auto bsit = bs.begin();
		while (bsit != bs.end())
		{
			if (!(*bsit)->Show())
			{
				bsit = bs.erase(bsit);
			}
			else
			{
				bsit++;
			}
		}
	a:
		auto esit = es.begin();
		while (esit != es.end())
		{
			if ((*esit)->Show() == 0)
			{
				esit = es.erase(esit);
			}
			else
			{
				if (RectDuangRect((*esit)->GetRect(), hp.GetRect()))
				{
					if (clock() - hurtlast >= hurttime)
					{
						is_play = hp.hurt();
						hurtlast = clock();
						esit = es.erase(esit);

						continue;
					}
				}
				bsit = bs.begin();
				while (bsit != bs.end())
				{
					if (RectDuangRect((*esit)->GetRect(), (*bsit)->GetRect()))
					{
						esit = es.erase(esit);
						bsit = bs.erase(bsit);
						kill++;
						goto a;
					}
					else
					{
						bsit++;
					}
				}
				if (bsing % 20 == 0)
				{
					RECT temp_point02 = { (*esit)->GetRect().left, (*esit)->GetRect().top ,(*esit)->GetRect().left + enemybuttle.getwidth(), (*esit)->GetRect().top + enemybuttle.getheight() };
					ebs.push_back(new EBullet(enemybuttle, temp_point02));
				}
				esit++;
			}
		}
		auto ebsit = ebs.begin();
		while (ebsit != ebs.end())
		{
			if (!(*ebsit)->Show())
			{
				ebsit = ebs.erase(ebsit);
			}
			else
			{
				if (RectDuangRect((*ebsit)->GetRect(), hp.GetRect()))
				{
					if (clock() - hurtlast >= hurttime)
					{
						is_play = hp.hurt();
						hurtlast = clock();
						ebsit = ebs.erase(ebsit);
						continue;
					}
				}
				ebsit++;
			}

		}
		DWORD end_time = GetTickCount();
		DWORD d_time = end_time - begin_time;
		//结束绘图
		FlushBatchDraw();
		//休眠
		if (d_time < 1000 / 144)
		{
			Sleep(1000 / 144 - d_time);
		}

	}
	EndBatchDraw();
	printf_s("e");
	Over(kill);
	return true;
}
bool STPlay()
{
	initgraph(swidth, sheight, EW_NOMINIMIZE | EW_SHOWCONSOLE);
	bool is_live0 = true;
	while (is_live0)
	{
		if(HWelcome())
		{
			is_live0 = Play();
		}
		else
		{
			is_live0 = 0;
		}
	}
	return 0;
}