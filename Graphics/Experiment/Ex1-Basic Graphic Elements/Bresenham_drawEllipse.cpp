#include <GLUT/GLUT.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>
using namespace std;

int WinWidth = 1024, Winheight = 720; //窗口宽高
int X0 = 0,Y0 = 0,X1 = 0,Y1 = 0; //起始坐标与终止坐标
void drawEllipse(int,int,int,int);
void DrawLine(){ //画线函数
    drawEllipse((X0+X1)/2, (Y1+Y0)/2, abs(X1-X0)/2, abs(Y1-Y0)/2);
}
void swap(GLint& a,GLint &b) {GLint t = a; a = b; b = t;}
void setPixel(GLint x,GLint y){
    glBegin(GL_POINTS); //把每个顶点作为一个点来处理
    glVertex2i(x,y); //int坐标画点
    glEnd();
}

void drawEllipse(int xc,int yc,int a,int b){
    glClear(GL_COLOR_BUFFER_BIT);//清除窗口显示内容
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

void dragmouse(int x,int y){ //鼠标拖拽
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

