//vcl: permite la utilización de la clase TList:
#include <vcl.h>
//math.h: se utiliza para el cálculo de la raíz cuadrada:
//TODO: incluir solo la raiz cuadrada
#include <math.h>

//forward declaration de las clases Node y Face:
//Como también se requiere la utilización de atributos de estas clases, el
//.h de cada una está incluído en el .cpp de Edge.
class Node;
class Face;
class Mesh;

class Edge {
 private:
  //Internal parameters:
  friend class Mesh;
  friend class Face;
  friend class Node;

  static TList* edge_list_; //The TList of pointers to all existing edges
  int id_; //id of the edge
  static int max_id_;

   //State Variables:
  bool is_visited_;
  bool need_sewing_;

  //Mesh linking:
  Node* left_node_;  //Pointer to the Nodes connected by this edge
  Node* right_node_; //Pointer to the Nodes connected by this edge
  Node* sew_node_;   //Pointer to the Node sewed to this edge after being divided
  Face* up_face_;    //Pointers to Faces sharing this edge
  Face* down_face_;

  //Adaptive routines:
  bool EdgeMelting(bool in_depth);  //true: allows recursion, false: 1-depth
  bool Inversion();

  //Linking:
  void RemoveFace(char pos); //Face pos: up (u), down (d)
  bool RemoveFace(Face* face);
  void AddFace(Face* face, char pos); //safe adding: face replaces in position pos
  bool AddFace(Face* face); //unsafe adding: face is added in first free position
  void RemoveNode(Node* node, char pos);  //Node pos: left (l), right (r), opposite (o)
  void AddNode(Node* node, char pos);
  void ReplaceNode(Node* prev_node, Node* new_node);

 public:
  //Constructor/Destructor:
  Edge(Node* pN1, Node* pN2); //Parameter Constructor
  ~Edge();                    //Destructor

  //Métodos:
  //Accesors/Mutators:
  __inline static TList* get_edge_list() { return edge_list_;};
  __inline int get_id() { return id_; };
  __inline static int get_max_id() { return max_id_; };
  Face* get_neighbour_face(Face* face); //get the neighbour face along this edge
  Node* get_opposite_node(Face* face); //get the opp_node of this edge inside face

  __inline void set_edge_list(TList* edge_list) {  edge_list_ = edge_list; }
  __inline void set_max_id(int id) { max_id_ = id; }

  double ComputeLength(); //Computes and returns the length of the edge

  //Operator == override, compares pointers to this->edge with another edge pointer
  __inline bool operator==(const Edge &other_edge) const
    {  this  == &other_edge ? true: false;
    };
};

