#if !defined(__VOLUMEN_H)
#define __VOLUMEN_H

class Volumen
        {
	private:
        int alto,ancho,prof;//alto, ancho, profundidad

	public:
        Volumen();
        Volumen(int ancho, int alto, int profundidad);
       ~Volumen();

       short int*** V; //puntero a triple array dinamico donde se almacenan los datos.
       void CrearV(); //Llena la matriz 3D: (V)

       void SetV(int x,int y,int z, short int valor); //Asigna un valor a V en las coord x,y,z
       short int GetV(int x,int y, int z);

       void LlenarTodo(short int valor); //Llena toda la matriz con el mismo valor

       int getAlto();
       int getAncho();
       int getProf();
        };
#endif
 