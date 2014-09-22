#include <vcl.h>
#include <math.h>
#if !defined(__VECTOR_H)
#define __VECTOR_H

class Vector
{
 protected:
	float Pi;
	float *dato;
	int cantidad;
 public:
	Vector();
	Vector(int n);
	Vector(Vector &v);
	~Vector();

	float& operator []( int n){
	  if( n >= 0 && n < cantidad)
		 return dato[n];
	  else
		 return dato[0];
	 }

	float Dato(int n);
	void Dato(float f,int n);
	void PoneCeros(int com,int fin);

	int LeeDatos(char *Nombre);
	int GuardaDatos(char *Nombre);
	int Cantidad(void);

	// operaciones

	Vector& operator =(const Vector &v);
	Vector operator +(const Vector &v);
	Vector operator -(const Vector &v);
	Vector operator *(const Vector &v);
	Vector operator /(const Vector &v);

	Vector operator +(const float f);
	Vector operator -(const float f);
	Vector operator *(const float f);
	Vector operator /(const float f);

	Vector& operator +=(const Vector &v);
	Vector& operator -=(const Vector &v);
	Vector& operator *=(const Vector &v);
	Vector& operator /=(const Vector &v);

	Vector& operator +=(const float f);
	Vector& operator -=(const float f);
	Vector& operator *=(const float f);
	Vector& operator /=(const float f);

	int operator !(void);
	float Maximo(void);
	float Minimo(void);
	void Copia(Vector v);
        int getCantidad();

   //Operaciones
   void Convolucion(Vector &v0,Vector &v1);
   char DFTs(int opcion, Vector Signal, Vector Signali, Vector& XR, Vector& XI);
   char FFTs(Vector sr, Vector si, Vector& XR, Vector& XI);
   int  IBR(int m, int v);
};
#endif
 