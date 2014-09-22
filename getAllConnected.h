//------------------------------------------------------------------------------
void getAllMatrixVolumeConnected(Punto* sp, Volumen *MatrizRecorridos, Volumen* Matriz, TList* AC, int thr)
{

//sp: seed-point: punto desde donde se toman todos los elementos conectados en el mismo plano:

//CP: Conjunto de puntos candidatos
//AC: Conjunto acumulativo de puntos conectados al sp
//PL: Conjunto de todos los puntos que satisfacen la condici�n dentro del volumen M
//    Finalmente, se elimina de PL todo elementos que est� en AC, funcionando como
//    una lista que agrupa todos los puntos NO conectados con sp.
//Matriz: matrices de 512*512*cantImagenes
//thr: Threshold: valor que debe cumplir un pixel para ser considerado de la misma
//     clase. Esta clase va a estar asociada con el valor dado por clasegrama.

        TList* CP = new TList;

        Punto* newsp = new Punto;

        int maxX = Matriz->getAlto();
        int maxY = Matriz->getAncho();
        int maxZ = Matriz->getProf()*0.6;

        CP->Add(sp);

        int x,y,z;

        int i,j,k;

	while(CP->Count!=0)
	{
                newsp = (Punto*)CP->First();
                AC->Add(newsp);
                CP->Delete(0);
                x=newsp->x; y=newsp->y; z=newsp->z;
                MatrizRecorridos->SetV(x,y,z,0);
		//CP = CP + NP{newsp}
		//-----------------------------------------------------------
                for(k=1;k>=-1;k--)
			for(i=-1;i<=1;i++)
				for(j=-1;j<=1;j++)
                                        if(!(i==0 && j==0 && k==0))
                                        {
                                                if(newsp->x+i>=0 && newsp->y+j>=0 && newsp->z+k>=0 && newsp->x+i<maxX && newsp->y+j<maxY && newsp->z+k<maxZ)
                                                {
						  if(Matriz->GetV(newsp->x+i,newsp->y+j,newsp->z+k)<=thr && MatrizRecorridos->GetV(newsp->x+i,newsp->y+j,newsp->z+k)==1)
			                	  {
                                                  Punto* cp = new Punto;
                                                  cp->x=newsp->x+i;cp->y=newsp->y+j;cp->z=newsp->z+k;
                                                  CP->Add(cp);
                                                  MatrizRecorridos->SetV(newsp->x+i,newsp->y+j,newsp->z+k, 0);   //0: recorrido; 1: sin recorrer
					     	  }
                                                }
                                         }
		//--------------------------------------------------------------
	}

        delete CP;
}
//------------------------------------------------------------------------------

void getAllMatrixPlaneConnected(Punto* sp, Volumen *MatrizRecorridos, Volumen* Matriz, TList* AC, int thr)
{

//sp: seed-point: punto desde donde se toman todos los elementos conectados en el mismo plano:

//CP: Conjunto de puntos candidatos
//AC: Conjunto acumulativo de puntos conectados al sp
//PL: Conjunto de todos los puntos que satisfacen la condici�n dentro del volumen M
//    Finalmente, se elimina de PL todo elementos que est� en AC, funcionando como
//    una lista que agrupa todos los puntos NO conectados con sp.
//Matriz: matrices de 512*512*cantImagenes
//thr: Threshold: valor que debe cumplir un pixel para ser considerado de la misma
//     clase. Esta clase va a estar asociada con el valor dado por clasegrama.

        TList* CP = new TList;

        Punto* newsp = new Punto;

        CP->Add(sp);

        int maxX = Matriz->getAlto();
        int maxY = Matriz->getAncho();
        int maxZ = Matriz->getProf();

        int x,y,z;

	while(CP->Count!=0)
	{
                newsp = (Punto*)CP->First();
                AC->Add(newsp);
                CP->Delete(0);
                x=newsp->x; y=newsp->y; z=newsp->z;
                MatrizRecorridos->SetV(x,y,z,0);
                
		//CP = CP + NP{newsp}
		//-----------------------------------------------------------
		int i,j,k;
                k=0;
              //for(k=1;k>=-1;k--)
			for(i=-1;i<=1;i++)
				for(j=-1;j<=1;j++)
                                        if(!(i==0 && j==0 && k==0))
                                        {
                                                if(newsp->x+i>=0 && newsp->y+j>=0 && newsp->z+k>=0 && newsp->x+i<maxX && newsp->y+j<maxY && newsp->z+k<maxZ)
                                                {
                                                  if(Matriz->GetV(newsp->x+i,newsp->y+j,newsp->z+k)<=thr && MatrizRecorridos->GetV(newsp->x+i,newsp->y+j,newsp->z+k)==1)
			                	  {
                                                  Punto* cp = new Punto;
                                                  cp->x=newsp->x+i;cp->y=newsp->y+j;cp->z=newsp->z+k;
                                                  CP->Add(cp);
                                                  MatrizRecorridos->SetV(newsp->x+i,newsp->y+j,newsp->z+k, 0);  //0: recorrido; 1: sin recorrer
					     	  }
                                                }
                                         }
		//--------------------------------------------------------------
	}

        delete CP;
}

