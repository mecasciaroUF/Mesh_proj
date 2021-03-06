//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Volumen.h"
#include "Matriz.h"

#include "structpunto.h"

#include "CACForm.h"

#include "getAllConnected.h"

#define CACGS 130 //CAC Gold Standard

struct placa
{
TList* puntos;
short int maxHU;
float area;
float score;
};

typedef struct placa Placa;
//******************************************************************************
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCACForm1 *CACForm1;
//---------------------------------------------------------------------------
__fastcall TCACForm1::TCACForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TCACForm1::FormActivate(TObject *Sender)
{
       CACvalue = 0;
       CACResultLabel->Caption = CACvalue;

       Swidth = Shape1->Width;
       Sheight = Shape1->Height;

       //Ninguna placa seleccionada
       seleccionado = -1;

       CACSliceLabel->Caption = UpDown1->Position;

       //Para disminuir flickering
       CACPanel->DoubleBuffered = true;

       //CuadroCAC = matriz donde se ve la imagen.
       CuadroCAC = new Matriz(alto,ancho);
       CuadroCAC->NColores = 0;

       UpDown1->Max=cantCortes;

       /*
       SelectionImage->Canvas->Pen->Color=clRed;
       SelectionImage->Canvas->Pen->Width=10;
       SelectionImage->Canvas->Brush->Color=clRed;
       */

       //Creaci�n del Volumen de posible CAC:
       CAC = new Volumen(alto,ancho,cantCortes+1);
       short int HU;

       CACProcessLabel->Caption = "Buscando Calcio";
       CACProgressBar->Max = cantCortes;
       CACProcessLabel->Visible = true;
       CACProgressBar->Visible = true;

       //Volumen CAC: Solo guarda los valores mayores a CACGS (CAC Gold Standard = 130HU)
       for(int k=0; k<cantCortes+1; k++)
       {CACProcessLabel->Refresh();
        CACProgressBar->Position=k;
         for(int i=0; i<alto; i++)
           for(int j=0; j<ancho; j++)
               {
                HU = MCAC->GetV(i,j,k);
                 if(HU>=CACGS)
                 CAC->SetV(i,j,k,1);
                 else CAC->SetV(i,j,k,0);
               }
         }

       //CACcorte almacena el valor de CAC para un �nico corte. Solo se usa para graficarlo
       CACcorte = new bool[alto*ancho];

       //CACcorte almacena el valor de CAC para una placa seleccioanda. Solo se usa para graficarla.
       CACSeleccion = new bool[alto*ancho];

       //Posibles puntos con Calcio
       ListaPuntosCAC = new TList;
       //Lista de almacenamiento de posibles placas:
       ListaPlacas = new TList;

       //Puntero auxiliar a Punto
       Punto* nsp;
       Punto* p;
       //Puntero auxiliar a Placa: el struct placa est� definido arriba.
       Placa* nP;

       //Umbral de area para ser considerado placa:
       CACAreaUmbral = 1.0; //1mm^2: Gold Standard

       short int maxHUPlaca = minPX;

       CACProcessLabel->Caption = "Agrupando Calcio: eliminado lesiones con A<1mm^2";

       //B�squeda de grupos CAC:
       for(int k=0; k<cantCortes+1; k++)
       {
       CACProgressBar->Position = k;
       CACProcessLabel->Refresh();

        for(int i=0; i<alto; i++)
         for(int j=0; j<ancho; j++)
           if(CAC->GetV(i,j,k))
           {
           nsp = new Punto;
           nsp->x=i; nsp->y=j; nsp->z=k;

           getAllPlaneConnected(nsp, CAC, ListaPuntosCAC);

              if(float(ListaPuntosCAC->Count)*dX*dY >= CACAreaUmbral)
              {
                nP = new Placa;
                nP->puntos = new TList;
                maxHUPlaca = minPX;

                 for(int l=0; l<ListaPuntosCAC->Count; l++)
                 {
                 p = (Punto*)ListaPuntosCAC->Items[l];
                 p->color = MCAC->GetV(p->x,p->y,p->z);

                  if(p->color>=maxHUPlaca)
                  maxHUPlaca = p->color;

                 nP->puntos->Add(p);
                 }

                nP->maxHU = maxHUPlaca;
                nP->area = float(nP->puntos->Count)*dX*dY;
                ListaPlacas->Add(nP);
               }

           ListaPuntosCAC->Clear();
         }
        }

     //Resetea Matriz CAC:
     for(int k=0; k<cantCortes+1; k++)
         for(int i=0; i<alto; i++)
           for(int j=0; j<ancho; j++)
                CAC->SetV(i,j,k,minPX);


      for(int i=0; i<ListaPlacas->Count; i++)
      {

      nP = (Placa*)ListaPlacas->Items[i];

        //Llena la matriz CAC con el n�mero de placa
        for(int l=0; l<nP->puntos->Count;l++)
        {
        nsp = (Punto*)nP->puntos->Items[l];
        CAC->SetV(nsp->x,nsp->y,nsp->z,i);
        }

       //Umbrales para el Score seg�n el paper original de Agatston:
       if(nP->maxHU>=130 && nP->maxHU<=199)
       nP->score = 1;
       if(nP->maxHU>=200 && nP->maxHU<=299)
       nP->score = 2;
       if(nP->maxHU>=300 && nP->maxHU<=399)
       nP->score = 3;
       if(nP->maxHU>=400)
       nP->score = 4;
      }

     CACProgressBar->Visible = false;
     CACProcessLabel->Visible = false;

     GraficarEnPantalla();
}
//---------------------------------------------------------------------------
void __fastcall TCACForm1::GraficarEnPantalla()
{
    int z = UpDown1->Position;

    for(int i=0; i<alto; i++)
     for(int j=0; j<ancho; j++)
     {
     CACcorte[i+j*ancho]=false;
     CACSeleccion[i+j*ancho]=false;
     }

    for(int i=0; i<ancho;i++)
        for(int j=0; j<alto;j++)
        {
          if(MCAC->GetV(i,j,z)>= minPX && MCAC->GetV(i,j,z)<= maxPX)
                 CuadroCAC->buffer(i,j)=LUT[MCAC->GetV(i,j,z)-minPX];
                 if(MCAC->GetV(i,j,z)> maxPX)
                 CuadroCAC->buffer(i,j)=LUT[maxPX-minPX];
                 if(MCAC->GetV(i,j,z)< minPX)
                 CuadroCAC->buffer(i,j)=LUT[0];

        if(CAC->GetV(i,j,z)>=0)
        CACcorte[i+j*ancho]=true;

        }

     CuadroCAC->Graficar(CACImage);

     Placa* P;
     Punto* p;

     if(seleccionado>=0)
     {
      P = (Placa*)ListaPlacas->Items[seleccionado];

      for(int j=0; j<P->puntos->Count; j++)
      {
       p = (Punto*)P->puntos->Items[j];

       if(p->z==z)
       CACSeleccion[p->x+p->y*ancho]=true;
      }
      CuadroCAC->Graficar(SelectionImage, 0, 255,0, CACSeleccion);
     }

     CuadroCAC->Graficar(TranspCACImage, 255, 255, 0, CACcorte);
}
//---------------------------------------------------------------------------
void __fastcall TCACForm1::UpDown1Click(TObject *Sender, TUDBtnType Button)
{
     TRect rect(0,0,ancho,alto);
     SelectionImage->Canvas->Brush->Color = clNone;
     SelectionImage->Canvas->FillRect(rect);
     SelectionImage->Canvas->Brush->Color = clRed;

     CACSliceLabel->Caption = UpDown1->Position;
     GraficarEnPantalla();
}
//---------------------------------------------------------------------------

