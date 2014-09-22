bool compararA(int* NP,float menos12,float mas15,float menos21)
{
bool es=TRUE;

//a1 = [0 -1 -1 0 -1 -1 0 -1 -1 0 -1 -1 0 1 0 -1 -1 0 -1 -1 0 -1 -1 0 -1 -1];
int array1[]={NP[1-1],NP[4-1],NP[7-1],NP[10-1],NP[13-1],NP[15-1],NP[18-1],NP[21-1],NP[24-1]};

if (all(array1,9) && NP[14-1] == 1)
    return es;

//a2 = [-1 -1 0 -1 -1 0 -1 -1 0 -1 -1 0 1 0 -1 -1 0 -1 -1 0 -1 -1 0 -1 -1 0];
int array2[]={NP[3-1], NP[6-1], NP[9-1], NP[12-1], NP[14-1], NP[17-1], NP[20-1], NP[23-1], NP[26-1]};

if (all(array2,9) && NP[13-1]==1 && menos12 == 1)
    return es;

//a3 = [-1 -1 -1 -1 -1 -1 0 0 0 -1 1 -1 -1 -1 0 0 0 -1 -1 -1 -1 -1 -1 0 0 0];
int array3[]={NP[7-1], NP[8-1], NP[9-1], NP[15-1], NP[16-1], NP[17-1], NP[24-1], NP[25-1], NP[26-1]};

if (all(array3,9) && NP[11-1]==1)
    return es;

//a4 = [0 0 0 -1 -1 -1 -1 -1 -1 0 0 0 -1 -1 -1 1 -1 0 0 0 -1 -1 -1 -1 -1 -1];
int array4[]={NP[1-1],NP[2-1],NP[3-1],NP[10-1],NP[11-1],NP[12-1],NP[18-1],NP[19-1],NP[20-1]};

if (all(array4,9) && NP[16-1] == 1 && mas15 == 1)
    return es;

//a5 = [-1 -1 -1 -1 1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 0 0 0 0 0 0 0 0 0];
int array5[]={NP[18-1],NP[19-1],NP[20-1],NP[21-1],NP[22-1],NP[23-1],NP[24-1],NP[25-1],NP[26-1]};

if (all(array5,9) && NP[5-1] == 1)
    return es;

//a6 = [0 0 0 0 0 0 0 0 0 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 1 -1 -1 -1 -1];
int array6[]={NP[1-1],NP[2-1],NP[3-1],NP[4-1],NP[5-1],NP[6-1],NP[7-1],NP[8-1],NP[9-1]};

if (all(array6,9) && NP[22-1] == 1 && menos21 == 1)
    return es;

es = FALSE;
return es;

}
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
bool compararB(int* NP,float menos12,float mas15,float menos21)
{
bool es=TRUE;
//b1 = [0 0 -1; 0 -1 -1; -1 -1 -1; 0 0 -1; 0 1; -1 1 -1; 0 0 -1; 0 -1 -1; -1 -1 -1];
int arrayb1[]={NP[1-1],NP[2-1],NP[4-1],NP[10-1],NP[11-1],NP[13-1],NP[18-1],NP[19-1],NP[21-1]};

if (all(arrayb1,9)&& NP[14-1]==1 && NP[16-1]==1 && mas15==1)
    return es;

//b2 = [-1 0 0; -1 -1 0; -1 -1 -1; -1 0 0; 1 0; -1 1 -1; -1 0 0; -1 -1 0 -1 -1 -1];
int arrayb2[]={NP[2-1],NP[3-1],NP[6-1],NP[11-1],NP[12-1],NP[14-1],NP[19-1],NP[20-1],NP[23-1]};

if (all(arrayb2,9)&& NP[13-1]==1 && NP[16-1]==1 && menos12==1 && mas15==1)
    return es;

//b3 = [-1 -1 -1; -1 -1 0; -1 0 0; -1 1 -1; 1 0; -1 0 0; -1 -1 -1; -1 -1 0; -1 0 0];
int arrayb3[]={NP[6-1],NP[8-1],NP[9-1],NP[14-1],NP[16-1],NP[17-1],NP[23-1],NP[25-1],NP[26-1]};

if (all(arrayb3,9) && NP[11-1]==1 && NP[13-1]==1 && menos12==1)
    return es;

//b4 = [-1 -1 -1; 0 -1 -1; 0 0 -1; -1 1 -1; 0 1; 0 0 -1; -1 -1 -1; 0 -1 -1; 0 0 -1];
int arrayb4[]={NP[4-1],NP[7-1],NP[8-1],NP[13-1],NP[15-1],NP[16-1],NP[21-1],NP[24-1],NP[25-1]};

if (all(arrayb4,9)&& NP[11-1]==1 && NP[14-1]==1)
    return es;

//b5 = [-1 -1 -1; -1 1 -1; -1 -1 -1; 0 0 0; -1 -1; -1 1 -1; 0 0 0; 0 0 0; -1 -1 -1];
int arrayb5[]={NP[10-1],NP[11-1],NP[12-1],NP[18-1],NP[19-1],NP[20-1],NP[21-1],NP[22-1],NP[23-1]};

if (all(arrayb5,9)&& NP[5-1]==1 && NP[16-1]==1 && mas15==1)
    return es;

//b6 = [-1 -1 -1 ;-1 1 -1; -1 -1 -1; -1 1 -1; -1 -1; 0 0 0; -1 -1 -1; 0 0 0; 0 0 0];
int arrayb6[]={NP[15-1],NP[16-1],NP[17-1],NP[21-1],NP[22-1],NP[23-1],NP[24-1],NP[25-1],NP[26-1]};

if (all(arrayb6,9) && NP[5-1]==1 && NP[11-1]==1)
    return es;

//b7 = [0 0 0; 0 0 0; -1 -1 -1; 0 0 0; -1 -1; -1 1 -1; -1 -1 -1; -1 1 -1; -1 -1 -1];
int arrayb7[]={NP[1-1],NP[2-1],NP[3-1],NP[4-1],NP[5-1],NP[6-1],NP[10-1],NP[11-1],NP[12-1]};

if (all(arrayb7,9) && NP[16-1]==1 && NP[22-1]==1 && mas15==1 && menos21==1)
    return es;

//b8 = [-1 -1 -1; 0 0 0; 0 0 0; -1 1 -1; -1 -1; 0 0 0; -1 -1 -1; -1 1 -1; -1 -1 -1];
int arrayb8[]={NP[4-1],NP[5-1],NP[6-1],NP[7-1],NP[8-1],NP[9-1],NP[15-1],NP[16-1],NP[17-1]};

if (all(arrayb8,9) && NP[11-1]==1 && NP[22-1]==1 && menos21==1)
    return es;

//b9 = [-1 -1 -1; -1 1 -1; -1 -1 -1; -1 -1 0; 1 0; -1 -1 0; -1 0 0; -1 0 0; -1 0 0];
int arrayb9[]={NP[12-1],NP[14-1],NP[17-1],NP[19-1],NP[20-1],NP[22-1],NP[23-1],NP[25-1],NP[26-1]};

if (all(arrayb9,9) && NP[5-1]==1 && NP[13-1]==1 && menos12==1)
    return es;

//b10= [-1 -1 -1; -1 1 -1; -1 -1 -1; 0 -1 -1; 0 1; 0 -1 -1; 0 0 -1; 0 0 -1; 0 0 -1];
int arrayb10[]={NP[10-1],NP[13-1],NP[15-1],NP[18-1],NP[19-1],NP[21-1],NP[22-1],NP[24-1],NP[25-1]};

if (all(arrayb10,9) && NP[5-1]==1 && NP[14-1]==1)
    return es;

//b11= [-1 0 0; -1 0 0; -1 0 0; -1 -1 0; 1 0; -1 -1 0; -1 -1 -1; -1 1 -1; -1 -1 -1];
int arrayb11[]={NP[2-1],NP[3-1],NP[5-1],NP[6-1],NP[8-1],NP[9-1],NP[12-1],NP[14-1],NP[17-1]};

if (all(arrayb11,9) && NP[13-1] && NP[22-1]==1 && menos12==1 && menos21==1)
    return es;

//b12= [0 0 -1; 0 0 -1; 0 0 -1; 0 -1 -1; 0 1; 0 -1 -1; -1 -1 -1; -1 1 -1; -1 -1 -1];
int arrayb12[]={NP[1-1],NP[2-1],NP[4-1],NP[5-1],NP[7-1],NP[8-1],NP[10-1],NP[13-1],NP[15-1]};

if (all(arrayb12,9) && NP[14-1] && NP[22-1]==1 && menos21==1)
    return es;

es = FALSE;
return es;
}
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
bool compararC(int* NP,float menos12,float mas15,float menos21)
{
bool es = TRUE;

//c1 = [-1 -1 -1; -1 1 -1; -1 -1 -1; -1 1 -1; 1 0; -1 0 0; -1 -1 -1; -1 0 0; -1 0 0];
int arrayc1[]={NP[14-1],NP[16-1],NP[17-1],NP[22-1],NP[23-1],NP[25-1],NP[26-1]};

if (all(arrayc1,7) && NP[5-1]==1 && NP[11-1]==1 && NP[13-1]==1 && menos12==1)
    return es;


//c2 = [-1 -1 -1; -1 1 -1; -1 -1 -1; -1 1 -1; 0 1; 0 0 -1; -1 -1 -1; 0 0 -1; 0 0 -1];
int arrayc2[]={NP[13-1],NP[15-1],NP[16-1],NP[21-1],NP[22-1],NP[24-1],NP[25-1]};
if (all(arrayc2,7) && NP[5-1]==1 && NP[11-1]==1 && NP[14-1]==1)
    return es;
    
//c3 = [-1 -1 -1; -1 1 -1; -1 -1 -1; 0 0 -1; 0 1; -1 1 -1; 0 0 -1; 0 0 -1; -1 -1 -1];
int arrayc3[]={NP[10-1],NP[11-1],NP[13-1],NP[18-1],NP[19-1],NP[21-1],NP[22-1]};
if (all(arrayc3,7) && NP[5-1]==1 && NP[14-1]==1 && NP[16-1]==1 && mas15==1 )
    return es;

//c4 = [-1 -1 -1; -1 1 -1; -1 -1 -1; -1 0 0; 1 0; -1 1 -1; -1 0 0; -1 0 0; -1 -1 -1];
int arrayc4[]={NP[11-1],NP[12-1],NP[14-1],NP[19-1],NP[20-1],NP[22-1],NP[23-1]};
if (all(arrayc4,7) && NP[5-1]==1 && NP[13-1]==1 && NP[16-1]==1 && menos12==1 && mas15==1)
    return es;

//c5 = [-1 -1 -1; -1 0 0; -1 0 0; -1 1 -1; 1 0; -1 0 0; -1 -1 -1; -1 1 -1; -1 -1 -1];
int arrayc5[]={NP[5-1],NP[6-1],NP[8-1],NP[9-1],NP[14-1],NP[16-1],NP[17-1]};
if (all(arrayc5,7) && NP[11-1]==1 && NP[13-1]==1 && NP[22-1]==1 && menos12==1 && menos21==1)
    return es;

//c6 = [-1 -1 -1; 0 0 -1; 0 0 -1; -1 1 -1; 0 1; 0 0 -1; -1 -1 -1; -1 1 -1; -1 -1 -1];
int arrayc6[]={NP[4-1],NP[5-1],NP[7-1],NP[8-1],NP[13-1],NP[15-1],NP[16-1]};
if (all(arrayc6,7) && NP[11-1]==1 && NP[14-1]==1 && NP[22-1]==1 && menos21==1)
    return es;

//c7 = [0 0 -1; 0 0 -1; -1 -1 -1; 0 0 -1; 0 1; -1 1 -1; -1 -1 -1; -1 1 -1; -1 -1 -1];
int arrayc7[]={NP[1-1],NP[2-1],NP[4-1],NP[5-1],NP[10-1],NP[11-1],NP[13-1]};
if (all(arrayc7,7) && NP[14-1]==1 && NP[16-1]==1 && NP[22-1]==1 && mas15==1 && menos21==1)
    return es;

//c8 = [-1 0 0; -1 0 0; -1 -1 -1; -1 0 0; 1 0; -1 1 -1; -1 -1 -1; -1 1 -1; -1 -1 -1];
int arrayc8[]={NP[2-1],NP[3-1],NP[5-1],NP[6-1],NP[11-1],NP[12-1],NP[14-1]};
if (all(arrayc8,7) && NP[13-1]==1 && NP[16-1]==1 && NP[22-1]==1 && menos12==1 && mas15==1 && menos21==1)
    return es;

es = FALSE;
return es;

}
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
bool compararD(int* NP, int* GP)
{

bool es = TRUE;

//d1 = [0 0 0; -1 0 -1; -1 1 -1; 0 0 0; -1 -1; -1 0 -1; 0 0 0; 0 0 0; 0 0 0];
int arrayd1[]={NP[1-1],NP[2-1],NP[3-1],NP[5-1],NP[10-1],NP[11-1],NP[12-1],NP[16-1],NP[18-1],NP[19-1],NP[20-1],NP[21-1],NP[22-1],NP[23-1],NP[24-1],NP[25-1],NP[26-1]};
if (all(arrayd1,17)&& NP[8-1]== 1 && (GP[1-1]||GP[2-1]||GP[3-1]||GP[4-1]||GP[22-1]))
        return es;

//d2 = [0 0 0; 0 0 0; 0 0 0; 0 0 0; -1 -1; -1 0 -1; 0 0 0; -1 0 -1; -1 1 -1];
int arrayd2[]={NP[1-1],NP[2-1],NP[3-1],NP[4-1],NP[5-1],NP[6-1],NP[7-1],NP[8-1],NP[9-1],NP[10-1],NP[11-1],NP[12-1],NP[16-1],NP[18-1],NP[19-1],NP[20-1],NP[22-1]};
if (all(arrayd2,17) && NP[25-1] == 1 && (GP[1-1]||GP[5-1]||GP[6-1]||GP[7-1]||GP[23-1]))
        return es;

//d3 = [0 0 0; -1 -1 0; -1 -1 -1; 0 0 0; 0 0; 1 0 0; 0 0 0; -1 -1 0; -1 -1 0];
int arrayd3[]={NP[1-1],NP[2-1],NP[3-1],NP[6-1],NP[10-1],NP[11-1],NP[12-1],NP[13-1],NP[14-1],NP[16-1],NP[17-1],NP[18-1],NP[19-1],NP[20-1],NP[23-1],NP[26-1]};
if (all(arrayd3,16) && NP[15-1]==1 && (GP[1-1]||GP[8-1]||GP[9-1]||GP[10-1]||GP[15-1]))
        return es;

//d4 = [0 0 0; 0 -1 -1; 0 -1 -1; 0 0 0; 0 0; 0 0 1; 0 0 0; 0 -1 -1; 0 -1 -1];
int arrayd4[]={NP[1-1],NP[2-1],NP[3-1],NP[4-1],NP[7-1],NP[10-1],NP[11-1],NP[12-1],NP[13-1],NP[14-1],NP[15-1],NP[16-1],NP[18-1],NP[19-1],NP[20-1],NP[21-1],NP[24-1]};
if (all(arrayd4,17) && NP[17-1]==1 && (GP[1-1]||GP[11-1]||GP[12-1]||GP[13-1]||GP[14-1]))
        return es;

//d5 = [-1 -1 0; 1 0 0; -1 -1 0; -1 -1 0; 0 0; -1 -1 0; 0 0 0; 0 0 0; 0 0 0];
int arrayd5[]={NP[3-1],NP[5-1],NP[6-1],NP[9-1],NP[12-1],NP[13-1],NP[14-1],NP[17-1],NP[18-1],NP[19-1],NP[20-1],NP[21-1],NP[22-1],NP[23-1],NP[24-1],NP[25-1],NP[26-1]};
if (all(arrayd5,17) && NP[4-1]==1 && (GP[15-1]||GP[16-1]||GP[17-1]||GP[18-1]||GP[22-1]))
        return es;

//d6 = [0 0 0; 0 0 0; 0 0 0; -1 -1 0; 0 0; -1 -1 0; -1 -1 0; 1 0 0; -1 -1 0];
int arrayd6[]={NP[1-1],NP[2-1],NP[3-1],NP[4-1],NP[5-1],NP[6-1],NP[7-1],NP[8-1],NP[9-1],NP[12-1],NP[13-1],NP[14-1],NP[17-1],NP[20-1],NP[22-1],NP[23-1],NP[26-1]};
if (all(arrayd6,17) && NP[21-1]==1 && (GP[15-1]||GP[19-1]||GP[20-1]||GP[21-1]||GP[23-1]))
        return es;

//d7 = [-1 -1 -1; -1 1 -1; -1 -1 -1; 0 0 1; 0 0; 0 0 0; -1 -1 -1; -1 1 -1; -1 -1 -1];
int arrayd7[]={NP[10-1],NP[11-1],NP[13-1],NP[14-1],NP[15-1],NP[16-1],NP[17-1]};
if (all(arrayd7,7) && NP[5-1]==1 && NP[12-1]==1 && NP[22-1]==1)
        return es;

//d8 = [-1 -1 -1; -1 1 -1; -1 -1 -1; 1 0 0; 0 0; 0 0 0; -1 -1 -1; -1 1 -1; -1 -1 -1];
int arrayd8[]={NP[11-1],NP[12-1],NP[13-1],NP[14-1],NP[15-1],NP[16-1],NP[17-1]};
if (all(arrayd8,7) && NP[5-1]==1 && NP[10-1]==1 && NP[22-1]==1)
        return es;

//d9 = [-1 1 -1; -1 0 -1; -1 0 -1; -1 0 -1; 1 1; -1 0 -1; -1 0 -1; -1 0 -1; -1 0 -1];
int arrayd9[]={NP[5-1],NP[8-1],NP[11-1],NP[16-1],NP[19-1],NP[22-1],NP[25-1]};
if (all(arrayd9,7) && NP[2-1]==1 && NP[13-1]==1 && NP[14-1]==1)
        return es;

//d10= [-1 0 -1 -1 0 -1 -1 0 -1 -1 0 -1 1 1 -1 0 -1 -1 1 -1 -1 0 -1 -1 0 -1];
int arrayd10[]={NP[2-1],NP[5-1],NP[8-1],NP[11-1],NP[16-1],NP[22-1],NP[25-1]};
if (all(arrayd10,7) && NP[13-1]==1 && NP[14-1]==1 && NP[19-1]==1)
        return es;

//d11= [-1 -1 -1; 0 0 0; -1 -1 -1; -1 1 -1; 0 0; -1 1 -1; -1 -1 -1; 0 0 1; -1 -1 -1];
int arrayd11[]={NP[4-1],NP[5-1],NP[6-1],NP[13-1],NP[14-1],NP[21-1],NP[22-1]};
if (all(arrayd11,7) && NP[11-1]==1 && NP[16-1]==1 && NP[23-1]==1)
        return es;

//d12= [-1 -1 -1; 0 -1 1; -1 -1 -1; -1 1 -1; 0 0; -1 1 -1; -1 -1 -1; 0 0 0; -1 -1 -1];
int arrayd12[]={NP[4-1],NP[13-1],NP[14-1],NP[21-1],NP[22-1],NP[23-1]};
if (all(arrayd12,6) && NP[6-1]==1 && NP[11-1]==1 && NP[16-1]==1)
        return es;

es = FALSE;
return es;
}
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------

