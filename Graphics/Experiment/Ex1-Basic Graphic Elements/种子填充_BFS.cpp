#include <GLUT/GLUT.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <queue>
#include <algorithm>
#include <iostream>
using namespace std;

int WinWidth = 1024, Winheight = 800; //窗口宽高
int vis[1200][1200];
int dir[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
int X0,Y0,X1,Y1; //起始坐标与终止坐标
void drawEllipse(int,int,int,int);
void DrawLine(){ //画线函数
    drawEllipse(X0, Y0, 200, 100);
}
void swap(GLint& a,GLint &b) {GLint t = a; a = b; b = t;}
void setPixel(GLint x,GLint y){
    glBegin(GL_POINTS); //把每个顶点作为一个点来处理
    glVertex2i(x,y); //int坐标画点
    glEnd();
}

void drawEllipse(int ox, int oy, int a, int b)//圆心x，圆心y，横长，纵长
{
    float d = b*b + a*a*(-b + 0.25);
    int x = 0, y = b, fx = a*a / sqrt((float)a*a + b*b);
    while (x != fx)
    {
        if (d < 0)
            d += b*b*(2 * x + 3);
        else
        {
            --y;
            d += b*b*(2 * x + 3) + a*a*(-2 * y + 2);
        }
        ++x;
        setPixel(ox + x, oy + y);
        setPixel(ox - x, oy + y);
        setPixel(ox + x, oy - y);
        setPixel(ox - x, oy - y);
    }
    //中点改为(x+0.5,y-1)所以d =
    d = b*b*(x + 0.5)*(x + 0.5) + a*a*(y - 1)*(y - 1) - a*a*b*b;
    while (y > 0)
    {
        if (d < 0)
        {
            ++x;
            d += b*b*(2 * x + 2) + a*a*(-2 * y + 3);
        }
        else
            d += a*a*(-2 * y + 3);
        --y;
        setPixel(ox + x, oy + y);
        setPixel(ox - x, oy + y);
        setPixel(ox + x, oy - y);
        setPixel(ox - x, oy - y);
    }
}

void bfs(int x,int y){
    queue<pair<int,int> > q;
    while(q.size()) q.pop();
    vis[x][y] = 1; setPixel(x, y); q.push(make_pair(x,y));
    while(q.size()){
        int X = q.front().first, Y = q.front().second;
        q.pop();
        for(int i = 0; i <= 3; i++){
            int xx = X+dir[i][0], yy = Y+dir[i][1];
            if(xx < 0 || yy < 0 || xx > WinWidth || yy > Winheight || vis[xx][yy]) continue;
            vis[xx][yy] = 1;
            setPixel(xx, yy);
            q.push(make_pair(xx,yy));
        }
    }
}
//void calc(int x,int y){
//    int color = GetPixelColor(x,y);
//    cout << "color: " << color << endl;
//    //    float* v = new float[3];
////    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, v);
////    cout << "R:" << v[0] << " ";
////    cout << "G:" << v[1] << " ";
////    cout << "B:" << v[2] << endl;
//}

void mymouse(int button, int state, int x, int y){
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP){ //GLUT_LEFT_BUTTON 左键 GLUT_UP 松开 GLUT_DOWN 按下
        X1 = x; Y1 = y;
//        printf("(%d,%d) -> (%d,%d)\n",X0,Y0,X1,Y1);
//        DrawLine();
        bfs(x,y);
//        calc(x,y);
        glFlush(); //强制绘图命令被执行
    }
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        X0 = x; Y0 = y;
    }
}

void init(){
    glClearColor(0,0,0,1); //(red green blue alpha) alpha表示混合因子
    glClear(GL_COLOR_BUFFER_BIT); //将屏幕所有像素点还原为"底色"
    glPointSize(5.0f); //指定栅格化点的直径
    glMatrixMode(GL_PROJECTION); //声明接下来要进行的操作，GL_PROJECTION 投影， GL_MODELVIEW 模型视图， GL_TEXTURE 纹理
    glLoadIdentity(); //加载一个单位矩阵
    gluOrtho2D(0,WinWidth,Winheight,0); //定义裁剪面
    glColor3f(255,215,0); //设置画点颜色 R-G-B
}

void display(){
    glutMouseFunc(mymouse);
//    glutPassiveMotionFunc(calc);
}

int main(int argc, char** argv){
    glutInit(&argc, argv); //初始化
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); //指定单缓存窗口与RGB颜色模式的窗口
    glutInitWindowPosition(500, 300); //设置初始窗口的位置，(0,0)为屏幕左上角位置
    glutInitWindowSize(WinWidth, Winheight); //设置窗口宽度与高度，单位为像素
    glutCreateWindow("OpenGL实验"); //创建窗口
    init();
    glutDisplayFunc(display); //绘制回调函数
    glutMainLoop();
}
