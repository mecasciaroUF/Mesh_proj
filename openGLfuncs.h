#include <gl\gl.h>
#include <gl\glu.h>
#include "glut.h"

//------------------------------------------------------------------------------
//**Dibuja ejes con los r�tulos "x" "y" "z"
void drawaxes(int medX, int medY, int medZ)
{
   /* glEnable(GL_DEPTH_TEST);

    //Eje X: Rojo
    glLineWidth(3.0f);
    glColor4f(1.0, 0.0, 0.0, 0.5);
    glBegin(GL_LINE_STRIP);
    glVertex3f(-medX/2.0, -medY/2.0, -medZ/2.0);
    glVertex3f(medX/2.0, -medY/2, -medZ/2.0);
    glVertex3f(medX/2.0+0.75, -medY/2.0+10.25, -medZ/2.0);
    glVertex3f(medX/2.0+0.75, -medY/2.0-10.25, -medZ/2.0);
    glVertex3f(medX/2.0, -medY/2.0, -medZ/2.0);
    glVertex3f(medX/2.0+0.75, -medY/2.0, -medZ/2.0+10.25);
    glVertex3f(medX/2.0+0.75, -medY/2.0, -medZ/2.0-10.25);
    glVertex3f(medX/2.0, -medY/2.0, -medZ/2.0);
    glEnd();

    glLineWidth(5.0f);
    glBegin(GL_LINE_STRIP);
    glVertex3f(-medX/2.0, medY/2.0, -medZ/2.0);
    glVertex3f(medX/2.0, medY/2.0, -medZ/2.0);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex3f(-medX/2.0, -medY/2.0, medZ/2.0);
    glVertex3f(medX/2.0, -medY/2.0, medZ/2.0);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex3f(-medX/2.0, medY/2.0, medZ/2.0);
    glVertex3f(medX/2.0, medY/2.0, medZ/2.0);
    glEnd();

    //Eje Y: Verde
    glLineWidth(3.0f);
    glColor4f(0.0, 1.0, 0.0, 0.5);
    glBegin(GL_LINE_STRIP);
    glVertex3f(-medX/2.0, -medY/2.0,-medZ/2.0);
    glVertex3f(-medX/2.0, medY/2.0, -medZ/2.0);
    glVertex3f(-medX/2.0, medY/2.0+0.75, -medZ/2.0+10.25);
    glVertex3f(-medX/2.0, medY/2.0+0.75, -medZ/2.0-10.25);
    glVertex3f(-medX/2.0, medY/2.0, -medZ/2.0);
    glVertex3f(-medX/2.0+10.25, medY/2.0+0.75, -medZ/2.0);
    glVertex3f(-medX/2.0-10.25, medY/2.0+0.75, -medZ/2.0);
    glVertex3f(-medX/2.0, medY/2.0, -medZ/2.0);
    glEnd();

    glLineWidth(5.0f);
    glBegin(GL_LINE_STRIP);
    glVertex3f(medX/2.0, -medY/2.0,-medZ/2.0);
    glVertex3f(medX/2.0, medY/2.0, -medZ/2.0);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex3f(-medX/2.0, -medY/2.0,medZ/2.0);
    glVertex3f(-medX/2.0, medY/2.0, medZ/2.0);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex3f(medX/2.0, -medY/2.0,medZ/2.0);
    glVertex3f(medX/2.0, medY/2.0, medZ/2.0);
    glEnd();

    //Eje Z: Azul
    glLineWidth(3.0f);
    glColor4f(0.0, 0.0, 1.0, 0.5);
    glBegin(GL_LINE_STRIP);
    glVertex3f(-medX/2.0, -medY/2.0,-medZ/2.0);
    glVertex3f(-medX/2.0, -medY/2.0, medZ/2.0);
    glVertex3f(-medX/2.0+10.25, -medY/2.0, medZ/2.0+0.75);
    glVertex3f(-medX/2.0-10.25, -medY/2.0, medZ/2.0+0.75);
    glVertex3f(-medX/2.0, -medY/2.0, medZ/2.0);
    glVertex3f(-medX/2.0, -medY/2.0+10.25, medZ/2.0+0.75);
    glVertex3f(-medX/2.0, -medY/2.0-10.25, medZ/2.0+0.75);
    glVertex3f(-medX/2.0, -medY/2.0, medZ/2.0);
    glEnd();

    glLineWidth(5.0f);
    glBegin(GL_LINE_STRIP);
    glVertex3f(medX/2.0, -medY/2.0,-medZ/2.0);
    glVertex3f(medX/2.0, -medY/2.0, medZ/2.0);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex3f(-medX/2.0, medY/2.0,-medZ/2.0);
    glVertex3f(-medX/2.0, medY/2.0, medZ/2.0);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex3f(medX/2.0, medY/2.0,-medZ/2.0);
    glVertex3f(medX/2.0, medY/2.0, medZ/2.0);
    glEnd();

    glDisable(GL_DEPTH_TEST);  */

    //largox,y,z: largo en px de los ejes
//offset: distancia entre ejes y r�tulos 'x', 'y', 'z'

    int largox = medX;
    int largoy = medY;
    int largoz = medZ;
    int offset = 20;

    glLineWidth(2);

    glEnable(GL_DEPTH_TEST);
   // glDepthMask(GL_TRUE);

    glBegin(GL_LINES);
    {
      //Y-axis:
      glColor3ub(255, 0, 0);
      glVertex3f(medY, medX, medZ);
      glVertex3f(medY+largoy, medX, medZ);

      //X-axis:
      glColor3ub(0, 150, 0);
      glVertex3f(medY, medX, medZ);
      glVertex3f(medY, medX+largox, medZ);

      //Z-axis:
      glColor3ub(0, 0, 255);
      glVertex3f(medY, medX, medZ);
      glVertex3f(medY, medX, medZ+largoz);
    }
    glEnd();

    //void glutSolidCone(GLdouble base, GLdouble height, GLint slices, GLint stacks);
    //Eje X-flecha:
    glColor3ub(255, 0, 0);
    glRasterPos3f(largox+offset, 0.0, 0.0);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'x');

    glPushMatrix();
     {
     glTranslatef(largox, 0.0, 0.0);
     glRotatef(90,0,1,0);

     //cono externo
     glFrontFace(GL_CCW);
     glutSolidCone(3, 5, 5, 1);

     //cono interno
     glColor3ub(150, 0, 0);
     glFrontFace(GL_CW);
     glutSolidCone(3, 5, 5, 1);
     glColor3ub(0, 0, 0);
     glutWireCone(3, 5, 5, 1);
     }
    glPopMatrix();

    //Eje Y-flecha:
    glColor3ub(0, 150, 0);
    glRasterPos3f(0.0, largoy+offset, 0.0);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'y');

    glPushMatrix();
    {
     glTranslatef(0.0, largoy, 0.0);
     glRotatef(-90,1,0,0);

     //cono externo
     glFrontFace(GL_CCW);
     glutSolidCone(3, 5, 5, 1);

     //cono interno
     glColor3ub(0, 50, 0);
     glFrontFace(GL_CW);
     glutSolidCone(3, 5, 5, 1);
     glColor3ub(0, 0, 0);
     glutWireCone(3, 5, 5, 1);
    }
    glPopMatrix();

    //Eje Z-flecha:
    glColor3ub(0, 0, 255);
    glRasterPos3f(0.0, 0.0, largoz+offset);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'z');

    glPushMatrix();
    {
     glTranslatef(0.0, 0.0, largoz);
     //glRotatef(90,0,0,1); :no es necesario por como dibuja el cono por default

     //cono externo
     glFrontFace(GL_CCW);
     glutSolidCone(3, 5, 5, 1);

     //cono interno
     glColor3ub(0, 0, 150);
     glFrontFace(GL_CW);
     glutSolidCone(3, 5, 5, 1);
     glColor3ub(0, 0, 0);
     glutWireCone(3, 5, 5, 1);
    }
    glPopMatrix();

   glDisable(GL_DEPTH_TEST);
   glLineWidth(1);
}
//------------------------------------------------------------------------------
void drawCarina(float xC, float yC, float zC)
{
    glPointSize(15.0);

    glBegin(GL_POINTS);
    {
      //Punto de la Carina
      glColor3ub(255.0, 0, 0);
      glVertex3f(xC, yC, zC);
    }
    glEnd();

    glColor3ub(255.0, 0, 0);
    glRasterPos3f(xC, yC, zC);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'K');

    glPointSize(1.0);

}
//------------------------------------------------------------------------------
void drawCircle(float xC, float yC, float zC, float R, int colorR, int colorG, int colorB)
{
    glPointSize(10.0);

    glBegin(GL_POINTS);
    {
      //Punto de la Carina
      glColor3ub(colorR, colorG, colorB);
      glVertex3f(xC, yC, zC);
    }
    glEnd();

   float x,y;

   glBegin(GL_LINE);
   //glPointSize(0.25);

   for(int i=0; i<=360; i++)
   {
   x = R*sin(float(i)/180.0*M_PI) + xC;
   y = R*cos(float(i)/180.0*M_PI) + yC;
   glVertex3f(x, y, zC);
   }


   glEnd();
}
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
void drawWalls(int medX, int medY, int medZ)
{
    //Pared XZ:
    glEnable(GL_BLEND);
    //glBlendFunc(GL_ONE,GL_DST_ALPHA);
    glBlendFunc(GL_DST_ALPHA,GL_SRC_ALPHA);
    glBegin(GL_QUADS);
    glShadeModel(GL_SMOOTH);
    glColor4f(0.6, 0.3, 0.6, 0.55);
    glVertex3f(-medX/2.0, -medY/2.0,-medZ/2.0);
    glVertex3f(medX/2.0, -medY/2.0, -medZ/2.0);
    glVertex3f(medX/2.0, -medY/2.0, medZ/2.0);
    glVertex3f(-medX/2.0, -medY/2.0, medZ/2.0);
    glEnd();
    glDisable(GL_BLEND);

    //Pared YZ;
    glEnable(GL_BLEND);
    //glBlendFunc(GL_ONE,GL_DST_ALPHA);
    glBlendFunc(GL_DST_ALPHA,GL_SRC_ALPHA);
    glBegin(GL_QUADS);
    glShadeModel(GL_SMOOTH);
    glColor4f(0.3, 0.6, 0.6, 0.55);
    glVertex3f(-medX/2.0, -medY/2.0,-medZ/2.0);
    glVertex3f(-medX/2.0, medY/2.0, -medZ/2.0);
    glVertex3f(-medX/2.0, medY/2.0,  medZ/2.0);
    glVertex3f(-medX/2.0, -medY/2.0, medZ/2.0);
    glEnd();
    glDisable(GL_BLEND);

    //Pared XY;
    glEnable(GL_BLEND);
    //glBlendFunc(GL_ONE,GL_DST_ALPHA);
    glBlendFunc(GL_DST_ALPHA,GL_SRC_ALPHA);
    glBegin(GL_QUADS);
    glShadeModel(GL_SMOOTH);
    glColor4f(0.6, 0.6, 0.3, 0.55);
    glVertex3f(-medX/2.0, -medY/2.0,-medZ/2.0);
    glVertex3f( medX/2.0, -medY/2.0,-medZ/2.0);
    glVertex3f( medX/2.0,  medY/2.0, -medZ/2.0);
    glVertex3f(-medX/2.0,  medY/2.0,-medZ/2.0);
    glEnd();
    glDisable(GL_BLEND);

    glDisable(GL_DEPTH_TEST);

}
//------------------------------------------------------------------------------
void drawVoxels(TList* Puntos,
                 TList* EP, TList* LP, TList* JP,
                 TList* BP, TList* EraseP, TList* NL, int n, TList* RL, int r, int** MNR, int cn, int cr,
                 short int medX, short int medY, short int medZ, bool OnTop)
{
/*
Puntos: puntos del esqueleto
EP: exit points, LP: line points, JP: joint-points
BP: bressenham points (puntos para unir por bressenham)
EraseP: puntos a eliminar.
NL: lista de nodos, con n=nodo a pintar
RL: lista de ramas, con r=rama a pintar
MNR: matrizNodoRama, se�alando la conectividad de cada nodo con cada rama
*/
      Punto* paux;

      float color[4];
      TList* AuxList;

      if(!OnTop)
      {
      glEnable(GL_DEPTH_TEST);

      //DibujaBressPoints:
       color[0]=0.9; color[1]=0.0; color[2]=0.9; color[3]=0.3;;

       for(int i=0; i<BP->Count; i++)
      {
        paux = (Punto*)BP->Items[i];
                if(paux->x!= NULL)
                {
                glPushMatrix();
            //      glEnable(GL_BLEND);
            //        glBlendFunc(GL_DST_ALPHA,GL_SRC_ALPHA);
                    glShadeModel(GL_SMOOTH);

                    //lados de la caja: glutWireCube
                    glTranslatef(paux->x-medX,paux->y-medY,-paux->z+medZ);
                    glColor4f(0.0f, 0.0f, 0.0f, 0.1f);
                    glutWireCube(1);

                    //paredes de la caja: glutSolidCube
                    glColor4f(color[0],color[1],color[2],color[3]);
                    glutSolidCube(1);
              //    glDisable(GL_BLEND);
                glPopMatrix();
                }
      }

      //DibujaErasePoints:
       color[0]=1.0; color[1]=0.0; color[2]=0.0; color[3]=0.3;

       for(int i=0; i<EraseP->Count; i++)
      {
        paux = (Punto*)EraseP->Items[i];
                if(paux->x!= NULL)
                {
                glPushMatrix();
                //  glEnable(GL_BLEND);
                //    glBlendFunc(GL_DST_ALPHA,GL_SRC_ALPHA);
                    glShadeModel(GL_SMOOTH);

                    //lados de la caja: glutWireCube
                    glTranslatef(paux->x-medX,paux->y-medY,-paux->z+medZ);
                    glColor4f(0.0f, 0.0f, 0.0f, 0.1f);
                    glutWireCube(1);

                    //paredes de la caja: glutSolidCube
                    glColor4f(color[0],color[1],color[2],color[3]);
                    glutSolidCube(1);
              //    glDisable(GL_BLEND);
                glPopMatrix();
                }
      }

      }

      //DibujaEndPoints:
      color[0]=0.0; color[1]=0.0; color[2]=1.0; color[3]=0.3;

      for(int i=0; i<EP->Count; i++)
      {
        paux = (Punto*)EP->Items[i];
                if(paux->x!= NULL)
                {
                glPushMatrix();
            //      glEnable(GL_BLEND);
            //        glBlendFunc(GL_DST_ALPHA,GL_SRC_ALPHA);
                    glShadeModel(GL_SMOOTH);

                    //lados de la caja: glutWireCube
                    glTranslatef(paux->x-medX,paux->y-medY,-paux->z+medZ);
                    glColor4f(0.0f, 0.0f, 0.0f, 0.1f);
                    glLineWidth(0.1f);
                    glutWireCube(1);

                    //paredes de la caja: glutSolidCube
                    glColor4f(color[0],color[1],color[2],color[3]);
                    glutSolidCube(1);
            //      glDisable(GL_BLEND);
                glPopMatrix();
                }
      }

      //DibujaLinePoints:
      color[0]=0.0; color[1]=0.5; color[2]=0.0; color[3]=0.3;

      for(int i=0; i<LP->Count; i++)
      {
        paux = (Punto*)LP->Items[i];
                if(paux->x!= NULL)
                {
                glPushMatrix();
             //     glEnable(GL_BLEND);
             //       glBlendFunc(GL_DST_ALPHA,GL_SRC_ALPHA);
                    glShadeModel(GL_SMOOTH);

                    //lados de la caja: glutWireCube
                    glTranslatef(paux->x-medX,paux->y-medY,-paux->z+medZ);
                    glColor4f(0.0f, 0.0f, 0.0f, 0.1f);
                    glLineWidth(0.1f);
                    glutWireCube(1);

                    //paredes de la caja: glutSolidCube
                    glColor4f(color[0],color[1],color[2],color[3]);
                    glutSolidCube(1);
            //      glDisable(GL_BLEND);
                glPopMatrix();
                }
      }

      //DibujaJointPoints:
      color[0]=0.8; color[1]=0.5; color[2]=0.0; color[3]=0.3;

      for(int i=0; i<JP->Count; i++)
      {
        paux = (Punto*)JP->Items[i];
                if(paux->x!= NULL)
                {
                glPushMatrix();
             //     glEnable(GL_BLEND);
            //        glBlendFunc(GL_DST_ALPHA,GL_SRC_ALPHA);
                    glShadeModel(GL_SMOOTH);

                    //lados de la caja: glutWireCube
                    glTranslatef(paux->x-medX,paux->y-medY,-paux->z+medZ);
                    glColor4f(0.0f, 0.0f, 0.0f, 0.1f);
                    glLineWidth(0.1f);
                    glutWireCube(1);

                    //paredes de la caja: glutSolidCube
                    glColor4f(color[0],color[1],color[2],color[3]);
                    glutSolidCube(1);
             //     glDisable(GL_BLEND);
                glPopMatrix();
                }
      }

     //DibujaNodo N:
     if(NL->Count)
     {
      color[0]=1.0; color[1]=1.0; color[2]=1.0; color[3]=0.3;
      AuxList = (TList*)NL->Items[n];

      for(int i=0; i<AuxList->Count; i++)
      {
        paux = (Punto*)AuxList->Items[i];
                if(paux->x!= NULL)
                {
                glPushMatrix();
            //      glEnable(GL_BLEND);
            //        glBlendFunc(GL_DST_ALPHA,GL_SRC_ALPHA);
                    glShadeModel(GL_SMOOTH);

                    //lados de la caja: glutWireCube
                    glTranslatef(paux->x-medX,paux->y-medY,-paux->z+medZ);
                    glColor4f(0.0f, 0.0f, 0.0f, 0.1f);
                    glLineWidth(0.1f);
                    glutWireCube(1);

                    //paredes de la caja: glutSolidCube
                    glColor4f(color[0],color[1],color[2],color[3]);
                    glutSolidCube(1);
            //      glDisable(GL_BLEND);
                glPopMatrix();
                }
      }
     }

     //DibujaRamas Conectadas al nodo N:
     if(NL->Count)
     {
      for(int k=0; k<cr; k++)
      {
        if(MNR[n][k]==1 || MNR[n][k]==2)
        {
         if(MNR[n][k]==1)
         {color[0]=1.0; color[1]=0.0; color[2]=1.0; color[3]=0.3;}

         if(MNR[n][k]==2)
         {color[0]=1.0; color[1]=0.0; color[2]=0.0; color[3]=0.3;}

         AuxList = (TList*)RL->Items[k];

          for(int i=0; i<AuxList->Count; i++)
          {
          paux = (Punto*)AuxList->Items[i];
                if(paux->x!= NULL)
                {
                glPushMatrix();
              //    glEnable(GL_BLEND);
              //      glBlendFunc(GL_DST_ALPHA,GL_SRC_ALPHA);
                    glShadeModel(GL_SMOOTH);

                    //lados de la caja: glutWireCube
                    glTranslatef(paux->x-medX,paux->y-medY,-paux->z+medZ);
                    glColor4f(0.0f, 0.0f, 0.0f, 0.1f);
                    glLineWidth(0.1f);
                    glutWireCube(1);

                    //paredes de la caja: glutSolidCube
                    glColor4f(color[0],color[1],color[2],color[3]);
                    glutSolidCube(1);
             //     glDisable(GL_BLEND);
                glPopMatrix();
                }
           }
        }
      }
     }

      //DibujaRama r:
     if(RL->Count)
     {
      color[0]=0.5; color[1]=0.5; color[2]=0.5; color[3]=0.3;

      AuxList = (TList*)RL->Items[r];

      for(int i=0; i<AuxList->Count; i++)
      {
        paux = (Punto*)AuxList->Items[i];
                if(paux->x!= NULL)
                {
                glPushMatrix();
             //     glEnable(GL_BLEND);
             //       glBlendFunc(GL_DST_ALPHA,GL_SRC_ALPHA);
                    glShadeModel(GL_SMOOTH);

                    //lados de la caja: glutWireCube
                    glTranslatef(paux->x-medX,paux->y-medY,-paux->z+medZ);
                    glColor4f(0.0f, 0.0f, 0.0f, 0.1f);
                    glLineWidth(0.1f);
                    glutWireCube(1);

                    //paredes de la caja: glutSolidCube
                    glColor4f(color[0],color[1],color[2],color[3]);
                    glutSolidCube(1);
              //    glDisable(GL_BLEND);
                glPopMatrix();
                }
      }
     }

     if(OnTop)
     {
     //DibujaBressPoints:
       color[0]=0.9; color[1]=0.0; color[2]=0.9; color[3]=0.3;;

       for(int i=0; i<BP->Count; i++)
      {
        paux = (Punto*)BP->Items[i];
                if(paux->x!= NULL)
                {
                glPushMatrix();
           //       glEnable(GL_BLEND);
           //         glBlendFunc(GL_DST_ALPHA,GL_SRC_ALPHA);
                    glShadeModel(GL_SMOOTH);

                    //lados de la caja: glutWireCube
                    glTranslatef(paux->x-medX,paux->y-medY,-paux->z+medZ);
                    glColor4f(0.0f, 0.0f, 0.0f, 0.1f);
                    glutWireCube(1);

                    //paredes de la caja: glutSolidCube
                    glColor4f(color[0],color[1],color[2],color[3]);
                    glutSolidCube(1);
          //        glDisable(GL_BLEND);
                glPopMatrix();
                }
      }

      //DibujaErasePoints:
       color[0]=1.0; color[1]=0.0; color[2]=0.0; color[3]=0.3;

       for(int i=0; i<EraseP->Count; i++)
      {
        paux = (Punto*)EraseP->Items[i];
                if(paux->x!= NULL)
                {
                glPushMatrix();
        //          glEnable(GL_BLEND);
        //            glBlendFunc(GL_DST_ALPHA,GL_SRC_ALPHA);
                    glShadeModel(GL_SMOOTH);

                    //lados de la caja: glutWireCube
                    glTranslatef(paux->x-medX,paux->y-medY,-paux->z+medZ);
                    glColor4f(0.0f, 0.0f, 0.0f, 0.1f);
                    glutWireCube(1);

                    //paredes de la caja: glutSolidCube
                    glColor4f(color[0],color[1],color[2],color[3]);
                    glutSolidCube(1);
        //          glDisable(GL_BLEND);
                glPopMatrix();
                }
      }

     }

     if(!OnTop)
     glDisable(GL_DEPTH_TEST);


}
//------------------------------------------------------------------------------------------------

