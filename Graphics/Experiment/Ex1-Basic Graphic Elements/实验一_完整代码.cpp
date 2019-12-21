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
void DrawCircle(int,int,int,int);
void DrawEllipse(int,int,int,int);
void BresenhamLine(int,int,int,int);
void Bresenham_Circle(int,int,int);
void Bresenham_Ellipse(int,int,int,int);
void Xscan_Algorithm();

typedef long double db;
const db EPS = 1e-15;
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
vector<BASE> Vline,Vcircle,Vellipse;
vector<pair<int,int> > Point;
vector<vector<pair<int,int> > > Polygon;

void ReDraw(){
    for(int i = 0; i < Vline.size(); i++)
        DrawLine(Vline[i].x0,Vline[i].y0,Vline[i].x1,Vline[i].y1);
    for(int i = 0; i < Vcircle.size(); i++)
        DrawCircle(Vcircle[i].x0,Vcircle[i].y0,Vcircle[i].x1,Vcircle[i].y1);
    for(int i = 0; i < Vellipse.size(); i++)
        DrawEllipse(Vellipse[i].x0,Vellipse[i].y0,Vellipse[i].x1,Vellipse[i].y1);
    for(int i = 0; i < Point.size(); i++){
        setPixel(Point[i].first, Point[i].second);
        if(i >= 1){
            DrawLine(Point[i-1].first, Point[i-1].second, Point[i].first, Point[i].second);
        }
    }
    vector<pair<int,int> > tp = Point;
    for(int i = 0; i < Polygon.size(); i++){
        Point = Polygon[i];
        Xscan_Algorithm();
    }
    Point = tp;
}

void display(){
    glClear (GL_COLOR_BUFFER_BIT);
    glRectf (-1.0, -1.0, 1.0, 1.0);
    ReDraw();
    glutSwapBuffers ();
}

struct Node{
    db x,dx,b;
    int ymax;
    bool operator < (Node xx) const {
        if(sign(x-xx.x)!=0) return (sign(x-xx.x) == -1);
        else if(sign(dx-xx.dx)!=0) return (sign(dx-xx.dx) == 1);
        else return ymax < xx.ymax;
    }
};

void update(){
    if(Xvalue == 1){
        if(Vline.size() == 0 || (Vline[Vline.size()-1].x0 != X0 && Vline[Vline.size()-1].y0 != Y0))
            Vline.push_back({X0,Y0,X1,Y1});
        else{
            Vline[Vline.size()-1].x1 = X1;
            Vline[Vline.size()-1].y1 = Y1;
        }
    }
    else if(Xvalue == 2){
        if(Vcircle.size() == 0 || (Vcircle[Vcircle.size()-1].x0 != X0 && Vcircle[Vcircle.size()-1].y0 != Y0))
            Vcircle.push_back({X0,Y0,X1,Y1});
        else{
            Vcircle[Vcircle.size()-1].x1 = X1;
            Vcircle[Vcircle.size()-1].y1 = Y1;
        }
    }
    else if(Xvalue == 3){
        if(Vellipse.size() == 0 || (Vellipse[Vellipse.size()-1].x0 != X0 && Vellipse[Vellipse.size()-1].y0 != Y0))
            Vellipse.push_back({X0,Y0,X1,Y1});
        else{
            Vellipse[Vellipse.size()-1].x1 = X1;
            Vellipse[Vellipse.size()-1].y1 = Y1;
        }
    }
    else if(Xvalue == 4){
        if(Point.size() == 0) Point.push_back(make_pair(X1,Y1));
        else if(make_pair(X1, Y1) != Point[Point.size()-1]){
            int xx = Point[Point.size()-1].first;
            int yy = Point[Point.size()-1].second;
            if(abs(xx-X1) + abs(yy-Y1) > 6) Point.push_back(make_pair(X1,Y1));
        }
    }
    display();
}

void Dragmouse(int x,int y){ //鼠标拖拽
    if(Xvalue == 4) return;
    X1 = x; Y1 = y; update();
}

