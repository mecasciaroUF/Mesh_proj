//Procesamiento Digital de Imágenes
//Universidad Favaloro
//Bioing. Sebastián Graf

#include <iostream.h>
#include "Matriz.h"
#include "Vector.h"


//---------------Constructor Clase Matriz (por defecto)-------------------------
Matriz::Matriz()
{
//Bmp = 0;
NumeroFilas = 512;
NumeroColumnas = 512;
Bmp = new unsigned char [long(512)*long(512)];
for( int f = 0; f < NumeroFilas; f++)
        for( int c = 0; c < NumeroColumnas; c++)
                buffer(f,c) = 0;
}

//---------------Contructor tamaño arbitrario-----------------------------------
Matriz::Matriz( int filas, int columnas)
{
NumeroFilas = filas;
NumeroColumnas = columnas;
//Bmp = 0;
Bmp = new unsigned char [long(filas)*long(columnas)];
for( int f = 0; f < NumeroFilas; f++)
        for( int c = 0; c < NumeroColumnas; c++)
                buffer(f,c) = 0;
}
//--------------DESTRUCTOR CLASE MATRIZ-----------------------------------------
Matriz::~Matriz()
{
if(Bmp)delete[] Bmp;
Bmp = NULL;
}
//-----------------CARGAR-------------------------------------------------------
void Matriz::Cargar(TImage *Image1, int filai, int coli)
{
BYTE * LinePtr;

        if(Image1->Picture->Bitmap->PixelFormat== pf24bit)
        {
                for (int f = filai; f < filai+(NumeroFilas-2); f++)
                {
                LinePtr = (BYTE *) Image1->Picture->Bitmap->ScanLine[f];
                        for (int c = coli*3; c <= (coli+NumeroColumnas-1)*3; c=c+3)
                        buffer(f-filai,(c-coli*3)/3) = (LinePtr[c]+LinePtr[c+1]+LinePtr[c+2])/3;
                }
        NColores = 24;
        return;
        }

        if(Image1->Picture->Bitmap->PixelFormat==pf8bit)
        {
                for (int f = filai; f <=(NumeroFilas-1); f++)
                {
                LinePtr = (BYTE *) Image1->Picture->Bitmap->ScanLine[f];
                        for (int c = coli; c <= (coli+NumeroColumnas); c++)
                        buffer(f-filai,c-coli) = LinePtr[c];
                        }
        NColores = 0;
        }
}
//-------------------------GRAFICAR---------------------------------------------
void Matriz::Graficar(TImage *Image1)
{
BYTE *LinePtr;
typedef struct
        {
        TLogPalette lpal;
        TPaletteEntry dummy[256];
        } LogPal;

Image1->Picture->Bitmap->PixelFormat = pf8bit;
Image1->Picture->Bitmap->Width = NumeroColumnas;
Image1->Picture->Bitmap->Height = NumeroFilas;
LogPal SysPal;
SysPal.lpal.palVersion = 0x300;
SysPal.lpal.palNumEntries = 256;

if(NColores==8){
for (int i = 0; i < 256; i++)
        {
        SysPal.lpal.palPalEntry[i].peRed = paleta[i].rgbRed;
        SysPal.lpal.palPalEntry[i].peGreen = paleta[i].rgbGreen;
        SysPal.lpal.palPalEntry[i].peBlue = paleta[i].rgbBlue;
        SysPal.lpal.palPalEntry[i].peFlags = 0;
        }
}

if(NColores==24){
for (int i = 0; i < 256; i++)
        {
        SysPal.lpal.palPalEntry[i].peRed = i;
        SysPal.lpal.palPalEntry[i].peGreen = i;
        SysPal.lpal.palPalEntry[i].peBlue = i;
        SysPal.lpal.palPalEntry[i].peFlags = 0;
        }
}
if(NColores==0){
for (int i = 0; i < 256; i++)
        {
        SysPal.lpal.palPalEntry[i].peRed = i;
        SysPal.lpal.palPalEntry[i].peGreen = i;
        SysPal.lpal.palPalEntry[i].peBlue = i;
        SysPal.lpal.palPalEntry[i].peFlags = 0;
        }
}
Image1->Picture->Bitmap->Palette = CreatePalette(&SysPal.lpal);
// Copia la imagen de vuelta para visualizar
for (int f = 0; f <= (NumeroFilas-1); f++)
        {
        LinePtr = (BYTE *) Image1->Picture->Bitmap->ScanLine[f];
        for (int c = 0; c <= (NumeroColumnas-1); c++)
                LinePtr[c]   = buffer(f,c);
        }
Image1->Refresh();
}
//------------------------------------------------------------------------------
void Matriz::Graficar(TImage *Image1, int rojo, int azul, int verde,
                      bool* CondExtra)
{
BYTE *LinePtr;
typedef struct
        {
        TLogPalette lpal;
        TPaletteEntry dummy[256];
        } LogPal;

Image1->Picture->Bitmap->PixelFormat = pf8bit;
Image1->Picture->Bitmap->Width = NumeroColumnas;
Image1->Picture->Bitmap->Height = NumeroFilas;
LogPal SysPal;
SysPal.lpal.palVersion = 0x300;
SysPal.lpal.palNumEntries = 256;
        
if(NColores==0)
  for (int i = 0; i < 256; i++)
        {
         SysPal.lpal.palPalEntry[i].peRed = i*float(rojo)/255.0;
         SysPal.lpal.palPalEntry[i].peGreen = i*float(verde)/255.0;
         SysPal.lpal.palPalEntry[i].peBlue = i*float(azul)/255.0;
         SysPal.lpal.palPalEntry[i].peFlags = 0;
        }
Image1->Picture->Bitmap->Palette = CreatePalette(&SysPal.lpal);

// Copia la imagen de vuelta para visualizar
for (int f = 0; f <= (NumeroFilas-1); f++)
        {
        LinePtr = (BYTE *) Image1->Picture->Bitmap->ScanLine[f];
        for (int c = 0; c <= (NumeroColumnas-1); c++)
                if(CondExtra[f+c*NumeroColumnas]==true)
                  LinePtr[c] = buffer(f,c);
                   else LinePtr[c] = 0;
        }

Image1->Refresh();
}
//----------------------IGUAL---------------------------------------------------
void Matriz::Igual(Matriz &v)
{
 int f,c;
       for(f = 0; f <NumeroColumnas; f++ )
        for(c = 0; c <NumeroFilas; c++)
                        buffer(f,c)=v.buffer(f,c);

}
//----------------------TRANSFORMAR---------------------------------------------
void Matriz::Transformar()
{
int color;
        for(int f=0; f<NumeroFilas;f++)
                for(int c=0;c<NumeroColumnas;c++)
                        {color=buffer(f,c);
                         buffer(f,c)=LUT[color];
                        }
}
//---------------------NEGATIVO-------------------------------------------------
void Matriz::Negativo()
{
  for(int i=0;i<256;i++)
  LUT[i]=255-i;
  Transformar();
}
//---------------------BINARIZACION---------------------------------------------
void Matriz::Binarizacion(int C)
{
    for(int i=0;i<C;i++)
    LUT[i]=0;
    for(int i=C;i<256;i++)
    LUT[i]=255;
    Transformar();
}
//----------------------------FILTROLAPLACIANO----------------------------------
void Matriz::FiltroLaplaciano()
{
  float coefs[3][3]={{0,-1,0},
                     {-1,4,-1},
                     {0,-1,0}};
  for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
        coefs[i][j]=coefs[i][j];

  SetMascara(coefs);
  AplicarMascara(0);
}
//----------------------APLICARMASCARA------------------------------------------
void Matriz::AplicarMascara(float umbral)
{
 float acum=0;
 int f,c;
 float* bufferp;
 float* bufferi;

 bufferp = new float[NumeroColumnas-2];//Buffers del filtro: fila par e impar
 bufferi = new float[NumeroColumnas-2];

 int index=0; //Indice para manejar posicion del array fil

 for(f=1; f <NumeroFilas-1; f++)
 {
 index=0;
  if(f>=3)
        {
        if(f%2==0)
           {
           for(int a=0;a<(NumeroColumnas-2);a++)
           buffer(f-2,a+1)=bufferp[a];
           }else{for(int a=0;a<(NumeroColumnas-2);a++)
                 buffer(f-2,a+1)=bufferi[a];}
        }
  for(c=1; c <NumeroColumnas-1; c++)
                {
                acum = mascara[0][0]*buffer(f-1,c-1)+
                         mascara[0][1]*buffer(f-1,c)  +
                         mascara[0][2]*buffer(f-1,c+1)+
                         mascara[1][0]*buffer(f,c-1)  +
                         mascara[1][1]*buffer(f,c)    +
                         mascara[1][2]*buffer(f,c+1)  +
                         mascara[2][0]*buffer(f+1,c-1)+
                         mascara[2][1]*buffer(f+1,c)  +
                         mascara[2][2]*buffer(f+1,c+1);
                 if (acum>255)
                 acum = 255;

                 if (acum<0)
                 acum = 0;

                 if(f%2==0)
                 {bufferp[index]=acum;}
                 else {bufferi[index]=acum;}

                 index++;
   }
  }
  delete bufferp;
  delete bufferi;
 }