//------------------------------------------------------------------------------
void getAllMatrixSamePlaneConnected(Punto* sp, Volumen *MatrizRecorridos, Volumen* Matriz, TList* AC, int thr)
{

//sp: seed-point: punto desde donde se toman todos los elementos conectados en el mismo plano:

//CP: Conjunto de puntos candidatos
//AC: Conjunto acumulativo de puntos conectados al sp
//PL: Conjunto de todos los puntos que satisfacen la condici�n dentro del volumen M
//    Finalmente, se elimina de PL todo elementos que est� en AC, funcionando como
//    una lista que agrupa todos los puntos NO conectados con sp.
//Matriz: matrices de 512*512*cantImagenes
//thr: Threshold: valor que debe cumplir un pixel para ser considerado de la misma
//     clase. Esta clase va a estar asociada con el valor dado por clasegrama.

bool mayor = true;

if(sp->color < thr)
mayor = false;


        TList* CP = new TList;

        Punto* newsp = new Punto;

        CP->Add(sp);

        int maxX = Matriz->getAlto();
        int maxY = Matriz->getAncho();
        int maxZ = Matriz->getProf()*0.6;

        int x,y,z;

	while(CP->Count!=0)
	{
                newsp = (Punto*)CP->First();
                AC->Add(newsp);
                CP->Delete(0);
                x=newsp->x; y=newsp->y; z=newsp->z;
                MatrizRecorridos->SetV(x,y,z,0);
		//CP = CP + NP{newsp}
		//-----------------------------------------------------------
		int i,j,k;
                k=0;
              //for(k=1;k>=-1;k--)
			for(i=-1;i<=1;i++)
				for(j=-1;j<=1;j++)
                                        if(!(i==0 && j==0 && k==0))
                                        {
                                                if(newsp->x+i>=0 && newsp->y+j>=0 && newsp->z+k>=0 && newsp->x+i<maxX && newsp->y+j<maxY && newsp->z+k<maxZ)
                                                {
                                                  if(!mayor)
                                                  if(Matriz->GetV(newsp->x+i,newsp->y+j,newsp->z+k)<=thr && MatrizRecorridos->GetV(newsp->x+i,newsp->y+j,newsp->z+k)==1)
			                	  {
                                                  Punto* cp = new Punto;
                                                  cp->x=newsp->x+i;cp->y=newsp->y+j;cp->z=newsp->z+k;
                                                  CP->Add(cp);
                                                  MatrizRecorridos->SetV(newsp->x+i,newsp->y+j,newsp->z+k, 0);  //0: recorrido; 1: sin recorrer
						  //Matriz->SetV(newsp->x+i,newsp->y+j,newsp->z+k,maxPX);
					     	  }

                                                  if(mayor)
                                                  if(Matriz->GetV(newsp->x+i,newsp->y+j,newsp->z+k)>thr && MatrizRecorridos->GetV(newsp->x+i,newsp->y+j,newsp->z+k)==1)
			                	  {
                                                  Punto* cp = new Punto;
                                                  cp->x=newsp->x+i;cp->y=newsp->y+j;cp->z=newsp->z+k;
                                                  CP->Add(cp);
                                                  MatrizRecorridos->SetV(newsp->x+i,newsp->y+j,newsp->z+k, 0);  //0: recorrido; 1: sin recorrer
						  //Matriz->SetV(newsp->x+i,newsp->y+j,newsp->z+k,maxPX);
					     	  }

                                                }
                                         }
		//--------------------------------------------------------------
	}

        delete CP;
}
//------------------------------------------------------------------------------