void Mymouse(int button, int state, int x, int y){
    if(button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN){
        Polygon.push_back(Point);
        Xscan_Algorithm();
        Point.clear();
        display();
    }
    else if(button == GLUT_LEFT_BUTTON && state == GLUT_UP){
        X1 = x; Y1 = y;
        update();
    }
    else if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        X0 = x; Y0 = y;
    }
}

int main(int argc, char** argv){
    glutInit(&argc, argv); //初始化
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); //指定单缓存窗口与RGB颜色模式的窗口
    glutInitWindowPosition(500, 300); //设置初始窗口的位置，(0,0)为屏幕左上角位置
    glutInitWindowSize(WinWidth, Winheight); //设置窗口宽度与高度，单位为像素
    glutCreateWindow("图形学实验一"); //创建窗口
    init();
    
    glutDisplayFunc(display);//注册显示回调函数
    glutCreateMenu(Mymenu);//注册菜单回调函数
    glutAddMenuEntry("DrawLine",1);//添加菜单项
    glutAddMenuEntry("DrawCircle",2);
    glutAddMenuEntry("DrawEllipse",3);
    glutAddMenuEntry("Xscan-AreaFilling",4);
    glutAddMenuEntry("ClearScreen",5);
    glutAddMenuEntry("Exit",6);
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
void DrawCircle(int x1,int y1,int x2,int y2){ //画圆函数
    int r = (int)sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
    Bresenham_Circle(x1, y1, r); //Bresenham算法
}
void DrawEllipse(int x1,int y1,int x2,int y2){ //画椭圆函数
    Bresenham_Ellipse((x1+x2)/2, (y1+y2)/2, abs(x1-x2)/2, abs(y1-y2)/2);
}

