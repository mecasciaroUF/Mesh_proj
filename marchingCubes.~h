#include "edgeTable.h"

void marchingCubes(short int stepx, short int stepy, short int stepz,
                   short int maxX, short int maxY, short int maxZ,
                   short int minX, short int minY, short int minZ,
                   bool*** M, TList* TL)
{

//stepx,y,z: tamaño del cubo que marcha para renderizar
//Matriz M
//TL: triangle list

      unsigned char vertices = 0;
    //  int triangIndex = 0;
      int edges[16];
      punto* vt;

      //desplaz: calcula el desplazamiento del marching cube:

      float desplaz[12][3] =
      {
       {0, float(stepy)*0.5, 0},
       {float(stepx)*0.5, stepy, 0},
       {stepx, 0.5*float(stepy), 0},
       {0.5*float(stepx), 0, 0},
       {0, 0.5*float(stepy), stepz},
       {0.5*float(stepx), stepy, stepz},
       {stepx, 0.5*float(stepy), stepz},
       {0.5*float(stepx), 0, stepz},
       {0, 0, 0.5*float(stepz)},
       {0, stepy, 0.5*float(stepz)},
       {stepx, stepy, 0.5*float(stepz)},
       {stepx, 0, 0.5*float(stepz)}
      };

      float dx,dy,dz;

      for (int i=0; i<maxX-minX+2; i=i+stepx)
       for (int j=0; j<maxY-minY+2; j=j+stepy)
        for (int k=0; k<maxZ-minZ+2; k=k+stepz)
           {
           vertices = 0;

           if(M[i][j][k]==1) vertices |=1 ;

           if(j+stepy<maxY-minY+2)
           if(M[i][j+stepy][k]==1) vertices |=2;

           if(j+stepy<maxY-minY+2 && i+stepx<maxX-minX+2)
           if(M[i+stepx][j+stepy][k]==1) vertices |=4;

           if(i+stepx<maxX-minX+2)
           if(M[i+stepx][j][k]==1) vertices |=8;

           if(k+stepz<maxZ-minZ+2)
           if(M[i][j][k+stepz]==1) vertices |=16;

           if(j+stepy<maxY-minY+2 && k+stepz<maxZ-minZ+2)
           if(M[i][j+stepy][k+stepz]==1) vertices |=32;

           if(j+stepy<maxY-minY+2 && k+stepz<maxZ-minZ+2)
           if(M[i+stepx][j+stepy][k+stepz]==1) vertices |=64;

           if(i+stepx<maxX-minX+2 && k+stepz<maxZ-minZ+2)
           if(M[i+stepx][j][k+stepz]==1) vertices |=128;

            //triangIndex = edgeTable[vert];
            for (int index=0;index<16;index++)
            edges[index]=triTable[vertices][index];

                /* Create the triangle */
                for (int indice=0;edges[indice]!=-1;indice+=3)
                {

                dx = desplaz[edges[indice]][0];
                dy = desplaz[edges[indice]][1];
                dz = desplaz[edges[indice]][2];
                vt = new punto; //1er vertice del triangulo
                vt->x = i+dx;
                vt->y = j+dy;
                vt->z = k+dz;
                TL->Add(vt);

                dx = desplaz[edges[indice+1]][0];
                dy = desplaz[edges[indice+1]][1];
                dz = desplaz[edges[indice+1]][2];
                vt = new punto; //2do vertice del triangulo
                vt->x = i+dx;
                vt->y = j+dy;
                vt->z = k+dz;
                TL->Add(vt);

                dx = desplaz[edges[indice+2]][0];
                dy = desplaz[edges[indice+2]][1];
                dz = desplaz[edges[indice+2]][2];
                vt = new punto; //3er vertice del triangulo
                vt->x = i+dx;
                vt->y = j+dy;
                vt->z = k+dz;
                TL->Add(vt);
                }
           }
}
//------------------------------------------------------------------------------
void interp (float* Punto1, float* Punto2,
             short int Valor1, short int Valor2,
             short int isovalue,
             float* PuntoRes)
{
 //Calcula la ubicación del vértice del triángulo:
 //Entrada: Punto1, Punto2: puntos a interpolar
 //         Valor1, Valor2: intensidades en HU de los valores a interpolar
 //         isovalue: isovalor de la superficie a renderizar
 //Salida: PuntoRes: Punto resultante:

 float peso1 = fabs(Valor1-isovalue);
 float peso2 = fabs(Valor2-isovalue);

 float sumapesos = peso1+peso2;
 //Cada coeficiente está ponderado por la diferencia del otro. Cuánto más
 //se parezca al valor de isovalue, menor será el peso del Otro punto.

 PuntoRes[0] = (peso2*Punto1[0]+peso1*Punto2[0])/(sumapesos);
 PuntoRes[1] = (peso2*Punto1[1]+peso1*Punto2[1])/(sumapesos);
 PuntoRes[2] = (peso2*Punto1[2]+peso1*Punto2[2])/(sumapesos);


}
//------------------------------------------------------------------------------
void marchingCubes(short int stepx, short int stepy, short int stepz,
                   Volumen* M, TList* TL, short int isolevel, bool masomenos)
{
//stepx,y,z: tamaño del cubo que marcha para renderizar
//isolevel: valor que determina el límite de la superficie a renderizar. 
//Matriz M: NO BINARIA. Permite Aceptan un volumen de short ints.
//TL: triangle list
//masomenos: mas(0): >isolevel; menos(1): <isolevel

      short int dimX, dimY, dimZ;
      dimX =  M->getAlto();
      dimY = M->getAncho();
      dimZ =  M->getProf();

      unsigned char vertices = 0;
      
      int edges[16];
      PuntoF* vt;
      PuntoF* ap;

      float dx,dy,dz;

      float color1, color2;

      float *P1, *P2;
      float *nP;

      P1 = new float[3];
      P2 = new float[3];
      nP = new float[3];

      float desplaz1[12][3] =
      {
       {0, 0, 0},         //0 .
       {0, stepy, 0},     //1 .
       {stepx, stepy, 0}, //2 .
       {stepx, 0, 0},     //3 .
       {0, 0, stepz},     //4 .
       {0, stepy, stepz}, //5 .
       {stepx, stepy, stepz},//6 .
       {stepx, 0, stepz},    //7 .
       {0, 0, 0},         //8  .
       {0, stepy, 0},     //9  .
       {stepx, stepy, 0}, //10 .
       {stepx, 0, 0}      //11
      };

      float desplaz2[12][3] =
      {
       {0, stepy, 0},     //0 .
       {stepx, stepy, 0}, //1 .
       {stepx, 0, 0},     //2 .
       {0, 0, 0},         //3 .
       {0, stepy, stepz}, //4 .
       {stepx, stepy, stepz}, //5 .
       {stepx, 0, stepz}, //6 .
       {0, 0, stepz},     //7 .
       {0, 0, stepz},     //8 .
       {0, stepy, stepz}, //9 .
       {stepx, stepy, stepz},//10 .
       {stepx, 0, stepz}  //11 .
      };

      for (int i=stepx+1; i<dimX-stepx-1; i=i+stepx)
       for (int j=stepy+1; j<dimY-stepy-1; j=j+stepy)
        for (int k=stepz+1; k<dimZ-stepz-1; k=k+stepz)
           {
           vertices = 0;

           if(masomenos==0)
           {
           if(M->GetV(i,j,k)>isolevel) vertices |=1 ;
           if(M->GetV(i,j+stepy,k)>isolevel) vertices |=2;
           if(M->GetV(i+stepx,j+stepy,k)>isolevel) vertices |=4;
           if(M->GetV(i+stepx,j,k)>isolevel) vertices |=8;
           if(M->GetV(i,j,k+stepz)>isolevel) vertices |=16;
           if(M->GetV(i,j+stepy,k+stepz)>isolevel) vertices |=32;
           if(M->GetV(i+stepx,j+stepy,k+stepz)>isolevel) vertices |=64;
           if(M->GetV(i+stepx,j,k+stepz)>isolevel) vertices |=128;
           }
           else
           {
           if(M->GetV(i,j,k)<isolevel) vertices |=1 ;
           if(M->GetV(i,j+stepy,k)<isolevel) vertices |=2;
           if(M->GetV(i+stepx,j+stepy,k)<isolevel) vertices |=4;
           if(M->GetV(i+stepx,j,k)<isolevel) vertices |=8;
           if(M->GetV(i,j,k+stepz)<isolevel) vertices |=16;
           if(M->GetV(i,j+stepy,k+stepz)<isolevel) vertices |=32;
           if(M->GetV(i+stepx,j+stepy,k+stepz)<isolevel) vertices |=64;
           if(M->GetV(i+stepx,j,k+stepz)<isolevel) vertices |=128;
           }
            //triangIndex = edgeTable[vert];
            for (int index=0;index<16;index++)
            edges[index]=triTable[vertices][index];

                // Create the triangle
                for (int indice=0;edges[indice]!=-1;indice+=3)
                {
                dx = desplaz1[edges[indice]][0];
                dy = desplaz1[edges[indice]][1];
                dz = desplaz1[edges[indice]][2];
                P1[0]=i+dx; P1[1]=j+dy; P1[2]=k+dz;
                color1 = M->GetV(i+dx,j+dy,k+dz);

                dx = desplaz2[edges[indice]][0];
                dy = desplaz2[edges[indice]][1];
                dz = desplaz2[edges[indice]][2];
                P2[0]=i+dx; P2[1]=j+dy; P2[2]=k+dz;
                color2 = M->GetV(i+dx,j+dy,k+dz);

                interp(P1,P2,color1,color2,isolevel, nP);

                vt = new PuntoF; //1er vertice del triangulo
                vt->x = nP[0];
                vt->y = nP[1];
                vt->z = nP[2];
                TL->Add(vt);

                dx = desplaz1[edges[indice+1]][0];
                dy = desplaz1[edges[indice+1]][1];
                dz = desplaz1[edges[indice+1]][2];
                P1[0]=i+dx; P1[1]=j+dy; P1[2]=k+dz;
                color1 = M->GetV(i+dx,j+dy,k+dz);

                dx = desplaz2[edges[indice+1]][0];
                dy = desplaz2[edges[indice+1]][1];
                dz = desplaz2[edges[indice+1]][2];
                P2[0]=i+dx; P2[1]=j+dy; P2[2]=k+dz;
                color2 = M->GetV(i+dx,j+dy,k+dz);

                interp(P1,P2,color1,color2,isolevel, nP);

                vt = new PuntoF; //2do vertice del triangulo
                vt->x = nP[0];
                vt->y = nP[1];
                vt->z = nP[2];
                TL->Add(vt);

                dx = desplaz1[edges[indice+2]][0];
                dy = desplaz1[edges[indice+2]][1];
                dz = desplaz1[edges[indice+2]][2];
                P1[0]=i+dx; P1[1]=j+dy; P1[2]=k+dz;
                color1 = M->GetV(i+dx,j+dy,k+dz);

                dx = desplaz2[edges[indice+2]][0];
                dy = desplaz2[edges[indice+2]][1];
                dz = desplaz2[edges[indice+2]][2];
                P2[0]=i+dx; P2[1]=j+dy; P2[2]=k+dz;
                color2 = M->GetV(i+dx,j+dy,k+dz);

                interp(P1,P2,color1,color2,isolevel, nP);

                vt = new PuntoF; //3er vertice del triangulo
                vt->x = nP[0];
                vt->y = nP[1];
                vt->z = nP[2];
                TL->Add(vt);
                }
           }

        delete P1;
        delete P2;
        delete nP;
}
//------------------------------------------------------------------------------


