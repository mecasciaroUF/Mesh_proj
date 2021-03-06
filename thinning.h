/*******************************************************************************
  El siguiente algoritmo es mi implementaci�n personal del algoritmo de Kalman
  Palagy et. al. en su paper "A Sequential 3D Thinning Algorithm and its Medical
  Applications". El mismo es un algoritmo secuencial de adelgazamiento
  progresivo basado en 6-subiteraciones, cada una para las direcciones Up, Down,
  North, South, East & West. El algoritmo 'PALAGY' busca secuencialmente puntos
  simples, que son aquellos puntos que pueden ser eliminados sin alterar la
  topolog�a de la imagen. Esto se logra reconociendo puntos simples como
  aquellos que satisfacen simult�neamente las siguientes caracter�sticas locales:
  a) Son puntos no aislados (tiene alg�n 26-vecino perteneciente a la imagen)
  b) El conjunto de 26-vecinos pertenecientes a la imagen est� 26-conectado en
     s� mismo (equivale a decir que no desconecta estrcuturas originalmente
     conectadas).
  c) El conjunto de 6-vecinos no pertenecientes a la imagen no est� vac�o (es
     decir, se considera como borde de la imagen a los puntos que tienen alg�n
     6-vecino fuera de la imagen).
  d) El conjunto de 6-vecinos no pertenecientes a la imagen est� 6-conectado en
     el conjunto de 18-vecinos no pertenecientes a la imagen (equivale a decir
     que no conecta cavidades originalmente desconectadas).

     Se emplea un criterio de conectividad mixta: 26-conectividad para puntos de
     la imagen, y 6-conectividad para puntos de fondo. 

  Los puntos simples son encontrados en una direcci�n determinada y marcados.
  Dentro de los puntos marcados, son eliminados aquellos que, luego de que alg�n
  vecino suyo fue eliminado aun conservan la propiedad de ser punto simple (rechequeo).
  Esto claramente indica que la capacidad de encontrar puntos simples del algoritmo
  depende del orden en que se eliminan los mismos. La ventaja es que garantiza un
  siempre un esqueleto conexo.

  Mariano E. Casciaro - Bs. As. - 01 de octubre de 2010. 
*******************************************************************************/
//------------------------------------------------------------------------------
bool IS_COND2_SATISFIED(bool* NP, bool*** M)
{
//NP: Vecindad de un punto p;
bool es = false;

//M: recibe (para no pedir en cada momemento) la Matriz de vecindad de p:
//5x5x5 en lugar de 3x3x3 para evitar problemas de contorno:

for(int i=0; i<5; i++)
 for(int j=0; j<5; j++)
  for(int k=0; k<5; k++)
   M[i][j][k]=0;

//Llenado de la Matriz Vecindad de p, sin p
int k = 0;

Punto* c = new Punto; //Puntos semillas para ver conectividad:

for(int z=1; z<4; z++)
    for(int x=1; x<4; x++)
        for(int y=1; y<4; y++)
        {
         if (!(x==2 && y==2 && z==2)) //Si no estoy en el medio, o sea, sobre el punto p
         {
          if (NP[k])
          {
           M[x][y][z]=1;
           c->x=x; c->y=y; c->z=z; //B�squeda del punto semilla "c"
          }
         k++;
         }
        }

TList* CP = new TList; //connected points
CP->Add(c);

TList* AC = new TList;

M[c->x][c->y][c->z]=0; //Eliminacion en M del punto semilla "c"

Punto* np;
Punto* ap;

TList* NewP = new TList; //array de coordenadas de los puntos vecinos

bool* NP2 = new bool[26]; //almacena la vecindad dentro de los puntos NP

while(CP->Count!=0)
{
 c = (Punto*)CP->Items[0];

 collect26neigh(c,M, NP2);

 COLLECT_26NEIGH_COORDS(NP2,c, NewP);

 for(int j=0; j<NewP->Count; j++)
 {
    ap = (Punto*)NewP->Items[j];
    np = new Punto;
    np->x = ap->x; np->y = ap->y; np->z = ap->z;
    CP->Add(np);
    M[ap->x][ap->y][ap->z]=0;
 }

 //np = new Punto;
 //np->x= c->x; np->y = c->y; np->z = c->z;
 //AC->Add(np);
   AC->Add(c);

 CP->Delete(0);

 for(int j=0; j<NewP->Count; j++)
 {
 ap = (Punto*)NewP->Items[j];
 delete ap;
 }
 NewP->Clear();

}//end while...

if (AC->Count==suma(NP) )
    es = true;

//Borrar elementos auxiliares:
delete[] NP2;

for(int j=0; j<NewP->Count; j++)
{
ap = (Punto*)NewP->Items[j];
delete ap;
}
NewP->Clear();
delete NewP;

for(int j=0; j<CP->Count; j++)
{
ap = (Punto*)CP->Items[j];
delete ap;
}
CP->Clear();
delete CP;

for(int j=0; j<AC->Count; j++)
{
ap = (Punto*)AC->Items[j];
delete ap;
}
AC->Clear();
delete AC;

return es;

}
//------------------------------------------------------------------------------
bool IS_COND4_SATISFIED(bool* Np, bool*** M)
{
//Input: Np: Vecindad de un punto p;

bool es = false;

//M: recibe (para no pedir en cada momemento) la Matriz de vecindad de p:
//5x5x5 en lugar de 3x3x3 para evitar problemas de contorno:
for(int i=0; i<5; i++)
 for(int j=0; j<5; j++)
  for(int k=0; k<5; k++)
   M[i][j][k]=0;

//Creacion de la Matriz Vecindad de p

//INVIERTO LA VECINDAD PARA TRABAJAR CON LOS MISMOS ALGORITMOS QUE BUSCAN
//CONECTIVIDAD ENTRE VECINOS NEGROS
for(int i=0; i<26; i++)
{
  if(Np[i])
  {
  Np[i]=false;
  continue;
  }

  if(!Np[i])
  {
  Np[i]=true;
  continue;
  }

}
//IGUALO A 0 TODOS LOS CONRNER POINTS, CONVIRTIENDO LA 26-VECINDAD EN 18-VECINDAD
M[1][1][1]=0;
M[1][2][1]= Np[1];
M[1][3][1]=0;
M[2][1][1]=Np[3];
M[2][2][1]=Np[4]; //U
M[2][3][1]=Np[5];
M[3][1][1]=0;
M[3][2][1]=Np[7];
M[3][3][1]=0;
 
M[1][1][2]=Np[9];
M[1][2][2]=Np[10];//%N
M[1][3][2]=Np[11];
M[2][1][2]=Np[12];//%W
M[2][2][2]=0;
M[2][3][2]=Np[13];//%E
M[3][1][2]=Np[14];
M[3][2][2]=Np[15];//%S
M[3][3][2]=Np[16];

M[1][1][3]=0;
M[1][2][3]=Np[18];
M[1][3][3]=0;
M[2][1][3]=Np[20];
M[2][2][3]=Np[21];//%D
M[2][3][3]=Np[22];
M[3][1][3]=0;
M[3][2][3]=Np[24];
M[3][3][3]=0;

 int UpFlag=0, NorthFlag=0, WestFlag=0, EastFlag=0, SouthFlag=0, DownFlag=0;

Punto* c = new Punto;
c=NULL;

//Encuentro cualquiera de los puntos U,D,N,S,E,W para usar como semilla de un
//crecimiento 6-connected:

if (M[2][2][1]==1)
{
    UpFlag = 1;
    c=new Punto;
    c->x=2; c->y=2; c->z=1;
}

if (M[1][2][2]==1)
{
    NorthFlag = 1;
    c=new Punto;
    c->x=1; c->y=2; c->z=2;
}

if (M[2][1][2]==1)
{
    WestFlag = 1;
    c=new Punto;
    c->x=2; c->y=1; c->z=2;
}

if (M[2][3][2]==1)
{
    EastFlag = 1;
    c=new Punto;
    c->x=2; c->y=3; c->z=2;
}

if (M[3][2][2]==1)
{
    SouthFlag = 1;
    c=new Punto;
    c->x=3; c->y=2; c->z=2;
}

if (M[2][2][3]==1)
{
    DownFlag = 1;
    c=new Punto;
    c->x=2; c->y=2; c->z=3;
}

int cant_borders=UpFlag+EastFlag+WestFlag+NorthFlag+SouthFlag+DownFlag;

if (c == NULL)
    return es;

TList* CP = new TList;

CP->Add(c);

TList* AC = new TList;

TList* NewP = new TList;

M[c->x][c->y][c->z]=0;

bool* NP = new bool[26];

Punto* ap;
Punto* np;

while (CP->Count!=0)
{
ap = (Punto*)CP->Items[0];
 //COLLECT_6_NEIGH2 y
 //COLLECT_6_COORDS para asegurarnos que estamos tomando vecinos 6
 //conectados.

 collect26neigh(ap,M, NP);
 COLLECT_6NEIGH_COORDS(NP,ap, NewP);

 for (int j=0; j<NewP->Count; j++)
 {
    ap = (Punto*)NewP->Items[j];
    M[ap->x][ap->y][ap->z]=0;
 }

 for (int j=0; j<NewP->Count; j++)
 {
    ap = (Punto*)NewP->Items[j];
    np = new Punto;
    np->x=ap->x; np->y=ap->y; np->z=ap->z;
    CP->Add(np);
 }
ap = (Punto*)CP->Items[0];
//np = new Punto;
//np->x=ap->x;np->y=ap->y; np->z=ap->z;
//AC->Add(np);
AC->Add(ap);
CP->Delete(0);

 for(int j=0; j<NewP->Count; j++)
 {
 ap = (Punto*)NewP->Items[j];
 delete ap;
 }
 NewP->Clear();
 
}

        //Limpieza de la matriz auxiliar
        for (int i=0; i<5; i++)
        for (int j=0; j<5; j++)
        for (int k=0; k<5; k++)
        M[i][j][k]=0;


for(int i=0; i<AC->Count; i++)
{
ap = (Punto*)AC->Items[i];
M[ap->x][ap->y][ap->z]=1;
}

int suma_border = int(M[2][2][1])+ int(M[1][2][2])+ int(M[2][1][2])+ int(M[2][3][2]) +
                  int(M[3][2][2])+ int(M[2][2][3]);

if (suma_border == cant_borders)
es = true;

//delete de elementos auxiliares dinamicos:
for(int j=0; j<CP->Count; j++)
{
ap = (Punto*)CP->Items[j];
delete ap;
}
CP->Clear();
delete CP;

for(int j=0; j<AC->Count; j++)
{
ap = (Punto*)AC->Items[j];
delete ap;
}
AC->Clear();
delete AC;

delete[] NP;

for(int j=0; j<NewP->Count; j++)
{
ap = (Punto*)NewP->Items[j];
delete ap;
}
NewP->Clear();
delete NewP;

return es;
}
//------------------------------------------------------------------------------
bool IS_BORDER_POINT(bool*** M, char dir, Punto* p)
{
 bool es = false;

 if (dir=='U' && M[p->x][p->y][p->z+1]==0)
 {
 es = true;
 return es;
 }

 if(dir =='D' && M[p->x][p->y][p->z-1]==0)
 {
 es = true;
 return es;
 }

 if(dir == 'E' && M[p->x][p->y+1][p->z]==0)
 {
 es = true;
 return es;
 }
 
 if(dir =='W' && M[p->x][p->y-1][p->z]==0)
 {
 es = true;
 return es;
 }

 if(dir =='N' && M[p->x-1][p->y][p->z]==0)
 {
 es = true;
 return es;
 }

 if(dir =='S' && M[p->x+1][p->y][p->z]==0)
 {
 es = true;
 return es;
 }

 return es;

}
//------------------------------------------------------------------------------
bool IS_ENDPOINT(bool* NP)
{
bool es = false;

 if(suma(NP)<=1) //clasifica como endpoint a puntos aislados asi como puntos verdaderos endpoints
 es = true;

return es;
}
//------------------------------------------------------------------------------

