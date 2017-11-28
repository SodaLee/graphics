//
//  main.cpp
//  Z-Buffer
//
//  Created by 李浩辰 on 2017/4/20.
//  Copyright © 2017年 李浩辰. All rights reserved.
//

#include <iostream>
#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>
#include <cstring>
#include <cmath>
using namespace std;
GLubyte red[3] = {0xff, 0, 0};
GLubyte green[3] = {0, 0xff, 0};
GLubyte blue[3] = {0, 0, 0xff};
GLubyte black[3] = {0, 0, 0};
GLubyte white[3] = {0xff, 0xff, 0xff};
GLubyte yellow[3] = {0xff, 0xff, 0};
const double eps = 1e-4;
double cross(double x1, double y1, double x2, double y2) {
    return x1 * y2 - x2 * y1;
}

class Matrix {
public:
    double* mat;
    int r, c;

    Matrix(int _r = 1, int _c = 1) {
        r = _r; c = _c;
        mat = new double[r*c];
        for (int i = 0; i < r; ++i)
            for (int j = 0; j < c; ++j)
                mat[i*c+j] = 0;
    }
    Matrix(const Matrix& m) {
        r = m.r; c = m.c;
        mat = new double[r*c];
        for (int i = 0; i < r; ++i)
            for (int j = 0; j < c; ++j)
                (*this)[i][j] = m[i][j];
    }
    ~Matrix() {delete []mat;}
    double* operator [] (int n) {
        return mat + n * c;
    }
    double* operator [] (int n) const {
        return mat + n * c;
    }
    Matrix operator * (const Matrix& m) const {
        Matrix ret(r, m.c);
        if (c != m.r) return ret;
        for (int i = 0; i < ret.r; ++i)
            for (int j = 0; j < ret.c; ++j)
                for (int k = 0; k < c; ++k)
                    ret[i][j] += (*this)[i][k] * m[k][j];
        return ret;
    }
    Matrix& operator = (const Matrix& m) {
        delete []mat;
        r = m.r; c = m.c;
        mat = new double[r*c];
        for (int i = 0; i < r; ++i)
            for (int j = 0; j < c; ++j)
                (*this)[i][j] = m[i][j];
        return *this;
    }
    Matrix operator + (const Matrix& m) const {
        Matrix ret(r, c);
        if (r != m.r || c != m.c) return ret;
        for (int i = 0; i < r; ++i)
            for (int j = 0; j < c; ++j)
                ret[i][j] = (*this)[i][j] + m[i][j];
        return ret;
    }
    Matrix operator - (const Matrix& m) const {
        Matrix ret(r, c);
        if (r != m.r || c != m.c) return ret;
        for (int i = 0; i < r; ++i)
            for (int j = 0; j < c; ++j)
                ret[i][j] = (*this)[i][j] - m[i][j];
        return ret;
    }
    void fill(double val) {
        for (int i = 0; i < r; ++i)
            for (int j = 0; j < c; ++j)
                (*this)[i][j] = val;
    }
    Matrix Inverse() {
        Matrix res(*this), inv(r, c);
        if (r != c) return inv;
        for (int i = 0; i < r; ++i) inv[i][i] = 1;
        int rank = 0;
        for (int i = 0; i < c; ++i)
            for (int j = rank; j < r; ++j)
                if (fabs(res[j][i]) > 1e-4) {
                    double tmp = res[j][i];
                    for (int k = i; k < c; ++k) {
                        res[j][k] /= tmp;
                        inv[j][k] /= tmp;
                        swap(res[j][k], res[rank][k]);
                        swap(inv[j][k], inv[rank][k]);
                    }
                    for (int k = 0; k < r; ++k) {
                        if (k == rank) continue;
                        double scale = res[k][i] / res[rank][i];
                        for (int l = 0; l < c; ++l) {
                            res[k][l] -= scale * res[rank][l];
                            inv[k][l] -= scale * inv[rank][l];
                        }
                    }
                    rank++;
                    break;
                }
        return inv;
    }
};

class Vector :public Matrix {
public:
    Vector(int _r = 1) {
        r = _r; c = 1;
        mat = new double[r];
        for (int i = 0; i < r; ++i)
            mat[i] = 0;
    }
    Vector(const Matrix& m = {1}) {
        r = m.r; c = 1;
        mat = new double[r];
        for (int i = 0; i < r; ++i)
            mat[i] = m[i][0];
    }
    Vector(const initializer_list<double> &m) {
        r = m.size();
        c = 1;
        mat = new double[m.size()];
        int tmp = 0;
        for (auto it = m.begin(); it != m.end(); it++)
            mat[tmp++] = *it;
    }
    double& operator [] (int n) {
        return *(mat + n);
    }
    double operator [] (int n) const {
        return *(mat + n);
    }
    Vector operator * (const Vector& v) const {
        Vector ret(3);
        if (r != v.r) return ret;
        ret[0] = cross(mat[1],mat[2],v[1],v[2]);
        ret[1] =-cross(mat[0],mat[2],v[0],v[2]);
        ret[2] = cross(mat[0],mat[1],v[0],v[1]);
        return ret;
    }
    Vector& operator = (const Vector& v) {
        delete []mat;
        r = v.r; c = 1;
        mat = new double[r];
        for (int i = 0; i < r; ++i)
            mat[i] = v[i];
        return *this;
    }
    Vector& operator = (const Matrix& v) {
        delete []mat;
        r = v.r; c = 1;
        mat = new double[r];
        for (int i = 0; i < r; ++i)
            mat[i] = v[i][0];
        return *this;
    }
    Vector operator - (const Vector& v) const {
        Vector ret(r);
        for (int i = 0; i < r; ++i)
            ret[i] = mat[i] - v[i];
        return ret;
    }
    void normalize() {
        double tt = 0;
        for (int i = 0; i < r; ++i)
            tt += mat[i] * mat[i];
        tt = sqrt(tt);
        if (tt < eps) return;
        for (int i = 0; i < r; ++i)
            mat[i] /= tt;
    }
    void Print() {
        printf("point:%.2f %.2f %.2f\n",mat[0],mat[1],mat[2]);
    }
};

