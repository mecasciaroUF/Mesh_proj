
/*
Rutina de mallado 3D siguiendo una curvatura arbitraria.
Par�metros de entrada:
CL: curva arbitraria de N puntos consecutivos.
r:  radio de la secci�n transversal del cilindro final.
nA: cantidad de puntos angulares en los que se divide una secci�n circular.
nL: cantidad de puntos a interpolar entre CL(i) y CL(i+1).

Par�metros de salida:
Nodes: lista de puntos nodales de la malla.
Edges: lista de lados de la malla.
Faces: lista de caras triangulares de la malla.

M�todolog�a:

Primero se aproxima la curva y su derivada mediante un spline c�bico, imponiendo
nL puntos entre los valores de CL (considerados 'anchor points' para la malla.
Con la derivada del spline se calcula la lista auxiliar NL de vectores normales al
spline. 

Autor: Mariano E. Casciaro - Universidad Favaloro - 2014
*/

//------------------------------------------------------------------------------
void Rotate(TList* C, PuntoF* Normal, PuntoF* Centro, float angulo)
{
int i,j;

PuntoF* aP;
float rX,rY,rZ;
float eje[3];
float norm_eje;

eje[0] =  Normal->x;
eje[1] =  Normal->y;
eje[2] =  Normal->z;

norm_eje = sqrt(eje[0]*eje[0] + eje[1]*eje[1] + eje[2]*eje[2]);
eje[0] /= norm_eje; eje[1] /=norm_eje; eje[2] /= norm_eje;

//Creaci�n de la matriz de Rotaci�n:
float c,s;
c = cos(angulo);
s = sin(angulo);
float R[3][3] = { {eje[0]*eje[0]+(1.0-eje[0]*eje[0])*c, eje[0]*eje[1]*(1.0-c)-eje[2]*s, eje[0]*eje[2]*(1.0-c)+eje[1]*s},
                  {eje[0]*eje[1]*(1.0-c)+eje[2]*s, eje[1]*eje[1]+(1.0-eje[1]*eje[1])*c, eje[1]*eje[2]*(1.0-c)-eje[0]*s},
                  {eje[0]*eje[2]*(1.0-c)-eje[1]*s, eje[1]*eje[2]*(1.0-c)+eje[0]*s, eje[2]*eje[2]+(1.0-eje[2]*eje[2])*c} };


for(i=0; i<C->Count; i++)
{
    aP = (PuntoF*)C->Items[i];

    rX = (aP->x - Centro->x) * R[0][0] + (aP->y - Centro->y) * R[1][0] + (aP->z - Centro->z) * R[2][0];
    rY = (aP->x - Centro->x) * R[0][1] + (aP->y - Centro->y) * R[1][1] + (aP->z - Centro->z) * R[2][1];
    rZ = (aP->x - Centro->x) * R[0][2] + (aP->y - Centro->y) * R[1][2] + (aP->z - Centro->z) * R[2][2];

    aP->x = rX + Centro->x;
    aP->y = rY + Centro->y;
    aP->z = rZ + Centro->z;
}

}
//------------------------------------------------------------------------------

