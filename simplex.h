#include "Matriz.h"
#include "Vector.h"
#include "nrutil.h"
#include "nrutil.c"

#define NMAX 5000
#define PI 3.14159265

#define GET_PSUM \
for (j=0;j<ndim;j++) {\
for (sum=0.0,i=0;i<mpts;i++) sum += p[i][j];\
psum[j]=sum;}

#define SWAP(a,b) {swap=(a);(a)=(b);(b)=swap;}

#define BUFFER_OFFSET(bytes) ((GLubyte*)NULL+(bytes))

//******************************************************************************
//FunkCIL: función que calcula la suma de las distancias ortogonales para el ajuste cilíndrico:
//En general, funk es la función multivariable a ajustar.
double funkCIL(float *v, float *x, float *y, float *z, float N, float RX, float RY, float RZ)
{
//v: vector de condiciones iniciales. También alberga el resultado final:
float vx = v[0];
float vy = v[1];
float Px = v[2];
float Py = v[3];
float R  = v[4];

//float Rx, Ry, Rz: escala en x, y y z:
//float *x,*y,*z: vectores con las coordenadas (x,y,z) de la nube de puntos X:

double D2=0;

double L=0;
       L=sqrt(vx*vx + vy*vy + 1);

//Ajuste cilíndrico:
double x00,y00,z00,x000,y000,z000,d2,d_R;
        for(int i=2;i<N;i++)
        {
         if(x[i]!=NULL)
         {
         x00=vx*vx*(RX*x[i]-Px)+vx*vy*(RY*y[i]-Py)+vx*RZ*z[i];
         y00=vx*vy*(RX*x[i]-Px)+vy*vy*(RY*y[i]-Py)+vy*RZ*z[i];
         z00=vx*(RX*x[i]-Px)+vy*(RY*y[i]-Py)+RZ*z[i];
         x000=RX*x[i]-Px-(1/L)*(1/L)*x00;
         y000=RY*y[i]-Py-(1/L)*(1/L)*y00;
         z000=RZ*z[i]-(1/L)*(1/L)*z00;
         d2=sqrt(x000*x000 + y000*y000 + z000*z000);
         d_R=(d2-R)*(d2-R);
         D2=D2+d_R;
         }
        }
        D2 = sqrt(D2);
        return D2;
}

//Amoeba y amotry: funciones utilizadas por el método Simplex de Nelder-Mead:
double amotryCIL(float **p, float y[], float psum[], int ndim, int ihi, float fac,float *x,
              float *yy, float *z, float N, float RX, float RY, float RZ)
{
        //p: vector de vertices(lados):
        int j;
        float fac1,fac2,ytry,*ptry;
        ptry=vector(0,ndim-1);
        fac1=(1.0-fac)/ndim;
        fac2=fac1-fac;

        for (j=0;j<ndim;j++)
                ptry[j]=psum[j]*fac1-p[ihi][j]*fac2;

                ytry=funkCIL(ptry,x,yy,z,N, RX, RY, RZ);

        if (ytry < y[ihi])
        {
                y[ihi]=ytry;
                for (j=0;j<ndim;j++)
                {
                        psum[j] += ptry[j]-p[ihi][j];
                        p[ihi][j]=ptry[j];
                 }
        }
free_vector(ptry,0,ndim-1);
return ytry;
}

