#include <glut.h>
#include <cmath>
#include <iostream>
#include <array>
#include <map>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define PI 3.1415f

void Initialize(); //Инициализация окна
void specialkeys(int key, int x, int y); //Обработчик нажатий на кнопки
void Display(); //Фкнция отрисовки главного окна
void scene(); //Функция изменения сцены
void drawAxes();
void draw_model();
void draw_sphere();


float xrot = 100;
float yrot = 0;
float x = 0;
float y = 0;
float z = 0;
float rotate = 2;
float move = 0.5;
bool isTexturingEnabled = true;
bool isLightEnabled = true;
bool isLightEnabled2 = true;

float Color[]{ 1, 1, 1, 1 };
float Position[]{ 0, 1, 0, 0 };
float diffuse[]{ 1, 1, 1, 1 };
float specular[]{ 1, 1, 1, 1 };
float ambient[]{ 1, 1, 1, 1 };

float Position2[]{ 9, 3, 5, 1 };
float diffuse2[]{ 0.1, 0.5, 0.8, 1.0 };
float specular2[]{ 0.0, 0.0, 0.0, 1.0 };
float ambient2[]{ 0.7, 0.7, 0.7, 1.0 };

enum TextureType {
    Wall=0,
    Wood,
    Bed,
    Carpet,
    Pillow
};

std::array<TextureType, 5> allTextures = {
    Wall,
    Wood,
    Bed,
    Carpet,
    Pillow
};
std::map<TextureType, std::string> allTexturesPaths = {
    {Wall, "wall.png"},
    {Wood, "wood.png"},
    {Bed, "bed.png"},
    {Carpet, "carpet.png"},
    {Pillow, "pillow.png"}
};

float deg2rad(float angle) {
    return angle / 180 * PI;
}

void LoadTexture(int id, const char* filename)
{
    int width, heigth, cnt;
    unsigned char* data = stbi_load(filename, &width, &heigth, &cnt, 0);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, heigth, 0, cnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
}

void InitTexture()
{
    for (auto id : allTextures)
    {
        LoadTexture(id, allTexturesPaths[id].c_str());
    }
}

void keyboardFunc(unsigned char key, int _x, int _y) {
    if (key == 'w' || key == 'W') {
        x -= std::sin(deg2rad(xrot)) * move;
        y += std::sin(deg2rad(yrot)) * move;
        z += std::cos(deg2rad(xrot)) * move;
    }

    if (key == 's' || key == 'S') {
        x += std::sin(deg2rad(xrot)) * move;
        y -= std::sin(deg2rad(yrot)) * move;
        z -= std::cos(deg2rad(xrot)) * move;
    }

    if (key == 't' || key == 'T') {
        isTexturingEnabled = !isTexturingEnabled;
        if (isTexturingEnabled) {
            glEnable(GL_TEXTURE_2D);
        }
        else {
            glDisable(GL_TEXTURE_2D);
        }
    }

    if (key == 'r' || key == 'R') {
        isLightEnabled = !isLightEnabled;
        if (isLightEnabled) {
            glEnable(GL_LIGHT0);
        }
        else {
            glDisable(GL_LIGHT0);
        }
    }
    if (key == 'e' || key == 'E') {
        isLightEnabled2 = !isLightEnabled2;
        if (isLightEnabled2) {
            glEnable(GL_LIGHT1);
        }
        else {
            glDisable(GL_LIGHT1);
        }
    }

    glutPostRedisplay();
}

void InitLight()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_NORMALIZE);
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    // направленный источник света
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Color);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, Position);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    //Точечный
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse2);
    glLightfv(GL_LIGHT1, GL_POSITION, Position2);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular2);
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient2);
}



void Initialize() {
    glClearColor(0.128, 0.218, 0.255, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, 800, 800);
    gluPerspective(60, 1, 0.1, 100);
    glMatrixMode(GL_MODELVIEW);

    InitTexture();
    InitLight();
}


