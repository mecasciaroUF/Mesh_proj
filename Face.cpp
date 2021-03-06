#include "Face.h"
#include "Node.h"
#include "Edge.h"
#include "Mesh.h"

//For bit_wise operations:
//b3 b2 b1
#define _BN	0x00 //nothing
#define _B1	0x01
#define _B2	0x02
#define _B3	0x04

//Inicializaci�n de constantes Static:
//TList* Face::FaceList = new TList;

//Arrays para determinar el right, left y opposite index de cada nodo asiciado a uno
//de sus edges:
TList* Face::face_list_ =  NULL; //new TList;
int Face::max_id_       = -1;

//Constructor por defecto----------------------------------------------------

Face::Face() {
  id_ = ++max_id_;
  
  face_list_->Add(this); //Cuidado, lo que se crea, debe asignarse a alg�n MasterMesh

  left_node_  = NULL;
  right_node_ = NULL;
  opp_node_   = NULL;

  left_edge_  = NULL;
  right_edge_ = NULL;
  down_edge_  = NULL;

  is_visited_ = false;
  is_active_  = false;
  is_newborn_ = true;
}
//Constructor por par�metros----------------------------------------------------

Face::Face(Node* nL, Node* nR, Node* nO, Edge* eD, Edge* eR, Edge* eL) {
  id_ = ++max_id_;
  face_list_->Add(this); //Cuidado, lo que se crea, debe asignarse a alg�n MasterMesh

  //Connects this face to the nodes:
  left_node_  = nL;
  right_node_ = nR;
  opp_node_   = nO;

  //Connects the nodes to this face:
  left_node_->face_list_->Add(this);
  right_node_->face_list_->Add(this);
  opp_node_->face_list_->Add(this);

  //Connects this face to the edges:
  left_edge_  = eL;
  right_edge_ = eR;
  down_edge_  = eD;

  //Connects the Edges to this face through up/downEdge pointers, searching for the first free:
  //default = 'up':

  left_edge_->AddFace(this);
  right_edge_->AddFace(this);
  down_edge_->AddFace(this);

  is_visited_ = false;
  is_active_  = true;
  is_newborn_ = true;

  //Precomputes normal when created
  ComputeNormal();
}

//Constructor Lento por Nodos-------------------------------------------------
Face::Face(Node* nL, Node* nR, Node* nO) {
  id_ = ++max_id_;
  face_list_->Add(this);

  //Connects this face to the nodes:
  left_node_  = nL;
  right_node_ = nR;
  opp_node_    = nO;

  //Connects the nodes to this face:
  left_node_->face_list_->Add(this);
  right_node_->face_list_->Add(this);
  opp_node_->face_list_->Add(this);

  //For each edge connected to leftNode, search if leftEdge already existes:
  //Auxiliary pointers:
  Node* aNode;
  Edge* aEdge;
  bool exists = false;

  for (int i=0; i<left_node_->edge_list_->Count; ++i) {
    aEdge = (Edge*)left_node_->edge_list_->Items[i];
    aNode = left_node_->GetOppositeNode(aEdge);

    if (aNode == nO) {
      left_edge_ = aEdge;
      exists = true;
      break;
    }
  }

  if (!exists) {
    aEdge = new Edge(left_node_, opp_node_);
    left_edge_ = aEdge;
  }

  exists = false;

  for (int i = 0; i < left_node_->edge_list_->Count; ++i) {
    aEdge = (Edge*)left_node_->edge_list_->Items[i];
    aNode = left_node_->GetOppositeNode(aEdge);

    if (aNode == nR) {
      down_edge_ = aEdge;
      exists = true;
      break;
    }
  }

  if (!exists) {
    aEdge = new Edge(left_node_, right_node_);
    down_edge_ = aEdge;
  }

  exists = false;

  for (int i = 0; i < right_node_->edge_list_->Count; ++i) {
    aEdge = (Edge*)right_node_->edge_list_->Items[i];
    aNode = right_node_->GetOppositeNode(aEdge);

    if (aNode == nO)  {
      right_edge_ = aEdge;
      exists = true;
      break;
    }
  }

  if (!exists) {
    aEdge = new Edge(right_node_, opp_node_);
    right_edge_ = aEdge;
  }

  //Connects the Edges to this face through up/downEdge pointers, searching for the first free:
  //default = 'up':
  left_edge_->AddFace(this);
  right_edge_->AddFace(this);
  down_edge_->AddFace(this);

  is_visited_ = false;
  is_active_  = true;
  is_newborn_ = true;

  //Precomputes normal when created
  ComputeNormal();
}