void ConnectedComponentLabelling(Volumen* MatrizRecorridos, Volumen *Matriz, TList* ConnectedComponentList, int thr, bool d2_d3)
{
//Matriz Recorridos: 1- sin recorrer; 0- recorrido
//Matriz: matriz de datos en HU
//MatrizRecorrido y Matriz deben tener, idealmente, el mismo tama�o.
//d2_d3: busca componentes conexos en el plano(2D) o volum�tricos (3D): true = 2D, false = 3D.

//ConnectedComponentList: lista de elemenentos conectados por el mismo color (encima o debajo del umbral 'thr')
//thr: umbral que define los elementos conectados

  Punto* sp; //seed point.
  TList* AC; //All Connected List

 for (int k=0; k<Matriz->getProf()*0.6; k++)
  for(int i=0; i<Matriz->getAlto(); i++)
   for(int j=0; j<Matriz->getAncho(); j++)
    {
     if( MatrizRecorridos->GetV(i, j, k)== 1)
     {
      sp = new Punto;
      sp->x = i; sp->y = j; sp->z = k;
      sp->color = Matriz->GetV(sp->x, sp->y, sp->z);

      if(d2_d3)
      {
      AC = new TList;
      getAllMatrixSamePlaneConnected(sp, MatrizRecorridos, Matriz, AC, thr);
      ConnectedComponentList->Add(AC);
      }
      else
      {
       AC = new TList;
       getAllMatrixVolumeConnected(sp, MatrizRecorridos, Matriz, AC, thr);
       ConnectedComponentList->Add(AC);
      }
     }
   }

}



//---------------------------------------------------------------------------
void getAllConnected(Punto* sp, Volumen* M, TList* PL, TList* AC)
{
//sp: Seeding Point
//CP: Conjunto de puntos candidatos
//AC: Conjunto acumulativo de puntos conectados al sp
//PL: Conjunto de todos los puntos que satisfacen la condici�n dentro del volumen M
//    Finalmente, se elimina de PL todo elementos que est� en AC, funcionando como
//    una lista que agrupa todos los puntos NO conectados con sp.    

        TList* CP = new TList;

        int maxX = M->getAlto();
        int maxY = M->getAncho();
        int maxZ = M->getProf();

        Punto* newsp = new Punto;

        CP->Add(sp);

	while(CP->Count!=0)
	{
                newsp = (Punto*)CP->First();
                AC->Add(newsp);
                CP->Delete(0);
                int x,y,z; x=newsp->x; y=newsp->y; z=newsp->z;
                M->SetV(x,y,z,0);

		//CP = CP + NP{newsp}
		//-----------------------------------------------------------
		int i,j,k;
        	for(k=1;k>=-1;k--)
			for(i=-1;i<=1;i++)
				for(j=-1;j<=1;j++)
                                        if(!(i==0 && j==0 && k==0))
                                        {
                                                if(newsp->x+i>=0 && newsp->y+j>=0 && newsp->z+k>=0 && newsp->x+i<maxX && newsp->y+j<maxY && newsp->z+k<maxZ)
						if(M->GetV(newsp->x+i,newsp->y+j,newsp->z+k)==1)
			                	{
                                                Punto* cp = new Punto;
                                                cp->x=newsp->x+i;cp->y=newsp->y+j;cp->z=newsp->z+k;
                                                CP->Add(cp);
						M->SetV(newsp->x+i,newsp->y+j,newsp->z+k,0);
					     	}
                                         }
		//--------------------------------------------------------------
	}

        Punto* p=new Punto;

        for(int i=0; i<PL->Count;i++)
        {
               p = (Punto*)PL->Items[i];

               if(M->GetV(p->x,p->y,p->z)==0)
               {
               //delete p;
               PL->Delete(i);
               i--;
               }
        }

        delete CP;
}
//------------------------------------------------------------------------------
void getAllMatrixConnected(Punto* sp, Volumen *Matriz, TList* AC, int thr, short int minPX)
{
//sp: Seeding Point
//CP: Conjunto de puntos candidatos
//AC: Conjunto acumulativo de puntos conectados al sp
//PL: Conjunto de todos los puntos que satisfacen la condici�n dentro del volumen M
//    Finalmente, se elimina de PL todo elementos que est� en AC, funcionando como
//    una lista que agrupa todos los puntos NO conectados con sp.
//Matriz: matrices de 512*512*cantImagenes
//thr: Threshold: valor que debe cumplir un pixel para ser considerado de la misma
//     clase.

    TList* CP = new TList;
    Punto* newsp = new Punto;

    CP->Add(sp);

    int maxX = Matriz->getAlto();
    int maxY = Matriz->getAncho();
    int maxZ = Matriz->getProf();

    int i,j,k;

    int l;

    int deltas[6][3] = { {1,0,0}, {-1,0,0}, {0,1,0}, {0,-1,0}, {0,0,1}, {0,0,-1} };

	while(CP->Count!=0)
	{
        newsp = (Punto*)CP->First();
        AC->Add(newsp);
        CP->Delete(0);
        int x,y,z; x=newsp->x; y=newsp->y; z=newsp->z;
        Matriz->SetV(x,y,z,minPX);

        for(l=0; l<6; l++)
        {
        i = deltas[l][0];
        j = deltas[l][1];
        k = deltas[l][2];

        if(newsp->x+i>=0 && newsp->y+j>=0 && newsp->z+k>=0
           &&
           newsp->x+i<maxX && newsp->y+j<maxY && newsp->z+k<maxZ)
		    if(Matriz->GetV(newsp->x+i,newsp->y+j,newsp->z+k)>thr)
			{
            Punto* cp = new Punto;
            cp->x=newsp->x+i;cp->y=newsp->y+j;cp->z=newsp->z+k;
            CP->Add(cp);
			Matriz->SetV(newsp->x+i,newsp->y+j,newsp->z+k,  thr);
		    }

        }


	}

        delete CP;
}
//------------------------------------------------------------------------------

