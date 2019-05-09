#include "math.h"
#include "Vector.h"
#include "Arduino.h"
Vector init_vector(float x,float y)
{
    Vector out;
    out.x=x;
    out.y=y;
    return out;
}

VectorE init_vectorE(float x,float y,float theta)
{
    VectorE out;
    out.vec.x=x;
    out.vec.y=y;
    out.theta=theta;
    return out;
}


Vector add(Vector v1,Vector v2)
{
    Vector out;
    out.x=v1.x+v2.x;
    out.y=v1.y+v2.y;
    return out;
}

Vector mult(float lambda,Vector v)
{
    Vector out;
    out.x=lambda*v.x;
    out.y=lambda*v.y;
    return out;
}

float cross(Vector v1,Vector v2)
{
    return v1.x*v2.x + v1.y*v2.y;
}


Vector minus(Vector v1,Vector v2)
{
    Vector out;
    out.x=v1.x-v2.x;
    out.y=v1.y-v2.y;
    return out;
}

Vector minusFAST(Vector* v1,Vector* v2)
{
    Vector out;
    out.x=v1->x-v2->x;
    out.y=v1->y-v2->y;
    return out;
}

float longueur(Vector v)
{
    return sqrt(cross(v,v));
}

float normalize(float theta)
{
    float out;
    out=theta-(2*PI)*((int)(theta/(2*PI)));
    if (out>PI)
        return (out-2*PI);
    else if (out<=-PI)
        return (out+2*PI);
    return out;
}

float angle(Vector v)
{
    return atan2(v.y,v.x);
}

Vector directeur(float theta)
{
    Vector out;
    out.x=cos(theta);
    out.y=sin(theta);
    return out;
}


Vector rotate(Vector v)
{
    Vector out;
    out.x=-1*v.y;
    out.y=v.x;
    return out;
}

Matrix init_matrix(float a,float b,float c,float d)
{
    Matrix out;
    out.a=a;
    out.b=b;
    out.c=c;
    out.d=d;
    return out;
}

float determinant(Matrix M)
{
    return M.a*M.d-M.c*M.b;
}

Matrix inverse(Matrix M)
{
    Matrix out;
    float det=determinant(M);
    out.a=M.d/det;
    out.b=-M.b/det;
    out.c=-M.c/det;
    out.d=M.a/det;
    return out;
}

Vector applique(Matrix M, Vector V)
{
    Vector out;
    out.y=M.a*V.y+M.b*V.x;
    out.x=M.c*V.y+M.d*V.x;
    return out;
}

Vector intersection(float x1,float y1,float theta1,float x2,float y2,float theta2)
{
    Vector M1=init_vector(x1,y1);
    Vector M2=init_vector(x2,y2);
    Matrix A=init_matrix(sin(theta1),sin(theta2),cos(theta1),cos(theta2));
    float t1=-(applique(inverse(A),minus(M1,M2)).y);
    return add(M1,mult(t1,directeur(theta1)));
}