//----------------------APLICARDOBLEMASCARA-------------------------------------
void Matriz::AplicarDobleMascara(float coefsH[3][3], float coefsV[3][3])
{
  float acum=0;
  int f,c;

  float* bufferp1;
  float* bufferi1;
  float* bufferp2;
  float* bufferi2;

 bufferp1 = new float[NumeroColumnas-2];//Buffers del filtro: fila par e impar
 bufferi1 = new float[NumeroColumnas-2];
 bufferp2 = new float[NumeroColumnas-2];//Buffers del filtro: fila par e impar
 bufferi2 = new float[NumeroColumnas-2];

 int index=0;

 for(f=1; f <NumeroFilas-1; f++)
 {
 index=0;
  if(f>=3)
        {
        if(f%2==0)
           {
           for(int a=0;a<(NumeroColumnas-2);a++)
           buffer(f-2,a+1)=sqrt(pow(bufferp1[a],2)+pow(bufferp2[a],2));
           }else{for(int a=0;a<(NumeroColumnas-2);a++)
                 buffer(f-2,a+1)=sqrt(pow(bufferi1[a],2)+pow(bufferi2[a],2));}
         }
  for(c=1; c <NumeroColumnas-1; c++)
                 {acum = coefsH[0][0]*buffer(f-1,c-1)+
                         coefsH[0][1]*buffer(f-1,c)  +
                         coefsH[0][2]*buffer(f-1,c+1)+
                         coefsH[1][0]*buffer(f,c-1)  +
                         coefsH[1][1]*buffer(f,c)    +
                         coefsH[1][2]*buffer(f,c+1)  +
                         coefsH[2][0]*buffer(f+1,c-1)+
                         coefsH[2][1]*buffer(f+1,c)  +
                         coefsH[2][2]*buffer(f+1,c+1);

                 if (acum>255)
                 acum = 255;

                 if (acum<0)
                 acum = 0;

                 if(f%2==0)
                 {bufferp1[index]=acum;}
                 else {bufferi1[index]=acum;}

                   acum= coefsV[0][0]*buffer(f-1,c-1)+
                         coefsV[0][1]*buffer(f-1,c)  +
                         coefsV[0][2]*buffer(f-1,c+1)+
                         coefsV[1][0]*buffer(f,c-1)  +
                         coefsV[1][1]*buffer(f,c)    +
                         coefsV[1][2]*buffer(f,c+1)  +
                         coefsV[2][0]*buffer(f+1,c-1)+
                         coefsV[2][1]*buffer(f+1,c)  +
                         coefsV[2][2]*buffer(f+1,c+1);

                   if (acum>255)
                 acum = 255;

                 if (acum<0)
                 acum = 0;

                 if(f%2==0)
                 {bufferp2[index]=acum;}
                 else {bufferi2[index]=acum;}

                 index++;
                        }

   }
 delete bufferp1;
 delete bufferi1;
 delete bufferp2;
 delete bufferi2;
}
//---------------------FILTROMEDIANA--------------------------------------------
void Matriz::FiltroMediana()
{
  float acum=0;
  int f,c;

 float* bufferp;
 float* bufferi;

 bufferp = new float[NumeroColumnas-2];//Buffers del filtro: fila par e impar
 bufferi = new float[NumeroColumnas-2];

 float coefs[9];
 int index=0;

 for(f=1; f <NumeroFilas-1; f++)
 {
 index=0;
  if(f>=3)
        {
        if(f%2==0)
           {
           for(int a=0;a<(NumeroColumnas-2);a++)
           buffer(f-2,a+1)=bufferp[a];
           }else{for(int a=0;a<(NumeroColumnas-2);a++)
                 buffer(f-2,a+1)=bufferi[a];}
         }
  for(c=1; c <NumeroColumnas-1; c++)
                 {
                 coefs[0]= buffer(f-1,c-1);
                 coefs[1]= buffer(f-1,c);
                 coefs[2]= buffer(f-1,c+1);
                 coefs[3]= buffer(f,c-1);
                 coefs[4]= buffer(f,c);
                 coefs[5]= buffer(f,c+1);
                 coefs[6]= buffer(f+1,c-1);
                 coefs[7]= buffer(f+1,c);
                 coefs[8]= buffer(f+1,c+1);

                 OrdenarMascara(coefs);
                 acum=mascara[1][1];

                 if (acum>255)
                 acum = 255;

                 if (acum<0)
                 acum = 0;

                 if(f%2==0)
                 {bufferp[index]=acum;}
                 else {bufferi[index]=acum;}

                 index++;
                        }
   }
   delete bufferp;
   delete bufferi;
}
//--------------------FILTOGAUSSIANO--------------------------------------------
void Matriz::FiltroGaussiano()
{
  float coefs[3][3]={{0.1077,0.1128,0.1077},
                     {0.1128,0.1181,0.1128},
                     {0.1077,0.1128,0.1077}};
 SetMascara(coefs);
 AplicarMascara(0);
}

