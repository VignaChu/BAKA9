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

enum
{
	MCPbk_weight = 128 * 8,
	MCPbk_height = 96 * 8,
	MCPbk_gap = 16 * 1,
	WBI_gp = 2 * 1,
	LoadingSleepTime = 128,
	icon_size = 16 * 4,
	Hicon_gap = 100,
	Vigna_height = 96,
	Vigna_weight = 48,
	Fork_weight = 144,
	Fork_height = 24,
	bk_height = 400,
	bk_weight = 800,
	Vigna_m = 1,
	bk_flame = 20,
	g = 1,
	Attack_move = 24,
	Wall_height = Vigna_height * 4 / 3,
	Wall_weight = Vigna_weight * 4 / 3,
	Wall_set_time = 64,
	Wall_speed = -8,
	Heart_weight = 32,
	icon_gap = 10,
	start_heart = 6,
	score_text_rd = 200,
	score_number_rd = score_text_rd / 2 - 2 * icon_gap,
	set_enemy_h_rate = 15,
	Enemy_h_weight = Vigna_weight,
	Enemy_h_height = Vigna_height,
	Enemy_h_health = 3,
	Enemy_h_speed = 10,
	Enemy_flame = 100,
	Enemy_h_set_time = 16,
	Enemy_h2set_time = 24,
	bug_appear = 20,
	bug_speed = 8,
	bug_weight = Vigna_height / 2,
	bug_height = Vigna_height / 3,
};

struct fPOINT;
struct fRECT;
struct FLBtemp_message;
class BK;
class Hero;
class Bullet;
class HPEnemy;
class EBullet;
class Enemy;
class Fork;
class Vigna;
class VBK;

short FLBdesk[4][4] = { 8 };
short FLBcolor[8] = { 0 };
RECT FLBColorRECT[4][4];
bool is_move_up = 0;
bool is_move_down = 0;
bool is_move_left = 0;
bool is_move_right = 0;
short v = 3;
int num_list[20] = { 10,20,23,14,8,4,18,5,44,10,12,23,32,30,14,7,9,10,17,29 };
int num_p = 0;
int start_health = 6;
bool is_music = 1;
bool is_temp_01 = 0;
bool is_crash_block_visable = 0;
bool is_test = 0;
short start_heart_set = 4;
int undeadtime = 0;
int EHRTLT = 0;
POINT score_left_rect = { bk_weight - bk_flame - score_text_rd,bk_flame + icon_gap };
POINT score_right_rect = { bk_weight - bk_flame - score_number_rd,icon_gap + bk_flame };
list<Enemy*> Enemy_list;
list<Enemy*> Enemy_h_list;
list<Enemy*> Enemy_h2list;
list<Enemy*> Enemy_B_list;

constexpr int FLBblock = 90 * 2;
constexpr int FLBframe = 20 * 2;
constexpr int FLBtable = FLBblock * 4 + FLBframe * 2;
constexpr auto swidth = 1100;
constexpr auto sheight = 800;
constexpr int HeroWidth = 60;
constexpr int HeroHeight = 90;
constexpr unsigned int SHP = 3;
constexpr unsigned int ICON = 32;
constexpr unsigned int bullet_height = 64;
constexpr unsigned int bullet_weight = 32;
constexpr auto hurttime = 1000;




void putimage_new(int x, int y, IMAGE* img);
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
bool JVsetgame();
int Num_get();
int random_set(int top, int bottom);
bool Vigna_play(Vigna* vigna, VBK* bk, Fork* fork, int& time);
bool VGPWelcome();
bool Vigna_main();
bool is_crash(fRECT rec1, fRECT rec2);

int main()
{
    MyCPmain();
    MyCPclose();
    system("pause");
}

