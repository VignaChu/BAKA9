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
using namespace std;
#pragma comment(lib,"MSIMG32.LIB")
#pragma warning(disable:4996)；

void putimage_new(int x, int y, IMAGE* img);
bool MyCPmain();

enum
{
    MCPbk_weight = 128 * 8,
    MCPbk_height = 96 * 8,
    MCPbk_gap = 16 * 1,
    WBI_gp = 2 * 1,
    LoadingSleepTime = 128,
    icon_size = 16 * 4,
};

int main()
{
    MyCPmain();
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
    RECT SHT_choice = { MCPbk_gap,MCPbk_height / 7 * 1,MCPbk_gap + icon_size,MCPbk_height + icon_size };
    RECT VGP_choice = { MCPbk_gap,MCPbk_height / 7 * 2,MCPbk_gap + icon_size,MCPbk_height + icon_size };
    RECT CCL_choice = { MCPbk_gap,MCPbk_height / 7 * 3,MCPbk_gap + icon_size,MCPbk_height + icon_size };
    RECT DPT_choice = { MCPbk_gap,MCPbk_height / 7 * 4,MCPbk_gap + icon_size,MCPbk_height + icon_size };
    IMAGE QUT_icon;
    IMAGE SHT_icon;
    IMAGE VGP_icon;
    IMAGE CCL_icon;
    IMAGE DPT_icon;
    loadimage(&QUT_icon, _T("source/QUTicon.png"), icon_size, icon_size);
    loadimage(&SHT_icon, _T("source/SHTicon.png"), icon_size, icon_size);
    loadimage(&VGP_icon, _T("source/VGPicon.png"), icon_size, icon_size);
    loadimage(&DPT_icon, _T("source/DPTicon.png"), icon_size, icon_size);
    loadimage(&CCL_icon, _T("source/CCLicon.png"), icon_size, icon_size);
    TCHAR QUT_char[] = _T("关机");
    TCHAR SHT_char[] = _T("躲避子弹");
    TCHAR VGP_char[] = _T("生存挑战");
    TCHAR CCL_char[] = _T("简单计算器");
    TCHAR DPT_char[] = _T("绘图");
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
        settextstyle(16, 0, _T("宋体"));
        outtextxy(QUT_choice.left, QUT_choice.top+icon_size+WBI_gp, QUT_char);
        outtextxy(SHT_choice.left, SHT_choice.top + icon_size + WBI_gp, SHT_char);
        outtextxy(VGP_choice.left, VGP_choice.top + icon_size + WBI_gp, VGP_char);
        outtextxy(CCL_choice.left, CCL_choice.top + icon_size + WBI_gp, CCL_char);
        outtextxy(DPT_choice.left, DPT_choice.top + icon_size + WBI_gp, DPT_char);
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
                return 0;
            }
            //else if (PtInRect(&texitr, choice))
            {
                //exit(0);
            }
            //else if (PtInRect(&thelpr, choice))
            {
                //MessageBox(GetHWnd(), _T("躲避障碍物与敌军\n按上跳跃左右移动\n鼠标左键攻击\n"), _T("说明"), MB_OK);
            }
           //else if (PtInRect(&tsetar, choice))
            {
                
            }
        }
        FlushBatchDraw();
    }
    Sleep(100);
}
