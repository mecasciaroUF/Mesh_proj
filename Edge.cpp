#include "Edge.h"
#include "Node.h"
#include "Face.h"

//Inicialización de los parámetros static:
TList* Edge::edge_list_    = NULL;//;new TList;
int    Edge::max_id_      = -1;

//------------------------------------------------------------------------------

Edge::Edge(Node* left_node, Node* right_node) {
  //Da identidad unívoca al nuevo edge:
  id_ = ++max_id_ ;

  //Agrega el Edge a la lista de todos los Edges:
  edge_list_->Add(this);

  //Vincula el edge a los Nodos pN1 y pN2
  left_node_  = left_node;
  right_node_ = right_node;
  sew_node_   = NULL;

  up_face_   = NULL;
  down_face_ = NULL;


  //Asocia los nodos utilizados a este nuevo Edge:
  left_node_->edge_list_->Add(this);
  right_node_->edge_list_->Add(this);

  is_visited_  = false;
  need_sewing_ = false;
}
//------------------------------------------------------------------------------

Edge::~Edge() {
//El destructor de Edge SUPONE que todas las caras y los nodos que apuntaban
//hacia el, ya fueron adecuadamente redireccionadas para apuntar a otro edge,
//o fueron destruidas de alguna forma:

  left_node_  = NULL;
  right_node_ = NULL;
  sew_node_   = NULL;

  up_face_    = NULL;
  down_face_  = NULL;

  if (id_ == get_max_id())
    max_id_ = ((Edge*)edge_list_->Last())->id_;

  //The pointer to this Edge is removed from EdgeList:
  edge_list_->Remove(this);
}
//------------------------------------------------------------------------------

void Edge::RemoveFace(char pos) {
  switch (pos)
  {
   case 'u':
    up_face_ = NULL;
    break;

  case 'd':
    down_face_ = NULL;
    break;
  }
}
//------------------------------------------------------------------------------

bool Edge::RemoveFace(Face* face) {
  if (face == up_face_) {
  up_face_ = NULL;
  return true;
  }

  if (face == down_face_) {
    down_face_ = NULL;
    return true;
  }
  //rF was not attached to this edge:
  return false;
}
//------------------------------------------------------------------------------

void Edge::AddFace(Face* face, char pos) {
  switch (pos) {
    case 'u':
     up_face_ = face;

    case 'd':
     down_face_ = face;
  }
}
//------------------------------------------------------------------------------

bool Edge::AddFace(Face* face) {
//Asocia con una cara en su primer posición libre:
//Por defecto, en upFace:
  if (up_face_ == NULL) {
    up_face_ = face;
    return true;
  }

  if (down_face_ == NULL) {
    down_face_ = face;
    return true;
  }

  //Edge already attached to 2 faces: WARNING!!
  return false;
}
//------------------------------------------------------------------------------

double Edge::ComputeLength() {
  float xl;
  float yl;
  float zl;
  float X;

  xl = left_node_->position_[0]-right_node_->position_[0];
  yl = left_node_->position_[1]-right_node_->position_[1];
  zl = left_node_->position_[2]-right_node_->position_[2];

  X = xl*xl + yl*yl + zl*zl;

  return sqrt(X);
  //TODO: optimize sqrt?
}
//------------------------------------------------------------------------------

bool Edge::Inversion() {
  //Si afecta la conectividad límite de los nodos que conecta, la inversión de
  //este lado se rechaza:
  if(right_node_->edge_list_->Count<4 || left_node_->edge_list_->Count<4)
    return false;

  Face* FU;
  Face* FD;

  FU = up_face_;
  FD = down_face_;

  //Searches the edge position of the shared edge within the faces:
  Edge *sE, *eLU, *eRU, *eLD, *eRD;
  Node *nL, *nR,  *nOU, *nOD;

  if (this == FU->down_edge_) {
    sE = FU->down_edge_;
    eLU = FU->left_edge_;
    eRU = FU->right_edge_;
    nL = FU->left_node_;
    nR = FU->right_node_;
    nOU = FU->opp_node_;
  }
  else if (this == FU->right_edge_) {
    sE = FU->right_edge_;
    eLU = FU->down_edge_;
    eRU = FU->left_edge_;
    nL = FU->right_node_;
    nR = FU->opp_node_;
    nOU = FU->left_node_;
  }
  else if (this == FU->left_edge_) {
    sE = FU->left_edge_;
    eLU = FU->right_edge_;
    eRU = FU->down_edge_;
    nL = FU->opp_node_;
    nR = FU->left_node_;
    nOU = FU->right_node_;
  }

  if (this == FD->down_edge_) {
    eLD = FD->left_edge_;
    eRD = FD->right_edge_;
    nOD = FD->opp_node_;
  }
  else if (this == FD->right_edge_) {
    eLD = FD->down_edge_;
    eRD = FD->left_edge_;
    nOD = FD->left_node_;
  }
  else if (this == FD->left_edge_) {
    eLD = FD->right_edge_;
    eRD = FD->down_edge_;
    nOD = FD->right_node_;
  }

  eRU->RemoveFace(FU);
  nR->face_list_->Remove(FU);
  nR->edge_list_->Remove(sE);

  if (this == FU->down_edge_) {
    FU->right_edge_ = NULL;
    FU->right_node_ = NULL;
  }
  else if (this == FU->right_edge_) {
    FU->left_edge_ = NULL;
    FU->opp_node_ = NULL;
  }
  else if(this == FU->left_edge_) {
    FU->down_edge_ = NULL;
    FU->left_node_ = NULL;
  }

  eRD->RemoveFace(FD);
  nL->face_list_->Remove(FD);
  nL->edge_list_->Remove(sE);

  sE->left_node_  = nOD;
  sE->right_node_ = nOU;

  FU->down_edge_ = sE;
  FU->right_edge_ = eLU;
  FU->left_edge_ = eRD;

  FU->left_node_ = nOD;
  FU->right_node_ = nOU;
  FU->opp_node_ = nL;

  nOD->edge_list_->Add(sE);
  eRD->AddFace(FU);
  nOD->face_list_->Add(FU);

  FD->down_edge_ = sE;
  FD->right_edge_ = eLD;
  FD->left_edge_ = eRU;

  FD->left_node_ = nOU;
  FD->right_node_ = nOD;
  FD->opp_node_ = nR;

  nOU->edge_list_->Add(sE);
  eRU->AddFace(FD);
  nOU->face_list_->Add(FD);

  nL->ComputeNormal();
  nR->ComputeNormal();
  nOU->ComputeNormal();
  nOD->ComputeNormal();

  return true;
}
//----------------------------------------------------------------------------