void amoebaCIL(float **p, float y[], int ndim, float ftol, int *nfunk, float *x, float *yy, float *z, float N, float RX, float RY, float RZ)
{
//float amotry(float **p, float y[], float psum[], int ndim, int ihi, float fac, float *x, float *yy, float *z, float  N, float RX, float RY, float RZ);
int i,ihi,ilo,inhi,j,mpts=ndim+1;
float rtol,sum,swap,ysave,ytry,*psum;
psum=vector(0,ndim-1);
*nfunk=0;

GET_PSUM

for (;;)
{
  ilo=1;
  ihi = y[1]>y[2] ? (inhi=2,1) : (inhi=1,2);
    for (i=0;i<mpts;i++)
    {
      if (y[i] <= y[ilo]) ilo=i;
        if (y[i] > y[ihi])
        {
        inhi=ihi;
        ihi=i;
        }
        else if (y[i] > y[inhi] && i != ihi) inhi=i;
    }

  rtol=2.0*fabs(y[ihi]-y[ilo])/(fabs(y[ihi])+fabs(y[ilo]));

  if (rtol < ftol)
  {
  SWAP(y[1],y[ilo]);

   for (i=0;i<ndim;i++)
   SWAP(y[1],y[ilo]);
   break;
}

if (*nfunk >= NMAX)
return;

//nrerror("NMAX exceeded");

*nfunk += 2;
ytry=amotryCIL(p,y,psum,ndim,ihi,-1.0, x, yy, z, N, RX, RY, RZ);

 if (ytry <= y[ilo])
 ytry=amotryCIL(p,y,psum,ndim,ihi,2.0, x, yy, z, N, RX, RY, RZ);
 else if (ytry >= y[inhi])
 {
 ysave=y[ihi];
 ytry=amotryCIL(p,y,psum,ndim,ihi,0.5, x, yy, z, N, RX, RY, RZ);
  if (ytry >= ysave)
  {
   for (i=0;i<mpts;i++)
   {
   if (i != ilo)
   {
   for (j=0;j<ndim;j++)
   p[i][j]=psum[j]=0.5*(p[i][j]+p[ilo][j]);

   y[i]=funkCIL(psum,x,yy,z,N, RX, RY, RZ);
  }
 }
*nfunk += ndim;

GET_PSUM

 }
}
else --(*nfunk);

}
free_vector(psum,0,ndim-1);
}

//******************************************************************************

//******************************************************************************
//FunkESF: función que calcula la suma de las distancias ortogonales para el ajuste esférico:
//En general, funk es la función multivariable a ajustar.
double funkESF(float *v, float *x, float *y, float *z, float N, float RX, float RY, float RZ)
{
//v: vector de condiciones iniciales. También alberga el resultado final:
float cx = v[0];
float cy = v[1];
float cz = v[2];
float R  = v[3];

//float cx, cy, cz: centro de la esfera
//float *x,*y,*z: vectores con las coordenadas (x,y,z) de la nube de puntos X:

double D2=0;

//Ajuste esférico:
double x00,y00,z00,x000,y000,z000,d2,d_R;
        for(int i=2;i<N;i++)
        {
         if(x[i]!=NULL)
         {
         x00=(RX*x[i]-cx)*(RX*x[i]-cx);
         y00=(RY*y[i]-cy)*(RY*y[i]-cy);
         z00=(RZ*z[i]-cz)*(RZ*z[i]-cz);
         d2=sqrt(x00+ y00 + z00);
         d_R=(R-d2)*(R-d2);

         D2=D2+d_R;
         }
        }
        D2 = sqrt(D2);
        return D2;
}

//Amoeba y amotry: funciones utilizadas por el método Simplex de Nelder-Mead:
double amotryESF(float **p, float y[], float psum[], int ndim, int ihi, float fac,float *x,
              float *yy, float *z, float N, float RX, float RY, float RZ)
{
        //p: vector de vertices(lados):
        int j;
        float fac1,fac2,ytry,*ptry;
        ptry=vector(0,ndim-1);
        fac1=(1.0-fac)/ndim;
        fac2=fac1-fac;

        for (j=0;j<ndim;j++)
                ptry[j]=psum[j]*fac1-p[ihi][j]*fac2;

                ytry=funkESF(ptry,x,yy,z,N, RX, RY, RZ);

        if (ytry < y[ihi])
        {
                y[ihi]=ytry;
                for (j=0;j<ndim;j++)
                {
                        psum[j] += ptry[j]-p[ihi][j];
                        p[ihi][j]=ptry[j];
                 }
        }
free_vector(ptry,0,ndim-1);
return ytry;
}