void __fastcall TCACForm1::SelectionImageMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
    //   if(Shift.Contains(ssLeft))
    //     SelectionImage->Canvas->LineTo(X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TCACForm1::SelectionImageMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
          if(Button==mbLeft && !CheckBox1->Checked)
          {
          seleccionado = CAC->GetV(Y,X,UpDown1->Position);
          GraficarEnPantalla();
          }

          if(Button==mbLeft && CheckBox1->Checked)
          {
          //seleccionado = CAC->GetV(Y,X,UpDown1->Position);
          //GraficarEnPantalla();
          Shape1->Left = X-Shape1->Width/2;
          Shape1->Top = Y-Shape1->Height/2;
          }

          if(Button == mbRight && seleccionado>=0 && !CheckBox1->Checked)
          {
          float xC, yC, z; //Coordenadas del centro de masa de la placa
          xC = 0; yC=0; z=0;

          //Medici�n por Conectividad:
          Placa* nP;
          Punto* p;
          nP = (Placa*)ListaPlacas->Items[seleccionado];
          CACvalue += nP->area * nP->score;
          CACResultLabel->Caption = CACvalue;


           for(int i=0; i<nP->puntos->Count; i++)
           {
           p = (Punto*)nP->puntos->Items[i];
           CAC->SetV(p->x,p->y,p->z,minPX);
           xC+=p->x;
           yC+=p->y;
           z +=p->z;
           }
           xC /= nP->puntos->Count;
           yC /= nP->puntos->Count;
           z  /= nP->puntos->Count;

           xC *= dX;
           yC = ancho - yC;
           yC *= dY;
           z  = cantCortes - z;
           z  *= dZ;

           XListBox->Items->Add(yC);
           YListBox->Items->Add(xC);
           ZListBox->Items->Add(z);
           CACListBox->Items->Add(nP->area * nP->score);
           AreaListBox->Items->Add(nP->area);

           MaxHUListBox->Items->Add(nP->maxHU);

          nP->puntos->Clear();
          nP->maxHU=0;
          nP->area=0;
          nP->score=0;

          GraficarEnPantalla();

          seleccionado = -1;
          }

          if(Button == mbRight && CheckBox1->Checked)
          {
          //Medici�n de CAC para esa placa mediante ROI:
          int cont=0;
          short int maxHU = minPX;

           for(int i=Shape1->Left; i<Shape1->Left+Shape1->Width;i++)
             for(int j=Shape1->Top; j<Shape1->Top+Shape1->Height;j++)
              if(MCAC->GetV(j,i,UpDown1->Position)>=CACGS) //CAC Gold Standard
              {
               if(MCAC->GetV(j,i,UpDown1->Position)>=maxHU)
               maxHU = MCAC->GetV(j,i,UpDown1->Position);

               CAC->SetV(j,i,UpDown1->Position,-1);

               cont++;
              }

           float area = dX*dY*cont;
           float score;

           //Umbrales para el Score seg�n el paper original de Agatston:
           if(maxHU>=130 && maxHU<=199)
           score = 1;
           if(maxHU>=200 && maxHU<=299)
           score = 2;
           if(maxHU>=300 && maxHU<=399)
           score = 3;
           if(maxHU>=400)
           score = 4;

           score = score*area;
           CACvalue += score;
           CACResultLabel->Caption = CACvalue;

          GraficarEnPantalla();
          }

