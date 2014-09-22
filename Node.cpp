#include "Node.h"
#include "Edge.h"
#include "Face.h"

//Inicialización de los atributos static:
TList* Node::node_list_     =  NULL; //new TList;
int    Node::max_id_        =  -1;
double Node::rest_distance_ =  0.0f;

double Node::stretch_coeff_ =  0.1f;  //coef de estiramiento: O<---->O
double Node::bend_coeff_    =  1.5f; //Coef de curvatura: O--O--O--O--O
double Node::ball_coeff_    =  1.5f;  //coef de inflado    O->
double Node::edge_coeff_    =  0.0f;  //coeficiente de fuerza de bordes ->O<-

//Seteo de las condiciones iniciales:
double Node::mass_       = 3.0f;
double Node::inv_mass_   = 0.333f;
double Node::damp_coeff_ = 1.5f;

double Node::max_intensity_thresh_  =  1.0f;    //Threshold Mayor para considerar fuerza de inflado
double Node::min_intensity_thresh_  = -1.0f;    //hreshold menor para considerar fuerza de inflado

double Node::scale_x_ =  1.0f;
double Node::scale_y_ =  1.0f;
double Node::scale_z_ =  1.0f;

double Node::bound_plane_normal_1_[3] =  {0.0, 0.0, 1.0};
double Node::bound_plane_normal_2_[3] =  {0.0, 0.0, -1.0};
double Node::dot_1_     =  1.0f;
double Node::dot_2_     =  1.0f;

bool Node::dark_background_  =  true;
double Node::min_movement_thresh_ =  1.0f;

short int Node::max_inactive_iterations_ = 1.0f;
int Node::total_inactive_ = 0;

double Node::bounding_box_[2][3] = {{0, 0, 0},{512, 512, 512}};

//------------------------------------------------------------------------------
Node::Node(double x, double y, double z) {

  //Sets Node id:
  //setea el max id: 0-start indexing
  id_ = ++max_id_;
  node_list_->Add(this);

  position_[0] = x;
  position_[1] = y;
  position_[2] = z;

  //Inicializa las listas de edge_list_ y pNode
  edge_list_ = new TList;
  face_list_ = new TList;

  //Por defecto no fue visitado nunca:
  this->is_visited_ = false;
  this->is_active_  = true;
  this->is_moving_ = true;

  //Lo inicializa en reposo:
  acceleration_[0] = 0;
  acceleration_[1] = 0;
  acceleration_[2] = 0;
  velocity_[0] = 0;
  velocity_[1] = 0;
  velocity_[2] = 0;

  //Por defecto, aun no actúan fuerzas sobre el:
  force_[0] = 0;
  force_[1] = 0;
  force_[2] = 0;
  this->ComputeNormal();
  is_interface_ = false;
  inactive_times_ = 0;
}
//-----------------------------------------------------------------------------

Node::~Node() {
  //Desvincula a este nodo de los edges a los cuales era parte:
  edge_list_->Clear();
  delete edge_list_;

  //Desvincula a este nodo de las caras a las cuales era parte:
  face_list_->Clear();
  delete face_list_;

  //Elimina el puntero a este nodo del node_list_:
  //Caso más complejo, ya que debe buscar en toda la lista para eliminarlo
  if(id_ == this->get_max_id())
    this->set_max_id( ((Node*)node_list_->Last())->id_ );

  node_list_->Remove(this);
}
//------------------------------------------------------------------------------

bool Node::operator==(const Node &otherNode) const {
  if(this == &otherNode)
    return true;
  else
    return false;
}

//------------------------------------------------------------------------------

Node* Node::GetOppositeNode(Edge* edge) {
  //Gets the opposite Node from 'this' along the Edge edge:
  return (edge->left_node_ == this ? edge->right_node_ :
         (edge->right_node_ == this ? edge->left_node_ :
          NULL));
}
//----------------------------------------------------------------------------

Edge* Node::GetOppositeEdge(Face* face) {
  //Gets the opposite Edge from 'this' inside the Face face:
  return (face->opp_node_== this ? face->down_edge_ :
         (face->left_node_ == this? face->right_edge_ :
          face->left_edge_));
}

//---------------------------------------------------------------------------

