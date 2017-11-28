//
//  main.cpp
//  Ray_tracing
//
//  Created by 李浩辰 on 2017/5/17.
//  Copyright © 2017年 李浩辰. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>

const GLfloat white[3] = {1,1,1};
const GLfloat black[3] = {0,0,0};
const GLfloat red[3] = {1,0,0};
const GLfloat green[3] = {0,1,0};
const GLfloat blue[3] = {0,0,1};
const GLfloat yellow[3] = {1,1,0};
const float Minweight = 0.1;
const float Wr = 0.8;
const float Wt = 0.3;
const float Wz = 0.6;
const char data[] = "data.txt";

struct Vector {
    float x, y, z, d;
    int suf;
    Vector(float _x = 0, float _y = 0, float _z = 0) {
        x = _x; y = _y; z = _z; suf = -1; d = 0;
    }
    Vector normalize() {
        float tmp = 0;
        tmp = x*x + y*y + z*z;
        tmp = sqrt(tmp);
        x /= tmp; y /= tmp; z /= tmp;
        return *this;
    }
    Vector operator * (const Vector& b) const {
        Vector ret;
        ret.x = z*b.y-y*b.z;
        ret.y = x*b.z-z*b.x;
        ret.z = y*b.x-x*b.y;
        return ret;
    }
    Vector operator * (const float b) const {
        return Vector(x*b,y*b,z*b);
    }
    Vector operator + (const Vector &b) const {
        return Vector(x+b.x,y+b.y,z+b.z);
    }
    Vector operator - (const Vector &b) const {
        return Vector(x-b.x,y-b.y,z-b.z);
    }
    float operator ^ (const Vector &b) const {
        return x*b.x+y*b.y+z*b.z;
    }
    float length() {
        return sqrt(x*x+y*y+z*z);
    }
    void P() {
        printf("%.3f %.3f %.3f\n",x,y,z);
    }
};

int ttSurface = 18;
struct Surface {
    GLubyte color[3];
    Vector p[4], n;
    bool en, transparent;
    float Ks, Kt, light, t;
    int ttpoint;
    void cal_n() {
        Vector x, y;
        x = p[1] - p[0];
        y = p[2] - p[0];
        n = y*x;
        n.normalize();
    }
    void Addpoint(Vector a) {
        p[ttpoint++] = a;
    }
    void Changecolor(const GLfloat _color[3]) {
        color[0] = _color[0];
        color[1] = _color[1];
        color[2] = _color[2];
    }
    bool isin(const Vector &b) {
        Vector tmp[4];
        for (int i = 0; i < 4; ++i)
            tmp[i] = p[i] - b;
        for (int i = 0; i < 4; ++i)
            if (((tmp[i]*tmp[(i+1)%4]).normalize()^n) >= 0) return false;
        return true;
    }
    Surface() {
        color[0] = color[1] = color[2] = 0;
        Ks = 0.2; Kt = 0.2;
        ttpoint = 0; light = 0; t = 0;
        en = true; transparent = false;
    }
}suf[100];

void Initialize() {
    suf[0].Addpoint(Vector(556.0,0.0,559.2));//back wall
    suf[0].Addpoint(Vector(0.0,0.0,559.2));
    suf[0].Addpoint(Vector(0.0,548.8,559.2));
    suf[0].Addpoint(Vector(556.0,548.8,559.2));
    suf[0].Changecolor(yellow);
    suf[0].cal_n(); suf[0].light = 1; suf[0].Ks = 0.1;
    suf[1].Addpoint(Vector(343.0,548.7,227.0));//light
    suf[1].Addpoint(Vector(343.0,548.7,332.0));
    suf[1].Addpoint(Vector(213.0,548.7,332.0));
    suf[1].Addpoint(Vector(213.0,548.7,227.0));
    suf[1].Changecolor(white);
    suf[1].cal_n(); suf[1].light = 2;
    suf[2].Addpoint(Vector(556.0,0.0,0.0));//floor
    suf[2].Addpoint(Vector(0.0,0.0,0.0));
    suf[2].Addpoint(Vector(0.0,0.0,559.2));
    suf[2].Addpoint(Vector(556.0,0.0,559.2));
    suf[2].Changecolor(white);
    suf[2].cal_n(); suf[2].light = 0.4;
    suf[3].Addpoint(Vector(556.0,0.0,0.0));//right wall
    suf[3].Addpoint(Vector(556.0,0.0,559.2));
    suf[3].Addpoint(Vector(556.0,548.8,559.2));
    suf[3].Addpoint(Vector(556.0,548.8,0.0));
    suf[3].Changecolor(red);
    suf[3].cal_n(); suf[3].light = 0.4;
    suf[4].Addpoint(Vector(0.0,0.0,559.2));//left wall
    suf[4].Addpoint(Vector(0.0,0.0,0.0));
    suf[4].Addpoint(Vector(0.0,548.8,0.0));
    suf[4].Addpoint(Vector(0.0,548.8,559.2));
    suf[4].Changecolor(green);
    suf[4].cal_n(); suf[4].light = 0.4;
    suf[5].Addpoint(Vector(556.0,548.8,0.0));//ceiling
    suf[5].Addpoint(Vector(556.0,548.8,559.2));
    suf[5].Addpoint(Vector(0.0,548.8,559.2));
    suf[5].Addpoint(Vector(0.0,548.8,0.0));
    suf[5].Changecolor(white);
    suf[5].cal_n(); suf[5].light = 0.4;
    std::ifstream fin(data);
    for (int i = 6; i <= 17; ++i) {
        float x,y,z;
        for (int j = 0; j < 4; ++j) {
            fin >> x >> y >> z;
            suf[i].Addpoint(Vector(x,y,z));
        }
        suf[i].Changecolor(blue);
        suf[i].cal_n(); suf[i].light = 0.4;
    }
    for (int i = 12; i <= 17; ++i) {
        suf[i].Changecolor(white);
        suf[i].transparent = true;
        suf[i].Ks = 0.1;
        suf[i].t = 0.5;
        suf[i].light = 0.1;
    }
    fin.close();
}