void specialkeys(int key, int x, int y) {
    if (key == GLUT_KEY_UP) {
        yrot -= rotate;
    }
    if (key == GLUT_KEY_DOWN) {
        yrot += rotate;
    }
    if (key == GLUT_KEY_LEFT) {
        xrot -= rotate;
    }
    if (key == GLUT_KEY_RIGHT) {
        xrot += rotate;
    }

    if (yrot < -90) {
        yrot = -90;
    }

    if (yrot > 90) {
        yrot = 90;
    }

    glutPostRedisplay();

}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    std::cout << "E/R - Enable/Disable Light" << std::endl;
    std::cout << "T -  Enable/Disable Texture" << std::endl;
    std::cout << "W/S - Go Forward/Back" << std::endl;
    std::cout << "KEY UP/DOWN/LEFT/Right - Move Camera" << std::endl;
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB); //используем буфер глубины, двойную буферизацию и представление цвета триадой RGB.
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(10, 20);
    glutCreateWindow("Volkov Vadim 91PG - PRISON");
    glutDisplayFunc(Display);
    glutKeyboardFunc(keyboardFunc);
    glutSpecialFunc(specialkeys);
    Initialize();
    glutMainLoop();
    return 0;
}


void drawRect(float dx, float dy, float count) {
    float z = 0;

    glBegin(GL_POLYGON);

    glTexCoord2f(0, 0);
    glNormal3f(-1, -1, 1);
    glVertex3f(0, -dy, z);
    glTexCoord2f(dx * count, 0);
    glNormal3f(1, -1, 1);
    glVertex3f(dx, -dy, z);
    glTexCoord2f(dx * count, dy * count);
    glNormal3f(1, 1, 1);
    glVertex3f(dx, 0, z);
    glTexCoord2f(0, dy * count);
    glNormal3f(-1, 1, 1);
    glVertex3f(0, 0, z);

    glEnd();
    //Отображение нормалей

    /*glBegin(GL_LINES);
    glVertex3f(-1, -1 - dy, 1);
    glVertex3f(0, -dy, z);
    glVertex3f(1 + dx, -1 - dy, 1);
    glVertex3f(dx, -dy, z);
    glVertex3f(1 +dx, 1, 1);
    glVertex3f(dx, 0, z);
    glVertex3f(-1, 1, 1);
    glVertex3f(0, 0, z);
    glEnd();*/
}