void amoebaESF(float **p, float y[], int ndim, float ftol, int *nfunk, float *x, float *yy, float *z, float N, float RX, float RY, float RZ)
{
//float amotry(float **p, float y[], float psum[], int ndim, int ihi, float fac, float *x, float *yy, float *z, float  N, float RX, float RY, float RZ);
int i,ihi,ilo,inhi,j,mpts=ndim+1;
float rtol,sum,swap,ysave,ytry,*psum;
psum=vector(0,ndim-1);
*nfunk=0;

GET_PSUM

for (;;)
{
  ilo=1;
  ihi = y[1]>y[2] ? (inhi=2,1) : (inhi=1,2);
    for (i=0;i<mpts;i++)
    {
      if (y[i] <= y[ilo]) ilo=i;
        if (y[i] > y[ihi])
        {
        inhi=ihi;
        ihi=i;
        }
        else if (y[i] > y[inhi] && i != ihi) inhi=i;
    }

  rtol=2.0*fabs(y[ihi]-y[ilo])/(fabs(y[ihi])+fabs(y[ilo]));

  if (rtol < ftol)
  {
  SWAP(y[1],y[ilo]);

   for (i=0;i<ndim;i++)
   SWAP(y[1],y[ilo]);
   break;
}

if (*nfunk >= NMAX)
return;

//nrerror("NMAX exceeded");

*nfunk += 2;
ytry=amotryESF(p,y,psum,ndim,ihi,-1.0, x, yy, z, N, RX, RY, RZ);

 if (ytry <= y[ilo])
 ytry=amotryESF(p,y,psum,ndim,ihi,2.0, x, yy, z, N, RX, RY, RZ);
 else if (ytry >= y[inhi])
 {
 ysave=y[ihi];
 ytry=amotryESF(p,y,psum,ndim,ihi,0.5, x, yy, z, N, RX, RY, RZ);
  if (ytry >= ysave)
  {
   for (i=0;i<mpts;i++)
   {
   if (i != ilo)
   {
   for (j=0;j<ndim;j++)
   p[i][j]=psum[j]=0.5*(p[i][j]+p[ilo][j]);

   y[i]=funkESF(psum,x,yy,z,N, RX, RY, RZ);
  }
 }
*nfunk += ndim;

GET_PSUM

 }
}
else --(*nfunk);

}
free_vector(psum,0,ndim-1);
}

//------------------------------------------------------------------------------

//******************************************************************************
//FunkElipse: función que calcula la cantidad de puntos dentro de una elipse que
//están en el rango de los vasos sanguíneos:

double funkElipse(float *v, float mu, float sigma, float zcorte,  Volumen* M)
{
//v: vector de condiciones iniciales. También alberga el resultado final:
float x0    = v[0]; //centro de la elipse, coordX
float y0    = v[1]; //centro de la elipse, coordY
float theta = v[2]; //ángulo de rotación
float rA    = v[3]; //semieje A
float rB    = v[4]; //semieje B

double F=0; //Funcional:

TList* Elipse = new TList;

sigma = 6*sigma;

strelElipse2D(rA, rB, theta, x0, y0, zcorte, Elipse);

//Cálculo del funcional: Acumulación de las diferencias al cuadrado;

PuntoF* paux;

        for(int i=0; i<Elipse->Count; i++) //para cada elemento de la elipse:
        {
         paux = (PuntoF*)Elipse->Items[i];
         float valor = M->GetV(paux->x, paux->y, paux->z);

         if(abs(valor-mu)>2*sigma  )
         F += 100;
         else F += -exp( -(valor-mu)*(valor-mu)/(2*sigma*sigma) );
        }

        //F /= float(Elipse->Count);
        //F =- float(Elipse->Count);

        for(int i=0; i<Elipse->Count; i++) //para cada elemento de la elipse:
        {
         paux = (PuntoF*)Elipse->Items[i];
         delete paux;
        }

        Elipse->Clear();
        delete Elipse;

        return F;
}

//Amoeba y amotry: funciones utilizadas por el método Simplex de Nelder-Mead:
double amotryElipse(float **p, float y[], float psum[], int ndim, int ihi, float fac,
float mu, float sigma, float zcorte, Volumen* M)
{
        //p: vector de vertices(lados):
        int j;
        float fac1,fac2,ytry,*ptry;
        ptry=vector(0,ndim-1);
        fac1=(1.0-fac)/ndim;
        fac2=fac1-fac;

        for (j=0;j<ndim;j++)
                ptry[j]=psum[j]*fac1-p[ihi][j]*fac2;

                ytry=funkElipse(ptry,mu,sigma,zcorte,M);

        if (ytry < y[ihi])
        {
                y[ihi]=ytry;
                for (j=0;j<ndim;j++)
                {
                        psum[j] += ptry[j]-p[ihi][j];
                        p[ihi][j]=ptry[j];
                 }
        }
free_vector(ptry,0,ndim-1);
return ytry;
}

