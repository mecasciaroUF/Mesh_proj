
#include "Mesh.h"
#include "Node.h"
#include "Edge.h"
#include "Face.h"

//---------------------------------------------------------------------------

Mesh::Mesh() {
  node_list_ = new TList;
  edge_list_ = new TList;
  face_list_ = new TList;

  //Seteo de las condiciones iniciales:
  stretch_coeff_ = -0.1; //coef de estiramiento: O<---->O
  bend_coeff_    =  3.5; //Coef de curvatura: O--O--O--O--O
  ball_coeff_    =  0.1; //coef de inflado    O->
  edge_coeff_    =  0.0; //coeficiente de fuerza de bordes ->O<-
  damp_coeff_    =  1.5;
  mass_          =  3.0;
  dark_background_ = true;
}
//---------------------------------------------------------------------------

Mesh::Mesh(TList* face_list, TList* edge_list, TList* node_list,
           double limit_edge_length, double scale_x, double scale_y,
           double scale_z) {
  //Crea una malla a partir de listas arbitrarias, previamente (y correctamente) enlazadas
  face_list_ = face_list;
  edge_list_ = edge_list;
  node_list_ = node_list;

  //Seteo de las condiciones iniciales:
  stretch_coeff_ = -0.1; //coef de estiramiento: O<---->O
  bend_coeff_    =  3.0; //Coef de curvatura: O--O--O--O--O
  ball_coeff_    =  0.2; //coef de inflado    O->
  edge_coeff_    =  0.0; //coeficiente de fuerza de bordes ->O<-
  damp_coeff_    =  1.5;
  mass_          =  3.0;
  dark_background_ = true;

  scale_x_ = scale_x;
  scale_y_ = scale_y;
  scale_z_ = scale_z;

  int faces_count;
  do {
    faces_count = face_list_->Count;
    for (int i=0; i < faces_count; ++i)
      ((Face*)face_list_->Items[i])->Quadsection() ;
  } while(ComputeMeanEdgeDistance() < limit_edge_length);

  min_length_ = 0.25*limit_edge_length;
  max_length_ =  2.0*limit_edge_length;
  euler_number_ =  node_list_->Count + face_list_->Count - edge_list_->Count;

  set_rest_distance(ComputeMeanEdgeDistance());

  ComputeFaceNormals();
}

//------------------------------------------------------------------------------

