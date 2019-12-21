#include <GLUT/GLUT.h>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <vector>
#include <iostream>
#include <list>
#include <algorithm>
using namespace std;

int WinWidth = 1024, Winheight = 720; //窗口宽高
int X0,Y0,X1,Y1,Xvalue; //起始坐标与终止坐标
void init();
void Mymenu(int);
void DrawLine(int,int,int,int);
void BresenhamLine(int,int,int,int);
void LiangBarsky(int,int,int,int,int,int,int,int);

typedef double db;
const db EPS = 1e-7;
inline int sign(db a) {return a < -EPS ? -1 : a > EPS; } //返回-1表示a < 0, 1表示a > 0, 0表示a = 0

void swap(GLint& a,GLint &b) {GLint t = a; a = b; b = t;}
void setPixel(GLint x,GLint y){
    glBegin(GL_POINTS); //把每个顶点作为一个点来处理
    glVertex2i(x,y); //int坐标画点
    glEnd();
}
struct BASE{
    int x0,y0,x1,y1;
};
vector<BASE> Vline,VRectangle,Vline1;
//Vline —— 保存已经画完的直线, Vline1 —— 保存当前画的直线, VRectangle —— 保存窗口坐标

void ReDraw(){
//    printf("Line.size(): %d\n",(int)Vline.size());
    if(VRectangle.size()){
        int x0 = VRectangle[0].x0, y0 = VRectangle[0].y0, x1 = VRectangle[0].x1, y1 = VRectangle[0].y1;
        int xlow = min(x0,x1), ylow = min(y0,y1), xh = max(x0,x1), yh = max(y0,y1);
        DrawLine(xlow, ylow, xlow, yh);
        DrawLine(xlow, ylow, xh, ylow);
        DrawLine(xlow, yh, xh, yh);
        DrawLine(xh, ylow, xh, yh);
        for(int i = 0; i < Vline.size(); i++){
            LiangBarsky(Vline[i].x0,Vline[i].y0,Vline[i].x1,Vline[i].y1,xlow,xh,ylow,yh);
        }
    }
    if(Vline1.size()) DrawLine(Vline1[0].x0, Vline1[0].y0, Vline1[0].x1, Vline1[0].y1);
}

void display(){
    glClear (GL_COLOR_BUFFER_BIT);
    glRectf (-1.0, -1.0, 1.0, 1.0);
    ReDraw();
    glutSwapBuffers ();
}

void update(){
    if(Xvalue == 1){ //Line
        Vline1[0].x1 = X1; Vline1[0].y1 = Y1;
        if(Vline.size() == 0 || (Vline[Vline.size()-1].x0 != X0 && Vline[Vline.size()-1].y0 != Y0))
            Vline.push_back({X0,Y0,X1,Y1});
        else{
            Vline[Vline.size()-1].x1 = X1;
            Vline[Vline.size()-1].y1 = Y1;
        }
    }
    else if(Xvalue == 2){
        VRectangle[0].x1 = X1; VRectangle[0].y1 = Y1;
    }
    display();
}

void Dragmouse(int x,int y){ //鼠标拖拽
    X1 = x; Y1 = y; update();
}

void Mymouse(int button, int state, int x, int y){
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP){
        X1 = x; Y1 = y;
        update();
    }
    else if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        X0 = x; Y0 = y;
        if(Xvalue == 1){
            Vline1.clear();
            Vline1.push_back((BASE){X0,Y0,X0,Y0});
        }
        else if(Xvalue == 2){
            Vline1.clear();
            VRectangle.clear();
            VRectangle.push_back((BASE){X0,Y0,X0,Y0});
        }
    }
}