Vector Check(Vector sta, Vector dir, bool isin) {
    float t, mi = 10000;
    Vector ret;
    for (int i = 0; i < ttSurface; ++i) {
        if (!suf[i].en || ((suf[i].n^dir) >= 0 && (!suf[i].transparent || !isin))
            || (isin && (suf[i].n^dir) < 0)) continue;
        t = (suf[i].n^(suf[i].p[0]-sta)) / (suf[i].n^dir);
        if (t < mi && t > 0 && suf[i].isin(sta+dir*t)) {
            mi = t;
            ret = sta + dir*t;
            ret.suf = i; ret.d = t;
        }
    }
    return ret;
}

float f(float d) {
    return std::min(1.0, 1000.0/(d+1.0));
}

void rt(Vector sta, Vector dir, float weight, GLfloat *color, bool isin = false) {
    if (weight < Minweight) {
        color[0] = color[1] = color[2] = 0;
    }
    else {
        GLfloat Ir[3] = {0,0,0}, It[3] = {0,0,0}, tmp[3];
        GLfloat Ilocal[3] = {0,0,0}, Iz[3] = {0,0,0};
        Vector p = Check(sta, dir.normalize(), isin), d;
        if (p.suf == -1) {
            color[0] = color[1] = color[2] = 0;
            return;
        }
        d = dir.normalize();
        Vector ref = d - suf[p.suf].n*(2*(d^suf[p.suf].n)), T;
        rt(p+ref*1e-4, ref, weight*Wr, Ir);
        for (int i = 0; i < 10; ++i) {
            Vector dirc = ref*3 + Vector(rand()/double(RAND_MAX)-0.5,rand()/double(RAND_MAX)-0.5,rand()/double(RAND_MAX)-0.5);
            rt(p+dirc*1e-4, dirc, weight*Wt, tmp);
            It[0] += tmp[0];
            It[1] += tmp[1];
            It[2] += tmp[2];
        }
        for (int i = 0; i < 3; ++i) It[i] /= 10;
        if (suf[p.suf].transparent) {
            Vector n;
            float ratio, cosi, cost2;
            if ((d^suf[p.suf].n) >= 0) {
                n = suf[p.suf].n;
                ratio = 1/suf[p.suf].t;
            }
            else {
                n = suf[p.suf].n*(-1);
                ratio = suf[p.suf].t;
            }
            cosi = d^n;
            cost2 = 1.0 - ratio*ratio*(1.0-cosi*cosi);
            if (cost2 >= 0 && sqrt(cost2) >= ratio*cosi) {
                T = d*ratio + n*(sqrt(cost2)-ratio*cosi);
                rt(p+T*1e-4, T, weight*Wz, Iz, !isin);
            }
            else {
                T = ref;
                rt(p+T*1e-4, T, weight*Wz, Iz, isin);
            }
        }
        for (int i = 0; i < 3; ++i)
            color[i] = Ilocal[i] + f(p.d)*(suf[p.suf].Ks*Ir[i]*fabs(ref.normalize()^suf[p.suf].n) +
                                           suf[p.suf].Kt*It[i] +
                                           (1-suf[p.suf].Ks)*Iz[i] +
                                           suf[p.suf].color[i]*suf[p.suf].light);
    }
}

void Ray_tracing(Vector ld, Vector ru, Vector viewpoint) {
    int width, height;
    width = ru.x - ld.x;
    height = ru.y - ld.y;
    GLfloat mat[width][height][3];
    glReadPixels(0, 0, width, height, GL_RGB, GL_FLOAT, mat);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            float color[3];
            rt(viewpoint, Vector(ld.x+j-viewpoint.x,ld.y+i-viewpoint.y,ld.z-viewpoint.z).normalize(),1,color);
            for (int k = 0; k < 3; ++k)
                mat[i][j][k] = color[k];
        }
        glDrawPixels(width, height, GL_RGB, GL_FLOAT, mat);
        glFlush();
    }
}

void Draw() {
    glClear(GL_COLOR_BUFFER_BIT);
    Initialize();
    Ray_tracing(Vector(0,0,0), Vector(560,560,0), Vector(278,273,-800));
    glFlush();
}

int main(int argc, char * argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(560, 560);
    glutCreateWindow("Ray_tracing by 1500012746 SodaLee");
    glutDisplayFunc(Draw);
    glutMainLoop();
    return 0;
}