/*       if(Button == mbLeft)
         SelectionImage->Canvas->MoveTo(X,Y);

       if(Button == mbRight)
       {
       int z = UpDown1->Position;
       //Calcula en CAC dentro de la regi�n marcada:
        for(int i=0; i<ancho;i++)
         for(int j=0; j<alto;j++)
          //Es un pixel marcado?
          if(SelectionImage->Canvas->Pixels[j][i]==clRed)
           //Existe calcio dentro del pixel seleccionado?
           if(CAC->GetV(i,j,z)>=CACGS)
           {
           CACvalue += MCAC->GetV(i,j,z); //Suma al CAC total y
           CAC->SetV(i,j,z,minPX); //borra el calcio seleccionado para evitar repeticiones
           }

       //Resetea la pantalla de selecci�n:
       TRect rect(0,0,ancho,alto);
       SelectionImage->Canvas->Brush->Color = clNone;
       SelectionImage->Canvas->FillRect(rect);
       SelectionImage->Canvas->Brush->Color = clRed;

       //Refresca el gr�fico:
       GraficarEnPantalla();
       }

       //Muestra el valor de CAC actualizado:
       CACResultLabel->Caption = CACvalue;      */
}
//---------------------------------------------------------------------------

void __fastcall TCACForm1::FormDeactivate(TObject *Sender)
{
      delete CuadroCAC;
      CuadroCAC = NULL;

      delete CAC;
      CAC = NULL;

      delete CACcorte;
      CACcorte = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TCACForm1::GuardarCACButtonClick(TObject *Sender)
{
   SaveDialog1->Filter = "TXT files (*.txt)|*.txt";
    if(SaveDialog1->Execute())
    {
       CACResultLabel->Caption = CACvalue;
    //   ListBox1->Items->Add(CACResultLabel->Caption); //Guarda el CAC total

       AnsiString linea;

       for(int i=0; i<XListBox->Items->Count; i++)  //Guarda el perfil placa x placa
       {
       linea  = XListBox->Items->Strings[i];
       linea += " ";
       linea += YListBox->Items->Strings[i];
       linea += " ";
       linea += ZListBox->Items->Strings[i];
       linea += " ";
       linea += CACListBox->Items->Strings[i];
       linea += " ";
       linea += AreaListBox->Items->Strings[i];
       linea += " ";
       linea += MaxHUListBox->Items->Strings[i];

       ListBox1->Items->Add(linea);
       }

       ListBox1->Items->SaveToFile(SaveDialog1->FileName);

    }

}
//---------------------------------------------------------------------------

void __fastcall TCACForm1::TrackBar1Change(TObject *Sender)
{
        

             if(TrackBar1->Position>=0)
             {
             Shape1->Width = Swidth*(TrackBar1->Position+1);
             Shape1->Height = Swidth*(TrackBar1->Position+1);
             }
             if(TrackBar1->Position<0)
             {
             Shape1->Width = Swidth/(abs(TrackBar1->Position)+1);
             Shape1->Height = Sheight/(abs(TrackBar1->Position)+1);
             }

}
//---------------------------------------------------------------------------


void __fastcall TCACForm1::FormClose(TObject *Sender, TCloseAction &Action)
{

        SaveDialog1->FileName = "";

        ListBox1->Clear();

        XListBox->Clear();
        YListBox->Clear();
        ZListBox->Clear();
        CACListBox->Clear();
        AreaListBox->Clear();
        MaxHUListBox->Clear();

        XListBox->Items->Clear();
        YListBox->Items->Clear();
        ZListBox->Items->Clear();
        CACListBox->Items->Clear();
        AreaListBox->Items->Clear();
        MaxHUListBox->Items->Clear();
}
//---------------------------------------------------------------------------



void __fastcall TCACForm1::BorrarScoreButtonClick(TObject *Sender)
{

        SaveDialog1->FileName = "";

        ListBox1->Clear();

        XListBox->Clear();
        YListBox->Clear();
        ZListBox->Clear();
        CACListBox->Clear();
        AreaListBox->Clear();
        MaxHUListBox->Clear();

        XListBox->Items->Clear();
        YListBox->Items->Clear();
        ZListBox->Items->Clear();
        CACListBox->Items->Clear();
        AreaListBox->Items->Clear();
        MaxHUListBox->Items->Clear();
}
//---------------------------------------------------------------------------


void __fastcall TCACForm1::CACSliceLabelDblClick(TObject *Sender)
{

     UpDown1->Position = 0;

     TRect rect(0,0,ancho,alto);
     SelectionImage->Canvas->Brush->Color = clNone;
     SelectionImage->Canvas->FillRect(rect);
     SelectionImage->Canvas->Brush->Color = clRed;

     CACSliceLabel->Caption = UpDown1->Position;

     GraficarEnPantalla();
}
//---------------------------------------------------------------------------

