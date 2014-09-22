bool all(int* array, int tamanio)
{

int contador=0;
bool es;

	for(int i=0;i<tamanio;i++)
	{
	contador = contador+array[i]; //devuelve valor>0 si algún elemento es distinto de 0.
	}

	if(contador==0)
	{
	es=TRUE;
	return es;
	}
	
	es=FALSE;
	return es;
//NOTA: podría ponerse un punto de return en el caso que encuentre un 1 prematuramente. Evaluar el ahorro de tiempo.
}
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
bool xor(bool a, bool b)
{
return ((a==TRUE || b==TRUE)-(a==TRUE && b==true));

}
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
int suma(int* NP)
{
int suma=0;
	for(int i=0;i<26;i++)
	{
	suma=suma+NP[i];
	}

return suma;
}
//---------------------------------------------------------------------------------------------------------------------
int suma(bool* NP)
{
int suma=0;
	for(int i=0;i<26;i++)
	{
	suma=suma+int(NP[i]);
	}

return suma;
}
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
void collectGP(Volumen* M,float p2[],int* G)
{
//Gp: Vecindad (NP) ampliada. 
//GP = [A a1 a2 a3 a4 a5 a6 a7 a8 a9 a10 a11 a12 a13 B b1 b2 b3 b4 b6 b6 C D]
int p[3];
p[0]=int(p2[0]);
p[1]=int(p2[1]);
p[2]=int(p2[2]);

G[0]=M->GetV(p[0]+2,p[1],p[2]);
G[1]=M->GetV(p[0]+2,p[1],p[2]+1);
G[2]=M->GetV(p[0]+2,p[1],p[2]+2);
G[3]=M->GetV(p[0]+1,p[1],p[2]+2);
G[4]=M->GetV(p[0]+2,p[1],p[2]-1);
G[5]=M->GetV(p[0]+2,p[1],p[2]-2);
G[6]=M->GetV(p[0]+1,p[1],p[2]-2);
G[7]=M->GetV(p[0]+2,p[1]-1,p[2]);
G[8]=M->GetV(p[0]+2,p[1]-2,p[2]);
G[9]=M->GetV(p[0]+1,p[1]-2,p[2]);
G[10]=M->GetV(p[0]+2,p[1]+1,p[2]);
G[11]=M->GetV(p[0]+2,p[1]+2,p[2]);
G[12]=M->GetV(p[0],p[1]+2,p[2]);
G[13]=M->GetV(p[0]-1,p[1]+2,p[2]);
G[14]=M->GetV(p[0],p[1]-2,p[2]);
G[15]=M->GetV(p[0],p[1]-2,p[2]+1);
G[16]=M->GetV(p[0],p[1]-2,p[2]+2);
G[17]=M->GetV(p[0],p[1]-1,p[2]+2);
G[18]=M->GetV(p[0],p[1]-2,p[2]-1);
G[19]=M->GetV(p[0],p[1]-2,p[2]-2);
G[20]=M->GetV(p[0],p[1]-1,p[2]-2);
G[21]=M->GetV(p[0],p[1],p[2]+2);
G[22]=M->GetV(p[0],p[1],p[2]-2);
}
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
void collect26neigh(float* p, Volumen* M, int* NP)
{
int j = 0;

int px=int(p[0]);
int py=int(p[1]);
int pz=int(p[2]);

int x,y,z;

        for(z=1;z>=-1;z--)
        {
	for(x=-1;x<=1;x++)
		for(y=-1;y<=1;y++)
		{
			if(!(x==0 && y==0 && z==0))
			{
			NP[j]=M->GetV(px+x,py+y,pz+z);
			j++;
                        }
		}
        }
}
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
void collect26neigh(Punto* p, bool*** M, bool* NP)
{
int j = 0;

int px=p->x;
int py=p->y;
int pz=p->z;

int x,y,z;

        for(z=1;z>=-1;z--)
        {
	for(x=-1;x<=1;x++)
		for(y=-1;y<=1;y++)
		{
			if(!(x==0 && y==0 && z==0))
			{
			NP[j]=M[px+x][py+y][pz+z];
			j++;
                        }
		}
        }
}
//------------------------------------------------------------------------------
void  COLLECT_26NEIGH_COORDS(bool* NP,Punto* p, TList* NewP)
{
     Punto* np;

     if(NP[0])
     {
     np = new Punto;
     np->x = p->x-1; np->y = p->y-1; np->z = p->z+1; //p(1)-1,p(2)-1,p(3)+1
     NewP->Add(np);
     }

     if(NP[1])
     {
     np = new Punto;
     np->x = p->x-1; np->y = p->y; np->z = p->z+1; //p(1)-1,p(2),p(3)+1; ...
     NewP->Add(np);
     }

     if(NP[2])
     {
     np = new Punto;
     np->x = p->x-1; np->y = p->y+1; np->z = p->z+1; //p(1)-1,p(2)+1,p(3)+1; ...
     NewP->Add(np);
     }


     if(NP[3])
     {
     np = new Punto;
     np->x = p->x; np->y = p->y-1; np->z = p->z+1; // p(1),p(2)-1,p(3)+1; ...
     NewP->Add(np);
     }

     if(NP[4])
     {
     np = new Punto;
     np->x = p->x; np->y = p->y; np->z = p->z+1; // p(1),p(2),p(3)+1; ...
     NewP->Add(np);
     }

     if(NP[5])
     {
     np = new Punto;
     np->x = p->x; np->y = p->y+1; np->z = p->z+1;  //p(1),p(2)+1,p(3)+1; ...
     NewP->Add(np);
     }

     if(NP[6])
     {
     np = new Punto;
     np->x = p->x+1; np->y = p->y-1; np->z = p->z+1; //p(1)+1,p(2)-1,p(3)+1; ...
     NewP->Add(np);
     }

     if(NP[7])
     {
     np = new Punto;
     np->x = p->x+1; np->y = p->y; np->z = p->z+1; //p(1)+1,p(2),p(3)+1; ...
     NewP->Add(np);
     }

     if(NP[8])
     {
     np = new Punto;
     np->x = p->x+1; np->y = p->y+1; np->z = p->z+1; //p(1)+1,p(2)+1,p(3)+1; ...
     NewP->Add(np);
     }

     if(NP[9])
     {
     np = new Punto;
     np->x = p->x-1; np->y = p->y-1; np->z = p->z; //p(1)-1,p(2)-1,p(3); ...
     NewP->Add(np);
     }

     if(NP[10])
     {
     np = new Punto;
     np->x = p->x-1; np->y = p->y; np->z = p->z; // p(1)-1,p(2),p(3); ...
     NewP->Add(np);
     }

     if(NP[11])
     {
     np = new Punto;
     np->x = p->x-1; np->y = p->y+1; np->z = p->z; //p(1)-1,p(2)+1,p(3); ...
     NewP->Add(np);
     }

     if(NP[12])
     {
     np = new Punto;
     np->x = p->x; np->y = p->y-1; np->z = p->z;  // p(1),p(2)-1,p(3); ...
     NewP->Add(np);
     }

     if(NP[13])
     {
     np = new Punto;
     np->x = p->x; np->y = p->y+1; np->z = p->z; // p(1),p(2)+1,p(3); ...
     NewP->Add(np);
     }

     if(NP[14])
     {
     np = new Punto;
     np->x = p->x+1; np->y = p->y-1; np->z = p->z; // p(1)+1,p(2)-1,p(3); ...
     NewP->Add(np);
     }

     if(NP[15])
     {
     np = new Punto;
     np->x = p->x+1; np->y = p->y; np->z = p->z; // p(1)+1,p(2),p(3); ...
     NewP->Add(np);
     }

     if(NP[16])
     {
     np = new Punto;
     np->x = p->x+1; np->y = p->y+1; np->z = p->z; // p(1)+1,p(2)+1,p(3); ...
     NewP->Add(np);
     }

     if(NP[17])
     {
     np = new Punto;
     np->x = p->x-1; np->y = p->y-1; np->z = p->z-1; //p(1)-1,p(2)-1,p(3)-1; ...
     NewP->Add(np);
     }

     if(NP[18])
     {
     np = new Punto;
     np->x = p->x-1; np->y = p->y; np->z = p->z-1;  //p(1)-1,p(2),p(3)-1; ...
     NewP->Add(np);
     }

     if(NP[19])
     {
     np = new Punto;
     np->x = p->x-1; np->y = p->y+1; np->z = p->z-1; //p(1)-1,p(2)+1,p(3)-1; ...
     NewP->Add(np);
     }

     if(NP[20])
     {
     np = new Punto;
     np->x = p->x; np->y = p->y-1; np->z = p->z-1; //p(1),p(2)-1,p(3)-1; ...
     NewP->Add(np);
     }

     if(NP[21])
     {
     np = new Punto;
     np->x = p->x; np->y = p->y; np->z = p->z-1; //p(1),p(2),p(3)-1; ...
     NewP->Add(np);
     }

     if(NP[22])
     {
     np = new Punto;
     np->x = p->x; np->y = p->y+1; np->z = p->z-1; //p(1),p(2)+1,p(3)-1; ...
     NewP->Add(np);
     }

     if(NP[23])
     {
     np = new Punto;
     np->x = p->x+1; np->y = p->y-1; np->z = p->z-1; //p(1)+1,p(2)-1,p(3)-1; ...
     NewP->Add(np);
     }

     if(NP[24])
     {
     np = new Punto;
     np->x = p->x+1; np->y = p->y; np->z = p->z-1;  //p(1)+1,p(2),p(3)-1; ...
     NewP->Add(np);
     }

     if(NP[25])
     {
     np = new Punto;
     np->x = p->x+1; np->y = p->y+1; np->z = p->z-1; //  p(1)+1,p(2)+1,p(3)-1];
     NewP->Add(np);
     }
}
//------------------------------------------------------------------------------
void  COLLECT_6NEIGH_COORDS(bool* NP,Punto* p, TList* NewP)
{
     Punto* np;

     if(NP[4]==1)
     {
     np = new Punto;
     np->x = p->x; np->y = p->y; np->z = p->z+1; // p(1),p(2),p(3)+1; ... %U
     NewP->Add(np);
     }

     if(NP[10]==1)
     {
     np = new Punto;
     np->x = p->x-1; np->y = p->y; np->z = p->z; // p(1)-1,p(2),p(3); ... %N
     NewP->Add(np);
     }

     if(NP[12]==1)
     {
     np = new Punto;
     np->x = p->x; np->y = p->y-1; np->z = p->z;  // p(1),p(2)-1,p(3); ...  %W
     NewP->Add(np);
     }

     if(NP[13]==1)
     {
     np = new Punto;
     np->x = p->x; np->y = p->y+1; np->z = p->z; // p(1),p(2)+1,p(3); ...  %E
     NewP->Add(np);
     }

     if(NP[15]==1)
     {
     np = new Punto;
     np->x = p->x+1; np->y = p->y; np->z = p->z; // p(1)+1,p(2),p(3); ...  %S
     NewP->Add(np);
     }

     if(NP[21]==1)
     {
     np = new Punto;
     np->x = p->x; np->y = p->y; np->z = p->z-1; //p(1),p(2),p(3)-1; ... %D
     NewP->Add(np);
     }

}
//------------------------------------------------------------------------------
void sumar(float* X,int sumando, int tamanio)
{
	for(int i=0;i<tamanio;i++)
	{
	X[i]=X[i]+sumando;
	}
}