void getBorderPoints(Volumen* M, TList* AC, TList* BAC)
{
//M: volumen: solo se utiliza para obtener los constraints de la caja.
//AC-> todos los puntos conectados a algun seed point (sp).
//BAC-> elementos de AC que
        int xf = M->getAlto()-1;
        int yf = M->getAncho()-1;
        int zf = M->getProf()-1;

         Punto* cp = new Punto;

        for(int i=0; i<AC->Count; i++)
        {
        cp = (Punto*)AC->Items[i];

          if(cp->x==0 || cp->x==xf || cp->y == 0 || cp->y == yf || cp->z == 0 || cp->z == zf)
          {
          //Punto* p = new Punto;
          //p->x=cp->x;p->y=cp->y;p->z=cp->z;
          BAC->Add(cp);
         // delete cp;
          AC->Delete(i);
          i--;
          }
        }
}
//------------------------------------------------------------------------------
int SurfacicGroups(Volumen* M, TList* BAC, TList* Grupos)
{
        //void getAllConnected(Punto* sp, Volumen* M, TList* PL, TList* AC, TList* NC)

          int alto, ancho, prof;
          alto = M->getAlto();
          ancho = M->getAncho();
          prof = M->getProf();

          Volumen* SubM = new Volumen(alto,ancho,prof);

          Punto* cp = new Punto;

          for(int i=0; i<BAC->Count;i++)
          {
          cp = (Punto*)BAC->Items[i];
          SubM->SetV(cp->x,cp->y,cp->z,1); //SubM: Matriz binaria.
          }

          Punto* p1 = new Punto;

          while(BAC->Count!=0)
          {
          p1 = (Punto*)BAC->First();

         getAllConnected(p1, SubM, BAC, Grupos);
         Punto* psep = new Punto;
         psep->x=-1; psep->y=-1; psep->z=-1;//psep: Punto separador. Tiene coordenadas fuera de la
         Grupos->Add(psep);                 //caja, indicando la finalizacion de un grupo conexo.
          }

          delete SubM;

          return 0;
}
//------------------------------------------------------------------------------

