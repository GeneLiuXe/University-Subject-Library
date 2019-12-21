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
using namespace std;

struct Vertex {
    float x, y, z; //定义三维图形的点
};
struct Face {
    Face(void) : verts(0) {};
    vector<Vertex> verts;   //记录面的所有顶点
    float normal[3];        //记录面的法向量，分别是x，y，z三个方向
    vector<pair<float,float> > texture;
};
struct myMesh {
    myMesh(void) : verts(0), faces(0) {};
    vector<Vertex> verts;
    vector<Face> faces;
    vector<pair<float,float> > texture;
}mesh;

int if_face=0, if_line=0, if_point=0, if_control_move = 0;
int GLUTwindow = 0, GLUTwindow_height = 800, GLUTwindow_width = 800;     //设置窗口的大小
int GLUTmouse[2] = { 0, 0 };
int GLUTbutton[3] = { 0, 0, 0 };
int GLUTarrows[4] = { 0, 0, 0, 0 };
int GLUTmodifiers = 0, scaling = 0, translating = 0, rotating = 0;
float scale = 1.0;
float center[3] = { 0.0, 0.0, 0.0 };
float rotation[3] = { 0.0, 0.0, 0.0 };
float translation[3] = { 0.0, 0.0, -4.0 };
GLfloat my_set_material[] = { 1.0, 1.0, 0.1, 0.8};
GLfloat light0_position[] = { 4.0, 4.0, 4.0, 0.0 };
GLuint texGround;
#define BMP_Header_Length 54  //图像数据在内存块中的偏移量

void display();
void draw_points();
void draw_lines();
void draw_faces();
void GLUTRedraw(void);
void ReaderOBj(string filepath);
void get_normal(Face& face);
void GLUTResize(int w, int h);
void GLUTMotion(int x, int y);
void GLUTInit(int *argc, char **argv);
void GLUTKeyboard(unsigned char key, int x, int y);
void GLUTSpecial(int key, int x, int y);
void GLUTMouse(int button, int state, int x, int y);
GLuint load_texture(const char* file_name);

int power_of_two(int n)     //判断一个整数是不是2的整数次幂
{
    if(n <= 0) return 0;
    return (n & (n-1)) == 0;
}