//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
bool IS_EXTRM_POINT(int* NP)
{
bool es = FALSE;

	if(suma(NP)==1)
	{
    	es = TRUE;
    	return es;
	}

return es;
}
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------

bool IS_NEARLY_EXTRM_POINT(int* NP,Volumen* M, float* p)
{
bool es = FALSE;

if (suma(NP)==2)
{
int NPN[26];
int NPS[26];
int NPE[26];
int NPW[26];
int NPU[26];
int NPD[26];

float pN[]={p[0]-1,p[1],p[2]};
float pS[]={p[0]+1,p[1],p[2]};
float pE[]={p[0],p[1]+1,p[2]};
float pW[]={p[0],p[1]-1,p[2]};
float pU[]={p[0],p[1],p[2]+1};
float pD[]={p[0],p[1],p[2]-1};

collect26neigh(pN,M,NPN);
collect26neigh(pS,M,NPS);
collect26neigh(pE,M,NPE);
collect26neigh(pW,M,NPW);
collect26neigh(pU,M,NPU);
collect26neigh(pD,M,NPD);

    //NOTA: considerar el cambio en "1"
    //Interpretación 1:

	if ((NP[16-1]==1 && NP[14-1]==1) && xor(IS_EXTRM_POINT(NPS),IS_EXTRM_POINT(NPE))||
       (NP[16-1]==1 &&  NP[5-1]==1) && xor(IS_EXTRM_POINT(NPS),IS_EXTRM_POINT(NPU))||
       (NP[11-1]==1 && NP[13-1]==1) && xor(IS_EXTRM_POINT(NPN),IS_EXTRM_POINT(NPW))||
       (NP[5-1]==1  && NP[13-1]==1) && xor(IS_EXTRM_POINT(NPU),IS_EXTRM_POINT(NPW))||
       (NP[11-1]==1 && NP[22-1]==1) && xor(IS_EXTRM_POINT(NPN),IS_EXTRM_POINT(NPD))||
       (NP[14-1]==1 && NP[22-1]==1) && xor(IS_EXTRM_POINT(NPE),IS_EXTRM_POINT(NPD)))
	{
    /*
    //Interpretación 2:
    if ((NP[16-1]==1 && NP[14-1]==1)||(NP[16-1]==1 && NP[5-1]==1)||
       (NP[11-1]==1 && NP[13-1]==1)||(NP[5-1]==1 && NP[13-1]==1)||
       (NP[11-1]==1 && NP[22-1]==1)||(NP[14-1]==1 && NP[22-1]==1))
       {
       */

    es = true;
    return es;

	}

}

return es;
}
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
bool is_tail_point(int* NP,Volumen* M, float* p)
{
bool es = FALSE;

	if (IS_EXTRM_POINT(NP) || IS_NEARLY_EXTRM_POINT(NP,M,p))
	{
    	es = TRUE;
    	return es;
	}
return es;
}
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
/*
int buscaMax(float* X, int tamanio)
{
int max=0;

for(int i=0; i<tamanio; i++)
	if(X[i]>max)
		max=X[i];

return max;

}
*/
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
/*
int buscaMin(float* X, int tamanio)
{
int min=buscaMax(X,tamanio);

for(int i=0; i<tamanio; i++)
	if(X[i]<min)
		min=X[i];

return min;
}
*/
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
bool collect26neighConUmbral(float* p, Volumen* M, float umbral)
{

int px=int(p[0]);
int py=int(p[1]);
int pz=int(p[2]);

int x,y,z;

        for(z=1;z>=-1;z--)
	  for(x=-1;x<=1;x++)
		for(y=-1;y<=1;y++)
			if(!(x==0 && y==0 && z==0))
			{
                          if(M->GetV(px+x,py+y,pz+z)<=umbral)
			  return true; //Es un punto de borde: tiene algún vecino con valor umbral
                        }

                        return false; //No es un punto de borde: no tiene ningun vecino con valor umbral
}
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
bool Aislado(float* p, Volumen* M, float umbral)
{

int px=int(p[0]);
int py=int(p[1]);
int pz=int(p[2]);

int x,y,z;

        for(z=1;z>=-1;z--)
	  for(x=-1;x<=1;x++)
		for(y=-1;y<=1;y++)
			if(!(x==0 && y==0 && z==0))
			{
                          if(M->GetV(px+x,py+y,pz+z)>umbral)
			  return false; //No es un punto aislado: tiene algún vecino con valor >umbral
                        }

                        return true; //Es un punto de aislado: No tiene ningún vecino con valor >umbral
}
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

 