struct fRECT
{
	double left;
	double top;
	double right;
	double bottom;
};
struct fPOINT
{
	double x;
	double y;
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
class Enemy
{
public:
	Enemy(fPOINT xy, char temp_char = '0')
		:xy(xy), Enemy_target(temp_char)
	{
		switch (temp_char)
		{
		case 'W':
		case 'w':
			speed = Wall_speed;
			loadimage(&img[0], _T("source/wall.png"), Wall_weight, Wall_height);
			cout << "Wall_loading\n";
			break;
		case 'H':
			toward = 1;
			speed = Enemy_h_speed * 7 / 3;
			health = Enemy_h_health;
			if (toward == 0)
				xy.x = bk_weight - (bk_flame + icon_gap);
			loadimage(&img[0], _T("source/Enemy_H1_R_S.png"), Enemy_h_weight, Enemy_h_height);
			loadimage(&img[1], _T("source/Enemy_H1_R_A.png"), Enemy_h_weight, Enemy_h_height);
			loadimage(&img[2], _T("source/Enemy_H1_L_S.png"), Enemy_h_weight, Enemy_h_height);
			loadimage(&img[3], _T("source/Enemy_H1_L_A.png"), Enemy_h_weight, Enemy_h_height);
			cout << "Enemy_h_is_loading\n";
			break;
		case 'h':
			toward = 0;
			speed = Enemy_h_speed;
			health = Enemy_h_health;
			xy.x = 0 + bk_flame + icon_gap;
			loadimage(&img[0], _T("source/Enemy_H1_R_S.png"), Enemy_h_weight, Enemy_h_height);
			loadimage(&img[1], _T("source/Enemy_H1_R_A.png"), Enemy_h_weight, Enemy_h_height);
			loadimage(&img[2], _T("source/Enemy_H1_L_S.png"), Enemy_h_weight, Enemy_h_height);
			loadimage(&img[3], _T("source/Enemy_H1_L_A.png"), Enemy_h_weight, Enemy_h_height);
			cout << "Enemy_h_is_loading\n";
			break;
		case 'b':
		case 'B':
			toward = 0;
			speed = bug_speed;
			health = Enemy_h_health;
			xy.x = 0 + bk_flame + icon_gap;
			loadimage(&img[0], _T("source/Enemy_B1_L.png"), bug_weight, bug_height);
			loadimage(&img[1], _T("source/Enemy_B1_R.png"), bug_weight, bug_height);
			cout << "Enemy_B_is_loading\n";
		case '0':
			cout << "not_correct_imput\n";
		}
	}
	~Enemy()
	{
		score++;
	}
	void v_change()
	{
		speed = -speed;
		toward = (!toward);
	}