//-------------------BARRASH----------------------------------------------------
void Matriz::BarrasH(int ancho)
{
NColores=0;
for(int f = 0; f <NumeroFilas; f++ )
        for(int c = 0; c <NumeroColumnas; c++ )
                buffer(f,c) = (f%ancho)<(ancho/2)?0:255;
}
//-------------------BARRASV----------------------------------------------------
void Matriz::BarrasV(int ancho)
{
        NColores=0;
        for(int c = 0; c <NumeroColumnas; c++ )
                for(int f = 0; f <NumeroFilas; f++ )
                buffer(f,c) = (c%ancho)<(ancho/2)?0:255;
}
//------------------FILL(RELLENA CANVAS DE UN COLOR)----------------------------
void Matriz::Fill(int color)
{
        NColores=0;
        for(int c = 0; c < NumeroColumnas; c++ )
                for(int f = 0; f < NumeroFilas; f++)
                buffer(f,c) = color;
}

//------------------PULSO-------------------------------------------------------
void Matriz::Pulso(int lado)
{
        NColores=0;
        Fill(0);
        for(int c = 0; c < lado; c++ )
                for(int f = 0; f < lado; f++ )
                buffer(f,c) = 255;
}

//------------------SENOIDAL----------------------------------------------------
void Matriz::Senoidal(int freq)
{
        NColores=0;
        Fill(0);
        for(int c = 0; c < NumeroColumnas; c++ )
                for(int f = 0; f < NumeroFilas; f++ )
                buffer(f,c) = 128+128*sin(2*3.1416*freq*c/NumeroFilas);
}

//------------------CIRCUNF-----------------------------------------------------
void Matriz::Circunf(int xo, int yo, int radio)
{
        NColores=0;
        Fill(0);
        int radio2=radio*radio;
        for(int c = 0; c <NumeroColumnas; c++ )
              for(int f = 0; f <NumeroFilas; f++ )
                        if(radio2 > ((c-xo)*(c-xo)+(f-yo)*(f-yo)))
                        buffer(f,c)=255;
}

//-----------------GRADIENTE----------------------------------------------------
void Matriz::GradienteAA()
{
  NColores=0;
  int f,c;
        for(c = 0; c <NumeroColumnas; c++ )
              for(f = 0; f <NumeroFilas; f++ )
                        buffer(f,c)=f;
}

//-------------------GRADIENTEID(izquierda a derecha)---------------------------
void Matriz::GradienteID()
{
  NColores=0;
  int f,c;
        for(c = 0; c <NumeroColumnas; c++ )
              for(f = 0; f <NumeroFilas; f++ )
                        buffer(f,c)=(unsigned char)c;
}

//-------------------ILUMINACION------------------------------------------------
void Matriz::Iluminacion()
{
  NColores=0;
  int f,c;
  for(c = 0; c <NumeroColumnas; c++ )
        for(f = 0; f <NumeroFilas; f++ )
                        buffer(f,c)=(buffer(f,c)/255);

        for(c = 0; c <NumeroColumnas; c++ )
              for(f = 0; f <NumeroFilas; f++ )
                       buffer(f,c)=buffer(f,c)*f;
}

