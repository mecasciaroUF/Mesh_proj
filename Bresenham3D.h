/*******************************************************************************
Algoritmo de Bresenham en 3D: input: punto inicial pi, punto final pf.
                              output: TList* Parche.

Une los puntos pi y pf con una serie de puntos "parche". El algoritmo garantiza
que los puntos del parche formen una línea continua, y de ancho unitario (es
decir, cada punto de la línea tiene como máximo 2 vecinos en la línea). 
*******************************************************************************/
void Bresenham3D(punto pi, punto pf, TList* Parche)
{
    int xxx = pi.x;
    int yyy = pi.y;
    int zzz = pi.z;

    int dx = pf.x - pi.x;
    int dy = pf.y - pi.y;
    int dz = pf.z - pi.z;

    int x_inc, y_inc, z_inc;
    int Adx, Ady, Adz;
    int dx2, dy2, dz2;

    int err_1, err_2;

    punto* paux;

   if (dx < 0 )
     x_inc = -1;
   else
     x_inc =  1;


   if (dy < 0)
     y_inc = -1;
   else
     y_inc =  1;

   if (dz < 0)
     z_inc = -1;
   else
     z_inc =  1;

    Adx = abs(dx);
    Ady = abs(dy);
    Adz = abs(dz);

    dx2 = Adx*2;
    dy2 = Ady*2;
    dz2 = Adz*2;
   
   if (Adx>=Ady && Adx>=Adz)
   {
      err_1 = dy2 - Adx;
      err_2 = dz2 - Adx;

      for (int Cont = 0; Cont<Adx-1; Cont++)
      {
         if (err_1 > 0)
         {
             yyy = yyy + y_inc;
             err_1 = err_1-dx2;
         }

         if (err_2 > 0)
         {
             zzz = zzz + z_inc;
             err_2 = err_2 - dx2;
         }

         err_1 = err_1 + dy2;
         err_2 = err_2 + dz2;
         xxx = xxx + x_inc;

        paux = new punto;
        paux->x=xxx; paux->y=yyy; paux->z=zzz;
        Parche->Add(paux);
      }

   }

  if (Ady>Adx && Ady>=Adz)
  {
     err_1 = dx2 - Ady;
     err_2 = dz2 - Ady;

     for (int Cont = 0; Cont<Ady-1; Cont++)
     {
       if (err_1 > 0)
       {
            xxx = xxx + x_inc;
            err_1 = err_1 - dy2;
       }

       if (err_2 > 0)
       {
            zzz = zzz + z_inc;
            err_2 = err_2 - dy2;
       }

        err_1 = err_1 + dx2;
        err_2 = err_2 + dz2;
        yyy = yyy + y_inc;

        paux = new punto;
        paux->x=xxx; paux->y=yyy; paux->z=zzz;
        Parche->Add(paux);
     }

  }

   if (Adz>Adx && Adz>Ady)
   {
      err_1 = dy2 - Adz;
      err_2 = dx2 - Adz;

      for (int Cont = 0; Cont<Adz-1; Cont++)
      {
         if (err_1 > 0)
         {
             yyy = yyy + y_inc;
             err_1 = err_1 - dz2;
         }

         if (err_2>0)
         {
             xxx = xxx + x_inc;
             err_2 = err_2 - dz2;
         }

         err_1 = err_1 + dy2;
         err_2 = err_2 + dx2;
         zzz = zzz + z_inc;

        paux = new punto;
        paux->x=xxx; paux->y=yyy; paux->z=zzz;
        Parche->Add(paux);
      }
  }

}

//------------------------------------------------------------------------------

void BressenhamCircle (int cx, int cy, int cz, int radius, TList* Circle)
{

//Input: (cx,cy,cz) = centro de la circunferencia
//           radius = radio del círculo

//Output: TList Circle: lista de Puntos con los puntos de la circunferencia 

  int error = -radius;
  int x = radius;
  int y = 0;

  punto* paux;
  
  while (x >= y)
  {

     paux = new punto;
     paux->x = cx+x;
     paux->y = cy+y;
     paux->z = cz;
     Circle->Add(paux);

     if (x != 0)
     {paux = new punto;
     paux->x = cx-x;
     paux->y = cy+y;
     paux->z = cz;
     Circle->Add(paux);}

     if (y != 0)
     {
     paux = new punto;
     paux->x = cx+x;
     paux->y = cy-y;
     paux->z = cz;
     Circle->Add(paux);}

     if (x != 0 && y != 0)
     {paux = new punto;
     paux->x = cx-x;
     paux->y = cy-y;
     paux->z = cz;
     Circle->Add(paux);}

   if (x != y)
   {
     paux = new punto;
     paux->x = cx+y;
     paux->y = cy+x;
     paux->z = cz;
     Circle->Add(paux);

     if (x != 0)
     {paux = new punto;
     paux->x = cx-y;
     paux->y = cy+x;
     paux->z = cz;
     Circle->Add(paux);}

     if (y != 0)
     {
     paux = new punto;
     paux->x = cx+y;
     paux->y = cy-x;
     paux->z = cz;
     Circle->Add(paux);}

     if (x != 0 && y != 0)
     {paux = new punto;
     paux->x = cx-y;
     paux->y = cy-x;
     paux->z = cz;
     Circle->Add(paux);}
   }

    error += y;
    ++y;
    error += y;

    // The following test may be implemented in assembly language in
    // most machines by testing the carry flag after adding 'y' to
    // the value of 'error' in the previous step, since 'error'
    // nominally has a negative value.
    if (error >= 0)
    {
      --x;
      error -= x;
      error -= x;
    }
  }
}