void GLUTMainLoop(void)
{
    float box_size[3][2];
    for(int i = 0; i < 3; i++){
        box_size[i][0] = 1.030F;
        box_size[i][1] = -1.030F;
    }
    for (int i = 0; i < (int)(mesh.verts.size()); i++) {
        float bx = mesh.verts[i].x, by = mesh.verts[i].y, bz = mesh.verts[i].z;
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
    ReaderOBj("/Users/gene_liu/Desktop/Graphics/实验三/OBJ/earth.obj");    //OBJ_reader；
    texGround = load_texture("/Users/gene_liu/Desktop/Graphics/实验三/BMP/Clouds.bmp");  //加载纹理
    //earth-Clouds
    //elefante-elefantefull
    GLUTMainLoop();
    return 0;
}

void get_normal(Face& face){
    // 通过公式计算面的法线
    face.normal[0] = face.normal[1] = face.normal[2] = 0;
    if((int)face.verts.size() == 0) return;
    Vertex v1 = face.verts[(int)face.verts.size()-1];
    for (int i = 0; i < (int)face.verts.size(); i++) {
        Vertex v2 = face.verts[i];
        //新建所有的点
        face.normal[0] += (v1.y - v2.y) * (v1.z + v2.z);
        face.normal[1] += (v1.z - v2.z) * (v1.x + v2.x);
        face.normal[2] += (v1.x - v2.x) * (v1.y + v2.y);
        //首先完成叉乘的工作
        v1 = v2;
    }
    //计算归一化法线
    float squared_normal_length = 0.0;
    for(int i = 0; i < 3; i++)
        squared_normal_length += face.normal[i]*face.normal[i];
    float normal_length = sqrt(squared_normal_length);
    
    //得到归一化长度
    if (normal_length > 1.0E-6) { //精度控制
        for(int i = 0; i < 3; i++)
            face.normal[i] /= normal_length;
    }
}

void ReaderOBj(string filepath)
{
    // 初始化正则
    const regex RE_OBJ_VERTEX("v (.*) (.*) (.*)");             // 读点正则
    const regex RE_OBJ_TEXVERTEX("vt ([^\\s]*) ([^\\s]*)");        // 读边正则
    const regex RE_OBJ_FACE("f (.*)");                         // 读面正则
    const regex RE_OBJ_FACE_VINDEX("([0-9]+)\/([0-9]+)(\/[0-9]+)*");    // 读面上顶点编号正则
    
    // 清空原数据
    mesh.verts.clear();
    mesh.faces.clear();
    mesh.texture.clear();
    // 一次性将文本内容读入到string
    ifstream ifs(filepath);
    string content((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>()), tmp;
    ifs.close();
    // 正则提取内容
    for(sregex_iterator p(content.cbegin(), content.cend(), RE_OBJ_TEXVERTEX), q; p != q; ++p){
        mesh.texture.push_back({(float)atof(p->format("$1").c_str()), (float)atof(p->format("$2").c_str())});
    }
    for(sregex_iterator p(content.cbegin(), content.cend(), RE_OBJ_VERTEX), q; p != q; ++p){
        mesh.verts.push_back((Vertex){(float)atof(p->format("$1").c_str()), (float)atof(p->format("$2").c_str()), (float)atof(p->format("$3").c_str())});
    }
    for(sregex_iterator p(content.cbegin(), content.cend(), RE_OBJ_FACE), q; p != q; ++p)
    {
        tmp = p->format("$1");
        Face a;
        a.verts.clear();
        a.texture.clear();
        for(sregex_iterator pp(tmp.cbegin(), tmp.cend(), RE_OBJ_FACE_VINDEX), qq; pp != qq; ++pp){
            int vx = atoi(pp->format("$1").c_str())-1;
            int vt = atoi(pp->format("$2").c_str())-1;
            a.verts.push_back(mesh.verts[vx]);
            a.texture.push_back(mesh.texture[vt]);
        }
        get_normal(a);
        mesh.faces.push_back(a);
    }
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

void draw_faces(){
    //设置满散射
    static GLfloat light0_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
    glBegin(GL_POINTS);
    for (int i = 0; i < (int)mesh.faces.size(); i++) {
        Face& face = mesh.faces[i];
        glBegin(GL_POLYGON);        //绘制多边形
        //在绘制面的过程中载入我们已经计算好的法线量信息
        glColor3f(0.5, 0.5, 0.5);
        glNormal3fv(face.normal);   //在绘制面的时候同时载入法向量信息
        for (int j = 0; j < (int)face.verts.size(); j++) {
            Vertex vert = face.verts[j];
            glVertex3f(vert.x, vert.y, vert.z);
        }
        glEnd();
    }
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();
    
    glDisable(GL_LIGHT0);
    glDisable(GL_NORMALIZE);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
}

void draw_points(){
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, my_set_material);
    //绘制点的信息
    glPointSize(2);
    for (int j  = 0 ;  j < (int)mesh.verts.size(); j++) {
        glColor3f(0.5, 0.5,  0.0);
        glBegin(GL_POINTS);
        glVertex3f( (float)(mesh.verts[j].x), (float)(mesh.verts[j].y), (float)(mesh.verts[j].z));
        glEnd();
    }
    glDisable(GL_LIGHT0);
    glDisable(GL_NORMALIZE);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
}

void draw_lines(){
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, my_set_material);
    double temp_x = 0, temp_y = 0, temp_z = 0;
    for (int i = 0; i < (int)mesh.faces.size(); i++) {
        Face& face = mesh.faces[i];
        glColor3f(0.0, 0.7, 0.7);
        glBegin(GL_LINES);
        for (int j = 0; j < (int)face.verts.size(); j++) {
            Vertex vert = face.verts[j];
            if(j==0){
                temp_x  = vert.x;
                temp_y  = vert.y;
                temp_z  = vert.z;
                continue;
            }
            glVertex3f(temp_x, temp_y, temp_z);
            glVertex3f(vert.x, vert.y, vert.z);
            temp_x  = vert.x;
            temp_y  = vert.y;
            temp_z  = vert.z;
        }
        glEnd();
    }
    glDisable(GL_LIGHT0);
    glDisable(GL_NORMALIZE);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
}

void display()
{
    static GLfloat light0_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texGround);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    
    for (int i = 0; i < (int)mesh.faces.size(); i++) {
        Face& face = mesh.faces[i];
        glBegin(GL_POLYGON); //绘制多边形
        //在绘制面的过程中载入我们已经计算好的法线量信息
        glNormal3fv(face.normal);           //在绘制面的时候同时载入法向量信息
        for (int j = 0; j < (int)face.verts.size(); j++) {
            glTexCoord2f(face.texture[j].first, face.texture[j].second);
            glVertex3f(face.verts[j].x, face.verts[j].y, face.verts[j].z);
        }
        glEnd();
    }
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
    glRotatef(rotation[2], 0.0, 0.0, 1.0);
    glTranslatef(-center[0], -center[1], -center[2]);
    
    //改变旋转中心
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //设置光照, 载入不同光源的位置
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    
    //这里可以调整环境颜色和散射颜色数组
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, my_set_material);
    
    // 绘制表面
    if(if_point == 1)
        draw_points();
    if(if_line == 1)
        draw_lines();
    if(if_face == 1)
        draw_faces();
    else if(if_face == 2)
        display();
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
        //控制旋转
        rotation[0] += -0.5 * (y - GLUTmouse[1]);
        rotation[2] += 0.5 * (x - GLUTmouse[0]);
        rotation[1] += 0.5 * (x - GLUTmouse[0]);
    }
    else if (scaling) {
        // 控制缩放, GLUTmouse存储了之前记录的点信息
        scale *= exp(2.0 * (float)( (x- GLUTmouse[0])) / (float) GLUTwindow_width);
        // 按照y方向控制: scale *= exp(2.0 * (float)( (y- GLUTmouse[1])) / (float) GLUTwindow_width);
    }
    else if (translating) {
        // 控制平移
        translation[0] += 2.0 * (float) (x - GLUTmouse[0]) / (float) GLUTwindow_width;
        translation[1] += 2.0 * (float) (y - GLUTmouse[1]) / (float) GLUTwindow_height;
    }
    //设置定点中心
    GLUTmouse[0] = x;
    GLUTmouse[1] = y;
}

