#include <GLUT/GLUT.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>
using namespace std;

int WinWidth = 1024, Winheight = 720; //窗口宽高
int X0,Y0,X1,Y1; //起始坐标与终止坐标
void DrawLine();
void BresenhamLine(int,int,int,int);

void DrawLine(){ //画线函数
    BresenhamLine(X0,Y0,X1,Y1); //Bresenham算法
}
void swap(GLint& a,GLint &b) {GLint t = a; a = b; b = t;}
void setPixel(GLint x,GLint y){
    glBegin(GL_POINTS); //把每个顶点作为一个点来处理
    glVertex2i(x,y); //int坐标画点
    glEnd();
}

void BresenhamLine(GLint x1,GLint y1,GLint x2,GLint y2){
//    glClear(GL_COLOR_BUFFER_BIT);//清除窗口显示内容
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

void dragmouse(int x,int y){ //鼠标拖拽
    glColor3f(0,0,0); //设置画点颜色 R-G-B
    DrawLine();
    glColor3f(255,215,0);
    X1 = x; Y1 = y;
    printf("(%d,%d) -> (%d,%d)\n",X0,Y0,X1,Y1);
    DrawLine();
    glFlush();
}

void mymouse(int button, int state, int x, int y){
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP){ //GLUT_LEFT_BUTTON 左键 GLUT_UP 松开 GLUT_DOWN 按下
        X1 = x; Y1 = y;
        printf("(%d,%d) -> (%d,%d)\n",X0,Y0,X1,Y1);
        DrawLine();
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

int main(int argc, char** argv){
    glutInit(&argc, argv); //初始化
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); //指定单缓存窗口与RGB颜色模式的窗口
    glutInitWindowPosition(500, 300); //设置初始窗口的位置，(0,0)为屏幕左上角位置
    glutInitWindowSize(WinWidth, Winheight); //设置窗口宽度与高度，单位为像素
    glutCreateWindow("OpenGL实验"); //创建窗口
    init();
    
    glutDisplayFunc(DrawLine); //绘制回调函数
    glutMouseFunc(mymouse);
    glutMotionFunc(dragmouse);
    glutMainLoop();
}