Face* Edge::get_neighbour_face(Face* face) {
//returns the pointer to the neighbour face sharing 'this', opposite to the
//reference face refF:
return (up_face_ == face? down_face_ : up_face_);
}

//----------------------------------------------------------------------------
Node* Edge::get_opposite_node(Face* face) {
//returns the pointer to the opposite node from 'this' inside the eference face
//refF:
//It doesn't check 'this' Edge to be one of refF edges:
return (face->down_edge_ == this? face->opp_node_ :
       (face->right_edge_ == this? face->left_node_ : face->right_node_));
}

//-----------------------------------------------------------------------------

bool Edge::EdgeMelting(bool in_depth) {

  Face* FU; //Up Face
  Face* FD; //Down Face

  Edge *eLU, *eRU, *eRD, *eLD; //left-up, right-up, left-down and right-down edges
  Node *nL, *nR, *nOU, *nOD;   //left, right, opposite-up and opposite down nodes
  Edge *eM = this;             //edge to be melted

  FU = up_face_;
  FD = down_face_;

  //down_labelling(Edge **eD, Edge **eR, Edge **eL, Node **nL, Node **nR, Node **nO)
  FU->DownLabelling(&eM, &eRU,&eLU,&nL,&nR,&nOU);
  FD->DownLabelling(&eM, &eLD,&eRD,&nR,&nL,&nOD);

  Face* FLU = eLU->get_neighbour_face(FU);
  Face* FRU = eRU->get_neighbour_face(FU);
  Face* FLD = eLD->get_neighbour_face(FD);
  Face* FRD = eRD->get_neighbour_face(FD);

  if (FRU == FLU || FLD == FRD)
    return false;

  if(FRU == FRD || FLU == FLD)
    return false;

  //Si afecta la conectividad límite de los nodos que conecta, la disolución de
  //este lado se rechaza:
  if (nOU->edge_list_->Count < 4) {
    if (in_depth) {
      if(FU->Flattening(eM))
        FU->DownLabelling(&eM,&eRU,&eLU,&nL,&nR,&nOU);
      else
        return false;
    }
    else {
      return false;
    }
  }

  if (nOD->edge_list_->Count<4) {
    if (in_depth) {
      if(FD->Flattening(eM))
        FD->DownLabelling(&eM, &eLD,&eRD,&nR,&nL,&nOD);
      else
        return false;
    }
    else {
      return false;
    }
  }

  nOU->face_list_->Remove(FU);
  nOD->face_list_->Remove(FD);
  nL->face_list_->Remove(FU);
  nL->face_list_->Remove(FD);
  nR->face_list_->Remove(FU);
  nR->face_list_->Remove(FD);
  nR->edge_list_->Remove(eRU);
  nR->edge_list_->Remove(eRD);
  nR->edge_list_->Remove(this);
  nL->edge_list_->Remove(this);

  nOU->edge_list_->Remove(eRU);
  nOD->edge_list_->Remove(eRD);

  //if unable to mimic, reverse action and return false:
  if (!nL->Mimic(nR)) {
    nOU->face_list_->Add(FU);
    nOD->face_list_->Add(FD);
    nL->face_list_->Add(FU);
    nL->face_list_->Add(FD);
    nR->face_list_->Add(FU);
    nR->face_list_->Add(FD);
    nR->edge_list_->Add(eRU);
    nR->edge_list_->Add(eRD);
    nR->edge_list_->Add(this);
    nL->edge_list_->Add(this);
    nOU->edge_list_->Add(eRU);
    nOD->edge_list_->Add(eRD);
    return false;
  }

  eLU->RemoveFace(FU);
  eLU->AddFace(FRU);
  eLD->RemoveFace(FD);
  eLD->AddFace(FRD);

  FRU->ReplaceEdge(eRU, eLU);
  FRD->ReplaceEdge(eRD, eLD);

  for (int i = 0; i < nR->face_list_->Count; ++i)
    ((Face*)nR->face_list_->Items[i] )->ReplaceNode(nR,nL);

  for(int i=0; i<nR->edge_list_->Count; i++)
    ((Edge*)nR->edge_list_->Items[i] )->ReplaceNode(nR,nL);

  nL->ComputeNormal();
  nOU->ComputeNormal();
  nOD->ComputeNormal();

  delete FU;
  delete FD;
  delete eRD;
  delete eRU;
  delete nR;

  delete this;

  return true;
}

//-----------------------------------------------------------------------------

void Edge::ReplaceNode(Node* prev_node, Node* new_node) {
  prev_node == left_node_? left_node_ = new_node : right_node_ = new_node;
}
//-----------------------------------------------------------------------------