bool SurfacicLabeling(Volumen* M, TList* Grupos)
{
//                  Si un grupo pertenece a una cara, a esa cara se le asigna un
//      --------    valor distinto de 0.
//      |       |
//      |       |
//      --------
//                 aL: arrayLados: 1-frente 2-fondo 3-izq 4-der 5-abajo 6-arriba
//                     decisi�n: nand(0,1)*nand(2,3)*nand(4,5) = 1 (caja correcta)
//                                                               0 (agrandar caja)
        int xf = M->getAlto()-1;
        int yf = M->getAncho()-1;
        int zf = M->getProf()-1;

      int aL[6]={0,0,0,0,0,0}; //arrayLados
      int h = 0; //lleva registro del reccorrido de Grupos
      Punto* p = new Punto;
      bool es=true;

      //if(cp->x==0 || cp->x==xf || cp->y == 0 || cp->y == yf || cp->z == 0 || cp->z == zf)

      while(h<Grupos->Count) //-1: evita el ultima punto separador
      {
        p = (Punto*)Grupos->Items[h];

        if(p->x!=-1)  //Busca el puntoseparador
        {
         if(p->x==0)
         aL[0]=1;     //frente
         if(p->x==xf)
         aL[1]=1;    //fondo

         if(p->y==0)
         aL[2]=1;     //izq
         if(p->y==yf)
         aL[3]=1;    //der

         if(p->z==0)
         aL[4]=1;     //abajo
         if(p->z==zf)
         aL[5]=1;    //arriba

         h++;
        }  else {

                bool a1, a2, a3;
                     a1=1-(aL[0]*aL[1]); //nand = 1-and
                     a2=1-(aL[2]*aL[3]);
                     a3=1-(aL[4]*aL[5]);

                es = a1*a2*a3;

                if(es == 0)//Si algun elemento est� en paredes opuestas:
                return es;

                h++;
                //Limpia aL;
                aL[0]=0; aL[1]=0; aL[2]=0; aL[3]=0; aL[4]=0; aL[5]=0;
                }
      }

        return es;
}
//------------------------------------------------------------------------------
void getAllPlaneConnected(Punto* fsp, Volumen* M, TList* PL, TList* AC, TProgressBar* barra)
{
//sp: Seeding Point
//CP: Conjunto de puntos candidatos
//AC: Conjunto acumulativo de puntos conectados al sp
//PL: Conjunto de todos los puntos que satisfacen la condici�n dentro de la caja
//    Finalmente, se elimina de PL todo elementos que est� en AC, funcionando como
//    una lista que agrupa todos los puntos NO conectados con sp.

        TList* CP = new TList;

        //CAMBIAR EN GETALLCONNECTED!!!!: 'x' es alto, 'y' es ancho
        int maxX = M->getAlto();
        int maxY = M->getAncho();
        int maxZ = M->getProf();

        Punto* newsp = new Punto;

     barra->Max=maxZ;barra->Position=0;

     for(int plano=0;plano<maxZ;plano++)
     {barra->Position++;

        Punto* sp = new Punto;
        sp->x=fsp->x; sp->y=fsp->y; sp->z=plano;
        CP->Add(sp);

	while(CP->Count!=0)
	{
                newsp = (Punto*)CP->First();
                AC->Add(newsp);
                CP->Delete(0);
                int x,y,z; x=newsp->x; y=newsp->y; z=newsp->z;
                M->SetV(x,y,z,0);

		//CP = CP + NP{newsp}
		//-----------------------------------------------------------
		int i,j;
        		for(i=-1;i<=1;i++)
				for(j=-1;j<=1;j++)
                                        if(!(i==0 && j==0))
                                        {
                                                if(newsp->x+i>=0 && newsp->y+j>=0 && newsp->z>=0 && newsp->x+i<maxX && newsp->y+j<maxY && newsp->z<maxZ)
						if(M->GetV(newsp->x+i,newsp->y+j,newsp->z)==1)
			                	{
                                                Punto* cp = new Punto;
                                                cp->x=newsp->x+i;cp->y=newsp->y+j;cp->z=newsp->z;
                                                CP->Add(cp);
						M->SetV(newsp->x+i,newsp->y+j,newsp->z,0);
					     	}
                                         }
		//--------------------------------------------------------------
	}//end while(CP->Count==0)
    }//end for plano

      /*  Punto* p=new Punto;

        for(int i=0; i<PL->Count;i++)
        {
               p = (Punto*)PL->Items[i];

               if(M->GetV(p->x,p->y,p->z)==0)
               {
               //delete p;
               PL->Delete(i);
               i--;
               }
        }
       */
       int i,j,k;

      PL->Clear();

      barra->Position=0;

      for(k=0;k<maxZ;k++)
      {barra->Position++;
       for(i=0; i<maxX; i++)
        for(j=0; j<maxY;j++)
          if(M->GetV(i,j,k)==1)
          {
          Punto* p = new Punto;
          p->x=i; p->y=j; p->z=k;
          PL->Add(p);
          }
      }
        delete CP;
}
//------------------------------------------------------------------------------