void SetupTexturesFromBMP(TImage* imagen, GLuint &textura, Graphics::TBitmap* bitmap)
{
    //Texturas:
    bitmap = new Graphics::TBitmap;
    bitmap = imagen->Picture->Bitmap;

    GLubyte bits[256][256][3];

      for(int i = 0; i < 256; i++)
    {
    	for(int j = 0; j < 256; j++)
        {
            bits[i][j][0]= (GLbyte)GetRValue(bitmap->Canvas->Pixels[i][j]);
            bits[i][j][1]= (GLbyte)GetRValue(bitmap->Canvas->Pixels[i][j]);
            bits[i][j][2]= (GLbyte)GetRValue(bitmap->Canvas->Pixels[i][j]);
           // bits[i][j][3]= (GLbyte)255;
        }
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
    glGenTextures(1, &textura);
    glBindTexture(GL_TEXTURE_2D, textura);

  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, bits);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
}
//------------------------------------------------------------------------------
void SetupTexturesFromArbitraryPlane(Volumen* M, float *LUT, short int minPX,
             PuntoF** Plano, int anchoPlano, int altoPlano, int cant, GLuint &textura)
{
//Texturas:
    GLubyte bits[512][512][3];

      for(int i = 0; i < altoPlano; i++)
    	for(int j = 0; j < anchoPlano; j++)
        {
            if(Plano[i][j].x>1&&Plano[i][j].x<altoPlano-1&&
               Plano[i][j].y>1&&Plano[i][j].y<anchoPlano-1&&
               Plano[i][j].z>1&&Plano[i][j].z<cant-1)
            {
            bits[i][j][0]= (GLbyte)LUT[M->GetV(Plano[i][j].x,Plano[i][j].y,Plano[i][j].z)-minPX+1];
            bits[i][j][1]= 0;//(GLbyte)LUT[M->GetV(Plano[i][j].x,Plano[i][j].y,Plano[i][j].z)-minPX]/3.0;
            bits[i][j][2]= 0;//(GLbyte)LUT[M->GetV(Plano[i][j].x,Plano[i][j].y,Plano[i][j].z)-minPX]/3.0;
            }
            else
            {
            bits[i][j][0]=0;
            bits[i][j][1]=0;
            bits[i][j][2]=0;
            }
        }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
    glGenTextures(1, &textura);
    glBindTexture(GL_TEXTURE_2D, textura);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, altoPlano, anchoPlano, 0, GL_RGB, GL_UNSIGNED_BYTE, bits);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
}
//------------------------------------------------------------------------------
void SetupTexturesFromMatrix(Volumen* M, short int minPX,
                             short int alto, short int ancho, short int prof,
                             float* LUT,
                             unsigned char direccion, short int fixeddir,
                             GLuint &textura)
{
//Texturas:
    GLubyte bits[512][512][3];

     for(int i = 0; i < 512; i++)
    {
    	for(int j = 0; j < 512; j++)
        {
            bits[i][j][0]= 0;
            bits[i][j][1]= 0;
            bits[i][j][2]= 0;
        }
    }

    if(direccion == 'x')
    for(int i = 0; i < ancho; i++)
    {
    	for(int j = 0; j < prof; j++)
        {
            bits[i][j][0]= (GLbyte)LUT[M->GetV(fixeddir,i,j)-minPX];
            bits[i][j][1]= (GLbyte)LUT[M->GetV(fixeddir,i,j)-minPX];
            bits[i][j][2]= (GLbyte)LUT[M->GetV(fixeddir,i,j)-minPX];
        }
    }

    if(direccion == 'y')
    for(int i = 0; i < prof; i++)
    {
    	for(int j = 0; j < alto; j++)
        {
            bits[i][j][0]= (GLbyte)LUT[M->GetV(j,fixeddir,i)-minPX];
            bits[i][j][1]= (GLbyte)LUT[M->GetV(j,fixeddir,i)-minPX];
            bits[i][j][2]= (GLbyte)LUT[M->GetV(j,fixeddir,i)-minPX];
        }
    }

    if(direccion == 'z')
    for(int i = 0; i < alto; i++)
    {
    	for(int j = 0; j < ancho; j++)
        {
            bits[i][j][0]= (GLbyte)LUT[M->GetV(i,j,fixeddir)-minPX];
            bits[i][j][1]= (GLbyte)LUT[M->GetV(i,j,fixeddir)-minPX];
            bits[i][j][2]= (GLbyte)LUT[M->GetV(i,j,fixeddir)-minPX];
        }
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
    glGenTextures(1, &textura);
    glBindTexture(GL_TEXTURE_2D, textura);

  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, bits);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
}

