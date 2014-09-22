//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop

#include "nrutil.h"
#include "nrutil.c"
#include "Unit1.h"
#include "iostream.h"
#define NMAX 5000

#define GET_PSUM \
for (j=0;j<ndim;j++) {\
for (sum=0.0,i=0;i<mpts;i++) sum += p[i][j];\
psum[j]=sum;}

#define SWAP(a,b) {swap=(a);(a)=(b);(b)=swap;}

float funk(float *v, float *x, float *y, float *z, float N)
{
float vx = v[0];
float vy = v[1];
float Px = v[2];
float Py = v[3];
float R = v[4];

float D2=0;

float L=sqrt(vx*vx + vy*vy + 1);

float x00,y00,z00,x000,y000,z000,d2,d_R;
        for(int i=0;i<N;i++)
        {
        x00=vx*vx*(x[i]-Px)+vx*vy*(y[i]-Py)+vx*z[i];
        y00=vx*vy*(x[i]-Px)+vy*vy*(y[i]-Py)+vy*z[i];
        z00=vx*(x[i]-Px)+vy*(y[i]-Py)+z[i];
        x000=x[i]-Px-(1/L)*(1/L)*x00;
        y000=y[i]-Py-(1/L)*(1/L)*y00;
        z000=z[i]-(1/L)*(1/L)*z00;
        d2=sqrt(x000*x000 + y000*y000 + z000*z000);
        d_R=sqrt((d2-R)*(d2-R));
        D2=D2+d_R;
        }
        return D2;
}

float amotry(float **p, float y[], float psum[], int ndim, int ihi, float fac,float *x, float *yy, float *z, float N)
{
        int j;
        float fac1,fac2,ytry,*ptry;
        ptry=vector(0,ndim-1);
        fac1=(1.0-fac)/ndim;
        fac2=fac1-fac;

        for (j=0;j<ndim;j++)
                ptry[j]=psum[j]*fac1-p[ihi][j]*fac2;

                ytry=funk(ptry,x,yy,z,N);

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

void amoeba(float **p, float y[], int ndim, float ftol, int *nfunk, float *x, float *yy, float *z, float N)
{
float amotry(float **p, float y[], float psum[], int ndim, int ihi, float fac, float *x, float *yy, float *z, float  N);
int i,ihi,ilo,inhi,j,mpts=ndim+1;
float rtol,sum,swap,ysave,ytry,*psum;
psum=vector(0,ndim-1);
*nfunk=0;

GET_PSUM

for (;;) {
ilo=1;
ihi = y[1]>y[2] ? (inhi=2,1) : (inhi=1,2);
for (i=0;i<mpts;i++) {
if (y[i] <= y[ilo]) ilo=i;
if (y[i] > y[ihi]) {
inhi=ihi;
ihi=i;
} else if (y[i] > y[inhi] && i != ihi) inhi=i;
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
ytry=amotry(p,y,psum,ndim,ihi,-1.0, x, yy, z, N);
if (ytry <= y[ilo])
ytry=amotry(p,y,psum,ndim,ihi,2.0, x, yy, z, N);
else if (ytry >= y[inhi]) {
ysave=y[ihi];
ytry=amotry(p,y,psum,ndim,ihi,0.5, x, yy, z, N);
if (ytry >= ysave) {
for (i=0;i<mpts;i++) {
if (i != ilo) {
for (j=0;j<ndim;j++)
p[i][j]=psum[j]=0.5*(p[i][j]+p[ilo][j]);
y[i]=funk(psum,x,yy,z,N);
}
}
*nfunk += ndim;

GET_PSUM

}
} else --(*nfunk);
}
free_vector(psum,0,ndim-1);
}

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
PointsListBox->Items->LoadFromFile("DMLM.DAT");

AnsiString linea;
CantPuntos =  PointsListBox->Items->Count;

PX = new float[CantPuntos];
PY = new float[CantPuntos];
PZ = new float[CantPuntos];


  for(int i=0;i<CantPuntos;i++)
                {
                linea = PointsListBox->Items->Strings[i];
                PX[i] = atoi(linea.SubString(1,linea.Pos(",")-1).c_str());
                linea = linea.SubString(linea.Pos(",")+1,linea.Length());
                PY[i] = atoi(linea.SubString(1,linea.Pos(",")-1).c_str());
                linea = linea.SubString(linea.Pos(",")+1,linea.Length());
                PZ[i] = atoi(linea.c_str());
                }     //for

float p0[5];

p0[0]= -0.8095; //vx
p0[1]= -0.2381;  //vy
p0[2]=59.7143;  //Px
p0[3]=117.8571; //Py
p0[4]=2.0000;   //R

/*
p = (1.5 -1 40 100 2)
    (2.5 -1 40 100 2)
    (1.5  0 40 100 2)
    (1.5 -1 80 100 2)
    (1.5 -1 40 200 2)
    (1.5 -1 40 200 4)
*/

float h[6];

h[0]=0;
h[1]=1;
h[2]=1;
h[3]=10;
h[4]=10;
h[5]=6;

float **p;

p=new float*[6];

for(int i=0;i<6;i++)
*(p+i)=new float[5];

for(int i=0;i<6;i++)
{
        for(int j=0;j<5;j++)
                *(*(p+i)+j)=p0[j];
        if(i>0)
            *(*(p+i)+i-1)=*(*(p+i)+i-1)+h[i];
}

float y[6];

int nfunc;

y[0]=funk(p0, PX, PY, PZ, CantPuntos);
y[1]=funk(p[1], PX, PY, PZ, CantPuntos);
y[2]=funk(p[2], PX, PY, PZ, CantPuntos);
y[3]=funk(p[3], PX, PY, PZ, CantPuntos);
y[4]=funk(p[4], PX, PY, PZ, CantPuntos);
y[5]=funk(p[5], PX, PY, PZ, CantPuntos);

amoeba(p, y, 5, 0.0001,&nfunc,PX,PY,PZ,CantPuntos);

for(int i=0;i<6;i++)
        for(int j=0;j<5;j++)
            ListBox1->Items->Add(y[j]);
            
for(int i=0;i<6;i++)
        for(int j=0;j<5;j++)
            ListBox1->Items->Add(p[i][j]);

   delete []p;
}
//---------------------------------------------------------------------------