void GLUTMouse(int button, int state, int x, int y)
{
    cout << "控制键状态: " << if_control_move << endl;
    y = GLUTwindow_height - y;
    int kind = 0;
    if(button == GLUT_LEFT_BUTTON && if_control_move == 1){
        translating = 1; kind = 2;
        cout << "double" << endl;
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
    cout << "此时的B 选择的操作："<< kind<< endl;
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
        for(int i = 0 ; i  < 4 ;i ++)
            light0_position[i] += 0.1;
    }
    else if(key == '2'){
        for(int i = 0 ; i  < 4 ;i ++)
            light0_position[i] -= 0.1;
    }
    GLUTmouse[0] = x;
    GLUTmouse[1] = GLUTwindow_height - y;
    GLUTmodifiers = glutGetModifiers();
}

void Mymenu(int value){
    if(value == 1) if_point = 1^if_point;
    else if(value == 2) if_line  = 1^if_line;
    else if(value == 3){
        if(if_face != 1) if_face = 1;
        else if_face = 0;
    }
    else if(value == 4){
        if(if_face != 2) if_face = 2;
        else if_face = 0;
    }
    else if(value == 5) if_control_move = 0;
    else if(value == 6) if_control_move = 1;
    else if(value == 7) if_control_move = 2;
    else if (value == 8) exit(0);
}

void GLUTInit(int *argc, char **argv)
{
    glutInit(argc, argv);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(GLUTwindow_width, GLUTwindow_height);
    GLUTwindow = glutCreateWindow("实验三_OBJLoading");
    glutReshapeFunc(GLUTResize);
//    glEnable(GL_TEXTURE_2D);    // 启用纹理
    
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
    glutAddMenuEntry("OBJ_Vertice",1);//添加菜单项
    glutAddMenuEntry("OBJ_Line",2);
    glutAddMenuEntry("OBJ_Face",3);
    glutAddMenuEntry("OBJ_Texture",4);
    glutAddMenuEntry("OBJ_Rotation",5);
    glutAddMenuEntry("OBJ_Translation",6);
    glutAddMenuEntry("OBJ_Scaling",7);
    glutAddMenuEntry("Exit",8);
    glutAttachMenu(GLUT_RIGHT_BUTTON);//把当前菜单注册到指定的鼠标键
    
    // 设置光照信息
    static GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
}