Mesh::Mesh(double limit_edge_length, double radius, double scale_x,
           double scale_y, double scale_z) {
//Crea una malla inicial circular: de radio radius:
  node_list_ = new TList;
  edge_list_ = new TList;
  face_list_ = new TList;

  //Seteo de las condiciones iniciales:
  stretch_coeff_ = 1.0; //coef de estiramiento: O<---->O
  bend_coeff_    = 2.0; //Coef de curvatura: O--O--O--O--O
  ball_coeff_    = 0.3; //coef de inflado    O->
  edge_coeff_    = 0.0; //coeficiente de fuerza de bordes ->O<-
  damp_coeff_    = 1.5;
  mass_          = 5.0;

  dark_background_ = true;

  //Subdivisions from a Icosaedron:
  //Índices del icosaedro:
  double X = .525731112119133606 ;
  double Z = .850650808352039932 ;

  // Mesh resolution:
  scale_x_ = scale_x;
  scale_y_ = scale_y;
  scale_z_ = scale_z;

  Node* aux_node_list[12];
  Edge* aux_edge_list[30];

  Node* pN;
  Edge* pE;
  Face* pF;

  pN->set_node_list(node_list_);
  pE->set_edge_list(edge_list_);
  pF->set_face_list(face_list_);

  double node_pos[12][3] = {{-X, 0.0, Z},{X, 0.0, Z},{-X, 0.0, -Z},{X, 0.0, -Z},
                            {0.0, Z, X},{0.0, Z, -X},{0.0, -Z, X},{0.0, -Z, -X},
                            {Z, X, 0.0},{-Z, X, 0.0},{Z, -X, 0.0},{-Z, -X, 0.0}};

  int edge_node_pos[30][2] = {{0,1},{1,4},{0,4},{4,9},{9,0},{1,6},{6,0},{6,11},
                              {11,0},{11,9},{11,7},{7,6},{6,10},{10,1},{7,10},
                              {10,8},{1,8},{8,4},{7,3},{3,10},{3,8},{4,5},{5,8},
                              {9,5},{3,5},{2,9},{11,2},{2,7},{3,2},{5,2}};


  int face_node_pos[20][3] = {{1,4,0},{4,9,0},{6,1,0},{11,6,0},{9,11,0},
                              {11,7,6},{10,1,6},{7,10,6},{10,8,1},{8,4,1},
                              {7,3,10},{3,8,10},{8,5,4},{5,9,4},{3,5,8},
                              {11,9,2},{7,11,2},{3,7,2},{5,3,2},{9,5,2}};

  int face_edge_pos[20][3] = {{1,2,0},{3,4,2},{5,0,6},{7,6,8},{9,8,4},
                              {10,11,7},{13,5,12},{14,12,11},{15,16,13},
                              {17,1,16},{18,19,14},{20,15,19},{22,21,17},
                              {23,3,21},{24,22,20},{9,25,26},{10,26,27},
                              {18,27,28},{24,28,29},{23,29,25}};

  //Posición de los 12 vértices:
  for (int i = 0; i < 12; ++i)
    aux_node_list[i] = new Node(node_pos[i][0], node_pos[i][1], node_pos[i][2]);

  //Creación de los lados del icosaedro:
  for (int i = 0; i < 30; ++i) {
    aux_edge_list[i] = new Edge(aux_node_list[edge_node_pos[i][0]],
                                aux_node_list[edge_node_pos[i][1]]);
  }

    //Creación de las 20 caras del icosaedro:
  for (int i = 0; i < 20; ++i) {
    pF = new Face(aux_node_list[face_node_pos[i][0]],
                  aux_node_list[face_node_pos[i][1]],
                  aux_node_list[face_node_pos[i][2]],
                  aux_edge_list[face_edge_pos[i][0]],
                  aux_edge_list[face_edge_pos[i][1]],
                  aux_edge_list[face_edge_pos[i][2]]);
  }


  // Normalización de la distancia de los nodos, y posterior proyección sobre el
  // radio de una esfera (si dX = dY = dZ), o un esferoide (si algún d es
  // diferente de los otros):
  float norma;

  for (int i = 0; i < node_list_->Count; ++i) {
    pN = (Node*) node_list_->Items[i];

    norma = sqrt(pN->position_[0]*pN->position_[0] + pN->position_[1]*pN->position_[1] +
                 pN->position_[2]*pN->position_[2]);
    pN->position_[0] /=norma;
    pN->position_[1] /=norma;
    pN->position_[2] /=norma;
    pN->position_[0] *=radius;
    pN->position_[1] *=radius;
    pN->position_[2] *=radius;
    pN->is_visited_ = true;
  }


  //Subdivision iterativa de las caras de la esfera mallada, hasta obtener la distancia
  //internodo promedio buscada (2*limitInterNodeDistance):
  int faces_count;

  do {
    faces_count = face_list_->Count;
    for(int i=0; i<faces_count; i++) {
      pF = (Face*)face_list_->Items[i];
      pF->Quadsection();
    }
  } while (ComputeMeanEdgeDistance() >= limit_edge_length);

 //Normalización de la distancia de los nodos, y posterior proyección sobre el radio
 //de una esfera (si dX = dY = dZ), o un esferoide (si algún d es diferente de los otros):
 for (int i = 0; i < node_list_->Count; ++i) {
     pN = (Node*)node_list_->Items[i];
     norma = sqrt(pN->position_[0]*pN->position_[0] + pN->position_[1]*pN->position_[1] +
                  pN->position_[2]*pN->position_[2]);
     pN->position_[0] /=norma;
     pN->position_[1] /=norma;
     pN->position_[2] /=norma;
     pN->position_[0] *=radius;
     pN->position_[1] *=radius;
     pN->position_[2] *=radius;
     pN->is_visited_=true;
    }

  min_length_ = 0.5*ComputeMeanEdgeDistance();
  max_length_ = 2.0*ComputeMeanEdgeDistance();
  set_rest_distance(ComputeMeanEdgeDistance());
  euler_number_ = node_list_->Count + face_list_->Count - edge_list_->Count;
  is_active_ = true;

  ComputeFaceNormals();
}
//---------------------------------------------------------------------------
Mesh::Mesh(double** center_line, int center_count, double** normal_vector,
           int normal_count, int angle_count, double radius,
           double limit_edge_length, double scale_x, double scale_y,
           double scale_z) {
  //nA: pasos angulares de las circunferencias en el centerline CL
  //Auxiliary indexes:
  int i;
  int j;
  int k;
  int m;

  double aux_point[3]; //auxiliary point
  double aux_normal[3];//auxiliary normal vector
  double n;            //normalization constant

  for ( i = 0; i < normal_count; ++i) {
    aux_normal[0] = normal_vector[i][0];
    aux_normal[1] = normal_vector[i][1];
    aux_normal[2] = normal_vector[i][2];
    n = 1.0f/sqrt(aux_normal[0]*aux_normal[0] + aux_normal[1]*aux_normal[1] +
                  aux_normal[2]*aux_normal[2]);
    normal_vector[i][0] *= n;
    normal_vector[i][0] *= n;
    normal_vector[i][0] *= n;
  }

  //%Parameter 't' (angle) going from 0º to 360º by a degº step:
  //t = 0:nA:360-nA;
  float delta_angle = 6.2832f/double(angle_count);
  float* t = new float[angle_count];

  for(i=0; i<angle_count; i++)
    t[i] = i*delta_angle;

  //circles_list initialization:
  double *** circle_list;
  circle_list = new double**[center_count];
  for (i = 0; i < center_count; ++i) {
    circle_list[i] = new double*[angle_count];
    for (j = 0; j < angle_count; ++j) {
      circle_list[i][j] = new double[3];
    }
  }

  double angle;  //angle to rotate
  double axis[3]; //rotation axis
  double axis_n; //axis normalization factor
  double x,y,z;  //auxiliar coordinates
  double rx,ry,rz; //auxiliar rotated coordinates

  //Rotation matrix coefficient:
  double c,s;  //cosine and sine
  //double rot_matrix[3][3];

  for (i = 0; i < center_count; ++i) {
    angle       = -acos(normal_vector[i][2]); //normal_vector_z
    axis[0] = -normal_vector[i][1];       //normal_vector_y
    axis[1] =  normal_vector[i][0];       //normal_vector_x
    axis[2] =  0.0;

    axis_n  = 1.0f/sqrt(axis[0]*axis[0] + axis[1]*axis[1] + axis[2]*axis[2]);
    axis[0] *= axis_n;
    axis[1] *= axis_n;
    axis[2] *= axis_n;

    c = cos(angle);
    s = sin(angle);
    double rot_matrix[3][3]
                    = { {axis[0]*axis[0]+(1.0-axis[0]*axis[0])*c,
                    axis[0]*axis[1]*(1.0-c)-axis[2]*s,
                    axis[0]*axis[2]*(1.0-c)+axis[1]*s},
                   {axis[0]*axis[1]*(1.0-c)+axis[2]*s,
                    axis[1]*axis[1]+(1.0-axis[1]*axis[1])*c,
                    axis[1]*axis[2]*(1.0-c)-axis[0]*s},
                   {axis[0]*axis[2]*(1.0-c)-axis[1]*s,
                    axis[1]*axis[2]*(1.0-c)+axis[0]*s,
                    axis[2]*axis[2]+(1.0-axis[2]*axis[2])*c} };

    for( j = 0; j < angle_count; ++j) {
      x = radius*sin(t[j]);
      y = radius*cos(t[j]);
      z = .0f;

      //Re-orientate circles to be in the plane perpendicular to normal_vector,
      //centered in center_point:
      rx = x * rot_matrix[0][0] + y * rot_matrix[1][0] + z * rot_matrix[2][0];
      ry = x * rot_matrix[0][1] + y * rot_matrix[1][1] + z * rot_matrix[2][1];
      rz = x * rot_matrix[0][2] + y * rot_matrix[1][2] + z * rot_matrix[2][2];

      circle_list[i][j][0] = rx + center_line[i][0];
      circle_list[i][j][1] = ry + center_line[i][1];
      circle_list[i][j][2] = rz + center_line[i][2];
    }
  }

  //3-Search for nearest succesive circle point and perform post-alignment:
  double dist;    //distance between two points
  double min_dist;//min distance between a point and all other points in a circle
  int i_close = 0; //index to the closest point
  double half_angle = delta_angle/2.0f;
  double xn, yn, zn;

  for (i = 0; i < center_count-1; ++i) {
    //aL   = (TList*)Circs->Items[i];   //Look at this circumference and...
    //aL_N = (TList*)Circs->Items[i+1]; //... next circumference.

    //Take the first point of the circumference...
    x = circle_list[i][0][0];
    y = circle_list[i][0][1];
    z = circle_list[i][0][2];

    //... and compare it with all points of the second circumference:
    xn = circle_list[i+1][0][0];
    yn = circle_list[i+1][0][1];
    zn = circle_list[i+1][0][2];

    dist     = 0;
    i_close  = 0;
    min_dist = sqrt(pow(xn-x,2.0) + pow(yn-y,2.0) + pow(zn-z,2.0));

    for (j = 0; j < angle_count; ++j) {
      xn = circle_list[i+1][0][0];
      yn = circle_list[i+1][0][1];
      zn = circle_list[i+1][0][2];
      dist = sqrt(pow(xn-x,2.0) + pow(yn-y,2.0) + pow(zn-z,2.0));

      if (dist <= min_dist) {
        min_dist = dist;
        i_close = j;
      }
    }

    //The closest point of the next circumference is in position i_close, so
    //i need to perform i_close forward-shifts to get i_close to 0 index:
    for(k = 0; k < angle_count - i_close; ++k) {
      for(m = angle_count - 1; m > 0; m--) {
        xn = circle_list[i+1][m+1][0];
        yn = circle_list[i+1][m+1][1];
        zn = circle_list[i+1][m+1][2];

        circle_list[i+1][m+1][0] = circle_list[i+1][m][0];
        circle_list[i+1][m+1][1] = circle_list[i+1][m][1];
        circle_list[i+1][m+1][2] = circle_list[i+1][m][2];

        circle_list[i+1][m][0] = xn;
        circle_list[i+1][m][1] = yn;
        circle_list[i+1][m][2] = zn;
      }
    }
  }

  //All CenterLine lying circumferences are now aligned in a way that no triangles
  //will be pre-intersecting.
  //Mesh algorithm begins:

  //The triangular mesh scheme relies on a 3-coordintate V (vertex) vector
  //corresponding to circular coordinates, and a T (triangle) vector
  //corresponding to the indexes of V that form a triangular face:

  //int n_circ = aL->Count; //amount of points by circle

  Node *aN1, *aN2, *aN3;
  Face *aF;
  Edge *aE;

  face_list_ = new TList;
  edge_list_ = new TList;
  node_list_ = new TList;

  aN1->set_node_list( node_list_ );
  aF->set_face_list(  face_list_ );
  aE->set_edge_list(  edge_list_ );

  //For every circle point, create a Node:
  for (i = 0; i < center_count; ++i) {
    for (j = 0; j < angle_count; ++j) {
      x = circle_list[i][j][0];
      y = circle_list[i][j][1];
      z = circle_list[i][j][2];
      new Node(x, y, z);
    }
  }

  for(i = 0; i < center_count-1; ++i) {
    //Triangle triplet indexes:
    //(i)*nCirc + j, (i+1)*nCirc+j, (i)*nCirc + (j+1)
    for(j = 0; j < angle_count-1; ++j) {
      aN1 = (Node*)node_list_->Items[ i*angle_count + j ];
      aN2 = (Node*)node_list_->Items[(i+1)*angle_count + j ];
      aN3 = (Node*)node_list_->Items[ i*angle_count + (j+1) ];

       // Face(Node* nL, Node* nR, Node* nO, Edge* eD, Edge* eR, Edge* eL);
       aF = new Face(aN3, aN1, aN2);

       //Inverted Triangles triplet indexes:
       //(i)*nCirc + (j+1), (i+1)*nCirc+j, (i+1)*nCirc + (j+1)
       aN1 = (Node*)node_list_->Items[ i*angle_count + (j+1) ];
       aN2 = (Node*)node_list_->Items[ (i+1)*angle_count + j ];
       aN3 = (Node*)node_list_->Items[ (i+1)*angle_count + (j+1) ];

       // Face(Node* nL, Node* nR, Node* nO, Edge* eD, Edge* eR, Edge* eL);
       aF = new Face(aN3, aN1, aN2);
    }

    //Join the extreme rectangles:
    //(i)*nCirc + nCirc, (i+1)*nCirc + nCirc, (i)*nCirc + 1
    //(i)*nCirc + 1, (i+1)*nCirc+nCirc, (i+1)*nCirc + 1

    aN1 = (Node*)node_list_->Items[ i*angle_count + (angle_count-1) ];
    aN2 = (Node*)node_list_->Items[ (i+1)*angle_count + (angle_count-1) ];
    aN3 = (Node*)node_list_->Items[ i*angle_count];

    // Face(Node* nL, Node* nR, Node* nO, Edge* eD, Edge* eR, Edge* eL);
    aF = new Face(aN3, aN1, aN2);

    aN1 = (Node*)node_list_->Items[ i*angle_count ];
    aN2 = (Node*)node_list_->Items[ (i+1)*angle_count + (angle_count-1) ];
    aN3 = (Node*)node_list_->Items[ (i+1)*angle_count];

    // Face(Node* nL, Node* nR, Node* nO, Edge* eD, Edge* eR, Edge* eL);
    aF = new Face(aN3, aN1, aN2);
  }


  //%5-Close aorta by meshing the ascendig and descending apertures:
  //5.1- Add two extra Nodes for the geometrical center of the first and last circle of 'c'.
  new Node(center_line[0][0], center_line[0][1], center_line[0][2]);
  new Node(center_line[center_count-1][0], center_line[center_count-1][1],
           center_line[center_count-1][2]);

  //First and last Node indexes:
  int pF_index, pL_index;
  pF_index = node_list_->Count-2;
  pL_index = node_list_->Count-1;

  //Traverses first circle:
  for (j = 0; j < angle_count-1; ++j) {
    //Mesh with concentric triangles
    aN3 = (Node*)node_list_->Items[j];
    aN2 = (Node*)node_list_->Items[pF_index];
    aN1 = (Node*)node_list_->Items[j+1];

    // Face(Node* nL, Node* nR, Node* nO, Edge* eD, Edge* eR, Edge* eL);
    aF = new Face(aN3, aN1, aN2);
  }

  //Closes the first circle:
  aN3 = (Node*)node_list_->Items[angle_count-1];
  aN2 = (Node*)node_list_->Items[pF_index];
  aN1 = (Node*)node_list_->Items[0];
  aF = new Face(aN3, aN1, aN2);

  //Traverses last circle:
  for(j = 0; j < angle_count-1; ++j) {
    //Mesh with concentric triangles
    aN1 = (Node*)node_list_->Items[ ((node_list_->Count-3) - (angle_count-1)) + j ];
    aN2 = (Node*)node_list_->Items[ pL_index ];
    aN3 = (Node*)node_list_->Items[ ((node_list_->Count-3) - (angle_count-1)) + (j+1) ];
    // Face(Node* nL, Node* nR, Node* nO, Edge* eD, Edge* eR, Edge* eL);
    aF = new Face(aN3, aN1, aN2);
  }

  //Closes the Last circle:
  aN1 = (Node*)node_list_->Items[ node_list_->Count-3 ];
  aN2 = (Node*)node_list_->Items[ pL_index ];
  aN3 = (Node*)node_list_->Items[ ((node_list_->Count-3)-(angle_count-1))];
  aF = new Face(aN3, aN1, aN2);

  delete[] t;

  //circles_list destruction:
  for (i = 0; i < center_count; ++i) {
    for (j = 0; j < angle_count; ++j) {
      delete [] circle_list[i][j];
    }
    delete [] circle_list[i];
  }
  delete [] circle_list;

  //Seteo de las condiciones iniciales:
  stretch_coeff_ = -0.1; //coef de estiramiento: O<---->O
  bend_coeff_    =  3.0; //Coef de curvatura: O--O--O--O--O
  ball_coeff_    =  0.2; //coef de inflado    O->
  edge_coeff_    =  0.0; //coeficiente de fuerza de bordes ->O<-
  damp_coeff_    =  1.5;
  mass_          =  3.0;
  dark_background_ = true;

  scale_x_ = scale_x;
  scale_y_ = scale_y;
  scale_z_ = scale_z;

  int faces_count;
  do {
    faces_count = face_list_->Count;
    for (int i=0; i < faces_count; ++i)
      ((Face*)face_list_->Items[i])->Quadsection() ;
  } while(ComputeMeanEdgeDistance() < limit_edge_length);

  min_length_ = 0.25*limit_edge_length;
  max_length_ =  2.0*limit_edge_length;
  euler_number_ =  node_list_->Count + face_list_->Count - edge_list_->Count;

  set_rest_distance(ComputeMeanEdgeDistance());

  ComputeFaceNormals();
}
//---------------------------------------------------------------------------