int main(int argc, char** argv){
    glutInit(&argc, argv); //初始化
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); //指定单缓存窗口与RGB颜色模式的窗口
    glutInitWindowPosition(500, 300); //设置初始窗口的位置，(0,0)为屏幕左上角位置
    glutInitWindowSize(WinWidth, Winheight); //设置窗口宽度与高度，单位为像素
    glutCreateWindow("图形学实验二"); //创建窗口
    init();
    
    glutDisplayFunc(display);//注册显示回调函数
    glutCreateMenu(Mymenu);//注册菜单回调函数
    glutAddMenuEntry("DrawLine",1);//添加菜单项
    glutAddMenuEntry("DrawRectangle",2);
    glutAddMenuEntry("ClearScreen",3);
    glutAddMenuEntry("Exit",4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);//把当前菜单注册到指定的鼠标键
    glutMainLoop();
}

void init(){
    glClearColor(0,0,0,1); //(red green blue alpha) alpha表示混合因子
    glClear(GL_COLOR_BUFFER_BIT); //将屏幕所有像素点还原为"底色"
    glPointSize(3.0f); //指定栅格化点的直径
    glMatrixMode(GL_PROJECTION); //声明接下来要进行的操作，GL_PROJECTION 投影， GL_MODELVIEW 模型视图， GL_TEXTURE 纹理
    glLoadIdentity(); //加载一个单位矩阵
    gluOrtho2D(0,WinWidth,Winheight,0); //定义裁剪面
    glColor3f(255,215,0); //设置画点颜色 R-G-B
    glutMouseFunc(Mymouse);
    glutMotionFunc(Dragmouse);
}

void DrawLine(int x1,int y1,int x2,int y2){ //画直线函数
    BresenhamLine(x1,y1,x2,y2);
}

void Mymenu(int value){
    Xvalue = value;
    if (value == 3){
        glClear(GL_COLOR_BUFFER_BIT);
        glutSwapBuffers();
        Vline1.clear();
        Vline.clear();
        VRectangle.clear();
    }
    else if (value == 4){
        exit(0);
    }
}

void BresenhamLine(int x1,int y1,int x2,int y2){
    int x = x1, y = y1, dx = abs(x2-x1), dy = abs(y2-y1), s1 = 1, s2 = 1, e, flag = 0;
    if(x1 >= x2) s1 = -1;
    if(y1 >= y2) s2 = -1;
    if(dy > dx) {swap(dx,dy); flag = 1;}
    e = -dx;
    int DX = 2*dx, DY = 2*dy;
    for(int i = 1; i <= dx; i++){
        setPixel(x, y);
        if(e >= 0){
            if(!flag) y += s2;
            else x += s1;
            e = e-DX;
        }
        if(!flag) x += s1;
        else y += s2;
        e = e+DY;
    }
}

void LiangBarsky(int x1, int y1, int x2, int y2, int xleft, int xright, int ybottom, int ytop){
    int p[5] = {0,x1-x2,x2-x1,y1-y2,y2-y1},
        q[5] = {0,x1-xleft,xright-x1,y1-ybottom,ytop-y1}, L = 1, R = 4;
    db u[5], umin = 1, umax = 0;
    if(p[1] == 0 && p[3] == 0) return;
    if(p[1] == 0){
        if(q[1] < 0 || q[2] < 0) return;
        else {L = 3; R = 4;}
    }
    else if(p[3] == 0){
        if(q[3] < 0 || q[4] < 0) return;
        else {L = 1; R = 2;}
    }
    for(int i = L; i <= R; i++){
        u[i] = (db)q[i]/(db)p[i];
        if(p[i] < 0) umax = max(umax,u[i]);
        else umin = min(umin,u[i]);
    }
    if(sign(umax-umin) == 1) return;
    db xans1 = (db)x1+umin*(db)(x2-x1), yans1 = (db)y1+umin*(db)(y2-y1), xans2 = (db)x1+umax*(db)(x2-x1), yans2 = (db)y1+umax*(db)(y2-y1);
    DrawLine((int)xans1, (int)yans1, (int)xans2, (int)yans2);
}