//------------------------------------------------------------------------------

void SetupTexturesFromMatrixPosition(Volumen* M, short int minPX,
                             short int alto, short int ancho, short int prof,
                             float* LUT,
                             GLuint& textura, int posicion, GLubyte*** bits)
{

     for(int i = 0; i < alto; i++)
        for(int j = 0; j < ancho; j++)
        {
        bits[i][j][0]= (GLbyte)LUT[M->GetV(i,j,posicion)-minPX];
        bits[i][j][1]= (GLbyte)LUT[M->GetV(i,j,posicion)-minPX];
        bits[i][j][2]= (GLbyte)LUT[M->GetV(i,j,posicion)-minPX];
       // bits[i][j][3]= (GLbyte)LUT[M->GetV(i,j,posicion)-minPX];
        }

 glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &textura);
    glBindTexture(GL_TEXTURE_2D, textura);

  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
    glGenTextures(1, &textura);
    glBindTexture(GL_TEXTURE_2D, textura);

  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, bits);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);




/*for(int i = 0; i < alto; i++)
 for(int j = 0; j < ancho; j++)
    delete[] bits[i][j];

for(int i = 0; i < alto; i++)
        delete[] bits[i];

delete[] bits; */

}
//------------------------------------------------------------------------------
void SetPixelFormatDescriptor(HDC &hdc, int &PixelFormat)
{
    //Rutina de inicializacion de OpenGL: Copiada textual de el ejemplo
    //presente en Borland/Examples/OpenGL
    PIXELFORMATDESCRIPTOR pfd = {
    	sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        24,
        0,0,0,0,0,0,
        0,0,
        0,0,0,0,0,
        32,
        0,
        0,
        PFD_MAIN_PLANE,
        0,
        0,0,
    };
    PixelFormat = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, PixelFormat, &pfd);
}
//------------------------------------------------------------------------------
void SetupRenderingContextPanel(TPanel* panel, HDC &hDC,  HGLRC &hRC,
                                int &PanelWidth, int &PanelHeight,
                                int &PixelFormat)
{
    hDC = GetDC(panel->Handle);//(PanelVista3D->Handle);

    SetPixelFormatDescriptor(hDC, PixelFormat);

    hRC = wglCreateContext(hDC);
    if(hRC == NULL)
    	ShowMessage(":-)~ hrc == NULL");
    if(wglMakeCurrent(hDC, hRC) == false)
    	ShowMessage("Could not MakeCurrent");

    PanelWidth = panel->Width;//;PanelVistaXY->Width;
    PanelHeight = panel->Height;//;PanelVistaXY->Height;

    // glutInitDisplayMode(GLUT_RGBA |      /* RGB and Alpha */
    //                GLUT_SINGLE|     /* Single buffer */
    //                GLUT_DEPTH);     /* Z buffer (depth) */

    //Color de Fondo
    //glClearColor(1.0f, 1.0f, 0.9f, 0.1f);
    //glClearColor(0.6f, 0.6f, 0.6f, 0.6f);
    glClearColor(0.2f, 0.2f, 0.8f, 0.0f);

    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);


  // glDisable(GL_DEPTH_TEST);
  //  glDepthFunc(GL_LESS);
}
//------------------------------------------------------------------------------
void drawMarcos(TScrollBar* XScrollBar, TScrollBar* YScrollBar, TScrollBar* ZScrollBar,
                short int anchoPX, short int altoPX, short int cantImagenes,
                int textura1, int textura2, int textura3)
{
      //DibujarMarcos:
      glLineWidth(5.0f);
      glColor4f(1.0, 0.0, 0.0, 0.5);

      glBegin(GL_LINE_STRIP);
      glVertex3f(-256.0f, -256.0f,  256.0f-ZScrollBar->Position);
      glVertex3f(-256.0f, 256.0f, 256.0-ZScrollBar->Position);
      glVertex3f(256.0f, 256.0f,  256.0-ZScrollBar->Position);
      glVertex3f(256.0f, -256.0f,  256.0-ZScrollBar->Position);
      glVertex3f(-256.0f, -256.0f,  256.0f-ZScrollBar->Position);
      glEnd();

      glLineWidth(5.0f);
      glColor4f(0.0, 0.7, 0.0, 0.5);
      glBegin(GL_LINE_STRIP);
      	glVertex3f(anchoPX/2-XScrollBar->Position, -256.0f,  256.0f);
	glVertex3f(anchoPX/2-XScrollBar->Position, 256.0f,  256.0f);
	glVertex3f(anchoPX/2-XScrollBar->Position,  256.0f,  -256.0f);
	glVertex3f(anchoPX/2-XScrollBar->Position,  -256.0f,  -256.0f);
        glVertex3f(anchoPX/2-XScrollBar->Position, -256.0f,  256.0f);
      glEnd();

      glLineWidth(5.0f);
      glColor4f(0.0, 0.0, 0.8, 0.5);
      glBegin(GL_LINE_STRIP);
      glVertex3f(-256.0f, YScrollBar->Position-256, 256.0f);
      glVertex3f(256.0f, YScrollBar->Position-256, 256.0f);
      glVertex3f(256.0f, YScrollBar->Position-256, -256.0f);
      glVertex3f(-256.0f, YScrollBar->Position-256, -256.0f);
      glVertex3f(-256.0f, YScrollBar->Position-256, 256.0f);
      glEnd();

      //DibujarSeparadores:
       glLineWidth(1.0f);
      glColor4f(1.0, 1.0, 1.0, 0.5);
      glBegin(GL_LINES);
      glVertex3f((anchoPX/2-XScrollBar->Position-1), (-256.0f),(256.0f-ZScrollBar->Position-1));
      glVertex3f((anchoPX/2-XScrollBar->Position-1), (256.0f), (256.0f-ZScrollBar->Position-1));
      glVertex3f((anchoPX/2-XScrollBar->Position-1), (-256.0f),(256.0f-ZScrollBar->Position+1));
      glVertex3f((anchoPX/2-XScrollBar->Position-1), (256.0f), (256.0f-ZScrollBar->Position+1));
      glVertex3f((anchoPX/2-XScrollBar->Position+1), (-256.0f),(256.0f-ZScrollBar->Position-1));
      glVertex3f((anchoPX/2-XScrollBar->Position+1), (256.0f), (256.0f-ZScrollBar->Position-1));
      glVertex3f((anchoPX/2-XScrollBar->Position+1), (-256.0f),(256.0f-ZScrollBar->Position+1));
      glVertex3f((anchoPX/2-XScrollBar->Position+1), (256.0f), (256.0f-ZScrollBar->Position+1));

      glVertex3f((anchoPX/2-XScrollBar->Position-1), (YScrollBar->Position-256-1), (-256.0f));
      glVertex3f((anchoPX/2-XScrollBar->Position-1), (YScrollBar->Position-256-1), ( 256.0f));
      glVertex3f((anchoPX/2-XScrollBar->Position-1), (YScrollBar->Position-256+1), (-256.0f));
      glVertex3f((anchoPX/2-XScrollBar->Position-1), (YScrollBar->Position-256+1), ( 256.0f));
      glVertex3f((anchoPX/2-XScrollBar->Position+1), (YScrollBar->Position-256+1), (-256.0f));
      glVertex3f((anchoPX/2-XScrollBar->Position+1), (YScrollBar->Position-256+1), ( 256.0f));
      glVertex3f((anchoPX/2-XScrollBar->Position+1), (YScrollBar->Position-256-1), (-256.0f));
      glVertex3f((anchoPX/2-XScrollBar->Position+1), (YScrollBar->Position-256-1), ( 256.0f));

      glVertex3f((-256.0f), (YScrollBar->Position-256-1), (256-ZScrollBar->Position-1));
      glVertex3f((256.0f ), (YScrollBar->Position-256-1), (256-ZScrollBar->Position-1));
      glVertex3f((-256.0f), (YScrollBar->Position-256-1), (256-ZScrollBar->Position+1));
      glVertex3f((256.0f ), (YScrollBar->Position-256-1), (256-ZScrollBar->Position+1));
      glVertex3f((-256.0f), (YScrollBar->Position-256+1), (256-ZScrollBar->Position+1));
      glVertex3f((256.0f ), (YScrollBar->Position-256+1), (256-ZScrollBar->Position+1));
      glVertex3f((-256.0f), (YScrollBar->Position-256+1), (256-ZScrollBar->Position-1));
      glVertex3f((256.0f ), (YScrollBar->Position-256+1), (256-ZScrollBar->Position-1));
      glEnd();

      glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textura1);
      glBegin(GL_QUADS);
	// XY Face
	glTexCoord2f(0.0f, 1.0f); glVertex3f((-256.0f), (-256.0f), (256.0f-ZScrollBar->Position));
	glTexCoord2f(1.0f, 1.0f); glVertex3f((-256.0f), ( 256.0f), (256.0f-ZScrollBar->Position));
	glTexCoord2f(1.0f, 0.0f); glVertex3f((256.0f ), ( 256.0f), (256.0f-ZScrollBar->Position));
	glTexCoord2f(0.0f, 0.0f); glVertex3f((256.0f ), (-256.0f), (256.0f-ZScrollBar->Position));
      glEnd();

      glBindTexture(GL_TEXTURE_2D, textura2);
      glBegin(GL_QUADS);
	// YZ Face
	glTexCoord2f(0.0f, 0.0f); glVertex3f((anchoPX/2-XScrollBar->Position),(-256.0f), (256.0f));
	glTexCoord2f(0.0f, 1.0f); glVertex3f((anchoPX/2-XScrollBar->Position),(256.0f ), (256.0f));
	glTexCoord2f(1.0f, 1.0f); glVertex3f((anchoPX/2-XScrollBar->Position),(256.0f ), (-256.0f));
	glTexCoord2f(1.0f, 0.0f); glVertex3f((anchoPX/2-XScrollBar->Position),(-256.0f), (-256.0f));
      glEnd();

       glBindTexture(GL_TEXTURE_2D, textura3);
      glBegin(GL_QUADS);
	// ZX Face
	glTexCoord2f(1.0f, 0.0f); glVertex3f((-256.0f),(YScrollBar->Position-256), (256.0f ));
	glTexCoord2f(0.0f, 0.0f); glVertex3f((256.0f ),(YScrollBar->Position-256), (256.0f ));
	glTexCoord2f(0.0f, 1.0f); glVertex3f((256.0f ),(YScrollBar->Position-256), (-256.0f));
	glTexCoord2f(1.0f, 1.0f); glVertex3f((-256.0f),(YScrollBar->Position-256), (-256.0f));
      glEnd();

      glDisable(GL_TEXTURE_2D);
}
//------------------------------------------------------------------------------