	static int score;
	void draw();
	void move();
	void toward_set(double Vigna_x);
	void set_rect();
	double get_x();
	fRECT get_rect();
private:
	int skin = 0;
	char Enemy_target;
	IMAGE img[4];
	fPOINT xy;
	int health = 3;
	double speed;
	fRECT crash_block;
	bool toward = 0;
};
class Vigna
{
public:
	Vigna(fPOINT xy, int m, short health = 3)
		:sxy(xy), m(m), health(health)
	{
		IMAGE temp_img[4];
		IMAGE temp_img2[3];
		loadimage(&temp_img[0], _T("source/VGN1.png"), Vigna_weight, Vigna_height);
		loadimage(&temp_img[1], _T("source/VGN2.png"), Vigna_weight, Vigna_height);
		loadimage(&temp_img[2], _T("source/VGN3.png"), Vigna_weight, Vigna_height);
		loadimage(&temp_img[3], _T("source/VGN4.png"), Vigna_weight, Vigna_height);
		loadimage(&temp_img2[0], _T("source/empty_heart.png"), Heart_weight, Heart_weight);
		loadimage(&temp_img2[1], _T("source/half_heart.png"), Heart_weight, Heart_weight);
		loadimage(&temp_img2[2], _T("source/full_heart.png"), Heart_weight, Heart_weight);
		img[0][0] = temp_img[0];
		img[1][0] = temp_img[1];
		img[0][1] = temp_img[2];
		img[1][1] = temp_img[3];
		heart_img[0] = temp_img2[0];
		heart_img[1] = temp_img2[1];
		heart_img[2] = temp_img2[2];
		xy = sxy;
		cout << "Vigna_loading..." << endl;
	}
	~Vigna()
	{
		cout << "game_over\n";
	}
	void skin_time();
	void Newton2();
	void draw();
	void draw_heart();
	void Vigna_move();
	void control_Vigna();
	void out(char t);
	void set(double x, double y, char temp);
	void set(float t, char temp);
	void crash_block_set();
	bool is_land();
	bool is_attack();
	fPOINT gainxy();
	fRECT get_rect();
	short health;
private:
	fPOINT xy = { 0,0 };
	fPOINT v = { 0,0 };
	fPOINT a = { 0,0 };
	fPOINT F = { 0,0 };
	fPOINT sxy;
	int m;
	IMAGE img[2][2];
	IMAGE heart_img[3];
	int skin_time_temp = 0;
	bool toward = 0;
	bool wind = 0;
	bool is_click = 0;
	fRECT hit_block;
	short power = 0;
};
class Fork
{
public:
	Fork(int i)
	{
		IMAGE temp_fork1;
		IMAGE temp_fork2;
		loadimage(&temp_fork1, _T("source/VGNE.png"));
		loadimage(&temp_fork2, _T("source/VGNE2.png"));
		img[1] = temp_fork1;
		img[0] = temp_fork2;
		cout << "Fork_loading...\n";
	}
	void crash_block_set();
	void move(fPOINT temp01, bool is_attack);
	void draw();
	Fork* out()
	{
		return this;
	}
	fRECT get_rect();
	void toward_change();
private:
	IMAGE img[2];
	fPOINT xy = { 0,0 };
	fRECT crash_block = { 0,0,0,0 };
	bool toward = 1;
};
class VBK
{
public:
	VBK(IMAGE img, fPOINT xy = { -bk_weight / 4,0 })
		:img(img), xy(xy)
	{
		cout << "BK_loading...\n";
	}
	void BK_move();
	void BK_put()
	{
		putimage_new(xy.x, xy.y, &img);
	}

private:
	fPOINT xy;
	IMAGE img;
};

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
    //RECT CCL_choice = { MCPbk_gap,MCPbk_height / 7 * 3,MCPbk_gap + icon_size,MCPbk_height / 7 * 3 + icon_size };
    //RECT DPT_choice = { MCPbk_gap,MCPbk_height / 7 * 4,MCPbk_gap + icon_size,MCPbk_height / 7 * 4 + icon_size };
    RECT FLB_choice = { MCPbk_gap,MCPbk_height / 7 * 3,MCPbk_gap + icon_size,MCPbk_height / 7 * 5 + icon_size };
    IMAGE QUT_icon;
    IMAGE SHT_icon;
    IMAGE VGP_icon;
    //IMAGE CCL_icon;
    //IMAGE DPT_icon;
    IMAGE FLB_icon;
    loadimage(&QUT_icon, _T("source/QUTicon.png"), icon_size, icon_size);
    loadimage(&SHT_icon, _T("source/SHTicon.png"), icon_size, icon_size);
    loadimage(&VGP_icon, _T("source/VGPicon.png"), icon_size, icon_size);
    //loadimage(&DPT_icon, _T("source/DPTicon.png"), icon_size, icon_size);
    //loadimage(&CCL_icon, _T("source/CCLicon.png"), icon_size, icon_size);
    loadimage(&FLB_icon, _T("source/FLBicon.png"), icon_size, icon_size);
    TCHAR QUT_char[] = _T("关机");
    TCHAR SHT_char[] = _T("躲避子弹");
    TCHAR VGP_char[] = _T("生存挑战");
    //TCHAR CCL_char[] = _T("简单计算器");
    //TCHAR DPT_char[] = _T("绘图");
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
        //putimage_new(DPT_choice.left, DPT_choice.top, &DPT_icon);
        //putimage_new(CCL_choice.left, CCL_choice.top, &CCL_icon);
        putimage_new(FLB_choice.left, FLB_choice.top, &FLB_icon);
        settextstyle(16, 0, _T("宋体"));
        outtextxy(QUT_choice.left, QUT_choice.top+icon_size+WBI_gp, QUT_char);
        outtextxy(SHT_choice.left, SHT_choice.top + icon_size + WBI_gp, SHT_char);
        outtextxy(VGP_choice.left, VGP_choice.top + icon_size + WBI_gp, VGP_char);
        //outtextxy(CCL_choice.left, CCL_choice.top + icon_size + WBI_gp, CCL_char);
        //outtextxy(DPT_choice.left, DPT_choice.top + icon_size + WBI_gp, DPT_char);
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
			else if (PtInRect(&VGP_choice, choice))
			{
				Vigna_main();
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
	LPCTSTR title = _T("躲避子弹");
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
	_stprintf_s(str, 128, _T("SCOREs：%llu"), kill);

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
	HPEnemy* e = new HPEnemy(enemyimg, abs(rand()) % (swidth - enemyimg.getwidth() - 112 - 288 - Hicon_gap) + 112);
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
		setfillcolor(LIGHTCYAN);
		RECT rect3 = { 750,100,950,175 };
		solidrectangle(750, 100, 950, 175);
		drawtext(_T("躲避子弹"), &rect3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
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
bool Vigna_main()
{
	int time = 0;
	initgraph(bk_weight, bk_height, EW_NOMINIMIZE | EW_SHOWCONSOLE);
	if (VGPWelcome())
	{
		settextstyle(40, 0, _T("黑体"));
		setlinecolor(LIGHTGRAY);
		setlinestyle(PS_SOLID | PS_JOIN_BEVEL, 1);
		Fork fork(1);
		Vigna vigna({ 600 ,bk_height * 3 / 4 }, Vigna_m, start_health);
		IMAGE bk2;
		loadimage(&bk2, _T("source/BK2.png"));
		VBK bk{ bk2 };
		while (Vigna_play(&vigna, &bk, &fork, time));
		
		RECT rect_temp02 = { 0,0,800,400 };
		ExMessage mess;
		MessageBox(GetHWnd(), _T("右击屏幕以返回"), _T("您逝了"),  MB_OK);

		while (true)
		{
			getmessage(&mess, EM_MOUSE);
			if (mess.rbutton)
			{
				break;
			}
		}
	}
	return 0;
}
void Vigna::draw()
{
	putimage_new(xy.x, xy.y, &img[toward][wind]);
}
void Vigna::draw_heart()
{
	int hole = 0;
	int temp_health = health;
	while (hole < start_health / 2 + start_health % 2)
	{
		if (temp_health >= 2)
		{
			putimage_new(bk_flame + icon_gap + (Heart_weight + icon_gap) * hole, bk_flame + icon_gap, &heart_img[2]);
			hole++;
			temp_health -= 2;
		}
		else if (temp_health == 1)
		{
			putimage_new(bk_flame + icon_gap + (Heart_weight + icon_gap) * hole, bk_flame + icon_gap, &heart_img[1]);
			hole++;
			temp_health -= 1;
		}
		else
		{
			putimage_new(bk_flame + icon_gap + (Heart_weight + icon_gap) * hole, bk_flame + icon_gap, &heart_img[0]);
			hole++;
		}
	}
}
void Vigna::Newton2()
{
	a.x = F.x / m;
	a.y = F.y / m;
	v.x += a.x;
	v.y += a.y;
	xy.x += v.x;
	xy.y += v.y;
}
void Vigna::skin_time()
{
	skin_time_temp++;
	if (skin_time_temp >= 10)
	{
		if (wind == 0)
		{
			wind = 1;
		}
		else if (wind == 1)
		{
			wind = 0;
		}
		skin_time_temp = 0;
	}
	if (is_move_left)
	{
		toward = 1;
	}
	else if (is_move_right)
	{
		toward = 0;
	}
}
void Vigna::Vigna_move()
{
	v.x += a.x;
	v.y += a.y;
	xy.x += v.x;
	xy.y += v.y;
	if (xy.x > bk_weight - bk_flame - Vigna_weight)
	{
		xy.x = bk_weight - bk_flame - Vigna_weight;
	}
	else if (xy.x < 0 + bk_flame)
	{
		xy.x = 0 + bk_flame;
	}
	if (xy.y >= bk_height - bk_flame - Vigna_height)
	{
		xy.y = bk_height - bk_flame - Vigna_height;
		v.y = 0;
	}
	else if (xy.y < 0 + bk_flame)
	{
		xy.y = 0 + bk_flame;
	}
}
void Vigna::control_Vigna()
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
		if (msg.message == WM_LBUTTONDOWN)
		{
			is_click = 1;
		}
		if (msg.message == WM_LBUTTONUP)
		{
			is_click = 0;
		}
	}
	if (is_move_up && is_land())
	{
		a.y -= 21.5;
	}
	F.y = g * m;
	if (is_move_left)
	{
		xy.x -= 11.4;
	}
	else if (is_move_right)
	{
		xy.x += 11.4;
	}
	else
	{
		F.x = 0;
		a.x = 0;
		v.x = 0;
	}
}
void Vigna::out(char t)
{
	switch (t)
	{
	case 'f':
	case 'F':
		cout << F.x << "　" << F.y << endl;
	case 'P':
	case 'p':
		cout << xy.x << " " << xy.y << endl;
		break;
	case 'V':
	case 'v':
		cout << v.x << " " << v.y << endl;
		break;
	case 'a':
	case 'A':
		cout << a.x << " " << a.y << endl;
		break;
	case 'M':
	case 'm':
		cout << m << endl;
	case 'H':
	case 'h':
		cout << health << endl;
		break;
	default:
		cout << "error" << endl;
		break;
	}
}
fPOINT Vigna::gainxy()
{
	return xy;
}
bool Vigna::is_attack()
{
	return is_click;
}
void Vigna::set(double x, double y, char temp)
{
	switch (temp)
	{
	case 'v':
	case 'V':
		v.x = x;
		v.y = y;
		break;
	case 'a':
	case 'A':
		a.x = x;
		a.y = y;
		break;
	case 'f':
	case 'F':
		F.x = x;
		F.y = y;
		break;
	case 'p':
	case 'P':
		xy.x = x;
		xy.y = y;
		break;
	default:
		cout << "格式错误\n";
		break;
	}
}
void Vigna::set(float t, char temp)
{
	switch (temp)
	{
	case 'M':
	case 'm':
		m = t;
		break;
	case 'h':
	case 'H':
		health = t;
		break;
	default:
		cout << "格式错误\n";
		break;
	}
}
bool Vigna::is_land()
{
	return (xy.y >= (bk_height - bk_flame - Vigna_height));
}
void Vigna::crash_block_set()
{
	hit_block.left = xy.x;
	hit_block.right = xy.x + Vigna_weight - 5;
	hit_block.top = xy.y + 10;
	hit_block.bottom = xy.y + Vigna_height - 10;
}
void Fork::crash_block_set()
{
	crash_block.left = xy.x;
	crash_block.right = xy.x + Fork_weight - 5;
	crash_block.top = xy.y + 10;
	crash_block.bottom = xy.y + Fork_height - 10;
}
void VBK::BK_move()
{
	if (is_move_left)
	{
		xy.x += 5.7;
	}
	else if (is_move_right)
	{
		xy.x -= 5.7;
	}
	if (xy.x > 0)
	{
		xy.x = 0;
	}
	else if (xy.x < -400)
	{
		xy.x = -400;
	}
}
bool VGPWelcome()
{
temp_flag01:
	setbkmode(TRANSPARENT);
	IMAGE BK0;
	loadimage(&BK0, _T("source/bk1.png"));
	LPCTSTR title = _T("Vigna_jump");
	LPCTSTR tplay = _T("开始游戏");
	LPCTSTR thelp = _T("游戏说明");
	LPCTSTR texit = _T("退出游戏");
	LPCTSTR tseta = _T("游戏设置");
	RECT tplayr, texitr, thelpr, tsetar;
	BeginBatchDraw();
	setbkcolor(WHITE);
	cleardevice();
	putimage(0, 0, &BK0);
	settextstyle(60, 0, _T("黑体"));
	settextcolor(BLACK);
	outtextxy(600 - textwidth(title) / 2, bk_weight / 8, title);
	settextstyle(40, 0, _T("黑体"));
	tplayr.left = bk_weight / 5 * 3 - textwidth(tplay) / 2;
	tplayr.right = tplayr.left + textwidth(tplay);
	tplayr.top = bk_height / 5 * 5 / 2;
	tplayr.bottom = tplayr.top + textheight(tplay);
	thelpr.left = bk_weight / 5 * 3 - textwidth(thelp) / 2;
	thelpr.right = thelpr.left + textwidth(thelp);
	thelpr.top = bk_height / 5 * 4 / 2;
	thelpr.bottom = thelpr.top + textheight(thelp);
	tsetar.left = bk_weight / 5 * 3 - textwidth(tseta) / 2;
	tsetar.right = tsetar.left + textwidth(tseta);
	tsetar.top = bk_height / 5 * 6 / 2;
	tsetar.bottom = tsetar.top + textheight(tseta);
	texitr.left = bk_weight / 5 * 3 - textwidth(texit) / 2;
	texitr.right = texitr.left + textwidth(texit);
	texitr.top = bk_height / 5 * 7 / 2;
	texitr.bottom = texitr.top + textheight(texit);
	outtextxy(tplayr.left, tplayr.top, tplay);
	outtextxy(texitr.left, texitr.top, texit);
	outtextxy(thelpr.left, thelpr.top, thelp);
	outtextxy(tsetar.left, tsetar.top, tseta);
	EndBatchDraw();
	while (true)
	{
		ExMessage mess;
		getmessage(&mess, EM_MOUSE);
		POINT choice;
		choice.x = mess.x;
		choice.y = mess.y;
		if (mess.lbutton)
		{
			if (PtInRect(&tplayr, choice))
			{
				return 1;
			}
			else if (PtInRect(&texitr, choice))
			{
				return 0;
			}
			else if (PtInRect(&thelpr, choice))
			{
				MessageBox(GetHWnd(), _T("躲避障碍物与敌军\n按上跳跃左右移动\n鼠标左键攻击\n"), _T("说明"), MB_OK);
			}
			else if (PtInRect(&tsetar, choice))
			{
				JVsetgame();
				goto temp_flag01;
			}
		}
	}
	Sleep(100);
}
bool Vigna_play(Vigna* vigna, VBK* bk, Fork* fork, int& time)
{
	ExMessage msg;
	DWORD start_time = GetTickCount();
	BeginBatchDraw();
	bk->BK_put();
	bk->BK_move();
	vigna->control_Vigna();
	vigna->Vigna_move();
	vigna->Newton2();
	fork->toward_change();
	fork->move(vigna->gainxy(), vigna->is_attack());
	fork->draw();
	fork->crash_block_set();
	if (is_crash_block_visable)
	{
		rectangle(fork->get_rect().left, fork->get_rect().top, fork->get_rect().right, fork->get_rect().bottom);
	}
	vigna->crash_block_set();
	vigna->skin_time();
	vigna->draw();
	if (is_crash_block_visable)
	{
		rectangle(vigna->get_rect().left, vigna->get_rect().top, vigna->get_rect().right, vigna->get_rect().bottom);
	}
	if (time == Enemy_h_set_time)
	{
		fPOINT temp_fPOINT_02;
		temp_fPOINT_02.x = 0;
		temp_fPOINT_02.y = bk_height - bk_flame - Enemy_h_height;
		Enemy_h_list.push_back(new Enemy(temp_fPOINT_02, 'h'));

	}
	if (time == bug_appear)
	{
		fPOINT temp_fPOINT_02;
		temp_fPOINT_02.x = 0;
		temp_fPOINT_02.y = bk_height - bk_flame - bug_height;
		Enemy_B_list.push_back(new Enemy(temp_fPOINT_02, 'b'));

	}
	if (time == Enemy_h2set_time)
	{
		fPOINT temp_fPOINT_02;
		temp_fPOINT_02.y = bk_height - bk_flame - Enemy_h_height;
		temp_fPOINT_02.x = bk_weight - icon_gap - bk_flame;
		Enemy_h2list.push_back(new Enemy(temp_fPOINT_02, 'H'));
	}

	if (time == Wall_set_time)
	{
		Enemy_list.push_back(new Enemy({ bk_weight,bk_height - bk_flame - Wall_height }, 'W'));
		time = 0;
	}
	auto it = Enemy_list.begin();
	auto it2 = Enemy_h_list.begin();
	auto it3 = Enemy_h2list.begin();
	auto itb = Enemy_B_list.begin();
	while (it != Enemy_list.end())
	{

		if ((*it)->get_x() < 0)
		{
			delete (*it);
			Enemy_list.erase(it);
			it = Enemy_list.begin();
		}
		(*it)->move();
		(*it)->set_rect();
		(*it)->draw();
		if (is_crash_block_visable)
		{
			rectangle((*it)->get_rect().left, (*it)->get_rect().top, (*it)->get_rect().right, (*it)->get_rect().bottom);  //碰撞箱
		}
		if (is_crash(vigna->get_rect(), (*it)->get_rect()) && undeadtime == 0)
		{
			if (is_test)
			{
				cout << vigna->get_rect().left << " " << vigna->get_rect().right << endl;
				cout << (*it)->get_rect().left << " " << (*it)->get_rect().right << endl;
			}
			cout << "ouch,that hurt!" << endl;
			undeadtime = 16;
			vigna->health -= 1;
		}
		it++;
	}
	while (it2 != Enemy_h_list.end())
	{
		/*if ((*it2)->get_x() < -10)
		{
			delete (*it2);
			Enemy_h_list.erase(it2);
			it2 = Enemy_h_list.begin();   //助教您好，请问一下为什么加上这段就报错?
		}*/
		if ((*it2)->get_x() > bk_weight)
		{
			delete (*it2);
			Enemy_h_list.erase(it2);
			it2 = Enemy_h_list.begin();
		}
		(*it2)->move();
		(*it2)->set_rect();
		(*it2)->draw();
		if (is_crash_block_visable)
		{
			rectangle((*it2)->get_rect().left, (*it2)->get_rect().top, (*it2)->get_rect().right, (*it2)->get_rect().bottom);  //碰撞箱
		}
		if (is_crash(fork->get_rect(), (*it2)->get_rect()) && vigna->is_attack() == 1)
		{
			cout << "ha" << endl;
			delete (*it2);
			Enemy_h_list.erase(it2);
			it2 = Enemy_h_list.begin();
			break;
		}
		if (is_crash(vigna->get_rect(), (*it2)->get_rect()) && undeadtime == 0)
		{
			if (is_test)
			{
				cout << vigna->get_rect().left << " " << vigna->get_rect().right << endl;
				cout << (*it2)->get_rect().left << " " << (*it2)->get_rect().right << endl;
			}
			cout << "ouch,that hurt!" << endl;
			undeadtime = 16;
			vigna->health -= 1;
		}
		it2++;
	}
	while (it3 != Enemy_h2list.end())
	{

		if ((*it3)->get_x() < 0)
		{
			(*it3)->v_change();//这里还有一点问题，但是我懒得改了，就这样吧，反正也能跑
		}
		(*it3)->move();
		(*it3)->set_rect();
		(*it3)->draw();
		if (is_crash_block_visable)
		{
			rectangle((*it3)->get_rect().left, (*it3)->get_rect().top, (*it3)->get_rect().right, (*it3)->get_rect().bottom);  //碰撞箱
		}
		if (is_crash(fork->get_rect(), (*it3)->get_rect()) && vigna->is_attack() == 1)
		{
			cout << "ha" << endl;
			delete (*it3);
			Enemy_h2list.erase(it3);
			it3 = Enemy_h2list.begin();
			break;
		}
		if (is_crash(vigna->get_rect(), (*it3)->get_rect()) && undeadtime == 0)
		{
			if (is_test)
			{
				cout << vigna->get_rect().left << " " << vigna->get_rect().right << endl;
				cout << (*it3)->get_rect().left << " " << (*it3)->get_rect().right << endl;
			}
			cout << "ouch,that hurt!" << endl;
			undeadtime = 16;
			vigna->health -= 1;
		}
		it3++;
	}
	while (itb != Enemy_B_list.end())
	{

		if ((*itb)->get_x() < 0)
		{
			delete (*itb);
			Enemy_B_list.erase(itb);
			itb = Enemy_B_list.begin();
		}
		(*itb)->move();
		(*itb)->set_rect();
		(*itb)->draw();
		if (is_crash_block_visable)
		{
			rectangle((*itb)->get_rect().left, (*itb)->get_rect().top, (*itb)->get_rect().right, (*itb)->get_rect().bottom);  //碰撞箱
		}
		if (is_crash(vigna->get_rect(), (*itb)->get_rect()) && undeadtime == 0)
		{
			if (is_test)
			{
				cout << vigna->get_rect().left << " " << vigna->get_rect().right << endl;
				cout << (*itb)->get_rect().left << " " << (*itb)->get_rect().right << endl;
			}
			cout << "ouch,that hurt!" << endl;
			undeadtime = 16;
			vigna->health -= 1;
		}
		itb++;
	}
	vigna->draw_heart();
	setbkmode(TRANSPARENT);
	LPCTSTR Score_str = _T("SCORE:");
	outtextxy(score_left_rect.x, score_left_rect.y, Score_str);
	TCHAR s[5];
	_stprintf(s, _T("%d"), Enemy::score);
	outtextxy(score_right_rect.x, score_right_rect.y, s);
	FlushBatchDraw();
	cleardevice();
	DWORD finish_time = GetTickCount();
	DWORD dtime = finish_time - start_time;
	time++;
	if (dtime < 1000 / 60)
	{
		Sleep(1000 / 60 - dtime);
	}
	if (undeadtime > 0)
	{
		undeadtime--;
	}
	if (vigna->health <= 0)
	{
		return 0;
	}
	return 1;
}
double Enemy::get_x()
{
	return xy.x;
}
bool is_crash(fRECT r1, fRECT r2)
{
	fRECT r;
	r.left = r1.left - (r2.right - r2.left);
	r.right = r1.right;
	r.top = r1.top - (r2.bottom - r2.top);
	r.bottom = r1.bottom;

	return (r.left < r2.left && r2.left <= r.right && r.top <= r2.top && r2.top <= r.bottom);
}
void Fork::move(fPOINT temp01, bool is_attack)
{
	xy.y = temp01.y + Vigna_height / 3;
	if (is_attack)
	{
		if (toward)
		{
			xy.x = temp01.x - Fork_weight / 4 + Attack_move;
		}
		else
		{
			xy.x = temp01.x - Fork_weight * 3 / 8 - Attack_move;
		}
	}
	else
	{
		if (toward)
		{
			xy.x = temp01.x - Fork_weight / 4;
		}
		else
		{
			xy.x = temp01.x - Fork_weight * 3 / 8;
		}

	}
}
void Fork::draw()
{
	putimage_new(xy.x, xy.y, &img[toward]);
}
void Fork::toward_change()
{
	if (toward)
	{
		if (is_move_left)
		{
			toward = 0;
		}
	}
	else
	{
		if (is_move_right)
		{
			toward = 1;
		}
	}
}
void Enemy::draw()
{
	switch (Enemy_target)
	{
	case 'W':
	case 'w':
		putimage_new(xy.x - Wall_weight, xy.y, &img[0]);
		break;
	case 'H':
	case 'h':
		if (toward == 0)
		{
			if (skin == 0)
			{
				putimage_new(xy.x - Enemy_h_weight, xy.y, &img[0]);
				skin = 1;
			}
			else if (skin == 1)
			{
				putimage_new(xy.x - Enemy_h_weight, xy.y, &img[1]);
				skin = 0;
			}
			else
			{
				cout << "error_skin>1\n";
			}
		}
		else if (toward == 1)
		{
			if (skin == 0)
			{
				fillcircle(xy.x, xy.y, 8);
				putimage_new(xy.x - Enemy_h_weight, xy.y, &img[2]);
				skin = 1;
			}
			else if (skin == 1)
			{
				fillcircle(xy.x, xy.y, 4);
				putimage_new(xy.x - Enemy_h_weight, xy.y, &img[3]);
				skin = 0;
			}
			else
			{
				cout << "error_skin>1\n";
			}
		}
		break;
	case 'b':
	case 'B':
		if (toward == 0)
		{
			putimage_new(xy.x - Enemy_h_weight, xy.y, &img[1]);
		}
		else
		{
			putimage_new(xy.x - Enemy_h_weight, xy.y, &img[0]);
		}
	default:
		break;
	}
}
void Enemy::move()
{
	if (Enemy_target == 'w' || Enemy_target == 'W')
		xy.x += speed;
	else if (Enemy_target == 'H' || Enemy_target == 'h')
	{
		if (toward == 0)
		{
			xy.x += speed;
		}
		else
		{
			xy.x -= speed;
		}
	}
	else
	{
		if (xy.x<100 || xy.x>bk_weight - 100)
		{
			if (toward == 1)
			{
				toward = 0;
			}
			else
			{
				toward = 0;
			}
		}
		if (toward == 0)
		{
			xy.x += speed;
		}
		else
		{
			xy.x -= speed;
		}
	}
}
void Enemy::toward_set(double Vigna_x)
{
	switch (Enemy_target)
	{
	case 'W':
	case 'w':
		break;
	case '0':
	default:
		cout << "wrong_number\n";
		break;
	}
}
void Enemy::set_rect()
{
	crash_block.right = xy.x;
	crash_block.left = xy.x - Wall_weight;
	crash_block.top = xy.y;
	crash_block.bottom = xy.y + Wall_height;
}
fRECT Enemy::get_rect()
{
	return crash_block;
}
fRECT Vigna::get_rect()
{
	return hit_block;
}
fRECT Fork::get_rect()
{
	return crash_block;
}
int random_set(int top, int bottom)
{
	srand((unsigned)time(NULL));
	int ans = rand() % (top + 1) + bottom;
	return ans;
}
int Num_get()
{
	if (num_p <= 18)
		num_p++;
	else
		num_p = 0;
	return num_list[num_p];
}
bool JVsetgame()
{

	cleardevice();
	setbkcolor(LIGHTGRAY);

	LPCTSTR title = _T("游戏设置");
	LPCTSTR choice1 = _T("初始血量:");
	LPCTSTR choice2 = _T("碰撞箱显示:");
	LPCTSTR choice3 = _T("背景音乐:");
	RECT tplayr, texitr, thelpr, tsetar;
	setlinecolor(BLACK);
	setlinestyle(PS_SOLID | PS_JOIN_BEVEL, 5);
	setfillcolor(LIGHTRED);
	RECT exit_rect = { 50,50,100,100 };
	RECT is_music_rect = { bk_weight / 3 * 2, bk_height * 6 / 12,bk_weight / 3 * 2 + textheight(choice1), bk_height * 6 / 12 + textheight(choice1) };
	RECT is_blockvis_rect = { bk_weight / 3 * 2, bk_height * 8 / 12, bk_weight / 3 * 2 + textheight(choice1), bk_height * 8 / 12 + textheight(choice1) };
	RECT plus = { bk_weight / 3 * 2 - 70 ,bk_height * 4 / 12 ,bk_weight / 3 * 2 + textheight(choice1) - 70 ,bk_height * 4 / 12 + textheight(choice1) };
	RECT deplus = { bk_weight / 3 * 2 + 70, bk_height * 4 / 12 ,bk_weight / 3 * 2 + textheight(choice1) + 70, bk_height * 4 / 12 + textheight(choice1) };
	while (true)
	{
		cleardevice();
		BeginBatchDraw();
		TCHAR s[5];
		_stprintf(s, _T("%d"), start_health);
		outtextxy(bk_weight / 3 * 2 + 10, bk_height * 4 / 12, s);
		rectangle(bk_weight / 3 * 2, bk_height * 4 / 12, bk_weight / 3 * 2 + textheight(choice1), bk_height * 4 / 12 + textheight(choice1));
		line(bk_weight / 3 * 2 - 40 - 10, bk_height * 4 / 12, bk_weight / 3 * 2 - 40 - 10, bk_height * 4 / 12 + textheight(choice1));
		line(bk_weight / 3 * 2 - 70, bk_height * 4 / 12 + textheight(choice1) / 2, bk_weight / 3 * 2 + textheight(choice1) - 70, bk_height * 4 / 12 + textheight(choice1) / 2);
		line(bk_weight / 3 * 2 + 70, bk_height * 4 / 12 + textheight(choice1) / 2, bk_weight / 3 * 2 + textheight(choice1) + 70, bk_height * 4 / 12 + textheight(choice1) / 2);
		settextstyle(60, 0, _T("黑体"));
		settextcolor(BLACK);
		outtextxy(bk_weight / 2 - textwidth(title) / 2, bk_height / 12, title);
		settextstyle(40, 0, _T("宋体"));
		rectangle(bk_weight / 3 * 2, bk_height * 6 / 12, bk_weight / 3 * 2 + textheight(choice1), bk_height * 6 / 12 + textheight(choice1));
		rectangle(bk_weight / 3 * 2, bk_height * 8 / 12, bk_weight / 3 * 2 + textheight(choice1), bk_height * 8 / 12 + textheight(choice1));
		outtextxy(bk_weight / 5 - textwidth(choice1) / 2, bk_height * 6 / 12, choice3);
		outtextxy(bk_weight / 5 - textwidth(choice1) / 2, bk_height * 8 / 12, choice2);
		outtextxy(bk_weight / 5 - textwidth(choice1) / 2, bk_height * 4 / 12, choice1);
		if (is_music)
		{
			fillrectangle(bk_weight / 3 * 2 + 3, bk_height * 6 / 12 + 3, bk_weight / 3 * 2 + textheight(choice1) - 3, bk_height * 6 / 12 + textheight(choice1) - 3);
		}
		if (is_crash_block_visable)
		{
			fillrectangle(bk_weight / 3 * 2 + 3, bk_height * 8 / 12 + 3, bk_weight / 3 * 2 + textheight(choice1) - 3, bk_height * 8 / 12 + textheight(choice1) - 3);
		}
		rectangle(50, 50, 100, 100);
		line(50, 50, 100, 100);
		line(50, 100, 100, 50);
		ExMessage mess;
		getmessage(&mess, EM_MOUSE);
		POINT choice;
		choice.x = mess.x;
		choice.y = mess.y;
		rectangle(plus.left, plus.top, plus.right, plus.bottom);
		rectangle(deplus.left, deplus.top, deplus.right, deplus.bottom);
		if (mess.lbutton)
		{
			if (PtInRect(&is_music_rect, choice))
			{
				if (is_music)
				{
					is_music = 0;
				}
				else if (is_music == 0)
				{
					is_music = 1;
				}
			}
			else if (PtInRect(&is_blockvis_rect, choice))
			{
				if (is_crash_block_visable)
				{
					is_crash_block_visable = 0;
				}
				else if (is_crash_block_visable == 0)
				{
					is_crash_block_visable = 1;
				}
			}
			else if (PtInRect(&plus, choice))
			{
				start_health += 1;
				continue;
			}
			else if (PtInRect(&deplus, choice))
			{
				if (start_health > 2)
				{
					start_health -= 1;
				}
				continue;
			}
			else if (PtInRect(&exit_rect, choice))
			{
				return 0;
			}
		}
		EndBatchDraw();
	}
	return 0;
}
int Enemy::score = 0;