//--------------------SIMCONT(contraste simultáneo)-----------------------------
void Matriz::SimCont(int CE, int CI)
{
NColores=0;
Fill(CE);
int f,c;
           for(c = 255/3; c < 255/3*2; c++ )
                for(f = 255/3; f < 255/3*2; f++ )
                buffer(f,c)=CI;
}
//-------------------REDUCCION--------------------------------------------------
void Matriz::Reduccion(int bits)
{
  NColores=0;
  int f,c;
  int N = 8-bits;
  int N2 = 2;
  for (int i=1;i<N;i++)
  N2 = N2*2;           // N2 = 2^N

  for(c = 0; c <NumeroColumnas; c++ )
        for(f = 0; f <NumeroFilas; f++)
                        buffer(f,c)=(buffer(f,c)>>bits)*(256-1)/(N2 - 1);
}
//-------------------LEERBMP----------------------------------------------------
void Matriz::LeerBMP(char *nombre)
{
    FILE *stream;
    stream = fopen(nombre, "rb");
    fread(&bmpfile,sizeof(BITMAPFILEHEADER),1,stream);
    if(bmpfile.bfType!=19778)       //19778 = "BM"
        return;

    fread(&bmpinfo, sizeof(BITMAPINFOHEADER),1,stream);

        if(bmpinfo.biBitCount==8)
        {       NColores = 8;
                fread(paleta,sizeof(RGBQUAD),256,stream);

                for(int f=NumeroFilas-1;f>=0;f--)
                        for(int c=0; c<NumeroColumnas;c++)
                                buffer(f,c)=fgetc(stream);
        }
        if(bmpinfo.biBitCount==24)
        {       NColores = 24;
                int aux1;
                int aux2;
                for(int f=NumeroFilas-1;f>=0;f--)
                        for(int c=0; c<NumeroColumnas;c++)
                                {aux1=fgetc(stream);
                                 aux2=fgetc(stream);
                                 buffer(f,c)=(fgetc(stream)+aux1+aux2)/3; //ROJO
                                 }
        }
}
//---------------------BRILLOCONTRASTE------------------------------------------
void Matriz::BrilloContraste(int b, int c)
{    for(int i=0;i<256;i++)
        {LUT[i]=i*c + b;

        if(LUT[i]>255)
        LUT[i]=255;

        if(LUT[i]<0)
        LUT[i]=0;
        }
}
//----------------------SETLUT--------------------------------------------------
void Matriz::SETLUT(int r1, int s1, int r2, int s2, int si, int sf)
{
  if (r1==0)
  r1=1;

  if (r2==255)
  r2=254;

  for(int i=0;i<r1;i++)
  {
  LUT[i]=(ceil((s1-si)/(r1)))*i;
  }

for(int i=r1;i<r2;i++)
 LUT[i]=(ceil((s2-s1)/(r2-r1)))*(i-r1)+s1;

for(int i=r2;i<256;i++)
  LUT[i]=(ceil((sf-s2)/(255-r2)))*(i-r2)+s2;
}

