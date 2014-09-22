//vcl: permite la utilización de la clase TList
#include <vcl.h>
#include <stdio.h>

//For bit_wise operations:
#define B_N	0x00 //nothing
#define B_1	0x01 //one bit up
#define B_2	0x02 //two bits up
#define B_3	0x04 //three bits up

//forward declaration of Node and Edge classes:
//Como también se requiere la utilización de atributos de estas clases, el
//.h de cada una está incluído en el .cpp de Face.

class Node;
class Edge;
class Face;

class Mesh {
 public:
  //Default constructor:
  Mesh();
  //List by list constructor:
  Mesh(TList* face_list, TList* edge_list, TList* node_list,
       double limit_edge_length, double scale_x, double scale_y, double scale_z);
  //Spherical constructor:
  Mesh(double limit_edge_length, double sphere_radius, double scale_x = 1.0f,
       double scale_y = 1.0f, double scale_z = 1.0f);
  //Cylindrical constructor:
  Mesh(double** center_line, int center_count, double** normal_vectors,
           int normal_count, int angles, double radius, double limit_edge_length,
           double scale_x, double scale_y, double scale_z);
       
  ~Mesh();

  void RefreshMesh();
  void AdaptiveMelting();
  void AdaptiveInversion();
  bool AdaptiveSubdivision();
  void MoveMesh(double x_trans, double y_trans, double z_trans);

  void   ComputeFaceNormals();      //Calcula las normales de las caras
  double ComputeTotalSignedVolume();//Calcula el volumen signado total de la malla

  double ComputeMeanEdgeDistance();//Calculo de distancia promedio entre nodos.
  double ComputeTotalVolume();     //Calcula el volumen total de la malla
  double ComputeActivity();        //Computes ActiveNodes/totalNodes

  void RefineToSubPixel(); //Refines mesh to inmmediate subpixel resolution
  void SaveMeshToFile(char* file_name);
  void LoadMeshFromFile(char* file_name);

  //Accesors:
  __inline int get_id() { return id_; };
  __inline int get_euler_number() { euler_number_ =
    node_list_->Count + face_list_->Count - edge_list_->Count;
    return euler_number_;
    }
  __inline TList* get_face_list()  {  if (face_list_) return face_list_; }
  __inline TList* get_edge_list()  {  if (edge_list_) return edge_list_; }
  __inline TList* get_node_list()  {  if (node_list_) return node_list_; }

  __inline double get_min_length() { return min_length_; }
  __inline double get_max_length() { return max_length_; }
  __inline bool   get_is_active()  { return is_active_;}
  __inline double get_rest_distance() { return rest_distance_;};  //at-rest edge distance

  __inline double get_max_intensity_thresh() {return max_intensity_thresh_;}  //max color to consider inside growing region
  __inline double get_min_intensity_thresh() {return min_intensity_thresh_;}  //min color to consider inside growing region
  __inline double get_scale_x() {  return scale_x_; }
  __inline double get_scale_y() {  return scale_y_; }
  __inline double get_scale_z() {  return scale_z_; }
  __inline double get_stretch_coeff() {  return stretch_coeff_; }  //coeficiente que pondera el stretching force
  __inline double get_bend_coeff() {  return bend_coeff_; }  //coeficiente que pondera el bending force
  __inline double get_ball_coeff() {  return ball_coeff_; }   //coeficiente que pondera el radial force
  __inline double get_edge_coeff() {  return edge_coeff_; }  //coeficiente que pondera el edge force
  __inline double get_damp_coeff() {  return damp_coeff_; }  //node damping coefficient
  __inline double get_mass() {  return mass_; }              //node mass
  __inline bool   get_dark_background() {  return dark_background_; }
  __inline const double* get_center_point() const { return center_point_; }
  __inline short int get_max_inactive_iterations() {
      return max_inactive_iterations_;
  }
  __inline double* get_bound_plane_normal(bool plane_1_2) {
    if (plane_1_2)
      return bound_plane_normal_1_;
    else
      return bound_plane_normal_2_;
  }
  __inline double* get_bound_plane_point(bool plane_1_2) {
    if (plane_1_2)
      return bound_plane_point_1_;
    else
      return bound_plane_point_2_;
  }