void getAllMatrixRangeConnected(Punto* sp, Volumen* Matriz, TList* AC, int center, int width, int cantImagenes)
{
//Funci�n: similar a getAllMatrixConnected, pero la conectividad se asocia a un
//         valor central +- un desv�o
//sp: Seeding Point
//CP: Conjunto de puntos candidatos
//AC: Conjunto acumulativo de puntos conectados al sp

//Matriz: matrices de 512*512*cantImagenes
//center: valor central en HU
//width: +- desv�o respecto a center en HU cualquier pixel debe cumplir estar en
//       el rango center +- width para ser considerado vecino.

        int PrimerElemento = AC->Count;

        TList* CP = new TList;

        Punto* newsp = new Punto;

        CP->Add(sp);

        int maxX, maxY, maxZ;
        maxX=512;
        maxY=512;
        maxZ=cantImagenes;
        int vuelta = 0;

	while(CP->Count!=0)
	{
                newsp = (Punto*)CP->First();

                if(vuelta!=0)
                AC->Add(newsp);

                CP->Delete(0);
                //int x,y,z; x=newsp->x; y=newsp->y; z=newsp->z;
                //Matriz[x][y][z] = Matriz[x][y][z]-2048;
		//CP = CP + NP{newsp}
		//-----------------------------------------------------------
		int i,j,k;
        	for(k=1;k>=-1;k--)
			for(i=-1;i<=1;i++)
				for(j=-1;j<=1;j++)
                                        if(!(i==0 && j==0 && k==0))
                                        {
                                                if(newsp->x+i>=0 && newsp->y+j>=0 && newsp->z+k>=0
                                                && newsp->x+i<maxX && newsp->y+j<maxY && newsp->z+k<maxZ)
						if(Matriz->GetV(newsp->x+i,newsp->y+j,newsp->z+k)<=center+width &&
                                                   Matriz->GetV(newsp->x+i,newsp->y+j,newsp->z+k)>=center-width)
			                	{
                                                Punto* cp = new Punto;
                                                cp->x=newsp->x+i;cp->y=newsp->y+j;cp->z=newsp->z+k;
                                                cp->color=Matriz->GetV(cp->x,cp->y,cp->z);
                                                CP->Add(cp);

                                                Matriz->SetV
                                                (newsp->x+i,newsp->y+j,newsp->z+k,
                                                Matriz->GetV(newsp->x+i,newsp->y+j,newsp->z+k)
                                                - 2048);
					     	}
                                         }
		//--------------------------------------------------------------
                 if(AC->Count>2000000)
                 break;
                 vuelta++;
	}


        for(int i=PrimerElemento; i<AC->Count;i++)
        {
        newsp = (Punto*)AC->Items[i];
        Matriz->SetV(newsp->x,newsp->y,newsp->z,Matriz->GetV(newsp->x,newsp->y,newsp->z)+2048);
        }

        delete CP;
}
//------------------------------------------------------------------------------

