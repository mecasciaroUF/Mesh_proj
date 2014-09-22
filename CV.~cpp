#include "CV.h"
#include <math.h>

//Constructor por defecto-------------------------------------------------------
CV::CV()
{
    x=0;
    y=0;
    dx=0;
    dy=0;
    scope=0;
    xs=0;
    ys=0;
    theta=0;
    nextCV = NULL;
    prevCV = NULL;
    viscosidad=0;
    inercia=0;
    MaxInercia=0;
    LUT=NULL;
}
//------------------------------------------------------------------------------

//Contructor por parámetros-----------------------------------------------------
CV::CV(float xi, float yi, float dxi, float dyi, short int scope, float inercia)
{
    x=xi;
    y=yi;
    dx=dxi;
    dy=dyi;
    this->scope=scope;
    this->inercia=inercia;
    MaxInercia=inercia;

    //Calculo del ángulo theta según los cuadrantes, tal que theta=[0,2pi]
    //..............................
    if(dx>0&&dy==0)  //t=0º
    theta=0;

    if(dx>0&&dy>0)
    theta=atan(dy/dx);   // 0º<t<90º

    if(dx==0&&dy>0)
    theta=3.141632/2.0; //t=90º

    if(dx<0&&dy>0)
    theta=3.141632-fabs(atan(dy/dx)); //90º<t<180º

    if(dx<0&&dy==0)
    theta=3.141632; //t=180º

    if(dx<0&&dy<0) //180º<t<270º
    theta=3.141632+atan(dy/dx);

    if(dx==0&&dy<0) //t=270º
    theta=3.0*3.141632/2.0;

    if(dx>0&&dy<0) //270º<t<360º
    theta=2.0*3.141632-fabs(atan(dy/dx));
    //..............................

    xs=x+float(scope)*cos(theta);
    ys=y+float(scope)*sin(theta);

    nextCV = NULL;
    prevCV = NULL;

    LUT = NULL;
}
//------------------------------------------------------------------------------

//Destructor--------------------------------------------------------------------
CV::~CV()
{
        nextCV = NULL;
        prevCV = NULL;
        LUT=NULL;
}
//------------------------------------------------------------------------------

//Liga CV con otra CV, proxima: next=1, anterior: next=0------------------------
void CV::ligarCV(CV* otraCV, bool next)
{
        if(next)
        nextCV=otraCV;
        else
        prevCV=otraCV;
}
//------------------------------------------------------------------------------

//Vincula con una LUT creada externamente---------------------------------------
void CV::ligarLUT(float* lut, short int min)
{
        LUT = lut;
        minLUT = min;
}
//------------------------------------------------------------------------------

////Ve el camino para obtener el promedio---------------------------------------
void CV::verCamino(Volumen* M, int corte)
{
  //Ve el camino desde x,y hasta xs,ys para promediarlo       
  //M: imagen sobre la cual se va promediar al camino
  //x,y: posicion actual
  //xs,ys: posición final

  //Declaración de variables auxiliares para el algoritmo de Bressenham2D:
  short int NumPixel;
  short int Decision;
  short int Incrd1, Incrd2;
  short int IncrXold, IncrXnew, IncrYold, IncrYnew;

  short int DistX = abs(xs - x);
  short int DistY = abs(ys - y);

   if(DistX >= DistY)
   {
    NumPixel = DistX + 1;
    Decision = (2 * DistY) - DistX;
    Incrd1 = DistY * 2;
    Incrd2 = (DistY - DistX) * 2;
    IncrXold = 1;
    IncrXnew = 1;
    IncrYold = 0;
    IncrYnew = 1;
   }else
   {
    NumPixel = DistY + 1;
    Decision = (2 * DistX) - DistY;
    Incrd1 = DistX * 2;
    Incrd2 = (DistX - DistY) * 2;
    IncrXold = 0;
    IncrXnew = 1;
    IncrYold = 1;
    IncrYnew = 1;
   }

   if (x > xs )
   {
    IncrXold = -IncrXold;
    IncrXnew = -IncrXnew;
   }

   if (y > ys)
   {
    IncrYold = -IncrYold;
    IncrYnew = -IncrYnew;
   }

    //resetea promCamino:
    int promCamino = 0;

    int xc, yc;
    xc=x;
    yc=y;

    for(short int Bucle = 0; Bucle<NumPixel; Bucle++)
    {
    promCamino = promCamino + M->GetV(xc,yc,corte);

           if(Decision < 0)
           {
            Decision = Decision + Incrd1;
            xc = xc + IncrXold;
            yc = yc + IncrYold;
           }else
           {
            Decision = Decision + Incrd2;
            xc = xc + IncrXnew;
            yc = yc + IncrYnew;
           }
     }

     promCamino = promCamino/NumPixel;

     //Calculo de la viscosidad de la CV:
     viscosidad = this->LUT[promCamino-minLUT];

}
//------------------------------------------------------------------------------

//calcula el movimiento a partir de la direccion y la inercia-------------------
void CV::moverSimple(int n)
{
      if(inercia-n*viscosidad>=0)
      {
      inercia = inercia-n*viscosidad;
      x=x+inercia/MaxInercia*cos(theta);
      y=y+inercia/MaxInercia*sin(theta);

      xs=x+float(scope)*cos(theta);
      ys=y+float(scope)*sin(theta);
      }
}
//------------------------------------------------------------------------------

//Promedio ponderado con las viscosidades de sus vecinos------------------------
void CV::moverLigado(int n)
{
        float viscoProm=0;

     // viscoProm = (viscosidad + nextCV->viscosidad + prevCV->viscosidad)/3.0;

      viscoProm = (viscosidad + nextCV->nextCV->viscosidad + nextCV->viscosidad +
                               prevCV->prevCV->viscosidad + prevCV->viscosidad)/5;

     // if(inercia-float(n)*viscoProm>=0)
      {
      inercia = inercia-float(n)*viscoProm;

    /*
      x=x+(inercia+nextCV->getInercia()+prevCV->getInercia()+
      nextCV->nextCV->getInercia()+prevCV->prevCV->getInercia())/(5*MaxInercia)*cos(theta);

      y=y+(inercia+nextCV->getInercia()+prevCV->getInercia()+
      nextCV->nextCV->getInercia()+prevCV->prevCV->getInercia())/(5*MaxInercia)*sin(theta);
    */

    x = x + (1.0-viscoProm)*cos(theta);

    y = y + (1.0-viscoProm)*sin(theta);

      xs=x+float(scope)*cos(theta);
      ys=y+float(scope)*sin(theta);
      }
}
//------------------------------------------------------------------------------

//Ingreso de parametros---------------------------------------------------------
void CV::setInercia(float inercia)
{
    this->inercia = inercia;
}
//------------------------------------------------------------------------------

//Devuelve la posición de la CV-------------------------------------------------
float CV::getPosX()
{
return x;
}

float CV::getPosY()
{
return y;
}

float CV::getInercia()
{
return inercia;
}

float CV::getViscosidad()
{
return viscosidad;
}

short int CV::getScopeX()
{
return xs;
}

short int CV::getScopeY()
{
return ys;
}

//------------------------------------------------------------------------------



