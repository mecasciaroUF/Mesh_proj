
//----------------------Algoritmo de Segmentación----------------------------------------------
double BayesianPosteriorProb(double valor, double mean, double std)
{
    //Devuelve la probabilidad posterior bayesiana:  Pr(C(x) = clase(mean,std) / I(x)=valor)
    //x: punto de la img, C(x): clase a la que pertenece x.
    //clase: pulmon, sangre, vaso, etc..., caracterizada por una media (mean) y desvio (std)
    // I(x):intensidad de x en HU.
    //Teoría: Pr(C(x)=c(mean,std)/I(x)=valor) = Pr(I(x)=v/C(x)=c)*Pr(C(x)=c)/(ProbabilidadTotal)
// Hipótesis:Pr(C(x)=c) es igual para toda clase c.
//           Pr(I(x)=v/C(x)=c): se supone Gaussiana => Pr = 1/raiz(2pi)*e(-(v-mean)^2/(2*std^2))
//           ProbTotal = sumatoria(para toda clase_i)Pr(I(x)=v/C(x)=c_i)*Pr(C(x)=c_i);

    double Pr;

    if(std!=-1)//Si std = -1 es porque no hay ningún punto de esa clase ...
      Pr = 1/sqrt(2*M_PI)*exp(-(valor-mean)*(valor-mean)/(2*std*std));
    else Pr=-1;//Entonces Pr = -1 (negativa) me asegura que no se le asigne esa clase a ningún px.
    
    return Pr;
}

