#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <string>
#include <regex>
#include <vector>
#include <fstream>
#include <GLUT/GLUT.h>
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define LOG1(x1,x2) cout << x1 << ": " << x2 << endl;
#define LOG2(x1,x2,y1,y2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << endl;
#define LOG3(x1,x2,y1,y2,z1,z2) cout << x1 << ": " << x2 << " , " << y1 << ": " << y2 << " , " << z1 << ": " << z2 << endl;
using namespace std;

GLUnurbsObj *theNurb, *theline; // 指向一个NURBS曲面对象的指针
GLfloat *ctlpoints, *knot1, *knot2, ***t_point, **knt, B[2][60][8];
int if_point = 0, if_control_polygon = 0, if_face = 0, if_control_move = 0, if_Iso_horizontal = 0, if_Iso_vertical = 0, n[2], K[2], Size_Iso_horizontal = 20, Size_Iso_vertical = 20;
int GLUTwindow = 0, GLUTwindow_height = 800, GLUTwindow_width = 800;     //设置窗口的大小
int GLUTmouse[2] = { 0, 0 };
int GLUTbutton[3] = { 0, 0, 0 };
int GLUTarrows[4] = { 0, 0, 0, 0 };
int GLUTmodifiers = 0, scaling = 0, translating = 0, rotating = 0;
float scale = 1.0;
float center[3] = { 0.0, 0.0, 0.0 };
float rotation[3] = { 0.0, 0.0, 0.0 };
float translation[3] = { 0.0, 0.0, -4.0 };
GLfloat light0_position[] = { 0.0, 0.0, 0.0, 0.0 };
GLuint texGround;
vector<GLfloat> Point[2][12]; //等参线上的点
#define BMP_Header_Length 54  //图像数据在内存块中的偏移量
typedef double db;
const db EPS = 1e-7;
inline int sign(db a) {return a < -EPS ? -1 : a > EPS; } //返回-1表示a < 0, 1表示a > 0, 0表示a = 0

void display();
void GLUTRedraw(void);
void GLUTResize(int w, int h);
void GLUTMotion(int x, int y);
void GLUTInit(int *argc, char **argv);
void GLUTKeyboard(unsigned char key, int x, int y);
void GLUTSpecial(int key, int x, int y);
void GLUTMouse(int button, int state, int x, int y);
GLuint load_texture(const char* file_name);
void init_surface(void);

void GLUTMainLoop(void)
{
    float box_size[3][2];
    for(int i = 0; i < 3; i++){
        box_size[i][0] = 1.030F;
        box_size[i][1] = -1.030F;
    }
    for(int i = 0; i < n[0]*n[1]*3; i += 3){
        float bx = ctlpoints[i], by = ctlpoints[i+1], bz = ctlpoints[i+2];
        box_size[0][0] = min(box_size[0][0],bx);
        box_size[0][1] = max(box_size[0][1],bx);
        
        box_size[1][0] = min(box_size[1][0],by);
        box_size[1][1] = max(box_size[1][1],by);
        
        box_size[2][0] = min(box_size[2][0],bz);
        box_size[2][1] = max(box_size[2][1],bz);
    }
    // 设置初始视图大小
    float dx = box_size[0][1] - box_size[0][0];
    float dy = box_size[1][1] - box_size[1][0];
    float dz = box_size[2][1] - box_size[2][0];
    scale = 2.0 / sqrt(dx*dx + dy*dy + dz*dz);
    // 设置初始视觉中心
    center[0] = 0.5 * (box_size[0][0] + box_size[0][1]);
    center[1] = 0.5 * (box_size[1][0] + box_size[1][1]);
    center[2] = 0.5 * (box_size[2][0] + box_size[2][1]);
    glutMainLoop();
}

int main(int argc, char **argv)
{
    GLUTInit(&argc, argv);
    texGround = load_texture("/Users/gene_liu/Desktop/BMP/checkerboard.bmp");  //加载纹理
    //AussenWand_C、checkerboard
    GLUTMainLoop();
    return 0;
}

void show_points(){
    glPointSize(2);
    glDisable(GL_LIGHTING);
    glColor3f(0.5, 0.5, 0.0);
    for(int i = 0; i < n[0]*n[1]*3; i += 3){
        glBegin(GL_POINTS);
        GLfloat tx = ctlpoints[i], ty = ctlpoints[i+1], tz = ctlpoints[i+2];
        glVertex3f(tx, ty, tz);
        glEnd();
    }
    glEnable(GL_LIGHTING);
}