void amoebaElipse(float **p, float y[], int ndim, float ftol, int *nfunk,
float mu, float sigma, float zcorte, Volumen* M)
{
//float amotry(float **p, float y[], float psum[], int ndim, int ihi, float fac, float *x, float *yy, float *z, float  N, float RX, float RY, float RZ);
int i,ihi,ilo,inhi,j,mpts=ndim+1;

float rtol,sum,swap,ysave,ytry,*psum;

psum=vector(0,ndim-1);

*nfunk=0;

GET_PSUM

for (;;)
{
  ilo=1;
  ihi = y[1]>y[2] ? (inhi=2,1) : (inhi=1,2);
    for (i=0;i<mpts;i++)
    {
      if (y[i] <= y[ilo]) ilo=i;
        if (y[i] > y[ihi])
        {
        inhi=ihi;
        ihi=i;
        }
        else if (y[i] > y[inhi] && i != ihi) inhi=i;
    }

  rtol=2.0*fabs(y[ihi]-y[ilo])/(fabs(y[ihi])+fabs(y[ilo]));

  if (rtol < ftol)
  {
  SWAP(y[1],y[ilo]);

   for (i=0;i<ndim;i++)
   SWAP(y[1],y[ilo]);
   break;
}

if (*nfunk >= NMAX)
return;

//nrerror("NMAX exceeded");

*nfunk += 2;
ytry=amotryElipse(p,y,psum,ndim,ihi,-1.0, mu,sigma,zcorte,M);

 if (ytry <= y[ilo])
 ytry=amotryElipse(p,y,psum,ndim,ihi,2.0, mu,sigma,zcorte,M);
 else if (ytry >= y[inhi])
 {
 ysave=y[ihi];
 ytry=amotryElipse(p,y,psum,ndim,ihi,0.5, mu,sigma,zcorte,M);
  if (ytry >= ysave)
  {
   for (i=0;i<mpts;i++)
   {
   if (i != ilo)
   {
   for (j=0;j<ndim;j++)
   p[i][j]=psum[j]=0.5*(p[i][j]+p[ilo][j]);

   y[i]=funkElipse(psum,mu,sigma,zcorte,M);
  }
 }
*nfunk += ndim;

GET_PSUM

 }
}
else --(*nfunk);

}
free_vector(psum,0,ndim-1);
}

//------------------------------------------------------------------------------

//******************************************************************************
//FunkCirc: función que calcula la cantidad de puntos dentro de una elipse que
//están en el rango de los vasos sanguíneos:

double funkCirc(float *v, float mu, float sigma, float zcorte,  Volumen* M, float xi, float yi,
float xL0,float xLf, float yL0, float yLf)
{
//v: vector de condiciones iniciales. También alberga el resultado final:
float x0    = v[0]; //centro de la circ, coordX
float y0    = v[1]; //centro de la circ, coordY
float r     = v[2]; //radio de la circ

//xL0, xLf, yL0, yLf: límites de una región de interés 

double F=0; //Funcional:

TList* Circulo = new TList;

sigma = 3.0*sigma;

//Miro si la circunferencia cae completamente dentro de la ROI, y en caso contrario...:
if( x0-r < xL0 || x0+r>xLf || y0-r<yL0 || y0+r>yLf )
{
        //...penalizo la elección simulando la maxima diferencia posible (al cuadrado)
        //en todos los puntos dentro de la ROI:
        F = mu*mu*(xLf-xL0)*(yLf-yL0);
        return F;
}
        float valor;

        for(int i=xL0; i<xLf; i++)
                for(int j=yL0; j<yLf; j++)
                {
                valor = M->GetV(i, j, zcorte);
                
                   if((i-x0)*(i-x0) + (j-y0)*(j-y0) <= r*r )
                   {
                   F = F + fabs(valor-mu);

                   }
                   else
                   {
                   F = F + fabs(valor);

                   }

               }

        return F;
}