//-----------------RANGODINAMICO------------------------------------------------
void Matriz::RangoDinamico(int c)
{
for(int i=0;i<256;i++)
        {LUT[i]=c*log10(1+i);

        if(LUT[i]>255)
        LUT[i]=255;

        if(LUT[i]<0)
        LUT[i]=0;
        }
}
//-----------------CALCHISTO----------------------------------------------------
void Matriz::CalcHisto()
{
for (int i=0; i<256;i ++)
histo[i]=0;

MAXpx=MINpx=TOTALpx=0;
MAXcl=MINcl=MEDcl=0;

TOTALpx=(NumeroFilas*NumeroColumnas);

int f,c;
  for(c = 0; c <NumeroColumnas; c++ )
        for(f = 0; f <NumeroFilas; f++ )
                       histo[buffer(f,c)]++;


for(int i=0;i<256;i++){

if(histo[i]>MAXpx)
MAXpx = histo[i];

if(histo[i]<MINpx)
MINpx = histo[i];
}

int i=0;
while(histo[i]==0)
{
acumulada[i]=0;
i++;
}
MINcl=i;

acumulada[i]=histo[i];

while(acumulada[i]<(TOTALpx/2))
{
acumulada[i+1]=histo[i+1] + acumulada[i];
i++;
}
MEDcl=i;

while(acumulada[i]<TOTALpx)
{
acumulada[i+1]=histo[i+1] + acumulada[i];
i++;
}
MAXcl=i;

for(i;i<256;i++)
acumulada[i]=TOTALpx;
}
//-----------------------GRAFHISTO----------------------------------------------
void Matriz::GrafHisto(TImage *Imagen)
{
Imagen->Canvas->Brush->Color=clWhite;
Imagen->Canvas->FillRect(TRect(0,0,256,256));
Imagen->Canvas->Pen->Color=clBlack;

        for(int i=0;i<256;i++)
        {
        Imagen->Canvas->MoveTo(i,255);
        Imagen->Canvas->LineTo(i,255-(histo[i]*(Imagen->Height)/MAXpx));
        }
}
//-------------------------GRAFACUM---------------------------------------------
void Matriz::GrafAcum(TImage *Imagen)
{
Imagen->Canvas->Brush->Color=clWhite;
Imagen->Canvas->FillRect(TRect(0,0,256,256));
Imagen->Canvas->Pen->Color=clBlack;

        for(int i=0;i<256;i++)
        {
        Imagen->Canvas->MoveTo(i,255);
        Imagen->Canvas->LineTo(i,255-(acumulada[i]/(256*256)*(Imagen->Height)));
        }
}
//----------------------ECUALIZAR-----------------------------------------------
void Matriz::Ecualizar()
{
  for(int i=0;i<256;i++)
  LUT[i]=(255*acumulada[i])/TOTALpx;
}
//----------------------GETMAXCL------------------------------------------------
int Matriz::GetMAXcl()
{
return MAXcl;
}
//----------------------GETMINCL------------------------------------------------
int Matriz::GetMINcl()
{
return MINcl;
}
//----------------------GETMEDCL------------------------------------------------
int Matriz::GetMEDcl()
{
return MEDcl;
}
//----------------------RUIDOSP-------------------------------------------------
void Matriz::RuidoSP(int NR, int SP)
{
randomize();

int f,c;
  for(c = 0; c <NumeroColumnas; c++ )
        for(f = 0; f <NumeroFilas; f++ )
                if(NR > random(100))
                {
                    if (SP > random(100))
                    {buffer(f,c)=255;
                    }else{buffer(f,c)=0;}
               }
}
//---------------------SETMASCARA-----------------------------------------------
void Matriz::SetMascara(float coefs[3][3])
{
for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
                mascara[i][j]=coefs[i][j];
}
//---------------------ORDENARMASCARA-------------------------------------------
void Matriz::OrdenarMascara(float coefs[9])
{
float aux;

for(int i=0;i<9;i++)
   for(int j=1;j<9;j++)
        {if(coefs[j]<coefs[j-1])
             {aux=coefs[j-1];
              coefs[j-1]=coefs[j];
              coefs[j]=aux;
             }
        }
mascara[1][1]=coefs[4];
}
//----------------------PASABAJOSESPACIAL---------------------------------------
void Matriz::PasaBajosEspacial()
{
float coefs[3][3];

        for(int i=0;i<3;i++)
                for(int j=0;j<3;j++)
                {coefs[i][j]=1;
                 coefs[i][j]=coefs[i][j]/9;
                }

                SetMascara(coefs);
                AplicarMascara(0);
}
//---------------------PASAALTOSESPACIAL----------------------------------------
void Matriz::PasaAltosEspacial()
{
float coefs[3][3];

for(int i=0;i<3;i++)
for(int j=0;j<3;j++)
{coefs[i][j]=-1;
 coefs[i][j]=coefs[i][j]/9;
}

coefs[1][1]=8;
coefs[1][1]=coefs[1][1]/9;

SetMascara(coefs);
AplicarMascara(0);
}
//---------------------HIGHBOOST------------------------------------------------
void Matriz::HighBoost(float A)
{
float coefs[3][3];

for(int i=0;i<3;i++)
for(int j=0;j<3;j++)
{coefs[i][j]=-1;
 coefs[i][j]=coefs[i][j]/9;
}

coefs[1][1]=ceil(9.0*A-1.0);
coefs[1][1]=ceil(coefs[1][1]/9.0);

SetMascara(coefs);
AplicarMascara(0);
}
//--------------------FILTROSOBEL-----------------------------------------------
void Matriz::FiltroSobel()
{
  float coefsH[3][3]={{-1,-2,-1},
                     {0,0,0},
                     {1,2,1}};

  float coefsV[3][3]={{-1,0,1},
                     {-2,0,2},
                     {-1,0,1}};

  for(int i=0;i<3;i++)
  for(int j=0;j<3;j++)
  {coefsH[i][j]=coefsH[i][j];
  coefsV[i][j]=coefsV[i][j];}

  AplicarDobleMascara(coefsH, coefsV);
}
//-------------------------FILTROPREWITT----------------------------------------
void Matriz::FiltroPrewitt()
{
  float coefsH[3][3]={{-1,-1,-1},
                     {0,0,0},
                     {1,1,1}};

  float coefsV[3][3]={{-1,0,1},
                     {-1,0,1},
                     {-1,0,1}};

  for(int i=0;i<3;i++)
  for(int j=0;j<3;j++)
  {coefsH[i][j]=coefsH[i][j];
   coefsV[i][j]=coefsV[i][j];}

   AplicarDobleMascara(coefsH, coefsV);
}
//-------------------------FILTROROBERTS----------------------------------------
void Matriz::FiltroRoberts()
{
  float coefsH[3][3]={{1,0,0},
                     {0,-1,0},
                     {0,0,0}};

  float coefsV[3][3]={{0,1,0},
                     {-1,0,0},
                     {0,0,0}};

   AplicarDobleMascara(coefsH, coefsV);
}
//----------------------------ANDLOGICO-----------------------------------------
void Matriz::AndLogico(Matriz &Imagen)
{
  int f,c;
  for(c=0; c <NumeroColumnas; c++)
        for(f=0; f <NumeroFilas; f++)
                 {
                 if(Imagen.buffer(f,c)==0)
                 buffer(f,c)=0;
                 }
}
//----------------------------ORLOGICO------------------------------------------
void Matriz::OrLogico(Matriz &Imagen)
{
  int f,c;
  for(c=0; c <NumeroColumnas; c++)
        for(f=0; f <NumeroFilas; f++)
                 {
                 if(Imagen.buffer(f,c)==255)
                 buffer(f,c)=255;
                 }
}
//----------------------------XORLOGICO-----------------------------------------
void Matriz::XorLogico(Matriz &Imagen)
{
  int f,c;
  for(c=0; c <NumeroColumnas; c++)
        for(f=0; f <NumeroFilas; f++)
                 {
                 if(Imagen.buffer(f,c)==0)
                 buffer(f,c)=255-buffer(f,c);
                 }
}
//----------------------------SUMA----------------------------------------------
void Matriz::Suma(Matriz &Imagen)
{
  int f,c;
  int aux;
  for(c=0; c <NumeroColumnas; c++)
        for(f=0; f <NumeroFilas; f++)
                 {
                 aux=buffer(f,c)+ Imagen.buffer(f,c);
                 if(aux>255)
                 aux=255;

                 if(aux<0)
                 aux=0;

                  buffer(f,c)=aux;
                 }
}
//----------------------------RESTA---------------------------------------------
void Matriz::Resta(Matriz &Imagen)
{

  int f,c;
  int aux;
  for(c=0; c <NumeroColumnas; c++)
        for(f=0; f <NumeroFilas; f++)
                 {
                 aux=buffer(f,c)-Imagen.buffer(f,c);
                 if(aux>255)
                 aux=255;

                 if(aux<0)
                 aux=0;

                 buffer(f,c)=aux;
                 }
}
//---------------------------PROMEDIO-------------------------------------------
void Matriz::Promedio(Matriz &Imagen)
{

  int f,c;
  int aux;
  for(c=0; c <NumeroColumnas; c++)
        for(f=0; f <NumeroFilas; f++)
                 {
                 aux=(buffer(f,c)+Imagen.buffer(f,c))*0.5;
                 if(aux>255)
                 aux=255;

                 if(aux<0)
                 aux=0;

                 buffer(f,c)=aux;
                 }
}
//----------------------------INTERPOL------------------------------------------
unsigned char Matriz::Interpol(float x, float y)
{
  float a1, a2, a3, a4;
  float a,b;

  a=x-floor(x);

  if(a>=0.5)
  a=1-a;

  b=y-floor(y);

  if(b>=0.5)
  b=1-b;

  a1=(1-a)*(1-b);
  a2=a*(1-b);
  a3=(1-a)*b;
  a4=a*b;

  return (a1*buffer(x,y) + a2*buffer(x,y+1)
         + a3*buffer(x+1,y) + a4*buffer(x+1,y+1));

}
//----------------------------AMPDFVECINO---------------------------------------
void Matriz::AmpDFVecino(float a, Matriz &Imagen)
{
  int f,c;
  float x,y;
  for(c=0; c <NumeroColumnas; c++)
        for(f=0; f <NumeroFilas; f++)
                 {
                 x=c/(a);
                 y=f/(a);

                 if(x<256 && y<256)
                 buffer(f,c)=Imagen.buffer(y,x);
                 }
}
//---------------------------AMPDFINTERPOL--------------------------------------
void Matriz::AmpDFInterpol(float a, Matriz &Imagen)
{
  int f,c;
  float x,y;
  for(c=0; c <NumeroColumnas; c++)
        for(f=0; f <NumeroFilas; f++)
                 {
                 x=c/(a);
                 y=f/(a);

                 if(x<256 && y<256)
                 buffer(f,c)=Imagen.Interpol(y,x);
                 }
}
//----------------------------ROTAR---------------------------------------------
void Matriz::Rotar(float eje, float angulo, Matriz &Imagen)
{
  int f,c;
  float x,y;
  angulo=angulo*3.141632/180.0;
  for(c=0; c <NumeroColumnas; c++)
        for(f=0; f <NumeroFilas; f++)
                 {
                 x=(cos(angulo)*(c+eje) - (f)*sin(angulo));
                 y=(sin(angulo)*(c+eje) + (f)*cos(angulo));

                 if(x<256 && y<256)
                 buffer(f,c)=Imagen.Interpol(y,x);
                 }
}
//----------------------------FFT2D---------------------------------------------
void Matriz::FFT2D()
{
   for(int i=0;i<256;i++)
   {
   filaR[i] = new Vector(256);
   filaI[i] = new Vector(256);
   FR[i] = new Vector(256);
   FI[i] = new Vector(256);
   }

  int f,c;
  //Multiplica por (-1)^(x+y)`para centrar el espectro, y
  //por 256 para utiliar el metodo de TFfilas y luego TF columnas
     for(f=0; f <NumeroFilas; f++)
      for(c=0; c <NumeroColumnas; c++)
        {filaR[f]->Dato(float(buffer(f,c))*pow(-1,c+f)*256,c);
         filaI[f]->Dato(0.0,c);}

  //Transformación de las filas
  for(f=0; f <NumeroFilas; f++)
    filaR[f]->FFTs(*filaR[f],*filaI[f],*FR[f],*FI[f]);

  //filaR y filaI se convierten en el vector de columnas de FR y FI
  for(c=0; c <NumeroColumnas; c++)
  for(f=0; f <NumeroFilas; f++)
     {(*filaR[c])[f]=(*FR[f])[c];
       (*filaI[c])[f]=(*FI[f])[c];}

  //Transformación de las columnas
  for(c=0; c <NumeroColumnas; c++)
    filaR[c]->FFTs(*filaR[c],*filaI[c],*FR[c],*FI[c]);

  //Coeficiente d: Mejora de la visualización del espectro
  int max=0;
  for(c=0; c <NumeroColumnas; c++)
   for(f=0; f <NumeroFilas; f++)
     if(log(1+sqrt((*FR[c])[f]*(*FR[c])[f] + (*FI[c])[f]*(*FI[c])[f]))>max)
       max=log(1+sqrt((*FR[c])[f]*(*FR[c])[f] + (*FI[c])[f]*(*FI[c])[f]));
       d=255/max;

   //Graba el espectro
   for(c=0; c <NumeroColumnas; c++)
    for(f=0; f <NumeroFilas; f++)
    {espectroR[f][c]=(*FR[c])[f];
     espectroI[f][c]=(*FI[c])[f];}

  //Gráfico del espectro
  for(c=0; c <NumeroColumnas; c++)
   for(f=0; f <NumeroFilas; f++)
   {

   int valor=d*log(1 + sqrt((*FR[c])[f]*(*FR[c])[f] + (*FI[c])[f]*(*FI[c])[f]));
   buffer(f,c)=d*log(1 + sqrt((*FR[c])[f]*(*FR[c])[f] + (*FI[c])[f]*(*FI[c])[f]));

   if(valor>255)
   buffer(f,c)=255;
    }

   delete[] filaR;
   delete[] filaI;
   delete[] FR;
   delete[] FI;
}
//----------------------------IFFT2D--------------------------------------------
void Matriz::IFFT2D()
{
   for(int i=0;i<256;i++)
   {
   filaR[i] = new Vector(256);
   filaI[i] = new Vector(256);
   FR[i] = new Vector(256);
   FI[i] = new Vector(256);
   }

   int f,c;

   //Carga el espectro
   for(c=0; c <NumeroColumnas; c++)
    for(f=0; f <NumeroFilas; f++)
      {(*FR[c])[f]=espectroR[f][c];
       (*FI[c])[f]=espectroI[f][c];}

//Antitransformación de las columnas
  for(c=0; c <NumeroColumnas; c++)
      { for(int i=0;i<256;i++)
       (*FI[c])[i]=0-(*FI[c])[i];

       filaR[c]->FFTs(*FR[c],*FI[c],*filaR[c],*filaI[c]);
      }
//Trasposición de los vectores
for(c=0; c <NumeroColumnas; c++)
  for(f=0; f <NumeroFilas; f++)
      {(*FR[c])[f]=(*filaR[f])[c];
       (*FI[c])[f]=(*filaI[f])[c];}

 //Antitransformación de las filas
  for(f=0; f <NumeroFilas; f++)
        filaR[f]->FFTs(*FR[f],*FI[f],*filaR[f],*filaI[f]);

  //Gráfico de la imagen
  for(c=0; c <NumeroColumnas; c++)
   for(f=0; f <NumeroFilas; f++)
   {   int valor=256*(*filaR[f])[c]*pow(-1,f+c);
       buffer(f,c)=256*(*filaR[f])[c]*pow(-1,f+c);

       if (valor>255)
       buffer(f,c)=255;

       if (valor<0)
       buffer(f,c)=0;
   }
   delete[] filaR;
   delete[] filaI;
   delete[] FR;
   delete[] FI;
}
//----------------------------GRAFSPCTR-----------------------------------------
void Matriz::GrafSpctr()
{   int f,c;
//Grafico el nuevo el espectro
   for(c=0; c <NumeroColumnas; c++)
    for(f=0; f <NumeroFilas; f++)
      {int valor=d*log(1 + sqrt(espectroR[f][c]*espectroR[f][c] + espectroI[f][c]*espectroI[f][c]));
     buffer(f,c)=d*log(1 + sqrt(espectroR[f][c]*espectroR[f][c] + espectroI[f][c]*espectroI[f][c]));

   if(valor>255)
   buffer(f,c)=255;
      }
}