  //Mutators:
  __inline void set_min_length(double length) { min_length_ = length; }
  __inline void set_max_length(double length) { max_length_ = length; }
  void set_rest_distance(double distance);
  __inline void set_scale_x(double scale_x) { scale_x_ = scale_x; }
  __inline void set_scale_y(double scale_y) { scale_y_ = scale_y; }
  __inline void set_scale_z(double scale_z) { scale_z_ = scale_z; }
  __inline void set_bound_plane(double *normal, double *point, bool plane_1_2) {
      if (plane_1_2) {
        bound_plane_normal_1_[0] = normal[0];
        bound_plane_normal_1_[1] = normal[1];
        bound_plane_normal_1_[2] = normal[2];
        bound_plane_point_1_[0]  = point[0];
        bound_plane_point_1_[1]  = point[1];
        bound_plane_point_1_[2]  = point[2];
      }
      else {
        bound_plane_normal_2_[0] = normal[0];
        bound_plane_normal_2_[1] = normal[1];
        bound_plane_normal_2_[2] = normal[2];
        bound_plane_point_2_[0]  = point[0];
        bound_plane_point_2_[1]  = point[1];
        bound_plane_point_2_[2]  = point[2];
      }
    }
  __inline void set_center_point(double *point) {
      center_point_[0] = point[0];
      center_point_[1] = point[1];
      center_point_[2] = point[2];
    }
  __inline void set_max_intensity_thresh(double intensity_thresh) {
      max_intensity_thresh_ = intensity_thresh;
    }
  __inline void set_min_intensity_thresh(double intensity_thresh) {
      min_intensity_thresh_ = intensity_thresh;
    }
  __inline void set_min_movement_thresh(double movement_thresh) {
      min_movement_thresh_ = movement_thresh;
    }
  __inline void set_max_inactive_iterations(short int iterations) {
      max_inactive_iterations_ = iterations;
    }
  __inline void set_is_active(bool active) {  is_active_ = active; }

  __inline void set_stretch_coeff(double coeff) {  stretch_coeff_ = coeff; }  //coeficiente que pondera el stretching force
  __inline void set_bend_coeff(double coeff) { bend_coeff_ = coeff; }  //coeficiente que pondera el bending force
  __inline void set_ball_coeff(double coeff) { ball_coeff_ = coeff; }   //coeficiente que pondera el radial force
  __inline void set_edge_coeff(double coeff) { edge_coeff_ = coeff; }  //coeficiente que pondera el edge force
  __inline void set_damp_coeff(double coeff) { damp_coeff_ = coeff; }  //node damping coefficient
  __inline void set_mass(double mass) { mass_ = mass;}    //node mass
  __inline void set_dark_background(bool dark) { dark_background_ = dark;}

  float color[3];

 private:
  int id_;            //mesh unique identifier
  int euler_number_;  //mesh Euler number: N+F-E
  double min_length_; //min allowed edge length
  double max_length_; //max allowed edge length
  bool is_active_;    //Defines if the mesh is active or inactive

  //Node-realated attributes:
  double rest_distance_;  //at-rest edge distance
  double max_intensity_thresh_;  //max color to consider inside growing region
  double min_intensity_thresh_;  //min color to consider inside growing region
  double scale_x_;  //x-coord scale (e.g. 0.5 mm/pixel)
  double scale_y_;  //y-coord scale (e.g. 0.5 mm/pixel)
  double scale_z_;  //z-coord scale (e.g. 0.5 mm/pixel)
  double stretch_coeff_;  //coeficiente que pondera el stretching force
  double bend_coeff_;  //coeficiente que pondera el bending force
  double ball_coeff_;  //coeficiente que pondera el radial force
  double edge_coeff_;  //coeficiente que pondera el edge force
  double damp_coeff_;  //node damping coefficient
  double mass_;    //node mass
  double total_volume_;  //total mesh volume

  double min_movement_thresh_;  //minimum node displacent to be considered "active"
  short int max_inactive_iterations_;//maximum iterations to wait for node to re-activate
  bool  dark_background_;  //booleano que indica si el fondo es oscuro (true) o claro (false)

  double center_point_[3];          //Central point
  double bound_plane_normal_1_[3];  //bounding plane normal 1
  double bound_plane_point_1_[3];   //bounding plane point 1
  double bound_plane_normal_2_[3];  //bounding plane normal 2
  double bound_plane_point_2_[3];   //bounding plane point 2

  TList* face_list_; //The list of face pointers to all related faces
  TList* edge_list_; //The list of face pointers to all related edges
  TList* node_list_; //The list of face pointers to all related nodes
};