//Amoeba y amotry: funciones utilizadas por el método Simplex de Nelder-Mead:
double amotryCirc(float **p, float y[], float psum[], int ndim, int ihi, float fac,
float mu, float sigma, float zcorte, Volumen* M, float xi, float yi,
float xL0,float xLf, float yL0, float yLf)
{
        //p: vector de vertices(lados):
        int j;
        float fac1,fac2,ytry,*ptry;
        ptry=vector(0,ndim-1);
        fac1=(1.0-fac)/ndim;
        fac2=fac1-fac;

        for (j=0;j<ndim;j++)
                ptry[j]=psum[j]*fac1-p[ihi][j]*fac2;

                ytry=funkCirc(ptry,mu,sigma,zcorte,M, xi, yi, xL0,xLf,yL0,yLf);

        if (ytry < y[ihi])
        {
                y[ihi]=ytry;
                for (j=0;j<ndim;j++)
                {
                        psum[j] += ptry[j]-p[ihi][j];
                        p[ihi][j]=ptry[j];
                 }
        }
free_vector(ptry,0,ndim-1);
return ytry;
}

void amoebaCirc(float **p, float y[], int ndim, float ftol, int *nfunk,
float mu, float sigma, float zcorte, Volumen* M, float xi, float yi,
float xL0,float xLf, float yL0, float yLf)
{
//float amotry(float **p, float y[], float psum[], int ndim, int ihi, float fac, float *x, float *yy, float *z, float  N, float RX, float RY, float RZ);
int i,ihi,ilo,inhi,j,mpts=ndim+1;

float rtol,sum,swap,ysave,ytry,*psum;

psum=vector(0,ndim-1);

*nfunk=0;

GET_PSUM

for (;;)
{
  ilo=1;
  ihi = y[1]>y[2] ? (inhi=2,1) : (inhi=1,2);
    for (i=0;i<mpts;i++)
    {
      if (y[i] <= y[ilo]) ilo=i;
        if (y[i] > y[ihi])
        {
        inhi=ihi;
        ihi=i;
        }
        else if (y[i] > y[inhi] && i != ihi) inhi=i;
    }

  rtol=2.0*fabs(y[ihi]-y[ilo])/(fabs(y[ihi])+fabs(y[ilo]));

  if (rtol < ftol)
  {
  SWAP(y[1],y[ilo]);

   for (i=0;i<ndim;i++)
   SWAP(y[1],y[ilo]);
   break;
}

if (*nfunk >= NMAX)
return;

//nrerror("NMAX exceeded");

*nfunk += 2;
ytry=amotryCirc(p,y,psum,ndim,ihi,-1.0, mu,sigma,zcorte,M, xi, yi, xL0,xLf,yL0,yLf);

 if (ytry <= y[ilo])
 ytry=amotryCirc(p,y,psum,ndim,ihi,2.0, mu,sigma,zcorte,M, xi, yi, xL0,xLf,yL0,yLf);
 else if (ytry >= y[inhi])
 {
 ysave=y[ihi];
 ytry=amotryCirc(p,y,psum,ndim,ihi,0.5, mu,sigma,zcorte,M, xi, yi, xL0,xLf,yL0,yLf);
  if (ytry >= ysave)
  {
   for (i=0;i<mpts;i++)
   {
   if (i != ilo)
   {
   for (j=0;j<ndim;j++)
   p[i][j]=psum[j]=0.5*(p[i][j]+p[ilo][j]);

   y[i]=funkCirc(psum,mu,sigma,zcorte,M, xi, yi, xL0,xLf,yL0,yLf);
  }
 }
*nfunk += ndim;

GET_PSUM

 }
}
else --(*nfunk);

}
free_vector(psum,0,ndim-1);
}

//------------------------------------------------------------------------------

//******************************************************************************
//FunkCirc: función que calcula la cantidad de puntos dentro de una elipse que
//están en el rango de los vasos sanguíneos:

double funkCircI(float *v, float mu, float sigma, float zcorte,  Volumen* M, float xi, float yi)
{
//v: vector de condiciones iniciales. También alberga el resultado final:
float x0    = v[0]; //centro de la circ, coordX
float y0    = v[1]; //centro de la circ, coordY
float r     = v[2]; //radio de la circ

//xL0, xLf, yL0, yLf: límites de una región de interés 

double F=0; //Funcional:

TList* Circulo = new TList;

sigma = 3.0*sigma;

float n = 1.0; //parámetro para pesar la región de interés
float distancia, D;

D = 50.0;

strelCirc2DContorno(r, x0, y0, zcorte, Circulo);

//Cálculo del funcional: Acumulación de las diferencias al cuadrado;

PuntoF* paux;

        for(int i=0; i<Circulo->Count; i++) //para cada elemento de la elipse:
        {
         paux = (PuntoF*)Circulo->Items[i];
         float valor = M->GetV(paux->x, paux->y, paux->z);

         //if(abs(valor-mu)>sigma)
         //F += 1000.0;    //Estar fuera del intervalo se vuelve (casi) prohibitivo
         //else F += -1.0;//-exp(-(1.0/2.0)*(valor-mu)*(valor-mu)/(sigma*sigma)); //

         //distancia = sqrt((x0-xi)*(x0-xi)+(y0-yi)*(y0-yi));
         //distancia /= D;
         F += (n/sigma)*abs(valor-mu)-n  ;// +  distancia;
        }


        for(int i=0; i<Circulo->Count; i++) //para cada elemento de la elipse:
        {
         paux = (PuntoF*)Circulo->Items[i];
         delete paux;
        }

        Circulo->Clear();
        delete Circulo;


        return F;
}

