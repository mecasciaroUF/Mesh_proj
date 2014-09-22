//Balloon.h tiene todas las rutinas para la geenración de un globo deformable,
// capaz de adaptarse a la superficie externa del corazón.

struct punto2D
{
float x,y;
};

struct circ //circunferencias
{
int xc,yc,zc,rc; //centro y radio
};

//------------------------------------------------------------------------------

void balloon(short int*** M, short int minPX, int x0, int y0, int z0, int r0,
short int umbral, int umbralPC, TList* Resultado)
{
//M: matriz de puntos de los estudios Dicom
//minPX: menor valor en HU de la matriz M
//x0,y0, z0: centro de gravedad de la imagen
//r0: radio inicial expandible.
//umbral: valor en HU que separa el miocardio de la región pulmonar
//umbralPC: umbral de puntos de contacto.

TList* Fresult  = new TList; //Lista de almacenamiento para el cálculo de la fuerza resultante

punto2D* paux; //Auxiliar para tomar el valor de las listas

float Fx=0; //luego se tiene que castear a int para redondear
float Fy=0; //luego se tiene que castear a int para redondear

float r=r0; 

float contactox, contactoy; //auxiliares para la normalizacion de los puntos de fuerza
short int signo; //Variable auxiliar para conservar el signo al redondear número

float ang=2.0*3.141632/360.0;
int x,y;

int cont=0;

bool Termino = false;

while(true)
{
   if(Termino)
   break;

    cont++;

     if(cont>10000)
     break;

   for(int i=0; i < 360; i++)
   {
    x = x0 + r*cos(i*ang);
    y = y0 + r*sin(i*ang);

                if(M[z0][x][y]<= umbral)
                {
                punto2D* puntof = new punto2D; //Punto de contacto: genera fuerza resultante
                contactox = -cos(i*ang);
                contactoy = -sin(i*ang);
                puntof->x=contactox; puntof->y=contactoy;
                Fresult->Add(puntof);
                }//end if: encuentra un punto de contacto en el borde de la circ
   }

       if(Fresult->Count>umbralPC) //Si la cantidad de puntos de contacto es mayor
       break;                     //que la el umbral, el algoritmo termina

       if(Fresult->Count!=0)//Si existe algun punto de contacto...
       {
       Fx=0; Fy=0;
       //Calculo de la fuerza acumulada resultante:
       for(int i=0; i<Fresult->Count; i++)
       {
       paux = (punto2D*)Fresult->Items[i];
       Fx = Fx + paux->x;
       Fy = Fy + paux->y;
       }

       if(sqrt(pow(Fx,2)+pow(Fy,2))<0.1) //Si el módulo de la fuerza resultante antes de normalizar
       {                                 //es muy bajo, el algoritmo termina
       Termino=true;
       break;
       }

       Fx = Fx/sqrt(pow(Fx,2)+pow(Fy,2));
       Fy = Fy/sqrt(pow(Fx,2)+pow(Fy,2));

       x0 = x0+Fx; //Desplazamiento del centro del globo
       y0 = y0+Fy;
   
       Fresult->Clear();
       }// end if (existe algun punto de contacto?)
       else
       r=r+0.5;

}//end while

  circ* result = new circ;
  result->xc=x0; result->yc=y0; result->zc=z0;
  result->rc=r;
  Resultado->Add(result);

}
//------------------------------------------------------------------------------