drawAllTextures(short int anchoPX, short int altoPX, short int cantImagenes, int textura, int posicion, int nlut)
{

      glBindTexture(GL_TEXTURE_2D, textura);
      //glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
     // glBlendFunc(GL_SRC_COLOR,GL_ONE_MINUS_SRC_COLOR);
      glBegin(GL_QUADS);
	// XY Face
	glTexCoord2f(0.0f, 1.0f); glVertex3f((-256.0f), (-256.0f), (256.0f-posicion));
	glTexCoord2f(1.0f, 1.0f); glVertex3f((-256.0f), ( 256.0f), (256.0f-posicion));
	glTexCoord2f(1.0f, 0.0f); glVertex3f((256.0f ), ( 256.0f), (256.0f-posicion));
	glTexCoord2f(0.0f, 0.0f); glVertex3f((256.0f ), (-256.0f), (256.0f-posicion));
      glEnd();

      glBindTexture(GL_TEXTURE_1D, nlut);
      //glBlendFunc(GL_SRC_COLOR,GL_ONE_MINUS_SRC_COLOR);
      glBegin(GL_QUADS);
	// XY Face
	glTexCoord2f(0.0f, 1.0f); glVertex3f((-256.0f), (-256.0f), (256.0f-posicion));
	glTexCoord2f(1.0f, 1.0f); glVertex3f((-256.0f), ( 256.0f), (256.0f-posicion));
	glTexCoord2f(1.0f, 0.0f); glVertex3f((256.0f ), ( 256.0f), (256.0f-posicion));
	glTexCoord2f(0.0f, 0.0f); glVertex3f((256.0f ), (-256.0f), (256.0f-posicion));
      glEnd();

}