Mesh::~Mesh() {
  Face* pF;
  Edge* pE;
  Node* pN;

  if (face_list_) {
    for (int i = 0; i < face_list_->Count; ++i) {
      pF = (Face*)face_list_->Items[i];
      delete pF;
    }

    face_list_->Clear();
    face_list_ = NULL;
  }

  if (edge_list_) {
    for(int i = 0; i < edge_list_->Count; ++i) {
      pE = (Edge*)edge_list_->Items[i];
      delete pE;
    }

    edge_list_->Clear();
    edge_list_ = NULL;
  }

  if (node_list_) {
    for (int i = 0; i < node_list_->Count; ++i) {
      pN = (Node*)node_list_->Items[i];
      delete pN;
    }

    node_list_->Clear();
    node_list_ = NULL;
  }

}
//-----------------------------------------------------------------------------

void Mesh::set_rest_distance(double distance) {
  rest_distance_ = distance;
  ((Node*)node_list_->First())->set_rest_distance(distance);
}
//------------------------------------------------------------------------------

void Mesh::MoveMesh(double x_trans, double y_trans, double z_trans) {
  Node* pN;
  for(int i = 0; i < node_list_->Count; ++i) {
    pN = (Node*)node_list_->Items[i];
    pN->position_[0] += x_trans;
    pN->position_[1] += y_trans;
    pN->position_[2] += z_trans;
  }
}
//----------------------------------------------------------------------------