//------------------------------------------------------------------------------
//Destructor de la clase Face:
Face::~Face() {
  //El destructor de Face SUPONE que todas las caras y los nodos que apuntaban
  //hacia el, ya fueron adecuadamente redireccionadas para apuntar a otra cara,
  //o fueron destruidas de alguna forma:
  left_node_  = NULL;
  right_node_ = NULL;
  opp_node_   = NULL;

  left_edge_  = NULL;
  right_edge_ = NULL;
  down_edge_  = NULL;

  if(id_== max_id_)
    set_max_id( ((Face*)face_list_->Last())->id_ );

  face_list_->Remove(this);
}

//Setters:----------------------------------------------------------------------
void Face::ComputeNormal() {
  normal_[0] = (right_node_->position_[1] - left_node_->position_[1]) *
               (opp_node_->position_[2] - left_node_->position_[2]) -
               (right_node_->position_[2] - left_node_->position_[2]) *
               (opp_node_->position_[1] - left_node_->position_[1]);
  normal_[1] = (right_node_->position_[2] - left_node_->position_[2]) *
               (opp_node_->position_[0] - left_node_->position_[0]) -
               (right_node_->position_[0] - left_node_->position_[0]) *
               (opp_node_->position_[2] - left_node_->position_[2]);
  normal_[2] = (right_node_->position_[0] - left_node_->position_[0]) *
               (opp_node_->position_[1] - left_node_->position_[1]) -
               (right_node_->position_[1] - left_node_->position_[1]) *
               (opp_node_->position_[0] - left_node_->position_[0]);

  //fast inverse sqrt: http://www.lomont.org/Math/Papers/2003/InvSqrt.pdf
  //fast 1.0/sqrt(x)
  xif x;
  x.xf = (normal_[0]*normal_[0] +
             normal_[1]*normal_[1] +
             normal_[2]*normal_[2]);

  float xhalf = 0.5f * x.xf;
  x.xi = 0x5f3759df - (x.xi >> 1);  // initial guess for Newton's method
  x.xf = x.xf*(1.5f - xhalf*x.xf*x.xf);   // One round of Newton's method

  normal_[0] *= x.xf;
  normal_[1] *= x.xf;
  normal_[2] *= x.xf;
}
//------------------------------------------------------------------------------

void Face::ComputeMassCenter() {
  mass_center_[0] = (right_node_->position_[0] + left_node_->position_[0] +
                     opp_node_->position_[0]) * 0.3333;
  mass_center_[1] = (right_node_->position_[1] + left_node_->position_[1] +
                     opp_node_->position_[1]) * 0.3333;
  mass_center_[2] = (right_node_->position_[2] + left_node_->position_[2] +
                     opp_node_->position_[2]) * 0.3333;
}
//Getters:----------------------------------------------------------------------
//void Face::setMaxId(int iVal) { max_id = iVal;}

//Linking-----------------------------------------------------------------------

void Face::ReplaceEdge(Edge* prevE, Edge* newE) {
  //Replace Edge makes an (unsafe) replacement of one face edge for another.
  //It does not link the new edge to 'this', nor prevents that prevE wasnt part
  //of this face:
  prevE == down_edge_ ? down_edge_ = newE : (prevE == right_edge_ ?
           right_edge_ = newE : left_edge_ = newE);
}
//-------------------------------------------------------------------------------

void Face::ReplaceNode(Node* prevN, Node* newN) {
  //Replace Node makes an (unsafe) replacement of one face node for another.
  //It does not link the new node to 'this', nor prevents that prevN wasnt part
  //of this face:
  prevN == left_node_ ? left_node_=newN : (prevN == right_node_ ?
           right_node_ = newN : opp_node_ = newN);
}

//------------------------------------------------------------------------------