//------------------------------------------------------------------------------
void drawPlanoOblic(PuntoF** PlanoCorte,short int anchoPX, short int altoPX,
                    int texturaOB)
{
          //Dibuja Marco:
          glLineWidth(5.0f);
          glColor3f(1.0,0.0,1.0);
          glBegin(GL_LINE_STRIP);
          glVertex3f(256-PlanoCorte[0][0].x,PlanoCorte[0][0].y-256,256-PlanoCorte[0][0].z);
          glVertex3f(256-PlanoCorte[0][altoPX-1].x,PlanoCorte[0][altoPX-1].y-256,
                     256-PlanoCorte[0][altoPX-1].z);
          glVertex3f(256-PlanoCorte[anchoPX-1][altoPX-1].x,PlanoCorte[anchoPX-1][altoPX-1].y-256,
                     256-PlanoCorte[anchoPX-1][altoPX-1].z);
          glVertex3f(256-PlanoCorte[anchoPX-1][0].x,PlanoCorte[anchoPX-1][0].y-256,
                     256-PlanoCorte[anchoPX-1][0].z);
          glVertex3f(256-PlanoCorte[0][0].x,PlanoCorte[0][0].y-256,256-PlanoCorte[0][0].z);
          glEnd();

      /*//DibujarSeparadores:
       glLineWidth(1.0f);
      glColor4f(1.0, 1.0, 1.0, 0.5);
      glBegin(GL_LINES);
      glVertex3f((anchoPX/2-XScrollBar->Position-1), (-256.0f),(256.0f-ZScrollBar->Position-1));
      glVertex3f((anchoPX/2-XScrollBar->Position-1), (256.0f), (256.0f-ZScrollBar->Position-1));
      glVertex3f((anchoPX/2-XScrollBar->Position-1), (-256.0f),(256.0f-ZScrollBar->Position+1));
      glVertex3f((anchoPX/2-XScrollBar->Position-1), (256.0f), (256.0f-ZScrollBar->Position+1));
      glVertex3f((anchoPX/2-XScrollBar->Position+1), (-256.0f),(256.0f-ZScrollBar->Position-1));
      glVertex3f((anchoPX/2-XScrollBar->Position+1), (256.0f), (256.0f-ZScrollBar->Position-1));
      glVertex3f((anchoPX/2-XScrollBar->Position+1), (-256.0f),(256.0f-ZScrollBar->Position+1));
      glVertex3f((anchoPX/2-XScrollBar->Position+1), (256.0f), (256.0f-ZScrollBar->Position+1));
      glEnd(); */

      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, texturaOB);
      glBegin(GL_QUADS);

        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(256.0f-PlanoCorte[0][0].x,
                   PlanoCorte[0][0].y-256.0f,
                   256.0f-PlanoCorte[0][0].z);

	glTexCoord2f(1.0f, 0.0f);
        glVertex3f(256.0f-PlanoCorte[0][altoPX-1].x,
                   PlanoCorte[0][altoPX-1].y-256.0f,
                   256.0f-PlanoCorte[0][altoPX-1].z);

	glTexCoord2f(1.0f, 1.0f);
        glVertex3f(256.0f-PlanoCorte[anchoPX-1][altoPX-1].x,
                   PlanoCorte[anchoPX-1][altoPX-1].y-256.0f,
                   256.0f-PlanoCorte[anchoPX-1][altoPX-1].z);

	glTexCoord2f(0.0f, 1.0f);
        glVertex3f(256.0f-PlanoCorte[anchoPX-1][0].x,
                   PlanoCorte[anchoPX-1][0].y-256.0f,
                   256.0f-PlanoCorte[anchoPX-1][0].z);

      glEnd();

      glDisable(GL_TEXTURE_2D);
}
//------------------------------------------------------------------------------
void drawPuntos(TList* RenderPoints,
                float* LUT, float* LH, float* LM, float *LS,
                short int medX, short int medY, short int medZ,
                short int minPX,
                float tamanioPixel, int k0, int kf, int paso, float umbralGrad)
{
      //Variables Auxiliares:
      short int valorHU;
      float valorLUT;

      float vh, vm, vs, vc;
      float nrm;
      float cr, cg, cb;

      short int x,y,z;
      
      RenderPunto* ap;

        //Lighting Functions:******************************
        GLfloat light_position[] = {100.0, 100.0, 100.0, 0.0};
        GLfloat light_position1[]= {-100.0, -100.0, -100.0, 0.0};

         GLfloat light_central[] = {0.0, 0.0, 100.0, 0.0};

        GLfloat white_light[] = {1.0, 1.0, 1.0, 0.0};
        GLfloat lmodel_ambient[] = {0.3, 0.3, 0.3, 0.0};

        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
        glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);

        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

        glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, white_light);
        glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);

        glLightfv(GL_LIGHT2, GL_POSITION, light_central);
        glLightfv(GL_LIGHT2, GL_DIFFUSE, white_light);

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        glEnable(GL_LIGHT2);
        //************************************************

        //Material Functions******************************
        GLfloat mat_h[]={0.35, 0.35, 0.22, 1.0};
        GLfloat mat_m[]={0.3 , 0.01, 0.01, 0.8};
        GLfloat mat_s[]={0.8 , 0.1 , 0.0 , 0.7};

        glColorMaterial(GL_FRONT, GL_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        //************************************************

         //Rendering Funcions*****************************
         glPointSize(tamanioPixel);

         glShadeModel(GL_SMOOTH);
         glEnable(GL_CULL_FACE);
         //***********************************************

          //Habilita Depth Test (z-buffer)*****************
         glEnable(GL_DEPTH_TEST);
         glEnable(GL_BLEND);
         glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
         //***********************************************
        
       //  glEnable(GL_NORMALIZE);

      //Dibuja Puntos Opacos:-----------------------------------------
      glBegin(GL_POINTS);
      for(int k=k0;k<kf;k+=paso)
      {
        if(ap = (RenderPunto*)RenderPoints->Items[k])
        {

         valorHU = ap->p.color;
         valorLUT=float(LUT[valorHU-minPX-1])/255.0;

          if(valorLUT > 0)
          {
          x=ap->p.x; y=ap->p.y; z=ap->p.z;
          
            vh = LH[valorHU-minPX-1]/255.0;
            vm = LM[valorHU-minPX-1]/255.0;
            vs = LS[valorHU-minPX-1]/255.0;

           // glColor4f(mat_hueso[0],mat_hueso[1],mat_hueso[2],valorLUT);
            cr = mat_h[0]*vh + mat_m[0]*vm, + mat_s[0]*vs;
            cg = mat_h[1]*vh + mat_m[1]*vm, + mat_s[1]*vs;
            cb = mat_h[2]*vh + mat_m[2]*vm, + mat_s[2]*vs;

          // Colores normalizados para mantener la intensidad:
          // nrm = sqrt(cr*cr+cg*cg+cb*cb);
          // cr = cr/nrm; cg = cg/nrm; cb = cb/nrm;

          // if(ap->norma - umbralGrad)
          // glColor4f(cr,cg,cb, valorLUT + 0.6);
          // else

           glColor4f(cr,cg,cb, valorLUT);
           glNormal3f(ap->n.x, ap->n.y, ap->n.z);
           glVertex3f(x-medX,y-medY,-z+medZ);
          }
         // else break;

        }
      }
      glEnd();
      //---------------------------------------------------------------

       glDisable(GL_BLEND);

      //Deshabilita Material:************
       glDisable(GL_COLOR_MATERIAL);
      //Deshabilita Luz:*****************
        glDisable(GL_LIGHT1);
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHT2);
        glDisable(GL_LIGHTING);

      //Deshabilita Depth Test(z-buffer)**
      glDisable(GL_DEPTH_TEST);
      //**********************************

      //Deshabilita Rendering Funcs*****
      glDisable(GL_CULL_FACE);
      glPointSize(1.0);
      //********************************

}
//------------------------------------------------------------------------------
void drawPuntosGrayScale(TList* RenderPoints,
                float* LUT, float* LH, float* LM, float *LS,
                short int medX, short int medY, short int medZ,
                short int minPX,
                float tamanioPixel, int k0, int kf, int paso, float umbralGrad)
{
      //Variables Auxiliares:
      short int valorHU;
      float valorLUT;

      float vh, vm, vs, vc;
      float nrm;
      float cr, cg, cb;

      short int x,y,z;

      RenderPunto* ap;

        //Lighting Functions:******************************
        GLfloat light_position[] = {100.0, 100.0, 100.0, 0.0};
        GLfloat light_position1[]= {-100.0, -100.0, -100.0, 0.0};

         GLfloat light_central[] = {0.0, 0.0, 100.0, 0.0};

        GLfloat white_light[] = {1.0, 1.0, 1.0, 0.0};
        GLfloat lmodel_ambient[] = {0.3, 0.3, 0.3, 0.0};

        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
        glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);

        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

        glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, white_light);
        glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);

        glLightfv(GL_LIGHT2, GL_POSITION, light_central);
        glLightfv(GL_LIGHT2, GL_DIFFUSE, white_light);

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        glEnable(GL_LIGHT2);
        //************************************************

        //Material Functions******************************
        GLfloat mat_h[]={0.5, 0.5, 0.5, 1.0};
        GLfloat mat_m[]={0.5 , 0.5, 0.5, 0.8};
        GLfloat mat_s[]={0.5 , 0.5 , 0.5 , 0.7};

        glColorMaterial(GL_FRONT, GL_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        //************************************************

         //Rendering Funcions*****************************
         glPointSize(tamanioPixel);

         glShadeModel(GL_SMOOTH);
         glEnable(GL_CULL_FACE);
         //***********************************************

          //Habilita Depth Test (z-buffer)*****************
         glEnable(GL_DEPTH_TEST);
         glEnable(GL_BLEND);
         glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
         //***********************************************

       //  glEnable(GL_NORMALIZE);

      //Dibuja Puntos Opacos:-----------------------------------------
      glBegin(GL_POINTS);
      for(int k=k0;k<kf;k+=paso)
      {
        if(ap = (RenderPunto*)RenderPoints->Items[k])
        {

         valorHU = ap->p.color;
         valorLUT=float(LUT[valorHU-minPX-1])/255.0;

          if(valorLUT > 0)
          {
          x=ap->p.x; y=ap->p.y; z=ap->p.z;

            vh = valorLUT;//LH[valorHU-minPX-1]/255.0;
            vm = valorLUT;//LM[valorHU-minPX-1]/255.0;
            vs = valorLUT;//LS[valorHU-minPX-1]/255.0;

           // glColor4f(mat_hueso[0],mat_hueso[1],mat_hueso[2],valorLUT);
            cr = mat_h[0]*vh + mat_m[0]*vm, + mat_s[0]*vs;
            cg = mat_h[1]*vh + mat_m[1]*vm, + mat_s[1]*vs;
            cb = mat_h[2]*vh + mat_m[2]*vm, + mat_s[2]*vs;

          // Colores normalizados para mantener la intensidad:
          // nrm = sqrt(cr*cr+cg*cg+cb*cb);
          // cr = cr/nrm; cg = cg/nrm; cb = cb/nrm;

          // if(ap->norma - umbralGrad)
          // glColor4f(cr,cg,cb, valorLUT + 0.6);
          // else

           glColor4f(cr,cg,cb, valorLUT);
           glNormal3f(ap->n.x, ap->n.y, ap->n.z);
           glVertex3f(x-medX,y-medY,-z+medZ);
          }
         // else break;

        }
      }
      glEnd();
      //---------------------------------------------------------------

       glDisable(GL_BLEND);

      //Deshabilita Material:************
       glDisable(GL_COLOR_MATERIAL);
      //Deshabilita Luz:*****************
        glDisable(GL_LIGHT1);
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHT2);
        glDisable(GL_LIGHTING);

      //Deshabilita Depth Test(z-buffer)**
      glDisable(GL_DEPTH_TEST);
      //**********************************

      //Deshabilita Rendering Funcs*****
      glDisable(GL_CULL_FACE);
      glPointSize(1.0);
      //********************************

}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void drawPuntosColor(TList* RenderPoints,
                float* LUT, float* LH, float* LM, float *LS,
                short int medX, short int medY, short int medZ,
                short int minPX,
                float tamanioPixel, int k0, int kf, int paso, float umbralGrad)
{


      //Variables Auxiliares:
      short int valorHU;
      float valorLUT;

      float vh, vm, vs, vc;
      float nrm;
      float cr, cg, cb;

      short int x,y,z;
      
      RenderPunto* ap;

        //Lighting Functions:******************************
        GLfloat light_position[] = {100.0, 100.0, 100.0, 0.0};
        GLfloat light_position1[]= {-100.0, -100.0, -100.0, 0.0};
         GLfloat light_central[] = {0.0, 0.0, 100.0, 0.0};

        GLfloat white_light[] = {1.0, 0.0, 1.0, 0.0};
        GLfloat lmodel_ambient[] = {0.3, 0.3, 0.3, 0.0};

        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
        glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);

        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

        glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, white_light);
        glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);

        glLightfv(GL_LIGHT2, GL_POSITION, light_central);
        glLightfv(GL_LIGHT2, GL_DIFFUSE, white_light);

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        glEnable(GL_LIGHT2);
        //************************************************

        //Material Functions******************************
        GLfloat mat_h[]={0.45, 0.15, 0.32, 1.0};
        GLfloat mat_m[]={0.4 , 0.01, 0.1, 0.8};
        GLfloat mat_s[]={0.5 , 0.1 , 0.5 , 0.7};

        glColorMaterial(GL_FRONT, GL_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        //************************************************

         //Rendering Funcions*****************************
         glPointSize(tamanioPixel);

         glShadeModel(GL_SMOOTH);
         glEnable(GL_CULL_FACE);
         //***********************************************

         //Habilita Depth Test (z-buffer)*****************
         glEnable(GL_DEPTH_TEST);
         //***********************************************

         glEnable(GL_BLEND);
         glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

       //  glEnable(GL_NORMALIZE);

      //Dibuja Puntos Opacos:-----------------------------------------
      glBegin(GL_POINTS);
      for(int k=k0;k<kf;k+=paso)
      {
        if(ap = (RenderPunto*)RenderPoints->Items[k])
        {

         valorHU = ap->p.color;
         valorLUT=float(LUT[valorHU-minPX-1])/255.0;

          if(valorLUT > 0)
          {
          x=ap->p.x; y=ap->p.y; z=ap->p.z;
          
            vh = LH[valorHU-minPX-1]/255.0;
            vm = LM[valorHU-minPX-1]/255.0;
            vs = LS[valorHU-minPX-1]/255.0;

           // glColor4f(mat_hueso[0],mat_hueso[1],mat_hueso[2],valorLUT);
            cr = mat_h[0]*vh + mat_m[0]*vm, + mat_s[0]*vs;
            cg = mat_h[1]*vh + mat_m[1]*vm, + mat_s[1]*vs;
            cb = mat_h[2]*vh + mat_m[2]*vm, + mat_s[2]*vs;

          // Colores normalizados para mantener la intensidad:
          // nrm = sqrt(cr*cr+cg*cg+cb*cb);
          // cr = cr/nrm; cg = cg/nrm; cb = cb/nrm;

          // if(ap->norma - umbralGrad)
          // glColor4f(cr,cg,cb, valorLUT + 0.6);
          // else

           glColor4f(cr,cg,cb, valorLUT);
           glNormal3f(ap->n.x, ap->n.y, ap->n.z);
           glVertex3f(x-medX,y-medY,-z+medZ);
          }
         // else break;

        }
      }
      glEnd();
      //---------------------------------------------------------------

      glDisable(GL_BLEND);

      //Deshabilita Material:************
       glDisable(GL_COLOR_MATERIAL);
      //Deshabilita Luz:*****************
        glDisable(GL_LIGHT1);
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHT2);
        glDisable(GL_LIGHTING);

      //Deshabilita Depth Test(z-buffer)**
      glDisable(GL_DEPTH_TEST);
      //**********************************

      //Deshabilita Rendering Funcs*****
      glDisable(GL_CULL_FACE);
      glPointSize(1.0);
      //********************************

}
//------------------------------------------------------------------------------
void drawBordes(TList* BL,
                short int medX, short int medY, short int medZ,
                short int minPX,
                float tamanioPixel, float umbralGrad)
{
      short int x,y,z;
      
      RenderPunto* ap;

        //Lighting Functions:******************************
        GLfloat light_position[] = {100.0, 100.0, 100.0, 0.0};
        GLfloat light_position1[]= {-100.0, -100.0, -100.0, 0.0};
         GLfloat light_central[] = {0.0, 0.0, 100.0, 0.0};

        GLfloat white_light[] = {1.0, 1.0, 1.0, 0.0};
        GLfloat lmodel_ambient[] = {0.6, 0.6, 0.6, 0.0};

        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
        glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);

        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

        glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, white_light);
        glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);

        glLightfv(GL_LIGHT2, GL_POSITION, light_central);
        glLightfv(GL_LIGHT2, GL_DIFFUSE, white_light);

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        glEnable(GL_LIGHT2);
        //************************************************

        glColorMaterial(GL_FRONT, GL_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);

        GLfloat mat[]={0.6, 0.57, 0.54, umbralGrad};
        //************************************************

         //Rendering Funcions*****************************
         glPointSize(tamanioPixel);
         glShadeModel(GL_SMOOTH);
         glEnable(GL_CULL_FACE);
         //***********************************************

         //Habilita Depth Test (z-buffer)*****************
         glEnable(GL_DEPTH_TEST);
         //***********************************************

         glEnable(GL_BLEND);
         glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

      //Dibuja Puntos Opacos:-----------------------------------------
      glBegin(GL_POINTS);
      for(int k=0;k<BL->Count;k++)
      {
        if(ap = (RenderPunto*)BL->Items[k])
        {

          x=ap->p.x; y=ap->p.y; z=ap->p.z;

          glColor4f(mat[0],mat[1],mat[2],mat[3]);
          glNormal3f(ap->n.x, ap->n.y, ap->n.z);
          glVertex3f(x-medX,y-medY,-z+medZ);
        }
      }
      glEnd();
      //---------------------------------------------------------------

       glDisable(GL_BLEND);

      //Deshabilita Material:************
       glDisable(GL_COLOR_MATERIAL);
      //Deshabilita Luz:*****************
        glDisable(GL_LIGHT1);
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHT2);
        glDisable(GL_LIGHTING);

      //Deshabilita Depth Test(z-buffer)**
      glDisable(GL_DEPTH_TEST);
      //**********************************

      //Deshabilita Rendering Funcs*****
      glDisable(GL_CULL_FACE);
      glPointSize(1.0);
      //********************************

}
//------------------------------------------------------------------------------
void drawMC(TList* TL, Volumen* M, float* LUT, 
                short int medX, short int medY, short int medZ,
                short int minPX, bool dir)
{
      PuntoF* ap1;
      PuntoF* ap2;
      PuntoF* ap3;

      short int valor1, valor2, valor3;

      short int xi, xf, yi, yf, zi, zf;
      float ax,ay,az;
      float bx,by,bz;

      float op;

      glBegin(GL_TRIANGLES);

      if(dir)
      for(int k=0;k<TL->Count;k=k+3)
      {
        ap1 = (PuntoF*)TL->Items[k];
        ap2 = (PuntoF*)TL->Items[k+1];
        ap3 = (PuntoF*)TL->Items[k+2];

        xi = floor(ap1->x); xf = xi+1;
        yi = floor(ap1->y); yf = yi+1;
        zi = floor(ap1->z); zf = zi+1;

        ax = ap1->x-xi; bx = xf-ap1->x;
        ay = ap1->y-yi; by = yf-ap1->y;
        az = ap1->z-zi; bz = zf-ap1->z;

        valor1 = bx*by*bz*M->GetV(xi,yi,zi) +
                 bx*ay*bz*M->GetV(xi,yf,zi) +
                 ax*by*bz*M->GetV(xf,yi,zi) +
                 ax*ay*bz*M->GetV(xf,yf,zi) +
                 bx*by*az*M->GetV(xi,yi,zi) +
                 bx*ay*az*M->GetV(xi,yf,zi) +
                 ax*by*az*M->GetV(xf,yi,zi) +
                 ax*ay*az*M->GetV(xf,yf,zi);

        
        xi = floor(ap2->x); xf = xi+1;
        yi = floor(ap2->y); yf = yi+1;
        zi = floor(ap2->z); zf = zi+1;

        ax = ap2->x-xi; bx = xf-ap2->x;
        ay = ap2->y-yi; by = yf-ap2->y;
        az = ap2->z-zi; bz = zf-ap2->z;

        valor2 = bx*by*bz*M->GetV(xi,yi,zi) +
                 bx*ay*bz*M->GetV(xi,yf,zi) +
                 ax*by*bz*M->GetV(xf,yi,zi) +
                 ax*ay*bz*M->GetV(xf,yf,zi) +
                 bx*by*az*M->GetV(xi,yi,zi) +
                 bx*ay*az*M->GetV(xi,yf,zi) +
                 ax*by*az*M->GetV(xf,yi,zi) +
                 ax*ay*az*M->GetV(xf,yf,zi);

        xi = floor(ap3->x); xf = xi+1;
        yi = floor(ap3->y); yf = yi+1;
        zi = floor(ap3->z); zf = zi+1;

        ax = ap3->x-xi; bx = xf-ap3->x;
        ay = ap3->y-yi; by = yf-ap3->y;
        az = ap3->z-zi; bz = zf-ap3->z;

        valor3 = bx*by*bz*M->GetV(xi,yi,zi) +
                 bx*ay*bz*M->GetV(xi,yf,zi) +
                 ax*by*bz*M->GetV(xf,yi,zi) +
                 ax*ay*bz*M->GetV(xf,yf,zi) +
                 bx*by*az*M->GetV(xi,yi,zi) +
                 bx*ay*az*M->GetV(xi,yf,zi) +
                 ax*by*az*M->GetV(xf,yi,zi) +
                 ax*ay*az*M->GetV(xf,yf,zi);

        valor1=LUT[(valor1+valor2+valor3)/3-minPX-1];
        op = float(valor1)/255.0;

        glColor4f(valor1/255.0,valor1/255.0,valor1/255.0, op);

        glVertex3f(ap1->x-medX,ap1->y-medY,-ap1->z+medZ);
        glVertex3f(ap2->x-medX,ap2->y-medY,-ap2->z+medZ);
        glVertex3f(ap3->x-medX,ap3->y-medY,-ap3->z+medZ);
      }
      else
      for(int k=TL->Count-1;k>0;k=k-3)
      {
        ap1 = (PuntoF*)TL->Items[k];
        ap2 = (PuntoF*)TL->Items[k-1];
        ap3 = (PuntoF*)TL->Items[k-2];

        valor1 = M->GetV(ap1->x,ap1->y,ap1->z);
        valor2 = M->GetV(ap2->x,ap2->y,ap2->z);
        valor3 = M->GetV(ap3->x,ap3->y,ap3->z);

        valor1=LUT[(valor1+valor2+valor3)/3-minPX-1];

        glColor4f(valor1/255.0,valor1/255.0,valor1/255.0, 0.7);

        glVertex3f(ap1->x-medX,ap1->y-medY,-ap1->z+medZ);
        glVertex3f(ap2->x-medX,ap2->y-medY,-ap2->z+medZ);
        glVertex3f(ap3->x-medX,ap3->y-medY,-ap3->z+medZ);
      }

      glEnd();

}
//------------------------------------------------------------------------------
void drawMC(TList* TL, TList* VL, TList* TC, TList* Normals, Volumen* M,  float* LUT,
                short int medX, short int medY, short int medZ,
                short int minPX, bool dir)
{
        GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
        GLfloat mat_shininess[] = {10.0};
        GLfloat light_position[] = {500.0, 500.0, 500.0, 0.0};
        GLfloat white_light[] = {1.0, 1.0, 1.0, 1.0};
        GLfloat lmodel_ambient[] = {0.7, 0.7, 0.7, 1.0};
        glShadeModel(GL_SMOOTH);

        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
        
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
        glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);

        glPointSize(10.0);
        glBegin(GL_POINTS);
        glColor3f(1.0, 1.0, 0.0);
        glVertex3f(light_position[0],light_position[1],light_position[2]);
        glEnd();
        glPointSize(1.0);

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

      //glEnable(GL_BLEND);
      //glBlendFunc(GL_DST_ALPHA,GL_SRC_ALPHA);