void migracionCV(short int*** M, short int minPX, short int maxPX, int x0, int y0, int z0, int r0,
short int umbral, int umbralPC, TList* Resultado, TList* DC, float maxMov, int umbralVueltas, TList* VC)
{
//Algoritmo de migración de células Virtuales. Las células son plantadas en la
//circunferencia que resulta del algoritmo "balloon":

//MaxMov: máxima movilidad (ej: 1)
//VC: ultima posicion de las CV.
//DC: direcciones excéntricas de las CV

balloon(M, minPX, x0, y0, z0, r0, umbral, umbralPC, Resultado);
//Resultado: Lista con todas las circunferencias=> Tomamos como valor iniciar
//           a Resultado->Last();

circ* circFinal = new circ;
circFinal = (circ*)Resultado->Last();

double acum=0;
double cont = 0;

//double acum2 = 0;

for(int i=circFinal->xc-circFinal->rc;i<circFinal->xc+circFinal->rc;i++)
        for(int j=circFinal->yc-circFinal->rc;j<circFinal->yc+circFinal->rc;j++)
            {
              if(i>0 && j>0 && i<512 && j<512)
              {
              acum = acum + M[z0][i][j];
              cont++;
              //acum2 = acum2 + pow(M[i][j][z0],2);
              }
            }


//double desv = sqrt(acum2/cont - acum*acum/(cont*cont)); //desvio estándar
//desv=desv/4;

//Función de movilidad: 0 si valor<sigmaA y satura si valor>sigmaB

float sigmaB = acum/cont; //sigmaB: valor medio dentro del globo resultante
//float sigmaB=175;

float sigmaA = float(umbral);

float* movilidad;
movilidad = new float[maxPX-minPX];  //movilidad: funciona como una LUT

       if(sigmaB<sigmaA) //el globo cayó en una zona errónea
       return;

        for (int i = 0; i<sigmaA-minPX; i++)
        movilidad[i]=0;

        //Lineal:
        for (int i = sigmaA-minPX; i<sigmaB-minPX; i++)
        movilidad[i]=maxMov/(sigmaB-sigmaA)*(i-(sigmaA-minPX));

        //Exponencial:
        /*
        for (int i = sigmaA-minPX; i<sigmaB-minPX; i++)
       movilidad[i] = exp(-0.5*pow((i-(sigmaA-minPX))/desv ,2)) ;
       */
        for (int i = sigmaB-minPX; i<maxPX-minPX; i++)
        movilidad[i]=maxMov;

//Crear Células Virtuales: Lista VC
//Crear Lista de direcciones unitarias DC:
//Crear lista de Movimiento de VC: VCnext;

VC->Capacity=360; //360 células virtuales: 1cv por º

float vcx, vcy, dcx, dcy; //Células virtuales // desplazamientos
float norma;

float viscosidad[360];
//Rehacer dinámico si funciona
//viscosidad: valor de intensidad en HU en el lugar de la célula virtual

for(int t=0;t<360;t++)
{
vcx=float(circFinal->rc)*sin(t*3.141632/180.0)+circFinal->xc;
vcy=float(circFinal->rc)*cos(t*3.141632/180.0)+circFinal->yc;
punto2D* vc = new punto2D;     //nueva celula virtual
punto2D* dc = new punto2D;     //nueva direccion radial normalizada

vc->x=vcx; vc->y=vcy;
VC->Add(vc);

dcx = circFinal->xc-vcx; //Vector que apunta al centro, normalizado
dcy = circFinal->yc-vcy;
norma = sqrt(pow(dcx,2)+pow(dcy,2));

dcx=1.0*dcx/norma;  //paso = 1=> cambiar si no funciona, o ingresar paso como un parámetro adicional
dcy=1.0*dcy/norma;

dc->x=dcx; dc->y=dcy;

DC->Add(dc);
}

punto2D* vcaux = new punto2D; //punto auxiliar;

punto2D* dcaux = new punto2D;

//Calcular movimiento de las celulas virtuales segun movilidad;

int vueltas=0;
int inext;
int iprev;

for(int i=0; i<VC->Count; i++)
        {
        vcaux = (punto2D*)VC->Items[i];
        dcaux = (punto2D*)DC->Items[i];
        viscosidad[i] = movilidad[M[z0][int(vcaux->x-dcaux->x)][int(vcaux->y-dcaux->y)]-minPX];
        }

      while(vueltas<umbralVueltas)
      {
        //Movimiento de la célula virtual
        for(int i=0; i<VC->Count; i++)
        {
        vcaux = (punto2D*)VC->Items[i];
        dcaux = (punto2D*)DC->Items[i];

        vcaux->x = vcaux->x - viscosidad[i]*dcaux->x;//Movimiento de la Célula Virtual
        vcaux->y = vcaux->y - viscosidad[i]*dcaux->y;
        }

        vueltas++;

        //Calculo de las nuevas viscosidades
        for(int i=0; i<VC->Count; i++)
        {
        vcaux = (punto2D*)VC->Items[i];
        dcaux = (punto2D*)DC->Items[i];
        viscosidad[i] = movilidad[M[z0][int(vcaux->x-dcaux->x)][int(vcaux->y-dcaux->y)]-minPX];
        }

        //Promedio de las nuevas viscosidades
        /*
        for(int i=0; i<VC->Count; i++)
        {
        inext=i+1;
        if(inext==360) inext=0;
        iprev=i-1;
        if(iprev==-1) iprev=359;
        viscosidad[i] = (viscosidad[i]+viscosidad[inext]+viscosidad[iprev])/4.0;
        }^*/
      }
      
delete movilidad;
}
//------------------------------------------------------------------------------
void contornoActivo(short int*** M, short int minPX, int z, int zprev, int r, TList* DC, TList* VC, TList* VCnuevo)
{
//VCnuevo: Contorno VC modificado acorde a un criterio de similitud de pixels
//          basado en las 8 direcciones de migración
//DC: direcciones radiales de las VC originales
//z: nuevo corte donde reubicar los VC
//zprev: corte anterior o siguiente (de donde provienen los VC originales)
//r: radio del scanLine de prueba

 punto2D* vcaux = new punto2D;
 punto2D* dcaux = new punto2D;

 float theta;
 float nx, ny; //desplazamiento por una linea de radio r pixels
 int dif; //diferencia entre niveles de gris del scanline
 int difminima = 10000;
 int rnuevo; //radio de la nuva CV;

 for(int i=0; i<DC->Count; i++)
 {
 dcaux = (punto2D*)DC->Items[i];
 vcaux = (punto2D*)VC->Items[i];

        for(int j=-r/2; j<=r/2; j++)
        {
        nx = vcaux->x-j*dcaux->x; //movimiento de la vc
        ny = vcaux->y-j*dcaux->y;
        dif = M[zprev][int(vcaux->x)][int(vcaux->y)]-M[z][int(nx)][int(ny)];

                if(abs(dif)<=difminima)
                {
                difminima = abs(dif);
                rnuevo = j;
                }
        }
         punto2D* nvc = new punto2D;
         nvc->x=vcaux->x-rnuevo*dcaux->x; nvc->y=vcaux->y-rnuevo*dcaux->y;
         VCnuevo->Add(nvc);
         difminima = 10000;
 }

}
//------------------------------------------------------------------------------
void Bressenham2D(int x0, int y0, int xf, int yf, TList* Coords)
{
//Algoritmo de Bressenham: devuelve en una lista las coordenadas del camino
//recto (discreto) que une los puntos X0 con Xf. Las coordenadas se almacenan
//en la lista Coords.

     Coords->Clear(); 

     int DistX = abs(xf - x0);
     int DistY = abs(yf - y0);
     int NumPixel,Decision;
     int Incrd1,Incrd2,Incrx0,Incrxf,Incry0,Incryf;

     if( DistX >= DistY)
	{//Then
         NumPixel = DistX + 1; //cantidad de pixels en la recta
         Decision = (2 * DistY) - DistX;
         Incrd1 = DistY * 2;
         Incrd2 = (DistY - DistX) * 2;
        //Incrementos:
	 Incrx0 = 1;
         Incrxf = 1;
         Incry0 = 0;
         Incryf = 1;
	}
      else
	{
         NumPixel = DistY + 1;
         Decision = (2 * DistX) - DistY;
         Incrd1 = DistX * 2;
         Incrd2 = (DistX - DistY) * 2;
        //Incrementos:
	 Incrx0 = 0;
         Incrxf = 1;
         Incry0 = 1;
         Incryf = 1;
     	}//end if-else


	//Cálculos de la dirección
	if( x0 > xf)
	{
        Incrx0 = -Incrx0;
        Incrxf = -Incrxf;
     	}

     	if( y0 > yf)
	{
        Incry0 = -Incry0;
        Incryf = -Incryf;
     	}

     	int x = x0;
     	int y = y0;

     	for(int i = 0; i < NumPixel; i++)
 	{
           //PSet (x,y), 1   => pinta de negro
           punto2D* coord = new punto2D;
           coord->x = x; coord->y=y;
           Coords->Add(coord);

          if( Decision < 0 )
	  {//Then
            Decision = Decision + Incrd1;
            x = x + Incrx0;
            y = y + Incry0;
          }
         else
	 {
            Decision = Decision + Incrd2;
            x = x + Incrxf;
            y = y + Incryf;
         }//end if-else

        }//end for

}//fin de la función
//------------------------------------------------------------------------------