void Mesh::AdaptiveMelting() {
  //too_short_flag counts the amount of small sides, to decide the melting
  //operation to perform
  int too_short_flag;
  Face* pF;

  for(int i = 0; i < face_list_->Count; ++i) {
    //Reset 'too short' flag
    too_short_flag = 0;

    pF = (Face*)face_list_->Items[i];

    pF->down_edge_->ComputeLength() < min_length_ ?
        too_short_flag |= B_3: too_short_flag |= B_N;
    pF->right_edge_->ComputeLength() < min_length_ ?
        too_short_flag |= B_2: too_short_flag |= B_N;
    pF->left_edge_->ComputeLength() < min_length_ ?
        too_short_flag |= B_1: too_short_flag |= B_N;

    pF->is_visited_ = false;

    switch (too_short_flag) {
      case 0: {
        //No operation
        pF->is_visited_ = true;
        break;
      }

      case 1: {
        //left too short, others OK
        pF->is_visited_ = true;
        if (pF->left_edge_->EdgeMelting(1))
          i--;
        break;
      }

      case 2: {
        pF->is_visited_ = true;
        if (pF->right_edge_->EdgeMelting(1))
          i--;
        break;
      }

      case 4: {
        pF->is_visited_ = true;
        if (pF->down_edge_->EdgeMelting(1))
          i--;
        break;
      }

      case 3: {
        pF->is_visited_ = true;
        if (pF->TriangleMelting() == 1)
          i--;
      break;
      }

      case 5: {
        pF->is_visited_ = true;
        if (pF->TriangleMelting() == 1)
          i--;
      break;
      }

      case 6: {
        pF->is_visited_ = true;
        if (pF->TriangleMelting() == 1)
          i--;
      break;
      }

      case 7: {
        pF->is_visited_ = true;
        if (pF->TriangleMelting() == 1)
          i--;
        break;
      }
    }
  }
}
//-----------------------------------------------------------------------------
void Mesh::AdaptiveInversion()
{
  int EU;
  int ED;
  Face *pFU;
  Face *pFD;
  Edge *pE;

  if (edge_list_) {
    for (int i = 0; i < edge_list_->Count; ++i) {
      pE = (Edge*)edge_list_->Items[i];

      if(pE->ComputeLength() < max_length_)
        continue;

      EU = 1;
      ED = 1;

      pFU = pE->up_face_;
      pFD = pE->down_face_;

      if (pE == pFU->down_edge_) {
        if (pFU->left_edge_->ComputeLength() > max_length_ ||
            pFU->right_edge_->ComputeLength() > max_length_) {
          EU = 0;
        }
      }

      if (pE == pFU->left_edge_) {
        if (pFU->down_edge_->ComputeLength() > max_length_ ||
            pFU->right_edge_->ComputeLength() > max_length_) {
          EU = 0;
        }
      }

      if (pE == pFU->right_edge_) {
        if(pFU->left_edge_->ComputeLength() > max_length_ ||
           pFU->down_edge_->ComputeLength() > max_length_) {
          EU = 0;
        }
      }

      if (pE == pFD->down_edge_) {
         if(pFD->left_edge_->ComputeLength() > max_length_ ||
            pFD->right_edge_->ComputeLength() > max_length_) {
          ED = 0;
          }
      }

      if (pE == pFD->left_edge_) {
         if(pFD->down_edge_->ComputeLength() > max_length_ ||
            pFD->right_edge_->ComputeLength() > max_length_) {
           ED = 0;
         }
      }

      if (pE == pFD->right_edge_) {
        if (pFD->left_edge_->ComputeLength() > max_length_ ||
            pFD->down_edge_->ComputeLength() > max_length_) {
          ED = 0;
        }
      }

      if (ED * EU)
        pE->Inversion();
    }
  }
}
//------------------------------------------------------------------------------