//Amoeba y amotry: funciones utilizadas por el método Simplex de Nelder-Mead:
double amotryCircI(float **p, float y[], float psum[], int ndim, int ihi, float fac,
float mu, float sigma, float zcorte, Volumen* M, float xi, float yi)
{
        //p: vector de vertices(lados):
        int j;
        float fac1,fac2,ytry,*ptry;
        ptry=vector(0,ndim-1);
        fac1=(1.0-fac)/ndim;
        fac2=fac1-fac;

        for (j=0;j<ndim;j++)
                ptry[j]=psum[j]*fac1-p[ihi][j]*fac2;

                ytry=funkCircI(ptry,mu,sigma,zcorte,M, xi, yi);

        if (ytry < y[ihi])
        {
                y[ihi]=ytry;
                for (j=0;j<ndim;j++)
                {
                        psum[j] += ptry[j]-p[ihi][j];
                        p[ihi][j]=ptry[j];
                 }
        }
free_vector(ptry,0,ndim-1);
return ytry;
}

void amoebaCircI(float **p, float y[], int ndim, float ftol, int *nfunk,
float mu, float sigma, float zcorte, Volumen* M, float xi, float yi)
{
//float amotry(float **p, float y[], float psum[], int ndim, int ihi, float fac, float *x, float *yy, float *z, float  N, float RX, float RY, float RZ);
int i,ihi,ilo,inhi,j,mpts=ndim+1;

float rtol,sum,swap,ysave,ytry,*psum;

psum=vector(0,ndim-1);

*nfunk=0;

GET_PSUM

for (;;)
{
  ilo=1;
  ihi = y[1]>y[2] ? (inhi=2,1) : (inhi=1,2);
    for (i=0;i<mpts;i++)
    {
      if (y[i] <= y[ilo]) ilo=i;
        if (y[i] > y[ihi])
        {
        inhi=ihi;
        ihi=i;
        }
        else if (y[i] > y[inhi] && i != ihi) inhi=i;
    }

  rtol=2.0*fabs(y[ihi]-y[ilo])/(fabs(y[ihi])+fabs(y[ilo]));

  if (rtol < ftol)
  {
  SWAP(y[1],y[ilo]);

   for (i=0;i<ndim;i++)
   SWAP(y[1],y[ilo]);
   break;
}

if (*nfunk >= NMAX)
return;

//nrerror("NMAX exceeded");

*nfunk += 2;
ytry=amotryCircI(p,y,psum,ndim,ihi,-1.0, mu,sigma,zcorte,M, xi, yi);

 if (ytry <= y[ilo])
 ytry=amotryCircI(p,y,psum,ndim,ihi,2.0, mu,sigma,zcorte,M, xi, yi);
 else if (ytry >= y[inhi])
 {
 ysave=y[ihi];
 ytry=amotryCircI(p,y,psum,ndim,ihi,0.5, mu,sigma,zcorte,M, xi, yi);
  if (ytry >= ysave)
  {
   for (i=0;i<mpts;i++)
   {
   if (i != ilo)
   {
   for (j=0;j<ndim;j++)
   p[i][j]=psum[j]=0.5*(p[i][j]+p[ilo][j]);

   y[i]=funkCircI(psum,mu,sigma,zcorte,M, xi, yi);
  }
 }
*nfunk += ndim;

GET_PSUM

 }
}
else --(*nfunk);

}
free_vector(psum,0,ndim-1);
}

//------------------------------------------------------------------------------