void Mymenu(int value){
    Xvalue = value;
    if(value == 4){
        Point.clear();
    }
    else if (value == 5){
        glClear(GL_COLOR_BUFFER_BIT);
        glutSwapBuffers();
        Vline.clear();
        Vcircle.clear();
        Vellipse.clear();
        Point.clear();
        Polygon.clear();
    }
    else if (value == 6){
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

void Bresenham_Circle(int xc,int yc,int r){
    int x, y, d;
    x = 0; y = r; d = 5 - 4 * r;
    setPixel(x+xc, y+yc);
    while(x < y)
    {
        if(d < 0) d = d + 8 * x + 12;
        else {d = d + 8 * ( x - y ) + 20; y--;}
        x++;
        setPixel(x+xc,y+yc); setPixel(y+xc,x+yc);
        setPixel(y+xc,-x+yc); setPixel(x+xc,-y+yc);
        setPixel(-x+xc,-y+yc); setPixel(-y+xc,-x+yc);
        setPixel(-x+xc,y+yc); setPixel(-y+xc,x+yc);
    }
}

void Bresenham_Ellipse(int xc,int yc,int a,int b){
    int x = 0, y = b, d1 = 4*b*b+a*a*(-4*b+1), d2; //一开始*4倍，浮点转整数
    setPixel(xc+x,yc+y); setPixel(xc+x, yc-y); setPixel(xc-x, yc+y); setPixel(xc-x, yc-y);
    while(2*b*b*(x+1) < a*a*(2*y-1)){ //先处理斜率 > -1的情况
        if(d1 < 0) d1 += 4*b*b*(2*x+3);
        else {d1 += 4*b*b*(2*x+3)+4*a*a*(-2*y+2); y--;}
        x++;
        setPixel(xc+x,yc+y); setPixel(xc+x, yc-y); setPixel(xc-x, yc+y); setPixel(xc-x, yc-y);
    }
    d2 = b*b*(2*x+1)*(2*x+1)+4*a*a*(y-1)*(y-1)-4*a*a*b*b;
    while(y > 0){ //再处理斜率 < -1的情况
        if(d2 < 0) {d2 += 4*b*b*(2*x+2)+4*a*a*(-2*y+3); x++;}
        else d2 += 4*a*a*(-2*y+3);
        y--;
        setPixel(xc+x,yc+y); setPixel(xc+x, yc-y); setPixel(xc-x, yc+y); setPixel(xc-x, yc-y);
    }
}

void Xscan_Algorithm(){
    int MaxY = 0, MinY = 1e5, n = (int)Point.size();
    for(int i = 0; i < n; i++){
        MaxY = max(MaxY, (int)Point[i].second);
        MinY = min(MinY, (int)Point[i].second);
    }
    vector<Node> NET[2048];
    for(int i = MinY; i <= MaxY; i++) NET[i].clear();
    list<Node> AET; AET.clear();
    for(int i = 0; i < n; i++){
        if(Point[i].second == Point[(i+1)%n].second) continue;
        pair<int,int> tmp; db b = 0.0;
        if(Point[i].second > Point[(i+1)%n].second) tmp = Point[(i+1)%n];
        else tmp = Point[i];
        if(Point[i].first != Point[(i+1)%n].first)
            b = tmp.second-((db)(Point[i].second-Point[(i+1)%n].second)*(db)(tmp.first)/(db)(Point[i].first-Point[(i+1)%n].first));
        Node thp = {(db)tmp.first,(db)(Point[i].first-Point[(i+1)%n].first)/(db)(Point[i].second-Point[(i+1)%n].second),b,max(Point[i].second,Point[(i+1)%n].second)};
        NET[min(Point[i].second,Point[(i+1)%n].second)].push_back(thp);
    }
    for(int i = MinY; i <= MaxY; i++){
        for(int j = 0; j < NET[i].size(); j++) AET.push_back(NET[i][j]);
        AET.sort(); list<Node>::iterator it = AET.begin();
        int flag = 0, flag2 = 0, flag3 = 0, ct = 0, yy[4];
        db xx[4];
        while(it != AET.end()){
            xx[++ct] = (*it).x; yy[ct] = (*it).ymax; it++;
            if(ct == 2 && yy[1] == yy[2] && yy[1] == i && (sign(xx[2]-xx[1]) == 0)) {ct = 0; flag2++;}
            if(ct == 2 && yy[1] > i && yy[2] > i && (sign(xx[2]-xx[1]) == 0)) {flag3++;}
            if(ct == 2 && (sign(xx[2]-xx[1]) == 0))
                if((yy[1] > i && yy[2] == i) || (yy[1] == i && yy[2] > i)) {flag++; ct = 1; xx[1] = xx[2]; yy[1] = yy[2];}
            if(ct == 2){
                int jud = 0;
                if(it != AET.end()){
                    jud = 1; xx[3] = (*it).x; yy[3] = (*it).ymax; it++;
                    if((sign(xx[3]-xx[2]) == 0)){
                        if((yy[3] > i && yy[2] == i) || (yy[2] > i && yy[3] == i)){ct = 2; jud = 0; flag++;}
                        else if(yy[2] == yy[3] && yy[2] == i){flag2++; ct = 1; continue;}
                        else if(yy[2] > i && yy[3] > i) flag3++;
                    }
                }
                for(int k = ceil(xx[1]); k <= floor(xx[2]); k++){
                    if(k < 0) continue;
                    setPixel(k, i);
                }
                if(jud) {xx[1] = xx[3]; yy[1] = yy[3];}
                ct = jud;
            }
        }
//        it = AET.begin(); //调试信息
//        if((int)AET.size()%2){
//            printf("size: %d, flag1: %d, flag2: %d, flag3: %d, Y: %d\n",(int)AET.size(),flag,flag2,flag3,i);
//            while(it != AET.end()) {printf("%Lf,%d ",(*it).x,(*it).ymax); it++;}
//            printf("\n");
//        }
        it = AET.begin();
        while(it != AET.end()){
            if(sign((*it).dx-0.0) != 0) (*it).x = (((db)i+1.0-(*it).b)*(*it).dx);
            if((*it).ymax == i) it = AET.erase(it);
            else it++;
        }
    }
}