bool Node::Mimic(Node* node) {
  //Mimic method copies both lists (edge_list_ and face_list_) from node to 'this'.
  //Mimic also places 'this' between this->position_ and node->position_:

  //Avoid strangling:
  //Strangling is detected as two nodes sharing an extra node that is not part
  //of a mesh triangle, but is a triangular shaped throat:
  Node* op_node;   //node opposite to this
  Node* op_op_node; //node opposite to op_node
  for (int i = 0; i < edge_list_->Count; ++i) {
    op_node = GetOppositeNode((Edge*)edge_list_->Items[i]);
    for (int j = 0; j < node->edge_list_->Count; ++j) {
      op_op_node = op_node->GetOppositeNode( (Edge*)node->edge_list_->Items[j] );
      if (op_op_node == node)
        return false; //Strangling must be prevented
    }
  }

  //TODO:
  //This shouldn't happen anymore: REMOVE with caution:
  //(if they share more than one edge, trouble):
  for (int i = 0; i < node->edge_list_->Count; ++i) {
    if (edge_list_->IndexOf(node->edge_list_->Items[i]) != -1)
      return false;
  }

  for (int i = 0; i < node->face_list_->Count; ++i)
    face_list_->Add(node->face_list_->Items[i]);

  for (int i = 0; i < node->edge_list_->Count; ++i)
    edge_list_->Add(node->edge_list_->Items[i]);

  //Average position:
  position_[0] += node->position_[0];
  position_[1] += node->position_[1];
  position_[2] += node->position_[2];
  position_[0] *= 0.5f;
  position_[1] *= 0.5f;
  position_[2] *= 0.5f;

  //Average velocity:
  velocity_[0] += node->velocity_[0];
  velocity_[1] += node->velocity_[1];
  velocity_[2] += node->velocity_[2];
  velocity_[0] *= 0.5f;
  velocity_[1] *= 0.5f;
  velocity_[2] *= 0.5f;

  //Average acceleration:
  acceleration_[0] += node->acceleration_[0];
  acceleration_[1] += node->acceleration_[1];
  acceleration_[2] += node->acceleration_[2];
  acceleration_[0] *= 0.5f;
  acceleration_[1] *= 0.5f;
  acceleration_[2] *= 0.5f;
  
  return true;
}

//---------------------------------------------------------------------------

void Node::ComputeMassCenter() {
  Edge* pE; //puntero auxiliar a Edge
  Node* pN; //puntero auxiliar a Node

  mass_center_[0] = 0.0f;
  mass_center_[1] = 0.0f;
  mass_center_[2] = 0.0f;

  double n = 1.0f/edge_list_->Count;

  for (int i = 0; i < edge_list_->Count; ++i) {
    pE = (Edge*)edge_list_->Items[i];
    pN = this->GetOppositeNode(pE);
    //Cálculo del centro de gravedad:
    mass_center_[0]+=pN->position_[0];
    mass_center_[1]+=pN->position_[1];
    mass_center_[2]+=pN->position_[2];
  }

  mass_center_[0] *= n;
  mass_center_[1] *= n;
  mass_center_[2] *= n;
}

//------------------------------------------------------------------------------

void Node::InternalForce() {
  //ATENCION!!!!!!!!!!!!!!!!!!
  //El cómputo de las fuerzas requiere que previamente se resetee
  //el valor de force_[3] a 0;
  StretchingForce ();
  BendingForce    ();
  DissipationForce();
}

//------------------------------------------------------------------------------

void Node::StretchingForce() {
  Edge* pE; //puntero auxiliar a Edge
  Node* pN; //puntero auxiliar a Node

  double distance;     //distancia internodos
  double dif_drest; //magnitud de la fuerza de stretch
  double delta;
  double n = stretch_coeff_/edge_list_->Count; //normalization coefficient
  double cx = 0, cy = 0, cz = 0; //auxiliares para la dirección de la fuerza internodos

  for (int i = 0; i < edge_list_->Count; ++i) {
    pE = (Edge*)edge_list_->Items[i];
    pN = GetOppositeNode(pE);

    distance = sqrt((position_[0]-pN->position_[0])*(position_[0]-pN->position_[0])+
                    (position_[1]-pN->position_[1])*(position_[1]-pN->position_[1])+
                    (position_[2]-pN->position_[2])*(position_[2]-pN->position_[2]));
    delta =(distance-rest_distance_)/distance;

    cx += (pN->position_[0]-position_[0]) * delta;
    cy += (pN->position_[1]-position_[1]) * delta;
    cz += (pN->position_[2]-position_[2]) * delta;
  }

  force_[0] += cx * n;
  force_[1] += cy * n;
  force_[2] += cz * n;
}
//------------------------------------------------------------------------------

void Node::BendingForce() {
  //ATENCION!!!!!!!!!!!!!!!!!!
  //El cómputo del bending force requiere el cálculo previo de todos los
  //cálculos de los centros de gravedad de la vecindad de los nodos:

  Edge* pE; //punteros auxiliares a Edge
  Node* pN; //punteros auxiliares a Node

  double  cx = .0f,  cy = .0f,  cz = .0f; //auxiliar para el centro de gravedad
  double  dcx =.0f, dcy = .0f, dcz = .0f; //auxiliar para el centro de gravedad de vecinos
  double  n =  1.0f/edge_list_->Count;

  cx = mass_center_[0]-position_[0];
  cy = mass_center_[1]-position_[1];
  cz = mass_center_[2]-position_[2];

  for (int i = 0; i < edge_list_->Count; ++i) {
    pE = (Edge*)edge_list_->Items[i];
    pN = this->GetOppositeNode(pE);

    dcx += (pN->mass_center_[0]-pN->position_[0]);
    dcy += (pN->mass_center_[1]-pN->position_[1]);
    dcz += (pN->mass_center_[2]-pN->position_[2]);
  }

  dcx *= n;
  dcy *= n;
  dcz *= n;
  cx  -= dcx;
  cy  -= dcy;
  cz  -= dcz;

  force_[0] += cx * bend_coeff_;
  force_[1] += cy * bend_coeff_;
  force_[2] += cz * bend_coeff_;
}
//------------------------------------------------------------------------------