Face* Face::Bisection(Edge* bE) {
  Node *nO, *nL, *nR;
  Edge *eR, *eL;

  DownLabelling(&bE, &eR, &eL, &nL, &nR, &nO);

  //Si el lado ya fue biseccionado en otro momento, est� atado al
  //nuevo nodo, y las nuevas caras deben ser "cosidas" entre ellas.
  //Finalmente, el lado a biseccionar debe desaparecer tras la costura:

  //Creo el nuevo nodo en la mitad de biEdge:
  Node* nNew;

  if (!bE->need_sewing_) {
    nNew = new Node((nR->position_[0] + nL->position_[0]) * 0.5f,
                    (nR->position_[1] + nL->position_[1]) * 0.5f,
                    (nR->position_[2] + nL->position_[2]) * 0.5f);

    //Desvinculo a los nodos del biEdge:
    nR->edge_list_->Remove(bE);
    nL->edge_list_->Remove(bE);
  }
  else {
    nNew = bE->sew_node_;
  }

  //Desvinculo a la cara del biEdge, rightEdge y rightNode y visceversa:
  bE->RemoveFace(this);
  eR->RemoveFace(this);
  nR->face_list_->Remove(this);

  //Coso el biEdge suelto al nuevo nodo, para atarlo m�s tarde:
  Edge *newRE, *newLE, *newME;

  if (!bE->need_sewing_) {
    bE->sew_node_ = nNew;
    //Creo los nuevos Edges:
    newRE = new Edge(nR, nNew);
    newLE = new Edge(nNew, nL);
    newLE->AddFace(this);
    bE->need_sewing_ = true;
  }
  else {
    newRE = (Edge*)nNew->edge_list_->Items[1];
    newLE = (Edge*)nNew->edge_list_->Items[0];
    newLE->AddFace(this);
   delete bE;
  }

  newME = new Edge(nO, nNew);
  newME->AddFace(this);

  //Ato los lados restantes a esta cara:
  nNew->face_list_->Add(this);

  right_edge_ = newME;
  left_edge_  = eL;
  down_edge_  = newLE;

  opp_node_   = nO;
  left_node_  = nL;
  right_node_ = nNew;

  //Creamos la nueva cara que tapa el hueco:
  //Face::Face(Node* nL, Node* nR, Node* nO, Edge* eD, Edge* eR, Edge* eL)
  nNew->ComputeNormal();
  nR->ComputeNormal();
  nO->ComputeNormal();

  Face* newF = new Face(nNew, nR, nO, newRE, eR, newME);
  nNew->velocity_[0] = (nR->velocity_[0] + nL->velocity_[0]) * 0.5f;
  nNew->velocity_[1] = (nR->velocity_[1] + nL->velocity_[1]) * 0.5f;
  nNew->velocity_[2] = (nR->velocity_[2] + nL->velocity_[2]) * 0.5f;
  nNew->acceleration_[0] = (nR->acceleration_[0] + nL->acceleration_[0]) * 0.5f;
  nNew->acceleration_[1] = (nR->acceleration_[1] + nL->acceleration_[1]) * 0.5f;
  nNew->acceleration_[2] = (nR->acceleration_[2] + nL->acceleration_[2]) * 0.5f;

  return newF;
}
//----------------------------------------------------------------------------

void Face::Trisection(Edge* longer_edge, Edge* long_edge) {
  Face* newF;
  newF = Bisection(longer_edge);
  long_edge == left_edge_ ? Bisection(long_edge) : newF->Bisection(long_edge);
}
//----------------------------------------------------------------------------

