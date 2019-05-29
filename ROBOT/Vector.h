#ifndef VECTOR_INCLUDED
#define VECTOR_INCLUDED

#include "1_CONSTANT.h"

struct Vector                       //Une classe Vecteur toute simple
{
    float x;
    float y;
};

struct Matrix
{
    float a;
    float b;
    float c;
    float d;
};

struct VectorE                      //E pour étendu, contient un angle
{
    Vector vec;
    float theta;
};


VectorE mirror(VectorE v);
Vector const vecNull= {0.0,0.0};    //Vecteur nul
Vector init_vector(float x,float y);     //Nouveau vecteur
VectorE init_vectorE(float x,float y,float theta);
Matrix init_matrix(float a,float b,float c,float d);     //Nouveau vecteur
float determinant(Matrix M);
Matrix inverse(Matrix M);
Vector applique(Matrix M, Vector V);
Vector intersection(float x1,float y1,float theta1,float x2,float y2,float theta2);
Vector add(Vector v1,Vector v2);    //Adition de vecteur
Vector mult(float lambda,Vector v); //Multiplication d'un scalaire et d'un vecteur
float cross(Vector v1,Vector v2);   //Produit scalaire de deux vecteurs
Vector minus(Vector v1,Vector v2);  //Renvoie le vecteur V2V1 ie V1-V2
Vector minusFAST(Vector* v1,Vector* v2);
float longueur(Vector v);           //Renvoie la taille du vecteur
float normalize(float theta);       //Passe theta entre -PI et PI
float angle(Vector v);              //Renvoie l'angle d'un vecteur
Vector directeur(float theta);      //Renvoie un vecteur unitaire d'angle theta
Vector rotate(Vector v);            //Fait tourner le vecteur v de PI/2 dans le sens trigo


#endif