void getAllMatrixRangeConstraintedConnected
(Punto* sp, Volumen* Matriz, TList* AC, int center, int width, int cantImagenes,
int cxm, int cxM, int cym, int cyM, int czm, int czM)
{
//Funci�n: similar a getAllMatrixRangeConnected, pero la conectividad se busca
//         dentro de una subregi�n de la Matriz
//sp: Seeding Point
//CP: Conjunto de puntos candidatos
//AC: Conjunto acumulativo de puntos conectados al sp
//PL: Conjunto de todos los puntos que satisfacen la condici�n dentro de la caja
//    Finalmente, se elimina de PL todo elementos que est� en AC, funcionando como
//    una lista que agrupa todos los puntos NO conectados con sp.
//Matriz: matrices de 512*512*cantImagenes
//center: valor central en HU
//width: +- desv�o respecto a center en HU cualquier pixel debe cumplir estar en
//       el rango center +- width para ser considerado vecino.
//cim: menor valor dentro de los que se debe buscar la conectividad en la direcci�n "i"
//ciM: mayor valor " ...

        int PrimerElemento = AC->Count;

        TList* CP = new TList;

        Punto* newsp = new Punto;

        CP->Add(sp);

        int vuelta = 0;

	while(CP->Count!=0)
	{
                newsp = (Punto*)CP->First();

                if(vuelta!=0)
                AC->Add(newsp);

                CP->Delete(0);
                //int x,y,z; x=newsp->x; y=newsp->y; z=newsp->z;
                //Matriz[x][y][z] = Matriz[x][y][z]-2048;
		//CP = CP + NP{newsp}
		//-----------------------------------------------------------
		int i,j,k;
        	for(k=1;k>=-1;k--)
			for(i=-1;i<=1;i++)
				for(j=-1;j<=1;j++)
                                        if(!(i==0 && j==0 && k==0))
                                        {
                                                if(newsp->x+i>=cxm && newsp->y+j>=cym && newsp->z+k>=czm
                                                && newsp->x+i<cxM && newsp->y+j<cyM && newsp->z+k<czM)
						if(Matriz->GetV(newsp->x+i,newsp->y+j,newsp->z+k)<=center+width &&
                                                   Matriz->GetV(newsp->x+i,newsp->y+j,newsp->z+k)>=center-width)
			                	{
                                                Punto* cp = new Punto;
                                                cp->x=newsp->x+i;cp->y=newsp->y+j;cp->z=newsp->z+k;
                                                cp->color = Matriz->GetV(cp->x,cp->y,cp->z);
                                                CP->Add(cp);

                                                Matriz->SetV(newsp->x+i,newsp->y+j,newsp->z+k,Matriz->GetV(newsp->x+i,newsp->y+j,newsp->z+k)-2048);
					     	}
                                         }
		//--------------------------------------------------------------
                // if(AC->Count>2000000)
                // break;
                 vuelta++;
	}

        for(int i=PrimerElemento; i<AC->Count;i++)
        {
        newsp = (Punto*)AC->Items[i];
        Matriz->SetV(newsp->x,newsp->y,newsp->z,Matriz->GetV(newsp->x,newsp->y,newsp->z)+2048);
        }

        delete CP;
}
//------------------------------------------------------------------------------
void getAllPlaneConnected(Punto* sp, Volumen* M, TList* AC)
{
//Devuelve todos los puntos conectados dentro de un plano.
//M: volumen de puntos. Binario: 1: de inter�s, 0: no de inter�s.
//sp: seeding point.
//PL: Inicialmente, todos puntos de posible inter�s dentro del plano (1s).
//    Finalmente, PL := PL - AC, guardando asi todos los puntos NO conectados a sp.
//AC: todos los puntos del plano conectados a fsp.

        //CP: connected points. Guarda temporalmente los puntos conectados.
        TList* CP = new TList;

        //L�mites del plano...
        int maxX = M->getAlto();
        int maxY = M->getAncho();

        //Puntero a Punto auxiliar, para ser el nuevo seeding point
        Punto* newsp;

        //Guarda el primer seeding point. 
        CP->Add(sp);

        //Puntero auxiliar para crear nuevos puntos para la lista CP
        Punto* cp;

        //Al no haber variaci�n en 'z', los puntos encontrados solo pueden
        //residir dentro del plano donde incialmente est� ubicado sp. 
	while(CP->Count!=0)
	{
                //Pasa el seeding point a AllConnected(AC) List
                newsp = (Punto*)CP->First();
                AC->Add(newsp);
                CP->Delete(0);

                int x,y,z; x=newsp->x; y=newsp->y; z=newsp->z;
                M->SetV(x,y,z,0);

		//CP = [CP U NP{newsp}]
		//-----------------------------------------------------------
		int i,j;
        		for(i=-1;i<=1;i++)
				for(j=-1;j<=1;j++)
                                        if(!(i==0 && j==0))
                                        {
                                                if(newsp->x+i>=0 && newsp->y+j>=0 && newsp->x+i<maxX && newsp->y+j<maxY)
						if(M->GetV(newsp->x+i,newsp->y+j,newsp->z)==1)
			                	{
                                                cp = new Punto;
                                                cp->x=newsp->x+i;cp->y=newsp->y+j;cp->z=newsp->z;
                                                CP->Add(cp);
						M->SetV(newsp->x+i,newsp->y+j,newsp->z,0);
					     	}
                                         }
		//--------------------------------------------------------------
	}

        //Se puede borrar CP directamente, por haber sido este vaciado
        //durante el transcurso del algoritmo.

        delete CP;
}
//******************************************************************************


