#include <iostream.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <conio.h>
#include "Vector.h"

#define MAX 1e12

Vector::Vector()
{
 cantidad = 16;
 dato = new float[cantidad];
 if (dato==NULL){
	cout<<"No hay memoria para el vector";
  }
 PoneCeros( 0,cantidad);
 Pi = 4.0*atan(1.0);
}

Vector::Vector(int n)
{
 if( n < 8)
	cantidad = 8;
 else
	cantidad = n;
 dato = new float[cantidad];
 if (dato==NULL){
	cout<<"No hay memoria para el vector";
  }
 PoneCeros( 0, cantidad);
 Pi = 4.0*atan(1.0);
}

Vector::Vector( Vector &v)
{
 dato = new float[v.cantidad];
 if( dato == NULL){
	cout << endl;
	cout << "No hay memoria para el vector"<<endl;
  }
 cantidad = v.cantidad;
 for( int j = 0; j < cantidad; j++)
	dato[j] = v.dato[j];
 Pi = 4.0*atan(1.0);
}

Vector::~Vector()
{
 delete dato;
}

float Vector::Dato(int n)
{
 if( n >= 0 && n < cantidad)
	return dato[n];
 else
	return 0.0;
}

void Vector::Dato(float f, int n)
{
 if( n >= 0 && n < cantidad)
	dato[n] = f;
}

int Vector::LeeDatos( char *Nombre)
{
 FILE *archivo;
 int j, Nro, Label;

 archivo = fopen(Nombre,"r");
 if (archivo == NULL)
	return 0;
 else
  {
	Label = fscanf(archivo,"%d",&Nro);
	for (j = 0 ; (j < Nro) && (Label > 0); j++)
	  Label = fscanf(archivo,"%f",dato + j);
	fclose(archivo);
  }
 return 1;
}

int Vector::GuardaDatos( char *Nombre)
{
 FILE *archivo;
 int j;

 archivo = fopen(Nombre,"w");
 if (archivo == NULL)
	return 0;
 else
  {
	fprintf(archivo,"%d\n",cantidad);
	for (j = 0 ; j < cantidad ; j++)
	  fprintf(archivo,"%f\n",*(dato+j));
	fclose(archivo);
  }
 return 1;
}

int Vector::Cantidad(void)
{
 return cantidad;
}

void Vector::PoneCeros( int com, int fin)
{
 if( com > fin)
	return;
 if( com < 0)
	return;
 if( fin > cantidad)
	return;
 for( int j = com; j < fin; j++)
	dato[j] = 0.0;
}

void Vector::Copia( Vector v)
{
 delete dato;
 dato = new float[v.cantidad];
 if( dato == NULL){
  cout << endl;
  cout << "No hay memoria para el vector"<<endl;
 }
 cantidad = v.cantidad;
 for( int j = 0; j < cantidad; j++)
 dato[j] = v.dato[j];
}

Vector& Vector::operator =( const Vector &v)
{
 delete dato;
 dato = new float[v.cantidad];
 if( dato == NULL){
	cout << endl;
	cout << "No hay memoria para el vector"<<endl;
  }
 cantidad = v.cantidad;
 for( int j = 0; j < cantidad; j++)
	dato[j] = v.dato[j];
 return *this;
}

Vector Vector::operator +( const Vector &v)
{
 Vector a( v.cantidad);
 if( cantidad != v.cantidad){
	cout << endl << "Vectores son diferentes !!!!!" << endl;
	return a;
  }
 else{
	for( int j = 0; j < cantidad; j++)
	  a.dato[j] = dato[j] + v.dato[j];
	return a;
  }
}

Vector Vector::operator -( const Vector &v)
{
 Vector a( v.cantidad);
 if( cantidad != v.cantidad){
	cout << endl << "Vectores son diferentes !!!!!" << endl;
	return a;
  }
 else{
	for( int j = 0; j < cantidad; j++)
	  a.dato[j] = dato[j] - v.dato[j];
	return a;
  }
}

Vector Vector::operator *( const Vector &v)
{
 Vector a( v.cantidad);
 if( cantidad != v.cantidad){
	cout << endl << "Vectores son diferentes" << endl;
	return a;
  }
	else{
	  for( int j = 0; j < cantidad; j++)
		 a.dato[j] = dato[j] * v.dato[j];
	  return a;
	 }
}

Vector Vector::operator /( const Vector &v)
{
 Vector a( v.cantidad);
 if( cantidad != v.cantidad){
	cout << endl << "Vectores son diferentes" << endl;
	return a;
  }
 else{
	for( int j = 0; j < cantidad; j++)
	  if( v.dato[j] != 0.0)
		 a.dato[j] = dato[j] / v.dato[j];
	  else
		 a.dato[j] = MAX;
	return a;
  }
}

Vector Vector::operator +( const float f)
{
 for( int j = 0; j < cantidad; j++)
	dato[j] += f;
 return *this;
}

Vector Vector::operator -( const float f)
{
 for( int j = 0; j < cantidad; j++)
	dato[j] -= f;
 return *this;
}

Vector Vector::operator *( const float f)
{
 for( int j = 0; j < cantidad; j++)
	dato[j] *= f;
 return *this;
}

Vector Vector::operator /( const float f)
{
 for( int j = 0; j < cantidad; j++)
	if( f != 0.0)
	  dato[j] /= f;
	else
	  dato[j] = MAX;
 return *this;
}