void Face::Quadsection() {
  //Eliminamos los v�nculos de los nodos a la cara:
  right_node_->face_list_->Remove(this);
  left_node_-> face_list_->Remove(this);

  //Eliminamos los v�nculos de los lados a la cara:
  down_edge_->RemoveFace (this);
  right_edge_->RemoveFace(this);
  left_edge_->RemoveFace (this);

  //Creo los nuevos lados y nodos:
  //Creo el nuevo nodo en la mitad de los lados viejos y
  //coso los viejos lados a los nuevos nodos:
  //Si es necesario, creo los nuevos nodos. Si ya fue biseccionado,
  //uso los viejos:
  Node* nMidLR;
  if (!down_edge_->need_sewing_) {
    nMidLR = new Node((right_node_->position_[0] + left_node_->position_[0]) * 0.5f,
                      (right_node_->position_[1] + left_node_->position_[1]) * 0.5f,
                      (right_node_->position_[2] + left_node_->position_[2]) * 0.5f);
    down_edge_->sew_node_ = nMidLR;
    left_node_->edge_list_->Remove(down_edge_);
    right_node_->edge_list_->Remove(down_edge_);
  }
  else {
    nMidLR = down_edge_->sew_node_;
  }

  Node* nMidRO;
  if (!right_edge_->need_sewing_) {
    right_node_->edge_list_->Remove(right_edge_);
    opp_node_->edge_list_->Remove(right_edge_);
    nMidRO = new Node((right_node_->position_[0] + opp_node_->position_[0]) * 0.5,
                      (right_node_->position_[1] + opp_node_->position_[1]) * 0.5,
                      (right_node_->position_[2] + opp_node_->position_[2]) * 0.5);
    right_edge_->sew_node_ = nMidRO;
  }
  else {
  nMidRO = right_edge_->sew_node_;
  }

  Node* nMidOL;
  if(!left_edge_->need_sewing_) {
    nMidOL = new Node((opp_node_->position_[0] + left_node_->position_[0]) * 0.5,
                      (opp_node_->position_[1] + left_node_->position_[1]) * 0.5,
                      (opp_node_->position_[2] + left_node_->position_[2]) * 0.5);
    left_node_->edge_list_->Remove(left_edge_);
    opp_node_->edge_list_->Remove(left_edge_);

    left_edge_->sew_node_ = nMidOL;
  }
  else {
    nMidOL = left_edge_->sew_node_;
  }

  //Creo los 9 nuevos lados, o los que sean necesarios:
  Edge* newDRE;
  Edge* newDLE;

  if (!down_edge_->need_sewing_) {
    newDRE = new Edge(right_node_, nMidLR);
    newDLE = new Edge(nMidLR, left_node_);
    down_edge_->need_sewing_ = true;

    if (down_edge_->up_face_ == NULL && down_edge_->down_face_ == NULL)
      delete down_edge_;
  }
  else {
    newDRE = (Edge*)nMidLR->edge_list_->Items[1];
    newDLE = (Edge*)nMidLR->edge_list_->Items[0];
    delete down_edge_;
  }

  Edge *newRRE, *newRLE;
  if (!right_edge_->need_sewing_) {
    newRRE = new Edge(opp_node_, nMidRO);
    newRLE = new Edge(nMidRO, right_node_);
    right_edge_->need_sewing_ = true;

    if (right_edge_->up_face_ == NULL && right_edge_->down_face_ == NULL)
    delete right_edge_;
  }
  else {
    newRRE = (Edge*)nMidRO->edge_list_->Items[1];
    newRLE = (Edge*)nMidRO->edge_list_->Items[0];
    delete right_edge_;
  }

  Edge *newLRE, *newLLE;
  if (!left_edge_->need_sewing_) {
    newLRE = new Edge(left_node_, nMidOL);
    newLLE = new Edge(nMidOL, opp_node_);
    left_edge_->need_sewing_ = true;
  }
  else {
    newLRE = (Edge*)nMidOL->edge_list_->Items[1];
    newLLE = (Edge*)nMidOL->edge_list_->Items[0];
    delete left_edge_;
  }

  //Creo los lados internos de los tri�ngulos:
  Edge* newMidDR = new Edge(nMidLR, nMidRO);
  Edge* newMidRL = new Edge(nMidRO, nMidOL);
  Edge* newMidLD = new Edge(nMidOL, nMidLR);

  //Creo las nuevas caras y convierto esta cara en una de las nuevas:
  //Face(Node* nL, Node* nR, Node* nO, Edge* eD, Edge* eR, Edge* eL);
  //Creo las 3 nuevas caras:
  Face* pF;
  pF  = new Face(nMidLR, right_node_, nMidRO, newDRE, newRLE, newMidDR);
  pF  = new Face(left_node_, nMidLR, nMidOL, newDLE, newMidLD, newLRE);
  pF  = new Face(nMidRO, nMidOL, nMidLR, newMidRL, newMidLD, newMidDR);

  //Convierto esta cara:
  left_node_  = nMidOL;
  right_node_ = nMidRO;

  down_edge_  = newMidRL;
  right_edge_ = newRRE;
  left_edge_  = newLLE;

  nMidOL->face_list_->Add(this);
  nMidRO->face_list_->Add(this);

  newMidRL->AddFace(this);
  newRRE->AddFace(this);
  newLLE->AddFace(this);

  nMidLR->velocity_[0] = (right_node_->velocity_[0] + left_node_->velocity_[0]) * 0.5;
  nMidLR->velocity_[1] = (right_node_->velocity_[1] + left_node_->velocity_[1]) * 0.5;
  nMidLR->velocity_[2] = (right_node_->velocity_[2] + left_node_->velocity_[2]) * 0.5;

  nMidRO->velocity_[0] = (right_node_->velocity_[0] + opp_node_->velocity_[0]) * 0.5;
  nMidRO->velocity_[1] = (right_node_->velocity_[1] + opp_node_->velocity_[1]) * 0.5;
  nMidRO->velocity_[2] = (right_node_->velocity_[2] + opp_node_->velocity_[2]) * 0.5;

  nMidOL->velocity_[0] = (opp_node_->velocity_[0] + left_node_->velocity_[0]) * 0.5;
  nMidOL->velocity_[1] = (opp_node_->velocity_[1] + left_node_->velocity_[1]) * 0.5;
  nMidOL->velocity_[2] = (opp_node_->velocity_[2] + left_node_->velocity_[2]) * 0.5;

  nMidLR->acceleration_[0] = (right_node_->acceleration_[0] + left_node_->acceleration_[0]) * 0.5;
  nMidLR->acceleration_[1] = (right_node_->acceleration_[1] + left_node_->acceleration_[1]) * 0.5;
  nMidLR->acceleration_[2] = (right_node_->acceleration_[2] + left_node_->acceleration_[2]) * 0.5;

  nMidRO->acceleration_[0] = (right_node_->acceleration_[0] + opp_node_->acceleration_[0]) * 0.5;
  nMidRO->acceleration_[1] = (right_node_->acceleration_[1] + opp_node_->acceleration_[1]) * 0.5;
  nMidRO->acceleration_[2] = (right_node_->acceleration_[2] + opp_node_->acceleration_[2]) * 0.5;

  nMidOL->acceleration_[0] = (opp_node_->acceleration_[0] + left_node_->acceleration_[0]) * 0.5;
  nMidOL->acceleration_[1] = (opp_node_->acceleration_[1] + left_node_->acceleration_[1]) * 0.5;
  nMidOL->acceleration_[2] = (opp_node_->acceleration_[2] + left_node_->acceleration_[2]) * 0.5;
}
//----------------------------------------------------------------------------