//-------------------------------------------------------------------------------------------------
float kmeans(Volumen* Vol, int k , int maxPX, int minPX, float* mediaRes, float* desvRes)
{
//Vol: Sub Volumen con los valores de intensidad en HU. Al final se convierte en la
//     máscara de clases, indicando a que clase pertenece cada pixel.
//k: cantidad de clusters
//minPX,maxPX: valores máximos y mínimos de intensidad en la matriz de donde se extrae Vol.
//             Tiene por finalidad servir como condición inicial para buscar el máximo y mínimo
//             dentro de Vol.

int* Histograma;
int* Clasegrama;

int minVOL=maxPX;      //Mal: poner el primer elemento.
int maxVOL=minPX;

//Calculo de maximo y minimo para limitar el tamaño de Histograma:
// NOTA: podría reducirse el tiempo del algoritmo si solo se hiciera un histograma
// de tamaño maxPX-minPX.
        for(int k=0; k<Vol->getProf(); k++)
                for(int j=0; j<Vol->getAncho(); j++)
                        for(int i=0; i<Vol->getAlto(); i++)
                        {
                        if(Vol->GetV(i,j,k)<minVOL)
                        minVOL = Vol->GetV(i,j,k);

                        if(Vol->GetV(i,j,k)>maxVOL)
                        maxVOL = Vol->GetV(i,j,k);
                        }

//Calculo del histograma:
float Rango = maxVOL-minVOL+1;  //+1: evita que histograma empiece en 0, y no acumule valor

Histograma = new int[Rango];
Clasegrama = new int[Rango];

        for(int i=1; i<Rango; i++)
        Histograma[i]=0;

        for(int k=0; k<Vol->getProf(); k++)
                for(int j=0; j<Vol->getAncho(); j++)
                        for(int i=0; i<Vol->getAlto(); i++)
                                Histograma[Vol->GetV(i,j,k)-minVOL+1]++;

//Array de medias del mismo tamaño que la cantidad de clases a segmentar.
float* mu = new float[k];
float* oldmu = new float[k];
float* desv = new float[k]; //vector de desvíos

//Valor inicial: subdivisión del rango de Intensidades en partes iguales:
        for(int i=0; i<k; i++)
                mu[i]=(float(i)+1.0)/(float(k)+1.0)*Rango;

//Array de distancias entre Intensidades y Medias
float* d = new float[k];

float diferencias; //valor que suma todas las diferencias entre mu y oldmu para condición de stop.
                   //Si mu = oldmu => mu-oldmu = 0 => diferencias = suma(mu-oldmu)=0

double* acumValor = new double[k];//almacena el producto h[i]*i=valor acumulado de intensidades de pixels con valor i, para todo i de la clase k
double* acumCant = new double[k]; //almacena la cantidad h[i]=cantidad total de pixels con int i , para todo i de la clase k
double* acum2 = new double[k]; //Almacena como acumValor^2, para el cálculo de desvíos de cada clase.

for(int i=0; i<k; i++)
{
acumValor[i]=0;
acumCant[i] =0;
acum2[i]=0;
}

while(true)
{
        //oldmu=mu
        for(int j=0; j<k; j++)
                oldmu[j]=mu[j];

        //Clasificación en Clasegrama:
        for(int i=1; i<Rango; i++)
        {
           for(int j=0;j<k;j++)
           d[j]=abs(i-mu[j]);

           float dmin=Rango; //maxima distancia posible entre dos valores de Intensidad
           int dminindex;

           for(int j=0;j<k;j++)
                if(d[j]<dmin)
                {
                dmin=d[j];
                dminindex = j;
                }

          Clasegrama[i]=dminindex; //La Posicion en Clasegrama es la Intensidad, y el valor la clase a la que pertenece
        }

        //Calculo de nuevas medias:
        for(int i=1; i<Rango; i++)
        {

                      int clasei = Clasegrama[i];
                         acumValor[clasei] = acumValor[clasei]+double(Histograma[i])*double(i); // Sum(h[i]*i)
                         acumCant[clasei] = acumCant[clasei]+double(Histograma[i]); //Sum(h[i])
                         acum2[clasei]=acum2[clasei]+ double(Histograma[i])*double(i*i);//Sum(h[i]*i^2)
        }

        for(int i=0;i<k;i++)
                if(acumCant[i]) mu[i]=acumValor[i]/acumCant[i];

        //Condición de finalizacion (mu=oldmu):
        diferencias = 0;
        for(int i=0; i<k; i++)
                diferencias = diferencias + (mu[i]-oldmu[i]);

        if(!diferencias)
                break;

        for(int j=0;j<k;j++)
         {
                acumValor[j] = 0;
                acumCant[j] = 0;
                acum2[j]=0;
         }

}//end while(true)

//Calculo de desvíos:
for(int i=0; i<k; i++)
        {
        double acum = acumValor[i];
        double cant = acumCant[i];
        double acum_2 = acum2[i];

         if(cant>1)
         desv[i]=sqrt((acum_2-(acum/cant)*acum)/(cant-1));
         else desv[i]=-1;

        }

//Calculo de la probabilidad posterior Bayesiana:
//Re-clasificación en Clasegrama:
for(int i=1; i<Rango; i++)
{
        double valor = i;

           for(int j=0;j<k;j++)
           {
           double std=double(desv[j]); if(std==0)std=1; //se asegura que no haya división por 0
           double mean=double(mu[j]);
           d[j]= (-1.0)*BayesianPosteriorProb(valor, mean, std); //El -1 me asegura que encuentro
                                                                 //el "máximo" de la Prob, repitiendo la
                                                                 //busqueda del mínimo por BubbleSort
           }

           float dmin=Rango; //maxima distancia posible entre dos valores de Intensidad
           int dminindex;

           for(int j=0;j<k;j++)
                if(d[j]<dmin)
                {
                dmin=d[j];
                dminindex = j;
                }

        Clasegrama[i]=dminindex; //La Posicion en Clasegrama es la Intensidad, y el valor la clase a la que pertenece
}


//Creación de la máscara de clases:
for(int k=0; k<Vol->getProf(); k++)
 for(int i=0; i<Vol->getAlto(); i++)
        for(int j=0; j<Vol->getAncho(); j++)
                        if(Vol->GetV(i,j,k)-minVOL+1>0 && Vol->GetV(i,j,k)-minVOL+1<Rango)
                        Vol->SetV(i,j,k, Clasegrama[Vol->GetV(i,j,k)-minVOL+1]);
                        else
                        Vol->SetV(i,j,k, minVOL);


//Pasa los vectores al resultado:
for(int i=0; i<k; i++)
{
        mediaRes[i]=mu[i];
        desvRes[i]=desv[i];
}

//Eliminación de vectores dinámicos auxiliares:
delete Histograma;
delete Clasegrama;
delete mu;
delete oldmu;
delete desv;
delete d;
delete acumValor;
delete acumCant;
delete acum2;

//Return:
return Rango;

}
//------------------------------------------------------------------------------
float kmeansVector(TList* Vector, int k , int Rango, int minVOL, float* mediaRes,
                   float* desvRes, int* Clasegrama)
{
//Vector: Vector con valores de los pixels (grupos o subgrupos) en HU.
//k: cantidad de clusters
//mediaRes - desvRes:  media y desvio dentro de cada clase.
//Clasegrama: funciona como máscara de clases: ci: clase para todo ci!=-1 (no es una clase)

int* Histograma;

short int* valor = new short int[1];

Histograma = new int[Rango];

        for(int i=1; i<Rango; i++)
        {
        Histograma[i]=-1; //-1: indica que no es una clase valida.
        Clasegrama[i]=-1;
        }

       for(int i=0; i<Vector->Count;i++)
       {
       valor = (short int*)Vector->Items[i];
       Histograma[int(valor[0])-minVOL+1]++;
       }

//Array de medias del mismo tamaño que la cantidad de clases a segmentar.
float* mu = new float[k];
float* oldmu = new float[k];
float* desv = new float[k]; //vector de desvíos

//Valor inicial: subdivisión del rango de Intensidades en partes iguales:
        for(int i=0; i<k; i++)
                mu[i]=(float(i)+1.0)/(float(k)+1.0)*Rango;

//Array de distancias entre Intensidades y Medias
float* d = new float[k];

float diferencias; //valor que suma todas las diferencias entre mu y oldmu para condición de stop.
                   //Si mu = oldmu => mu-oldmu = 0 => diferencias = suma(mu-oldmu)=0

double* acumValor = new double[k];//almacena el producto h[i]*i=valor acumulado de intensidades de pixels con valor i, para todo i de la clase k
double* acumCant = new double[k]; //almacena la cantidad h[i]=cantidad total de pixels con int i , para todo i de la clase k
double* acum2 = new double[k]; //Almacena como acumValor^2, para el cálculo de desvíos de cada clase.

for(int i=0; i<k; i++)
{
acumValor[i]=0;
acumCant[i] =0;
acum2[i]=0;
}

while(true)
{
        //oldmu=mu
        for(int j=0; j<k; j++)
                oldmu[j]=mu[j];

        //Clasificación en Clasegrama:
        for(int i=1; i<Rango; i++)
        {
          if(Histograma[i]!=-1)
          {
            for(int j=0;j<k;j++)
            d[j]=abs(i-mu[j]); //d[j]:

            float dmin=Rango; //maxima distancia posible entre dos valores de Intensidad
            int dminindex;

            for(int j=0;j<k;j++)
                if(d[j]<dmin)
                {
                dmin=d[j];
                dminindex = j;
                }

           Clasegrama[i]=dminindex; //La Posicion en Clasegrama es la Intensidad, y el valor la clase a la que pertenece
          }
        }

        //Calculo de nuevas medias:
        for(int i=1; i<Rango; i++)
        {
          if(Histograma[i]!=-1)
          {
          int clasei = Clasegrama[i];
          acumValor[clasei] = acumValor[clasei]+double(Histograma[i])*double(i); // Sum(h[i]*i)
          acumCant[clasei] = acumCant[clasei]+double(Histograma[i]); //Sum(h[i])
          acum2[clasei]=acum2[clasei]+ double(Histograma[i])*double(i*i);//Sum(h[i]*i^2)
          }
        }

        for(int i=0;i<k;i++)
                if(acumCant[i]) mu[i]=acumValor[i]/acumCant[i];

        //Condición de finalizacion (mu=oldmu):
        diferencias = 0;
        for(int i=0; i<k; i++)
                diferencias = diferencias + (mu[i]-oldmu[i]);

        if(!diferencias)
                break;

        for(int j=0;j<k;j++)
         {
                acumValor[j] = 0;
                acumCant[j] = 0;
                acum2[j]=0;
         }

}//end while(true)

//Calculo de desvíos:
for(int i=0; i<k; i++)
        {
        double acum = acumValor[i];
        double cant = acumCant[i];
        double acum_2 = acum2[i];

         if(cant)
         desv[i]=sqrt((acum_2-(acum/cant)*acum)/(cant-1));
         else desv[i]=-1;

        }

//Calculo de la probabilidad posterior Bayesiana:
//Re-clasificación en Clasegrama:
for(int i=1; i<Rango; i++)
{
      if(Histograma[i]!=-1)
      {
        double valor = i;

           for(int j=0;j<k;j++)
           {
           double std=double(desv[j]); if(std==0)std=1; //se asegura que no haya división por 0
           double mean=double(mu[j]);
           d[j]= (-1.0)*BayesianPosteriorProb(valor, mean, std); //El -1 me asegura que encuentro
                                                                 //el "máximo" de la Prob, repitiendo la
                                                                 //busqueda del mínimo por BubbleSort
           }

           float dmin=Rango; //maxima distancia posible entre dos valores de Intensidad
           int dminindex;

           for(int j=0;j<k;j++)
                if(d[j]<dmin)
                {
                dmin=d[j];
                dminindex = j;
                }

        Clasegrama[i]=dminindex; //La Posicion en Clasegrama es la Intensidad, y el valor la clase a la que pertenece

     }
}

//Pasa los vectores al resultado:
for(int i=0; i<k; i++)
{
        mediaRes[i]=mu[i];
        desvRes[i]=desv[i];
}

//Eliminación de vectores dinámicos auxiliares:
delete Histograma; delete mu; delete oldmu; delete desv; delete d;
delete acumValor; delete acumCant; delete acum2;

//Return:
return Rango;

}
//------------------------------------------------------------------------------
float SupervisedKmeans(Volumen* Vol, int k , int max, int min, float* medias,
                       int* Histograma, int* Clasegrama)
{
//Igual que k-means, pero con las medias iniciales de clase ingresadas en el array "medias".
 //Vol: Sub Volumen con los valores de intensidad en HU. Al final se convierte en la
//     máscara de clases, indicando a que clase pertenece cada pixel.
//k: cantidad de clusters
//medias: medias de clase iniciales, externas al algoritmo.
//maxPX: máximo dentro de la Caja
//minPX: mínimo dentro de la Caja

// int* Histograma;
// int* Clasegrama;

int minVOL=min;
int maxVOL=max;

//Calculo de maximo y minimo para limitar el tamaño de Histograma:
// NOTA: podría reducirse el tiempo del algoritmo si solo se hiciera un histograma
// de tamaño maxPX-minPX.
                                 /*
        for(int k=0; k<Vol->getProf(); k++)
                for(int j=0; j<Vol->getAlto(); j++)
                        for(int i=0; i<Vol->getAncho(); i++)
                        {
                        if(Vol->GetV(i,j,k)<minVOL)
                        minVOL = Vol->GetV(i,j,k);

                        if(Vol->GetV(i,j,k)>maxVOL)
                        maxVOL = Vol->GetV(i,j,k);
                        }      */

//Calculo del histograma:
float Rango = maxVOL-minVOL+1;  //+1: evita que histograma empiece en 0, y no acumule valor

// Histograma = new int[Rango];
// Clasegrama = new int[Rango];

        for(int i=1; i<Rango; i++)
        Histograma[i]=0;

        for(int k=0; k<Vol->getProf(); k++)
                for(int j=0; j<Vol->getAlto(); j++)
                        for(int i=0; i<Vol->getAncho(); i++)
                                Histograma[Vol->GetV(i,j,k)-minVOL+1]++;

//Array de medias del mismo tamaño que la cantidad de clases a segmentar.
float* mu = new float[k];
float* oldmu = new float[k];
float* desv = new float[k]; //vector de desvíos

//Valor inicial: subdivisión del rango de Intensidades en partes iguales:
        for(int i=0; i<k; i++)
                mu[i]=medias[i]-minVOL+1;

                //Valor inicial: subdivisión del rango de Intensidades en partes iguales:
      //  for(int i=0; i<k; i++)
      //          mu[i]=(float(i)+1.0)/(float(k)+1.0)*Rango;

//Array de distancias entre Intensidades y Medias
float* d = new float[k];

float diferencias; //valor que suma todas las diferencias entre mu y oldmu para condición de stop.
                   //Si mu = oldmu => mu-oldmu = 0 => diferencias = suma(mu-oldmu)=0

double* acumValor = new double[k];//almacena el producto h[i]*i=valor acumulado de intensidades de pixels con valor i, para todo i de la clase k
double* acumCant = new double[k]; //almacena la cantidad h[i]=cantidad total de pixels con int i , para todo i de la clase k
double* acum2 = new double[k]; //Almacena como acumValor^2, para el cálculo de desvíos de cada clase.

for(int i=0; i<k; i++)
{
acumValor[i]=0;
acumCant[i] =0;
acum2[i]=0;
}

while(true)
{
        //oldmu=mu
        for(int j=0; j<k; j++)
                oldmu[j]=mu[j];

        //Clasificación en Clasegrama:
        for(int i=1; i<Rango; i++)
        {
           for(int j=0;j<k;j++)
           d[j]=abs(i-mu[j]);

           float dmin=Rango; //maxima distancia posible entre dos valores de Intensidad
           int dminindex;

           for(int j=0;j<k;j++)
                if(d[j]<dmin)
                {
                dmin=d[j];
                dminindex = j;
                }

          Clasegrama[i]=dminindex; //La Posicion en Clasegrama es la Intensidad, y el valor la clase a la que pertenece
        }

        //Calculo de nuevas medias:
        for(int i=1; i<Rango; i++)
        {

                      int clasei = Clasegrama[i];
                         acumValor[clasei] = acumValor[clasei]+double(Histograma[i])*double(i); // Sum(h[i]*i)
                         acumCant[clasei] = acumCant[clasei]+double(Histograma[i]); //Sum(h[i])
                         acum2[clasei]=acum2[clasei]+ double(Histograma[i])*double(i*i);//Sum(h[i]*i^2)
        }

        for(int i=0;i<k;i++)
                if(acumCant[i]) mu[i]=acumValor[i]/acumCant[i];

        //Condición de finalizacion (mu=oldmu):
        diferencias = 0;
        for(int i=0; i<k; i++)
                diferencias = diferencias + (mu[i]-oldmu[i]);

        if(!diferencias)
                break;

        for(int j=0;j<k;j++)
         {
                acumValor[j] = 0;
                acumCant[j] = 0;
                acum2[j]=0;
         }

}//end while(true)

//Calculo de desvíos:
for(int i=0; i<k; i++)
        {
        double acum = acumValor[i];
        double cant = acumCant[i];
        double acum_2 = acum2[i];

         if(cant>1)
         desv[i]=sqrt((acum_2-(acum/cant)*acum)/(cant-1));
         else desv[i]=-1;

        }

//Calculo de la probabilidad posterior Bayesiana:
//Re-clasificación en Clasegrama:
for(int i=1; i<Rango; i++)
{
        double valor = i;

           for(int j=0;j<k;j++)
           {
           double std=double(desv[j]); if(std==0)std=1; //se asegura que no haya división por 0
           double mean=double(mu[j]);
           d[j]= (-1.0)*BayesianPosteriorProb(valor, mean, std); //El -1 me asegura que encuentro
                                                                 //el "máximo" de la Prob, repitiendo la
                                                                 //busqueda del mínimo por BubbleSort
           }

           float dmin=Rango; //maxima distancia posible entre dos valores de Intensidad
           int dminindex;

           for(int j=0;j<k;j++)
                if(d[j]<dmin)
                {
                dmin=d[j];
                dminindex = j;
                }

        Clasegrama[i]=dminindex; //La Posicion en Clasegrama es la Intensidad, y el valor la clase a la que pertenece
}


//Creación de la máscara de clases:
for(int k=0; k<Vol->getProf(); k++)
        for(int j=0; j<Vol->getAlto(); j++)
                for(int i=0; i<Vol->getAncho(); i++)
                        Vol->SetV(i,j,k, Clasegrama[Vol->GetV(i,j,k)-minVOL+1]);

//Eliminación de vectores dinámicos auxiliares:
//delete Histograma; delete Clasegrama;
delete mu; delete oldmu; delete desv; delete d;
delete acumValor; delete acumCant; delete acum2;

//Return:
return Rango;

}
//------------------------------------------------------------------------------