void Node::ExternalForce(double intensity, double fx, double fy, double fz) {
  BalloonForce(intensity);
  //edgeForce(fx,fy,fz);
}
//------------------------------------------------------------------------------

void Node::RefreshPosition() {
  //Reemplaza xyz por el nuevo xyz:
  if (OffLimits()) {
    is_interface_ = true;
    return;
  }

  is_interface_ = false;

  //compute the distance the node is going to move:
  double delta_x = (next_position_[0]-position_[0]);
  double delta_y = (next_position_[1]-position_[1]);
  double delta_z = (next_position_[2]-position_[2]);
  double move_distance = sqrt(delta_x*delta_x + delta_y*delta_y +delta_z*delta_z);

  if (move_distance < min_movement_thresh_) {
    ++inactive_times_;

    if (inactive_times_ > max_inactive_iterations_) {
      //this->active = false;
      is_moving_ = false;
      ++total_inactive_;  //Total inactive is reset to 0 after mesh activation
    }
  }
  else {
    inactive_times_ = 0;
    is_moving_ = true;
    //this->active = true;
  }

  if (is_moving_) {
    position_[0] = next_position_[0];
    position_[1] = next_position_[1];
    position_[2] = next_position_[2];

    next_position_[0]<bounding_box_[0][0] ? bounding_box_[0][0]=next_position_[0] : false;
    next_position_[1]<bounding_box_[0][1] ? bounding_box_[0][1]=next_position_[1] : false;
    next_position_[2]<bounding_box_[0][2] ? bounding_box_[0][2]=next_position_[2] : false;

    next_position_[0]>bounding_box_[1][0] ? bounding_box_[1][0]=next_position_[0] : false;
    next_position_[1]>bounding_box_[1][1] ? bounding_box_[1][1]=next_position_[1] : false;
    next_position_[2]>bounding_box_[1][2] ? bounding_box_[1][2]=next_position_[2] : false;
  }
}

//------------------------------------------------------------------------------

void Node::ComputeNormal() {
  Face* pF;

  double nx=0,ny=0,nz=0; //auxiliares para el promedio de las normales:

  //Ciudado! Empieza pidiendo que calcule la normal, aunque aún no esté asociado a ninguna cara!!!
  //         o aunque las caras aún no hayan calculado su normal!!!!
  //Recordar: el orden de creación es: Nodo, Edge, Cara!

  //Si es un nodo que aún no está asociado a ninguna cara, no hay nada que calcular...
  if(!face_list_->Count)
    return;

  for(int i=0; i<face_list_->Count; i++) {
    pF = (Face*)face_list_->Items[i];
    nx +=pF->normal_[0];
    ny +=pF->normal_[1];
    nz +=pF->normal_[2];
  }

  //The vector is computed by normalizing the sum of the normal vectors of adjacent triangles:
  double norma;
  double n;
  norma = sqrt(nx*nx+ny*ny+nz*nz);
  if (norma) {
   n = 1.0f/norma;
   normal_[0] = nx*n;
   normal_[1] = ny*n;
   normal_[2] = nz*n;
  }
  else {
    is_active_ = false;
  }
}
//-----------------------------------------------------------------------------

bool Node::OffLimits() {
    double v[3];
    double xin;
    double vn;
    double lambdaPi;
    bool offLimit1 = false;
    bool offLimit2 = false;

    v[0] = next_position_[0] - position_[0];
    v[1] = next_position_[1] - position_[1];
    v[2] = next_position_[2] - position_[2];

    xin = position_[0]*bound_plane_normal_1_[0] + position_[1]*bound_plane_normal_1_[1] + position_[2]*bound_plane_normal_1_[2];
    vn = v[0]*bound_plane_normal_1_[0]   +   v[1]*bound_plane_normal_1_[1] +   v[2]*bound_plane_normal_1_[2];

    //if perpendicular to bounding plane, it is on limit for this plane:
    if (vn == 0) {
      offLimit1 = false;
      lambdaPi = 2.0f;
    }
    else {
      lambdaPi = (dot_1_ - xin)/vn;
    }

    if(fabs(lambdaPi) < 1.0f)
      offLimit1 = true;

    //repeat for second bounding plane:
    xin = position_[0]*bound_plane_normal_2_[0] + position_[1]*bound_plane_normal_2_[1] + position_[2]*bound_plane_normal_2_[2];
    vn  = v[0]*bound_plane_normal_2_[0]   +   v[1]*bound_plane_normal_2_[1] +   v[2]*bound_plane_normal_2_[2];

    //if perpendicular to bounding plane, it is on limit for this plane:
    if (!vn) {
      offLimit2 = false;
      lambdaPi = 2.0f;
    }
    else
      lambdaPi = (dot_2_ - xin)/vn;

    if (fabs(lambdaPi) < 1.0f)
      offLimit2 = true;

    if (offLimit1 || offLimit2)
      return true;
    else
      return false;

}
//------------------------------------------------------------------------------