void Matriz::PasaAltosFrecuencial(int fc)
{
   int f,c;
    for(c=0; c <NumeroColumnas; c++)
        for(f=0; f <NumeroFilas; f++)
          if(sqrt((c-128)*(c-128) + (f-128)*(f-128))<fc)
          {espectroR[f][c]=0;
           espectroI[f][c]=0;}
     GrafSpctr();
}

void Matriz::PasaBajosFrecuencial(int fc)
{
    int f,c;
    for(c=0; c <NumeroColumnas; c++)
        for(f=0; f <NumeroFilas; f++)
          if(sqrt((c-128)*(c-128) + (f-128)*(f-128))>fc)
          {espectroR[f][c]=0;
           espectroI[f][c]=0;}
        GrafSpctr();
}

void Matriz::Notch(int x1,int y1,int x2, int y2)
{
      int f,c;
    for(c=0; c <NumeroColumnas; c++)
        for(f=0; f <NumeroFilas; f++)
          if(c>x1 && c<x2 && f>y1 && f<y2)
          {espectroR[f][c]=0;
           espectroI[f][c]=0;}
        GrafSpctr();
}
//-------------------PASAALTOSBUTTERFRECUENCIAL---------------------------------
void Matriz::PasaAltosButterFrecuencial(int fc, int n)
{
int f,c;
float raiz;
for(c=0; c <NumeroColumnas; c++)
for(f=0; f <NumeroFilas; f++)
{ raiz= sqrt(pow(float(f)-128,2)+pow(float(c)-128,2));
  if (raiz==0)
  raiz=0.0000000001;

 espectroR[f][c]=espectroR[f][c]*(1.0/(1+pow(float(fc)/raiz,2*float(n))));
 espectroI[f][c]=espectroI[f][c]*(1.0/(1+pow(float(fc)/raiz,2*float(n))));}
GrafSpctr();
}
//---------------PASABAJOSBUTTERFRECUENCIAL-------------------------------------
void Matriz::PasaBajosButterFrecuencial(int fc, int n)
{
    int f,c;
    float raiz;
    for(c=0; c <NumeroColumnas; c++)
        for(f=0; f <NumeroFilas; f++)
          {raiz= sqrt(pow(float(f)-128,2)+pow(float(c)-128,2));
          espectroR[f][c]=espectroR[f][c]*(1.0/(1+pow(raiz/float(fc),2*float(n))));
           espectroI[f][c]=espectroI[f][c]*(1.0/(1+pow(raiz/float(fc),2*float(n))));}
        GrafSpctr();
}
//-------------------FILTRARHISTO-----------------------------------------------
void Matriz::FiltrarHisto()
{
    float histomov[256];

    for(int i=1;i<255;i++)
    histomov[i]=(histo[i]+histo[i+1]+histo[i-1])/3;

    histomov[0]=(histo[0]+histo[1])/2;
    histomov[256]= (histo[256]+histo[255])/2;

    for(int i=0;i<256;i++)
    histo[i]=histomov[i];
}
//---------------------------ISODATA--------------------------------------------
void Matriz::Isodata()
{
    float u0,u1,uT1,uT2,uMean;
    u0=u1=uT1=uT2=0;
    uMean=128;
    float uMean2=128;
    float dif=0;
    FiltrarHisto();

    do{
    uT1=uT2=0;
    u0=u1=0;
    if(0<uMean<255)
    {
    for(int i=0;i<uMean;i++)
    {u0=u0+histo[i]*i;
     uT1=uT1+histo[i];
    }
    for(int i=uMean;i<256;i++)
     {u1=u1+histo[i]*i;
     uT2=uT2+histo[i];
     }
     }
      if(uT1!=0)
     u0=u0/uT1;
       else break;
      if(uT2!=0)
     u1=u1/uT2;
      else break;

     uMean2=(u0+u1)/2;
     dif= fabs(uMean2-uMean);
     uMean=uMean2;
     }while(dif>1);

    Binarizacion(uMean);
    Transformar();
}
//-------------------DETECTARPUNTOS---------------------------------------------
void Matriz::DetectarPuntos()
{
float coefs[3][3];

for(int i=0;i<3;i++)
for(int j=0;j<3;j++)
{coefs[i][j]=-1;
 coefs[i][j]=coefs[i][j];
}
coefs[1][1]=8;

SetMascara(coefs);
AplicarMascara(120);
}
//----------------------DETECTARLINEASH-----------------------------------------
void Matriz::DetectarLineaH()
{
float coefs[3][3];

coefs[0][0]=coefs[0][1]=coefs[0][2]=coefs[2][0]=coefs[2][1]=coefs[2][2]=-1;
coefs[1][0]=coefs[1][1]=coefs[1][2]=2;

SetMascara(coefs);
AplicarMascara(120);
}
//----------------------DETECTARLINEASV-----------------------------------------
void Matriz::DetectarLineaV()
{
float coefs[3][3];

coefs[0][0]=coefs[1][0]=coefs[2][0]=coefs[0][2]=coefs[1][2]=coefs[2][2]=-1;
coefs[0][1]=coefs[1][1]=coefs[2][1]=2;

SetMascara(coefs);
AplicarMascara(120);
}
//----------------------DETECTARLINEAS45----------------------------------------
void Matriz::DetectarLinea45()
{
float coefs[3][3];

coefs[0][0]=coefs[0][1]=coefs[1][0]=coefs[1][2]=coefs[2][1]=coefs[2][2]=-1;
coefs[0][2]=coefs[1][1]=coefs[2][0]=2;

SetMascara(coefs);
AplicarMascara(120);
}
//----------------------DETECTARLINEAS_45---------------------------------------
void Matriz::DetectarLinea_45()
{
float coefs[3][3];

coefs[0][1]=coefs[0][2]=coefs[1][0]=coefs[1][2]=coefs[2][0]=coefs[2][1]=-1;
coefs[0][0]=coefs[1][1]=coefs[2][2]=2;

SetMascara(coefs);
AplicarMascara(120);
}
//----------------------SQUARETRACING-------------------------------------------
int Matriz::SquareTracing(int N, int x0, int y0, int d0, Matriz &Aux)    //0:up 1:right 2:down 3:left
{
int perim=0;
int f_actual=y0;
int c_actual=x0;
int f_inicio=f_actual;
int c_inicio=c_actual;
int limite;

int dir=d0;

        if(dir==0)
        limite=200;
        if(dir==1)
        limite=320;
        if(dir==2)
        limite=200;
        if(dir==3)
        limite=320;

//guarda el sentido de movimiento
//0:up 1:right 2:down 3:left

while(buffer(f_actual,c_actual)==0 && limite!=0)
{
        if(dir==0)
        f_actual--;
        if(dir==1)
        c_actual++;
        if(dir==2)
        f_actual++;
        if(dir==3)
        c_actual--;

        limite--;
}

f_inicio=f_actual;
c_inicio=c_actual;

c_actual--;

 while(N!=0)
 {
      while((f_actual!=f_inicio && c_actual!=c_inicio)||(f_actual==f_inicio && c_actual!=c_inicio)||(f_actual!=f_inicio && c_actual==c_inicio))
      {
        if(buffer(f_actual,c_actual)==255)
        {
        espectroR[f_actual][c_actual]=255;
        dir--;

        if(dir<0)
        dir=3;

        if(dir==0)
        f_actual--;
        if(dir==1)
        c_actual++;
        if(dir==2)
        f_actual++;
        if(dir==3)
        c_actual--;
        }else{
        dir++;
        if(dir>3)
        dir=0;
        if(dir==0)
        f_actual--;
        if(dir==1)
        c_actual++;
        if(dir==2)
        f_actual++;
        if(dir==3)
        c_actual--;
        }

      }
        N--;
        dir--;
        if(dir<0)
        dir=3;

        if(dir==0)
        f_actual--;
        if(dir==1)
        c_actual++;
        if(dir==2)
        f_actual++;
        if(dir==3)
        c_actual--;
 }
     float valor;

     for(int f=0;f<NumeroFilas;f++)
     for(int c=0;c<NumeroColumnas;c++)
     {
     Aux.buffer(f,c)= (Aux.buffer(f,c)+espectroR[f][c])/2;
     //if (buffer(f,c)==0)
     //perim++;
     }
  return perim;
}
//---------------------LIMPIAR--------------------------------------------------
void Matriz::Limpiar(int color)
{             for(int c = 0; c < NumeroColumnas; c++ )
                for(int f = 0; f < NumeroFilas; f++)
                espectroR[f][c]=color;
}
//--------------------CENTROG---------------------------------------------------
int Matriz::CentroG()
{
int n=0;
XG=YG=0;
       for(int c = 0; c < NumeroColumnas; c++ )
                for(int f = 0; f < NumeroFilas/2; f++)
                   {
                   if(buffer(f,c)!=0)
                   {n++;
                    XG=XG+c;
                    YG=YG+f;
                   }
                   }
      if(n!=0)
      {
      XG=XG/n;
      YG=YG/n;
      }

      return (320-XG); 
}

int Matriz::GetXG()
{  return XG;
}

int Matriz::GetYG()
{   return YG;
}
//------------------------------------------------------------------------------