bool Mesh::AdaptiveSubdivision() {

  //Return true if any subdivision took place, false otherwise:
  int face_count = face_list_->Count;
  int too_long_flag;
  bool any_sub = false;
  Face* pF;

  //3. Subdivision:
  for(int i = 0; i < face_count; ++i) {
    //Reset 'too long' flag
    too_long_flag = 0;

    pF = (Face*)face_list_->Items[i];
    pF->down_edge_->ComputeLength () > max_length_ ? too_long_flag |= B_3 :
                                                     too_long_flag |= B_N;
    pF->right_edge_->ComputeLength() > max_length_ ? too_long_flag |= B_2 :
                                                     too_long_flag |= B_N;
    pF->left_edge_->ComputeLength () > max_length_ ? too_long_flag |= B_1 :
                                                     too_long_flag |= B_N;

    switch (too_long_flag) {
      case 0: {
        break;
      }

      case 1: {
        pF->Bisection(pF->left_edge_);
        any_sub = true;
        break;
      }

      case 2: {
        pF->Bisection(pF->right_edge_);
        any_sub = true;
        break;
      }

      case 3: {
        if (pF->left_edge_->ComputeLength() > pF->right_edge_->ComputeLength())
          pF->Trisection(pF->left_edge_, pF->right_edge_);
        else
          pF->Trisection(pF->right_edge_, pF->left_edge_);
        any_sub = true;
        break;
      }

      case 4: {
        pF->Bisection(pF->down_edge_);
        any_sub = true;
        break;
      }

      case 5: {
        if (pF->down_edge_->ComputeLength() > pF->left_edge_->ComputeLength())
          pF->Trisection(pF->down_edge_, pF->left_edge_);
        else
          pF->Trisection(pF->left_edge_, pF->down_edge_);
        any_sub = true;
        break;
      }

      case 6: {
        if (pF->down_edge_->ComputeLength() > pF->right_edge_->ComputeLength())
          pF->Trisection(pF->down_edge_, pF->right_edge_);
        else
          pF->Trisection(pF->right_edge_, pF->down_edge_);
        any_sub = true;
        break;
      }

      case 7: {
        pF->Quadsection();
        any_sub = true;
        break;
      }
    }
  }

  return any_sub;
}
//------------------------------------------------------------------------------