void Orientate(TList* C, PuntoF* Normal, PuntoF* Centro)
{
int i,j;

float angulo;
float norm_eje;
float eje[3];

PuntoF* aP;
float rX,rY,rZ;

angulo = -acos(Normal->z);

eje[0] = -Normal->y;
eje[1] =  Normal->x;
eje[2] =  0.0;

norm_eje = sqrt(eje[0]*eje[0] + eje[1]*eje[1] + eje[2]*eje[2]);
eje[0] /= norm_eje; eje[1] /=norm_eje; eje[2] /= norm_eje;

//Creaci�n de la matriz de Rotaci�n:
float c,s;
c = cos(angulo);
s = sin(angulo);
float R[3][3] = { {eje[0]*eje[0]+(1.0-eje[0]*eje[0])*c, eje[0]*eje[1]*(1.0-c)-eje[2]*s, eje[0]*eje[2]*(1.0-c)+eje[1]*s},
                  {eje[0]*eje[1]*(1.0-c)+eje[2]*s, eje[1]*eje[1]+(1.0-eje[1]*eje[1])*c, eje[1]*eje[2]*(1.0-c)-eje[0]*s},
                  {eje[0]*eje[2]*(1.0-c)-eje[1]*s, eje[1]*eje[2]*(1.0-c)+eje[0]*s, eje[2]*eje[2]+(1.0-eje[2]*eje[2])*c} };


for(i=0; i<C->Count; i++)
{
    aP = (PuntoF*)C->Items[i];

    rX = aP->x * R[0][0] + aP->y * R[1][0] + aP->z * R[2][0];
    rY = aP->x * R[0][1] + aP->y * R[1][1] + aP->z * R[2][1];
    rZ = aP->x * R[0][2] + aP->y * R[1][2] + aP->z * R[2][2];

    aP->x = rX + Centro->x;
    aP->y = rY + Centro->y;
    aP->z = rZ + Centro->z;
}

}
//------------------------------------------------------------------------------


void UniformCubicSpline(TList* CL,int nL, TList* SL, TList* NL)
{
//Entrada:
//CL: N puntos del CenterLine ('anchor points')
//nL: par�metro de muestreo del B-Spline continuo.

//Salida:
//SL: spline muestreado para dar (1/nL)*N puntos uniformemente espaciados.
//NL: vectores normales al spline en cada punto muestreado.

// (ej: alfa = 0.1 => el
// %par�metro t que modula el spline de cada segmento se muestrea entre t=0 y
// %t=0.9 cada 0.1.

// Referencia: Wikipedia: B-Spline:
// http://en.wikipedia.org/wiki/B-spline#cite_note-8

//�ndices auxiliares: i,j,k:
int i,j,k,m;

float alfa = 1.0/float(nL);
float* t = new float [nL];

for(i=0; i<nL; i++)
t[i] = i*alfa;

//Matriz de coeficientes para el spline c�bico uniforme:
float C[4][4] = {{-1,3,-3,1}, {3,-6,3,0}, {-3,0,3,0}, {1,4,1,0}};
for(i=0; i<4; i++)
    for(j=0; j<4; j++)
        C[i][j] *= 1.0/6.0;

//Matriz T del par�metro t elevados hasta potencia de 3:
// T[i][j] = t[j]^i  con i=3,2,1,0
float* T[4];

for(i=0; i<4; i++)
    T[i] = new float[nL];

for(i=3; i>0; i--)
    for(j=0; j<nL; j++)
        T[3-i][j] = pow(t[j],float(i));
for(j=0; j<nL; j++)
    T[3][j] = 1.0;

//Punteros auxiliares:
PuntoF* nP;
PuntoF* aP,*aPN;
float CxCL[4][3];  //auxiliar para almacenar el producto matricial de C con CL(i-1,i,i+1,i+2)

for(i=1; i<CL->Count-2; i++)
{
    for(j=0; j<4; j++)
    {
    CxCL[j][0] = 0;
    CxCL[j][1] = 0;
    CxCL[j][2] = 0;

        for(k=0; k<4; k++)
        {
            aP = (PuntoF*)CL->Items[i+k-1];
            CxCL[j][0] += C[j][k]* aP->x;
            CxCL[j][1] += C[j][k]* aP->y;
            CxCL[j][2] += C[j][k]* aP->z;
        }
    }


    for(j=0; j<nL; j++)
    {
        nP = new PuntoF;
        nP->x = 0;
        nP->y = 0;
        nP->z = 0;

        for(k=0; k<4; k++)
        {
            nP->x += CxCL[k][0]*T[k][j];
            nP->y += CxCL[k][1]*T[k][j];
            nP->z += CxCL[k][2]*T[k][j];
        }

        SL->Add(nP);
    }
}


/*
//C�lculo de las normales: M�todo 2:
for(i=0; i<SL->Count-1; i++)
{
    aP = (PuntoF*)SL->Items[i];
    aPN = (PuntoF*)SL->Items[i+1];

    nP = new PuntoF;

    nP->x = aPN->x - aP->x;
    nP->y = aPN->y - aP->y;
    nP->z = aPN->z - aP->z;

    NL->Add(nP);
}

//Repite la �ltima normal para mantener iguales los tama�os se SL y NL:
nP = new PuntoF;
nP->x = aPN->x - aP->x;
nP->y = aPN->y - aP->y;
nP->z = aPN->z - aP->z;

NL->Add(nP);

*/

//C�lculo de las Normales a CL: M�todo 1:
//T = [3*t.^2 2*t ones(size(t)) zeros(size(t))];
for(i=2; i>0; i--)
    for(j=0; j<nL; j++)
    T[2-i][j] = float(i+1)*pow(t[j],float(i));
for(j=0; j<nL; j++)
{
    T[2][j] = 1.0;
    T[3][j] = 0.0;
}


for(i=1; i<CL->Count-2; i++)
{
    for(j=0; j<4; j++)
    {
    CxCL[j][0] = 0;
    CxCL[j][1] = 0;
    CxCL[j][2] = 0;

        for(k=0; k<4; k++)
        {
            aP = (PuntoF*)CL->Items[i+k-1];
            CxCL[j][0] += C[j][k]* aP->x;
            CxCL[j][1] += C[j][k]* aP->y;
            CxCL[j][2] += C[j][k]* aP->z;
        }
    }

    for(j=0; j<nL; j++)
    {
        nP = new PuntoF;
        nP->x = 0.0;
        nP->y = 0.0;
        nP->z = 0.0;

        for(k=0; k<4; k++)
        {
            nP->x += CxCL[k][0]*T[k][j];
            nP->y += CxCL[k][1]*T[k][j];
            nP->z += CxCL[k][2]*T[k][j];
        }

        NL->Add(nP);
    }
}

delete[] t;

for(i=0; i<4; i++)
    delete[] T[i];
}
//------------------------------------------------------------------------------

