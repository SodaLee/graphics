//
//  main.cpp
//  bumpmap
//
//  Created by 李浩辰 on 2017/5/14.
//  Copyright © 2017年 李浩辰. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <queue>
#include <cmath>
#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>
using namespace std;

const char texture_path[] = "brick.txt", normalmap_path[] = "brick_n.txt";
GLfloat texture[512][512][3], normalmap[512][512][3], light[3];

GLfloat dot(GLfloat a[3], GLfloat b[3]) {
    GLfloat tmp = 0;
    for (int i = 0; i < 3; ++i)
        tmp += a[i] * b[i];
    return tmp;
}

GLfloat leng(GLfloat a[3]) {
    GLfloat tmp = 0;
    for (int i = 0; i < 3; ++i)
        tmp += a[i] * a[i];
    return sqrt(tmp);
}

void bumpmap(int width, int height) {
    GLfloat mat[height][width][3];
    glReadPixels(0, 0, width, height, GL_RGB, GL_FLOAT, mat);
    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            for (int k = 0; k < 3; ++k)
                mat[i][j][k] = texture[i][j][k];
    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j) {
            GLfloat co = 1;
            co = dot(light, normalmap[i][j]) / (leng(light)*leng(normalmap[i][j]));
            for (int k = 0; k < 3; ++k)
                mat[i][j][k] *= co;
        }
    glDrawPixels(width, height, GL_RGB, GL_FLOAT, mat);
}

void Draw() {//材质坐标轴正向→，↓，·
    glClear(GL_COLOR_BUFFER_BIT);
    //光源为相对原点方向
    light[0] = 1; light[1] = -1; light[2] = 1; //右上光源
    //light[0] = 0; light[1] = -1; light[2] = 1; //上光源
    bumpmap(512, 512);
    glFlush();
}

void ReadPicture() {
    ifstream fin(texture_path);
    for (int i = 0; i < 512; ++i)
        for (int j = 0; j < 512; ++j)
            for (int k = 0; k < 3; ++k)
                fin >> texture[511-i][j][k];
    fin.close();
    fin.open(normalmap_path);
    for (int i = 0; i < 512; ++i)
        for (int j = 0; j < 512; ++j)
            for (int k = 0; k < 3; ++k)
                fin >> normalmap[511-i][j][k];
    fin.close();
}

int main(int argc, char * argv[]) {
    ReadPicture();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(512, 512);
    glutCreateWindow("Bumpmap by 1500012746 SodaLee");
    glutDisplayFunc(Draw);
    glutMainLoop();
    return 0;
}