double Mesh::ComputeMeanEdgeDistance() {
  double edgeprom = 0;
  for(int i = 0; i < edge_list_->Count; ++i) {
    edgeprom += ((Edge*)edge_list_->Items[i])->ComputeLength();
  }
  return edgeprom/double(edge_list_->Count);
}
//------------------------------------------------------------------------------

void Mesh::ComputeFaceNormals() {
  for(int i = 0; i < face_list_->Count; ++i)
    ((Face*)face_list_->Items[i])->ComputeNormal();
}
//------------------------------------------------------------------------------

double Mesh::ComputeTotalSignedVolume() {
   double acum = 0;
   for (int i = 0; i < face_list_->Count; ++i)
     acum += ((Face*)face_list_->Items[i])->ComputeSignedVolume();
   return acum;
}
//------------------------------------------------------------------------------

void Mesh::RefreshMesh() {
  Node* pN;
  Edge* pE;
  Face* pF;

  pN = (Node*)node_list_->First();
  pE = (Edge*)edge_list_->First();
  pF = (Face*)face_list_->First();

  pN->set_node_list(node_list_);
  pE->set_edge_list(edge_list_);
  pF->set_face_list(face_list_) ;

  pN->set_rest_distance(rest_distance_);
  pN->set_ball_coeff(ball_coeff_);
  pN->set_stretch_coeff(stretch_coeff_);
  pN->set_bend_coeff(bend_coeff_);
  pN->set_edge_coeff(edge_coeff_);
  pN->set_dark_background(dark_background_);
  pN->set_damp_coeff(damp_coeff_);
  pN->set_mass(mass_);

  pN->set_max_intensity_thresh(max_intensity_thresh_);
  pN->set_min_intensity_thresh(min_intensity_thresh_);

  pN->set_scale_x(scale_x_);
  pN->set_scale_y(scale_y_);
  pN->set_scale_z(scale_z_);

  //pN->normal[1] = bound_plane_normal_1_[1];
  //pN->normal[1] = bound_plane_normal_1_[1];
  //bound_plane_normal_1_[2] = pN->normal[2];
  //bound_plane_point_1_[0]  = pN->point[0];
  //bound_plane_point_1_[1]  = pN->point[1];
  //bound_plane_point_1_[2]  = pN->point[2];

  pN->set_bound_plane(bound_plane_normal_1_,true);  //true: plane 1
  pN->set_bound_plane(bound_plane_normal_2_,false); //false: plane 2
  pN->set_dot_1(bound_plane_normal_1_[0]*bound_plane_point_1_[0] +
           bound_plane_normal_1_[1]*bound_plane_point_1_[1] +
           bound_plane_normal_1_[2]*bound_plane_point_1_[2]);

  pN->set_dot_2(bound_plane_normal_2_[0]*bound_plane_point_2_[0] +
           bound_plane_normal_2_[1]*bound_plane_point_2_[1] +
           bound_plane_normal_2_[2]*bound_plane_point_2_[2]);

  pN->set_min_movement_thresh(min_movement_thresh_);
  pN->set_max_inactive_iterations(max_inactive_iterations_);
  pN->ResetTotalInactive();
}
//------------------------------------------------------------------------------

double Mesh::ComputeActivity() {
  Node* pN;
  pN = (Node*)node_list_->First();
  return 1.0 - double(pN->get_total_inactive()) / double(node_list_->Count);
}
//------------------------------------------------------------------------------

void Mesh::RefineToSubPixel() {
  max_length_ = 1.0;
  min_length_ = 1.0;
  while(AdaptiveSubdivision());
}
//------------------------------------------------------------------------------