bool Face::Flattening(Edge* eD) {
  Node *nO, *nL, *nR;
  Edge *eR, *eL;
  DownLabelling(&eD, &eR,&eL,&nL,&nR,&nO);

  //nO is the Node to be flattened, with Connectivity = 3. If not the case,
  //flattening is avoided:
  if (nO->face_list_->Count != 3)
    return false;
  Edge* eM;

  for (int i = 0; i < nO->edge_list_->Count; ++i)
    if ((Edge*)nO->edge_list_->Items[i] != eL && (Edge*)nO->edge_list_->Items[i] != eR)
      eM = (Edge*)nO->edge_list_->Items[i];

  if (!eM->EdgeMelting(0))
    return false;

  return true;
}
//---------------------------------------------------------------------------

short int Face::TriangleMelting() {
  //Connectivity preservation:
  Node *n1, *n2, *n3;

  n1 = down_edge_->get_opposite_node(down_edge_->get_neighbour_face(this));
  n2 = right_edge_->get_opposite_node(right_edge_->get_neighbour_face(this));
  n3 = left_edge_->get_opposite_node(left_edge_->get_neighbour_face(this));

  //Triangle melting would cause mesh degeneration:
  if (n1 == n2 || n1 == n3 || n2 == n3)
    return -1;  //return failure, 2 triangle faces are neighbours. Proceed to flatten or edge-melt

  //Connectivity will decrease by one for each node:
  int cn1 = n1->edge_list_->Count -1;
  int cn2 = n2->edge_list_->Count -1;
  int cn3 = n3->edge_list_->Count -1;

  //Si afecta la conectividad l�mite de los nodos que conecta, la disoluci�n de
  //esta cara se rechaza:
  if(cn1<3 || cn2<3 || cn3<3)
    return 0; //Triangle melting was prevented to avoid connectivity issues

  //Switch face up/face down from eM so that FD always points to it:
  //eM: edge to melt
  Edge* eM = down_edge_;

 if (eM->up_face_ == this) {
   eM->up_face_   = eM->down_face_;
   eM->down_face_ = this;
 }

 Edge* neM; //next edge to melt

 eM->left_node_->position_[0] = (left_node_->position_[0] + right_node_->position_[0] +
                           opp_node_->position_[0]) * 0.333f;
 eM->left_node_->position_[1] = (left_node_->position_[1] + right_node_->position_[1] +
                           opp_node_->position_[1]) * 0.333f;
 eM->left_node_->position_[2] = (left_node_->position_[2] + right_node_->position_[2] +
                           opp_node_->position_[2]) * 0.333f;

 eM->right_node_->position_[0] = eM->left_node_->position_[0];
 eM->right_node_->position_[1] = eM->left_node_->position_[1];
 eM->right_node_->position_[2] = eM->left_node_->position_[2];

 neM = (eM == down_edge_ ? right_edge_ : ((eM == right_edge_) ?
        left_edge_ : down_edge_ ));

 neM->left_node_->position_[0] = eM->left_node_->position_[0];
 neM->left_node_->position_[1] = eM->left_node_->position_[1];
 neM->left_node_->position_[2] = eM->left_node_->position_[2];

 neM->right_node_->position_[0] = eM->left_node_->position_[0];
 neM->right_node_->position_[1] = eM->left_node_->position_[1];
 neM->right_node_->position_[2] = eM->left_node_->position_[2];

 if (eM->EdgeMelting(1) && neM->EdgeMelting(1))
   return 1;  //Return success
 else
   return 0;
}