void drawScene() {

    glBindTexture(GL_TEXTURE_2D, TextureType::Carpet);
    glPushMatrix();
    glTranslatef(-6, -3, -3);
    glRotatef(270, 1, 0, 0);
    drawRect(15, 8, 0.5);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(6, 9, 3);
    glRotatef(90, 1, 0, 0);
    drawRect(3, 3, 0.5);
    glPopMatrix();
    if (isTexturingEnabled) {
        glEnable(GL_TEXTURE_2D);
    }
    else {
        glDisable(GL_TEXTURE_2D);
    }
    
    glBindTexture(GL_TEXTURE_2D, TextureType::Wall);
    glPushMatrix();
    glTranslatef(9, 3, 5);
    glRotatef(180, 0, 1, 0);
    drawRect(15, 6, 0.1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(9, 3, -3);
    glRotatef(-90, 0, 1, 0);
    drawRect(8, 6, 0.1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-6, 3, -3);
    drawRect(15, 6, 0.1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-6, 3, 5);
    glRotatef(90, 0, 1, 0);
    drawRect(8, 6, 0.1);
    glPopMatrix();

}

void drawLamp()
{
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(8.75, 2.9, 4.9);
    glutSolidSphere(0.1,50,50);
    glPopMatrix();
    if (isTexturingEnabled) {
        glEnable(GL_TEXTURE_2D);
    }
    else {
        glDisable(GL_TEXTURE_2D);
    }
}

void drawBed()
{
    glBindTexture(GL_TEXTURE_2D, TextureType::Bed);
    //ножка кровати 1
    glPushMatrix();
    glTranslatef(3.1, -2, -3);
    glRotatef(180, 0, 1, 0);
    drawRect(0.1, 1, 0.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3, -2, -2.9);
    drawRect(0.1, 1, 0.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3.1, -2, -2.9);
    glRotatef(90, 0, 1, 0);
    drawRect(0.1, 1, 0.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3, -2, -3);
    glRotatef(-90, 0, 1, 0);
    drawRect(0.1, 1, 0.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3, -2, -2.9);
    glRotatef(90, 0, 0, 1);
    glRotatef(90, 0, 1, 0);
    drawRect(0.1, 0.1, 0.5);
    glPopMatrix();

    //ножка кровати 2
    glPushMatrix();
    glTranslatef(8.9, -2, -3);
    glRotatef(180, 0, 1, 0);
    drawRect(0.1, 1, 0.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(8.8, -2, -2.9);
    drawRect(0.1, 1, 0.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(8.9, -2, -2.9);
    glRotatef(90, 0, 1, 0);
    drawRect(0.1, 1, 0.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(8.8, -2, -3);
    glRotatef(-90, 0, 1, 0);
    drawRect(0.1, 1, 0.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(8.8, -2, -2.9);
    glRotatef(90, 0, 0, 1);
    glRotatef(90, 0, 1, 0);
    drawRect(0.1, 0.1, 0.5);
    glPopMatrix();

    //ножка кровати 3
    glPushMatrix();
    glTranslatef(3.1, -2, -1);
    glRotatef(180, 0, 1, 0);
    drawRect(0.1, 1, 0.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3, -2, -0.9);
    drawRect(0.1, 1, 0.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3.1, -2, -0.9);
    glRotatef(90, 0, 1, 0);
    drawRect(0.1, 1, 0.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3, -2, -1);
    glRotatef(-90, 0, 1, 0);
    drawRect(0.1, 1, 0.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3, -2, -0.9);
    glRotatef(90, 0, 0, 1);
    glRotatef(90, 0, 1, 0);
    drawRect(0.1, 0.1, 0.5);
    glPopMatrix();

    //ножка кровати 4
    glPushMatrix();
    glTranslatef(8.9, -2, -1);
    glRotatef(180, 0, 1, 0);
    drawRect(0.1, 1, 0.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(8.8, -2, -0.9);
    drawRect(0.1, 1, 0.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(8.9, -2, -0.9);
    glRotatef(90, 0, 1, 0);
    drawRect(0.1, 1, 0.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(8.8, -2, -1);
    glRotatef(270, 0, 1, 0);
    drawRect(0.1, 1, 0.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(8.8, -2, -0.9);
    glRotatef(90, 0, 0, 1);
    glRotatef(90, 0, 1, 0);
    drawRect(0.1, 0.1, 0.5);
    glPopMatrix();

    //Матрас
    glPushMatrix();
    glTranslatef(8.8, -2.2, -2.9);
    glRotatef(180, 0, 1, 0);
    drawRect(5.8, 0.25, 0.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3, -2.2, -0.9);
    drawRect(5.8, 0.25, 0.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(8.8, -2.2, -0.9);
    glRotatef(90, 0, 1, 0);
    drawRect(1.9, 0.25, 0.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3, -2.2, -2.9);
    glRotatef(-90, 0, 1, 0);
    drawRect(1.9, 0.25, 0.5);
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, TextureType::Pillow);
    glPushMatrix();
    glTranslatef(8.8, -2.2, -2.9);
    glRotatef(-90, 0, 0, 1);
    glRotatef(270, 0, 1, 0);
    drawRect(2, 5.8, 0.5);
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, TextureType::Wood);
    glPushMatrix();
    glTranslatef(8.8, -2.45, -0.9);
    glRotatef(-90, 0, 0, 1);
    glRotatef(90, 0, 1, 0);
    drawRect(2, 5.8, 0.5);
    glPopMatrix();

}

void drawComod()
{
    glBindTexture(GL_TEXTURE_2D, TextureType::Wood);
    //ножка 1
    glPushMatrix();
    glTranslatef(8.1, -2.8, 0);
    glRotatef(180, 0, 1, 0);
    drawRect(0.1, 0.2, 0.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(8, -2.8, 0.1);
    drawRect(0.1, 0.2, 0.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(8.1, -2.8, 0.1);
    glRotatef(90, 0, 1, 0);
    drawRect(0.1, 0.2, 0.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(8, -2.8, 0);
    glRotatef(-90, 0, 1, 0);
    drawRect(0.1, 0.2, 0.5);
    glPopMatrix();

    //ножка 2
    glPushMatrix();
    glTranslatef(8.9, -2.8, 0);
    glRotatef(180, 0, 1, 0);
    drawRect(0.1, 0.2, 0.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(8.8, -2.8, 0.1);
    drawRect(0.1, 0.2, 0.5);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(8.9, -2.8, 0.1);
    glRotatef(90, 0, 1, 0);
    drawRect(0.1, 0.2, 0.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(8.8, -2.8, 0);
    glRotatef(-90, 0, 1, 0);
    drawRect(0.1, 0.2, 0.5);
    glPopMatrix();


    //ножка 3
    glPushMatrix();
    glTranslatef(8.1, -2.8, 1.5);
    glRotatef(180, 0, 1, 0);
    drawRect(0.1, 0.2, 0.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(8, -2.8, 1.6);    
    drawRect(0.1, 0.2, 0.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(8.1, -2.8, 1.6);
    glRotatef(90, 0, 1, 0);
    drawRect(0.1, 0.2, 0.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(8, -2.8, 1.5);
    glRotatef(-90, 0, 1, 0);
    drawRect(0.1, 0.2, 0.5);
    glPopMatrix();

    //ножка 4
    glPushMatrix();
    glTranslatef(8.9, -2.8, 1.5);
    glRotatef(180, 0, 1, 0);
    drawRect(0.1, 0.2, 0.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(8.8, -2.8, 1.6);
    drawRect(0.1, 0.2, 0.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(8.9, -2.8, 1.6);
    glRotatef(90, 0, 1, 0);
    drawRect(0.1, 0.2, 0.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(8.8, -2.8, 1.5);
    glRotatef(-90, 0, 1, 0);
    drawRect(0.1, 0.2, 0.5);
    glPopMatrix();
    glPushMatrix();

    //Сам комод
    glPushMatrix();
    glTranslatef(8.9, -1, 0);
    glRotatef(180, 0, 1, 0);
    drawRect(0.9, 1.8, 0.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(8, -1, 1.6);
    drawRect(0.9, 1.8, 0.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(8.9, -1, 1.6);
    glRotatef(90, 0, 1, 0);
    drawRect(1.6, 1.8, 0.5);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(8, -1, 0);
    glRotatef(270,0,1,0);
    drawRect(1.6, 1.8, 0.5);
    glPopMatrix();

    
    glPushMatrix();
    glTranslatef(8, -1, 1.6);
    glRotatef(-270, 0, 0, 1);
    glRotatef(90, 0, 1, 0);
    drawRect(1.6, 0.9, 0.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(8.9, -2.8, 1.6);
    glRotatef(-90, 0, 0, 1);
    glRotatef(90, 0, 1, 0);
    drawRect(1.6, 0.9, 0.5);
    glPopMatrix();
}

void Display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glRotatef(yrot, 1, 0, 0);
    glRotatef(xrot, 0, 1, 0);

    glTranslatef(x, y, z);

    drawScene();

    drawBed();

    drawComod();

    drawLamp();

    glutSwapBuffers();

}