Mesh* CylinderMeshing (TList* CL, TList* NL, int nA, float r, TList* Faces, TList* Edges, TList* Nodes,
float limitInterNodeDistance ,double dx, double dy, double dz)
{
//nA: pasos angulares de las circunferencias en el centerline CL
//�ndices auxiliares: i,j,k:
int i,j,k,m;
PuntoF *aP, *aP_N;
float Norma;
//Normales unitarias:
for(i=0; i<NL->Count; i++)
{
    aP = (PuntoF*)NL->Items[i];
    Norma = sqrt(aP->x*aP->x + aP->y*aP->y + aP->z*aP->z);
  //  if(Norma)
  //  {
    aP->x /= Norma;
    aP->y /= Norma;
    aP->z /= Norma;
  //  }
}

//%Parameter 't' (angle) going from 0� to 360� by a deg� step:
//t = 0:nA:360-nA;

float deltaA = 2.0*3.1415/float(nA);
float* t = new float[nA];
for(i=0; i<nA; i++)
t[i] = float(i*deltaA);
TList* Circs;
Circs = new TList;

//for i=1:size(Nm,1) %For each point Cm(i), with its corresponding normal Nm(i)...
//c(i).coords = [sind(t)' cosd(t)' zeros(size(t,2),1)]; %... fill circle structure coordinates with a unitary circle in polar coodinates, origin-centered...
//c(i).coords = c(i).coords*mean([D(i)/2 D(i+1)/2]);%... scale the unitary circle by mean anterior-posterior radius...
//c(i).coords = Orientate(c(i).coords, [0 0 1], Cm(i,:), Nm(i,:));%...and rotate the scaled circle to match Nm(i) direction, and translate it to Cm(i)
//end ...
TList* aL;

for(i=0; i<CL->Count; i++)
{
    aL = new TList;
    for(j=0; j<nA ; j++)
    {
    aP = new PuntoF;
    aP->x = r*sin(t[j]);
    aP->y = r*cos(t[j]);
    aP->z = 0.0;
    aL->Add(aP);
    }

    Orientate(aL, (PuntoF*)NL->Items[i], (PuntoF*)CL->Items[i]);
    Circs->Add(aL);
}

//3-Search for nearest succesive circle point and perform post-alignment:
float signo = 0.0;
TList* aL_N;
float dist;
float minDist;
int i_close = 0;
float half_angle = deltaA/2.0;

for(i=0; i<CL->Count-1; i++)
{
     aL     = (TList*)Circs->Items[i];   //Look at this circumference and...
     aL_N   = (TList*)Circs->Items[i+1]; //... next circumference.

     //Take the first point of the circumference...
     aP = (PuntoF*)aL->Items[0];
     //... and compare it with all points of the second circumference:
     aP_N = (PuntoF*)aL_N->Items[0];

     dist    = 0;
     i_close = 0;
     minDist = sqrt( pow(aP_N->x-aP->x,2.0) + pow(aP_N->y-aP->y,2.0) + pow(aP_N->z-aP->z,2.0) );

     for(j=0; j<aL_N->Count; j++)
     {
     aP_N = (PuntoF*)aL_N->Items[j];
     dist = sqrt( pow(aP_N->x-aP->x,2.0) + pow(aP_N->y-aP->y,2.0) + pow(aP_N->z-aP->z,2.0) );

        if( dist <= minDist)
        {
        minDist = dist;
        i_close = j;
        }
     }
     //The closest point of the next circumference is in position i_close, so
     //i need to perform i_close forward-shifts to get i_close to 0 index:
     for(k=0; k<aL_N->Count-i_close; k++)
        for(m=aL_N->Count-1; m>0; m--)
            aL_N->Exchange( m, m-1);
}

for(i=0; i<CL->Count; i++)
{
     signo *=-1.0;
     aL = (TList*)Circs->Items[i];
     Rotate(aL, (PuntoF*)NL->Items[i], (PuntoF*)CL->Items[i], half_angle*signo);
}

//All CenterLine lying circumferences are now aligned in a way that no triangles
//will be pre-intersecting.
//Mesh algorithm begins:

//The triangular mesh scheme relies on a 3-coordintate V (vertex) vector
//corresponding to circular coordinates, and a T (triangle) vector
//corresponding to the indexes of V that form a triangular face:

int nCirc = aL->Count; //amount of points by circle

Node *aN1, *aN2, *aN3;
Face *aF;
Edge *aE;

Mesh* cylMesh;

TList* aux_face_list = new TList;
TList* aux_edge_list = new TList;
TList* aux_node_list = new TList;

aN1->set_node_list( aux_node_list );
aF->set_face_list( aux_face_list );
aE->set_edge_list( aux_edge_list );

//For every circle point, create a Node:
for(i=0; i<CL->Count; i++)
{
     aL  = (TList*)Circs->Items[i];

     for(j=0; j<aL->Count; j++)
     {
     aP = (PuntoF*)aL->Items[j];
     new Node(aP->x, aP->y, aP->z);
     }
}

Nodes = aN1->get_node_list();

for(i=0; i<CL->Count-1; i++)
{
     //Triangle triplet indexes:
     //(i)*nCirc + j, (i+1)*nCirc+j, (i)*nCirc + (j+1)

     for(j=0; j<nCirc-1; j++)
     {
     aN1 = (Node*)Nodes->Items[ i*nCirc + j ];
     aN2 = (Node*)Nodes->Items[(i+1)*nCirc + j ];
     aN3 = (Node*)Nodes->Items[ i*nCirc + (j+1) ];

     // Face(Node* nL, Node* nR, Node* nO, Edge* eD, Edge* eR, Edge* eL);
     aF = new Face(aN3, aN1, aN2);

     //Inverted Triangles triplet indexes:
     //(i)*nCirc + (j+1), (i+1)*nCirc+j, (i+1)*nCirc + (j+1)
     aN1 = (Node*)Nodes->Items[ i*nCirc + (j+1) ];
     aN2 = (Node*)Nodes->Items[ (i+1)*nCirc + j ];
     aN3 = (Node*)Nodes->Items[ (i+1)*nCirc + (j+1) ];

     // Face(Node* nL, Node* nR, Node* nO, Edge* eD, Edge* eR, Edge* eL);
     aF = new Face(aN3, aN1, aN2);
     }

     //Join the extreme rectangles:
     //(i)*nCirc + nCirc, (i+1)*nCirc + nCirc, (i)*nCirc + 1
     //(i)*nCirc + 1, (i+1)*nCirc+nCirc, (i+1)*nCirc + 1

     aN1 = (Node*)Nodes->Items[ i*nCirc + (nCirc-1) ];
     aN2 = (Node*)Nodes->Items[ (i+1)*nCirc + (nCirc-1) ];
     aN3 = (Node*)Nodes->Items[ i*nCirc];

     // Face(Node* nL, Node* nR, Node* nO, Edge* eD, Edge* eR, Edge* eL);
     aF = new Face(aN3, aN1, aN2);

     aN1 = (Node*)Nodes->Items[ i*nCirc ];
     aN2 = (Node*)Nodes->Items[ (i+1)*nCirc + (nCirc-1) ];
     aN3 = (Node*)Nodes->Items[ (i+1)*nCirc];

     // Face(Node* nL, Node* nR, Node* nO, Edge* eD, Edge* eR, Edge* eL);
     aF = new Face(aN3, aN1, aN2);
}


//%5-Close aorta by meshing the ascendig and descending apertures:
//5.1- Add two extra Nodes for the geometrical center of the first and last circle of 'c'.

aP = (PuntoF*)CL->First();
new Node(aP->x, aP->y, aP->z);

aP = (PuntoF*)CL->Last();
new Node(aP->x, aP->y, aP->z);

//First and last Node indexes:
int pF_index, pL_index;
pF_index = Nodes->Count-2;
pL_index = Nodes->Count-1;

//Traverses first circle:
for(j=0; j<nCirc-1; j++)
{
     //Mesh with concentric triangles
     aN3 = (Node*)Nodes->Items[j];
     aN2 = (Node*)Nodes->Items[pF_index];
     aN1 = (Node*)Nodes->Items[j+1];

     // Face(Node* nL, Node* nR, Node* nO, Edge* eD, Edge* eR, Edge* eL);
     aF = new Face(aN3, aN1, aN2);
}

//Closes the first circle:
aN3 = (Node*)Nodes->Items[nCirc-1];
aN2 = (Node*)Nodes->Items[pF_index];
aN1 = (Node*)Nodes->Items[0];
aF = new Face(aN3, aN1, aN2);

//Traverses last circle:
for(j=0; j<nCirc-1; j++)
{
     //Mesh with concentric triangles
     aN1 = (Node*)Nodes->Items[ ((Nodes->Count-3) - (nCirc-1)) + j ];
     aN2 = (Node*)Nodes->Items[ pL_index ];
     aN3 = (Node*)Nodes->Items[ ((Nodes->Count-3) - (nCirc-1)) + (j+1) ];
     // Face(Node* nL, Node* nR, Node* nO, Edge* eD, Edge* eR, Edge* eL);
     aF = new Face(aN3, aN1, aN2);
}

//Closes the Last circle:
aN1 = (Node*)Nodes->Items[ Nodes->Count-3 ];
aN2 = (Node*)Nodes->Items[ pL_index ];
aN3 = (Node*)Nodes->Items[ ((Nodes->Count-3)-(nCirc-1))];
aF = new Face(aN3, aN1, aN2);

//Edges = aF->get_left_edge()->getEdgeList();
//Faces = aF->get_face_list();
//aF->setD(dx,dy,dz);

cylMesh = new Mesh(aux_face_list, aux_edge_list, aux_node_list,
                   limitInterNodeDistance, dx, dy, dz);

delete[] t;

for(i=0; i<CL->Count; i++)
{
    aL = (TList*)Circs->Items[i];

    for(j=0; j<aL->Count; j++)
    {
    aP = (PuntoF*)aL->Items[j];
    delete aP;
    }

    aL->Clear();
}
Circs->Clear();
delete Circs;


return cylMesh;

/*
%Cm: Centerline midpoints: matrix containing all midpoints between C
%points.
Cm =(circshift(C,-1)+ C)/2;
Cm(end,:)=[];

%Nm: normal vector at centerline midpoints Cm, obtained though average of
%two consecutive normal vectors:
Nm = (circshift(N,-1)+N)/2;
Nm(end,:) = [];

%Nm normalization:
for i=2:size(Nm,1)
n = Nm(i,:);
n = n/norm(n);
Nm(i,:) = n;
end

%2-Circumference parametrization:

%Parameter 't' (angle) going from 0� to 360� by a deg� step:
t = 0:deg:360-deg;

c = struct; %preallocate space for all CSA's strucures.
c.coords = zeros(size(t,2),3);

for i=1:size(Nm,1) %For each point Cm(i), with its corresponding normal Nm(i)...
c(i).coords = [sind(t)' cosd(t)' zeros(size(t,2),1)]; %... fill circle structure coordinates with a unitary circle in polar coodinates, origin-centered...
c(i).coords = c(i).coords*mean([D(i)/2 D(i+1)/2]);%... scale the unitary circle by mean anterior-posterior radius...
c(i).coords = Orientate(c(i).coords, [0 0 1], Cm(i,:), Nm(i,:));%...and rotate the scaled circle to match Nm(i) direction, and translate it to Cm(i)
end

%3-Search for nearest succesive circle point and perform post-alignment:
signo = -1;

for i=1:size(Nm,1)-1

    %ci: first element of c(i).coords struct:
    ci = c(i).coords(1,:);

    %cii: all points of the next circle, c(i+1).coords struct:
    cii = c(i+1).coords;

    %d: vector joining ci with every point of the next circle cii:
    d = cii-repmat(ci, [size(cii,1), 1]);

    %The position of the mininum of d-vector resuls in the position of the
    %nearest point to ci in cii:
    for j=1:size(d,1)
        n = norm(d(j,:));
        d(j,:) = [n 0 0];
    end

    d = d(:,1);

    %mind: min distance; posmind: position (index) to the minimum distance:
    [mind, posmind] = min(d); %#ok

    %Signo alternates +-1 to generates displacemente that will result in a triangular, delta-shift alternated mesh:
    signo = signo*-1;

    c(i+1).coords = circshift(c(i+1).coords,-posmind);
end


%4-Mesh generating algorithm:

%The triangular mesh scheme relies on a 3-coordintate V (vertex) vector
%corresponding to circular coordinates, and a T (triangle) vector
%corresponding to the indexes of V that form a triangular face:

nCirc = size(c(1).coords,1); %amount of points by circle

V = [];

%Creacion of V (consider preallocation to avoid warning):
for i=1:size(Nm,1)-1
    for j=1:size(c(i).coords,1)
        V = [V; c(i).coords(j,:)]; %#ok
    end
end

%Creation of T (consider preallocation to avoid warning):
T = [];

for i=1:size(Nm,1)-2 %For the i-th circle...

    for j=1:nCirc-1 %... take the j-th element of the circle ...
        % ... and rotate around the circle making the rectangles:
    T = [T; (i-1)*nCirc + j, i*nCirc+j, (i-1)*nCirc + (j+1)]; %#ok
    T = [T; (i-1)*nCirc + (j+1), i*nCirc+j, i*nCirc + (j+1)]; %#ok
    end

    %Join the extreme rectangles:
    T = [T; (i-1)*nCirc + nCirc, i*nCirc + nCirc, (i-1)*nCirc + 1]; %#ok
    T = [T; (i-1)*nCirc + 1, i*nCirc+nCirc, i*nCirc + 1];           %#ok

end


*/

}