void show_control_polygon(){
    glDisable(GL_LIGHTING);
    rep(i,0,n[0]-1)
        rep(j,0,n[1]-2){
            glColor3f(0.0, 0.0, 0.0);
            glBegin(GL_LINES);
            glVertex3f(t_point[i][j][0], t_point[i][j][1], t_point[i][j][2]);
            glVertex3f(t_point[i][j+1][0], t_point[i][j+1][1], t_point[i][j+1][2]);
            glEnd();
        }
    
    rep(i,0,n[1]-1)
        rep(j,0,n[0]-2){
            glColor3f(0.0, 0.0, 0.0);
            glBegin(GL_LINES);
            glVertex3f(t_point[j][i][0], t_point[j][i][1], t_point[j][i][2]);
            glVertex3f(t_point[j+1][i][0], t_point[j+1][i][1], t_point[j+1][i][2]);
            glEnd();
        }
    glEnable(GL_LIGHTING);
}

void calc_B(GLfloat u,int id){
    rep(i,0,n[id]-1){
        if(u != 1 && u >= knt[id][i] && u < knt[id][i+1]) B[id][i][1] = 1;
        else if(u == 1 && u > knt[id][i] && u <= knt[id][i+1]) B[id][i][1] = 1;
        else B[id][i][1] = 0;
    }
    rep(k,2,K[id]+1){
        rep(i,0,n[id]-1){
            B[id][i][k] = 0;
            if(sign(knt[id][i+k-1]-knt[id][i]) != 0)
                B[id][i][k] += (B[id][i][k-1]*(u-knt[id][i]))/(knt[id][i+k-1]-knt[id][i]);
            if(sign(knt[id][i+k]-knt[id][i+1]) != 0)
                B[id][i][k] += (B[id][i+1][k-1])*(knt[id][i+k]-u)/(knt[id][i+k]-knt[id][i+1]);
        }
    }
}

void show_line(int value,int Base){
    glPointSize(2);
    glDisable(GL_LIGHTING);
    if(value == 0) glColor3f(0.0, 0.5, 0.5);
    else glColor3f(0.6, 0.0, 0.0);
    for(int j = 0; j <= Base; j++){ //线数
        for(int i = j*101*3; i < ((j+1)*101*3-3); i += 3){
            GLfloat x1 = Point[value][Base/5][i], y1 = Point[value][Base/5][i+1], z1 = Point[value][Base/5][i+2];
            GLfloat x2 = Point[value][Base/5][i+3], y2 = Point[value][Base/5][i+4], z2 = Point[value][Base/5][i+5];
            glBegin(GL_LINE_STRIP);
            glVertex3f(x1, y1, z1);
            glVertex3f(x2, y2, z2);
            glEnd();
        }
    }
    glEnable(GL_LIGHTING);
}

void show_face(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(0.0, 1.0, 0.0);
    glRotatef(1.0, 0.7, -0.6, 1.0); // 旋转变换
    gluBeginSurface(theNurb); // 开始曲面绘制
    gluNurbsSurface(theNurb, n[0]+K[0]+1, knot1, n[1]+K[1]+1, knot2, n[1] * 3, 3, ctlpoints, K[0]+1, K[1]+1, GL_MAP2_VERTEX_3); // 定义曲面的数学模型，确定其形状
    gluEndSurface(theNurb); // 结束曲面绘制
}

void show_texture(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glRotatef(1.0, 0.7, -0.6, 1.0); // 旋转变换
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texGround);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    
    float uTextureknots[4] = {0.0, 0.0, 12.0, 12.0};
    float vTextureknots[4] = {0.0, 0.0, 7.0, 7.0};
    float texturePoints[2][2][2] =
    {
        {{0.0, 0.0}, {0.0, 5.0}},
        {{5.0, 0.0}, {5.0, 5.0}}
    };
    
    gluBeginSurface(theNurb); // 开始曲面绘制
    gluNurbsSurface(theNurb, 4, uTextureknots, 4, vTextureknots, 4, 2, texturePoints[0][0], 2, 2, GL_MAP2_TEXTURE_COORD_2);
    gluNurbsSurface(theNurb, n[0]+K[0]+1, knot1, n[1]+K[1]+1, knot2, n[1] * 3, 3, ctlpoints, K[0]+1, K[1]+1, GL_MAP2_VERTEX_3); // 定义曲面的数学模型，确定其形状
    gluEndSurface(theNurb); // 结束曲面绘制
    glDisable(GL_TEXTURE_2D);
}