void Print_Matrix(const Matrix &m) {
    for (int i = 0; i < m.r; ++i) {
        for (int j = 0; j < m.c; ++j)
            printf("%.2f ", m[i][j]);
        puts("");
    }
}

class Transformer {
public:
    Matrix B;
    Transformer(const Matrix& A = {1}):B(A) {}
    void transform(const Vector& viewpoint, const Vector& lookat, const Vector& yy) {
        Vector z = viewpoint - lookat, y = yy;
        y.normalize();
        z.normalize();
        Vector x = y * z;
        Matrix t1(4,4), t2(4,4);
        for (int i = 0; i < 3; ++i) {
            t1[i][0] = x[i];
            t1[i][1] = y[i];
            t1[i][2] = z[i];
        }
        t1[3][3] = 1;
        for (int i = 0; i < 4; ++i)
            t2[i][i] = 1;
        for (int i = 0; i < 3; ++i)
            t2[i][3] = -viewpoint[i];
        B = t1.Inverse() * t2;
    }
    Vector a2v(Vector& Xa) {
        Vector a(4), b(4), Xb(3);
        for (int i = 0; i < 3; ++i)
            a[i] = Xa[i];
        a[3] = 1;
        b = B * a;
        for (int i = 0; i < 3; ++i)
            Xb[i] = b[i];
        return Xb;
    }
};

class Surface {
public:
    Vector point[3] = {{0,0,0},{0,0,0},{0,0,0}};
    GLubyte color[3] = {0,0,0};
    Surface() {}
    Surface& operator = (const Surface& s) {
        for (int i = 0; i < 3; ++i) {
            point[i] = s.point[i];
            color[i] = s.color[i];
        }
        return *this;
    }
    Surface(const Vector& p0, const Vector& p1, const Vector& p2, GLubyte _color[3]) {
        point[0] = p0;
        point[1] = p1;
        point[2] = p2;
        for (int i = 0; i < 3; ++i)
            color[i] = _color[i];
    }
    double cal_z(const Vector& p) {
        Vector a = point[1] - point[0], b = point[2] - point[0], c = p - point[0];
        double x = (c*b)[2] / (a*b)[2], y = (c*a)[2] / (b*a)[2];
        if (x < 0 || x > 1 || y < 0 || y > 1 || (x + y) > 1)
            return -1e10;
        return point[0][2] + x * a[2] + y * b[2];
    }
    void Print() {
        for (int i = 0; i < 3; ++i) {
            printf("point %d ",i);
            point[i].Print();
        }
    }
}s[10];

void Z_Buffer(int height, int width, int ttS) {
    Transformer view;
    view.transform({0,-1,-1}, {0,0,0}, {0,1,-1});
    for (int i = 0; i < ttS; ++i)
        for (int j = 0; j < 3; ++j)
            s[i].point[j] = view.a2v(s[i].point[j]);
    GLubyte mat[width][height][3];
    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            for (int k = 0; k < 3; ++k)
                mat[i][j][k] = 0;
    double highest[height][width];
    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            highest[i][j] = -1e8;
    int dx = width / 2, dy = height / 2;
    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            for (int k = 0; k < ttS; ++k) {
                double z = s[k].cal_z({static_cast<double>(i-dy),static_cast<double>(j-dx),0});
                if (z > highest[i][j] - eps) {
                    for (int l = 0; l < 3; ++l)
                        mat[j][i][l] = s[k].color[l];
                    highest[i][j] = z;
                }
            }
    glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, mat);
}

void Draw() {
    glClear(GL_COLOR_BUFFER_BIT);
    s[0] = Surface({100,0,100},{-100,0,100},{0,0,-100},yellow);
    s[1] = Surface({100,0,100},{-100,0,100},{0,100,0},red);
    s[2] = Surface({100,0,100},{0,0,-100},{0,100,0},blue);
    s[3] = Surface({-100,0,100},{0,0,-100},{0,100,0},green);
    Z_Buffer(600, 600, 4);
    glFlush();
}

int main(int argc, char * argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Z-Buffer by 1500012746 SodaLee");
    glutDisplayFunc(Draw);
    glutMainLoop();
    return 0;
}