//Triangulos:*******************************************************************
      PuntoF* ap1;
      PuntoF* ap2;
      PuntoF* ap3;


      PuntoF* n;

      float *col1, *col2, *col3;

      float val1, val2, val3;

    //  int colLim = TC->Count;

      int ni=0;
      int k;

      if(dir)
      {
      glFrontFace(GL_CW);
      glBegin(GL_TRIANGLES);
       for(k=0; k<TL->Count; k=k+3)
       {
        ap1 = (PuntoF*)TL->Items[k];
        ap2 = (PuntoF*)TL->Items[k+1];
        ap3 = (PuntoF*)TL->Items[k+2];

        col1 = (float*)TC->Items[k];
        col2 = (float*)TC->Items[k+1];
        col3 = (float*)TC->Items[k+2];

        n = (PuntoF*)Normals->Items[ni];
        ni++;

        glNormal3f(n->x, n->y, n->z);

        val1 = LUT[int(*col1)-minPX-1];
        val1 = float(val1)/255.0;
        glColor4f(val1,val1,val1, 0.0);
        glVertex3f(ap1->x-medX,ap1->y-medY,-ap1->z+medZ);

        val2 = LUT[int(*col2)-minPX-1];
        val2 = float(val2)/255.0;
        glColor4f(val2,val2,val2, 0.0);
        glVertex3f(ap2->x-medX,ap2->y-medY,-ap2->z+medZ);

        val3 = LUT[int(*col3)-minPX-1];
        val3 = float(val3)/255.0;
        glColor4f(val3,val3,val3, 0.0);
        glVertex3f(ap3->x-medX,ap3->y-medY,-ap3->z+medZ);
       }
      glEnd();

      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glFrontFace(GL_CCW);

      ni=0;

      glBegin(GL_TRIANGLES);
      for(k=0; k<TL->Count; k=k+3)
       {
        ap1 = (PuntoF*)TL->Items[k];
        ap2 = (PuntoF*)TL->Items[k+1];
        ap3 = (PuntoF*)TL->Items[k+2];

        col1 = (float*)TC->Items[k];
        col2 = (float*)TC->Items[k+1];
        col3 = (float*)TC->Items[k+2];

        n = (PuntoF*)Normals->Items[ni];
        ni++;

        glNormal3f(n->x, n->y, n->z);

        val1 = LUT[int(*col1)-minPX-1];
        val1 = float(val1)/255.0;
        glColor4f(val1,0.0,0.0, 0.0);
        glVertex3f(ap1->x-medX,ap1->y-medY,-ap1->z+medZ);

        val2 = LUT[int(*col2)-minPX-1];
        val2 = float(val2)/255.0;
        glColor4f(val2,0.0,0.0, 0.0);
        glVertex3f(ap2->x-medX,ap2->y-medY,-ap2->z+medZ);

        val3 = LUT[int(*col3)-minPX-1];
        val3 = float(val3)/255.0;
        glColor4f(val3,0.0,0.0, 0.0);
        glVertex3f(ap3->x-medX,ap3->y-medY,-ap3->z+medZ);
       }
       glEnd();

      }
      else
      {
      glFrontFace(GL_CCW);
      glBegin(GL_TRIANGLES);
       for(k=TL->Count-1; k>0; k=k-3)
       {
        ap1 = (PuntoF*)TL->Items[k];
        ap2 = (PuntoF*)TL->Items[k-1];
        ap3 = (PuntoF*)TL->Items[k-2];

        col1 = (float*)TC->Items[k];
        col2 = (float*)TC->Items[k-1];
        col3 = (float*)TC->Items[k-2];

        n = (PuntoF*)Normals->Items[ni];
        ni++;

        glNormal3f(-n->x, -n->y, -n->z);

        val1 = LUT[int(*col1)-minPX-1];
        val1 = float(val1)/255.0;
        glColor4f(val1,val1,val1, 0.0);
        glVertex3f(ap1->x-medX,ap1->y-medY,-ap1->z+medZ);

        val2 = LUT[int(*col2)-minPX-1];
        val2 = float(val2)/255.0;
        glColor4f(val2,val2,val2, 0.0);
        glVertex3f(ap2->x-medX,ap2->y-medY,-ap2->z+medZ);

        val3 = LUT[int(*col3)-minPX-1];
        val3 = float(val3)/255.0;
        glColor4f(val3,val3,val3, 0.0);
        glVertex3f(ap3->x-medX,ap3->y-medY,-ap3->z+medZ);
       }
       glEnd();

       ni=0;

       glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
       glFrontFace(GL_CW);

       glBegin(GL_TRIANGLES);
       for(k=TL->Count-1; k>0; k=k-3)
       {
        ap1 = (PuntoF*)TL->Items[k];
        ap2 = (PuntoF*)TL->Items[k-1];
        ap3 = (PuntoF*)TL->Items[k-2];

        col1 = (float*)TC->Items[k];
        col2 = (float*)TC->Items[k-1];
        col3 = (float*)TC->Items[k-2];

        n = (PuntoF*)Normals->Items[ni];
        ni++;

        glNormal3f(-n->x, -n->y, -n->z);

        val1 = LUT[int(*col1)-minPX-1];
        val1 = float(val1)/255.0;
        glColor4f(val1,0.0,0.0, 0.0);
        glVertex3f(ap1->x-medX,ap1->y-medY,-ap1->z+medZ);

        val2 = LUT[int(*col2)-minPX-1];
        val2 = float(val2)/255.0;
        glColor4f(val2,0.0,0.0, 0.0);
        glVertex3f(ap2->x-medX,ap2->y-medY,-ap2->z+medZ);

        val3 = LUT[int(*col3)-minPX-1];
        val3 = float(val3)/255.0;
        glColor4f(val3,0.0,0.0, 0.0);
        glVertex3f(ap3->x-medX,ap3->y-medY,-ap3->z+medZ);
       }
       glEnd();
      }
//******************************************************************************


      glDisable(GL_CULL_FACE);
     // glDisable(GL_BLEND);

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHTING);

}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void drawMC(TList* TL, TList* VL, TList* NL, Volumen* M,  float* LUT,
        short int medX, short int medY, short int medZ,
        short int minPX, bool dir, int offset,
        float opacity)
{

      glEnable(GL_DEPTH_TEST);
      glEnable(GL_BLEND);
      glBlendFunc(GL_DST_ALPHA,GL_SRC_ALPHA);
      glShadeModel(GL_SMOOTH);

        GLfloat mat_specular[] = {0.001, 0.001, 0.001, 0.0};
        GLfloat  mat_diffuse[] = {0.65, 0.65, 0.2, 1.0};
        GLfloat mat_shininess[] = {10.0};

        GLfloat light_position[] = {100.0, 100.0, 100.0, 0.0};
        GLfloat light_position1[] = {-100.0, -100.0, -100.0, 0.0};

        GLfloat white_light[] = {1.0, 1.0, 1.0, 0.0};
        GLfloat lmodel_ambient[] = {0.3, 0.3, 0.3, 0.0};

        glShadeModel(GL_SMOOTH);

        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
        glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);

        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

        glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, white_light);
        glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);

        glEnable(GL_CULL_FACE);

        glPointSize(10.0);
        glBegin(GL_POINTS);
        glColor4f(1.0, 1.0, 0.0, opacity);
        
        glVertex3f(light_position[0],light_position[1],light_position[2]);
        glVertex3f(light_position1[0],light_position1[1],light_position1[2]);
        glEnd();
        glPointSize(1.0);

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
//Triangulos:*******************************************************************
      PuntoF* ap1;
      PuntoF* ap2;
      PuntoF* ap3;

      PuntoF* n1;
      PuntoF* n2;
      PuntoF* n3;

      PuntoI* tri;

      int k;

      if(dir)
      {
      glFrontFace(GL_CW);
      glBegin(GL_TRIANGLES);
       for(k=0; k<TL->Count; k=k+offset)
       {
       tri = (PuntoI*)TL->Items[k];

       ap1 = (PuntoF*)VL->Items[tri->xyz[0]];
       ap2 = (PuntoF*)VL->Items[tri->xyz[1]];
       ap3 = (PuntoF*)VL->Items[tri->xyz[2]];

       n1 = (PuntoF*)NL->Items[tri->xyz[0]];
       n2 = (PuntoF*)NL->Items[tri->xyz[1]];
       n3 = (PuntoF*)NL->Items[tri->xyz[2]];

        glNormal3f(n1->x,n1->y,n1->z);
        glVertex3f(ap1->x-medX,ap1->y-medY,-ap1->z+medZ);

        glNormal3f(n2->x,n2->y,n2->z);
        glVertex3f(ap2->x-medX,ap2->y-medY,-ap2->z+medZ);

        glNormal3f(n3->x,n3->y,n3->z);
        glVertex3f(ap3->x-medX,ap3->y-medY,-ap3->z+medZ);
       }
      glEnd();

      glFrontFace(GL_CCW);
      glBegin(GL_TRIANGLES);
       for(k=0; k<TL->Count; k=k+offset)
       {
       tri = (PuntoI*)TL->Items[k];

       ap1 = (PuntoF*)VL->Items[tri->xyz[0]];
       ap2 = (PuntoF*)VL->Items[tri->xyz[1]];
       ap3 = (PuntoF*)VL->Items[tri->xyz[2]];

       n1 = (PuntoF*)NL->Items[tri->xyz[0]];
       n2 = (PuntoF*)NL->Items[tri->xyz[1]];
       n3 = (PuntoF*)NL->Items[tri->xyz[2]];

        glNormal3f(n1->x,n1->y,n1->z);
        glVertex3f(ap1->x-medX,ap1->y-medY,-ap1->z+medZ);

        glNormal3f(n2->x,n2->y,n2->z);
        glVertex3f(ap2->x-medX,ap2->y-medY,-ap2->z+medZ);

        glNormal3f(n3->x,n3->y,n3->z);
        glVertex3f(ap3->x-medX,ap3->y-medY,-ap3->z+medZ);
       }
      glEnd();

      }
      else
      {
      glFrontFace(GL_CW);
      glBegin(GL_TRIANGLES);
       for(k=TL->Count-1; k>-1; k=k-offset)
       {
       tri = (PuntoI*)TL->Items[k];

       ap1 = (PuntoF*)VL->Items[tri->xyz[0]];
       ap2 = (PuntoF*)VL->Items[tri->xyz[1]];
       ap3 = (PuntoF*)VL->Items[tri->xyz[2]];

       n1 = (PuntoF*)NL->Items[tri->xyz[0]];
       n2 = (PuntoF*)NL->Items[tri->xyz[1]];
       n3 = (PuntoF*)NL->Items[tri->xyz[2]];

        glNormal3f(n1->x,n1->y,n1->z);
        glVertex3f(ap1->x-medX,ap1->y-medY,-ap1->z+medZ);

        glNormal3f(n2->x,n2->y,n2->z);
        glVertex3f(ap2->x-medX,ap2->y-medY,-ap2->z+medZ);

        glNormal3f(n3->x,n3->y,n3->z);
        glVertex3f(ap3->x-medX,ap3->y-medY,-ap3->z+medZ);
       }
      glEnd();

      glFrontFace(GL_CCW);
      glBegin(GL_TRIANGLES);
       for(k=TL->Count-1; k>-1; k=k-offset)
       {
       tri = (PuntoI*)TL->Items[k];

       ap1 = (PuntoF*)VL->Items[tri->xyz[0]];
       ap2 = (PuntoF*)VL->Items[tri->xyz[1]];
       ap3 = (PuntoF*)VL->Items[tri->xyz[2]];

       n1 = (PuntoF*)NL->Items[tri->xyz[0]];
       n2 = (PuntoF*)NL->Items[tri->xyz[1]];
       n3 = (PuntoF*)NL->Items[tri->xyz[2]];

        glNormal3f(n1->x,n1->y,n1->z);
        glVertex3f(ap1->x-medX,ap1->y-medY,-ap1->z+medZ);

        glNormal3f(n2->x,n2->y,n2->z);
        glVertex3f(ap2->x-medX,ap2->y-medY,-ap2->z+medZ);

        glNormal3f(n3->x,n3->y,n3->z);
        glVertex3f(ap3->x-medX,ap3->y-medY,-ap3->z+medZ);
       }
      glEnd();

      }
