//---------------------------------------------------------------------------

#include <vcl.h>
#include <math.h>

#pragma hdrstop

#include "Unit1.h"

#include <gl\gl.h>
#include <gl\glu.h>
#include "glut.h"

//estructuras:
#include "structpunto.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall DrawThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall DrawThread::DrawThread(bool CreateSuspended)
        : TThread(CreateSuspended)
{
	 Priority = tpIdle;
}
//---------------------------------------------------------------------------

 void __fastcall  DrawThread::SetParams(TList* RP, float* L, float* Lh, float* Lm, float* Ls,
                                  short int mX, short int mY, short int mZ, short int min,
                                  float sPX, int i0, int p, float uG)
 {

 RenderPoints = RP;
 LUT = L;
 LH = Lh;
 LM = Lm;
 LS = Ls;
 medX = mX;
 medY = mY;
 medZ = mZ;
 minPX = min;
 tamanioPixel = sPX;
 k0 = i0;
 paso = p;
 umbralGrad = uG;

}

//---------------------------------------------------------------------------

void __fastcall DrawThread::Execute()
{
        //---- Place thread code here ----:
     while(true)
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
        GLfloat light_position1[] = {-100.0, -100.0, -100.0, 0.0};
         GLfloat light_central[] = {0.0, 0.0, 100.0, 0.0};

        GLfloat white_light[] = {1.0, 1.0, 1.0, 0.0};
        GLfloat lmodel_ambient[] = {0.1, 0.1, 0.1, 0.0};

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
        GLfloat  mat_h[]={0.45, 0.45, 0.25, 1.0};
        GLfloat  mat_m[]={0.55, 0.1, 0.1, 1.0};
        GLfloat  mat_s[]={0.9, 0.1, 0.0, 1.0};

        glColorMaterial(GL_FRONT, GL_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        //************************************************

         //Rendering Funcions*****************************
         glPointSize(tamanioPixel);

       //  glShadeModel(GL_SMOOTH);
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
      for(int k=k0;k<RenderPoints->Count;k+=paso)
      {
        if(ap = (RenderPunto*)RenderPoints->Items[k])
        {
        x=ap->p.x; y=ap->p.y; z=ap->p.z;

        valorHU = ap->p.color;
        valorLUT=float(LUT[valorHU-minPX-1])/255.0;

          if(valorLUT > 0)
          {
            vh = LH[valorHU-minPX-1];
            vm = LM[valorHU-minPX-1];
            vs = LS[valorHU-minPX-1];

           // glColor4f(mat_hueso[0],mat_hueso[1],mat_hueso[2],valorLUT);
            cr = (mat_h[0]*vh + mat_m[0]*vm, + mat_s[0]*vs)/255.0;
            cg = (mat_h[1]*vh + mat_m[1]*vm, + mat_s[1]*vs)/255.0;
            cb = (mat_h[2]*vh + mat_m[2]*vm, + mat_s[2]*vs)/255.0;

            //Colores normalizados para mantener la intensidad:
         //   nrm = sqrt(cr*cr+cg*cg+cb*cb);
         //   cr = cr/nrm; cg = cg/nrm; cb = cb/nrm;

            if(ap->norma>umbralGrad)
            glColor4f(cr,cg,cb, valorLUT + 0.5);
            else
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
}
//---------------------------------------------------------------------------
 