void Mesh::SaveMeshToFile(char* file_name) {

  FILE* fh;
  fh = fopen(file_name, "wb");

  fwrite(&id_, sizeof(id_), 1, fh);
  fwrite(&is_active_, sizeof(is_active_), 1, fh);
  fwrite(&dark_background_, sizeof(dark_background_), 1, fh);

  fwrite(&rest_distance_, sizeof(rest_distance_), 1, fh);
  fwrite(&max_intensity_thresh_, sizeof(max_intensity_thresh_), 1, fh);
  fwrite(&min_intensity_thresh_, sizeof(min_intensity_thresh_), 1, fh);
  fwrite(&scale_x_, sizeof(scale_x_), 1, fh);
  fwrite(&scale_y_, sizeof(scale_x_), 1, fh);
  fwrite(&scale_z_, sizeof(scale_x_), 1, fh);
  fwrite(&stretch_coeff_, sizeof(stretch_coeff_), 1, fh);
  fwrite(&bend_coeff_, sizeof(bend_coeff_), 1, fh);
  fwrite(&ball_coeff_, sizeof(ball_coeff_), 1, fh);
  fwrite(&edge_coeff_, sizeof(edge_coeff_), 1, fh);

  fwrite(&mass_, sizeof(mass_), 1, fh);
  fwrite(&damp_coeff_, sizeof(damp_coeff_), 1, fh);

  fwrite(&min_length_, sizeof(min_length_), 1, fh);
  fwrite(&max_length_, sizeof(max_length_), 1, fh);

  fwrite(center_point_, sizeof(center_point_[0]), 3, fh);
  fwrite(bound_plane_normal_1_, sizeof(bound_plane_normal_1_[0]), 3, fh);
  fwrite(bound_plane_normal_2_, sizeof(bound_plane_normal_2_[0]), 3, fh);
  fwrite(bound_plane_point_1_, sizeof(bound_plane_point_1_[0]), 3, fh);
  fwrite(bound_plane_point_2_, sizeof(bound_plane_point_2_[0]), 3, fh);

  fwrite(&min_movement_thresh_, sizeof(min_movement_thresh_), 1, fh);
  fwrite(&max_inactive_iterations_, sizeof(max_inactive_iterations_), 1, fh);

  //Save total nodes quantity:
  fwrite(&(node_list_->Count), sizeof(int), 1, fh);

  //Save all Associated Nodes:
  //First Step: rename all to a linear index-type equivalent id, and apend
  //all nodes info:
  Node* pN;
  int new_node_id = -1;

  for (int i = 0; i < node_list_->Count; ++i) {
    pN = (Node*)node_list_->Items[i];
    pN->id_ = ++new_node_id;

    fwrite(pN->position_, sizeof(double), 3, fh);
    fwrite(pN->next_position_, sizeof(double), 3, fh);
    fwrite(pN->normal_, sizeof(double), 3, fh);
    fwrite(pN->acceleration_, sizeof(double), 3, fh);
    fwrite(pN->velocity_, sizeof(double), 3, fh);
    fwrite(pN->force_, sizeof(double), 3, fh);
    fwrite(pN->mass_center_, sizeof(double), 3, fh);
    fwrite(&(pN->id_), sizeof(int), 1, fh);
    fwrite(&(pN->is_interface_), sizeof(bool), 1, fh);
    fwrite(&(pN->is_moving_), sizeof(bool), 1, fh);
    fwrite(&(pN->is_visited_), sizeof(bool), 1, fh);
    fwrite(&(pN->is_active_), sizeof(bool), 1, fh);
    fwrite(&(pN->inactive_times_), sizeof(unsigned char), 1, fh);
  }
  pN->set_max_id(new_node_id);

  //Save total Edges quantity:
  fwrite(&(edge_list_->Count), sizeof(int), 1, fh);

  //Second Step: rename all Edges to a linear index-type equivalent id:
  Edge* pE;

  int new_edge_id = -1;

  for (int i = 0 ; i < edge_list_->Count; ++i) {
    pE = (Edge*)edge_list_->Items[i];
    pE->id_ = ++new_edge_id;

    fwrite(&(pE->id_), sizeof(int), 1, fh);
    fwrite(&(pE->left_node_->id_), sizeof(int), 1, fh);
    fwrite(&(pE->right_node_->id_), sizeof(int), 1, fh);
    fwrite(&(pE->is_visited_), sizeof(bool), 1, fh);
  }
  pE->set_max_id(new_edge_id);

  //Save total Faces quantity:
  fwrite(&(face_list_->Count), sizeof(int), 1, fh);

  //Third Step: rename all Faces to a linear index-type equivalent id:
  Face* pF;
  int new_face_id = -1;

  for (int i = 0; i < face_list_->Count; ++i) {
    pF = (Face*)face_list_->Items[i];
    pF->id_ = ++new_face_id;

    fwrite(&(pF->id_), sizeof(int), 1, fh);
    fwrite(&(pF->left_node_->id_), sizeof(int), 1, fh);
    fwrite(&(pF->right_node_->id_), sizeof(int), 1, fh);
    fwrite(&(pF->opp_node_->id_), sizeof(int), 1, fh);

    fwrite(&(pF->left_edge_->id_), sizeof(int), 1, fh);
    fwrite(&(pF->right_edge_->id_), sizeof(int), 1, fh);
    fwrite(&(pF->down_edge_->id_), sizeof(int), 1, fh);

    fwrite(&(pF->is_visited_), sizeof(bool), 1, fh);
    fwrite(&(pF->is_active_), sizeof(bool), 1, fh);
    fwrite(&(pF->is_newborn_), sizeof(bool), 1, fh);
  }
  pF->set_max_id(new_face_id);

  fclose(fh);
}
//------------------------------------------------------------------------------