bool IS_SIMPLE(bool* NP, bool*** M)
{
bool es = false;

 if(IS_COND2_SATISFIED(NP, M))
        if(IS_COND4_SATISFIED(NP, M))
           es = true;

return es;
}
//------------------------------------------------------------------------------
int SUBITER(TList* Y, bool*** M, char dir, bool*** Masc)
{
int modified = 0;

    TList* list = new TList;//Almacena las coordenas de los puntos a ser eliminados en la iteracion
    list->Capacity = Y->Count;

    int* index_list = new int[Y->Count];//Almacena el indice de los valores a ser eliminados de Y

    int j=0; //indice de la lista que almacena los puntos a eliminar

    bool* NP = new bool[26]; //array para guardar el valor de los puntos vecinos a un punto

    Punto* ap; //Puntero a punto auxiliar
    Punto* np; //Puntero a nuevo punto

    int nei[26];

    for (int i=0; i< Y->Count; i++)
    {
        ap = (Punto*)Y->Items[i];

        if(IS_BORDER_POINT(M,dir,ap) )
        {
            //COLLECT_... : input: M,ap - output: NP
            collect26neigh(ap,M, NP);

                if(!IS_ENDPOINT(NP))
                {
                    if(IS_SIMPLE(NP, Masc))
                    {
                    np = new Punto;
                    np->x = ap->x;
                    np->y = ap->y;
                    np->z = ap->z;
                    list->Add(np);

                    index_list[j] = i;
                    j++;
                    }//end IS_SIMPLE?
                }//end IS_ENDPOINT?
        }//end IS_BORDER_POINT?
    }//end for

    //rechequeo: los puntos que son simples lo siguen siendo aunque sus vecinos
    //sean eliminados?
    int* index_list2 = new int[Y->Count];

    int k = 0;
    for(int i = 0; i < j; i++) //j o j-1?? chequear y borrar este comentario
    {
        ap = (Punto*)list->Items[i];
        collect26neigh(ap,M, NP);
            if (!IS_ENDPOINT(NP))
            {
                    if(IS_SIMPLE(NP, Masc))
                    {
                    M[ap->x][ap->y][ap->z]=0;
                    index_list2[k]=index_list[i];
                    k++;
                    modified++;
                    }
            }

    }

    for(int i=0; i<k; i++) //k o k-1?? probar y borrar este comentario
    {
    Y->Items[index_list2[i]] = NULL;  //hace apuntar a null a los elementos a borrar
    }

    //elimina todos los elementos que apuntan a null:
    Y->Pack();
    Y->Capacity = Y->Count;

    for(int j=0; j<list->Count; j++)
    {
    ap = (Punto*)list->Items[j];
    delete ap;
    }
    list->Clear();
    delete list;

    delete[] index_list;
    delete[] index_list2;
    delete[] NP;

    return modified;
}
//------------------------------------------------------------------------------
void PALAGY(TList* X, TList* Y,
            short int minX, short int minY, short int minZ,
            short int maxX, short int maxY, short int maxZ)
{
//Input: X: nube de puntos (x,y,z) representando el objeto a esqueletonizar
//       min,max: extremos de los puntos contenidos en X.
//Output: Y: esqueleto topol�gicamente equivalente de X.

//Los bordes del menor cubo que contiene a todos los puntos de X se llevan al origen y
//se agrandan en una distancia de 2, para no tener problemas de vecindad en los puntos
//de los extremos:
Punto* ap; //puntero a Punto auxiliar
Punto* np; //Puntero a nuevo punto

//1er paso: Y = X: el equeleto ES el volumen completo
for(int i=0; i<X->Count; i++)
{
  ap = (Punto*)X->Items[i]; //Se desplazan en +1 para evitar problemas de vecindad hacia atr�s
  np = new Punto;
  np->x = ap->x - minX + 1;
  np->y = ap->y - minY + 1;
  np->z = ap->z - minZ + 1;
  Y->Add(np);
}

//Creacion de la matriz booleana que representa a los puntos de X como "1" y a los puntos
//no pertenecientes a X como "0":
bool*** M;

        M= new bool** [(maxX-minX+1) + 2];          //+2: extiende las dimensiones de la
        for (int i=0; i<(maxX-minX+1) + 2; i++)     //    m�scara para evitar problemas
        {                                           //    de contorno.
        M[i]=new bool* [(maxY-minY+1) + 2];
                for (int j=0; j<(maxY-minY+1) + 2; j++)
                {
                M[i][j]=new bool [(maxZ-minZ+1) + 2];
                      for (int k=0; k<(maxZ-minZ+1) + 2; k++)
                         M[i][j][k]=0;
                }
        }

      //Mascara de Vecindad:
      bool*** Masc;
        Masc= new bool** [5];          //+2: extiende las dimensiones de la
        for (int i=0; i<5; i++)     //    m�scara para evitar problemas
        {                                           //    de contorno.
        Masc[i]=new bool* [5];
                for (int j=0; j<5; j++)
                {
                Masc[i][j]=new bool [5];
                      for (int k=0; k<5; k++)
                         Masc[i][j][k]=0;
                }
        }


for(int i=0; i<Y->Count; i++)
{
  ap = (Punto*)Y->Items[i];
  M[ap->x][ap->y][ap->z]=1;
}

//2do paso: esqueletonizar mediante "thinning" secuencial:
short int modified = -1; //indica la cantidad de puntos eliminados en la subiteracion

short int modU, modD, modN, modS, modE, modW; //cantidad de puntos modificados en
                                              //cada direcci�n: Up, Down, North, etc..

//NOTA: El orden de las direcciones de subiteracion MODIFICA el resultado del algoritmo.
//El enfoque es hacia un tipo de subiteracion sim�trica. Palagy et al proponenen
//en su paper "A Sequential 3D Thinning Algorithm and its Medical Applications"
//una subiteraci�n sim�trica del tipo U-D,N-S,E-W para favorecer la esqueletonizacion
//de estructuras con simetr�a tubular:

 while(modified != 0)
 {
    modified = 0;

     //SUBITER: Input: Y,M,'direccion'
     //         Output: mod'direccion'
     modU = SUBITER(Y,M,'U', Masc);
     modD = SUBITER(Y,M,'D', Masc);

     modN = SUBITER(Y,M,'N', Masc);
     modS = SUBITER(Y,M,'S', Masc);

     modE = SUBITER(Y,M,'E', Masc);
     modW = SUBITER(Y,M,'W', Masc);

    modified = modU+modD+modN+modS+modE+modW;

 } //end while: bucle principal

//3er paso: Re-transladar los valores del esqueleto obtenido para que coincidan
//con las coordenadas originales:
 for(int i=0; i<Y->Count; i++)
 {
  ap = (Punto*)Y->Items[i];
  ap->x = ap->x + minX - 1;
  ap->y = ap->y + minY - 1;
  ap->z = ap->z + minZ - 1;
 }

   //Eliminacion de la matriz auxiliar:
   for (int i=0; i<(maxX-minX+1) + 2; ++i)
           for (int j=0; j<(maxY-minY+1)+2; ++j)
                delete[] M[i][j];
        for (int i=0; i<(maxX-minX+1) + 2; ++i)
                delete[] M[i];
        delete[] M;

    //Eliminacion de la vecindad auxiliar:
    for (int i=0; i<5; ++i)
           for (int j=0; j<5; ++j)
                delete[] Masc[i][j];
        for (int i=0; i<5; ++i)
                delete[] Masc[i];
        delete[] Masc;

}//Fin del algoritmo
//------------------------------------------------------------------------------
void clasificacionEsqueleto(TList* Esqueleto,
                                short int minX, short int minY, short int minZ,
                                short int maxX, short int maxY, short int maxZ,
                                TList* EP, TList* LP, TList* JP)
{
 //Input: Esqueleto y dimensiones de la caja contenedora.
 //EP: End-Points => 1 vecino
 //LP: Line-Points => 2 vecinos
 //JP: Joint-Points => >2 vecinos

 //Creacion de la matriz booleana que representa a los puntos de X como "1" y a los puntos
 //no pertenecientes a X como "0":
 bool*** M;

        M= new bool** [(maxX-minX+1) + 2];          //+2: extiende las dimensiones de la
        for (int i=0; i<(maxX-minX+1) + 2; i++)     //    m�scara para evitar problemas
        {                                           //    de contorno.
        M[i]=new bool* [(maxY-minY+1) + 2];
                for (int j=0; j<(maxY-minY+1) + 2; j++)
                {
                M[i][j]=new bool [(maxZ-minZ+1) + 2];
                      for (int k=0; k<(maxZ-minZ+1) + 2; k++)
                         M[i][j][k]=0;
                }
        }

 Punto* ap; //Puntero a punto auxiliar

 for(int i=0; i<Esqueleto->Count; i++)
 {
  ap = (Punto*)Esqueleto->Items[i];
  M[ap->x-minX+1][ap->y-minY+1][ap->z-minZ+1]=1;
 }

 bool *N26;
 N26 = new bool[26]; //array de la 26 vecindad

 for(int i=0; i<Esqueleto->Count; i++)
 {
 ap = (Punto*)Esqueleto->Items[i];
 ap->x -= minX - 1;
 ap->y -= minY - 1;
 ap->z -= minZ - 1;

 collect26neigh(ap,M, N26);
 int grupo = suma(N26);

 ap->x += minX - 1;
 ap->y += minY - 1;
 ap->z += minZ - 1;

 if(grupo==0)
 EP->Add(ap);

 if(grupo==1)
 EP->Add(ap);

 if(grupo==2)
 LP->Add(ap);

 if(grupo>2)
 JP->Add(ap);
 }

   for (int i=0; i<(maxX-minX+1) + 2; ++i)
           for (int j=0; j<(maxY-minY+1)+2; ++j)
                delete[] M[i][j];
        for (int i=0; i<(maxX-minX+1) + 2; ++i)
                delete[] M[i];
        delete[] M;

  delete[] N26;
}
//------------------------------------------------------------------------------