void GLUTRedraw(void)
{
    //进行空间的重绘
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glScalef(scale, scale, scale); //放大
    glTranslatef(translation[0], translation[1], 0.0); //移动
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat) GLUTwindow_width /(GLfloat) GLUTwindow_height, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(translation[0], translation[1], translation[2]);
    glScalef(scale, scale, scale);
    
    //刷新放缩的大小
    glRotatef(rotation[0], 1.0, 0.0, 0.0);
    glRotatef(rotation[1], 0.0, 1.0, 0.0);          //控制不同角度
    glTranslatef(-center[0], -center[1], -center[2]);
    
    //改变旋转中心
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //设置光照, 载入不同光源的位置
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    
    if(if_face == 1) show_face();
    else if(if_face == 2) show_texture();
    if(if_point == 1) show_points();
    if(if_Iso_horizontal == 1) show_line(1, Size_Iso_horizontal);
    if(if_Iso_vertical == 1) show_line(0, Size_Iso_vertical);
    if(if_control_polygon == 1) show_control_polygon();
    glutSwapBuffers();
}

void GLUTResize(int w, int h)
{
    glViewport(0, 0, w, h);
    //拖拽之后用获取的高度和宽度信息更新全局变量
    GLUTwindow_width = w;
    GLUTwindow_height = h;
    glutPostRedisplay();
}

void GLUTMotion(int x, int y)
{
    y = GLUTwindow_height - y;
    if (rotating) {
        rotation[0] += -0.5 * (y - GLUTmouse[1]);
        rotation[1] += 0.5 * (x - GLUTmouse[0]);
    }
    else if (scaling) {
        scale *= exp(2.0 * (float)( (x- GLUTmouse[0])) / (float) GLUTwindow_width);
        // 按照y方向控制: scale *= exp(2.0 * (float)( (y- GLUTmouse[1])) / (float) GLUTwindow_width);
    }
    else if (translating) {
        translation[0] += 2.0 * (float) (x - GLUTmouse[0]) / (float) GLUTwindow_width;
        translation[1] += 2.0 * (float) (y - GLUTmouse[1]) / (float) GLUTwindow_height;
    }
    //设置定点中心
    GLUTmouse[0] = x;
    GLUTmouse[1] = y;
}

void GLUTMouse(int button, int state, int x, int y)
{
    y = GLUTwindow_height - y;
    int kind = 0;
    if(button == GLUT_LEFT_BUTTON && if_control_move == 1){
        translating = 1; kind = 2;
        rotating = scaling = 0;
    }else{
        if(button == GLUT_LEFT_BUTTON && if_control_move == 0){
            kind = 0; rotating = 1;
            scaling = translating = 0;
        }else if(button == GLUT_LEFT_BUTTON && if_control_move == 2){
            kind = scaling = 1;
            rotating = translating = 0;
        }
    }
    if (rotating || scaling || translating) glutIdleFunc(GLUTRedraw);
    else glutIdleFunc(0);
    GLUTbutton[kind] = (state == GLUT_DOWN) ? 1 : 0;
    GLUTmodifiers = glutGetModifiers();
    GLUTmouse[0] = x; GLUTmouse[1] = y;
}

void GLUTSpecial(int key, int x, int y)
{
    y = GLUTwindow_height - y;
    //记录当下鼠标点击的位置
    GLUTmouse[0] = x; GLUTmouse[1] = y;
    GLUTmodifiers = glutGetModifiers();
    glutPostRedisplay();
}

void GLUTKeyboard(unsigned char key, int x, int y)
{
    if(key == '1'){
        GLfloat value = 0.5;
        if(light0_position[0] != 0) value = 0;
        for(int i = 0 ; i  < 4 ;i ++)
            light0_position[i] = value;
    }
    else if(key == 'q')
        Size_Iso_horizontal = min(Size_Iso_horizontal+5,50);
    else if(key == 'w')
        Size_Iso_horizontal = max(Size_Iso_horizontal-5,5);
    else if(key == 'a')
        Size_Iso_vertical = min(Size_Iso_vertical+5,50);
    else if(key == 's')
        Size_Iso_vertical = max(Size_Iso_vertical-5,5);
    GLUTmouse[0] = x;
    GLUTmouse[1] = GLUTwindow_height - y;
    GLUTmodifiers = glutGetModifiers();
}