//******************************************************************************

       glDisable(GL_BLEND);
       glDisable(GL_DEPTH_TEST);
       glDisable(GL_CULL_FACE);
        glDisable(GL_LIGHT1);
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHTING);

}
//------------------------------------------------------------------------------
void drawCils(TList* CL, short int medX, short int medY, short int medZ)
{
            Cilindro* cil;

            //Dibuja Cilindros:
            for(int i=0; i<CL->Count; i++)
            {
            cil = (Cilindro*)CL->Items[i];

            double vx = cil->pf.x - cil->pi.x;
            double vy = cil->pf.y - cil->pi.y;
            double vz = cil->pf.z - cil->pi.z;
            double v = sqrt( vx*vx + vy*vy + vz*vz);  // cylinder length

            double ax = -(180.0/3.141632)*acos(vz/v); // rotation angle
            if (vz <= 0.0) ax = -ax;

            double rx = -vy*vz;
            double ry = +vx*vz;

               glPushMatrix();

               glLineWidth(0.5f);

               GLUquadricObj* cyl;
               cyl = gluNewQuadric();

               glTranslatef(GLdouble(cil->pf.x-medX),
                            GLdouble(cil->pf.y-medY),
                            GLdouble(-cil->pf.z+medZ));
                 // translate to point 1
               glRotatef(ax, rx, ry, 0.0);           // Rotate about rotation vector

               glColor4f(0.5, 0.5, 0.6, 0.9);
               gluQuadricDrawStyle(cyl, GLU_FILL);
               gluCylinder(cyl, cil->radio, cil->radio, v, 2*cil->radio, 0.5*v); // draw side faces of cylinder

               glColor4f(1.0, 1.0, 1.0, 1.0);
               gluQuadricDrawStyle(cyl, GLU_LINE);
               gluCylinder(cyl, cil->radio, cil->radio, v, 0.5*cil->radio, 0.5*v); // draw side faces of cylinder
               glPopMatrix();
            }


            glLineWidth(3.0f);
            glColor4f(0.0, 1.0, 0.0, 0.8);

            //Dibuja Centerlines:
            glBegin(GL_LINES);
            for(int i=0; i<CL->Count; i++)
            {
            cil = (Cilindro*)CL->Items[i];
            glVertex3f(cil->pi.x-medX,cil->pi.y-medY,-cil->pi.z +medZ);
            glVertex3f(cil->pf.x-medX,cil->pf.y-medY,-cil->pf.z +medZ);
            }
            glEnd();

            glLineWidth(1.0f);
}
//------------------------------------------------------------------------------
void drawEsfs(TList* EL, short int medX, short int medY, short int medZ)
{
            Esfera* esf;

            //Dibuja Cilindros:
            for(int i=0; i<EL->Count; i++)
            {
            esf = (Esfera*)EL->Items[i];

            double cx = esf->centro.x;
            double cy = esf->centro.y;
            double cz = esf->centro.z;
            double r = esf->radio;

               glPushMatrix();

               GLUquadricObj* esfgl;
               esfgl = gluNewQuadric();

               glTranslatef(GLdouble(cx-medX),
                            GLdouble(cy-medY),
                            GLdouble(-cz+medZ));
                 // translate to point 1

               glColor4f(0.5, 0.5, 0.6, 0.9);
               gluQuadricDrawStyle(esfgl, GLU_FILL);
               gluSphere(esfgl, r, 2*r, 0.5*r); // draw side faces of sphere

               glColor4f(1.0, 1.0, 1.0, 1.0);
               gluQuadricDrawStyle(esfgl, GLU_LINE);
               gluSphere(esfgl, r, 2*r, 0.5*r); // draw side faces of sphere
               glPopMatrix();
            }

            glPointSize(10.0f);
            glColor4f(1.0, 0.0, 0.0, 1.0);

            //Dibuja Centro:
            glBegin(GL_POINTS);
            for(int i=0; i<EL->Count; i++)
            {
            esf = (Esfera*)EL->Items[i];
            glVertex3f(esf->centro.x-medX,esf->centro.y-medY,-esf->centro.z +medZ);
            }
            glEnd();

            glPointSize(1.0f);
}
//------------------------------------------------------------------------------
void drawVolRama(TList* VolRamaList,
                float* LUT, float* LH, float* LM, float *LS,
                short int medX, short int medY, short int medZ,
                short int minPX,
                float tamanioPixel, int k0, int kf, int paso, float umbralGrad, double Mr, double mr,
                double* lr, double* lsr, double* vr)
{
//lr & vr: arrays con los largos y volumenes de las ramas; lsr: idem, pero con rama suavizada
//Variables Auxiliares:
      short int valorHU;
      float valorLUT;

      float vh, vm, vs, vc;
      float nrm;
      float cr, cg, cb;

      short int x,y,z;

      RenderPunto* ap;

        //Lighting Functions:******************************
        GLfloat light_position[] = {100.0, 100.0, 100.0, 0.0};
        GLfloat light_position1[]= {-100.0, -100.0, -100.0, 0.0};

         GLfloat light_central[] = {0.0, 0.0, 100.0, 0.0};

        GLfloat white_light[] = {1.0, 1.0, 1.0, 0.0};
        GLfloat lmodel_ambient[] = {0.3, 0.3, 0.3, 0.0};

  /*      glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
        glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);

        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

        glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, white_light);
        glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);

        glLightfv(GL_LIGHT2, GL_POSITION, light_central);
        glLightfv(GL_LIGHT2, GL_DIFFUSE, white_light);

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        glEnable(GL_LIGHT2);      */
        //************************************************

        //Material Functions******************************
      /*  GLfloat mat_h[]={0.5, 0.5, 0.5, 1.0};
        GLfloat mat_m[]={0.5 , 0.5, 0.5, 0.8};
        GLfloat mat_s[]={0.5 , 0.5 , 0.5 , 0.7}; */

    /*    glColorMaterial(GL_FRONT, GL_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);   */
        //************************************************

         //Rendering Funcions*****************************
         glPointSize(tamanioPixel);

         glShadeModel(GL_SMOOTH);
         glEnable(GL_CULL_FACE);
         //***********************************************

          //Habilita Depth Test (z-buffer)*****************
         glEnable(GL_DEPTH_TEST);
     //    glEnable(GL_BLEND);
     //    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
         //***********************************************

         glEnable(GL_NORMALIZE);

       TList* AuxList;

       float largo;
       float volumen;
       float largo_s;

      //Dibuja Puntos Opacos:-----------------------------------------
      glBegin(GL_POINTS);

     for(int j=0; j<VolRamaList->Count; j++)
     {
     AuxList = (TList*)VolRamaList->Items[j];

     ap = (RenderPunto*)AuxList->Items[AuxList->Count/2];

     largo   = lr[j];
     largo_s = lsr[j];
     volumen = vr[j];

     glColor3ub(255, 255, 255);
     glRasterPos3f(ap->p.x-medX,ap->p.y-medY ,medZ-ap->p.z);
     glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, largo);

     glRasterPos3f(ap->p.x-medX+5,ap->p.y-medY+5 ,medZ-ap->p.z+5);
     glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, largo_s);

     glRasterPos3f(ap->p.x-medX+10,ap->p.y-medY+10 ,medZ-ap->p.z+10);
     glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, volumen);

     for(int k=0; k<AuxList->Count; k++)
      if(ap = (RenderPunto*)AuxList->Items[k])
      {
         valorHU = ap->p.color;
         valorLUT=float(LUT[valorHU-minPX-1])/255.0;

          if(valorLUT > 0)
          {
          x=ap->p.x; y=ap->p.y; z=ap->p.z;

            vh = valorLUT;//LH[valorHU-minPX-1]/255.0;
            vm = valorLUT;//LM[valorHU-minPX-1]/255.0;
            vs = valorLUT;//LS[valorHU-minPX-1]/255.0;

           // glColor4f(mat_hueso[0],mat_hueso[1],mat_hueso[2],valorLUT);
           /* cr = mat_h[0]*vh + mat_m[0]*vm, + mat_s[0]*vs;
            cg = mat_h[1]*vh + mat_m[1]*vm, + mat_s[1]*vs;
            cb = mat_h[2]*vh + mat_m[2]*vm, + mat_s[2]*vs; */

            cr = double(AuxList->Count)/Mr;
            cg = 1.0-double(AuxList->Count)/Mr;
            cb = 1.0-fabs((double(AuxList->Count)-Mr/2.0)/Mr*2);

          // Colores normalizados para mantener la intensidad:
          // nrm = sqrt(cr*cr+cg*cg+cb*cb);
          // cr = cr/nrm; cg = cg/nrm; cb = cb/nrm;

          // if(ap->norma - umbralGrad)
          // glColor4f(cr,cg,cb, valorLUT + 0.6);
          // else

           glColor4f(cr,cg,cb, valorLUT);
           glNormal3f(ap->n.x, ap->n.y, ap->n.z);
           glVertex3f(x-medX,y-medY,-z+medZ);
          }
         // else break;

        }
      }
      glEnd();
      //---------------------------------------------------------------

    //   glDisable(GL_BLEND);

      //Deshabilita Material:************
    //   glDisable(GL_COLOR_MATERIAL);
      //Deshabilita Luz:*****************
     /*   glDisable(GL_LIGHT1);
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHT2);
        glDisable(GL_LIGHTING); */

      //Deshabilita Depth Test(z-buffer)**
      glDisable(GL_DEPTH_TEST);
      //**********************************

      //Deshabilita Rendering Funcs*****
      glDisable(GL_CULL_FACE);
      glPointSize(1.0);
      //********************************

}
//------------------------------------------------------------------------------

void drawAortaCenterLine(TList* RenderPoints,
                float dx, float dy, float dz)
{
    //Variables Auxiliares:
    PuntoF* paux;

    glPointSize(5.0);
    glColor4f(0.0, 1.0, 0.0, 1.0);

    glBegin(GL_POINTS);
    {
      for(int i=0; i<RenderPoints->Count-1; ++i)
      {
      paux = (PuntoF*)RenderPoints->Items[i];
      glVertex3f(paux->y, paux->x, paux->z);
      }
    }
    glEnd();

}
//------------------------------------------------------------------------------

