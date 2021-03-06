struct punto3D
{
float x,y,z;
float dx,dy,dz; //direccion de movimiento.
};

//------------------------------------------------------------------------------
void balloon3D(short int*** M, short int minPX, int x0, int y0, int z0, int r0,
short int umbral, int umbralPC, TList* Resultado)
{
//M: matriz de puntos de los estudios Dicom
//minPX: menor valor en HU de la matriz M
//x0,y0, z0: centro de gravedad de la imagen
//r0: radio inicial expandible.
//umbral: valor en HU que separa el miocardio de la regi�n pulmonar
//umbralPC: umbral de puntos de contacto.

TList* Fresult = new TList; //Lista de almacenamiento para el c�lculo de la fuerza resultante
punto3D* paux; //Auxiliar para tomar el valor de las listas

float theta = 2*3.141632/360;
float fi = 3.141632/180;

float xC, yC, zC; //Centro del globo.
xC=x0;
yC=y0;
zC=z0;

float Fx=0; //luego se tiene que castear a int para redondear
float Fy=0; //luego se tiene que castear a int para redondear
float Fz=0;
float normF;
bool Termino=false;

float r=r0; //int?

float contactox, contactoy, contactoz; //auxiliares para la normalizacion de los puntos de fuerza
short int signo; //Variable auxiliar para conservar el signo al redondear n�mero

int cont=0;

while(true)
{
     cont++;

     if(cont>1000)
     break;

     if(Termino)
     break;
     
     for(int i = 0; i<360; i++)
      for(int j = 0; j<180; j++)
       {
           float x=r*sin(theta*i)*sin(fi*j)+xC;
           float y=r*cos(theta*i)*sin(fi*j)+yC;
           float z=r*cos(fi*j)+zC;

                if(M[int(z)][int(x)][int(y)]<= umbral)
                {
                punto3D* puntof = new punto3D; //Punto de contacto: genera fuerza resultante
                contactox = -sin(theta*i)*sin(fi*j);
                contactoy = -cos(theta*i)*sin(fi*j);
                contactoz = -cos(fi*j);
                puntof->x=contactox; puntof->y=contactoy; puntof->z=contactoz;
                Fresult->Add(puntof);
                }//end if: encuentra un punto de contacto en el borde de la circ
        }//end for: termina de recorrer la esfera

       if(Fresult->Count>umbralPC) //Si la cantidad de puntos de contacto es mayor
       {
       Termino=true;
       break;                     //que la el umbral, el algoritmo termina
       }

       if(Fresult->Count!=0)//Si existe algun punto de contacto...
       {
        Fx=0; Fy=0; Fz=0;
        //Calculo de la fuerza acumulada resultante:
        for(int i=0; i<Fresult->Count; i++)
        {
        paux = (punto3D*)Fresult->Items[i];
        Fx = Fx + paux->x;
        Fy = Fy + paux->y;
        Fz = Fz + paux->z;
        }

        double NormF=sqrt(pow(Fx,2.0)+pow(Fy,2.0)+pow(Fz,2.0));

        //Si el m�dulo de la fuerza resultante
        if(NormF<0.01)//es muy bajo, el algoritmo termina
        {
        Termino=true;
        break;
        }

        Fx=Fx/NormF;
        Fy=Fy/NormF;
        Fz=Fz/NormF;

        xC = xC+ Fx; //Desplazamiento del centro del globo
        yC = yC+ Fy;
        zC = zC+ Fz;

        Fresult->Clear();
       }// end if (existe algun punto de contacto?)
       else
       r=r+0.5; //Si no hay puntos de contacto, aumento el radio

}//end while

  circ* result = new circ;
  result->xc=xC; result->yc=yC; result->zc=zC;
  result->rc=r;
  Resultado->Add(result);

}
//------------------------------------------------------------------------------
 