void Esqueletonizacion(float PoX[], float PoY[], float PoZ[], int tamanio,float minx,float miny,
                       float minz, float maxx, float maxy, float maxz,TListBox* EsqueLista, AnsiString NombreArchivo)
{

float* PX,*PY,*PZ;

PX = new float[tamanio];
PY = new float[tamanio];
PZ = new float[tamanio];

for(int i=2;i<tamanio+2;i++)  //+2: Las dos primeras lineas de PX,PY y PZ son la palabra ROIXYZ y el nombre del directorio
{
PX[i-2]=PoX[i];
PY[i-2]=PoY[i];
PZ[i-2]=PoZ[i];
}

//Nota: podría lograrse el mismo resultado utilizando max y min mandados por el modulo Paralax
int minX = int(minx);
int minY = int(miny);
int minZ = int(minz);

int maxX = int(maxx);
int maxY = int(maxy);
int maxZ = int(maxz);

int vuelta = 0;
int j, index;
bool d_f; //delete_flag: indica si el punto debe borrarse en la siguiente iteracion

//deberia pasarle referencia?

sumar(PX,-(minX)+2,tamanio);
sumar(PY,-(minY)+2,tamanio);
sumar(PZ,-(minZ)+2,tamanio);

int tMx=(maxX-minX)+5;
int tMy=(maxY-minY)+5;
int tMz=(maxZ-minZ)+5;

int*** M;

//inicializa en 0? for(for(for()))

//llenar matriz

M=new int** [tMx];

for (int i=0; i<tMx; ++i)
{
M[i]=new int* [tMy];

        for (int j=0; j<tMy; ++j)
        {
        M[i][j]=new int [tMz];

                for (int k=0; k<tMz; ++k)
                M[i][j][k]=0;
        }
}

for (int i=0;i<tamanio;i++)
M[int(PX[i])][int(PY[i])][int(PZ[i])]=1;

float* index_list;
index_list = new float[tamanio];

//p: punto extraido de (Px,PZ,PZ);
float p[3];

float menos12,mas15,menos21;

int NP[26];
int GP[23];

while(TRUE)
{

j=0;

//Hay que hacer el clear de del_list y de index_list;
	for(int i=0;i<tamanio;i++)
	{
         if(PX[i]!=-1)
         {
	p[0]=PX[i]; //px
	p[1]=PY[i]; //py
	p[2]=PZ[i]; //pz

      	collect26neigh(p,M,NP); //equivale a: NP=collect26neigh(p,M)

	    	if(suma(NP)<26)
	    	{
		    	if(is_tail_point(NP,M,p)==FALSE)
			{
		   	menos12 = M[int(p[0])][int(p[1])-2][int(p[2])];
		   	mas15   = M[int(p[0])+2][int(p[1])][int(p[2])];
		   	menos21 = M[int(p[0])][int(p[1])][int(p[2])-2];

                            d_f = FALSE;

		       	if(compararA(NP, menos12, mas15, menos21)==TRUE)
		       		d_f=TRUE;

	   	       	if(d_f == FALSE)
                       	        if (compararB(NP, menos12, mas15, menos21)==TRUE)
                       	                d_f=TRUE;

	   	       	if(d_f == FALSE)
                       	        if (compararC(NP, menos12, mas15, menos21)==TRUE)
                                     d_f=TRUE;

            	      	if(d_f == FALSE)
		      	{
                      	collectGP(M,p,GP); //26_vecinos extendidos (23-perifericos)

                      	if (compararD(NP,GP)==TRUE)
                      	 d_f=TRUE;
                      	}

			if (d_f==TRUE)   //la vecindad de p coincide con alguna plantilla?
			{

            		index_list[j] = i;

            		j++;

            		//j almacena la cantidad de puntos a ser borrados en cada iteracion.
                        //Tb funciona como indice para del_list e index_list

			}//if(d_f == TRUE)...

		      	}//if is_tail_point...

	     	}//if suma(26)...

             }//if(PX[i]!=-1)

	} //for...


	if (j==0)  //No encontró ningun valor para borrar?
	{

    	delete[] M;
	delete[] index_list;

        EsqueLista->Items->Add("ESQUELETO:" + NombreArchivo);

        for(int i=0;i<tamanio;i++)
                if(PX[i]!=-1)
                        EsqueLista->Items->Add(AnsiString(PX[i]+minX-2)+","+AnsiString(PY[i]+minY-2)+","+AnsiString(PZ[i]+minZ-2));
                 //Los valores de Y se llevan a sus coordenadas originales.  //Fin de algoritmo.
        delete[] PX;
        delete[] PY;
        delete[] PZ;
	return;
    	}

    for(int i=0; i<j; i++)
    {
    index = index_list[i];
    M[int(PX[index])][int(PY[index])][int(PZ[index])]=0;
    PX[index]=-1;
    PY[index]=-1;
    PZ[index]=-1;          //Elimina los valores correspondientes a los valores de index_list
    }

    vuelta ++;


}// while...



}
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
void Shell(float PX[], float PY[], float PZ[], int tamanio,float minx,float miny,
                       float minz, float maxx, float maxy, float maxz)
{

//Nota: podría lograrse el mismo resultado utilizando max y min mandados por el modulo Paralax
int minX = int(minx);
int minY = int(miny);
int minZ = int(minz);

int maxX = int(maxx);
int maxY = int(maxy);
int maxZ = int(maxz);

int j, index;
bool d_f; //delete_flag: indica si el punto debe borrarse en la siguiente iteracion

//deberia pasarle referencia?

sumar(PX,-(minX)+2,tamanio);
sumar(PY,-(minY)+2,tamanio);
sumar(PZ,-(minZ)+2,tamanio);

int tMx=(maxX-minX)+5;
int tMy=(maxY-minY)+5;
int tMz=(maxZ-minZ)+5;

int*** M;

//inicializa en 0? for(for(for()))

//llenar matriz

M=new int** [tMx];

for (int i=0; i<tMx; ++i)
{
M[i]=new int* [tMy];

        for (int j=0; j<tMy; ++j)
        {
        M[i][j]=new int [tMz];

                for (int k=0; k<tMz; ++k)
                M[i][j][k]=0;
        }
}

for (int i=2;i<tamanio;i++)
M[int(PZ[i])][int(PX[i])][int(PY[i])]=1;

float* index_list;
index_list = new float[tamanio];

//p: punto extraido de (Px,PZ,PZ);
float p[3];

int NP[26];

j=0;

//Hay que hacer el clear de del_list y de index_list;
	for(int i=2;i<tamanio;i++)
	{
	p[0]=PX[i]; //px
	p[1]=PY[i]; //py
	p[2]=PZ[i]; //pz

      	collect26neigh(p,M,NP); //equivale a: NP=collect26neigh(p,M)
  //Elimina: Puntos interiores (sNP=26) y puntos en el extremo de la caja(px=maxX o minX, etc.)
	    	if(suma(NP)==26||p[0]==2||p[1]==2||p[2]==2||p[0]==tMx-3||p[1]==tMy-3||p[2]==tMz-3)
	    	{
            		index_list[j] = i;
            		j++;

	     	}//if suma(26)...

	} //for...

    for(int i=0; i<j; i++)
    {
    index = index_list[i];
    M[int(PX[index])][int(PY[index])][int(PZ[index])]=0;
    PX[index]=NULL;
    PY[index]=NULL;
    PZ[index]=NULL;          //Elimina los valores correspondientes a los valores de index_list
    }

    delete[] M;
    delete[] index_list;

    //Los valores de (x,y,z) se llevan a sus coordenadas originales.
       for(int i=0;i<tamanio;i++)
                if(PX[i]!=NULL)
                        {
                        PX[i]=PX[i]+minX-2;
                        PY[i]=PY[i]+minY-2;
                        PZ[i]=PZ[i]+minZ-2;
                        }

    //Fin de algoritmo.
	return;

}
//---------------------------------------------------------------------------------------------------------------------