void drawAortaRadius(TList* CenterLine, TList* Normales, TList* Radios,
                float dx, float dy, float dz)
{
    //Variables Auxiliares:
    PuntoF* pauxC;
    PuntoF* pauxN;
    float* r;

    float xC, yC, zC, xN, yN, zN;

    float x,y,z;

    float a1,a2,a3;
    float b1,b2,b3;

    glPointSize(2.0);
    glColor4f(0.0, 0.5, 0.0, 0.5);


    for(int i=0; i<CenterLine->Count; i++)
    {
    pauxC = (PuntoF*)CenterLine->Items[i];
    pauxN = (PuntoF*)Normales->Items[i];
    r = (float*)Radios->Items[i];

    xC = pauxC->x;
    yC = pauxC->y;
    zC = pauxC->z;

    xN = pauxN->x;
    yN = pauxN->y;
    zN = pauxN->z;

    if(xN!=0)
    {
    a2 = 1.0;
    a3 = 1.0;
    a1 = (-yN*a2-zN*a3)/xN;
    }
    else if(yN!=0)
    {
    a1 = 1.0;
    a3 = 1.0;
    a2 = (-xN*a1-zN*a3)/yN;
    }
    else
    {
    a1 = 1.0;
    a2 = 1.0;
    a3 = (-xN*a1-yN*a2)/zN;
    }

    float norm_a = sqrt(a1*a1 + a2*a2 + a3*a3);
    a1/=norm_a; a2/=norm_a; a3/=norm_a;

    b1 = a2*zN-a3*yN;
    b2 = a3*xN-a1*zN;
    b3 = a1*yN-a2*xN;

    float norm_b = sqrt(b1*b1 + b2*b2 + b3*b3);
    b1/=norm_b; b2/=norm_b; b3/=norm_b;

    glBegin(GL_POINTS);
    for(float a=0; a<=360; a++)
    {
    x=xC+r[0]*cos(a/180.0*M_PI)*a1+r[0]*sin(a/180.0*M_PI)*b1;
    y=yC+r[0]*cos(a/180.0*M_PI)*a2+r[0]*sin(a/180.0*M_PI)*b2;
    z=zC+r[0]*cos(a/180.0*M_PI)*a3+r[0]*sin(a/180.0*M_PI)*b3;

    x /=dx;
    y /=dy;
    z /=dz;

    glVertex3f(y, x, z);
    }
    glEnd();

   }

}
//------------------------------------------------------------------------------
void drawAortaRadius(TList* CenterLine, TList* Normales, TList* Radios,
                float dx, float dy, float dz, int Position)
{
    //Variables Auxiliares:
    PuntoF* pauxC;
    PuntoF* pauxN;
    float* r;

    float xC, yC, zC, xN, yN, zN;

    float x,y,z;

    float a1,a2,a3;
    float b1,b2,b3;

    glPointSize(5.0);
    glColor4f(0.5, 0.0, 0.1, 0.8);

    pauxC = (PuntoF*)CenterLine->Items[Position];
    pauxN = (PuntoF*)Normales->Items[Position];
    r = (float*)Radios->Items[Position];

    xC = pauxC->x;
    yC = pauxC->y;
    zC = pauxC->z;

    xN = pauxN->x;
    yN = pauxN->y;
    zN = pauxN->z;

    if(xN!=0)
    {
    a2 = 1.0;
    a3 = 1.0;
    a1 = (-yN*a2-zN*a3)/xN;
    }
    else if(yN!=0)
    {
    a1 = 1.0;
    a3 = 1.0;
    a2 = (-xN*a1-zN*a3)/yN;
    }
    else
    {
    a1 = 1.0;
    a2 = 1.0;
    a3 = (-xN*a1-yN*a2)/zN;
    }

    float norm_a = sqrt(a1*a1 + a2*a2 + a3*a3);
    a1/=norm_a; a2/=norm_a; a3/=norm_a;

    b1 = a2*zN-a3*yN;
    b2 = a3*xN-a1*zN;
    b3 = a1*yN-a2*xN;

    float norm_b = sqrt(b1*b1 + b2*b2 + b3*b3);
    b1/=norm_b; b2/=norm_b; b3/=norm_b;

    glBegin(GL_POINTS);
    for(float a=0; a<=360; a++)
    {
    x=xC+r[0]*cos(a/180.0*M_PI)*a1+r[0]*sin(a/180.0*M_PI)*b1;
    y=yC+r[0]*cos(a/180.0*M_PI)*a2+r[0]*sin(a/180.0*M_PI)*b2;
    z=zC+r[0]*cos(a/180.0*M_PI)*a3+r[0]*sin(a/180.0*M_PI)*b3;

    x /=dx;
    y /=dy;
    z /=dz;

    glVertex3f(y, x, z);
    }
    glEnd();


}
//-----------------------------------------------------------------------------
void drawAortaRadius(TList* CenterLine, TList* Normales, TList* Radios,
                float dx, float dy, float dz, int Position, AnsiString texto)
{
    //Variables Auxiliares:
    PuntoF* pauxC;
    PuntoF* pauxN;
    float* r;

    float xC, yC, zC, xN, yN, zN;

    float x,y,z;

    float a1,a2,a3;
    float b1,b2,b3;

    glPointSize(5.0);
    glColor4f(0.2, 0.2, 0.2, 0.8);



    pauxC = (PuntoF*)CenterLine->Items[Position];
    pauxN = (PuntoF*)Normales->Items[Position];
    r = (float*)Radios->Items[Position];

    xC = pauxC->x;
    yC = pauxC->y;
    zC = pauxC->z;

    xN = pauxN->x;
    yN = pauxN->y;
    zN = pauxN->z;

    if(xN!=0)
    {
    a2 = 1.0;
    a3 = 1.0;
    a1 = (-yN*a2-zN*a3)/xN;
    }
    else if(yN!=0)
    {
    a1 = 1.0;
    a3 = 1.0;
    a2 = (-xN*a1-zN*a3)/yN;
    }
    else
    {
    a1 = 1.0;
    a2 = 1.0;
    a3 = (-xN*a1-yN*a2)/zN;
    }

    float norm_a = sqrt(a1*a1 + a2*a2 + a3*a3);
    a1/=norm_a; a2/=norm_a; a3/=norm_a;

    b1 = a2*zN-a3*yN;
    b2 = a3*xN-a1*zN;
    b3 = a1*yN-a2*xN;

    float norm_b = sqrt(b1*b1 + b2*b2 + b3*b3);
    b1/=norm_b; b2/=norm_b; b3/=norm_b;

    glBegin(GL_POINTS);
    for(float a=0; a<=360; a++)
    {
    x=xC+r[0]*cos(a/180.0*M_PI)*a1+r[0]*sin(a/180.0*M_PI)*b1;
    y=yC+r[0]*cos(a/180.0*M_PI)*a2+r[0]*sin(a/180.0*M_PI)*b2;
    z=zC+r[0]*cos(a/180.0*M_PI)*a3+r[0]*sin(a/180.0*M_PI)*b3;

    x /=dx;
    y /=dy;
    z /=dz;

    glVertex3f(y, x, z);
    }
    glEnd();

    glRasterPos3f(y, x, z-5 );
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, texto.c_str()[0]);


}
//-----------------------------------------------------------------------------
void drawMesh(TList* FaceList, float* color)
{
    //Punteros auxiliares a nodos y caras:
    const Node *pn0;
    const Node *pn1;
    const Node *pn2;
    const double* normal;
    Face* pF;

    const double* bbox_ur;
    const double* bbox_ll;


     //Rendering Funcions*****************************
     glEnable(GL_DEPTH_TEST);
     glShadeModel(GL_SMOOTH);
     glEnable(GL_CULL_FACE);
     //***********************************************
     // glEnable(GL_BLEND);
     // glBlendFunc(GL_DST_ALPHA,GL_SRC_ALPHA);

    //Lighting Functions:******************************
    GLfloat light_position0[] = {10.0, 0, 0, 0.0};
    GLfloat light_position1[]= {-10.0, 10.0, 0, 0.0};
    GLfloat light_position2[]= {0, 10.0, 0, 0.0};
    GLfloat light_position3[]= {0, -10.0, 0, 0.0};
    GLfloat light_position4[]= {0, 0, 10.0, 0.0};
    GLfloat light_position5[]= {0, 10.0, -10.0, 0.0};
    GLfloat light_central[] = {0.0, 0.0, -10.0, 0.0};

    GLfloat white_light[] = {0.15, 0.15, 0.15, 0.0};
    GLfloat lmodel_ambient[] = {0.3, 0.3, 0.3, 0.0};

    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white_light);

    glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, white_light);
    glLightfv(GL_LIGHT3, GL_POSITION, light_position3);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, white_light);

    glLightfv(GL_LIGHT4, GL_POSITION, light_position4);
    glLightfv(GL_LIGHT4, GL_DIFFUSE, white_light);

    glLightfv(GL_LIGHT5, GL_POSITION, light_position5);
    glLightfv(GL_LIGHT5, GL_DIFFUSE, white_light);

    //glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_LIGHT3);
    glEnable(GL_LIGHT4);
    glEnable(GL_LIGHT5);
    //************************************************

    //Material Functions******************************
    glColorMaterial(GL_FRONT, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    //************************************************

    Face* pFa;
    float dx,dy,dz;
    pF = (Face*)FaceList->Items[0];
    //dx = pF->opNode->dx;
    //dy = pF->opNode->dy;
    //dz = pF->opNode->dz;

    glPointSize(5.0);

    const double* xyz0;
    const double* xyz1;
    const double* xyz2;

    glCullFace(GL_FRONT);

    for (int i = 0; i < FaceList->Count; ++i) {
      pF = (Face*)FaceList->Items[i];
      pn0 = pF->get_left_node();
      pn1 = pF->get_right_node();
      pn2 = pF->get_opp_node();
      normal = pF->get_normal();

      if (pn0->get_is_interface() || pn1->get_is_interface() ||
          pn2->get_is_interface())
        continue;

      xyz0 = pn0->get_position();
      xyz1 = pn1->get_position();
      xyz2 = pn2->get_position();

      bbox_ur = pn0->get_bounding_box_upper_right();
      bbox_ll = pn0->get_bounding_box_lower_left();
      //Cara interna:
      if (pn0->get_is_moving() && pn1->get_is_moving()
          && pn2->get_is_moving() ) {
        glBegin(GL_LINES);
        glNormal3f(-normal[1], -normal[0], -normal[2]);
        glColor3f(color[0], color[1], color[2]);
        glVertex3f( xyz0[1], xyz0[0], xyz0[2]);
        glVertex3f( xyz1[1], xyz1[0], xyz1[2]);
        glColor3f(  color[0],color[1],color[2]);
        glVertex3f( xyz2[1], xyz2[0], xyz2[2]);
        glColor3f(color[0], color[1], color[2]);
        glVertex3f( xyz0[1], xyz0[0], xyz0[2]);
        glEnd();
      }
      else {
        glBegin(GL_LINES);
        glNormal3f(-normal[1], -normal[0], -normal[2]);
        glColor3f(color[0]*0.8, color[1]*0.8, color[2]*0.8);
        glVertex3f( xyz0[1], xyz0[0], xyz0[2]);
        glVertex3f( xyz1[1], xyz1[0], xyz1[2]);
        glColor3f(  color[0]*0.8,color[1]*0.8,color[2]*0.8);
        glVertex3f( xyz2[1], xyz2[0], xyz2[2]);
        glColor3f(color[0]*0.8, color[1]*0.8, color[2]*0.8);
        glVertex3f( xyz0[1], xyz0[0], xyz0[2]);
        glEnd();
      }
    }

    /*

    float cgx,cgy,cgz;

    for(int i=0;i<FaceList->Count; ++i)
    {

    pF = (Face*)FaceList->Items[i];

    pF->ComputeMassCenter();

    //cgx = pF->cgx;
    //cgy = pF->cgy;
    //cgz = pF->cgz;

    //glBegin(GL_LINES);
    //glColor3f(1.0, 0.0, 0.0);
    //glVertex3f( cgy, cgx, cgz);
    //glVertex3f( (cgy + pF->yn), (cgx + pF->xn), (cgz + pF->zn));
    //glEnd();

    //glCullFace(GL_BACK);
    //Cara externa:
    /*if(pF->newborn)
        glColor3f(1.0, 0.0, 1.0);  //Recien nacido: violeta
    else
        glColor3f(1.0, 0.0, 0.0);  //Viejo: rojo

    glNormal3f(pF->yn, pF->xn, pF->zn);
    glVertex3f(pn2->xyz[1]/dy,pn2->xyz[0]/dx,pn2->xyz[2]/dz);
    glVertex3f(pn1->xyz[1]/dy,pn1->xyz[0]/dx,pn1->xyz[2]/dz);
    glVertex3f(pn0->xyz[1]/dy,pn0->xyz[0]/dx,pn0->xyz[2]/dz);
    */
    //}


    //Deshabilita Material:************
    glDisable(GL_COLOR_MATERIAL);
    //Deshabilita Luz:*****************
    glDisable(GL_LIGHT1);
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHT2);
    glDisable(GL_LIGHT3);
    glDisable(GL_LIGHT4);
    glDisable(GL_LIGHT5);
    //glDisable(GL_LIGHTING);

    //Draw Bounding Box
    glBegin(GL_LINES);
    glVertex3f( bbox_ll[1], bbox_ll[0], bbox_ll[2]);
    glVertex3f( bbox_ll[1], bbox_ll[0], bbox_ur[2]);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f( bbox_ll[1], bbox_ur[0], bbox_ll[2]);
    glVertex3f( bbox_ll[1], bbox_ur[0], bbox_ur[2]);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f( bbox_ur[1], bbox_ur[0], bbox_ll[2]);
    glVertex3f( bbox_ur[1], bbox_ur[0], bbox_ur[2]);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f( bbox_ur[1], bbox_ll[0], bbox_ll[2]);
    glVertex3f( bbox_ur[1], bbox_ll[0], bbox_ur[2]);
    glEnd();

    //Cara inferior:
    glBegin(GL_LINE_STRIP);
    glColor3f(  color[0]*0.5, color[1]*0.5, color[2]*0.5);
    glVertex3f( bbox_ll[1], bbox_ll[0], bbox_ll[2]);
    glVertex3f( bbox_ll[1], bbox_ur[0], bbox_ll[2]);
    glVertex3f( bbox_ur[1], bbox_ur[0], bbox_ll[2]);
    glVertex3f( bbox_ur[1], bbox_ll[0], bbox_ll[2]);
    glVertex3f( bbox_ll[1], bbox_ll[0], bbox_ll[2]);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex3f( bbox_ll[1], bbox_ll[0], bbox_ur[2]);
    glVertex3f( bbox_ll[1], bbox_ur[0], bbox_ur[2]);
    glVertex3f( bbox_ur[1], bbox_ur[0], bbox_ur[2]);
    glVertex3f( bbox_ur[1], bbox_ll[0], bbox_ur[2]);
    glVertex3f( bbox_ll[1], bbox_ll[0], bbox_ur[2]);
    glEnd();

    //Deshabilita Depth Test(z-buffer)
    glDisable(GL_DEPTH_TEST);
    //**********************************
    //Deshabilita Rendering Funcs*****
    glDisable(GL_CULL_FACE);
    //********************************
    glPointSize(1.0);
}
//-----------------------------------------------------------------------------

void drawNodes(TList* Nodes)
{
    glPointSize(2.0);
    Node* node;

    float dx,dy,dz;
    const double* xyz;
    node = (Node*)Nodes->Items[0];

    //Resolucion: dx,dy,dz

    dx = node->get_scale_x();
    dy = node->get_scale_y();
    dz = node->get_scale_z();

    glBegin(GL_POINTS);
    for(int i=0; i<Nodes->Count; ++i) {
      node = (Node*)Nodes->Items[i];
      xyz = node->get_position();
      glVertex3f(xyz[1], xyz[0], xyz[2]);
    }
    glEnd();

    glPointSize(1.0);
}

//------------------------------------------------------------------------------