void Mesh::LoadMeshFromFile(char* file_name) {

  //Mesh must previously exist!!
  if(this == NULL)
    return;

  FILE* fh;

  fh = fopen(file_name, "rb");

  fread(&id_, sizeof(id_), 1, fh);
  fread(&is_active_, sizeof(is_active_), 1, fh);
  fread(&dark_background_, sizeof(dark_background_), 1, fh);

  fread(&rest_distance_, sizeof(rest_distance_), 1, fh);
  fread(&max_intensity_thresh_, sizeof(max_intensity_thresh_), 1, fh);
  fread(&min_intensity_thresh_, sizeof(min_intensity_thresh_), 1, fh);
  fread(&scale_x_, sizeof(scale_x_), 1, fh);
  fread(&scale_y_, sizeof(scale_x_), 1, fh);
  fread(&scale_z_, sizeof(scale_x_), 1, fh);
  fread(&stretch_coeff_, sizeof(stretch_coeff_), 1, fh);
  fread(&bend_coeff_, sizeof(bend_coeff_), 1, fh);
  fread(&ball_coeff_, sizeof(ball_coeff_), 1, fh);
  fread(&edge_coeff_, sizeof(edge_coeff_), 1, fh);

  fread(&mass_, sizeof(mass_), 1, fh);
  fread(&damp_coeff_, sizeof(damp_coeff_), 1, fh);

  fread(&min_length_, sizeof(min_length_), 1, fh);
  fread(&max_length_, sizeof(max_length_), 1, fh);

  fread(center_point_, sizeof(center_point_[0]), 3, fh);
  fread(bound_plane_normal_1_, sizeof(bound_plane_normal_1_[0]), 3, fh);
  fread(bound_plane_normal_2_, sizeof(bound_plane_normal_2_[0]), 3, fh);
  fread(bound_plane_point_1_, sizeof(bound_plane_point_1_[0]), 3, fh);
  fread(bound_plane_point_2_, sizeof(bound_plane_point_2_[0]), 3, fh);

  fread(&min_movement_thresh_, sizeof(min_movement_thresh_), 1, fh);
  fread(&max_inactive_iterations_, sizeof(max_inactive_iterations_), 1, fh);

  //Load and create Nodes:
  Node* pN;
  pN->set_node_list(node_list_);

  int node_count;
  fread(&node_count, sizeof(int), 1, fh);

  double position_[3];
  double n_position_[3];
  double normal[3];
  double acc[3];
  double vel[3];
  double f[3];
  double c[3];
  int nid;
  bool interf;
  bool is_moving;
  bool visited;
  bool active;
  short int inactive_times;

  for (int i = 0; i < node_count; ++i) {
    fread(position_, sizeof(double), 3, fh);
    fread(n_position_, sizeof(double), 3, fh);
    fread(normal, sizeof(double), 3, fh);
    fread(acc, sizeof(double), 3, fh);
    fread(vel, sizeof(double), 3, fh);
    fread(f, sizeof(double), 3, fh);
    fread(c, sizeof(double), 3, fh);
    fread(&nid, sizeof(int), 1, fh);
    fread(&interf, sizeof(bool), 1, fh);
    fread(&is_moving, sizeof(bool), 1, fh);
    fread(&visited, sizeof(bool), 1, fh);
    fread(&active, sizeof(bool), 1, fh);
    fread(&inactive_times, sizeof(unsigned char), 1, fh);

    pN = new Node(position_[0],position_[1],position_[2]);
    pN->next_position_[0] = n_position_[0];
    pN->next_position_[1] = n_position_[1];
    pN->next_position_[2] = n_position_[2];

    pN->normal_[0] = normal[0];
    pN->normal_[1] = normal[1];
    pN->normal_[2] = normal[2];

    pN->acceleration_[0] = acc[0];
    pN->acceleration_[1] = acc[1];
    pN->acceleration_[2] = acc[2];

    pN->velocity_[0] = vel[0];
    pN->velocity_[1] = vel[1];
    pN->velocity_[2] = vel[2];

    pN->force_[0] = f[0];
    pN->force_[1] = f[1];
    pN->force_[2] = f[2];

    pN->mass_center_[0] = c[0];
    pN->mass_center_[1] = c[1];
    pN->mass_center_[2] = c[2];

    //El nuevo ID se pondrá automáticamente al crear el nodo con el new.
    //El viejo id solo se grabó por cuestiones de seguridad:
    pN->is_interface_ = interf;
    pN->is_moving_ = is_moving;
    pN->is_active_ = active;
    pN->inactive_times_ = inactive_times;
  }

  //Load and create Edges:
  Edge* pE;
  pE->set_edge_list(edge_list_);

  int edge_count;
  fread(&edge_count, sizeof(int), 1, fh);

  int eid;
  int lnid;
  int rnid;

  for (int i = 0; i < edge_count; ++i) {
    //edge ID:
    fread(&eid,  sizeof(int), 1, fh);
    fread(&lnid, sizeof(int), 1, fh);
    fread(&rnid, sizeof(int), 1, fh);
    fread(&visited, sizeof(bool), 1, fh);

    pE = new Edge((Node*)node_list_->Items[lnid],
                  (Node*)node_list_->Items[rnid]);

    pE->is_visited_ = visited;
  }

  //Load and create Faces:
  Face* pF;
  pF->set_face_list(face_list_);

  int face_count;
  fread(&face_count, sizeof(int), 1, fh);

  int fid;
  int onid;
  int leid;
  int reid;
  int deid;
  bool newborn;

  for (int i = 0; i < face_count; ++i) {
    fread(&fid,  sizeof(int), 1, fh);
    fread(&lnid, sizeof(int), 1, fh);
    fread(&rnid, sizeof(int), 1, fh);
    fread(&onid, sizeof(int), 1, fh);
    fread(&leid, sizeof(int), 1, fh);
    fread(&reid, sizeof(int), 1, fh);
    fread(&deid, sizeof(int), 1, fh);

    fread(&visited, sizeof(bool), 1, fh);
    fread(&active,  sizeof(bool), 1, fh);
    fread(&newborn, sizeof(bool), 1, fh);

    pF = new Face((Node*)node_list_->Items[lnid],
                  (Node*)node_list_->Items[rnid],
                  (Node*)node_list_->Items[onid],
                  (Edge*)edge_list_->Items[deid],
                  (Edge*)edge_list_->Items[reid],
                  (Edge*)edge_list_->Items[leid]);
    pF->is_visited_ = visited;
    pF->is_active_  = active;
    pF->is_newborn_ = newborn;
  }

  fclose(fh);
}
//------------------------------------------------------------------------------