void Mymenu(int value){
    if(value == 1) if_point ^= 1;
    else if(value == 2) if_control_polygon ^= 1;
    else if(value == 3) if_Iso_horizontal ^= 1;
    else if(value == 4) if_Iso_vertical ^= 1;
    else if(value == 5) if_face = (if_face != 1) ? 1 : 0;
    else if(value == 6) if_face = (if_face != 2) ? 2 : 0;
    else if(value == 7) if_control_move = 0;
    else if(value == 8) if_control_move = 1;
    else if(value == 9) if_control_move = 2;
    else if (value == 10) exit(0);
}

void init_surface(void){
    freopen("/Users/gene_liu/Desktop/face.sur","r",stdin);
    scanf("%d %d",&n[0],&n[1]);
    scanf("%d %d",&K[0],&K[1]);
    knot1 = new GLfloat[n[0]+K[0]+1];
    knot2 = new GLfloat[n[1]+K[1]+1];
    ctlpoints = new GLfloat[n[0]*n[1]*3];
    knt = new GLfloat *[2];
    knt[0] = new GLfloat [n[0]+K[0]+1];
    rep(i,0,n[0]+K[0]){
        scanf("%f",&knot1[i]);
        knt[0][i] = knot1[i];
    }
    knt[1] = new GLfloat [n[1]+K[1]+1];
    rep(i,0,n[1]+K[1]){
        scanf("%f",&knot2[i]);
        knt[1][i] = knot2[i];
    }
    t_point = new GLfloat **[n[0]];
    int tot = 0;
    rep(i,0,n[0]-1){
        t_point[i] = new GLfloat *[n[1]];
        rep(j,0,n[1]-1){
            t_point[i][j] = new GLfloat[3];
            GLfloat a,b,c,d; scanf("%f%f%f%f",&a,&b,&c,&d);
            ctlpoints[tot++] = a; ctlpoints[tot++] = b; ctlpoints[tot++] = c;
            t_point[i][j][0] = a; t_point[i][j][1] = b; t_point[i][j][2] = c;
        }
    }
    for(int value = 0; value <= 1; value++){
        for(int base = 5; base <= 50; base+=5){
            rep(vv,0,base){
                GLfloat v = (1.0/base)*vv;
                calc_B(v,value);
                rep(uu,0,100){
                    GLfloat u = (1.0/100)*(GLfloat)uu, tx = 0, ty = 0, tz = 0;
                    calc_B(u,value^1);
                    rep(i,0,n[0]-1){
                        rep(j,0,n[1]-1){
                            tx += t_point[i][j][0]*B[0][i][K[0]+1]*B[1][j][K[1]+1];
                            ty += t_point[i][j][1]*B[0][i][K[0]+1]*B[1][j][K[1]+1];
                            tz += t_point[i][j][2]*B[0][i][K[0]+1]*B[1][j][K[1]+1];
                        }
                    }
                    if(tx != 0 || ty != 0 || tz != 0){
                        Point[value][base/5].push_back(tx);
                        Point[value][base/5].push_back(ty);
                        Point[value][base/5].push_back(tz);
                    }
                }
            }
        }
    }
}

void GLUTInit(int *argc, char **argv)
{
    init_surface();
    glutInit(argc, argv);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(GLUTwindow_width, GLUTwindow_height);
    GLUTwindow = glutCreateWindow("实验四_B样条曲面");
    glutReshapeFunc(GLUTResize);
    
    //设置重绘信息
    glutDisplayFunc(GLUTRedraw);
    
    //注册键盘事件
    glutKeyboardFunc(GLUTKeyboard);
    glutSpecialFunc(GLUTSpecial);
    glutMouseFunc(GLUTMouse);
    glutMotionFunc(GLUTMotion);
    glutIdleFunc(0);
    
    //注册菜单
    glutCreateMenu(Mymenu);//注册菜单回调函数
    glutAddMenuEntry("B-spline_ControlPoint",1);//添加菜单项
    glutAddMenuEntry("B-spline_ControlPolygon",2);
    glutAddMenuEntry("B-spline_Isoparm_horizontal",3);
    glutAddMenuEntry("B-spline_Isoparm_vertical",4);
    glutAddMenuEntry("B-spline_Face",5);
    glutAddMenuEntry("B-spline_Texture",6);
    glutAddMenuEntry("B-spline_Rotation",7);
    glutAddMenuEntry("B-spline_Translation",8);
    glutAddMenuEntry("B-spline_Scaling",9);
    glutAddMenuEntry("Exit",10);
    glutAttachMenu(GLUT_RIGHT_BUTTON);//把当前菜单注册到指定的鼠标键
    
    // 设置光照信息
    glClearColor(0.0, 1.0, 0.0, 1.0);
    //代码开关3：设置材质与光源
    GLfloat ambient[] = { 0.4, 0.6, 0.2, 1.0 };
    GLfloat mat_diffuse[] = { 0.8, 0.6, 0.3, 1.0 };
    GLfloat mat_specular[] = { 1.0, 0.5, 0.3, 0.5 };
    GLfloat mat_shininess[] = { 50.0 };
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
    theNurb = gluNewNurbsRenderer(); // 创建一个NURBS曲面对象
    theline = gluNewNurbsRenderer();
    gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 5.0);
    gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);
}