Vector& Vector::operator +=( const Vector &v)
{
 if( cantidad != v.cantidad){
	cout << endl << "Vectores son diferentes"<<endl;
	return *this;
  }
 else{
	for( int j = 0; j < cantidad; j++)
			dato[j] += v.dato[j];
	return *this;
  }
}

Vector& Vector::operator -=( const Vector &v)
{
 if( cantidad != v.cantidad){
	cout << endl << "Vectores son diferentes"<<endl;
	return *this;
  }
 else{
	for( int j = 0; j < cantidad; j++)
	  dato[j] -= v.dato[j];
	return *this;
  }
}

Vector& Vector::operator *=( const Vector &v)
{
 if( cantidad != v.cantidad){
	cout << endl << "Vectores son diferentes" <<endl;
	return *this;
  }
 else{
	for( int j = 0; j < cantidad; j++)
	  dato[j] *= v.dato[j];
	return *this;
  }
}

Vector& Vector::operator /=( const Vector &v)
{
 if( cantidad != v.cantidad){
	cout << endl << "Vectores son diferentes" << endl;
	return *this;
  }
 else{
	for( int j = 0; j < cantidad; j++)
	  if( v.dato[j] != 0.0)
		 dato[j] /= v.dato[j];
	  else
		 dato[j] = MAX;
  }
 return *this;
}

Vector& Vector::operator +=( const float f)
{
 for( int j = 0; j < cantidad; j++)
	dato[j] += f;
 return *this;
}

Vector& Vector::operator -=( const float f)
{
 for( int j = 0; j < cantidad; j++)
	dato[j] -= f;
 return *this;
}

Vector& Vector::operator *=( const float f)
{
 for( int j = 0; j < cantidad; j++)
	dato[j] *= f;
 return *this;
}

Vector& Vector::operator /=( const float f)
{
 for( int j = 0; j < cantidad; j++)
	if( f != 0.0)
	  dato[j] /= f;
	else
	  dato[j] = MAX;
 return *this;
}

int Vector::operator !( void)
{
 return cantidad;
}

float Vector::Maximo( void)
{
 float max = dato[0];
 for( int j = 1; j < cantidad; j++)
	if( dato[j] > max)
	  max = dato[j];
 return max;
}

float Vector::Minimo( void)
{
 float min = dato[0];
 for( int j = 1; j < cantidad; j++)
	if( dato[j] < min)
	  min = dato[j];
 return min;
}

void Vector::Convolucion(Vector &v0,Vector &v1)
{
 float suma;
 for(int n=0;n<cantidad;n++)
	{
	 suma=0.0;
	 for(int i=0;i<=n;i++)
		 suma+=v0[i]*v1[n-i];
	 dato[n]=suma;
	}
}


int  Vector::IBR(int m, int v)
{
int P,c,i;
P=0;
	for(i=0; i<v; i++)
	{
	c=(m>>i)&1;
	P+=c<<(v-i-1);
	}
return P;
}

char Vector::DFTs(int opcion, Vector Signal, Vector Signali, Vector& XR, Vector& XI)
{
float divisor, signo, sumaR, sumaI;
int N=Signal.Cantidad();

if(opcion == 1)        //Transformada
{divisor = 1;
 signo = 1 ;
}

if(opcion == 0)                        //Antitransformada
{divisor = 1/float(N);
 signo = -1;
}

for(float n=0; n <= N-1; n++)
 { sumaR=sumaI=0.0;
	for(float k=0; k <= N-1; k++)
   {sumaR += divisor*(Signal[k]*cos(2*Pi*n*k/float(N)) + signo*Signali[k]*sin(2*Pi*n*k/float(N)));
    sumaI += divisor*(Signali[k]*cos(2*Pi*n*k/float(N)) - signo*Signal[k]*sin(2*Pi*n*k/float(N)));
   }
   XR[n]=sumaR;
   XI[n]=sumaI;
 }
return 1;
}

char Vector::FFTs(Vector sr, Vector si, Vector& XR, Vector& XI)
{
float divisor;
int N = sr.Cantidad();
int N2=(N/2);
int v = int(log10(N)/log10(2));
int I;

int L = 1;
int m=0;
int p=0;

int k = 0;
float Aux,T1R,T1I;

divisor=1/float(N);

Vector WR(N2);
Vector WI(N2);

for(int i=0;i<N2;i++)
{
WR[i]=cos(2*Pi*float(i)/float(N));
WI[i]=-sin(2*Pi*float(i)/float(N));
  XR[i]= sr[i]*divisor;
  XI[i]= si[i]*divisor;
  XR[i+N2]= sr[i+N2]*divisor;
  XI[i+N2]= si[i+N2]*divisor;
}

while(L<=v)
{
  do
  {
  I = 0;
  m = k >> (v-L);
  p=IBR(m,v);

  	do
  	{
  	T1R=WR[p]*XR[k+N2]-WI[p]*XI[k+N2];
  	T1I=WI[p]*XR[k+N2]+WR[p]*XI[k+N2];
  	XR[k+N2]=XR[k]-T1R;
  	XI[k+N2]=XI[k]-T1I;
  	XR[k]=XR[k]+T1R;
  	XI[k]=XI[k]+T1I;

  	k++;
  	I++;
  	}while(I<N2);
  k=k+N2;
  }while(k<(N-1));
L++;
N2=N2/2;
k=0;
}

for(k=0;k<(N-1);k++)
{
m=IBR(k,v);
	if(m>k)
   	{
       Aux = XR[k];
       XR[k]=XR[m];
       XR[m]=Aux;
       Aux = XI[k];
       XI[k]=XI[m];
       XI[m]=Aux;
      }
}

return 1;
}






 