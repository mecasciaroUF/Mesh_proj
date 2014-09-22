#if !defined(__CV_H)
#define __CV_H

#include "Volumen.h"
/* Clase CV: C�lula Virtual
*/

class CV
        {
	private:

        float x,y,z; //Posici�n
        float dx,dy; //Direcci�n de movimiento
        short int scope; //Largo del camino de visi�n

        float xs,ys; //fin de visi�n.
        float theta; //�ngulo de la direcci�n de movimiento respecto a la horizontal

        CV* nextCV; //puntero al vecino pr�ximo;
        CV* prevCV; //puntero al vecino anterior;

        float viscosidad; //viscosidad del camino
        float inercia; //Inercia de movimiento

        float MaxInercia;//Inercia inicial de la CV

        float* LUT; //puntero a una LUT de velocidad en funci�n del camino
        short int minLUT; //calcula el offset para los indices dentro de LUT

	public:
        //Constructor por defecto
        CV();

        //Contructor por par�metros:
        CV(float xi, float yi, float dxi, float dyi, short int scope, float inercia);

        //Destructor:
        ~CV();

        //Devuelve la posici�n de la CV:
        float getPosX();
        float getPosY();

        //Devuelve Inercia
        float getInercia();

        //Devuelve viscosidad
        float getViscosidad();

        //Devuelve la posici�n del Scope:
        short int getScopeX();
        short int getScopeY();

        //Ingresar Parametros:
        void setInercia(float inercia);

        //Ve el camino para obtener el promedio: 
        void verCamino(Volumen* M, int corte);

        //Liga CV con otra CV, proxima: next=1, anterior: next=0 :
        void ligarCV(CV* otraCV, bool next);

        //Vincula con una LUT creada externamente :
        void ligarLUT(float* LUT, short int minLUT);

        //calcula el movimiento a partir de la direccion y la inercia:
        void moverSimple(int n);

        //calcula el movimiento a partir de la direccion, la inercia, y el
        //movimiento de los vecinos cercanos:
        void moverLigado(int n);
        };
#endif