int power_of_two(int n)     //判断一个整数是不是2的整数次幂
{
    if(n <= 0) return 0;
    return (n & (n-1)) == 0;
}

GLuint load_texture(const char* file_name)
{
    GLint width, height, total_bytes;
    GLubyte* pixels = 0;
    GLuint last_texture_ID=0, texture_ID = 0;
    
    // 打开文件，如果失败，返回
    FILE* pFile = fopen(file_name, "rb");
    if( pFile == 0 )
        return 0;
    
    // 读取文件中图象的宽度和高度
    fseek(pFile, 0x0012, SEEK_SET);
    fread(&width, 4, 1, pFile);
    fread(&height, 4, 1, pFile);
    fseek(pFile, BMP_Header_Length, SEEK_SET);
    
    // 计算每行像素所占字节数，并根据此数据计算总像素字节数
    {
        GLint line_bytes = width * 3;
        while( line_bytes % 4 != 0 )
            ++line_bytes;
        total_bytes = line_bytes * height;
    }
    
    // 根据总像素字节数分配内存
    pixels = (GLubyte*)malloc(total_bytes);
    if( pixels == 0 )
    {
        fclose(pFile);
        return 0;
    }
    
    // 读取像素数据
    if( fread(pixels, total_bytes, 1, pFile) <= 0 )
    {
        free(pixels);
        fclose(pFile);
        return 0;
    }
    
    // 对就旧版本的兼容，如果图象的宽度和高度不是的整数次方，则需要进行缩放
    // 若图像宽高超过了OpenGL规定的最大值，也缩放
    {
        GLint max;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
        if( !power_of_two(width)
           || !power_of_two(height)
           || width > max
           || height > max )
        {
            const GLint new_width = 256;
            const GLint new_height = 256; // 规定缩放后新的大小为边长的正方形
            GLint new_line_bytes, new_total_bytes;
            GLubyte* new_pixels = 0;
            
            // 计算每行需要的字节数和总字节数
            new_line_bytes = new_width * 3;
            while( new_line_bytes % 4 != 0 )
                ++new_line_bytes;
            new_total_bytes = new_line_bytes * new_height;
            
            // 分配内存
            new_pixels = (GLubyte*)malloc(new_total_bytes);
            if( new_pixels == 0 )
            {
                free(pixels);
                fclose(pFile);
                return 0;
            }
            
            // 进行像素缩放
            gluScaleImage(GL_RGB,
                          width, height, GL_UNSIGNED_BYTE, pixels,
                          new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);
            
            // 释放原来的像素数据，把pixels指向新的像素数据，并重新设置width和height
            free(pixels);
            pixels = new_pixels;
            width = new_width;
            height = new_height;
        }
    }
    
    // 分配一个新的纹理编号
    glGenTextures(1, &texture_ID);
    if( texture_ID == 0 )
    {
        free(pixels);
        fclose(pFile);
        return 0;
    }
    // 绑定新的纹理，载入纹理并设置纹理参数
    // 在绑定前，先获得原来绑定的纹理编号，以便在最后进行恢复
    GLint lastTextureID=last_texture_ID;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTextureID);
    glBindTexture(GL_TEXTURE_2D, texture_ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                 GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
    glBindTexture(GL_TEXTURE_2D, lastTextureID);  //恢复之前的纹理绑定
    free(pixels);
    
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    //    glEnable(GL_TEXTURE_2D);
    return texture_ID;
}
