#include <iostream.h>
#include "Volumen.h"

//---------------Contructor por defecto-----------------------------------
Volumen::Volumen()
{
alto, ancho, prof = 20;
CrearV();
}

//---------------Contructor tamaño arbitrario-----------------------------------
Volumen::Volumen(int al, int an, int pr)
{
alto=al; ancho=an; prof=pr;
CrearV();
}
//--------------DESTRUCTOR CLASE Volumen----------------------------------------
Volumen::~Volumen()
{                                                                                                   
        for (int i=0; i<prof; ++i)
           for (int j=0; j<alto; ++j)
                delete[] V[i][j];

        for (int i=0; i<prof; ++i)
                delete[] V[i];

        delete[] V;

        V = NULL;
}
//------------------------------------------------------------------------------
void Volumen::CrearV()
{
     V=new short int** [prof];

        for (int i=0; i<prof; ++i)
        {
        V[i]=new short int* [alto];

                for (int j=0; j<alto; ++j)
                {
                V[i][j]=new short int [ancho];

                for (int k=0; k<ancho; ++k)
                        V[i][j][k]=0;
                }
        }
}
//------------------------------------------------------------------------------
void Volumen::SetV(int x,int y,int z, short int valor)
{
     if(x>-1 && x<alto && y>-1 && y<ancho && z>-1 && z<prof)
     V[z][x][y] = valor;
}
//------------------------------------------------------------------------------
short int Volumen::GetV(int x,int y, int z)
{
        if(x>-1 && x<alto && y>-1 && y<ancho && z>-1 && z<prof)
        {
        short int valor = V[z][x][y];
        return valor;
        }
}
//------------------------------------------------------------------------------

int Volumen::getAlto()
{
        return alto;
}
//------------------------------------------------------------------------------

int Volumen::getAncho()
{
        return ancho;
}
//------------------------------------------------------------------------------

int Volumen::getProf()
{
        return prof;
}
//------------------------------------------------------------------------------
void Volumen::LlenarTodo(short int valor)
{
        for (int i=0; i<prof; ++i)
            for (int j=0; j<alto; ++j)
                   for (int k=0; k<ancho; ++k)
                        V[i][j][k]=valor;
}
//------------------------------------------------------------------------------