//---------------------------------------------------------------------------

void Face::DownLabelling(Edge **eD, Edge **eR, Edge **eL, Node **nL, Node **nR,
                         Node **nO) {
  //down_labelling: correctly labels right, down and left edges as well as
  //right, left and opposite nodes from the real position of the selected eD
  //("down" edge):
  char pos;
  *eD == down_edge_ ? pos = 'd' : (*eD == left_edge_ ? pos = 'l' : pos = 'r');

  switch (pos) {
   case 'l': {
     *nO = right_node_;
     *nL = opp_node_;
     *nR = left_node_;
     *eR = down_edge_;
     *eL = right_edge_;
     break;
   }

   case 'r': {
    *nO = left_node_;
    *nL = right_node_;
    *nR = opp_node_;
    *eL = down_edge_;
    *eR = left_edge_;
    break;
   }

   case 'd': {
    *nO = opp_node_;
    *nL = left_node_;
    *nR = right_node_;
    *eL = left_edge_;
    *eR = right_edge_;
    break;
   }
  }
}
//---------------------------------------------------------------------------

double Face::ComputeSignedVolume() {
  signed_volume_ = 0.1667f  * (-opp_node_->position_[0]*right_node_->position_[1] *
        left_node_->position_[2]  + right_node_->position_[0]*opp_node_->position_[1]   *
        left_node_->position_[2]  + opp_node_->position_[0]*left_node_->position_[1]    *
        right_node_->position_[2] - left_node_->position_[0]*opp_node_->position_[1]    *
        right_node_->position_[2] - right_node_->position_[0]*left_node_->position_[1]  *
        opp_node_->position_[2]   + left_node_->position_[0]*right_node_->position_[1]  *
        opp_node_->position_[2]);
  return signed_volume_;
}
//---------------------------------------------------------------------------

void Face::Deactivate() {
  opp_node_->is_active_   = false;
  left_node_->is_active_  = false;
  right_node_->is_active_ = false;
  is_active_ = false;
}
//----------------------------------------------------------------------------

void Face::Activate() {
  opp_node_->is_active_  = true;
  left_node_->is_active_ = true;
  right_node_->is_active_= true;
  is_active_ = true;
}
//----------------------------------------------------------------------------

const Node* Face::get_left_node() const {
  return left_node_;
}
//----------------------------------------------------------------------------

const Node* Face::get_right_node() const {
  return right_node_;
}
//-----------------------------------------------------------------------------

const Node* Face::get_opp_node() const {
  return opp_node_;
}
//-----------------------------------------------------------------------------

const Edge* Face::get_left_edge() const {
  return left_edge_;
}
//-----------------------------------------------------------------------------

const Edge* Face::get_right_edge() const {
  return right_edge_;
}
//-----------------------------------------------------------------------------

const Edge* Face::get_down_edge() const {
  return down_edge_;
}
//-----------------------------------------------------------------------------








