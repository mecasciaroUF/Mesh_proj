//******************************************************************************
/*
structpunto.h:
Estructuras auxiliares:
punto, puntoF, vertice, triangulo, lado_red, caja.
*/
//******************************************************************************
struct punto          //Punto de coordenadas enteras(short)
{
short int x,y,z;
short int xR, yR, zR;    //Auxiliares para calcular sus valores
                     //de 'x' y de 'y' luego de una rotaci�n
short int color;
};
typedef struct punto Punto;
//******************************************************************************
struct puntoI          //Punto de coordenadas enteras(int)
{
 int xyz[3];

};
typedef struct puntoI PuntoI;
//******************************************************************************

struct puntoF    //Punto de coordenadas flotantes
{
float x,y,z;
short int xR, yR, zR;  //Auxiliares para calcular sus valores
                   //de 'x' y de 'y' luego de una rotaci�n
short int color;
};
typedef struct puntoF PuntoF;
//******************************************************************************

struct renderpunto
{
Punto p;   //posicion, proyeccion y color
PuntoF n;  //normal
float norma;
};
typedef struct renderpunto RenderPunto;
//******************************************************************************

struct triangulo
{
PuntoI indices;          //Indices de los v�rtices

};
typedef struct triangulo Triangulo;
//******************************************************************************

struct lado_red
{
PuntoF* Vertice;
TList* Triangs;
};
typedef struct lado_red Lado_Red;

//******************************************************************************

struct tri
{
int tri_num;
char vert_num;
};
typedef struct tri Tri;

//******************************************************************************

struct caja
{
punto sp;
punto cp;
int alto,ancho,prof;
};

typedef struct caja Caja;
//******************************************************************************

//******************************************************************************

struct cilindro
{
puntoF pi;
puntoF pf;
float radio;
};

typedef struct cilindro Cilindro;
//******************************************************************************

struct esfera
{
puntoF centro;
float radio;
};

typedef struct esfera Esfera;
//******************************************************************************

struct dynPunto
{
//Punto din�mico para transformada de Hough circular y c�lculo del camino de m�nimo costo:
        short int x,y,z,r,H;
        bool visited;
        dynPunto* previous; //Albergar� el anterior nodo en la cadena �ptima
        int index; //indice lineal en el grafo completo
//Lista de costos a los pr�ximos vecinos:
        TList* Vecinos; //Tendr� asociados los punteros a los dynPuntos vecinos
        TList* Costos; //Lista de costos por moverse hacia los vecinos
};

//-----------------------------------------------------------------------------
