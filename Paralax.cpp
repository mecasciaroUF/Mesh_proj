//------------------------------------------------------------------------------
#include <vcl.h>

//Librerías externas:
#include "iostream.h"
#include "Paralax.h"

//Constantes:
#define CACGS 130 //CAC Gold Standard
#pragma hdrstop
#include "include/dicomdll.h"

//------------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

typedef struct
{
  TLogPalette lpal;
  TPaletteEntry dummy[256];
} LogPal;

struct encabezado
{
        //frame: InstanceNumber;
	short int frame;
	short int alto,ancho;
	short int min,max;
	float dX, dY, dZ;
	//dX: PixelSpacingX
	//dY: PixelSpacingY
	//dZ: SliceLocation->calculo posterior de dZ en Borland a partir de las diferencias.
};


typedef struct encabezado Encabezado;

  //Sort TList
        int __fastcall SorterRot(void* Item1, void* Item2)
        {
        Punto* X1 = (Punto*)Item1;
        Punto* X2 = (Punto*)Item2;
        int x1,x2;
        x1=X1->xR; //mean = xRotado
        x2=X2->xR; //mean = xRotado
        return x1-x2;
        }

     //Sort Index TList
        int __fastcall IndicesSorter(void* Item1, void* Item2)
        {
        int* X1 = (int*)Item1;
        int* X2 = (int*)Item2;
        int x1,x2;
        x1=X1[0];
        x2=X2[0];
        return x2-x1;
        }

int __fastcall CompareColors(void *Item1, void *Item2)
{
  return ((Punto*)Item2)->color - ((Punto*)Item1)->color;
}

int __fastcall CompareZR(void *Item1, void *Item2)
{
  return ((Punto*)Item2)->z - ((Punto*)Item1)->z;
}

int redondeo(double valor)
{
  float dcim = fabs(valor-floor(valor));
  int redondeo;

  if(dcim >=0.5 && valor<0)
  redondeo = floor(valor);

  if(dcim <0.5 && valor<0)
  redondeo = ceil(valor);

  if(dcim >=0.5 && valor>0)
  redondeo = ceil(valor);

  if(dcim <0.5 && valor>0)
  redondeo = floor(valor);

  return redondeo;
}


TForm1 *Form1;

//------------------------------------------------------------------------------

__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
    Meshes = new TList;
    PointMap = new TList;

    printf("hola");

    size_t w = sizeof(WORD);
    size_t dw = sizeof(DWORD);
    size_t lo = sizeof(LONG);

    size_t bfh = sizeof(BITMAPFILEHEADER);
    size_t bif = sizeof(BITMAPINFOHEADER);

    activemesh = false;

    t = 0.0;
    empuje = false;

    //get shade.pso dir
    char CurrentDir[1000];
    GetCurrentDirectory(1000,CurrentDir);
    ShaderDir=CurrentDir;
    ShaderDir+="\\shader.pso";

    histo_thread_terminado = false;
    HistoChart->Enabled    = false;

    firstRender            = false;

    PasoTrackBar->Enabled   = false;
    PasoEdit->Enabled       = false;
    ZnormCheckBox->Enabled  = false;
    ZnormCheckBox->Checked  = true;

    MinzScrollBar->Enabled  = false;
    MaxzScrollBar->Enabled   = false;

    CenterTrackBar->Enabled = false;
    WidthTrackBar->Enabled  = false;
    CenterEdit->Enabled     = false;
    WidthEdit->Enabled      = false;
    CSATrackBar->Enabled    = false;
    CropCheckBox->Enabled   = false;

    //Deshabilito Ortho View, asignándole un controlador nulo en lugar de
    //TabSheet:
    OrthoViewSheet->PageControl = NULL;
    //Idem para VOI Control:
    TabSheet2->PageControl = NULL;

    //Deshabilito todas las funciones que no utilizo:
    ROI1->Visible = false;
    Filtros1->Visible = false;
    Ajuste1->Visible = false;
    Ventana1->Visible = false;
    Segmentacin1->Visible = false;
    Render1->Visible = false;
    Texturas1->Visible = false;

    muAorta = 0;
    sigmaAorta = 0;
    nAorta = 0;
    usarAortaProm = false;

    isAortaSegmented = false;
    isCenterLineDetected = false;
    isModelComplete = false;
    carinaDetectada = false;
    CACDDetectados = false;

    moviendoCACD = false;

    ImagenEnPantalla = false;
    ImageScrollBar->Enabled=false;
    Pantalla->Visible=false;

    MPRPanel->DoubleBuffered = true;

    annulusIndex = -1;
    CAIndex = -1;
    CDIndex = -1;
    BCAIndex = -1;
    LSAIndex = -1;
    apexSinusIndex = -1;
    diaphragmIndex = -1;
    ModificaX=false;
    ModificaY=false;
    ModificaCentro=false;
    dibujaCils = false;
    dibujaEsfs = false;

    RenderFlag=false;
    SimpleFlag=false;
    RenderPoints = new TList;

    RenderRotatedPoints = new TList;
    EsquelPoints = new TList;
    EP = new TList;
    LP = new TList;
    JP = new TList;
    BP = new TList;
    EraseP = new TList;

    pasoX = 1;
    pasoY = 1;
    pasoZ = 1;

    distX=200;
    distY=200;
    distZ=200;

    //OnIdle: utilizado por OpenGl: solo renderiza cuando el programa no está ocupado
    Application->OnIdle = IdleLoop;

    //Inicio de variables
    size = 150.0f;
    distancia=1000.0;
    elevacion=90.0;
    azimuth=0.0;
    trHor,trVert=0;
    CantPuntos=0;
    redmask = 0x000000ff;
    greenmask = 0x0000ff00;
    bluemask = 0x00ff0000;
    res[0]=1.0;
    res[1]=1.0;
    res[2]=1.0;

    PanelPantalla->DoubleBuffered = true;
    HistoChart->DoubleBuffered = true;

    offset = 0;
    anguloRotArb=0;
    CilindrosList  = new TList;
    EsferasList    = new TList;
    SelectedPoints = new TList;
    BorderPoints   = new TList;

    NodeList       = new TList;
    RamaList       = new TList;
    RamaSmoothList = new TList;
    VolRamaList    = new TList;

    drawAorta      = false;
    mostrarTex3D   = false;

    paso = PasoTrackBar->Position;

    xCarina = 0;
    yCarina = 0;
    zCarina = 0;

    meshOK = false;

    ManualCenterLine = new TList;
    UFCSpline      = NULL;
    NormalesSpline = NULL;
}

//------------------------------------------------------------------------------

void __fastcall TForm1::Cerrar1Click(TObject *Sender)
{
      if(!ImagenEnPantalla)
      return;

      firstRender = false;

      PasoTrackBar->Enabled   = false;
      ZnormCheckBox->Enabled  = false;
      ZnormCheckBox->Checked  = true;
      PasoTrackBar->Position = 4;
      PasoEdit->Enabled = false;
      PasoEdit->Text = "2";

      HistoChart->Enabled = false;

      MinzScrollBar->Enabled  = false;
      MinzScrollBar->Position = MinzScrollBar->Min;
      MaxzScrollBar->Enabled  = false;
      MaxzScrollBar->Position = MaxzScrollBar->Max;

      CropCheckBox->Enabled   = false;

      MaxPxLabel->Caption="-";
      MinPxLabel->Caption="-";

      CenterTrackBar->OnChange = NULL;
      CenterTrackBar->Min=0;
      CenterTrackBar->Max=10;
      CenterTrackBar->Position=0;
      Center = 0;

      CenterEdit->OnChange = NULL;
      CenterEdit->Text = 0;
      CenterEdit->OnChange = CenterEditChange;
      CenterTrackBar->OnChange = CenterTrackBarChange;

      WidthTrackBar->OnChange = NULL;
      WidthTrackBar->Min=0;
      WidthTrackBar->Max=10;
      WidthTrackBar->Position=0;
      Width =  1000;
      WidthEdit->OnChange = NULL;
      WidthEdit->Text = 1000;
      CenterEdit->OnChange = WidthEditChange;
      WidthTrackBar->OnChange = WidthTrackBarChange;

      CenterTrackBar->Enabled = false;
      WidthTrackBar->Enabled  = false;
      CenterEdit->Enabled     = false;
      WidthEdit->Enabled      = false;

      muAorta = 0;
      sigmaAorta = 0;
      nAorta = 0;
      usarAortaProm = false;

      isAortaSegmented = false;
      isCenterLineDetected = false;
      isModelComplete = false;
      drawAorta = false;
      mostrarTex3D = false;

      carinaDetectada = false;
      CACDDetectados = false;

      moviendoCACD = false;

      annulusIndex = -1;
      CAIndex = -1;
      CDIndex = -1;
      BCAIndex = -1;
      LSAIndex = -1;
      apexSinusIndex = -1;
      diaphragmIndex = -1;

      ModificaX=false;
      ModificaY=false;
      ModificaCentro=false;
      dibujaCils = false;
      dibujaEsfs = false;

      RenderFlag=false;
      SimpleFlag=false;

      pasoX = 1;
      pasoY = 1;
      pasoZ = 1;

      distX=200;
      distY=200;
      distZ=200;

     //Inicio de variables
     size = 150.0f;
     distancia=1000.0;
     elevacion=90.0;
     azimuth=0.0;
     trHor,trVert=0;
     CantPuntos=0;

     redmask = 0x000000ff;
     greenmask = 0x0000ff00;
     bluemask = 0x00ff0000;

     res[0]=1.0;
     res[1]=1.0;
     res[2]=1.0;

     offset = 0;
     anguloRotArb=0;

     xCarina = 0;
     yCarina = 0;
     zCarina = 0;

     //Borra las pantallas
     ImagenEnPantalla = false;
     Pantalla->Visible = false;
     histo_thread_terminado = false;

     //Restaura y deshabilita ImageScrollBar
     ImageScrollBar->Position = 0;
     ImageScrollBar->Enabled = false;

     AP->Clear();

     //Limpia SliceListBox
     SliceListBox->Clear();
     CarpetaLabel->Caption = "";

     //Elimina y deshabilita ROI, deselecciona y deshabilita ContourCheckBox
     ROI1->Enabled = false;
     ROIShape->Visible = false;
     ROICheckBox->State=cbUnchecked;
     ROICheckBox->Enabled=false;

     //Limpia las etiquetas:
     ArchivoLabel->Caption = "";
     CarpetaLabel->Caption = "";

     AorticRegionsGroupBox->Enabled = false;
     AnnulusCheckBox->Checked       = false;
     CACheckBox->Checked            = false;
     BCACheckBox->Checked           = false;
     LSACheckBox->Checked           = false;
     CDCheckBox->Checked            = false;
     ApexSinusCheckBox->Checked     = false;
     DiaphragmCheckBox->Checked     = false;

     CarinaButton->Enabled          = true;
     CACDButton->Enabled            = false;
     CreateToroidButton->Enabled    = false;
     SegmentDAButton->Enabled       = false;
     SegmentArchButton->Enabled     = false;
     OrthoCorrectionButton->Enabled = false;

     CSATrackBar->Enabled           = false;
     CSATrackBar->Max               = 10;
     CSATrackBar->Min               = 0;
     CSATrackBar->Position          = 0;

     VistaRadioGroup->ItemIndex     = 0;
     VistaRadioGroup->Enabled       = false;

     SaveModelButton->Enabled       = false;
     PrintReportButton->Enabled     = false;

      if(Cuadro)
      {
      delete Cuadro;
      Cuadro = NULL;
      }

      if(CuadroCorteOblicuo)
      {
      delete CuadroCorteOblicuo;
      CuadroCorteOblicuo = NULL;
      }

  //Eliminación de los elementos dinámicos asociados a la apertura de un estudio:
   if(M)
   {
   delete M;
   M = NULL;
   }

   if(Mauxiliar)
   {
   delete Mauxiliar;
   Mauxiliar = NULL;
   }

   if(Mvisitado)
   {
   delete Mvisitado;
   Mvisitado = NULL;
   }

   RenderPoints->Clear();

   if(LUT)
   {
   delete[] LUT;
   LUT = NULL;
   }

   if(histoCaja)
   {
   delete[] histoCaja;
   histoCaja = NULL;
   }

   for(int i=0; i<altoPX; i++)
   {
        delete[] PlanoCorte[i];
        delete[] PlanoOrig[i];
   }
   delete[] PlanoCorte;
   delete[] PlanoOrig;

   PuntoF* paux;
   float* raux;

   if(AortaCenterLine)
   {
    for(int i=0; i<AortaCenterLine->Count; i++)
    {
     paux = (PuntoF*)AortaCenterLine->Items[i];
     delete paux;
    }
   AortaCenterLine->Clear();
   delete AortaCenterLine;
   AortaCenterLine = NULL;
   }

   if(AortaNormales)
   {
    for(int i=0; i<AortaNormales->Count; i++)
    {
     paux = (PuntoF*)AortaNormales->Items[i];
     delete paux;
    }
    AortaNormales->Clear();
    delete AortaNormales;
    AortaNormales = NULL;
   }

   if(AortaRadios)
   {
    for(int i=0; i<AortaRadios->Count; i++)
    {
     raux = (float*)AortaRadios->Items[i];
     delete raux;
    }
    AortaRadios->Clear();
    delete AortaRadios;
    AortaRadios = NULL;
   }

   if(CurvaToroide)
   {
    for(int i=0; i<CurvaToroide->Count; i++)
    {
     paux = (PuntoF*)CurvaToroide->Items[i];
     delete paux;
    }
    CurvaToroide->Clear();
    delete CurvaToroide;
    CurvaToroide = NULL;
   }


   if(NormalToroide)
   {
    for(int i=0; i<NormalToroide->Count; i++)
    {
    paux = (PuntoF*)NormalToroide->Items[i];
    delete paux;
    }
    NormalToroide->Clear();
    delete NormalToroide;
    NormalToroide = NULL;
   }

   Shape8->Visible = false;
   RShape8->Visible = false;
   Shape7->Visible = false;
   RShape7->Visible = false;

   //if(myMesh)
   //delete myMesh;


   if(ManualCenterLine)
   delete ManualCenterLine;

}
//------------------------------------------------------------------------------

void __fastcall TForm1::Salir1Click(TObject *Sender)
{
        if( MessageBox(NULL, "¿Desea Salir?", "Salir",MB_YESNO|MB_ICONQUESTION) == IDNO)
        return;

        Cerrar1Click(Sender);

        Close();
}
//------------------------------------------------------------------------------

void __fastcall TForm1::ROIShapeMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
        //Selección de la ROI: determina si se quiere ampliar en X, Y o desplazar su Centro
         /*
         3----4----5
         |         |
         2    C    6
         |         |
         1----8----7
         */

         //Modificación de Lados o Esquinas:
         //...................................
         int yf=ROIShape->Height;
         int xf = ROIShape->Width;
         int espesor = ROIShape->Pen->Width;

         //...................................
          if((X<=espesor)&&(Y>=yf-espesor))
          {
                  Cursor = crSizeNESW;
                  ModificaEsq = true;
                  Lado = 1;
                  SetCaptureControl(Pantalla);
          return;
          }
         //...................................
          if((Y>espesor&&Y<yf-espesor) && (X<=espesor))
          {
                  ModificaLado = true;
                  Lado = 2;
                  SetCaptureControl(Pantalla);
                  Cursor = crSizeWE;
          return;
          }
         //...................................
          if((X<=espesor)&& (Y<=espesor))
          {
                  ModificaEsq = true;
                  Lado = 3;
                  SetCaptureControl(Pantalla);
                  Cursor = crSizeNWSE;
          return;
          }
         //...................................
          if((X>espesor && X<xf-espesor)&& (Y<=espesor))
          {
                  ModificaLado = true;
                  Lado = 4;
                  SetCaptureControl(Pantalla);
                  Cursor = crSizeNS;
          return;
          }
         //...................................
          if((X>=xf-espesor)&& (Y<=espesor))
          {
                  ModificaEsq = true;
                  Lado = 5;
                  SetCaptureControl(Pantalla);
                  Cursor = crSizeNESW;
          return;
          }
         //...................................
          if((X>=xf-espesor)&& (Y>espesor && Y<yf-espesor))
          {
                  ModificaLado = true;
                  Lado = 6;
                  SetCaptureControl(Pantalla);
                  Cursor = crSizeWE;
          return;
          }
         //...................................
          if((X>=xf-espesor)&& (Y>=yf-espesor))
          {
                  ModificaEsq = true;
                  Lado = 7;
                  SetCaptureControl(Pantalla);
                  Cursor = crSizeNWSE;
          return;
          }
         //...................................
          if((X>espesor && X<xf-espesor)&& (Y<yf && Y>yf-espesor))
          {
                  ModificaLado = true;
                  Lado = 8;
                  SetCaptureControl(Pantalla);
                  Cursor = crSizeNS;
          return;
          }
         //...................................

          //Si no entra a ninguna de las anteriores, entonces no se están modificando los lados:
                //se está modificando el centro
          ModificaCentro=true;
          xP = X;
          yP = Y;
          SetCaptureControl(Pantalla);
}
//------------------------------------------------------------------------------
void __fastcall TForm1::ROIShapeMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
         //Modifica los contornos de la ROI cuando se levanta el mouse.

         if(ModificaEsq)
         {
         ModificaEsq = false;
         ReleaseCapture();
         }

         if(ModificaLado)
         {
         ModificaLado = false;
         ReleaseCapture();
         }

         if(ModificaCentro)
         {
         ModificaCentro = false;
         ReleaseCapture();
         }

        if(moviendoCACD)
        {
        moviendoCACD = false;
        ReleaseCapture();
        }

         Cursor = crDefault;
}
//------------------------------------------------------------------------------
void __fastcall TForm1::PantallaMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
        Label10->Caption=X;
        Label11->Caption=Y;
        //Realiza dinámicamente lo mismo que en ROIShapeMouseDown

        //Permite corrección Manual de la posición y radios de AortaAscendente
        //y Aorta Descendente:
        if(CACDDetectados && moviendoCACD)
        {
             //Si estoy dentro del shape8:
             if(X<RShape8->Left+RShape8->Width && X>RShape8->Left)
             {
             Shape8->Left = (X-xP);
             Shape8->Top = (Y-yP);

             RShape8->Left = Shape8->Left-RShape8->Width/2+Shape8->Width/2;
             RShape8->Top = Shape8->Top-RShape8->Height/2+Shape8->Height/2;

             return;
             }

             //Si estoy dentro del shape7:
             if(X<RShape7->Left+RShape7->Width && X>RShape7->Left)
             {
             Shape7->Left = (X-xP);
             Shape7->Top = (Y-yP);

             RShape7->Left = Shape7->Left-RShape7->Width/2+Shape7->Width/2;
             RShape7->Top = Shape7->Top-RShape7->Height/2+Shape7->Height/2;

             return;
             }
        }

         if(ModificaCentro == true)
         {ROIShape->Left = X-xP;
         ROIShape->Top = Y-yP;}

         if(ModificaEsq)
         {
                if(Lado==1) //Esquina inferior izquierda
                {
                ROIShape->Width=ROIShape->Width-X+ROIShape->Left;
                ROIShape->Left=X;
                ROIShape->Height=Y-ROIShape->Top;
                }

                if(Lado==3) //Esquina superior izquierda
                {
                ROIShape->Width=ROIShape->Width-X+ROIShape->Left;
                ROIShape->Left=X;
                ROIShape->Height=ROIShape->Height-Y+ROIShape->Top;
                ROIShape->Top=Y;
                }

                if(Lado==5) //Esquina superior derecha
                {
                ROIShape->Width=X-ROIShape->Left;
                ROIShape->Height=ROIShape->Height-Y+ROIShape->Top;
                ROIShape->Top=Y;
                }

                if(Lado==7) //Esquina inferior derecha
                {
                ROIShape->Width=X-ROIShape->Left;
                ROIShape->Height=Y-ROIShape->Top;
                }
         }

         if(ModificaLado)
         {
            if(Lado==2) //Lado Izquierdo
                {ROIShape->Width=ROIShape->Width-X+ROIShape->Left;
                 ROIShape->Left=X;}

           if(Lado==4) //Lado Superior
                {ROIShape->Height=ROIShape->Height-Y+ROIShape->Top;
                 ROIShape->Top=Y;}


          if(Lado==6) //Lado Derecho
                {ROIShape->Width=X-ROIShape->Left;}

          if(Lado==8) //Lado Inferior
                {ROIShape->Height=Y-ROIShape->Top;}
         }

         if(ImagenEnPantalla)
        {int zindex = atoi(SliceListBox->Items->Strings[SliceListBox->ItemIndex].c_str());
         HULabel->Caption=M->GetV(Y,X,zindex);

             float meanArray[27];

             int h=0;
         if(zindex>0 && zindex<cantImagenes-1)
         {
         for(int z=-1;z<=1;z++)
                for(int y=-1;y<=1;y++)
                  for(int x=-1; x<=1;x++)
                          {
                          if(Y-x<anchoPX && Y-x>0 && X-y<altoPX && X-y>0)
                          meanArray[h]= M->GetV(Y-x,X-y,zindex-z);
                          h++;
                          }
         double acum=0;
         for(int i=0;i<27;i++)
            acum = acum + meanArray[i];

         mean = acum/27.0;

         acum=0;
         /* ARREGLAR:
         for(int i=0;i<27;i++)
            acum = acum + pow(meanArray[i]-mean,2);
         */

          StD = sqrt(acum/27.0);

         }else mean=maxPX+1;

         if(mean != maxPX+1)
         {
         MeanLabel->Caption=mean;
         StDLabel->Caption=StD;
         }

         }
}
//------------------------------------------------------------------------------

void __fastcall TForm1::Guardar1Click(TObject *Sender)
{
    //Permite al usuario guardar una ROI para un estudio determinado.
    // La ROI se graba en formato .txt
   bool ROIVOI; //0: ROI, 1: VOI
   bool WindowSaveFlag;

   if(ROISaveForm1->ShowModal()==mrOk)
   {
   if(ROISaveForm1->ROIRadioGroup->ItemIndex==0)
   ROIVOI=0;

   if(ROISaveForm1->ROIRadioGroup->ItemIndex==1)
   ROIVOI=1;

   if(ROISaveForm1->WindowSaveCheckBox->Checked)
   WindowSaveFlag=true;
   else WindowSaveFlag=false;

   }else return;

    SaveDialog1->Filter = "TXT files (*.txt)|*.txt";
    if(SaveDialog1->Execute())
    {
      if(!ROIVOI)
      {
      ROIListBox->Items->Add("ROI: Left/Top/Height/Width");
      ROIListBox->Items->Add(ROIShape->Left);
      ROIListBox->Items->Add(ROIShape->Top);
      ROIListBox->Items->Add(ROIShape->Height);
      ROIListBox->Items->Add(ROIShape->Width);
      }

      if(ROIVOI)
      {
      ROIListBox->Items->Add("ROI: Left/Top/Height/Width/Ini/Fin");
      ROIListBox->Items->Add(ROIShape->Left);
      ROIListBox->Items->Add(ROIShape->Top);
      ROIListBox->Items->Add(ROIShape->Height);
      ROIListBox->Items->Add(ROIShape->Width);
      ROIListBox->Items->Add(IniSliceEdit->Text);
      ROIListBox->Items->Add(FinSliceEdit->Text);
      }

      if(WindowSaveFlag)
      {
      ROIListBox->Items->Strings[0]=ROIListBox->Items->Strings[0]+"/Center/Width";
      ROIListBox->Items->Add(Center);
      ROIListBox->Items->Add(Width);
      }

      ROIListBox->Items->SaveToFile(SaveDialog1->FileName);
      ROIListBox->Clear();
    }
}
//------------------------------------------------------------------------------

void __fastcall TForm1::Cargar1Click(TObject *Sender)   //Cargar ROI pregrabada
{
    //Carga una ROI pregrabada mediante TForm1::Guardar1Click
    OpenDialog1->Filter = "TXT files (*.txt)|*.txt";
      if (OpenDialog1->Execute())
        {
        ROIListBox->Items->LoadFromFile(OpenDialog1->FileName);

                if(ROIListBox->Items->Strings[0]=="ROI: Left/Top/Height/Width")
                {
                ROIShape->Left = atoi(ROIListBox->Items->Strings[1].c_str());
                ROIShape->Top = atoi(ROIListBox->Items->Strings[2].c_str());
                ROIShape->Height = atoi(ROIListBox->Items->Strings[3].c_str());
                ROIShape->Width = atoi(ROIListBox->Items->Strings[4].c_str());
                ROIShape->Visible=true;
                ROICheckBox->State=cbChecked;
                ROIListBox->Clear();
                return;
                }

                if(ROIListBox->Items->Strings[0]=="ROI: Left/Top/Height/Width/Center/Width")
                {
                ROIShape->Left = atoi(ROIListBox->Items->Strings[1].c_str());
                ROIShape->Top = atoi(ROIListBox->Items->Strings[2].c_str());
                ROIShape->Height = atoi(ROIListBox->Items->Strings[3].c_str());
                ROIShape->Width = atoi(ROIListBox->Items->Strings[4].c_str());
                Center=atoi(ROIListBox->Items->Strings[5].c_str());
                Width =atoi(ROIListBox->Items->Strings[6].c_str());
                ROIShape->Visible=true;
                ROICheckBox->State=cbChecked;
                ROIListBox->Clear();
                return;
                }

                if(ROIListBox->Items->Strings[0]=="ROI: Left/Top/Height/Width/Ini/Fin")
                {
                ROIShape->Left = atoi(ROIListBox->Items->Strings[1].c_str());
                ROIShape->Top = atoi(ROIListBox->Items->Strings[2].c_str());
                ROIShape->Height = atoi(ROIListBox->Items->Strings[3].c_str());
                ROIShape->Width = atoi(ROIListBox->Items->Strings[4].c_str());
                IniSliceEdit->Text = ROIListBox->Items->Strings[5];
                FinSliceEdit->Text = ROIListBox->Items->Strings[6];
                ROIShape->Visible=true;
                ROICheckBox->State=cbChecked;
                ROIListBox->Clear();
                return;
                }

                if(ROIListBox->Items->Strings[0]=="ROI: Left/Top/Height/Width/Ini/Fin/Center/Width")
                {
                ROIShape->Left = atoi(ROIListBox->Items->Strings[1].c_str());
                ROIShape->Top = atoi(ROIListBox->Items->Strings[2].c_str());
                ROIShape->Height = atoi(ROIListBox->Items->Strings[3].c_str());
                ROIShape->Width = atoi(ROIListBox->Items->Strings[4].c_str());
                IniSliceEdit->Text = ROIListBox->Items->Strings[5];
                FinSliceEdit->Text = ROIListBox->Items->Strings[6];
                CenterEdit->Text =ROIListBox->Items->Strings[7];
                WidthEdit->Text  =ROIListBox->Items->Strings[8];
                ROIShape->Visible=true;

                ROICheckBox->State=cbChecked;
                ROIListBox->Clear();
                return;
                }
                
                else {MessageBox(NULL, "El archivo seleccionado no corresponde a una ROI válida", "Error",MB_OK);}
        }
}
//------------------------------------------------------------------------------

void __fastcall TForm1::ROICheckBoxClick(TObject *Sender)
{                if(ROICheckBox->Checked == true)
                {
                //Activa ROI
                ROIShape->Visible = true;
                }

                if(ROICheckBox->Checked == false)
                {
                //Desactiva ROI
                ROIShape->Visible = false;
                }
}
//------------------------------------------------------------------------------

void __fastcall TForm1::IdleLoop(TObject*, bool& done)
{
        if(histo_thread_terminado)
        {
        histo_thread_terminado = false;

        MaxPxLabel->Caption=maxPX;
        MinPxLabel->Caption=realminPX;

        CenterTrackBar->OnChange = NULL;
        CenterTrackBar->Min=realminPX;
        CenterTrackBar->Max=maxPX;
        CenterTrackBar->Position=int((maxPX+realminPX)/2);
        Center = CenterTrackBar->Position;
        CenterEdit->OnChange = NULL;
        CenterEdit->Text = Center;
        CenterEdit->OnChange = CenterEditChange;
        CenterTrackBar->OnChange = CenterTrackBarChange;

        WidthTrackBar->OnChange = NULL;
        WidthTrackBar->Min=1;
        WidthTrackBar->Max=int((maxPX-realminPX)/2);
        WidthTrackBar->Position=int((maxPX-realminPX)/2);
        Width =  WidthTrackBar->Position;
        WidthEdit->OnChange = NULL;
        WidthEdit->Text = Width;
        CenterEdit->OnChange = WidthEditChange;
        WidthTrackBar->OnChange = WidthTrackBarChange;


        HistoChart->BottomAxis->AutomaticMinimum = false;
        HistoChart->BottomAxis->Minimum = realminPX;

        //Creación de la LUT:
        RefreshLUT();
        GraficarEnPantalla();
        }

        if(auxiliar==0)
        {
        start = clock();
        done = false;
        RenderGLScene();
        SwapBuffers(hDC);
        end = clock();
        Label3->Caption = (end-start)/CLK_TCK;
        return;
        }
}
//------------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
     //Elimina todos los arrays dinamicos para guardar puntos, proyecciones y buffers
     if(Cuadro)
     delete Cuadro;

     if(CuadroCorteOblicuo)
     delete CuadroCorteOblicuo;

     if(LUT)
     delete[] LUT;

     delete interwined;

     if(RenderPoints)
     {
     RenderPoints->Clear();
     delete RenderPoints;
     }

     delete CAC;
     /*
     for(int i=0; i<750;i++)
     for(int j=0; j<650;j++)
     {
      if (MapaIndices[i][j])
      {
      MapaIndices[i][j]->Clear();
      delete MapaIndices[i][j];
      }
     }*/

      // delete[] mu;
      //    delete[] omega;
             // delete[] histoCaja;
       /*
        for (int i=0; i<cantImagenes; ++i)
           for (int j=0; j<altoPX; ++j)
                delete[] M[i][j];
        for (int i=0; i<cantImagenes; ++i)
                delete M[i];
        delete[] M; */

    if(M)
    delete M;

    if(bitmap)
    delete bitmap;

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);


}
//------------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
    //Inicialización del ViewPort en PanelVista3D
    SetupRenderingContextPanel(ROIPanel, hDC, hRC, PanelWidth, PanelHeight, PixelFormat);

}
//------------------------------------------------------------------------------

void __fastcall TForm1::FormResize(TObject *Sender)
{
   //size = distancia;
   //GLfloat nRange = 3*size;

    //PanelWidth = ROIPanel->Width; //PanelVista3D->Width;
    //PanelHeight = ROIPanel->Height; //PanelVista3D->Height;

    if(PanelHeight == 0)
    	PanelHeight = 1;

    //Ventana
    glViewport(0.0, 0.0, PanelWidth, PanelHeight);

    //Tipo de visión en perspectiva
    //  glMatrixMode(GL_PROJECTION);

    //glLoadIdentity();

    /*if (PanelWidth <= PanelHeight)
        glOrtho (-nRange*2, nRange*2, -nRange*PanelHeight/PanelWidth*2, nRange*PanelHeight/PanelWidth*2, -nRange*50, nRange*50);
    else
    	glOrtho (-nRange*PanelWidth/PanelHeight*2, nRange*PanelWidth/PanelHeight*2, -nRange*2, nRange*2, -nRange*50, nRange*50);
    */

    /* glLoadIdentity();
      if(azimuth<0)
      azimuth=360+azimuth;
      if(azimuth>360)
      azimuth=azimuth-360;

      if(elevacion<0)
      elevacion=180+elevacion;
      if(elevacion>180)
      elevacion=elevacion-180;

      float elevacion_rad = float(elevacion/180.0*M_PI);
      float azimuth_rad = float(azimuth/180.0*M_PI);

      float eyex, eyey, eyez;
      float nox, noy, noz;
      eyex = distancia*sin(azimuth_rad)*cos(elevacion_rad);
      eyey = distancia*sin(azimuth_rad)*sin(elevacion_rad);
      eyez = distancia*cos(elevacion_rad);

      nox = distancia*cos(azimuth_rad)*cos(elevacion_rad);
      noy = distancia*cos(azimuth_rad)*sin(elevacion_rad);
      noz = -distancia*sin(elevacion_rad);
      */

      glMatrixMode(GL_PROJECTION);
      gluPerspective (50.0,float(PanelWidth)/float(PanelHeight), 100.0, 2*distancia);
      glMatrixMode(GL_MODELVIEW);

      //gluLookAt(eyex,eyey,eyez , 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);

      //glTranslated(-medX, -medY, -medZ);

      //glTranslated(xTrans, yTrans, 0.0);

      //glRotated(0.0, 0.0, 0.0, 1.0);
      //glRotated(elevacion, 1.0, 0.0, 0.0);
      //glRotated(azimuth, 0.0, 0.0, 1.0);

      //Escalamiento:
     // glScalef(res[0],res[1],res[2]);

     // DrawObjects();

      //glTranslated(-medX,-medY,-medZ);
      //glScalef(res[0],res[1],res[2]);

    //  glFlush();

}
//------------------------------------------------------------------------------

void __fastcall TForm1::RenderGLScene()
{
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      if(azimuth<0)
      azimuth=360+azimuth;
      if(azimuth>360)
      azimuth=azimuth-360;

      if(elevacion<=0)
      elevacion=0;
      if(elevacion>=180)
      elevacion=180;

      ValorElLabel->Caption = elevacion;
      ValorAzLabel->Caption = azimuth;
      ValorDistLabel->Caption = distancia;

      float elevacion_rad = float(elevacion/180.0*M_PI);
      float azimuth_rad = float(azimuth/180.0*M_PI);

      float eyex, eyey, eyez;
      float nox, noy, noz;
      float distancial = 1000.0;

      eyex = distancial*sin(elevacion_rad)*cos(azimuth_rad);
      eyey = distancial*sin(elevacion_rad)*sin(azimuth_rad);
      eyez = distancial*cos(elevacion_rad);

      if(ZnormCheckBox->Checked)
      {
      nox = distancial*cos(elevacion_rad)*cos(azimuth_rad);
      noy = distancial*cos(elevacion_rad)*sin(azimuth_rad);
      noz = -distancial*sin(elevacion_rad);
      }
      else
      {
      nox = -distancial*cos(elevacion_rad)*cos(azimuth_rad);
      noy = -distancial*cos(elevacion_rad)*sin(azimuth_rad);
      noz = distancial*sin(elevacion_rad);
      }

      float tx,ty,tz;

      tx = noy*eyez-noz*eyey;
      ty = noz*eyex-nox*eyez;
      tz = nox*eyey-noy*eyex;

      tx = tx-eyex;
      ty = ty-eyey;
      tz = tz-eyez;

      float norma_ = sqrt(tx*tx+ty*ty+tz*tz);

      tx /= norma_;
      ty /= norma_;
      tz /= norma_;

      norma_ = sqrt(nox*nox+noy*noy+noz*noz);

      nox /= norma_;
      noy /= norma_;
      noz /= norma_;

      float fov = 25.0/1000.0*distancia;

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      //Change Perspective ("depth" sensation):
      gluPerspective (fov,float(PanelWidth)/float(PanelHeight), 0.01, 2*distancial);

      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      //Camera positioning:
      gluLookAt(eyey+ty*xTrans-noy*yTrans, eyex+tx*xTrans-nox*yTrans,
                eyez+tz*xTrans-noz*yTrans,
                ty*xTrans-noy*yTrans, tx*xTrans-nox*yTrans,
                tz*xTrans-noz*yTrans,
                nox, noy, noz);

      //Object transformation:
      //Move and scale the object:
      glScalef(res[0],res[1],res[2]);
      glTranslatef(-medX, -medY, -medZ);
      //glTranslatef(xTrans ,yTrans , 0.0);

      DrawObjects();
      glFlush();
}
//------------------------------------------------------------------------------
void __fastcall TForm1::DrawObjects()
{
         //if(RenderFlag && TabSheet->ActivePage==ROISheet && mostrarTex3D)
         if(TabSheet->ActivePage==ROISheet && mostrarTex3D)
         {
         //drawaxes(distY,distX,distZ);
         if(CLCheckBox->Checked)
         {
         drawAortaCenterLine(ManualCenterLine, res[0], res[1], res[2]);
         drawAortaCenterLine(UFCSpline,        res[0], res[1], res[2]);
         }

         Mesh* myMesh;

         if(meshOK)
         {
            for(int i=0; i<Meshes->Count; ++i)
            {
                myMesh = (Mesh*)Meshes->Items[i];
                myMesh->RefreshMesh();
                drawMesh(myMesh->get_face_list(), myMesh->color);
                //drawNodes(myMesh->getNodeList());
            }

         if(TresDCheckBox->Checked)
         MostrarVolumenTexturas3D();

         //drawCarina(yCarina, xCarina, zCarina);
         }

         if(isCenterLineDetected)
         drawAortaCenterLine(AortaCenterLine,  res[0], res[1], res[2]);

         //Dibuja todo el modelo aortico:
         if(isAortaSegmented)
         drawAortaRadius(AortaCenterLine, AortaNormales, AortaRadios, res[0], res[1], res[2]);

         //Dibuja el CSA en la posición del plano oblicuo:
         if(isAortaSegmented)
         drawAortaRadius(AortaCenterLine, AortaNormales, AortaRadios, res[0], res[1], res[2], CSATrackBar->Position);

         //Dibuja los landmarks aórticos:
         if(AnnulusCheckBox->Checked && isAortaSegmented)
         drawAortaRadius(AortaCenterLine, AortaNormales, AortaRadios, res[0], res[1], res[2], annulusIndex, "1");

         if(CACheckBox->Checked&& isAortaSegmented)
         drawAortaRadius(AortaCenterLine, AortaNormales, AortaRadios, res[0], res[1], res[2], CAIndex, "2");

         if(BCACheckBox->Checked&& isAortaSegmented)
         drawAortaRadius(AortaCenterLine, AortaNormales, AortaRadios, res[0], res[1], res[2], BCAIndex, "3");

         if(LSACheckBox->Checked&& isAortaSegmented)
         drawAortaRadius(AortaCenterLine, AortaNormales, AortaRadios, res[0], res[1], res[2], LSAIndex, "4");

         if(CDCheckBox->Checked&& isAortaSegmented)
         drawAortaRadius(AortaCenterLine, AortaNormales, AortaRadios, res[0], res[1], res[2], CDIndex, "5");

         if(ApexSinusCheckBox->Checked&& isAortaSegmented)
         drawAortaRadius(AortaCenterLine, AortaNormales, AortaRadios, res[0], res[1], res[2], apexSinusIndex, "6");

         if(DiaphragmCheckBox->Checked&& isAortaSegmented)
         drawAortaRadius(AortaCenterLine, AortaNormales, AortaRadios, res[0], res[1], res[2], diaphragmIndex, "7");

         drawCircle(yCA, xCA, zCarina, rA, 0, 0, 255);
         drawCircle(yCD, xCD, zCarina, rD, 255, 0, 0);
         }

         
         /*
         //Habilita Depth Test (z-buffer)*****************
         glEnable(GL_DEPTH_TEST);

        //glDepthMask(GL_TRUE);

         glEnable(GL_BLEND);
        // glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_SRC_COLOR);
        //glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_DST_COLOR);
        // glBlendFunc(GL_ONE, GL_DST_COLOR);
        glBlendFunc(GL_SRC_COLOR,GL_ONE_MINUS_SRC_COLOR);

         glEnable(GL_TEXTURE_2D);
         glEnable(GL_TEXTURE_1D);

         if(!CheckBox2->Checked)
         {
         for(int i=0; i<cantTexturas; i++)
         drawAllTextures(anchoPX, altoPX, cantImagenes, texturas[i], i, lutex);
         }
         else
         {

         for(int i=cantTexturas-1; i>=0; i--)
         drawAllTextures(anchoPX, altoPX, cantImagenes, texturas[i], i, lutex);
         }

         glDisable(GL_TEXTURE_2D);
         glDisable(GL_TEXTURE_1D);

        glDisable(GL_BLEND);
         //drawPlanoOblic(PlanoCorte,anchoPX,altoPX,texturaOB);

         glTranslatef(0,0,200);

         /*glColor3f(1.0,1.0,0.0);
         glPushMatrix();
         glTranslatef(256-C1.x,C1.y-256,256-C1.z);
         glutSolidCube(20);
         glPopMatrix();

         glColor3f(0.0,1.0,1.0);
         glPushMatrix();
         glTranslatef(256-C2.x,C2.y-256,256-C2.z);
         glutSolidCube(20);
         glPopMatrix();

         glColor3f(0.4,0.8,0.4);
         glPushMatrix();
         glTranslatef(256-Cc.x,Cc.y-256,256-Cc.z);
         glutSolidCube(20);
         glPopMatrix();

          glLineWidth(5.0f);
          glColor3f(1.0,1.0,1.0);
          glLineStipple(3, 0xAAAA);
          glEnable(GL_LINE_STIPPLE);
          glBegin(GL_LINE_STRIP);
          glVertex3f(256-C1.x,C1.y-256,256-C1.z);
          glVertex3f(256-C2.x,C2.y-256,256-C2.z);
          glEnd();
          glDisable(GL_LINE_STIPPLE); */

           /*
           if(dibujaCils)
           drawCils(CilindrosList, medX, medY, medZ);

           if(dibujaEsfs)
           drawEsfs(EsferasList, medX, medY, medZ);

           if(RotandoImagen + Aumentando + Transladando)
             paso = atoi(PasoEdit->Text.c_str());
           else
             paso = PasoTrackBar->Position;

           if(distancia!=0)
           tamanioPixel = paso*atof(PxDistEdit->Text.c_str())/float(distancia);

           if(!EsconderCheckBox->Checked)
             if(SelectedPoints->Count)
               drawPuntosColor(SelectedPoints,LUT,LUT_Hueso, LUT_Musculo, LUT_Sangre,
                               medX, medY, medZ, minPX, tamanioPixel, offset, SelectedPoints->Count,
                               paso, AtenuacionTrackBar->Position);

          if(SmoothCheckBox->Checked)
            glEnable(GL_POINT_SMOOTH);
          else
            glDisable(GL_POINT_SMOOTH);

         if(!EsconderCheckBox->Checked)
          if(RenderPoints->Count && !CheckBox1->Checked)
          {
             drawPuntos(RenderPoints,LUT,LUT_Hueso, LUT_Musculo, LUT_Sangre,
                        medX, medY, medZ, minPX, tamanioPixel, offset, final, paso, AtenuacionTrackBar->Position);
          }

         if(!EsconderCheckBox->Checked)
          if(RenderPoints->Count && CheckBox1->Checked)
          {
          drawPuntosGrayScale(RenderPoints,LUT,LUT_Hueso, LUT_Musculo, LUT_Sangre,
                              medX, medY, medZ, minPX, tamanioPixel, offset, final, paso, AtenuacionTrackBar->Position);
          }

         if(LabelForm1->VolRamaCheckBox)
          if(VolRamaList->Count)
          {
           drawVolRama(VolRamaList,LUT,LUT_Hueso, LUT_Musculo, LUT_Sangre,
                       medX, medY, medZ, minPX, tamanioPixel, offset, final, paso, AtenuacionTrackBar->Position, maxPuntosRama, minPuntosRama,
                       LargosRama, LargosSmoothRama, VolumenRama);
          }

         if(BorderPoints->Count)
           drawBordes(BorderPoints,
           medX, medY, medZ, minPX, tamanioPixel, float(MCFaceOpacityTrackBar->Position)/10.0);

          if(ParedesCheckBox->Checked)
           drawWalls(distX,distY,distZ);

          if(TriangList->Count)
            if((RotandoImagen+Aumentando+Transladando))
            {
              if(azimuth<180)
               drawMC(TriList, TriangList, Normals, M,LUT, medX, medY, medZ,minPX,true, 2,
                      float(MCFaceOpacityTrackBar->Position)/10.0);
              else
               drawMC(TriList, TriangList, Normals, M,LUT, medX, medY, medZ,minPX,false, 2,
                      float(MCFaceOpacityTrackBar->Position)/10.0);
            }
            else
              if(azimuth<180)
               drawMC(TriList, TriangList, Normals, M,LUT, medX, medY, medZ,minPX,true,1,
                      float(MCFaceOpacityTrackBar->Position)/10.0);
              else
               drawMC(TriList, TriangList, Normals, M,LUT, medX, medY, medZ,minPX,false,1,
                      float(MCFaceOpacityTrackBar->Position)/10.0);

         drawaxes(distX,distY,distZ);

         drawVoxels(EsquelPoints,EP,LP,JP,BP,EraseP,
                    NodeList,LabelForm1->NodeListBox->ItemIndex,
                    RamaList,LabelForm1->RamaListBox->ItemIndex,
                    matrizNodoRama, NodeList->Count, RamaList->Count,
                    medX,medY,medZ, EsqOnTopCheckBox->Checked);
         }

         if(TabSheet->ActivePage==OrthoViewSheet && ImagenEnPantalla)
         {
        // drawMarcos(XScrollBar,YScrollBar,ZScrollBar,anchoPX,altoPX,cantImagenes,
        //           textura1,textura2,textura3);


          //Habilita Depth Test (z-buffer)*****************
         glEnable(GL_DEPTH_TEST);
         glEnable(GL_BLEND);
        // glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_SRC_COLOR);
        glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_DST_COLOR);

         glEnable(GL_TEXTURE_2D);
         for(int i=0; i<cantTexturas; i++)
         drawAllTextures(anchoPX, altoPX, cantImagenes, texturas[i], i, lutex);
         glDisable(GL_TEXTURE_2D);

        glDisable(GL_BLEND);
         //drawPlanoOblic(PlanoCorte,anchoPX,altoPX,texturaOB);

         glColor3f(1.0,1.0,0.0);
         glPushMatrix();
         glTranslatef(256-C1.x,C1.y-256,256-C1.z);
         glutSolidCube(20);
         glPopMatrix();

         glColor3f(0.0,1.0,1.0);
         glPushMatrix();
         glTranslatef(256-C2.x,C2.y-256,256-C2.z);
         glutSolidCube(20);
         glPopMatrix();

         glColor3f(0.4,0.8,0.4);
         glPushMatrix();
         glTranslatef(256-Cc.x,Cc.y-256,256-Cc.z);
         glutSolidCube(20);
         glPopMatrix();

          glLineWidth(5.0f);
          glColor3f(1.0,1.0,1.0);
          glLineStipple(3, 0xAAAA);
          glEnable(GL_LINE_STIPPLE);
          glBegin(GL_LINE_STRIP);
          glVertex3f(256-C1.x,C1.y-256,256-C1.z);
          glVertex3f(256-C2.x,C2.y-256,256-C2.z);
          glEnd();
          glDisable(GL_LINE_STIPPLE);

        /*
         glBegin(GL_POINTS);
         for(int i=0; i<altoPX; i++)
          for(int j=0; j<anchoPX; j++)
          glVertex3f(256-PlanoCorte[i][j].x,PlanoCorte[i][j].y-256,256-PlanoCorte[i][j].z);
         glEnd();
        */

      /* }


       if(drawAorta)
       {
       drawAortaCenterLine(AortaCenterLine,medX, medY, medZ);
       }
       */
}
//------------------------------------------------------------------------------
void __fastcall TForm1::XYButtonClick(TObject *Sender)
{
            azimuth=0;
            elevacion=0;
}
//------------------------------------------------------------------------------
void __fastcall TForm1::AbrirVolumen1Click(TObject *Sender)
{
      //Si ya hay un archivo abierto:
      if(ImagenEnPantalla)
      {
      Cerrar1Click(Sender);
      }

      OpenDialog1->Filter = "BIN files (*.bin)|*.BIN|Dicom files (*.dcm)|*.DCM |VOL files (*.vol)|*.VOL| Any file (*.*)| *.*";

      if( !OpenDialog1->Execute() )
      return;

      //Archivo para abrir:
      FILE* fp;
      fp = NULL;
      AnsiString FullFileName = OpenDialog1->FileName;
      AnsiString VolFullPath;

      //Reconoce la forma de abrir según sea .bin o .dcm:
      if(ExtractFileExt(FullFileName)!=".bin" && ExtractFileExt(FullFileName)!=".vol")
      {
      int resultado;
      /*
      dicomPath: path donde se encuentran los archivos .DCM (siempre poner la barra "\" al final)
      targetPath: ruta del archivo binario (siempre poner la barra "\" al final)
      binFileName: nombre con el que se genera el archivo binario.
      type: tipo de estudio 'T' - tomografias / 'R' - resonancias
      */
      AnsiString FolderPath = ExtractFileDir(FullFileName);
      resultado = createBinFile_ext(FolderPath.c_str(), FolderPath.c_str() ,"\\volumen.bin",'T');
      VolFullPath = FolderPath + "\\volumen.bin"  ;
      }
      else VolFullPath = OpenDialog1->FileName;


         //FilePath:
         CarpetaLabel->Caption=ExtractFilePath(VolFullPath);
         ArchivoLabel->Caption=ExtractFileName(VolFullPath);

         //Apertura de los volumenes .bin:
         fp = fopen( VolFullPath.c_str(),  "r+b");

         encabezado enc1;
         //size_t fread(void *ptr, size_t num_bytes, size_t count, FILE *stream);

         //Lee la cantidad de imagenes y el valor de frame inicial: sirve para
         //archivos incompletos cuyo minframe != 0:

         fread(&cantImagenes,sizeof(short int),1,fp);
         fread(&minFrame,sizeof(short int),1,fp);

         //Lee el primer encabezado, de donde extrae los valores de minPX y maxPX:
         //También se usa para obtener la resolución de un corte del estudio "rX" y "rY":
         fread(&enc1,sizeof(encabezado),1,fp);
         maxPX = enc1.max;
         minPX = enc1.min;
         realminPX = minPX;

        //indica si se encontró el corte sucesivo a primercorte, para
        //calcular la distancia entre cortes "rZ":
        bool encontradoSucesivo = false;

       //Damian://maxPX=0;
                //minPX=0;

        //Obtiene las dimensiones del estudio del primer encabezado:
        anchoPX = enc1.ancho;
        altoPX = enc1.alto;
        res[0]=enc1.dX;
        res[1]=enc1.dY;
        //Obtiene dZ como la posición del primer corte: 
        res[2]=enc1.dZ;
        
        RXEdit->Text = res[0];
        RYEdit->Text = res[1];

        //Creación del volumen que va a contener a las imágenes:
        M = new Volumen(anchoPX,altoPX,cantImagenes);

        int corte = enc1.frame /*Damian: +1*/;

        int primerCorte = corte;

        ProcessLabel->Visible = true;
        ProcessLabel->Caption = "Cargando Estudio...";
        ProcessLabel->Refresh();
        ProcessLabel->Repaint();

        ProgressBar1->Max=cantImagenes;
        ImageScrollBar->Enabled = true;

        ImageScrollBar->Min=0;
        ImageScrollBar->Max=cantImagenes-1;

        firstRender = true;

       //Apertura de los archivos sucesivos, fila X fila, NO elemento X elemento:
       short int valoraux;

       for (int k=0;k<cantImagenes;k++)
       {
       ProgressBar1->Position=k;
        for(int i=0; i<enc1.ancho;i++)
                       fread(M->V[corte-minFrame][i],enc1.alto*sizeof(short int),1,fp);

        fread(&enc1,sizeof(encabezado),1,fp);
        corte = enc1.frame;

        if(corte==primerCorte+1 && !encontradoSucesivo)
        {
        res[2]=fabs(res[2]-enc1.dZ);

        if(res[2]==0)  //Implica que se grabó de un estudio previamente
        res[2]=enc1.dZ;//pasado a .bin
      
        RZEdit->Text = res[2];

        encontradoSucesivo = true;
        }

        if(enc1.max>maxPX)
         maxPX = enc1.max;
       }

       //Cierra el archivo:
       fclose( fp );

       //Carga las imágenes dentro del Volumen Principal, "M":
        for (int i=0; i<enc1.alto; i++) for (int j=0; j<enc1.alto; j++) for (int k=0; k<cantImagenes; k++)
        M->SetV(i,j,k, M->GetV(i,j,k));

        /*{M[i][j][k]=M[i][j][k]/*Damian:-16384*//*Damian:if(M[i][j][k]==-16384)M[i][j][k]=-2048;}*/
        /* Damian: minPX = -2048; maxPX = maxPX-16384;*/

        //Resetea el ProgressBar:
        ProgressBar1->Position=0;
        ProcessLabel->Visible = false;


        //Preparacion de las vistas ortogonales:
        /*
        //.....................................................
        CuadroXY = new Matriz(XYImage->Width, XYImage->Height);
        YScrollBar->Max = altoPX-1;
        YScrollBar->Position = YScrollBar->Max/2;

        CuadroYZ = new Matriz(YZImage->Width, YZImage->Height);
        ZScrollBar->Max = cantImagenes-1;
        ZScrollBar->Position = ZScrollBar->Max/2;

        CuadroZX = new Matriz(ZXImage->Width, ZXImage->Height);
        XScrollBar->Max = anchoPX-1;
        XScrollBar->Position = XScrollBar->Max/2;

        CuadroZoom = new Matriz(ZoomImage->Width, ZoomImage->Height);


        OrthoViewSheet->Enabled = true;

        ZoomRadioGroup->ItemIndex = 0;  */
        //.....................................................

        CuadroCorteOblicuo = new Matriz(ArbitraryCutForm1->Image1->Width,
                                        ArbitraryCutForm1->Image1->Height);

        Pantalla->Visible=true;
        ImagenEnPantalla=true;

        //ROICheckBox->Enabled=true;
        //ROI1->Enabled = true;

        MaxPxLabel->Caption=maxPX;
        MinPxLabel->Caption=minPX;

        //CAC = new bool[altoPX*anchoPX];

        //Preparación de las LUTs:
        //*******************************************************
        //LUT B&N:
        LUT = new float[maxPX-minPX+1];

        //Inicialización de LUTs
        for(int i=0;i<maxPX-minPX;i++)
        {
            LUT[i]=0;
          //  LUT_Sangre[i]=0;
          //  LUT_Musculo[i]=0;
          //  LUT_Hueso[i]=0;
        }


        /*
        for(int i=100-minPX;i<400-minPX;i++)
        LUT_Sangre[i]=205.0/300.0 * (i-(100-minPX))+50.0;
        for(int i=400-minPX;i<600-minPX;i++)
        LUT_Sangre[i]=205.0 -205.0/300.0 * (i-(400-minPX))+50.0;

        for(int i=-600-minPX;i<0.0-minPX;i++)
        LUT_Musculo[i]=255.0/600.0 *(i-(-600-minPX));
        for(int i=0.0-minPX;i<800-minPX;i++)
        LUT_Musculo[i]=255.0 -255.0/800.0 * (i-(0.0-minPX));

        for(int i=100-minPX;i<600-minPX;i++)
        LUT_Hueso[i]=155.0/500.0 *(i-(100-minPX))+100.0;
        for(int i=600-minPX;i<900-minPX;i++)
        LUT_Hueso[i]=200.0;
        for(int i=900-minPX;i<maxPX-minPX;i++)
        LUT_Hueso[i]=255.0;
        //*********************************************************
         */


        /*
        AtenuacionTrackBar->Max = maxPX-minPX;
        AtenuacionTrackBar->Min = 1;
        AtenuacionTrackBar->Position = (maxPX-minPX)/2;
        */

       if(ImagenEnPantalla)
       {
        if(TabSheet->ActivePage==ROISheet)
        {
         TabSheetChange(Sender);
        }

        HistoChart->Enabled = true;
        CropCheckBox->Enabled   = true;

        //Reseteo las series del HistoChart:
        HistoChart->Series[0]->Clear();
        HistoChart->Series[1]->Clear();

        HistoChart->Series[1]->AddXY(Center,100000,"",clRed);//double(histoCaja[Center-minPX]),"",clRed);
        HistoChart->Series[1]->AddXY(Ls,100000,"",clBlue);//double(histoCaja[Ls-minPX]),"",clBlue);
        HistoChart->Series[1]->AddXY(Li,100000,"",clBlue);//double(histoCaja[Li-minPX]),"",clBlue);

        //Ajusta los trackbars que modifican Width y Height

        //Cargar primer imagen en pantalla:
        Cuadro = new Matriz(anchoPX,altoPX);

        CenterTrackBar->Enabled = true;
        WidthTrackBar->Enabled  = true;
        CenterEdit->Enabled     = true;
        WidthEdit->Enabled      = true;

        CenterTrackBar->OnChange = NULL;
        CenterTrackBar->Min=realminPX;
        CenterTrackBar->Max=maxPX;
        CenterTrackBar->Position=int((maxPX+realminPX)/2);
        Center = CenterTrackBar->Position;
        CenterEdit->OnChange = NULL;
        CenterEdit->Text = Center;
        CenterEdit->OnChange = CenterEditChange;
        CenterTrackBar->OnChange = CenterTrackBarChange;

        WidthTrackBar->OnChange = NULL;
        WidthTrackBar->Min=1;
        WidthTrackBar->Max=int((maxPX-realminPX)/2);
        WidthTrackBar->Position=int((maxPX-realminPX)/2);
        Width =  WidthTrackBar->Position;
        WidthEdit->OnChange = NULL;
        WidthEdit->Text = Width;
        CenterEdit->OnChange = WidthEditChange;
        WidthTrackBar->OnChange = WidthTrackBarChange;

        //Creación de la LUT:
        RefreshLUT();

       //Llena el SliceListBox con los índices de las imágenes:
       for (int k=0;k<cantImagenes;k++)
       SliceListBox->Items->Strings[k]=k;

       SliceListBox->ItemIndex=0;

       //int zindex = atoi(SliceListBox->Items->Strings[0].c_str());

       //RefreshPantallaFiltros();

       //Inicia Thread Para Cálculo de Histogramas:

       //**********************************************************************
       histoCaja = new float[maxPX-minPX+1];

       histo_thread = new calcHistoThread(true);
       histo_thread->receiveParams(M, histoCaja, LUT, maxPX-minPX+1,
                                   minPX,HistoChart,HistoProcessLabel, &realminPX, &histo_thread_terminado);

       histo_thread->Resume();
       //***********************************************************************

      }

      //Creación de la Matriz de Agrupación para la Goma3D (MapaIndices) e
      //inicialización de matrices auxiliares para Marching Cubes:
      //************************************************************************

      /*
      IndicesBorrar = new TList;
      for(int i=0; i<750; i++)
        for(int j=0; j<650; j++)
          MapaIndices[i][j] = new TList;

      TriangList = new TList;
      TriangColors = new TList;
      Normals =  new TList;

      TriList = new TList;  */
      //************************************************************************

      PC1.x=float(altoPX)/2.0;
      PC1.y=float(anchoPX)/2.0;
      PC1.z=0;

      PlanoCorte=new PuntoF* [altoPX];
      for (int i=0; i<altoPX; ++i)
      {
        PlanoCorte[i]=new PuntoF[anchoPX];
      }

      PlanoOrig=new PuntoF* [altoPX];

      for (int i=0; i<altoPX; ++i)
      {
        PlanoOrig[i]=new PuntoF[anchoPX];
      }

      for(int i=0; i<altoPX; i++)
        for(int j=0; j<anchoPX; j++)
        {
            PlanoOrig[i][j].x=i;
            PlanoOrig[i][j].y=j;
            PlanoOrig[i][j].z=0;
        }


      vector1.x=1;
      vector1.y=0;
      vector1.z=0;
      //************************************************************************

      volVoxel = res[0]*res[1]*res[2];

      medX = anchoPX/2.0;
      medY = altoPX/2.0;
      medZ = cantImagenes/2.0;

      CarinaButton->Enabled = true;

      CSATrackBar->Enabled = false;
      CACDButton->Enabled = false;
      CreateToroidButton->Enabled = false;
      SegmentDAButton->Enabled = false;
      SegmentArchButton->Enabled = false;
      OrthoCorrectionButton->Enabled = false;

      AorticRegionsGroupBox->Enabled = false;

      GraficarEnPantalla();
}
//------------------------------------------------------------------------------
void __fastcall TForm1::SliceListBoxClick(TObject *Sender)
{
        ImageScrollBar->Position=(SliceListBox->ItemIndex);
}
//------------------------------------------------------------------------------

void __fastcall TForm1::ImageScrollBarChange(TObject *Sender)
{
        SliceListBox->ItemIndex=ImageScrollBar->Position;

        //Si ya pasó por la detección de la carina, la nueva posición será
        //tomada como una corrección manual de este valor:
        if(carinaDetectada)
        zCarina = ImageScrollBar->Position;

        zP = ImageScrollBar->Position;
        GraficarEnPantalla();

}
//------------------------------------------------------------------------------

void __fastcall TForm1::GraficarEnPantalla()
{
      int zindex = atoi(SliceListBox->Items->Strings[SliceListBox->ItemIndex].c_str());

      if(CACCheckBox->Checked)
      {
      TranspImage->Left=Pantalla->Left;
      TranspImage->Top=Pantalla->Top;
      TranspImage->Width=Pantalla->Width;
      TranspImage->Height=Pantalla->Height;
      }

      if(CACCheckBox->Checked)
       for(int i=0; i<anchoPX;i++)
          for(int j=0; j<altoPX;j++)
          CAC[i+j*512]=0;


               for(int i=0; i<anchoPX;i++)
                 for(int j=0; j<altoPX;j++)
                 {
                 if(M->GetV(i,j,zindex)>= minPX && M->GetV(i,j,zindex)<= maxPX)
                 Cuadro->buffer(i,j)=LUT[M->GetV(i,j,zindex)-minPX];
                 if(M->GetV(i,j,zindex)> maxPX)
                 Cuadro->buffer(i,j)=LUT[maxPX-minPX];
                 if(M->GetV(i,j,zindex)< minPX)
                 Cuadro->buffer(i,j)=LUT[0];

                   if(CACCheckBox->Checked)
                    if(M->GetV(i,j,zindex)>=CACGS) //Gold Standard de Umbral para CAC definido arriba
                    {
                    CAC[i+j*512]=true;
                    }
                 }

                 Cuadro->Graficar(Pantalla);

               // if(CACCheckBox->Checked)
               //  Cuadro->Graficar(TranspImage, 255, 255, 0, CAC);
}
//------------------------------------------------------------------------------
void __fastcall TForm1::NegativoCheckBoxClick(TObject *Sender)
{
        for(int i=0;i<maxPX-minPX+1;i++)
        {
        LUT[i] = 255.0-LUT[i];
        if(LUT[i]<0)
        LUT[i]=0;

        if(LUT[i]>255.0)
        LUT[i]=255.0;
        }

        GraficarEnPantalla();
}
//------------------------------------------------------------------------------
void __fastcall TForm1::Mediana1Click(TObject *Sender)
{
        FiltroMediana(MToroide);
}
//------------------------------------------------------------------------------

void __fastcall TForm1::AperturaClick(TObject *Sender)
{
        //Apertura:
        float r0 = res[0]*10.0; //1cm

        AnsiString radioString = r0;
        int r;

       //if(InputQuery("Ingrese Radio del Elemento Estructurante","Radio:",radioString))
       //{
       // r = atoi(radioString.c_str());
       //}else {return;}
       
        r = r0;

        //Realiza filtro de apertura morfológica dentro de la ROI:
        //Realiza filtro de mediana dentro de la ROI:
        int zf=ImageScrollBar->Position+1;
        int zi=ImageScrollBar->Position;

        if(zf==ImageScrollBar->Max)
        return;

        int xi = ROIShape->Top;
        int xf = ROIShape->Top+ROIShape->Height;
        int yi = ROIShape->Left;
        int yf = ROIShape->Left+ROIShape->Width;

      //  ProgressBar1->Max = zf-zi+1;

        //Listas y arrays auxiliares para la creación del elemento estructurante
        //particionado circular de radio=r
       TList* Mascara = new TList;

       strel2D(r, Mascara);

       Volumen* volAux = new Volumen(altoPX, anchoPX, cantImagenes);

       short int max = minPX;
       short int min = maxPX;

       Punto *paux;

      for (int k=zi;k<zf;k++)
         for(int j=yi+r; j<yf-r;j++)
            for(int i=xi+r; i<xf-r;i++)
            {

             for(int s = 0; s<Mascara->Count; s++)
             {
              paux = (Punto*)Mascara->Items[s];

               if(M->GetV(i+paux->x, j+paux->y, k)<min)
               {
               min = M->GetV(i+paux->x, j+paux->y, k);
               }

             }

             volAux->SetV(i,j,k,min);

             min = maxPX;
            }



        for (int k=zi;k<zf;k++)
         for(int j=yi+r; j<yf-r;j++)
            for(int i=xi+r; i<xf-r;i++)
            {

             for(int s = 0; s<Mascara->Count; s++)
             {
              paux = (Punto*)Mascara->Items[s];

               if(volAux->GetV(i+paux->x, j+paux->y, k)>max)
               {
               max = volAux->GetV(i+paux->x, j+paux->y, k);
               }

             }

             M->SetV(i,j,k,max);

             max = minPX;
            }

    // GraficarEnPantalla();

        for(int i=0; i<Mascara->Count; i++)
               delete Mascara->Items[0];

        delete Mascara;

        delete volAux;
        volAux = NULL;
}
//------------------------------------------------------------------------------

void __fastcall TForm1::PantallaMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int Y, int X)
{
                xP = X;
                yP = Y;

                xCarina = X;
                yCarina = Y;

                meanC = 0;
                stdC  = 0;

       //Cálculo de media y desvío dentro de la malla inicial:
       double acumV=0;
       double acumV2=0;
       double cont=0;

       //media y desvio inicial:
       for(int i= xP- 10; i< xP+10; i++)
         for(int j= yP-10; j< yP+10; j++)
          for(int k= zP-10; k< zP+10; k++)
          {
           if( ( (i-xP)*(i-xP)+(j-yP)*(j-yP)+(k-zP)*(k-zP) ) <25 )
           {
           acumV+=  double(M->GetV(i,j,k));
           acumV2+= double(M->GetV(i,j,k))*double(M->GetV(i,j,k));
           cont++;
           }
          }

          meanC= acumV/cont++;
          stdC = sqrt((acumV2 - 2*meanC*acumV + cont*meanC*meanC)/cont);

          if(RGCheckBox->Checked)
          fastfloodfill3D( M, Center+Width, Center-Width, maxPX, xP, yP, zP );

          if(CLCheckBox->Checked)
          {
          PuntoF* nP = new PuntoF;
          nP->x = X;
          nP->y = Y;
          nP->z = ImageScrollBar->Position;
          ManualCenterLine->Add(nP);
          nP = NULL;
          }

          GraficarEnPantalla();

}
//------------------------------------------------------------------------------
void __fastcall TForm1::GuardarVolumen1Click(TObject *Sender)
{
        if(SaveDialog1->Execute())
        {
         FILE* fp;
         AnsiString FileName = SaveDialog1->FileName;

         if(!(FileName.AnsiPos(".bin") || FileName.AnsiPos(".BIN")))
         FileName=FileName+".bin";

         fp=fopen(FileName.c_str(),"w+b");

         ProgressBar1->Max=cantImagenes;
         ProgressBar1->Position=0;

        encabezado enc1;

       //size_t fread(void *ptr, size_t num_bytes, size_t count, FILE *stream);
       fwrite(&cantImagenes,sizeof(short int),1,fp);
       fwrite(&minFrame,sizeof(short int),1,fp);

       enc1.dX=res[0];
       enc1.dY=res[1];
       enc1.dZ=res[2];

       enc1.ancho=anchoPX;
       enc1.alto=altoPX;
       enc1.min=minPX;
       enc1.max=maxPX;

       for (int k=0; k<cantImagenes; ++k)
       {
       ProgressBar1->Position=k;
       enc1.frame=k+minFrame;
       fwrite(&enc1,sizeof(encabezado),1,fp);
         for (int i=0; i<enc1.ancho; ++i)
           for (int j=0; j<enc1.alto; ++j)
             {
             fwrite(&M->V[k][i][j],sizeof(short int),1,fp);
             if(M->GetV(i,j,k)>maxPX)
             enc1.max = M->GetV(i,j,k);
             }
       }
        fclose( fp );
           ProgressBar1->Position=0;
     }
}
//------------------------------------------------------------------------------

void __fastcall TForm1::Histograma1Click(TObject *Sender)
{
     if(ROIShape->Visible == true)
     {

       int zf=atoi(FinSliceEdit->Text.c_str());
        int zi=atoi(IniSliceEdit->Text.c_str());

        int xi = ROIShape->Top;
        int xf = ROIShape->Top+ROIShape->Height;
        int yi = ROIShape->Left;
        int yf = ROIShape->Left+ROIShape->Width;

       float maxROI = minPX;
       float minROI = maxPX;

       for(int k=zi;k<zf;k++)
           for(int i=xi; i<xf;i++)
                 for(int j=yi; j<yf;j++)
                   {
                   if(M->GetV(i,j,k)<minROI)
                   minROI = M->GetV(i,j,k);

                   if(M->GetV(i,j,k)>maxROI)
                   maxROI = M->GetV(i,j,k);
                   }


        int* HistoArray = new int[maxROI-minROI];

                 for(int h=0; h<maxROI-minROI; h++)
                        HistoArray[h]=0;

                 for(int k=zi;k<zf;k++)
                        for(int i=xi; i<xf;i++)
                                for(int j=yi; j<yf;j++)
                                {
                                int indice = M->GetV(i,j,k);
                                HistoArray[indice-int(minROI)]++;
                                }


                for(int h=0; h<maxROI-minROI; h++)
                {
                ProgressBar1->Position=h;
                AP->Items->Add(AnsiString(h+minROI)+","+AnsiString(HistoArray[h]));
                }

                AP->Items->SaveToFile("HistoResult.dat");
                ProgressBar1->Position=0;

        delete HistoArray;
      }
}
//------------------------------------------------------------------------------

void __fastcall TForm1::Resta1Click(TObject *Sender)
{
        int n=0;
        AnsiString nn = "2";
        if(InputQuery("Iresta = Iactual-n*Icargada","n:",nn))
        {
        n = atoi(nn.c_str());
        }else {return;}

        OpenDialog1->Filter = "BIN files (*.bin)|*.BIN";
       if(OpenDialog1->Execute())
       {
       FILE* fp;
       fp = fopen( OpenDialog1->FileName.c_str(),  "r+b");
       encabezado enc1;
       short int cortes;
       short int minFrame;
       //size_t fread(void *ptr, size_t num_bytes, size_t count, FILE *stream);

       fread(&cortes,sizeof(short int),1,fp);
       fread(&minFrame,sizeof(short int),1,fp);
       fread(&enc1,sizeof(encabezado),1,fp);

       short int ancho = enc1.ancho;
       short int alto = enc1.alto;
       short int corte = enc1.frame;

                if(cortes==cantImagenes && ancho==anchoPX && alto==altoPX)
                {
                ProgressBar1->Position=0;

                        for (int k=0;k<cantImagenes+1;k++)
                        {
                        ProgressBar1->Position=k;
                                for(int i=0; i<enc1.ancho;i++)
                                        for(int j=0; j<enc1.alto;j++)
                                        {
                                        short int valor;
                                        fread(&valor,sizeof(short int),1,fp);
                                        M->SetV(i,j,corte-minFrame,
                                        n*valor-n*(M->GetV(i,j,corte-minFrame) ));//+ abs(minPX)/2.0));
                                                //Evita Saturación:
                                                if(M->GetV(i,j,corte-minFrame)<minPX)
                                                M->SetV(i,j,corte-minFrame,minPX);
                                                if(M->GetV(i,j,corte-minFrame)>maxPX)
                                                M->SetV(i,j,corte-minFrame,maxPX);
                                        }

                        fread(&enc1,sizeof(encabezado),1,fp);
                        corte = enc1.frame;

                        }

                fclose( fp );
                ProgressBar1->Position=0;
                }
       }
}
//------------------------------------------------------------------------------



void __fastcall TForm1::GrabarAC1Click(TObject *Sender)
{
/*
             Punto* p = new Punto;

             for(int i=0; i<AAC->Count; i++)
             {
             p = (Punto*)AAC->Items[i];
             int x,y,z;
             x = p->x; y = p->y; z = p->z;

             AnsiString linea = AnsiString(x) + "," +  AnsiString(y) + "," + AnsiString(z);
             AP->Items->Add(linea);
             }
             AAC->Clear();
             AP->Items->SaveToFile("AAC.dat");
*/
        int zf,zi;

        if(FinSliceEdit->Text!="")
                zf=atoi(FinSliceEdit->Text.c_str());
        else zf=SliceListBox->ItemIndex;

        if(IniSliceEdit->Text!="")
                zi=atoi(IniSliceEdit->Text.c_str());
        else zi=SliceListBox->ItemIndex-1;

        int cantCortes = zf-zi;

        short int umbral;

        AnsiString umbralString;
        itoa(int(minPX),umbralString.c_str(),10);

        if(InputQuery("Ingrese umbral","Umbral:",umbralString))
        {
        umbral = atoi(umbralString.c_str());
        }else {return;}

if(ROIShape->Visible==true)
{
}else{return;}

        int xi = ROIShape->Top;
        int xf = ROIShape->Top+ROIShape->Height;
        int yi = ROIShape->Left;
        int yf = ROIShape->Left+ROIShape->Width;

//AP->Items->Clear();

        AnsiString linea;

        ProgressBar1->Max=cantCortes;
        ProgressBar1->Position=0;

        for (int k=zi;k<zf;k++)
        {ProgressBar1->Position++;
           for(int i=xi; i<xf;i++)
                 for(int j=yi; j<yf;j++)
                 {
                        if(M->GetV(i,j,k)>=umbral)
                        {
                        linea = AnsiString(i) + "," +  AnsiString(j) + "," + AnsiString(k);
                        AP->Items->Add(linea);
                        }
                 }
         }

         ProgressBar1->Position=0;

        AP->Items->SaveToFile("AAC.dat");
}
//------------------------------------------------------------------------------

void __fastcall TForm1::Segmentacion21Click(TObject *Sender)
{
        int zf,zi;

        if(FinSliceEdit->Text!="")
                zf=atoi(FinSliceEdit->Text.c_str());
        else zf=SliceListBox->ItemIndex;

        if(IniSliceEdit->Text!="")
                zi=atoi(IniSliceEdit->Text.c_str());
        else zi=SliceListBox->ItemIndex-1;

if(ROIShape->Visible==true)
{
}else{return;}

        int clases;
        AnsiString clasesString=3;

        if(InputQuery("Ingrese Cantidad de clases","Clases:",clasesString))
        {
        clases = atoi(clasesString.c_str());
        }else {return;}

        int xi = ROIShape->Top;
        int xf = ROIShape->Top+ROIShape->Height;
        int yi = ROIShape->Left;
        int yf = ROIShape->Left+ROIShape->Width;

        Volumen* volAux = new Volumen(xf-xi,yf-yi,zf-zi);

        for (int k=zi;k<zf;k++)
           for(int i=xi; i<xf;i++)
                 for(int j=yi; j<yf;j++)
                  volAux->SetV(i-xi,j-yi,k-zi,M->GetV(i,j,k));

        float Rango;

        //kmeans: clustering por diferencias:
        float* mu = new float[clases];
        float* desv = new float[clases];
        Rango = kmeans(volAux, clases, minPX, maxPX, mu, desv); //Maux-> se convierte en máscara de clases

        //Dibuja las clases sobre M
        for (int k=zi;k<zf;k++)
           for(int i=xi; i<xf;i++)
                 for(int j=yi; j<yf;j++)
                 {
                 float valor = float(volAux->GetV(i-xi,j-yi,k-zi)+1.0)/(float(clases)+1.0)*Rango+minPX;
                 M->SetV(i,j,k,int(valor));
                 }

        delete volAux;
        volAux = NULL;

        GraficarEnPantalla();
}
//------------------------------------------------------------------------------

void __fastcall TForm1::EliminarPulmon1Click(TObject *Sender)
{
        int zf,zi;

        if(FinSliceEdit->Text!="")
                zf=atoi(FinSliceEdit->Text.c_str());
        else zf=SliceListBox->ItemIndex;

        if(IniSliceEdit->Text!="")
                zi=atoi(IniSliceEdit->Text.c_str());
        else zi=SliceListBox->ItemIndex-1;

        if(!ROIShape->Visible)
         return;

        int clases=3;

        int xi = ROIShape->Top;
        int xf = ROIShape->Top+ROIShape->Height;
        int yi = ROIShape->Left;
        int yf = ROIShape->Left+ROIShape->Width;

        Volumen* volAux = new Volumen(xf-xi,yf-yi,zf-zi);

        //Cargar Matriz Auxiliar:
        for (int k=zi;k<zf;k++)
           for(int i=xi; i<xf;i++)
                 for(int j=yi; j<yf;j++)
                 {
                     volAux->SetV(i-xi,j-yi,k-zi,M->GetV(i,j,k));
                 }

                 float Rango;

        //kmeans: clustering por diferencias:
        float* mu = new float[clases];
          float* desv = new float[clases];
            Rango = kmeans(volAux, clases, maxPX, minPX, mu, desv);//Maux-> se convierte en máscara de clases

        //Connectividad:
        TList* VML = new TList;//Vaso-Musculo List
        TList* NC = new TList; //NotConnected List

        for (int k=zi;k<zf;k++)
           for(int i=xi; i<xf;i++)
                 for(int j=yi; j<yf;j++)
                        if(volAux->GetV(i-xi,j-yi,k-zi)>0)
                        {
                           volAux->SetV(i-xi,j-yi,k-zi,1);
                           Punto* p = new Punto;
                           p->x=i-xi; p->y=j-yi; p->z=k-zi;
                           VML->Add(p);
                        }else M->SetV(i,j,k,minPX);

        Punto* sp = new Punto;

        //Inicialización: mitad de la ROI => NOTA: cambiar por el centro de masa de la aorta.
        sp->x=(xf-xi)/2;
        sp->y=(yf-yi)/2;
        sp->z=0;

        //ARREGLARRRRRRRRRRRRRRRRR
        getAllPlaneConnected(sp, volAux, VML, NC, ProgressBar1);

        ProgressBar1->Max = VML->Count;
        ProgressBar1->Position=0;

        //Eliminar Pulmón:
        Punto* paux = new Punto;
        for(int i=0; i<VML->Count;i++)
             {ProgressBar1->Position++;
             paux = (Punto*)VML->Items[i];
             M->SetV(paux->x+xi,paux->y+yi,paux->z+zi,minPX);
             }

        ProgressBar1->Position=0;

        VML->Clear(); NC->Clear();
        delete NC; delete VML;

        delete volAux;
        volAux = NULL;

        GraficarEnPantalla();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LUT1Click(TObject *Sender)
{
        int zf,zi;

        if(FinSliceEdit->Text!="")
                zf=atoi(FinSliceEdit->Text.c_str());
        else zf=SliceListBox->ItemIndex;

        if(IniSliceEdit->Text!="")
                zi=atoi(IniSliceEdit->Text.c_str());
        else zi=SliceListBox->ItemIndex-1;

        if(!ROIShape->Visible)
         return;

        int xi = ROIShape->Top;
        int xf = ROIShape->Top+ROIShape->Height;
        int yi = ROIShape->Left;
        int yf = ROIShape->Left+ROIShape->Width;

        //Carga lista RenderPoints con todos los puntos de borde:
        ProgressBar1->Max = zf-zi;
        ProgressBar1->Position=0;

       // RenderPoints->Clear();

        short int  valor;
        short int Center = atof(CenterEdit->Text.c_str());
        short int Width = atof(WidthEdit->Text.c_str());
        //float p[3];

        RenderPunto* rp;
        PuntoF* rn;
        float norma;
        short int* color;

        for (int k=zi;k<zf;k++)
        {ProgressBar1->Position++;
           for(int i=xi; i<xf;i++)
                 for(int j=yi; j<yf;j++)
                    if(M->GetV(i,j,k)>minPX)
                    {
                       valor = M->GetV(i,j,k);

                       if( valor<=Center+Width && valor>=Center-Width)
                       {
                         //Punto* rp = new Punto; rp->x=i; rp->y=j; rp->z=-k+zi+zf;//Invierte Z
                         rp = new RenderPunto; rp->p.x=i; rp->p.y=j; rp->p.z=k;//Invierte Z

                         rp->p.color = (M->GetV(i,j,k) +
                                       M->GetV(i+1,j,k) + M->GetV(i-1,j,k) +
                                       M->GetV(i,j+1,k) + M->GetV(i,j-1,k) +
                                       M->GetV(i,j,k+1) + M->GetV(i,j,k-1))/7.0  ;

                         rp->n.x = (M->GetV(i+1,j,k)-M->GetV(i-1,j,k))*res[0];
                         rp->n.y = (M->GetV(i,j+1,k)-M->GetV(i,j-1,k))*res[1];
                         rp->n.z = (M->GetV(i,j,k+1)-M->GetV(i,j,k-1))*res[2];

                         norma = sqrt(rp->n.x*rp->n.x + rp->n.y*rp->n.y + rp->n.z*rp->n.z);

                         rp->n.x = rp->n.x/norma;
                         rp->n.y = rp->n.y/norma;
                         rp->n.z = rp->n.z/norma;

                         rp->norma = norma;

                         RenderPoints->Add(rp);
                       }
                     }
        }

        ProgressBar1->Position=0;

        medX=(xf+xi)/2.0;
        medY=(yf+yi)/2.0;
        medZ=(zf+zi)/2.0;

        distX = (xf-xi);
        distY = (yf-yi);
        distZ = (zf-zi);

        int zmin=distZ/5.0;
        zINT[0]=zmin;

        for(int i=1;i<6;i++)        //?
        zINT[i]=zINT[i-1]+zmin;

        RenderFlag=true;
        CantPuntosSegLabel->Caption=RenderPoints->Count;
        VolRenderLabel->Caption = RenderPoints->Count*
                             atof(RXEdit->Text.c_str())*
                             atof(RYEdit->Text.c_str())*
                             atof(RZEdit->Text.c_str());

     //   RenderPoints->Sort(CompareColors);
     RenderPoints->Sort(CompareZR);

        offset = 0;
        final = RenderPoints->Count;


      //  mct = new DrawThread(true);

      //  mct->SetParams(RenderPoints,LUT,LUT_Hueso, LUT_Musculo, LUT_Sangre,
      //  medX, medY, medZ, minPX, 3.0, 0, 1, AtenuacionTrackBar->Position);

      //  mct->Resume();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Goma3DCheckBoxClick(TObject *Sender)
{
  res[0]=atof(RXEdit->Text.c_str());
  res[1]=atof(RYEdit->Text.c_str());
  res[2]=atof(RZEdit->Text.c_str());

  float escala[3];

              escala[0] = res[0];
              escala[1] = res[1];
              escala[2] = res[2];

           Punto* paux = new Punto;
           int xrot, yrot;

              GLint viewport[4],realy;
              GLdouble mvmatrix[16],projmatrix[16],wx,wy,wz;

              glGetIntegerv(GL_VIEWPORT, viewport);//leo el viewport
              glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);//leo matriz de view
              glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);//leo matriz de proyeccion

              int* indice;
              IndicesBorrar->Clear();

              for(int i=0; i<750;i++)
                for(int j=0; j<650;j++)
                MapaIndices[i][j]->Clear();

             // RenderPoints->Pack();
             // RenderPoints->Capacity = RenderPoints->Count;

              for(int i=0; i<RenderPoints->Count; i++)
              {
               if(paux = (Punto*)RenderPoints->Items[i])
               {
               gluProject((GLdouble)paux->x-medX,(GLdouble)paux->y-medY,-(GLdouble)paux->z+medZ,mvmatrix,projmatrix,viewport,&wx,&wy,&wz);
               xrot = wx;
               yrot = viewport[3]-wy-1; //corrige el cambio de coordenadas
               paux->xR=xrot;   //utiliza mean y StD como auxiliares para guardar x,y
               paux->yR=yrot;    // tras el cálculo de la rotación
               paux->zR=wz;
               indice = new int[1];
               indice[0] = i;

               if( int(paux->xR)>-1 && int(paux->xR)<750 && int(paux->yR)>-1 && int(paux->yR)<650)
               MapaIndices[int(paux->xR)][int(paux->yR)]->Add(indice);
               }
              }

           //  RenderPoints->Sort(CompareColors);


}
//---------------------------------------------------------------------------

void __fastcall TForm1::DoneButtonClick(TObject *Sender)
{
       Punto* ap = new Punto; //puntero a punto auxiliar
       short int valorResaltar = atoi(ResaltarEdit->Text.c_str());

       for(int k=0;k<RenderPoints->Count;k++)
             if(ap = (Punto*)RenderPoints->Items[k]) //Si no apunta a NULL...
                M->SetV(int(ap->x),int(ap->y),int(ap->z),valorResaltar);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Clustering1Click(TObject *Sender)
{
 int zf,zi;

        if(FinSliceEdit->Text!="")
                zf=atoi(FinSliceEdit->Text.c_str());
        else zf=SliceListBox->ItemIndex;

        if(IniSliceEdit->Text!="")
                zi=atoi(IniSliceEdit->Text.c_str());
        else zi=SliceListBox->ItemIndex-1;

if(ROIShape->Visible==true)
{
}else{return;}

       //1era Segmentación------------------------------------------------------
        int clases = 2;   //2 clases: corazón - pulmón
        int valorMax, valorMin; //Guardan el máximo valor dentro del VOI

        int xi = ROIShape->Top;
        int xf = ROIShape->Top+ROIShape->Height;
        int yi = ROIShape->Left;
        int yf = ROIShape->Left+ROIShape->Width;

        TList* Vector = new TList;

        valorMax = M->GetV(xi,yi,zi);
        valorMin = M->GetV(xi,yi,zi);

        //Creación del vector:
        for (int k=zi;k<zf;k++)
           for(int i=xi; i<xf;i++)
                 for(int j=yi; j<yf;j++)
                 {
                  short int* valor = new short int[1];
                  valor[0] = M->GetV(i,j,k);
                  Vector->Add(valor);

                  if(valor[0]>valorMax)
                  valorMax = valor[0];

                  if(valor[0]<valorMin)
                  valorMin = valor[0];
                 }

        //kmeans: clustering por diferencias:
        float* mu = new float[clases];
        float* desv = new float[clases];

       int Rango = valorMax-valorMin+1;
        int* MascaraDeClases = new int[Rango];

        Rango = kmeansVector(Vector, clases, Rango , valorMin, mu, desv, MascaraDeClases);

        //El resultado de la primera segmentacion devuelve grupos 0 o 1:
        //0: pulmón; 1: corazón. => la segunda segmentación se realizará sobre
        //el grupo 1 para segmentar en sangre-músculo.
        Vector->Clear();
      //------------------------------------------------------------------------

      //2da Segmentación--------------------------------------------------------
        int valorMin2, valorMax2;
        int clase;
        valorMin2 = maxPX;
        valorMax2 = minPX;

        //Guarda en Vector todas las intensidades de los pixels de clase 1: corazon
        for (int k=zi;k<zf;k++)
           for(int i=xi; i<xf;i++)
                 for(int j=yi; j<yf;j++)
                 {
                  clase = MascaraDeClases[M->GetV(i,j,k)-valorMin+1];

                  if(clase == 1) //1: corazon = musculo + sangre (clara/oscura)
                  {
                  short int* valor = new short int[1];
                  valor[0]=M->GetV(i,j,k);

                        if(valor[0]>valorMax2)
                        valorMax2 = valor[0];

                        if(valor[0]<valorMin2)
                        valorMin2 = valor[0];

                  Vector->Add(valor);
                  }

                 }

                 clases = 2; //Musculo y sangre (clara/oscura)

                 int Rango2 = valorMax2-valorMin2+1;

                 float* mu2 = new float[clases];
                 float* desv2 = new float[clases];
                 int* MascaraDeClases2 = new int[Rango2];

                 Rango2 = kmeansVector(Vector, clases, Rango2 , valorMin2, mu2, desv2, MascaraDeClases2);
                 Vector->Clear();

             //-----------------------------------------------------------------
                 int clase1, clase2;

                 Vector->Clear();
                  delete Vector;

         for (int k=zi;k<zf;k++)
            for(int i=xi; i<xf;i++)
                 for(int j=yi; j<yf;j++)
                 {

                 clase1 = MascaraDeClases[M->GetV(i,j,k)-valorMin+1];
                 clase2 = MascaraDeClases2[M->GetV(i,j,k)-valorMin2+1];

                  if(clase1 == 1 && clase2==2) //1-2: sangre oscura
                  {
                  M->SetV(i,j,k,maxPX-1);
                  }

                  if(clase1 == 1 && clase2==1) //1-1: sangre clara
                  {
                  M->SetV(i,j,k,maxPX-1);
                  }

                  if(clase1 == 1 && clase2==0) //1-0: músculo
                  {
                  M->SetV(i,j,k,(maxPX+minPX)/2);
                  }

                  if(clase1 == 0) // pulmón
                  {
                  M->SetV(i,j,k,minPX);
                  }

                 }

         delete MascaraDeClases;
         delete MascaraDeClases2;

         delete mu; delete desv;
         delete mu2; delete desv2;

        GraficarEnPantalla();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::KMeansClusterClick(TObject *Sender)
{
  int zf,zi;

        if(FinSliceEdit->Text!="")
                zf=atoi(FinSliceEdit->Text.c_str());
        else zf=SliceListBox->ItemIndex;

        if(IniSliceEdit->Text!="")
                zi=atoi(IniSliceEdit->Text.c_str());
        else zi=SliceListBox->ItemIndex-1;

if(ROIShape->Visible==true)
{
}else{return;}

        int clases;
        AnsiString clasesString=5;

        if(InputQuery("Ingrese Cantidad de clases","Clases:",clasesString))
        {
        clases = atoi(clasesString.c_str());
        }else {return;}

        int xi = ROIShape->Top;
        int xf = ROIShape->Top+ROIShape->Height;
        int yi = ROIShape->Left;
        int yf = ROIShape->Left+ROIShape->Width;

        Volumen* Maux = new Volumen(xf-xi,yf-yi,zf-zi);

        for (int k=zi;k<zf;k++)
           for(int i=xi; i<xf;i++)
                 for(int j=yi; j<yf;j++)
                  Maux->SetV(i-xi,j-yi,k-zi,M->GetV(i,j,k));

        float Rango;

        //kmeans: clustering por diferencias:
        float* mu = new float[clases];
        float* desv = new float[clases];
        Rango = kmeans(Maux, clases, maxPX, minPX, mu, desv); //Maux-> se convierte en máscara de clases

       //Dibuja las clases sobre M
        for (int k=zi;k<zf;k++)
           for(int i=xi; i<xf;i++)
                 for(int j=yi; j<yf;j++)
                 {
                 float valor = float(Rango/clases)*Maux->GetV(i-xi,j-yi,k-zi)+minPX;
                 M->SetV(i,j,k,int(valor));
                 }

                 delete Maux;
                 Maux = NULL;
/*
OpenDialog1->Filter = "BIN files (*.bin)|*.BIN";
       if(OpenDialog1->Execute())
       {
       FILE* fp;
       fp = fopen( OpenDialog1->FileName.c_str(),  "r+b");
       encabezado enc1;
       short int cortes;
       //size_t fread(void *ptr, size_t num_bytes, size_t count, FILE *stream);

       fread(&cortes,sizeof(short int),1,fp);
       fread(&enc1,sizeof(encabezado),1,fp);

       short int ancho = enc1.ancho;
       short int alto = enc1.alto;
       short int corte = enc1.frame;

                if(cortes==cantImagenes && ancho==anchoPX && alto==altoPX)
                {
                ProgressBar1->Position=0;

                        for (int k=0;k<cantImagenes+1;k++)
                        {
                        ProgressBar1->Position=k;
                                for(int i=0; i<enc1.ancho;i++)
                                        for(int j=0; j<enc1.alto;j++)
                                        {
                                        short int valor;
                                        fread(&valor,sizeof(short int),1,fp);
                                       // M[i][j][corte-1]=valor -(M[i][j][corte-1]+abs(minPX));
                                       if(M->GetV(i,j,corte-1)!=4 && M->GetV(i,j,k)!=5)
                                       M->SetV(i,j,corte-1,valor);
                                       else M->SetV(i,j,corte-1,minPX);

                                                //Evita Saturación:
                                                if(M->GetV(i,j,corte-1)<minPX)
                                                M->SetV(i,j,corte-1,minPX);
                                                if(M->GetV(i,j,corte-1)>maxPX)
                                                M->SetV(i,j,corte-1,maxPX);
                                        }

                        fread(&enc1,sizeof(encabezado),1,fp);
                        corte = enc1.frame;

                        }

                fclose( fp );
                ProgressBar1->Position=0;
                }
       }
*/
}
//---------------------------------------------------------------------------
void __fastcall TForm1::GuardarPTSbuttonClick(TObject *Sender)
{
if(SaveDialog1->Execute())
       {

         RenderPoints->Pack();
           RenderPoints->Capacity = RenderPoints->Count;

        FILE* fp;
        AnsiString FileName = SaveDialog1->FileName;

        if(!(FileName.AnsiPos(".dat") || FileName.AnsiPos(".DAT")))
        FileName=FileName+".dat";

        fp=fopen(FileName.c_str(),"w+b");

        int cantPuntos = RenderPoints->Count;

        RenderPunto* ap = new RenderPunto; //punto auxiliar

        fwrite(&cantPuntos, sizeof(int), 1, fp);

        int x,y,z;
        float nx, ny, nz;
        float norma;

         for(int k=0;k<RenderPoints->Count;k++)
         {
             if(ap = (RenderPunto*)RenderPoints->Items[k])
             {
             x=ap->p.x; y=ap->p.y; z=ap->p.z;
             nx = ap->n.x; ny = ap->n.y; nz = ap->n.z;
             norma = ap->norma;
             //size_t fwrite(const void *ptr, size_t size, size_t n, FILE *stream);
             fwrite(&x, sizeof(int), 1, fp);
             fwrite(&y, sizeof(int), 1, fp);
             fwrite(&z, sizeof(int), 1, fp);
             fwrite(&ap->p.color, sizeof(short int), 1, fp);

             fwrite(&nx, sizeof(float), 1, fp);
             fwrite(&ny, sizeof(float), 1, fp);
             fwrite(&nz, sizeof(float), 1, fp);
             fwrite(&norma, sizeof(float), 1, fp);
             }
         }

        fclose(fp);
       }

       RenderPoints->Sort(CompareColors);
       final = RenderPoints->Count;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::SegmentaCorazn1Click(TObject *Sender)
{
        AnsiString umbralString;
        int umbral;  //Umbral en HU para detectar zonas fuera del corazón

        AnsiString radio0String;
        int r0;  //Radio inicial de la circuenferencia

        AnsiString umbralPuntosContactoString;
        int umbralPC; //Cantidad de Puntos de Contacto admitidos

        if(SegmentaForm1->ShowModal()==mrOk)
        {
        radio0String = SegmentaForm1->radio0Edit->Text;
        r0=atoi(radio0String.c_str());

        umbralString = SegmentaForm1->umbralEdit->Text;
        umbral = atoi(umbralString.c_str());

        umbralPuntosContactoString = SegmentaForm1->umbralPCEdit->Text;
        umbralPC = atoi(umbralPuntosContactoString.c_str());

        }else return;

         int zf,zi;

        if(FinSliceEdit->Text!="")
                zf=atoi(FinSliceEdit->Text.c_str()); //Slice Final
        else zf=SliceListBox->ItemIndex;

        if(IniSliceEdit->Text!="")
                zi=atoi(IniSliceEdit->Text.c_str());  //Slice Inicial
        else zi=SliceListBox->ItemIndex-1;


        double acumX, acumY, acumZ, acumINT;
        acumX=0; acumY=0; acumZ=0; acumINT=0;

        ProgressBar1->Position=0;

        int totalCV = 120;
        float ang = 2*3.141632/float(totalCV);

       //..........Cálculo del Centro de Gravedad de la Matriz..................
       for (int k=zi;k<zf;k++)
       {ProgressBar1->Position=k;
       for (int i=0; i<altoPX; i++)
         for(int j=0; j<anchoPX; j++)
          {
          acumX = acumX + (M->GetV(i,j,k)-minPX)*i;
          acumY = acumY + (M->GetV(i,j,k)-minPX)*j;
          acumZ = acumZ + (M->GetV(i,j,k)-minPX)*k;
          acumINT = acumINT + M->GetV(i,j,k)-minPX;
          }}

          double xGf,yGf,zGf;
          xGf = acumX/acumINT;
          yGf = acumY/acumINT;
          zGf = acumZ/acumINT;

          int xG,yG,zG;
          xG=xGf; yG=yGf; zG=zGf;
          ProgressBar1->Position=0;

         // int ci=zG-zi;

          TList* Resultados = new TList;

        //  circ* caux = new circ; //circunferencia auxiliar;
          int cgX;
          int cgY;

      //  CV** arrayCV;
        arrayCV = new CV*[totalCV];

       float radio;
       float xc;
       float yc;
       float zc;

        //Calculo del promedio dentro de la circunferencia inicial:
          balloon(M->V, minPX, xG, yG, zG, r0,
                         umbral, umbralPC, Resultados);


         circ* result = new circ;
         result = (circ*)Resultados->Items[0];

       radio = result->rc;
       xc = result->xc;
       yc = result->yc;
       zc = result->zc;

        double acum=0;
        double cant=0;

        for(int i=xc-radio;i<xc+radio;i++)
                for(int j=yc-radio;j<yc+radio;j++)
                        if((i-xc)*(i-xc)+(j-yc)*(j-yc)<radio*radio)
                        {
                        acum=acum+M->GetV(i,j,int(zc));
                        cant++;
                        }

        double media=acum/cant;

        acum=0;

         for(int i=xc-radio;i<xc+radio;i++)
                for(int j=yc-radio;j<yc+radio;j++)
                        if((i-xc)*(i-xc)+(j-yc)*(j-yc)<radio*radio)
                        {
                        acum=acum+pow(M->GetV(i,j,int(zc))-media,2);
                        }

         double desv = 2*sqrt(acum/cant);

        //Creación de la función de viscosidad......................

        float* YVC = new float[maxPX-minPX];//Función de viscosidad

        for(float i=0; i<maxPX-minPX; i++)
        YVC[int(i)]=(1.0 - exp(-1.0/(desv*desv)*pow(i+minPX-media,2)));

        //Creación de las VC ....................................
        float x,y;
        short int scope = 3;
        float inercia = 15;

        for(int i=0; i<totalCV; i++)
        {
         x = xc + radio*cos(i*ang);
         y = yc + radio*sin(i*ang);
         arrayCV[i] = new CV(x, y, cos(i*ang), sin(i*ang), scope, inercia);
         arrayCV[i]->ligarLUT(YVC, minPX);
        }

         for(int i=0; i<totalCV; i++)
        {
         if(i==totalCV-1)
         arrayCV[i]->ligarCV(arrayCV[0],1);
         else arrayCV[i]->ligarCV(arrayCV[i+1],1);

         if(i==0)
         arrayCV[i]->ligarCV(arrayCV[totalCV-1],0);
         else arrayCV[i]->ligarCV(arrayCV[i-1],0);
        }
        //........................................................

        //Movimiento de las Células Virtuales......................
       //int cantIteraciones = 50;
        int cantIteraciones = 1;

        for(int i=0; i<cantIteraciones; i++)
        {
                for(int i=0; i<totalCV; i++)
                {
                arrayCV[i]->verCamino(M,zc);
                arrayCV[i]->moverLigado(1.0);
                }
        }
        //.........................................................

        //Dibujo del resultado.....................................
         short int px, py;
         for(int i=0; i<totalCV; i++)
         {
         px=arrayCV[i]->getPosX();
         py=arrayCV[i]->getPosY();
         arrayCV[i]->verCamino(M,zc);
         M->SetV(int(px),int(py),int(zc),maxPX);
         }
         //.........................................................

    Resultados->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::EliminaRenderButtonClick(TObject *Sender)
{
      RenderPunto* ap;
      for(int i=0; i<RenderPoints->Count; i++)
      {
      ap = (RenderPunto*)RenderPoints->Items[i];
      delete ap;
      }
      RenderPoints->Clear();

      PuntoF* atp;
      for(int i=0; i<TriangList->Count; i++)
      {
      atp = (PuntoF*)TriangList->Items[i];
      delete atp;
      }
      TriangList->Clear();

      float* atc;
      for(int i=0; i<TriangColors->Count; i++)
      {
      atc = (float*)TriangColors->Items[i];
      delete atc;
      }
      TriangColors->Clear();

      PuntoI* tr;
      for(int i=0; i<TriList->Count; i++)
      {
      tr = (PuntoI*)TriList->Items[i];
      delete tr;
      }
      TriList->Clear();

      PuntoF* n;
      for(int i=0; i<Normals->Count; i++)
      {
      n = (PuntoF*)Normals->Items[i];
      delete n;
      }
      Normals->Clear();

}
//---------------------------------------------------------------------------

void __fastcall TForm1::CargarPTSbuttonClick(TObject *Sender)
{
OpenDialog1->Filter = "pts Cloud (*.dat)|*.dat";
if(OpenDialog1->Execute())
       {
        RenderPoints->Clear();

        FILE* fp;
        AnsiString FileName = OpenDialog1->FileName;

        fp=fopen(FileName.c_str(),"r+b");

        int cantPuntos; 
        //fread(void *ptr, size_t num_bytes, size_t count, FILE *stream);
        fread(&cantPuntos, sizeof(int), 1, fp);

        int x,y,z;
        float nx, ny, nz;
        float norma;
        short int color;

        RenderPoints->Capacity=cantPuntos;

         for(int k=0;k<RenderPoints->Capacity;k++)
         {

             fread(&x, sizeof(int), 1, fp);
             fread(&y, sizeof(int), 1, fp);
             fread(&z, sizeof(int), 1, fp);
             fread(&color, sizeof(short int), 1, fp);

             fread(&nx, sizeof(float), 1, fp);
             fread(&ny, sizeof(float), 1, fp);
             fread(&nz, sizeof(float), 1, fp);
             fread(&norma, sizeof(float), 1, fp);

             RenderPunto* ap = new RenderPunto; //punto auxiliar
             ap->p.x = x; ap->p.y = y; ap->p.z = z; ap->p.color = color;
             ap->n.x = nx; ap->n.y = y; ap->n.z = z; ap->norma = norma;

             RenderPoints->Add(ap);
         }

        fclose(fp);


       RenderFlag=true;
       SimpleFlag=true;

           RenderPunto* ap;
           ap = (RenderPunto*)RenderPoints->Items[0];

           minX = ap->p.x;
           maxX = ap->p.x;

           minY = ap->p.y;
           maxY = ap->p.y;

           minZ = ap->p.z;
           maxZ = ap->p.z;

           for(int i=0; i<RenderPoints->Count; i++)
           {
              ap = (RenderPunto*)RenderPoints->Items[i];

              if(ap->p.x<minX)
              minX = ap->p.x;

              if(ap->p.x>maxX)
              maxX = ap->p.x;

              if(ap->p.y<minY)
              minY = ap->p.y;

              if(ap->p.y>maxY)
              maxY = ap->p.y;

              if(ap->p.z<minZ)
              minZ = ap->p.z;

              if(ap->p.z>maxZ)
              maxZ = ap->p.z;
           }

        IniSliceEdit->Text = minZ;
        FinSliceEdit->Text = maxZ;
       }

       RenderPoints->Sort(CompareColors);
       final = RenderPoints->Count;

}
//---------------------------------------------------------------------------


void __fastcall TForm1::Cierre1Click(TObject *Sender)
{
    int zf=atoi(FinSliceEdit->Text.c_str());
        int zi=atoi(IniSliceEdit->Text.c_str());
        int cantCortes = zf-zi;

        int xi = ROIShape->Top;
        int xf = ROIShape->Top+ROIShape->Height;
        int yi = ROIShape->Left;
        int yf = ROIShape->Left+ROIShape->Width;

        //Apertura:
        AnsiString radioString = "8";
        int r;

        if(InputQuery("Ingrese Radio del Elemento Estructurante","Radio:",radioString))
        {
        r = atoi(radioString.c_str());
        }else {return;}

        ProgressBar1->Max=cantCortes-2*r+1;

        //Listas y arrays auxiliares para la creación del elemento estructurante
        //particionado circular de radio=r
        TList* Back = new TList;
        TList* Front = new TList;
        TList* Core = new TList;
        
        bool*** MorphM;

        //Inicializacion de la máscara de morph:
        MorphM = new bool** [2*r+1 + 2];    //+2: extiende las dimensiones de la
        for (int i=0; i<2*r+1 + 2; ++i)     //    máscara para evitar problemas
        {                                   //    de contorno.
        MorphM[i]=new bool* [2*r+1 + 2];

                for (int j=0; j<2*r+1 + 2; ++j)
                {
                MorphM[i][j]=new bool [2*r+1 + 2];
                      for (int k=0; k<2*r+1 + 2; ++k)
                         MorphM[i][j][k]=0;
                }
        }

        //Volumen Auxiliar para guardar resultados:
        Volumen* Maux = new Volumen(xf-xi+1, yf-yi+1, zf-zi+1);

        //Creación del elemento estructurante particionado:
        //strelM(TList* Back, TList* Front, TList* Core, bool*** M)
        //: definida en "strel.h"
        strelM(r,Back, Front, Core, MorphM);

       short int posmaxF[3]; //coordenadas x,y,z de la posicion del mínimo de Front
        short int posmaxC[3]; //coordenadas x,y,z de la posicion del mínimo de Core

        short int posmaxF2[3]; //coordenadas x,y,z de la posicion del mínimo de Front

         short int maxB;
         short int maxF;
         short int maxC;

         short int maxF2;

         short int max;

         unsigned char grupoMax = 'c';

      for (int k=zi+r;k<zf-r;k++)
       {ProgressBar1->Position=k-zi-r;
         for(int j=yi+r; j<yf-r;j++)
            for(int i=xi+r; i<xf-r;i++)
            {
             if(i == xi+r || grupoMax=='b') //Es la primera vuelta o se perdió  registro del maximo anterior?
             {
             //Dilatación: búsqueda del máximo en Back
                maxB=minPX-1;
                               for (int h = 0; h<Back->Count; h++)
                                        if(M->GetV(i+((Punto*)Back->Items[h])->x,
                                                   j+((Punto*)Back->Items[h])->y,
                                                   k+((Punto*)Back->Items[h])->z)

                                            >maxB)
                                                maxB = M->GetV(i+((Punto*)Back->Items[h])->x,
                                                   j+((Punto*)Back->Items[h])->y,
                                                   k+((Punto*)Back->Items[h])->z);
             //Búsqueda del mínimo en Core
                maxC=minPX-1;
                               for (int h = 0; h<Core->Count; h++)
                                        if(M->GetV(i+((Punto*)Core->Items[h])->x,
                                                   j+((Punto*)Core->Items[h])->y,
                                                   k+((Punto*)Core->Items[h])->z)
                                                 >maxC)
                                                {
                                                maxC = M->GetV(i+((Punto*)Core->Items[h])->x,
                                                   j+((Punto*)Core->Items[h])->y,
                                                   k+((Punto*)Core->Items[h])->z);
                                                posmaxC[0] = i+((Punto*)Core->Items[h])->x;
                                                posmaxC[1] = j+((Punto*)Core->Items[h])->y;
                                                posmaxC[2] = k+((Punto*)Core->Items[h])->z;
                                                }
             //Búsqueda del mínimo en Front
                maxF=minPX-1;
                               for (int h = 0; h<Front->Count; h++)
                                        if(M->GetV(i+((Punto*)Front->Items[h])->x,
                                                   j+((Punto*)Front->Items[h])->y,
                                                   k+((Punto*)Front->Items[h])->z)
                                               >maxF)
                                               {
                                                maxF = M->GetV(i+((Punto*)Front->Items[h])->x,
                                                   j+((Punto*)Front->Items[h])->y,
                                                   k+((Punto*)Front->Items[h])->z);
                                                posmaxF[0] = i+((Punto*)Front->Items[h])->x;
                                                posmaxF[1] = j+((Punto*)Front->Items[h])->y;
                                                posmaxF[2] = k+((Punto*)Front->Items[h])->z;
                                                }

                             //búsqueda del mínimo entre los mínimos:
                             max = maxB;
                             grupoMax = 'b';

                             if(maxC>=max)
                             {
                             max = maxC;
                             grupoMax = 'c';
                             }

                             if(maxF>max)
                             {
                             max = maxF;
                             grupoMax = 'f';

                             posmaxC[0] = posmaxF[0];
                             posmaxC[1] = posmaxF[1];
                             posmaxC[2] = posmaxF[2];
                             }
                             else if (maxF == max)
                             {
                                 if (grupoMax == 'c')
                                   if(posmaxF[0]>posmaxC[0])
                                   {
                                   max = maxF;
                                   grupoMax = 'f';

                                   posmaxC[0] = posmaxF[0];
                                   posmaxC[1] = posmaxF[1];
                                   posmaxC[2] = posmaxF[2];
                                   }
                             }

             //Escribe en el vol auxiliar:
             Maux->SetV(i-xi-r,j-yi-r,k-zi-r,max);

             } //fin de "¿es la primera vuelta?"  o "se perdió registro de ..."?
             else
             {//Búsqueda del mínimo en Front
                               maxF2 = minPX-1;
                               for (int h = 0; h<Front->Count; h++)
                                            if( M->GetV(i+((Punto*)Front->Items[h])->x,
                                                   j+((Punto*)Front->Items[h])->y,
                                                   k+((Punto*)Front->Items[h])->z)
                                            >maxF2)
                                             {
                                             maxF2 =  M->GetV(i+((Punto*)Front->Items[h])->x,
                                                   j+((Punto*)Front->Items[h])->y,
                                                   k+((Punto*)Front->Items[h])->z);
                                             posmaxF2[0] = i+((Punto*)Front->Items[h])->x;
                                             posmaxF2[1] = j+((Punto*)Front->Items[h])->y;
                                             posmaxF2[2] = k+((Punto*)Front->Items[h])->z;
                                             }

             if(maxF2>max || (maxF2==max && posmaxF2[0]>posmaxC[0]))
             {
             max = maxF2;
             grupoMax = 'f';
             posmaxC[0] = posmaxF2[0];
             posmaxC[1] = posmaxF2[1];
             posmaxC[2] = posmaxF2[2];

                 if(!MorphM[posmaxF2[0]-i+r+1-1][posmaxF2[1]-j+r+1][posmaxF2[2]-k+r+1])
                 grupoMax = 'b';
             }
             else  //Si no encontre el minimo en el nuevo Front
             {//Si el punto mínimo fue a parar a back
                 if(!MorphM[posmaxC[0]-i+r+1-1][posmaxC[1]-j+r+1][posmaxC[2]-k+r+1])
                 grupoMax = 'b';
             }

             Maux->SetV(i-xi-r,j-yi-r,k-zi-r,max);
             }//fin de else "¿es la primera vuelta?"

         }//end for i, j
       }//end for k

           ProgressBar1->Position=0;

       //Recupera la imagen dilatada para erosionar:
       for (int k=zi+r;k<zf-r;k++)
       {
           ProgressBar1->Position=k-zi-r;
           for(int j=yi+r; j<yf-r;j++)
            for(int i=xi+r; i<xf-r;i++)
            M->SetV(i,j,k,Maux->GetV(i-xi-r,j-yi-r,k-zi-r));
       }
       ProgressBar1->Position=0;

       short int posminF[3]; //coordenadas x,y,z de la posicion del mínimo de Front
        short int posminC[3]; //coordenadas x,y,z de la posicion del mínimo de Core

        short int posminF2[3]; //coordenadas x,y,z de la posicion del mínimo de Front

         short int minB;
         short int minF;
         short int minC;

         short int minF2;

         short int min;

         unsigned char grupoMin = 'c';

      for (int k=zi+r;k<zf-r;k++)
       {ProgressBar1->Position=k-zi-r;
         for(int j=yi+r; j<yf-r;j++)
            for(int i=xi+r; i<xf-r;i++)
            {
             if(i == xi+r || grupoMin=='b') //Es la primera vuelta o se perdió  registro del mínimo anterior?
             {
             //Erosión: búsqueda del mínimo en Back
                minB=maxPX+1;
                               for (int h = 0; h<Back->Count; h++)
                                        if( M->GetV(i+((Punto*)Back->Items[h])->x,
                                                   j+((Punto*)Back->Items[h])->y,
                                                   k+((Punto*)Back->Items[h])->z)
                                            <minB)
                                                minB = M->GetV(i+((Punto*)Back->Items[h])->x,
                                                   j+((Punto*)Back->Items[h])->y,
                                                   k+((Punto*)Back->Items[h])->z);
             //Búsqueda del mínimo en Core
                minC=maxPX+1;
                               for (int h = 0; h<Core->Count; h++)
                                        if( M->GetV(i+((Punto*)Core->Items[h])->x,
                                                   j+((Punto*)Core->Items[h])->y,
                                                   k+((Punto*)Core->Items[h])->z)
                                                 <minC)
                                                {
                                                minC = M->GetV(i+((Punto*)Core->Items[h])->x,
                                                   j+((Punto*)Core->Items[h])->y,
                                                   k+((Punto*)Core->Items[h])->z);
                                                posminC[0] = i+((Punto*)Core->Items[h])->x;
                                                posminC[1] = j+((Punto*)Core->Items[h])->y;
                                                posminC[2] = k+((Punto*)Core->Items[h])->z;
                                                }
             //Búsqueda del mínimo en Front
                minF=maxPX+1;
                               for (int h = 0; h<Front->Count; h++)
                                        if(M->GetV(i+((Punto*)Front->Items[h])->x,
                                                   j+((Punto*)Front->Items[h])->y,
                                                   k+((Punto*)Front->Items[h])->z)
                                               <minF)
                                               {
                                                minF = M->GetV(i+((Punto*)Front->Items[h])->x,
                                                   j+((Punto*)Front->Items[h])->y,
                                                   k+((Punto*)Front->Items[h])->z);
                                                posminF[0] = i+((Punto*)Front->Items[h])->x;
                                                posminF[1] = j+((Punto*)Front->Items[h])->y;
                                                posminF[2] = k+((Punto*)Front->Items[h])->z;
                                                }

                             //búsqueda del mínimo entre los mínimos:
                             min = minB;
                             grupoMin = 'b';

                             if(minC<=min)
                             {
                             min = minC;
                             grupoMin = 'c';
                             }

                             if(minF<min)
                             {
                             min = minF;
                             grupoMin = 'f';

                             posminC[0] = posminF[0];
                             posminC[1] = posminF[1];
                             posminC[2] = posminF[2];
                             }
                             else if (minF == min)
                             {
                                 if (grupoMin == 'c')
                                   if(posminF[0]>posminC[0])
                                   {
                                   min = minF;
                                   grupoMin = 'f';

                                   posminC[0] = posminF[0];
                                   posminC[1] = posminF[1];
                                   posminC[2] = posminF[2];
                                   }
                             }

             //Escribe en el vol auxiliar:
             Maux->SetV(i-xi-r,j-yi-r,k-zi-r,min);

             } //fin de "¿es la primera vuelta?"  o "se perdió registro de ..."?
             else
             {//Búsqueda del mínimo en Front
                               minF2 = maxPX+1;
                               for (int h = 0; h<Front->Count; h++)
                                            if(M->GetV(i+((Punto*)Front->Items[h])->x,
                                                   j+((Punto*)Front->Items[h])->y,
                                                   k+((Punto*)Front->Items[h])->z)
                                            <minF2)
                                             {
                                             minF2 = M->GetV(i+((Punto*)Front->Items[h])->x,
                                                   j+((Punto*)Front->Items[h])->y,
                                                   k+((Punto*)Front->Items[h])->z);
                                             posminF2[0] = i+((Punto*)Front->Items[h])->x;
                                             posminF2[1] = j+((Punto*)Front->Items[h])->y;
                                             posminF2[2] = k+((Punto*)Front->Items[h])->z;
                                             }

             if(minF2<min || (minF2==min && posminF2[0]>posminC[0]))
             {
             min = minF2;
             grupoMin = 'f';
             posminC[0] = posminF2[0];
             posminC[1] = posminF2[1];
             posminC[2] = posminF2[2];

                 if(!MorphM[posminF2[0]-i+r+1-1][posminF2[1]-j+r+1][posminF2[2]-k+r+1])
                 grupoMin = 'b';
             }
             else  //Si no encontre el minimo en el nuevo Front
             {//Si el punto mínimo fue a parar a back
                 if(!MorphM[posminC[0]-i+r+1-1][posminC[1]-j+r+1][posminC[2]-k+r+1])
                 grupoMin = 'b';
             }

             Maux->SetV(i-xi-r,j-yi-r,k-zi-r,min);
             }//fin de else "¿es la primera vuelta?"
         }//end for i, j
       }//end for k

           ProgressBar1->Position=0;

       //Recupera la imagen erosionada para dilatar:
       for (int k=zi+r;k<zf-r;k++)
       {
           ProgressBar1->Position=k-zi-r;
           for(int j=yi+r; j<yf-r;j++)
            for(int i=xi+r; i<xf-r;i++)
            M->SetV(i,j,k,Maux->GetV(i-xi-r,j-yi-r,k-zi-r));
       }
       ProgressBar1->Position=0;

     delete Maux;
     Maux = NULL;

       for (int i=0; i<2*r+1+2; ++i)
           for (int j=0; j<2*r+1+2; ++j)
                delete[] MorphM[i][j];
        for (int i=0; i<2*r+1+2; ++i)
                delete MorphM[i];
        delete[] MorphM;

     GraficarEnPantalla();

        for(int i=0; i<Back->Count; i++)
               delete Back->Items[0];

        for(int i=0; i<Core->Count; i++)
               delete Core->Items[0];

        for(int i=0; i<Front->Count; i++)
               delete Front->Items[0];
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Umbral1Click(TObject *Sender)
{
 int zf,zi;

        if(FinSliceEdit->Text!="")
                zf=atoi(FinSliceEdit->Text.c_str());
        else zf=SliceListBox->ItemIndex;

        if(IniSliceEdit->Text!="")
                zi=atoi(IniSliceEdit->Text.c_str());
        else zi=SliceListBox->ItemIndex-1;

        if(!ROIShape->Visible)
         return;

        int xi = ROIShape->Top;
        int xf = ROIShape->Top+ROIShape->Height;
        int yi = ROIShape->Left;
        int yf = ROIShape->Left+ROIShape->Width;

        //Carga lista RenderPoints con todos los puntos de borde:
        ProgressBar1->Max = zf-zi;
        ProgressBar1->Position=0;

        RenderPunto* rp;
        PuntoF* rn;
        float norma;
        short int* color;

        for (int k=zi;k<zf;k++)
        {ProgressBar1->Position++;
           for(int i=xi; i<xf;i++)
                 for(int j=yi; j<yf;j++)
                    if(M->GetV(i,j,k)>minPX)
                    {
                       if( M->GetV(i,j,k)>=Center)
                       {
                         //Punto* rp = new Punto; rp->x=i; rp->y=j; rp->z=-k+zi+zf;//Invierte Z
                         rp = new RenderPunto; rp->p.x=i; rp->p.y=j; rp->p.z=k;//Invierte Z

                         rp->p.color = (M->GetV(i,j,k) +
                                       M->GetV(i+1,j,k) + M->GetV(i-1,j,k) +
                                       M->GetV(i,j+1,k) + M->GetV(i,j-1,k) +
                                       M->GetV(i,j,k+1) + M->GetV(i,j,k-1))/7.0  ;

                         rp->n.x = M->GetV(i+1,j,k)-M->GetV(i-1,j,k);
                         rp->n.y = M->GetV(i,j+1,k)-M->GetV(i,j-1,k);
                         rp->n.z = M->GetV(i,j,k+1)-M->GetV(i,j,k-1);

                         norma = sqrt(rp->n.x*rp->n.x + rp->n.y*rp->n.y + rp->n.z*rp->n.z);

                         rp->n.x = rp->n.x/norma;
                         rp->n.y = rp->n.y/norma;
                         rp->n.z = rp->n.z/norma;

                         rp->norma = norma;

                         RenderPoints->Add(rp);
                       }
                     }
        }

        ProgressBar1->Position=0;

        medX=(xf+xi)/2.0;
        medY=(yf+yi)/2.0;
        medZ=(zf+zi)/2.0;

        distX = (xf-xi);
        distY = (yf-yi);
        distZ = (zf-zi);

        int zmin=distZ/5.0;
        zINT[0]=zmin;

        for(int i=1;i<6;i++)        //?
        zINT[i]=zINT[i-1]+zmin;

        RenderFlag=true;
        CantPuntosSegLabel->Caption=RenderPoints->Count;
        VolRenderLabel->Caption = RenderPoints->Count*
                             atof(RXEdit->Text.c_str())*
                             atof(RYEdit->Text.c_str())*
                             atof(RZEdit->Text.c_str());

        RenderPoints->Sort(CompareColors);

        offset = 0;
        final = RenderPoints->Count;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::ElimButtonClick(TObject *Sender)
{
       Punto* ap = new Punto; //puntero a punto auxiliar

       for(int k=0;k<RenderPoints->Count;k++)
             if(ap = (Punto*)RenderPoints->Items[k]) //Si no apunta a NULL...
                M->SetV(int(ap->x),int(ap->y),int(ap->z),minPX);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SquareTracing1Click(TObject *Sender)
{
        int zf,zi;

        if(FinSliceEdit->Text!="")
                zf=atoi(FinSliceEdit->Text.c_str());
        else zf=SliceListBox->ItemIndex;

        if(IniSliceEdit->Text!="")
                zi=atoi(IniSliceEdit->Text.c_str());
        else zi=SliceListBox->ItemIndex-1;

        if(!ROIShape->Visible)
         return;

        int xi = ROIShape->Top;
        int xf = ROIShape->Top+ROIShape->Height;
        int yi = ROIShape->Left;
        int yf = ROIShape->Left+ROIShape->Width;

  //int zs = atoi(SliceListBox->Items->Strings[SliceListBox->ItemIndex].c_str())-1;
  //int xs = xP;
  //int ys = yP;

  short int Center = atof(CenterEdit->Text.c_str());
  short int Width = atof(WidthEdit->Text.c_str());


  // getAllMatrixRangeConnected(sp,M,RenderPoints,RenderColors,Center,Width,cantImagenes);

  TList* ListaST = new TList; //Lista del Square Tracing
  Punto* np;
  bool encontro = false;
  int cuentaPuntos = 0;

for(int k=zi; k<zf; k++)
for(int i=xi; i<xf; i++)
{
  encontro = false;
  //From bottom to top and left to right scan the cells of T until a black pixel,
  //sp is found...
      for(int j=yi; j<yf; j++)
      {
          if(M->GetV(i,j,k)<Center+Width && M->GetV(i,j,k)>Center-Width)
          {
          //Insert s in ListaST...
          np = new Punto;
          np->x=i; np->y=j; np->z=k;
             ListaST->Add(np);
             encontro=true;
             break;
           }
      }

      if(encontro)
      {
        Punto sp; //Starting point
        sp.x = np->x; sp.y = np->y; sp.z = np->z;

        Punto ap; //Actual point

        //Direcciones:
        // 0, 1, 2, 3:
        //0: izq der,
        //1: ab-arr
        //2: der-izq
        //3: arr-ab

        unsigned char dir = '0';

        unsigned char dir0 = dir;
        
        //Set the current pixel, ap, to be the starting pixel, sp...
        ap.x = sp.x; ap.y = sp.y; ap.z = sp.z;

        //Turn left i.e. visit the left adjacent pixel of ap...
        //Update ap i.e. set it to be the current pixel.
         switch(dir)
         {
          case '0':
          {ap.x = ap.x-1;dir = '1';break;}
          case '1':
          {ap.y = ap.y-1;dir = '2';break;}
          case '2':
          {ap.x = ap.x+1;dir = '3';break;}
          case '3':
          {ap.y = ap.y+1;dir = '0';break;}
         }

         cuentaPuntos = ListaST->Count;

        //While ap not equal to sp do:
        while(ap.x!=sp.x || ap.y!=sp.y || ap.z!=sp.z || dir!=dir0
              &&
             (ap.x>=xi && ap.x<=xf && ap.y>=yi && ap.y<=yf && ap.z>=zi && ap.z<=zf))
        {
          //If the current pixel ap is black (and is contrainted inside the box):
          if(M->GetV(ap.x,ap.y,ap.z)<Center+Width && M->GetV(ap.x,ap.y,ap.z)>Center-Width)
          {
          np = new Punto;
          np->x = ap.x;
          np->y = ap.y;
          np->z = ap.z;
          //insert ap in ListaST and turn left (visit the left adjacent pixel of ap):
          ListaST->Add(np);
                switch(dir)
                {
                case '0':
                {ap.x = ap.x-1;dir = '1';break;}
                case '1':
                {ap.y = ap.y-1;dir = '2';break;}
                case '2':
                {ap.x = ap.x+1;dir = '3';break;}
                case '3':
                {ap.y = ap.y+1;dir = '0';break;}
                }
           } //if ...
           //turn right (visit the right adjacent pixel of ap):
           else
           {
            switch(dir)
                {
                //Update ap i.e. set it to be the current pixel:
                 case '0':
                 {ap.x = ap.x+1;dir = '3';break;}
                 case '1':
                 {ap.y = ap.y+1;dir = '0';break;}
                 case '2':
                 {ap.x = ap.x-1;dir = '1';break;}
                 case '3':
                 {ap.y = ap.y-1;dir = '2';break;}
                }
            }  //else...
        } //while ...

      int maxx = 0;

      for(int i = cuentaPuntos; i<ListaST->Count; i++)
        {
        np = (Punto*)ListaST->Items[i];
        if(np->x>maxx)
        maxx = np->x;
        }

     i=maxx+1;

      } //if(encontro)

} //for x...

        for(int i = 0; i<ListaST->Count; i++)
        {
        np = (Punto*)ListaST->Items[i];
        M->SetV(np->x,np->y,np->z,500);
        }

        ListaST->Clear();
        delete ListaST;

  /*    medX=(xf+xi)/2.0;
        medY=(yf+yi)/2.0;
        medZ=(zf+zi)/2.0;

        distX = (xf-xi);
        distY = (yf-yi);
        distZ = (zf-zi); */
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Global1Click(TObject *Sender)
{
 int zf,zi;

        if(FinSliceEdit->Text!="")
                zf=atoi(FinSliceEdit->Text.c_str());
        else zf=SliceListBox->ItemIndex;

        if(IniSliceEdit->Text!="")
                zi=atoi(IniSliceEdit->Text.c_str());
        else zi=SliceListBox->ItemIndex-1;

        if(!ROIShape->Visible)
         return;

        int xi = ROIShape->Top;
        int xf = ROIShape->Top+ROIShape->Height;
        int yi = ROIShape->Left;
        int yf = ROIShape->Left+ROIShape->Width;

  int zs = atoi(SliceListBox->Items->Strings[SliceListBox->ItemIndex].c_str())-1;
  int xs = xP;
  int ys = yP;

  Punto* sp = new Punto(); //seed point.
  sp->x=xs; sp->y=ys;sp->z=zs;

  //RenderPoints->Clear();  //Comentado: sirve para acumular puntos.
  //RenderColors->Clear();

  short int Center = atof(CenterEdit->Text.c_str());
  short int Width = atof(WidthEdit->Text.c_str());

   TMenuItem *MI = dynamic_cast<TMenuItem*>(Sender);

   TList* RenderPointsAux = new TList;


   if(!MI->Tag)
   getAllMatrixRangeConnected(sp,M,RenderPointsAux,Center,Width,cantImagenes);
   else
   getAllMatrixRangeConstraintedConnected(sp,M,RenderPointsAux,Center,Width,cantImagenes,
                                         xi,xf,yi,yf,zi,zf);

    RenderPunto* rp;
    Punto* rn;
    float norma;
    short int* color;

    for(int i = 0; i<RenderPointsAux->Count; i++)
    {

     rn = (Punto*)RenderPointsAux->Items[i];

     rp = new RenderPunto;
     rp->p.x=rn->x;
     rp->p.y=rn->y;
     rp->p.z=rn->z;
     rp->p.color = M->GetV(rn->x, rn->y, rn->z);

     rp->n.x = M->GetV(rn->x+1,rn->y,rn->z)-M->GetV(rn->x-1,rn->y,rn->z);
     rp->n.y = M->GetV(rn->x,rn->y+1,rn->z)-M->GetV(rn->x,rn->y-1,rn->z);
     rp->n.z = M->GetV(rn->x,rn->y,rn->z+1)-M->GetV(rn->x,rn->y,rn->z-1);

     norma = sqrt(rp->n.x*rp->n.x + rp->n.y*rp->n.y + rp->n.z*rp->n.z);

     rp->n.x = rp->n.x/norma;
     rp->n.y = rp->n.y/norma;
     rp->n.z = rp->n.z/norma;

     rp->norma = norma;
     RenderPoints->Add(rp);

     delete rn;
     rn = NULL;
    }

    RenderPointsAux->Clear();
    delete RenderPointsAux;

        medX=(xf+xi)/2.0;
        medY=(yf+yi)/2.0;
        medZ=(zf+zi)/2.0;

        distX = (xf-xi);
        distY = (yf-yi);
        distZ = (zf-zi);

   RenderFlag=true;
   CantPuntosSegLabel->Caption=RenderPoints->Count;
   VolRenderLabel->Caption = RenderPoints->Count*
                             atof(RXEdit->Text.c_str())*
                             atof(RYEdit->Text.c_str())*
                             atof(RZEdit->Text.c_str());

   RenderPoints->Sort(CompareColors);

   offset = 0;
   final = RenderPoints->Count;
}

//---------------------------------------------------------------------------
void __fastcall TForm1::Erosion1Click(TObject *Sender)
{
        int zf=atoi(FinSliceEdit->Text.c_str());
        int zi=atoi(IniSliceEdit->Text.c_str());
        int cantCortes = zf-zi;

        int xi = ROIShape->Top;
        int xf = ROIShape->Top+ROIShape->Height;
        int yi = ROIShape->Left;
        int yf = ROIShape->Left+ROIShape->Width;

        //Apertura:
        AnsiString radioString = "8";
        int r;

        if(InputQuery("Ingrese Radio del Elemento Estructurante","Radio:",radioString))
        {
        r = atoi(radioString.c_str());
        }else {return;}

        ProgressBar1->Max=cantCortes-2*r+1;

        //Listas y arrays auxiliares para la creación del elemento estructurante
        //particionado circular de radio=r
        TList* Back = new TList;
        TList* Front = new TList;
        TList* Core = new TList;

        bool*** MorphM;

        short int posminF[3]; //coordenadas x,y,z de la posicion del mínimo de Front
        short int posminC[3]; //coordenadas x,y,z de la posicion del mínimo de Core

        short int posminF2[3]; //coordenadas x,y,z de la posicion del mínimo de Front

         short int minB;
         short int minF;
         short int minC;

         short int minF2;

         short int min;

         unsigned char grupoMin = 'c';

        //Inicializacion de la máscara de morph:
        MorphM = new bool** [2*r+1 + 2];    //+2: extiende las dimensiones de la
        for (int i=0; i<2*r+1 + 2; ++i)     //    máscara para evitar problemas
        {                                   //    de contorno.
        MorphM[i]=new bool* [2*r+1 + 2];

                for (int j=0; j<2*r+1 + 2; ++j)
                {
                MorphM[i][j]=new bool [2*r+1 + 2];
                      for (int k=0; k<2*r+1 + 2; ++k)
                         MorphM[i][j][k]=0;
                }
        }

        //Volumen Auxiliar para guardar resultados:
        Volumen* Maux = new Volumen(xf-xi+1, yf-yi+1, zf-zi+1);

        //Creación del elemento estructurante particionado:
        //strelM(TList* Back, TList* Front, TList* Core, bool*** M)
        //: definida en "strel.h"
        strelM(r,Back, Front, Core, MorphM);

      for (int k=zi+r;k<zf-r;k++)
       {ProgressBar1->Position=k-zi-r;
         for(int j=yi+r; j<yf-r;j++)
            for(int i=xi+r; i<xf-r;i++)
            {
             if(i == xi+r || grupoMin=='b') //Es la primera vuelta o se perdió  registro del mínimo anterior?
             {
             //Erosión: búsqueda del mínimo en Back
                minB=maxPX+1;
                               for (int h = 0; h<Back->Count; h++)
                                        if(M->GetV(i+((Punto*)Back->Items[h])->x,
                                                   j+((Punto*)Back->Items[h])->y,
                                                   k+((Punto*)Back->Items[h])->z)
                                            <minB)
                                                minB = M->GetV(i+((Punto*)Back->Items[h])->x,
                                                   j+((Punto*)Back->Items[h])->y,
                                                   k+((Punto*)Back->Items[h])->z);
             //Búsqueda del mínimo en Core
                minC=maxPX+1;
                               for (int h = 0; h<Core->Count; h++)
                                        if(M->GetV(i+((Punto*)Core->Items[h])->x,
                                                   j+((Punto*)Core->Items[h])->y,
                                                   k+((Punto*)Core->Items[h])->z)
                                                 <minC)
                                                {
                                                minC = M->GetV(i+((Punto*)Core->Items[h])->x,
                                                   j+((Punto*)Core->Items[h])->y,
                                                   k+((Punto*)Core->Items[h])->z);
                                                posminC[0] = i+((Punto*)Core->Items[h])->x;
                                                posminC[1] = j+((Punto*)Core->Items[h])->y;
                                                posminC[2] = k+((Punto*)Core->Items[h])->z;
                                                }
             //Búsqueda del mínimo en Front
                minF=maxPX+1;
                               for (int h = 0; h<Front->Count; h++)
                                        if(M->GetV(i+((Punto*)Front->Items[h])->x,
                                                   j+((Punto*)Front->Items[h])->y,
                                                   k+((Punto*)Front->Items[h])->z)
                                               <minF)
                                               {
                                                minF = M->GetV(i+((Punto*)Front->Items[h])->x,
                                                   j+((Punto*)Front->Items[h])->y,
                                                   k+((Punto*)Front->Items[h])->z);
                                                posminF[0] = i+((Punto*)Front->Items[h])->x;
                                                posminF[1] = j+((Punto*)Front->Items[h])->y;
                                                posminF[2] = k+((Punto*)Front->Items[h])->z;
                                                }

                             //búsqueda del mínimo entre los mínimos:
                             min = minB;
                             grupoMin = 'b';

                             if(minC<=min)
                             {
                             min = minC;
                             grupoMin = 'c';
                             }

                             if(minF<min)
                             {
                             min = minF;
                             grupoMin = 'f';

                             posminC[0] = posminF[0];
                             posminC[1] = posminF[1];
                             posminC[2] = posminF[2];
                             }
                             else if (minF == min)
                             {
                                 if (grupoMin == 'c')
                                   if(posminF[0]>posminC[0])
                                   {
                                   min = minF;
                                   grupoMin = 'f';

                                   posminC[0] = posminF[0];
                                   posminC[1] = posminF[1];
                                   posminC[2] = posminF[2];
                                   }
                             }

             //Escribe en el vol auxiliar:
             Maux->SetV(i-xi-r,j-yi-r,k-zi-r,min);

             } //fin de "¿es la primera vuelta?"  o "se perdió registro de ..."?
             else
             {//Búsqueda del mínimo en Front
                               minF2 = maxPX+1;
                               for (int h = 0; h<Front->Count; h++)
                                            if(M->GetV(i+((Punto*)Front->Items[h])->x,
                                                   j+((Punto*)Front->Items[h])->y,
                                                   k+((Punto*)Front->Items[h])->z)
                                            <minF2)
                                             {
                                             minF2 = M->GetV(i+((Punto*)Front->Items[h])->x,
                                                   j+((Punto*)Front->Items[h])->y,
                                                   k+((Punto*)Front->Items[h])->z);
                                             posminF2[0] = i+((Punto*)Front->Items[h])->x;
                                             posminF2[1] = j+((Punto*)Front->Items[h])->y;
                                             posminF2[2] = k+((Punto*)Front->Items[h])->z;
                                             }

             if(minF2<min || (minF2==min && posminF2[0]>posminC[0]))
             {
             min = minF2;
             grupoMin = 'f';
             posminC[0] = posminF2[0];
             posminC[1] = posminF2[1];
             posminC[2] = posminF2[2];

                 if(!MorphM[posminF2[0]-i+r+1-1][posminF2[1]-j+r+1][posminF2[2]-k+r+1])
                 grupoMin = 'b';
             }
             else  //Si no encontre el minimo en el nuevo Front
             {//Si el punto mínimo fue a parar a back
                 if(!MorphM[posminC[0]-i+r+1-1][posminC[1]-j+r+1][posminC[2]-k+r+1])
                 grupoMin = 'b';
             }

             Maux->SetV(i-xi-r,j-yi-r,k-zi-r,min);
             }//fin de else "¿es la primera vuelta?"

         }//end for i, j
       }//end for k

           ProgressBar1->Position=0;

       //Recupera la imagen dilatada para erosionar:
       for (int k=zi+r;k<zf-r;k++)
       {
           ProgressBar1->Position=k-zi-r;
           for(int j=yi+r; j<yf-r;j++)
            for(int i=xi+r; i<xf-r;i++)
            M->SetV(i,j,k,Maux->GetV(i-xi-r,j-yi-r,k-zi-r));
       }
       ProgressBar1->Position=0;


     delete Maux;
     Maux = NULL;


        for (int i=0; i<2*r+1+2; ++i)
           for (int j=0; j<2*r+1+2; ++j)
                delete[] MorphM[i][j];
        for (int i=0; i<2*r+1+2; ++i)
                delete MorphM[i];
        delete[] MorphM;


     GraficarEnPantalla();

        for(int i=0; i<Back->Count; i++)
               delete Back->Items[0];

        for(int i=0; i<Core->Count; i++)
               delete Core->Items[0];

        for(int i=0; i<Front->Count; i++)
               delete Front->Items[0];
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Dilatacin1Click(TObject *Sender)
{
       int zf=atoi(FinSliceEdit->Text.c_str());
        int zi=atoi(IniSliceEdit->Text.c_str());
        int cantCortes = zf-zi;

        int xi = ROIShape->Top;
        int xf = ROIShape->Top+ROIShape->Height;
        int yi = ROIShape->Left;
        int yf = ROIShape->Left+ROIShape->Width;

        //Apertura:
        AnsiString radioString = "8";

        int r;

        if(InputQuery("Ingrese Radio del Elemento Estructurante","Radio:",radioString))
        {
        r = atoi(radioString.c_str());
        }else {return;}

        ProgressBar1->Max=cantCortes-2*r+1;

        //Listas y arrays auxiliares para la creación del elemento estructurante
        //particionado circular de radio=r
        TList* Back = new TList;
        TList* Front = new TList;
        TList* Core = new TList;
        
        bool*** MorphM;

        short int posmaxF[3]; //coordenadas x,y,z de la posicion del mínimo de Front
        short int posmaxC[3]; //coordenadas x,y,z de la posicion del mínimo de Core

        short int posmaxF2[3]; //coordenadas x,y,z de la posicion del mínimo de Front

         short int maxB;
         short int maxF;
         short int maxC;

         short int maxF2;

         short int max;

         unsigned char grupoMax = 'c';

        //Inicializacion de la máscara de morph:
        MorphM = new bool** [2*r+1 + 2];    //+2: extiende las dimensiones de la
        for (int i=0; i<2*r+1 + 2; ++i)     //    máscara para evitar problemas
        {                                   //    de contorno.
        MorphM[i]=new bool* [2*r+1 + 2];

                for (int j=0; j<2*r+1 + 2; ++j)
                {
                MorphM[i][j]=new bool [2*r+1 + 2];
                      for (int k=0; k<2*r+1 + 2; ++k)
                         MorphM[i][j][k]=0;
                }
        }

        //Volumen Auxiliar para guardar resultados:
        Volumen* Maux = new Volumen(xf-xi+1, yf-yi+1, zf-zi+1);

        //Creación del elemento estructurante particionado:
        //strelM(TList* Back, TList* Front, TList* Core, bool*** M)
        //: definida en "strel.h"
        strelM(r,Back, Front, Core, MorphM);

      for (int k=zi+r;k<zf-r;k++)
       {ProgressBar1->Position=k-zi-r;
         for(int j=yi+r; j<yf-r;j++)
            for(int i=xi+r; i<xf-r;i++)
            {
             if(i == xi+r || grupoMax=='b') //Es la primera vuelta o se perdió  registro del maximo anterior?
             {
             //Dilatación: búsqueda del máximo en Back
                maxB=minPX-1;
                               for (int h = 0; h<Back->Count; h++)
                                        if(M->GetV(i+((Punto*)Back->Items[h])->x,
                                                   j+((Punto*)Back->Items[h])->y,
                                                   k+((Punto*)Back->Items[h])->z)
                                            >maxB)
                                                maxB = M->GetV(i+((Punto*)Back->Items[h])->x,
                                                   j+((Punto*)Back->Items[h])->y,
                                                   k+((Punto*)Back->Items[h])->z);
             //Búsqueda del mínimo en Core
                maxC=minPX-1;
                               for (int h = 0; h<Core->Count; h++)
                                        if(M->GetV(i+((Punto*)Core->Items[h])->x,
                                                   j+((Punto*)Core->Items[h])->y,
                                                   k+((Punto*)Core->Items[h])->z)
                                                 >maxC)
                                                {
                                                maxC = M->GetV(i+((Punto*)Core->Items[h])->x,
                                                   j+((Punto*)Core->Items[h])->y,
                                                   k+((Punto*)Core->Items[h])->z);
                                                posmaxC[0] = i+((Punto*)Core->Items[h])->x;
                                                posmaxC[1] = j+((Punto*)Core->Items[h])->y;
                                                posmaxC[2] = k+((Punto*)Core->Items[h])->z;
                                                }
             //Búsqueda del mínimo en Front
                maxF=minPX-1;
                               for (int h = 0; h<Front->Count; h++)
                                        if(M->GetV(i+((Punto*)Front->Items[h])->x,
                                                   j+((Punto*)Front->Items[h])->y,
                                                   k+((Punto*)Front->Items[h])->z)
                                               >maxF)
                                               {
                                                maxF = M->GetV(i+((Punto*)Front->Items[h])->x,
                                                   j+((Punto*)Front->Items[h])->y,
                                                   k+((Punto*)Front->Items[h])->z);
                                                posmaxF[0] = i+((Punto*)Front->Items[h])->x;
                                                posmaxF[1] = j+((Punto*)Front->Items[h])->y;
                                                posmaxF[2] = k+((Punto*)Front->Items[h])->z;
                                                }

                             //búsqueda del mínimo entre los mínimos:
                             max = maxB;
                             grupoMax = 'b';

                             if(maxC>=max)
                             {
                             max = maxC;
                             grupoMax = 'c';
                             }

                             if(maxF>max)
                             {
                             max = maxF;
                             grupoMax = 'f';

                             posmaxC[0] = posmaxF[0];
                             posmaxC[1] = posmaxF[1];
                             posmaxC[2] = posmaxF[2];
                             }
                             else if (maxF == max)
                             {
                                 if (grupoMax == 'c')
                                   if(posmaxF[0]>posmaxC[0])
                                   {
                                   max = maxF;
                                   grupoMax = 'f';

                                   posmaxC[0] = posmaxF[0];
                                   posmaxC[1] = posmaxF[1];
                                   posmaxC[2] = posmaxF[2];
                                   }
                             }

             //Escribe en el vol auxiliar:
             Maux->SetV(i-xi-r,j-yi-r,k-zi-r,max);

             } //fin de "¿es la primera vuelta?"  o "se perdió registro de ..."?
             else
             {//Búsqueda del mínimo en Front
                               maxF2 = minPX-1;
                               for (int h = 0; h<Front->Count; h++)
                                            if(M->GetV(i+((Punto*)Front->Items[h])->x,
                                                   j+((Punto*)Front->Items[h])->y,
                                                   k+((Punto*)Front->Items[h])->z)
                                            >maxF2)
                                             {
                                             maxF2 = M->GetV(i+((Punto*)Front->Items[h])->x,
                                                   j+((Punto*)Front->Items[h])->y,
                                                   k+((Punto*)Front->Items[h])->z);
                                             posmaxF2[0] = i+((Punto*)Front->Items[h])->x;
                                             posmaxF2[1] = j+((Punto*)Front->Items[h])->y;
                                             posmaxF2[2] = k+((Punto*)Front->Items[h])->z;
                                             }

             if(maxF2>max || (maxF2==max && posmaxF2[0]>posmaxC[0]))
             {
             max = maxF2;
             grupoMax = 'f';
             posmaxC[0] = posmaxF2[0];
             posmaxC[1] = posmaxF2[1];
             posmaxC[2] = posmaxF2[2];

                 if(!MorphM[posmaxF2[0]-i+r+1-1][posmaxF2[1]-j+r+1][posmaxF2[2]-k+r+1])
                 grupoMax = 'b';
             }
             else  //Si no encontre el minimo en el nuevo Front
             {//Si el punto mínimo fue a parar a back
                 if(!MorphM[posmaxC[0]-i+r+1-1][posmaxC[1]-j+r+1][posmaxC[2]-k+r+1])
                 grupoMax = 'b';
             }

             Maux->SetV(i-xi-r,j-yi-r,k-zi-r,max);
             }//fin de else "¿es la primera vuelta?"

         }//end for i, j
       }//end for k

           ProgressBar1->Position=0;

       //Recupera la imagen dilatada para erosionar:
       for (int k=zi+r;k<zf-r;k++)
       {
           ProgressBar1->Position=k-zi-r;
           for(int j=yi+r; j<yf-r;j++)
            for(int i=xi+r; i<xf-r;i++)
            M->SetV(i,j,k,Maux->GetV(i-xi-r,j-yi-r,k-zi-r));
       }
       ProgressBar1->Position=0;

     delete Maux;
     Maux = NULL;

       for (int i=0; i<2*r+1+2; ++i)
           for (int j=0; j<2*r+1+2; ++j)
                delete[] MorphM[i][j];
        for (int i=0; i<2*r+1+2; ++i)
                delete MorphM[i];
        delete[] MorphM;

     GraficarEnPantalla();

        for(int i=0; i<Back->Count; i++)
               delete Back->Items[0];

        for(int i=0; i<Core->Count; i++)
               delete Core->Items[0];

        for(int i=0; i<Front->Count; i++)
               delete Front->Items[0];
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ROIPanelMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
             if(!Goma3DCheckBox->Checked)
                  {
                  paso = PasoTrackBar->Position;

                  if(RotandoImagen)
                  {
                  RotandoImagen = false;
                  }

                  if(Aumentando)
                  {
                  Aumentando = false;
                  }

                  if(Transladando)
                  {
                  Transladando = false;
                  }

                  ReleaseCapture();
                  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ROIPanelMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
           if(!Goma3DCheckBox->Checked && !SelectEsqCheckBox->Checked)
             {
               paso = atoi(PasoEdit->Text.c_str());

               if(Button == mbLeft)
               {
               xP=X;
               yP=Y;
               RotandoImagen = true;
              // SetCaptureControl(Image3D);
               }

               if(Button == mbRight)
               {
               xP=X;
               yP=Y;
               Aumentando = true;
               }

                if(Button == mbMiddle)
               {
               xP=X;
               yP=Y;
               Transladando = true;
               }
             }
             else if (SelectEsqCheckBox->Checked)
             {
               if(Button == mbMiddle)
               {
               EsquelPoints->Pack();

                 if(UnirCheckBox->Checked)
                 {
                  Punto* ap;
                  if(BP->Count>1)
                  {
                  TList* Parche = new TList;
                  Bresenham3D(*(Punto*)BP->Items[0], *(Punto*)BP->Items[1], Parche);

                   for(int k=0; k<BP->Count; k++)
                   {
                   ap = (Punto*)BP->Items[k];
                   EsquelPoints->Add(ap);
                   }

                   for(int k=0; k<Parche->Count; k++)
                   {
                   ap = (Punto*)Parche->Items[k];
                   EsquelPoints->Add(ap);
                   }

                  Parche->Clear();
                  delete Parche;
                  BP->Clear();
                  }
                 }

               EP->Clear();
               LP->Clear();
               JP->Clear();
               EraseP->Clear();
               BP->Clear();
               clasificacionEsqueleto(EsquelPoints,
                                minX, minY, minZ,
                                maxX, maxY, maxZ,
                                EP, LP, JP);
               }
             }//else
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ROIPanelMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
//¿Borrar o Rotar/Transladar?

if(!Goma3DCheckBox->Checked && !SelectEsqCheckBox->Checked)
{
//Rotar/Transladar:
// if user has left mouse button down then ...
    if(Shift.Contains(ssLeft) && RotandoImagen)
    {
    // ... rotate the view based on thier mouse movement

     if(ZnormCheckBox->Checked)
     {
     elevacion += (Y-yP) * 0.333;
     azimuth   += (X-xP) * 0.333;
     }
     else
     {
     elevacion -= (Y-yP) * 0.333;
     azimuth   -= (X-xP) * 0.333;
     }
    }

// if user has right mouse button down then ...
    if(Shift.Contains(ssRight) && Aumentando)
    {
    // ... zoom the view based on mouse movement
        distancia+=(yP-Y);
       // FormResize(ROIPanel);
    }

    //Transladar:
    if(Shift.Contains(ssMiddle) && Transladando)
    {
    // ... translate the view based on mouse movement
        xTrans+=(X-xP)/2.0f;
        yTrans+=(yP-Y)/2.0f;
    }

    xP=X;
    yP=Y;
}  else
    {
    //Borrar:
    //********************************************************************
    if(Shift.Contains(ssLeft) && !AjusteCheckBox->Checked)
    {
     Goma3DCoef =  atoi(Goma3DEdit->Text.c_str());

     int* indice;

     for(int i=X-Goma3DCoef; i<X+Goma3DCoef; i++)
        for(int j=Y-Goma3DCoef; j<Y+Goma3DCoef; j++)
        {
                for(int k=0;k<MapaIndices[i][j]->Count;k++)
                {
                indice = new int[1];
                indice = (int*)MapaIndices[i][j]->Items[k];
                IndicesBorrar->Add(indice);
                }
                MapaIndices[i][j]->Clear();
        }

        Punto* np;

        for(int i=0; i<IndicesBorrar->Count;i++)
        {
        indice = (int*)IndicesBorrar->Items[i];
        //RenderPoints->Delete(indice[0]);
        np = (Punto*)RenderPoints->Items[indice[0]];
        if(VolcarCheckBox->Checked)
        M->SetV(np->x,np->y,np->z,atoi(ResaltarEdit->Text.c_str()));
        RenderPoints->Items[indice[0]]=NULL;
        }

        IndicesBorrar->Clear();

        CantPuntosSegLabel->Caption=RenderPoints->Count;
        VolRenderLabel->Caption = RenderPoints->Count*
                             atof(RXEdit->Text.c_str())*
                             atof(RYEdit->Text.c_str())*
                             atof(RZEdit->Text.c_str());

      }//end If
      //********************************************************************

    //Resaltar para ajuste:
    //********************************************************************
    if(Shift.Contains(ssLeft) && AjusteCheckBox->Checked)
    {
     Goma3DCoef =  atoi(Goma3DEdit->Text.c_str());

     int* indice;

     for(int i=X-Goma3DCoef; i<X+Goma3DCoef; i++)
        for(int j=Y-Goma3DCoef; j<Y+Goma3DCoef; j++)
        {
                for(int k=0;k<MapaIndices[i][j]->Count;k++)
                {
                indice = new int[1];
                indice = (int*)MapaIndices[i][j]->Items[k];
                IndicesBorrar->Add(indice);
                }
                MapaIndices[i][j]->Clear();
        }

        RenderPunto* ap;
        RenderPunto* np;

        for(int i=0; i<IndicesBorrar->Count;i++)
        {
        indice = (int*)IndicesBorrar->Items[i];

        ap = (RenderPunto*)RenderPoints->Items[indice[0]];

        np = new RenderPunto;
        np->p.x = ap->p.x;
        np->p.y = ap->p.y;
        np->p.z = ap->p.z;

        np->n.x = ap->n.x;
        np->n.y = ap->n.y;
        np->n.z = ap->n.z;

        np->norma = ap->norma;

        np->p.color = ap->p.color;
        SelectedPoints->Add(np);
        }

        IndicesBorrar->Clear();

      }//end If
      //********************************************************************

     }//end else

    //Seleccion de Esqueleto:
    //********************************************************************
    if(SelectEsqCheckBox->Checked && !UnirCheckBox->Checked)
    {
     int coef;

     if(distancia!=0)
     coef = fabs(60.0/distancia);
     else coef = 60.0;

     if(Shift.Contains(ssLeft))
     {
      Punto* ap;

      for(int k = 0; k<EsquelPoints->Count; k++)
      {
      if(ap=(Punto*)EsquelPoints->Items[k])
       if(ap->xR<X+coef)
        if(ap->xR>X-coef)
         if(ap->yR<Y+coef)
          if(ap->yR>Y-coef)
          {
          EsquelPoints->Items[k]=NULL;
          EraseP->Add(ap);
          }
       }
      }
    }//end if(SelectEsqCheckBox->Checked)

    if(SelectEsqCheckBox->Checked && UnirCheckBox->Checked)
    {
    int coef;

     if(distancia!=0)
     coef = fabs(60.0/distancia);
     else coef = 60.0;

     if(Shift.Contains(ssLeft))
     {
      Punto* ap;

      for(int k = 0; k<EsquelPoints->Count; k++)
      {
      if(ap=(Punto*)EsquelPoints->Items[k])
       if(ap->xR<X+coef)
        if(ap->xR>X-coef)
         if(ap->yR<Y+coef)
          if(ap->yR>Y-coef)
          {
          BP->Add(ap);
          EsquelPoints->Items[k]=NULL;
          }
       }
      }
    }
   //********************************************************************

}
//---------------------------------------------------------------------------

void __fastcall TForm1::TabSheetChange(TObject *Sender)
{
         /*
    if(TabSheet->ActivePage==ROISheet || TabSheet->ActivePage==OrthoViewSheet)
    {
    PageControl1->Visible = false;
    SetupRenderingContextPanel(OrthoPanel, hDC, hRC, PanelWidth, PanelHeight, PixelFormat);
    FormResize(Sender);

    PageControl1->Visible = true;
    SetupRenderingContextPanel(ROIPanel, hDC, hRC, PanelWidth, PanelHeight, PixelFormat);
    FormResize(Sender);

     if(ImagenEnPantalla)
     {

     cantTexturas = cantImagenes-1;
     texturas = new GLuint[cantTexturas];

     GLubyte bits[512][512][3];

     bits = new GLubyte***[cantTexturas];

      for(int ct = 0; ct<cantTexturas; ct++)
      {
        bits[ct] = new GLubyte**[512];

         for(int i = 0; i < 512; i++)
         {
         bits[ct][i] = new GLubyte*[512];
         for(int j = 0; j < 512; j++)
         {
          bits[ct][i][j] = new GLubyte[3];

                for(int k=0; k<3; k++)
                bits[ct][i][j][k] = 0;
         }
        }
       }


      for(int i = 0; i < 512; i++)
        for(int j = 0; j < 512; j++)
          for(int k=0; k<3; k++)
                bits[i][j][k] = 0;

     RefreshOrtho(Sender);

     SetupTexturesFromMatrix(M,minPX,altoPX, anchoPX, cantImagenes,LUT, 'z',
                                ZScrollBar->Position,textura1);
     SetupTexturesFromMatrix(M,minPX,altoPX, anchoPX, cantImagenes,LUT, 'x',
                                XScrollBar->Position,textura2);
     SetupTexturesFromMatrix(M,minPX,altoPX, anchoPX, cantImagenes,LUT, 'y',
                                YScrollBar->Position,textura3);


     int alto,ancho;
     alto = 512;
     ancho = 512;
     int posicion;

      for(int t=0; t<cantTexturas; t++)
      {
      posicion = t;
       for(int i = 0; i < alto; i++)
        for(int j = 0; j < ancho; j++)
        {

        bits[i][j][0]= (GLbyte)LUT[M->GetV(i,j,posicion)-minPX];
        bits[i][j][1]= (GLbyte)LUT[M->GetV(i,j,posicion)-minPX];
        bits[i][j][2]= (GLbyte)LUT[M->GetV(i,j,posicion)-minPX];

        }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &texturas[t]);
    glBindTexture(GL_TEXTURE_2D, texturas[t]);
           */
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

   // glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
   // glGenTextures(1, &texturas[t]);
   // glBindTexture(GL_TEXTURE_2D, texturas[t]);

  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

  /*
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, bits);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

      }

      RenderFlag = true;

      SetupTexturesFromMatrixPosition(M, minPX,altoPX, anchoPX, cantImagenes,LUT, texturas[i], i, &bits[i]);


      for(int ct = 0; ct<cantTexturas; ct++)
      {

      for(int i = 0; i < 512; i++)
        for(int j = 0; j < 512; j++)
        delete[] bits[ct][i][j];

        for(int i = 0; i < 512; i++)
        delete[] bits[ct][i];

        delete[] bits[ct];
      }

      delete[] bits;
     //SetupTexturesFromBMP(XYImage, textura1, bitmap);
     //SetupTexturesFromBMP(YZImage, textura2, bitmap);
     //SetupTexturesFromBMP(ZXImage, textura3, bitmap);
     }
    }
     */

  /* if (TabSheet->ActivePage==ROISheet)
   {
    PageControl1->Visible = true;
    SetupRenderingContextPanel(ROIPanel, hDC, hRC, PanelWidth, PanelHeight, PixelFormat);
    FormResize(Sender);
   }  */

    if (TabSheet->ActivePage==ImagenSheet)
    {
    PageControl1->Visible = true;
    }



    if (TabSheet->ActivePage==ROISheet)
    {

    if(!ImagenEnPantalla)
    return;

    PasoTrackBar->Enabled = true;
    PasoEdit->Enabled = true;
    ZnormCheckBox->Enabled = true;
    MinzScrollBar->Enabled = true;
    MaxzScrollBar->Enabled = true;

    if(!firstRender)
    return;
    else firstRender = false;

    PageControl1->Visible = true;

    SetupRenderingContextPanel(ROIPanel, hDC, hRC, PanelWidth, PanelHeight, PixelFormat);
    FormResize(Sender);

    CrearTexturas();

    ProgressBar1->Max = cantImagenes;
    ProcessLabel->Caption = "Rendering 3D Volume";
    ProcessLabel->Visible = true;
    ProcessLabel->Refresh();
    ProcessLabel->Repaint();

    for(int i=0; i<cantImagenes;i++)
    {
    ProgressBar1->StepIt();
    LlenarPlanoConMatriz(i);
    ActualizarTextura3D(i);
    }

    ProgressBar1->Position = 0;
    ProcessLabel->Visible = false;
    mostrarTex3D = true;

    RefreshLUT();

    }

    if(TabSheet->ActivePage==ImagenSheet)
    {
    PasoTrackBar->Enabled  = false;
    PasoEdit->Enabled      = false;
    ZnormCheckBox->Enabled = false;
    MinzScrollBar->Enabled = false;
    MaxzScrollBar->Enabled = false;
    }



}
//---------------------------------------------------------------------------
void __fastcall TForm1::RefreshOrtho(TObject *Sender)
{
        bool cambiaXY;
         bool cambiaYZ;
          bool cambiaZX;

        TScrollBar* senderscroll;

        if(AnsiString(Sender->ClassName())=="TPageControl")
        {
        cambiaXY=true;
        cambiaYZ=true;
        cambiaZX=true;
        }
        else  if (AnsiString(Sender->ClassName())=="TScrollBar")
        {
        senderscroll = dynamic_cast<TScrollBar*>(Sender);

        if(senderscroll->Tag==0)
        cambiaZX = true;

        if(senderscroll->Tag==1)
        cambiaXY = true;

        if(senderscroll->Tag==2)
        cambiaYZ = true;
        }

        int y = YScrollBar->Position;
        YShape->Left = XYImage->Left
        + float(XYImage->Width)/float(YScrollBar->Max)*float(YScrollBar->Position);
        YShape2->Top = YZImage->Top
        +  float(XYImage->Height)/float(YScrollBar->Max)*float(YScrollBar->Position);

        int x = XScrollBar->Position;
        XShape->Left = ZXImage->Left
        + float(ZXImage->Width)/float(XScrollBar->Max)*float(XScrollBar->Position);
        XShape2->Top = XYImage->Top
        +  float(XYImage->Height)/float(XScrollBar->Max)*float(XScrollBar->Position);

        int z = ZScrollBar->Position;
        ZShape->Left = YZImage->Left
        + float(YZImage->Width)/float(ZScrollBar->Max)*float(ZScrollBar->Position);
        ZShape2->Top = ZXImage->Top
        +  float(ZXImage->Height)/float(ZScrollBar->Max)*float(ZScrollBar->Position);

        float propx, propy, propz;
        propx = float(anchoPX-1)/float(XYImage->Width);
        propy = float(altoPX-1)/float(YZImage->Height);
        propz = float(cantImagenes-1)/float(ZXImage->Height);

               //................................................
               if(cambiaXY)
               for(int i=0; i<XYImage->Width;i++)
                 for(int j=0; j<XYImage->Height;j++)
                 {
                 CuadroXY->buffer(i,j)=LUT[M->GetV(int(propx*float(i)),
                                                   int(propy*float(j)),z)
                                            -minPX];
                 }
                 CuadroXY->Graficar(XYImage);
                //................................................
                if(cambiaYZ)
                 for(int i=0; i<YZImage->Width;i++)
                 for(int j=0; j<YZImage->Height;j++)
                 {
                 CuadroYZ->buffer(i,j)=LUT[M->GetV(x,int(propy*float(i)),
                                            int(propz*float(j)))
                                            -minPX];
                 }
                 CuadroYZ->Graficar(YZImage);
                 //................................................
                 if(cambiaZX)
                 for(int i=0; i<ZXImage->Width;i++)
                 for(int j=0; j<ZXImage->Height;j++)
                 {
                 CuadroZX->buffer(i,j)=LUT[M->GetV(int(propx*float(j)),y,
                                              int(propz*float(i)))
                                              -minPX];
                 }

                 CuadroZX->Graficar(ZXImage);
                //................................................
                //Zoom Vecino + cercano:
                float x0, y0;
                x0 = propx*float(XShape2->Top-XYImage->Top);
                y0 = propy*float(YShape->Left-XYImage->Left);

                int zoomindex = ZoomRadioGroup->ItemIndex;
                if(zoomindex == 0 || zoomindex == 1)
                zoomindex = zoomindex +1;
                else
                zoomindex = zoomindex*2;

                int distindex = ZoomImage->Width/2.0/zoomindex;
                x0 = x0-distindex;
                y0 = y0-distindex;

                ZoomShape->Width = float(ZoomImage->Width)*1.0/propx/zoomindex;
                ZoomShape->Height = float(ZoomImage->Height)*1.0/propx/zoomindex;
                ZoomShape->Top = 1.0/propx*x0 + XYImage->Top;
                ZoomShape->Left= 1.0/propy*y0 + XYImage->Left;

                float xa, xp, ya, yp;
                float c1, c2, c3, c4;
                float vint;

                for(int i=0; i<ZoomImage->Width;i++)
                 for(int j=0; j<ZoomImage->Height;j++)
                 {
                  xa = floor(x0+1.0/zoomindex*i);
                   xp = ceil( x0+1.0/zoomindex*i);
                   ya = floor(y0+1.0/zoomindex*j);
                   yp = ceil( y0+1.0/zoomindex*j);

                 if(xa>-1 && xp<anchoPX && ya>-1 && yp<altoPX)
                  {
                   c1 = float(xp-x0-1.0/zoomindex*i);
                   c2 = float(x0+1.0/zoomindex*i-xa);
                   c3 = float(yp-y0-1.0/zoomindex*j);
                   c4 = float(y0+1.0/zoomindex*j-ya);

                   vint = c1*c3*float(M->GetV(xa,ya,z)) + c1*c4*float(M->GetV(xa,yp,z))+
                          c2*c3*float(M->GetV(xp,ya,z)) + c2*c4*float(M->GetV(xp,yp,z));

                  CuadroZoom->buffer(i,j)=LUT[int(vint)-minPX];
                  }
                  else CuadroZoom->buffer(i,j)=0;
                 }

                 CuadroZoom->Graficar(ZoomImage);
                //................................................

                cambiaXY=false;
                cambiaYZ=false;
                cambiaZX=false;

}
//---------------------------------------------------------------------------
void __fastcall TForm1::YScrollBarChange(TObject *Sender)
{
     if(ImagenEnPantalla)
     {
     RefreshOrtho(Sender);

     TScrollBar* senderscroll = dynamic_cast<TScrollBar*>(Sender);

     if (senderscroll == NULL)
     return;

     if(senderscroll->Tag==0)
     SetupTexturesFromMatrix(M,minPX,altoPX, anchoPX, cantImagenes,LUT, 'y',
                                YScrollBar->Position,textura3);
     //SetupTexturesFromBMP(ZXImage, textura3, bitmap);

     if(senderscroll->Tag==1)
          SetupTexturesFromMatrix(M,minPX,altoPX, anchoPX, cantImagenes,LUT, 'z',
                                ZScrollBar->Position,textura1);
     //SetupTexturesFromBMP(XYImage, textura1, bitmap);

     if(senderscroll->Tag==2)
     SetupTexturesFromMatrix(M,minPX,altoPX, anchoPX, cantImagenes,LUT, 'x',
                                XScrollBar->Position,textura2);

     //SetupTexturesFromBMP(YZImage, textura2, bitmap);

     IdleLoop(Sender,1);
     }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::RXEditChange(TObject *Sender)
{
      //res[0]= atof(RXEdit->Text.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RYEditChange(TObject *Sender)
{
      //res[1]= atof(RYEdit->Text.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RZEditChange(TObject *Sender)
{
       //res[2]= atof(RZEdit->Text.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MascaraButtonClick(TObject *Sender)
{
        // Cerrar1Click(MascaraButton);
        // AbrirVolumen1Click(MascaraButton);

           Punto* ap;// = new Punto;
           short int color;

      /* for(int k=0;k<RenderPoints->Count;k++)
              if(ap = (Punto*)RenderPoints->Items[k])
              {
              color = ap->color;
              } */

        /*for (int i=0; i<altoPX; i++)
                for (int j=0; j<anchoPX; j++)
                      for (int k=0; k<cantImagenes; k++)
                      M->SetV(i,j,k,minPX);
        */

        double acum = 0;
        double acum2 = 0;
        double cont=0;

        for(int k=0;k<RenderPoints->Count;k++)
              if(ap = (Punto*)RenderPoints->Items[k])
              {
              ap->color = M->GetV(ap->x,ap->y,ap->z);
              acum+=M->GetV(ap->x,ap->y,ap->z);
              acum2+=M->GetV(ap->x,ap->y,ap->z)*M->GetV(ap->x,ap->y,ap->z);
              M->SetV(ap->x,ap->y,ap->z,0);
              cont++;
              }

      mediaRender = acum/cont;
      desvRender = sqrt(cont*acum2-acum*acum)/cont;

      MediaRenderLabel->Caption = mediaRender;
      DesvRenderLabel->Caption = desvRender;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::FixCheckBoxClick(TObject *Sender)
{
        if(FixCheckBox->Checked)
        {
        CenterEdit->Enabled = false;
        CenterTrackBar->Enabled = false;
        }else
        {
        CenterEdit->Enabled = true;
        CenterTrackBar->Enabled = true;
        }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::EsqueletoButtonClick(TObject *Sender)
{
           if(!RenderPoints->Count)
           return;

           EsquelPoints->Clear();
           EP->Clear();
           LP->Clear();
           JP->Clear();
           BP->Clear();
           EraseP->Clear();

           RenderPoints->Pack();
           RenderPoints->Capacity = RenderPoints->Count;

           Punto* ap;
           ap = (Punto*)RenderPoints->Items[0];

           minX = ap->x;
           maxX = ap->x;

           minY = ap->y;
           maxY = ap->y;

           minZ = ap->z;
           maxZ = ap->z;

           for(int i=0; i<RenderPoints->Count; i++)
           {
              ap = (Punto*)RenderPoints->Items[i];

              if(ap->x<minX)
              minX = ap->x;

              if(ap->x>maxX)
              maxX = ap->x;

              if(ap->y<minY)
              minY = ap->y;

              if(ap->y>maxY)
              maxY = ap->y;

              if(ap->z<minZ)
              minZ = ap->z;

              if(ap->z>maxZ)
              maxZ = ap->z;
           }
           //Palagy: esqueletonización de los RenderPoints:
           PALAGY(RenderPoints, EsquelPoints,
                  minX-1, minY-1, minZ-1, maxX+1, maxY+1, maxZ+1);
           
           //2do: Clasificación de los EsquelPoints:
           clasificacionEsqueleto(EsquelPoints,
                                minX, minY, minZ,
                                maxX, maxY, maxZ,
                                EP, LP, JP);

            EsquelPoints->Pack();

                                
           
}
//---------------------------------------------------------------------------

void __fastcall TForm1::EliminarEsqButtonClick(TObject *Sender)
{
      EsquelPoints->Clear();
      EP->Clear();
      LP->Clear();
      JP->Clear();
      BP->Clear();
      EraseP->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SelectEsqCheckBoxClick(TObject *Sender)
{
           Punto* paux = new Punto;
           int xrot, yrot;

              GLint viewport[4],realy;
              GLdouble mvmatrix[16],projmatrix[16],wx,wy,wz;

              glGetIntegerv(GL_VIEWPORT, viewport);//leo el viewport
              glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);//leo matriz de view
              glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);//leo matriz de proyeccion

              int* indice;

              for(int i=0; i<EsquelPoints->Count; i++)
              {
               if(paux = (Punto*)EsquelPoints->Items[i])
               {
               gluProject((GLdouble)paux->x-medX,(GLdouble)paux->y-medY,-(GLdouble)paux->z+medZ,mvmatrix,projmatrix,viewport,&wx,&wy,&wz);
               xrot = wx;
               yrot = viewport[3]-wy-1; //corrige el cambio de coordenadas
               paux->xR=xrot;   //utiliza mean y StD como auxiliares para guardar x,y
               paux->yR=yrot;    // tras el cálculo de la rotación
               }
              }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MarchingCubes1Click(TObject *Sender)
{

       AnsiString paso = 1;
       int pasoi = 1;

        if(InputQuery("Tamaño del Cubo", "Ingrese el tamaño del cubo", paso))
        pasoi=atoi(paso.c_str());
        else
        return;

        int zfn,zin;

        if(FinSliceEdit->Text!="")
                zfn=atoi(FinSliceEdit->Text.c_str());
        else zfn=SliceListBox->ItemIndex;

        if(IniSliceEdit->Text!="")
                zin=atoi(IniSliceEdit->Text.c_str());
        else zin=SliceListBox->ItemIndex-1;

        if(!ROIShape->Visible)
         return;

        int xin = ROIShape->Top;
        int xfn = ROIShape->Top+ROIShape->Height;
        int yin = ROIShape->Left;
        int yfn = ROIShape->Left+ROIShape->Width;

        //Carga lista RenderPoints con todos los puntos de borde:
        ProgressBar1->Max = zfn-zin;
        ProgressBar1->Position=0;

        //Creacion del volumen auxiliar:
        Volumen* NV = new Volumen(xfn-xin+4, yfn-yin+4, zfn-zin+4);

        for(int i = xin; i<xfn; i++)
                for(int j = yin; j<yfn; j++)
                        for(int k = zin; k<zfn; k++)
                        {
                        NV->SetV(i-xin+1,j-yin+1,k-zin+1,M->GetV(i,j,k));
                        }
                        
       int nv = TriangList->Count;

       if(pasoi>0)
       marchingCubes(pasoi,pasoi,pasoi, NV, TriangList, Center, 0);
       if(pasoi<0)
       {
       pasoi=abs(pasoi);
       marchingCubes(pasoi,pasoi,pasoi, NV, TriangList, Center, 1);
       }

       delete NV;

       RenderFlag = true;

       //Creacion de la red:
       PuntoF* ap; //Puntero a PuntoF auxiliar

       Lado_Red**** Grid;
       Grid=new Lado_Red*** [(xfn-xin)*2+4];

        for (int i=0; i<(xfn-xin)*2+4; ++i)
        {
        Grid[i]=new Lado_Red** [(yfn-yin)*2+4];

                for (int j=0; j<(yfn-yin)*2+4; ++j)
                {
                Grid[i][j]=new Lado_Red* [(zfn-zin)*2+4];

                for (int k=0; k<(zfn-zin)*2+4; ++k)
                        Grid[i][j][k]=NULL;
                }
        }

       //Clasificacion de los Triangulos:
       int indx, indy, indz;
       Tri* triaux;

       TList* indList = new TList;

       short int* indices_utiles;

       int cont=0;

       for(int k=nv;k<TriangList->Count;k++)
       {
       ap = (PuntoF*)TriangList->Items[k];

       indx = ceil(ap->x)+ floor(ap->x);
       indy = ceil(ap->y)+ floor(ap->y);
       indz = ceil(ap->z)+ floor(ap->z);

         //Si es la primera vez que se cae en un Lado de Grid
         if(Grid[indx][indy][indz]==NULL)
         {
         Grid[indx][indy][indz] = new Lado_Red;
         Grid[indx][indy][indz]->Vertice = ap;
         cont++;
         Grid[indx][indy][indz]->Triangs = new TList;

         indices_utiles = new short int[3];
         indices_utiles[0] = indx;
         indices_utiles[1] = indy;
         indices_utiles[2] = indz;
         indList->Add(indices_utiles);

         triaux = new Tri;
         triaux->tri_num = k/3;
         triaux->vert_num = k%3;

         Grid[indx][indy][indz]->Triangs->Add(triaux);

         triaux;
         
         ap = NULL;
         }
         else //si no es la primera vez que se cae en ese lado
         {
         triaux = new Tri;
         triaux->tri_num = k/3;
         triaux->vert_num = k%3;

         Grid[indx][indy][indz]->Triangs->Add(triaux);

         triaux = NULL;

         delete ap;
         ap = NULL;
         }
       }

       //Ya no se necesita TriangList de esa forma:
       PuntoI* indices;

       TriList->Capacity = TriangList->Count/3;
       TriList->Count = TriangList->Count/3;

        //Inicializacion de TriList para mejorar performance:
        for(int i=nv; i<TriList->Count; i++)
        {
        indices = new PuntoI;
        TriList->Items[i]=indices;
        }

       TriangList->Clear();

        //Creacion de las listas de triangulos y vertices útiles:
        for(int k=nv; k<indList->Count; k++)
        {
        indices_utiles = (short int*)indList->Items[k];

        TriangList->Add(Grid[indices_utiles[0]][indices_utiles[1]][indices_utiles[2]]->Vertice);
        Grid[indices_utiles[0]][indices_utiles[1]][indices_utiles[2]]->Vertice = NULL;

                for(int i=0; i<Grid[indices_utiles[0]][indices_utiles[1]][indices_utiles[2]]->Triangs->Count;i++)
                {
                triaux = (Tri*)Grid[indices_utiles[0]][indices_utiles[1]][indices_utiles[2]]->Triangs->Items[i];
                indices = (PuntoI*)TriList->Items[triaux->tri_num];
                indices->xyz[triaux->vert_num]=k;
                }
        }

        float nx, ny, nz;
        float v1x,v1y,v1z; //vector: p3-p1
        float v2x,v2y,v2z; //vector: p2-p1
        float snx, sny, snz; //Suma de normales, componentes x,y,z.
        float norma;

        PuntoF* ap1, *ap2, *ap3;

        //Búsqueda de las normales promediadas para graficación:
        for(int k=0; k<indList->Count; k++)
        {
        indices_utiles = (short int*)indList->Items[k];
        snx=0;
        sny=0;
        snz=0;

           for(int i=0;
               i<Grid[indices_utiles[0]][indices_utiles[1]][indices_utiles[2]]->Triangs->Count;
               i++)
           {
           triaux = (Tri*)Grid[indices_utiles[0]]
                              [indices_utiles[1]]
                              [indices_utiles[2]]->Triangs->Items[i];

           indices = (PuntoI*)TriList->Items[triaux->tri_num];

           ap1 = (PuntoF*)TriangList->Items[indices->xyz[0]];
           ap2 = (PuntoF*)TriangList->Items[indices->xyz[1]];
           ap3 = (PuntoF*)TriangList->Items[indices->xyz[2]];

           v1x = ap2->x-ap1->x; v1y = ap2->y-ap1->y; v1z = ap2->z-ap1->z;
           v2x = ap3->x-ap1->x; v2y = ap3->y-ap1->y; v2z = ap3->z-ap1->z;

           nx = v1y*v2z-v1z*v2y;
           ny = v1z*v2x-v1x*v2z;
           nz = v1x*v2y-v1y*v2x;

           snx=snx+nx;
           sny=sny+ny;
           snz=snz+nz;
           }

           snx=snx/float(Grid[indices_utiles[0]][indices_utiles[1]][indices_utiles[2]]->Triangs->Count);
           sny=sny/float(Grid[indices_utiles[0]][indices_utiles[1]][indices_utiles[2]]->Triangs->Count);
           snz=snz/float(Grid[indices_utiles[0]][indices_utiles[1]][indices_utiles[2]]->Triangs->Count);

           norma = sqrt(snx*snx+sny*sny+snz*snz);

           snx=snx/norma;
           sny=sny/norma;
           snz=snz/norma;

           ap = new PuntoF;
           ap->x=snx; ap->y=sny; ap->z=snz;
           Normals->Add(ap);
        }

        //Borrar Lista de Indices Utiles:
        for(int i=0; i<indList->Count; i++)
        {
        indices_utiles = (short int*)indList->Items[i];
        delete[] indices_utiles;
        }
        indList->Clear();

        //Borrar la Grilla de Lados:
        for (int i=0; i<(xfn-xin)*2+4; ++i)
         for (int j=0; j<(yfn-yin)*2+4; ++j)
         {
          for (int k=0; k<(zfn-zin)*2+4; ++k)
          {

           if(Grid[i][j][k]!=NULL)
           {
            for(int l=0; l<Grid[i][j][k]->Triangs->Count;l++)
            {
            triaux = (Tri*)Grid[i][j][k]->Triangs->Items[l];
            delete triaux;
            }
            Grid[i][j][k]->Triangs->Clear();
            delete Grid[i][j][k]->Triangs;
            }

           delete Grid[i][j][k];
          }

          delete[] Grid[i][j];
         }

         for (int i=0; i<(xfn-xin)*2+4; ++i)
         delete[] Grid[i];

         delete Grid;

       //Reacomodacion de los puntos del triangulo
       for(int i=nv; i<TriangList->Count; i++)
       {
       ap = (PuntoF*)TriangList->Items[i];
       int aca =0;
       ap->x += xin-1;ap->y += yin-1;ap->z += zin-1;
       }

       medX = (xin+xfn)/2;
       medY = (yin+yfn)/2;
       medZ = (zin+zfn)/2;
       
     /*
       //Cálculo de los colores:
       int xi,yi,zi,xf,yf,zf;
       PuntoF *ap1,*ap2,*ap3;
       float ax,ay,az,bx,by,bz;
       float valor1,valor2,valor3;
       float* col;
       PuntoF* normal;
       float norma;
       float v1[3];
       float v2[3];

       for(int k=nv;k<TriangList->Count;k=k+3)
      {
        //Vertices del triangulo:
        ap1 = (PuntoF*)TriangList->Items[k];
        ap2 = (PuntoF*)TriangList->Items[k+1];
        ap3 = (PuntoF*)TriangList->Items[k+2];

        normal = new PuntoF;

        v1[0] = ap2->x-ap1->x;
        v1[1] = ap2->y-ap1->y;
        v1[1] = ap2->z-ap1->z;

        v2[0] = ap3->x-ap1->x;
        v2[1] = ap3->y-ap1->y;
        v2[1] = ap3->z-ap1->z;

        normal->x = v1[1]*v2[2]-v1[2]*v2[1];
        normal->y = v1[2]*v2[0]-v1[0]*v2[2];
        normal->z = v1[0]*v2[1]-v1[1]*v2[0];

        norma = sqrt(normal->x*normal->x + normal->y*normal->y + normal->z*normal->z);

        normal->x = normal->x/norma;
        normal->y = normal->y/norma;
        normal->z = normal->z/norma;

        Normals->Add(normal);

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

        col = new float;
        *col = valor1;
        TriangColors->Add(col);

        col = new float;
        *col = valor2;
        TriangColors->Add(col);

        col = new float;
        *col = valor3;
        TriangColors->Add(col);
       }
       */
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Suma1Click(TObject *Sender)
{
 OpenDialog1->Filter = "BIN files (*.bin)|*.BIN";
       if(OpenDialog1->Execute())
       {
       FILE* fp;
       fp = fopen( OpenDialog1->FileName.c_str(),  "r+b");
       encabezado enc1;
       short int cortes;
       short int minFrame;
       //size_t fread(void *ptr, size_t num_bytes, size_t count, FILE *stream);

       fread(&cortes,sizeof(short int),1,fp);
       fread(&minFrame,sizeof(short int),1,fp);
       fread(&enc1,sizeof(encabezado),1,fp);

       short int ancho = enc1.ancho;
       short int alto = enc1.alto;
       short int corte = enc1.frame;

                if(cortes==cantImagenes && ancho==anchoPX && alto==altoPX)
                {
                ProgressBar1->Position=0;

                        for (int k=0;k<cantImagenes+1;k++)
                        {
                        ProgressBar1->Position=k;
                                for(int i=0; i<enc1.ancho;i++)
                                        for(int j=0; j<enc1.alto;j++)
                                        {
                                        short int valor;
                                        fread(&valor,sizeof(short int),1,fp);
                                        M->SetV(i,j,corte-minFrame,
                                        valor+(M->GetV(i,j,corte-minFrame)));

                                                //Evita Saturación:
                                                if(M->GetV(i,j,corte-minFrame)<minPX)
                                                M->SetV(i,j,corte-minFrame,minPX);
                                                if(M->GetV(i,j,corte-minFrame)>maxPX)
                                                M->SetV(i,j,corte-minFrame,maxPX);
                                        }

                        fread(&enc1,sizeof(encabezado),1,fp);
                        corte = enc1.frame;

                        }

                fclose( fp );
                ProgressBar1->Position=0;
                }
       }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::HistoChartDblClick(TObject *Sender)
{
       int color = 0;
       for(int i=realminPX-minPX; i<maxPX-minPX+1;i++)
       {
       //Calcular Color por LUT
       color = int(LUT[i]);
       HistoChart->Series[0]->AddXY(double(i+minPX),double(histoCaja[i]),"",
       color*16*16*16*16+color*16*16+color);
       }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RefreshLUT()
{
        if(ImagenEnPantalla==true)
        {

         if(!FixCheckBox->Checked)
         {
         Li = Center-Width;
         Ls = Center+Width;
         }
         else
         {
         Ls = Center+Width;
         Center = (Ls+Li)/2;
         }

         if(mostrarTex3D)
         {

         float valor_intensidad;
         float valor_alfa;
         int iL = 0;

         for(int i=0;i<tfSize;i++)
            {

            iL = float(i)*float(maxPX-minPX)/float(tfSize);

            if(iL< (Center-minPX)-Width/2.0)
            {
            valor_intensidad = 0.0;
            valor_alfa = 0.0;
            }

            if(iL <= (Center-minPX)+Width/2.0 && iL>= (Center-minPX)-Width/2.0)
            {
            valor_intensidad = 1.0/float(Width)*float(iL-Center+minPX + Width/2.0);
            valor_alfa = valor_intensidad/5.0;

            if(BinCheckBox->Checked && iL<Center)
            {
             valor_intensidad = 0.0;
             valor_alfa = valor_intensidad/5.0;
            }

            if(BinCheckBox->Checked && iL>Center)
            {
             valor_intensidad = 1.0;
             valor_alfa = valor_intensidad/5.0;
            }
            //valor_alfa = 1.0;
            }

            if(iL > (Center-minPX)+Width/2.0)
            {
              if(CropCheckBox->Checked)
              {
              valor_intensidad = 0.0;
              valor_alfa = 0.0;
              }
              else
              {
              valor_intensidad = 1.0;
              valor_alfa = 1.0/5.0;
              }
            }

            pTransferFunction[i*4]=valor_intensidad;
            pTransferFunction[i*4+1]=valor_intensidad;
            pTransferFunction[i*4+2]=valor_intensidad;
            pTransferFunction[i*4+3]=valor_alfa;

            }

         glBindTexture(GL_TEXTURE_1D,texName[1]);
         glTexImage1D(GL_TEXTURE_1D,0,GL_RGBA,tfSize,0,GL_RGBA,GL_FLOAT,pTransferFunction);
         GLenum error= glGetError();

          paso = PasoTrackBar->Position;
         }


        for(int i=0;i<maxPX-minPX+1;i++)
            {
            if(i>=0 && i<Li-minPX)
            LUT[i]=0;
            if(i>=(Li-minPX) && i<=(Ls-minPX))
            LUT[i]=255.0/(2*Width)*float(i-(Li-minPX));
            if(i>Ls-minPX)
            if(CropCheckBox->Checked)LUT[i]=0; else LUT[i]=255.0;
            }
        // HistoChart->Series[1]->Clear();
         HistoChart->Series[1]->XValues->Value[0]=Center; //Centro
         HistoChart->Series[1]->XValues->Value[1]=Ls; //Ls
         HistoChart->Series[1]->XValues->Value[2]=Li; //Li
         HistoChart->Series[1]->Repaint();

         if(NegativoCheckBox->Checked)
         {
                for(int i=0;i<maxPX-minPX+1;i++)
                {
                LUT[i] = 255.0-LUT[i];
                if(LUT[i]<0)
                LUT[i]=0;

                if(LUT[i]>255.0)
                LUT[i]=255.0;
                }
         }
      }
}
 //---------------------------------------------------------------------------
void __fastcall TForm1::CenterTrackBarChange(TObject *Sender)
{
   if(ImagenEnPantalla==true)
   {
   if(CenterTrackBar->Position==Center)
   return;

   paso = atoi(PasoEdit->Text.c_str());
   Center = CenterTrackBar->Position;
   Width =  WidthTrackBar->Position;

   CenterEdit->OnChange = NULL;
   CenterEdit->Text = Center;
   CenterEdit->OnChange = CenterEditChange;

   WidthEdit->OnChange = NULL;
   WidthEdit->Text = Width;
   WidthEdit->OnChange = WidthEditChange;

   RefreshLUT();
   GraficarEnPantalla();
   }

}
//---------------------------------------------------------------------------

void __fastcall TForm1::WidthTrackBarChange(TObject *Sender)
{
   if(ImagenEnPantalla==true)
   {
   if(WidthTrackBar->Position==Width)
   return;

   paso = atoi(PasoEdit->Text.c_str());

   Width =  WidthTrackBar->Position;
   Center = CenterTrackBar->Position;

   CenterEdit->OnChange = NULL;
   CenterEdit->Text = Center;
   CenterEdit->OnChange = CenterEditChange;

   WidthEdit->OnChange = NULL;
   WidthEdit->Text = Width;
   WidthEdit->OnChange = WidthEditChange;

   RefreshLUT();

   GraficarEnPantalla();
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CenterEditChange(TObject *Sender)
{
   if(ImagenEnPantalla==true)
   {

   paso = atoi(PasoEdit->Text.c_str());

   Center = atoi(CenterEdit->Text.c_str());
   Width  = atoi(WidthEdit->Text.c_str());

   CenterTrackBar->OnChange = NULL;
   CenterTrackBar->Position = Center;
   CenterTrackBar->OnChange = CenterTrackBarChange;

   WidthTrackBar->OnChange = NULL;
   WidthTrackBar->Position = Width;
   WidthTrackBar->OnChange = WidthTrackBarChange;

   RefreshLUT();

   GraficarEnPantalla();
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::WidthEditChange(TObject *Sender)
{
   if(ImagenEnPantalla==true)
   {
   paso = atoi(PasoEdit->Text.c_str());

   Center = atoi(CenterEdit->Text.c_str());
   Width  = atoi(WidthEdit->Text.c_str());

   if(Width<=0 || Width==NULL)
   {
   Width = 1;
   WidthEdit->Text = 1;
   }

   CenterTrackBar->OnChange = NULL;
   CenterTrackBar->Position = Center;
   CenterTrackBar->OnChange = CenterTrackBarChange;

   WidthTrackBar->OnChange = NULL;
   WidthTrackBar->Position = Width;
   WidthTrackBar->OnChange = WidthTrackBarChange;

   RefreshLUT();

   GraficarEnPantalla();
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Cilindro1Click(TObject *Sender)
{
   //Parámetros de inicialización de condiciones iniciales del espacio SIMPLX:
   float usual_delta;
   float zero_delta;

   bool secundario=false, terciario=false;

   if(AjusteForm1->ShowModal()==mrOk)
   {
   usual_delta = atof(AjusteForm1->UsualDeltaEdit->Text.c_str());
   zero_delta  = atof(AjusteForm1->ZeroTermDeltaEdit->Text.c_str());

   if(AjusteForm1->AutovectorRadioGroup->ItemIndex==1)
   secundario=true;

   if(AjusteForm1->AutovectorRadioGroup->ItemIndex==2)
   terciario=true;
   }
   else return;

   if(!SelectedPoints->Count)
   return;

  //Cálculo del centro de gravedad:
  //****************************************************************
  float cgx,cgy,cgz;
  float acumx, acumy, acumz;
  float total = 0;

  acumx=0;
  acumy=0;
  acumz=0;

  RenderPunto* ap;

  for(int i=0; i<SelectedPoints->Count; i++)
  {
   if( ap = (RenderPunto*)SelectedPoints->Items[i])
   {
   acumx += ap->p.x;
   acumy += ap->p.y;
   acumz += ap->p.z;
   total++;
   }
  }

  cgx = acumx/total;
  cgy = acumy/total;
  cgz = acumz/total;
  //********************************************************************

  //Transformada de Hotteling:
  //********************************************************************
   //1: Cálculo de la matriz de Covarianza (Cx):
   float Mx[3][3];
   float XX[3][3];
   float Cx[3][3];

    //Vectores auxiliares para introducir en el cálculo de ajuste 'amoeba'
    float* PX = new float[total];
    float* PY = new float[total];
    float* PZ = new float[total];

   Mx[0][0]=cgx*cgx; Mx[0][1]=cgx*cgy; Mx[0][2]=cgx*cgz;
   Mx[1][0]=cgy*cgx; Mx[1][1]=cgy*cgy; Mx[1][2]=cgy*cgz;
   Mx[2][0]=cgz*cgx; Mx[2][1]=cgz*cgy; Mx[2][2]=cgz*cgz;

  double acumxx=0;
  double acumyy=0;
  double acumzz=0;

  double acumxy=0;
  double acumxz=0;
  double acumyz=0;

  int j=0;

  for(int i=0; i<SelectedPoints->Count; i++)
  {
   if( ap = (RenderPunto*)SelectedPoints->Items[i])
   {
   acumxx += ap->p.x*ap->p.x;
   acumyy += ap->p.y*ap->p.y;
   acumzz += ap->p.z*ap->p.z;

   acumxy += ap->p.x*ap->p.y;
   acumxz += ap->p.x*ap->p.z;
   acumyz += ap->p.y*ap->p.z;

   PX[j]= ap->p.x;
   PY[j]= ap->p.y;
   PZ[j]= ap->p.z;
   j++;
   }
  }

  acumxx=acumxx/total; acumyy=acumyy/total; acumzz=acumzz/total;
  acumxy=acumxy/total; acumxz=acumxz/total; acumyz=acumyz/total;

  XX[0][0]=acumxx; XX[0][1]=acumxy; XX[0][2]=acumxz;
  XX[1][0]=acumxy; XX[1][1]=acumyy; XX[1][2]=acumyz;
  XX[2][0]=acumxz; XX[2][1]=acumyz; XX[2][2]=acumzz;

  Cx[0][0]=XX[0][0]-Mx[0][0]; Cx[0][1]=XX[0][1]-Mx[0][1]; Cx[0][2]=XX[0][2]-Mx[0][2];
  Cx[1][0]=XX[1][0]-Mx[1][0]; Cx[1][1]=XX[1][1]-Mx[1][1]; Cx[1][2]=XX[1][2]-Mx[1][2];
  Cx[2][0]=XX[2][0]-Mx[2][0]; Cx[2][1]=XX[2][1]-Mx[2][1]; Cx[2][2]=XX[2][2]-Mx[2][2];
  //********************************************************************

  //Cálculo del autovector principal de la matriz de covarianza:
  //********************************************************************
  //Inicialización:
  float pvec[]={1.0, 1.0, 1.0};
  float pvec2[]={1.0, 1.0, 1.0};
  float pvec3[]={1.0, 1.0, 1.0};


  float norma;    //norma auxiliar
  float vx,vy,vz; //coordenadas auxiliares

  //Aproximado por el método iterativo de las potencias:
  //Referencia: http://es.wikipedia.org/wiki/M%C3%A9todo_de_las_potencias
  int cantIter = 5000; //Posibilidad de setear la sensibilidad
  float mainLambda;

  for(int i=0; i<cantIter; i++)
  {
  vx = Cx[0][0]*pvec[0]+Cx[0][1]*pvec[1]+Cx[0][2]*pvec[2];
  vy = Cx[1][0]*pvec[0]+Cx[1][1]*pvec[1]+Cx[1][2]*pvec[2];
  vz = Cx[2][0]*pvec[0]+Cx[2][1]*pvec[1]+Cx[2][2]*pvec[2];
  norma = sqrt(vx*vx+vy*vy+vz*vz);

  pvec[0] = vx/norma;
  pvec[1] = vy/norma;
  pvec[2] = vz/norma;
  }

  norma =  sqrt(pvec[0]*pvec[0]+pvec[1]*pvec[1]+pvec[2]*pvec[2]);
  pvec[0] = pvec[0]/norma;
  pvec[1] = pvec[1]/norma;
  pvec[2] = pvec[2]/norma;

    if(secundario || terciario)
    {
    mainLambda = Cx[0][0]*pvec[0]/pvec[0]+
               Cx[0][1]*pvec[1]/pvec[0]+
               Cx[0][2]*pvec[2]/pvec[0];

    //Eliminación del autovector para cálculo del vector/valor secundario:
    //Propiedad: si e=eig(A) => (e-m)=eig(A-mI)
    //Igualando m=e => 0 será un eig de A-eI. El resto de los eig permanece inmutable:
    Cx[0][0]=Cx[0][0]-mainLambda;
                               Cx[1][1]=Cx[1][1]-mainLambda;
                                                            Cx[2][2]=Cx[2][2]-mainLambda;


      //Calculo del autovector secundario:
      for(int i=0; i<cantIter; i++)
      {
      vx = Cx[0][0]*pvec2[0]+Cx[0][1]*pvec2[1]+Cx[0][2]*pvec2[2];
      vy = Cx[1][0]*pvec2[0]+Cx[1][1]*pvec2[1]+Cx[1][2]*pvec2[2];
      vz = Cx[2][0]*pvec2[0]+Cx[2][1]*pvec2[1]+Cx[2][2]*pvec2[2];
      norma = sqrt(vx*vx+vy*vy+vz*vz);

      pvec2[0] = vx/norma;
      pvec2[1] = vy/norma;
      pvec2[2] = vz/norma;
      }

      norma =  sqrt(pvec2[0]*pvec2[0]+pvec2[1]*pvec2[1]+pvec2[2]*pvec2[2]);
      pvec2[0] = pvec2[0]/norma;
      pvec2[1] = pvec2[1]/norma;
      pvec2[2] = pvec2[2]/norma;

         if(terciario)
         {
         //cálculo del autovector 3º por producto cruz entre 1º y 2º:
         //Debido a la propiedad de ortonormalidad de matrices simétricas:
         pvec3[0]=pvec2[1]*pvec[2]-pvec2[2]*pvec[1];
         pvec3[1]=pvec2[2]*pvec[0]-pvec2[0]*pvec[2];
         pvec3[2]=pvec2[0]*pvec[1]-pvec2[1]*pvec[0];

         norma = sqrt(pvec3[0]*pvec3[0]+pvec3[1]*pvec3[1]+pvec3[2]*pvec3[2]);
         pvec3[0] = pvec3[0]/norma;
         pvec3[1] = pvec3[1]/norma;
         pvec3[2] = pvec3[2]/norma;
         }//if(terciario)
    }//if(secundario)

  if(secundario)
  {
  pvec[0]=pvec2[0];
  pvec[1]=pvec2[1];
  pvec[2]=pvec2[2];
  }

  if(terciario)
  {
  pvec[0]=pvec3[0];
  pvec[1]=pvec3[1];
  pvec[2]=pvec3[2];
  }

  //Centro G:
  cg.x = cgx;
  cg.y = cgy;
  cg.z = cgz;

  //*********************************************************************
  //Aprovecho la estructura RenderPunto para usar los valores de xR e yR
  //como lambda y distancia respecto del centro g: xR = lambda; yR = dist.
  float distMedia = 0;
  int indMin;
  int indMax;
  float minL, maxL;
  bool first = false;
  float lambda = 0;

  //Búsqueda del mínimo y máximo de lambda y cálculo de lambda y dist:
  for(int i = 0; i<SelectedPoints->Count; i++)
  {
    if( ap = (RenderPunto*)SelectedPoints->Items[i])
    {
    //"lambda":
    ap->p.xR = (ap->p.x-cg.x)*pvec[0]+(ap->p.y-cg.y)*pvec[1]+(ap->p.z-cg.z)*pvec[2];
    lambda = float(ap->p.x-cg.x)*pvec[0]+float(ap->p.y-cg.y)*pvec[1]+float(ap->p.z-cg.z)*pvec[2];

     if(!first)
     {
     minL = ap->p.xR;
     maxL = ap->p.xR;
     indMin = i;
     indMax = i;
     first = true;
     }

     //"dist":
     vx = (float)ap->p.x-cg.x - lambda*pvec[0];
     vy = (float)ap->p.y-cg.y - lambda*pvec[1];
     vz = (float)ap->p.z-cg.z - lambda*pvec[2];
     norma = sqrt(vx*vx+vy*vy+vz*vz);
     ap->p.yR = norma;
     distMedia += norma;

     if(ap->p.xR<minL)
     {
     indMin = i;
     minL = ap->p.xR;
     }

     if(ap->p.xR>maxL)
     {
     indMax = i;
     maxL = ap->p.xR;
     }
    }
  }

  distMedia = distMedia/total;
  //*********************************************************************

  RenderPunto* ap1, *ap2; //Punteros auxiliares
  //*********************************************************************

  //Cálculo de las Condiciones Iniciales:
  //p0: Vector de condiciones iniciales: p0 = {Vx, Vy, Px, Py, R}
  float p0[5];

  ap1 = (RenderPunto*)SelectedPoints->Items[indMin];
  ap2 = (RenderPunto*)SelectedPoints->Items[indMax];

  //Vx:
  p0[0]=pvec[0]/pvec[2];//float(ap2->p.xR-ap1->p.xR)*pvec[0]/float(ap2->p.xR-ap1->p.xR);
  //Vy:
  p0[1]=pvec[1]/pvec[2];//float(ap2->p.y-ap1->p.y)/float(ap2->p.z-ap1->p.z);

  //Px:
  p0[2]= p0[0]*(-cg.z)+float(cg.x);

  //Py:
  p0[3]= p0[1]*(-cg.z)+float(cg.y);

  p0[4] = distMedia;

  //*********************************************************************
  //Criterio de inicialización: Considera un desvío porcentual dado por los
  //parámetros usual_delta para parámetros distintos de 0, y zero_delta para
  //parámetros iguales a 0. El método está adaptado de la forma de inicializacíón
  //utilizada por 'fminsearch' de MATLAB:

   /*usual_delta = 0.05;             % 5 percent deltas for non-zero terms
    zero_term_delta = 0.00025;       % Even smaller delta for zero elements of x
    for j = 1:n
    y = xin;
    if y(j) ~= 0
        y(j) = (1 + usual_delta)*y(j);
    else
        y(j) = zero_term_delta;
    end
    v(:,j+1) = y;
    end
    */

  //p: Vector de vértices del subespacio SIMPLEX: p es de tamaño 6x5:
  //6=número de vértices, 5=cant de coeficientes a evaluar por vértice.
  float **p;

  p=new float*[6];

  for(int i=0;i<6;i++)
  p[i]=new float[5];

  float ysonda[5];

  for(int j=0; j<5; j++)
  {
  ysonda[0]=p0[0];
  ysonda[1]=p0[1];
  ysonda[2]=p0[2];
  ysonda[3]=p0[3];
  ysonda[4]=p0[4];

    if(ysonda[j]!=0)
    ysonda[j]=(1+usual_delta)*ysonda[j];
    else
    ysonda[j]=zero_delta;

  p[j+1][0]=ysonda[0];
  p[j+1][1]=ysonda[1];
  p[j+1][2]=ysonda[2];
  p[j+1][3]=ysonda[3];
  p[j+1][4]=ysonda[4];
  }

  int nfunc;

  //inicializa el subespacio SIMPLEX con vértices 'y':
  float y[6];

  y[0]=funkCIL(p[0], PX, PY, PZ, total, 1.0, 1.0, 1.0);
  y[1]=funkCIL(p[1], PX, PY, PZ, total, 1.0, 1.0, 1.0);
  y[2]=funkCIL(p[2], PX, PY, PZ, total, 1.0, 1.0, 1.0);
  y[3]=funkCIL(p[3], PX, PY, PZ, total, 1.0, 1.0, 1.0);
  y[4]=funkCIL(p[4], PX, PY, PZ, total, 1.0, 1.0, 1.0);
  y[5]=funkCIL(p[5], PX, PY, PZ, total, 1.0, 1.0, 1.0);

  amoebaCIL(p, y, 5, 1e-5,&nfunc,PX,PY,PZ,total, 1.0, 1.0, 1.0);

   delete PX;
   delete PY;
   delete PZ;

   vx = p[0][0];
   vy = p[0][1];
   vz = 1.0;

   norma = sqrt(vx*vx+vy*vy+vz*vz);
   vx = vx/norma;
   vy = vy/norma;
   vz = vz/norma;

   float px, py, pz;
   px = p[0][2];
   py = p[0][3];
   pz = 0.0;

   Cilindro* cil = new Cilindro;

   lambda = (float)(ap1->p.x-px)*vx+(float)(ap1->p.y-py)*vy+(float)(ap1->p.z-pz)*vz;
   cil->pi.x = lambda*vx+px;
   cil->pi.y = lambda*vy+py;
   cil->pi.z = lambda*vz+pz;

   lambda = (float)(ap2->p.x-px)*vx+(float)(ap2->p.y-py)*vy+(float)(ap2->p.z-pz)*vz;
   cil->pf.x = lambda*vx+px;
   cil->pf.y = lambda*vy+py;
   cil->pf.z = lambda*vz+pz;

   cil->radio = p[0][4];

   for(int i=0; i<SelectedPoints->Count; i++)
   {
   ap = (RenderPunto*)SelectedPoints->Items[i];
   delete ap;
   }
   SelectedPoints->Clear();

   delete []p;
    
   CilindrosList->Add(cil);
   dibujaCils = true;
  
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DesmarcarButtonClick(TObject *Sender)
{
      RenderPunto* ap;
      for(int i=0; i<SelectedPoints->Count; i++)
      {
      ap = (RenderPunto*)SelectedPoints->Items[i];
      delete ap;
      }
      SelectedPoints->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Esfera1Click(TObject *Sender)
{
   if(!SelectedPoints->Count)
   return;

  //Cálculo del centro de gravedad:
  //****************************************************************
  float cgx,cgy,cgz;
  float acumx, acumy, acumz;
  float total = 0;

  acumx=0;
  acumy=0;
  acumz=0;

  RenderPunto* ap;

  int j=0;

  for(int i=0; i<SelectedPoints->Count; i++)
  {
   if( ap = (RenderPunto*)SelectedPoints->Items[i])
   {
   acumx += ap->p.x;
   acumy += ap->p.y;
   acumz += ap->p.z;
   total++;
   }
  }

  //Vectores auxiliares para introducir en el cálculo de ajuste 'amoeba'
  float* PX = new float[total];
  float* PY = new float[total];
  float* PZ = new float[total];

  for(int i=0; i<SelectedPoints->Count; i++)
  {
   if( ap = (RenderPunto*)SelectedPoints->Items[i])
   {
   PX[j]= ap->p.x;
   PY[j]= ap->p.y;
   PZ[j]= ap->p.z;
   j++;
   }
  }

  cgx = acumx/total;
  cgy = acumy/total;
  cgz = acumz/total;
  //********************************************************************

  //*********************************************************************
  //Cálculo de la distancia media:
  float distMedia = 0;
  float vx,vy,vz; //vector auxiliar
  float norma;    //norma auxiliar

  //Búsqueda del mínimo y máximo de lambda y cálculo de lambda y dist:
  for(int i = 0; i<SelectedPoints->Count; i++)
  {
    if( ap = (RenderPunto*)SelectedPoints->Items[i])
    {
     //"dist":
     vx = (float)ap->p.x-cgx;
     vy = (float)ap->p.y-cgy;
     vz = (float)ap->p.z-cgz;
     norma = sqrt(vx*vx+vy*vy+vz*vz);
     ap->p.yR = norma;
     distMedia += norma;
    }
  }

  distMedia = distMedia/total;
  //*********************************************************************

  //*********************************************************************
  //Cálculo de las Condiciones Iniciales:
  //Criterio de inicialización: considerar que el centro de gravedad (A) se mueve una
  // {dx, dy, dz}={20,20,20} con un radio 1*r  y (B) no se desplaza, con un radio 1,5*r:

  //p: Vector de vértices del subespacio SIMPLEX: p es de tamaño 5x4:
  //5=número de vértices, 4=cant de coeficientes a evaluar por vértice.
  float **p;

  p=new float*[5];

  for(int i=0;i<5;i++)
  p[i]=new float[4];

  float radio = distMedia;
  //Cálculo de las condiciones iniciales:
  p[0][0] = cgx-radio;
  p[0][1] = cgy-radio;
  p[0][2]=  cgz-radio;
  p[0][3]= 1.5*radio;

  p[1][0] = cgx;
  p[1][1] = cgy+radio/3.0;
  p[1][2]=  cgz;
  p[1][3]= 1.5*radio;

  p[2][0] = cgx+radio/3.0;
  p[2][1] = cgy;
  p[2][2]=  cgz+radio/3.0;
  p[2][3]= 1.5*radio;

  p[3][0] = cgx+radio/3.0;
  p[3][1] = cgy+radio/3.0;
  p[3][2]=  cgz+radio/3.0;
  p[3][3]=  radio;

  p[4][0] = cgx;
  p[4][1] = cgy;
  p[4][2]=  cgz;
  p[4][3]= 1.5*radio;

  int nfunc;

  //inicializa el subespacio SIMPLEX con vértices 'y':
  float y[5];

  y[0]=funkESF(p[0], PX, PY, PZ, total, 1.0, 1.0, 1.0);
  y[1]=funkESF(p[1], PX, PY, PZ, total, 1.0, 1.0, 1.0);
  y[2]=funkESF(p[2], PX, PY, PZ, total, 1.0, 1.0, 1.0);
  y[3]=funkESF(p[3], PX, PY, PZ, total, 1.0, 1.0, 1.0);
  y[4]=funkESF(p[4], PX, PY, PZ, total, 1.0, 1.0, 1.0);

  amoebaESF(p, y, 4, 1e-5,&nfunc,PX,PY,PZ,total, 1.0, 1.0, 1.0);

   delete PX;
   delete PY;
   delete PZ;

   Esfera* esf = new Esfera;

   esf->radio = p[0][3];

   esf->centro.x = p[0][0];
   esf->centro.y = p[0][1];
   esf->centro.z = p[0][2];

   for(int i=0; i<SelectedPoints->Count; i++)
   {
   ap = (RenderPunto*)SelectedPoints->Items[i];
   delete ap;
   }
   SelectedPoints->Clear();

   delete []p;

   EsferasList->Add(esf);
   dibujaEsfs = true;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Global2Click(TObject *Sender)
{
     int zf=cantImagenes-2;
     int zi=2;
     int cantCortes = zf-zi;

     int xi = 2;
     int xf = anchoPX-2;
     int yi = 2;
     int yf = altoPX-2;

      //Máscara básica PasaBajos:
     float  msc[3][3][3] = {
                           {{1,1,1},{1,1,1},{1,1,1}},
                           {{1,1,1},{1,1,1},{1,1,1}},
                           {{1,1,1},{1,1,1},{1,1,1}}
                           };
     ApplyMasc333(msc, xi, xf, yi, yf, zi, zf);

     GraficarEnPantalla();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VOI2Click(TObject *Sender)
{
    //Pasabajos_VOI:
     int zf=atoi(FinSliceEdit->Text.c_str());
     int zi=atoi(IniSliceEdit->Text.c_str());

     int xi = ROIShape->Top;
     int xf = ROIShape->Top+ROIShape->Height;
     int yi = ROIShape->Left;
     int yf = ROIShape->Left+ROIShape->Width;

     //Máscara básica PasaBajos:
     float msc[3][3][3] =  {
                           {{1,1,1},{1,1,1},{1,1,1}},
                           {{1,1,1},{1,1,1},{1,1,1}},
                           {{1,1,1},{1,1,1},{1,1,1}}
                           };
     ApplyMasc333(msc, xi, xf, yi, yf, zi, zf);

     GraficarEnPantalla();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::VOI3Click(TObject *Sender)
{
     int zf=atoi(FinSliceEdit->Text.c_str());
     int zi=atoi(IniSliceEdit->Text.c_str());

     int xi = ROIShape->Top;
     int xf = ROIShape->Top+ROIShape->Height;
     int yi = ROIShape->Left;
     int yf = ROIShape->Left+ROIShape->Width;

      //Máscara básica Altos:
     float msc[3][3][3] =  {
                           {{-1,-1,-1},{-1,-1,-1},{-1,-1,-1}},
                           {{-1,-1,-1},{-1,26,-1},{-1,-1,-1}},
                           {{-1,-1,-1},{-1,-1,-1},{-1,-1,-1}}
                           };
     ApplyMasc333(msc, xi, xf, yi, yf, zi, zf);
     
     GraficarEnPantalla();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::ApplyMasc333(float msc[3][3][3], int xi, int xf, int yi, int yf, int zi, int zf)
{
     Volumen* auxVol = new Volumen(xf-xi,yf-yi,zf-zi);

     ProgressBar1->Max=zf-zi;

        float valor=0;

        for (int k=zi;k<zf;k++)
       {
        ProgressBar1->Position=k-zi-2;
        for(int i=xi+2; i<xf-2;i++)
                 for(int j=yi+2; j<yf-2;j++)
                        {
                        valor = (
                        M->GetV(i-1,j-1,k-1)*msc[0][0][0] +
                        M->GetV(i,j-1,k-1)*msc[0][0][1]   +
                        M->GetV(i+1,j-1,k-1)*msc[0][0][2] +
                        M->GetV(i-1,j,k-1)*msc[0][1][0]   +
                        M->GetV(i,j,k-1)*msc[0][1][1]     +
                        M->GetV(i+1,j,k-1)*msc[0][1][2]   +
                        M->GetV(i-1,j+1,k-1)*msc[0][2][0] +
                        M->GetV(i,j+1,k-1)*msc[0][2][1]   +
                        M->GetV(i+1,j+1,k-1)*msc[0][2][2] +

                        M->GetV(i-1,j-1,k)*msc[1][0][0]   +
                        M->GetV(i,j-1,k)*msc[1][1][0]     +
                        M->GetV(i+1,j-1,k)*msc[1][2][0]   +
                        M->GetV(i-1,j,k)*msc[1][0][1]     +
                        M->GetV(i,j,k)*msc[1][1][1]       +
                        M->GetV(i+1,j,k)*msc[1][2][1]     +
                        M->GetV(i-1,j+1,k)*msc[1][0][2]   +
                        M->GetV(i,j+1,k)*msc[1][1][2]     +
                        M->GetV(i+1,j+1,k)*msc[1][2][2]   +

                        M->GetV(i-1,j-1,k+1)*msc[2][0][0] +
                        M->GetV(i,j-1,k+1)*msc[2][0][1]   +
                        M->GetV(i+1,j-1,k+1)*msc[2][0][2] +
                        M->GetV(i-1,j,k+1)*msc[2][1][0]   +
                        M->GetV(i,j,k+1)*msc[2][1][1]     +
                        M->GetV(i+1,j,k+1)*msc[2][1][2]   +
                        M->GetV(i-1,j+1,k+1)*msc[2][2][0] +
                        M->GetV(i,j+1,k+1)*msc[2][2][1]   +
                        M->GetV(i+1,j+1,k+1)*msc[2][2][2])/9;

                        auxVol->SetV(i-xi-2, j-yi-2, k-zi, valor);
                        }
        }

       ProgressBar1->Position=0;

       for (int k=zi;k<zf;k++)
       {
       ProgressBar1->Position=k-zi-2;
        for(int i=xi+2; i<xf-2;i++)
                 for(int j=yi+2; j<yf-2;j++)
                 {
                 //M->SetV(i,j,k,auxVol->GetV(i-xi-2,j-yi-2,k-zi));
                 Mauxiliar->SetV(i,j,k,auxVol->GetV(i-xi-2,j-yi-2,k-zi));
                 }
        }

        delete auxVol;
        auxVol = NULL;

        ProgressBar1->Position=0;
}
//------------------------------------------------------------------------------

void __fastcall TForm1::SetCursorButtonClick(TObject *Sender)
{
     if(CursorRadioGroup->ItemIndex==0)
     {
     C1.x=XScrollBar->Position;
     C1.y=YScrollBar->Position;
     C1.z=ZScrollBar->Position;
     }

     if(CursorRadioGroup->ItemIndex==1)
     {
     C2.x=XScrollBar->Position;
     C2.y=YScrollBar->Position;
     C2.z=ZScrollBar->Position;
     }

     Cc.x=(C1.x+C2.x)/2.0;
     Cc.y=(C1.y+C2.y)/2.0;
     Cc.z=(C1.z+C2.z)/2.0;

     SetArbitraryPlane();

}
//---------------------------------------------------------------------------

void __fastcall TForm1::SetArbitraryPlane()
{
     float norma;
     vector2.x = res[0]*(C2.x-C1.x);
     vector2.y = res[1]*(C2.y-C1.y);
     vector2.z = res[2]*(C2.z-C1.z);
     norma = sqrt(vector2.x*vector2.x+vector2.y*vector2.y+vector2.z*vector2.z);
     vector2.x=vector2.x/norma;
     vector2.y=vector2.y/norma;
     vector2.z=vector2.z/norma;

     for(int i=0; i<altoPX; i++)
         for(int j=0; j<anchoPX; j++)
         {

         PlanoCorte[i][j].x=res[0]*(PlanoOrig[i][j].x-PC1.x);
         PlanoCorte[i][j].y=res[1]*(PlanoOrig[i][j].y-PC1.y);
         PlanoCorte[i][j].z=res[2]*(PlanoOrig[i][j].z-PC1.z);
         }

    //Seteo de la matriz de Rotación:
    //1- calculo la matriz de rotación, vector1 siempre es el (1,0,0):
    float alfa=acos((vector1.x*vector2.x+vector1.y*vector2.y+vector1.z*vector2.z));

    //2-Producto CRUZ para obtener el eje de rotación normalizado:
    vectorN.x=vector2.y*vector1.z-vector2.z*vector1.y;
    vectorN.y=vector2.z*vector1.x-vector2.x*vector1.z;
    vectorN.z=vector2.x*vector1.y-vector2.y*vector1.x;
    norma = sqrt(vectorN.x*vectorN.x + vectorN.y*vectorN.y + vectorN.z*vectorN.z);
    vectorN.x=vectorN.x/norma;
    vectorN.y=vectorN.y/norma;
    vectorN.z=vectorN.z/norma;

    //3- coficientes de la Matriz de Rotación:
    float c=cos(alfa);
    float s=sin(alfa);

    R[0][0]=c+vectorN.x*vectorN.x*(1.0-c);
    R[0][1]=vectorN.x*vectorN.y*(1.0-c)-vectorN.z*s;
    R[0][2]=vectorN.x*vectorN.z*(1.0-c)+vectorN.y*s;

    R[1][0]=vectorN.y*vectorN.x*(1.0-c)+vectorN.z*s;
    R[1][1]=c+vectorN.y*vectorN.y*(1.0-c);
    R[1][2]=vectorN.y*vectorN.z*(1.0-c)-vectorN.x*s;

    R[2][0]=vectorN.z*vectorN.x*(1.0-c)-vectorN.y*s;
    R[2][1]=vectorN.z*vectorN.y*(1.0-c)+vectorN.x*s;
    R[2][2]=c+vectorN.z*vectorN.z*(1.0-c);

    float x, y, z;
    //Roto los valores del plano de muestreo, centrado y escalado:
       for(int i=0; i<altoPX; i++)
         for(int j=0; j<anchoPX; j++)
         {

         x = PlanoCorte[i][j].x*R[0][0]+
             PlanoCorte[i][j].y*R[1][0]+
             PlanoCorte[i][j].z*R[2][0];

         y = PlanoCorte[i][j].x*R[0][1]+
             PlanoCorte[i][j].y*R[1][1]+
             PlanoCorte[i][j].z*R[2][1];

         z = PlanoCorte[i][j].x*R[0][2]+
             PlanoCorte[i][j].y*R[1][2]+
             PlanoCorte[i][j].z*R[2][2];

         PlanoCorte[i][j].x=x;
         PlanoCorte[i][j].y=y;
         PlanoCorte[i][j].z=z;
         }

     anguloRotArb = 3.141592/180.0*(float)ArbitraryCutScrollBar->Position;
     c=cos(anguloRotArb);
     s=sin(anguloRotArb);

    //Matriz de rotación2:
    R[0][0]=c+vector2.x*vector2.x*(1.0-c);
    R[0][1]=vector2.x*vector2.y*(1.0-c)-vector2.z*s;
    R[0][2]=vector2.x*vector2.z*(1.0-c)+vector2.y*s;

    R[1][0]=vector2.y*vector2.x*(1.0-c)+vector2.z*s;
    R[1][1]=c+vector2.y*vector2.y*(1.0-c);
    R[1][2]=vector2.y*vector2.z*(1.0-c)-vector2.x*s;

    R[2][0]=vector2.z*vector2.x*(1.0-c)-vector2.y*s;
    R[2][1]=vector2.z*vector2.y*(1-0-c)+vector2.x*s;
    R[2][2]=c+vector2.z*vector2.z*(1.0-c);

    for(int i=0; i<altoPX; i++)
         for(int j=0; j<anchoPX; j++)
         {

         x = PlanoCorte[i][j].x*R[0][0]+
             PlanoCorte[i][j].y*R[1][0]+
             PlanoCorte[i][j].z*R[2][0];

         y = PlanoCorte[i][j].x*R[0][1]+
             PlanoCorte[i][j].y*R[1][1]+
             PlanoCorte[i][j].z*R[2][1];

         z = PlanoCorte[i][j].x*R[0][2]+
             PlanoCorte[i][j].y*R[1][2]+
             PlanoCorte[i][j].z*R[2][2];

         PlanoCorte[i][j].x=x;
         PlanoCorte[i][j].y=y;
         PlanoCorte[i][j].z=z;
         }

       //Vuelvo a las coordenadas de la imagen, y lo centro:
       for(int i=0; i<altoPX; i++)
         for(int j=0; j<anchoPX; j++)
         {
         PlanoCorte[i][j].x=(1.0/res[0])*PlanoCorte[i][j].x+Cc.x;
         PlanoCorte[i][j].y=(1.0/res[1])*PlanoCorte[i][j].y+Cc.y;
         PlanoCorte[i][j].z=(1.0/res[2])*PlanoCorte[i][j].z+Cc.z;
         }

}
//------------------------------------------------------------------------------
void __fastcall TForm1::VistaOBButtonClick(TObject *Sender)
{
     ArbitraryCutForm1->Show();

    //Interpolación TRILIEAL:
    //http://en.wikipedia.org/wiki/Trilinear_interpolation
    float x,y,z;
    float xd, yd, zd;
    float xp, xt, yp, yt, zp, zt;
    float i1, i2, j1, j2, w1, w2;
    float valor;

        for(int i=0; i<CuadroCorteOblicuo->NumeroFilas; i++)
         for(int j=0; j<CuadroCorteOblicuo->NumeroColumnas; j++)
         {
          CuadroCorteOblicuo->buffer(i,j)=0;
         }

        for(int i=0; i<altoPX; i++)
         for(int j=0; j<anchoPX; j++)
         {
         if(PlanoCorte[i][j].x<altoPX-1 && PlanoCorte[i][j].x>1)
          if(PlanoCorte[i][j].y<anchoPX-1 && PlanoCorte[i][j].y>1)
           if(PlanoCorte[i][j].z<cantImagenes-1 && PlanoCorte[i][j].z>1)
           {
           
            x = PlanoCorte[i][j].x;
            y = PlanoCorte[i][j].y;
            z = PlanoCorte[i][j].z;

            xp = floor(PlanoCorte[i][j].x);
            xt =  ceil(PlanoCorte[i][j].x);
            yp = floor(PlanoCorte[i][j].y);
            yt =  ceil(PlanoCorte[i][j].y);
            zp = floor(PlanoCorte[i][j].z);
            zt =  ceil(PlanoCorte[i][j].z);

            xd = x-xp;
            yd = y-yp;
            zd = z-zp;

            i1 = (LUT[M->GetV(xp,yp,zp)-minPX]*(1-zd)+ LUT[M->GetV(xp,yp,zt)-minPX]*zd);
            i2 = (LUT[M->GetV(xp,yt,zp)-minPX]*(1-zd)+ LUT[M->GetV(xp,yt,zt)-minPX]*zd);
            j1 = (LUT[M->GetV(xt,yp,zp)-minPX]*(1-zd)+ LUT[M->GetV(xt,yp,zt)-minPX]*zd);
            j2 = (LUT[M->GetV(xt,yt,zp)-minPX]*(1-zd)+ LUT[M->GetV(xt,yt,zt)-minPX]*zd);
            w1 = i1*(1-yd)+i2*yd;
            w2 = j1*(1-yd)+j2*yd;

            valor = __int8(w1*(1-xd)+w2*xd);

           CuadroCorteOblicuo->buffer(i,j)=valor;
           }
         }

        CuadroCorteOblicuo->Graficar(ArbitraryCutForm1->Image1);

        SetupTexturesFromArbitraryPlane(M, LUT, minPX, PlanoCorte,
                                        anchoPX, altoPX, cantImagenes, texturaOB);
        IdleLoop(Sender,1);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ArbitraryCutScrollBarChange(TObject *Sender)
{
       SetArbitraryPlane();
       VistaOBButtonClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FlipCursorButtonClick(TObject *Sender)
{
     PuntoF paux;

     paux.x=C2.x;
     paux.y=C2.y;
     paux.z=C2.z;

     C2.x=C1.x;
     C2.y=C1.y;
     C2.z=C1.z;

     C1.x=paux.x;
     C1.y=paux.y;
     C1.z=paux.z;

     SetArbitraryPlane();

     VistaOBButtonClick(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::ParaOrthoButtonClick(TObject *Sender)
{
   if (vector1.z==0) //Esta en vista Paralela: Setear Vista Ortogonal
   {
   vector1.x=0.0;
   vector1.y=0.0;
   vector1.z=1.0;
   ParaOrthoButton->Caption = "Para";
   SetArbitraryPlane();
   VistaOBButtonClick(Sender);
   return;
   }

   if (vector1.z==1)
   {
   vector1.x=1.0;
   vector1.y=0.0;
   vector1.z=0.0;
   ParaOrthoButton->Caption = "Ortho";
   SetArbitraryPlane();
    VistaOBButtonClick(Sender);
   return;
   }

}
//---------------------------------------------------------------------------

void __fastcall TForm1::PuntoCentralScrollBarChange(TObject *Sender)
{
     float L = sqrt((C2.x-C1.x)*(C2.x-C1.x) + (C2.y-C1.y)*(C2.y-C1.y) + (C2.z-C1.z)*(C2.z-C1.z));
     float delta = L/float(PuntoCentralScrollBar->Max-PuntoCentralScrollBar->Min);


     Cc.x = (C1.x+C2.x)/2.0+(float)PuntoCentralScrollBar->Position*delta*vector2.x;
     Cc.y = (C1.y+C2.y)/2.0+(float)PuntoCentralScrollBar->Position*delta*vector2.y;
     Cc.z = (C1.z+C2.z)/2.0+(float)PuntoCentralScrollBar->Position*delta*vector2.z;

     SetArbitraryPlane();
      VistaOBButtonClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CropCheckBoxClick(TObject *Sender)
{
   RefreshLUT();
   GraficarEnPantalla();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::UpAPButtonClick(TObject *Sender)
{
   if(UpAPButton->Caption == "^")
   {
   Label4->Visible=false;
   Label5->Visible=false;
   ArbitraryCutScrollBar->Visible=false;
   PuntoCentralScrollBar->Visible=false;
   ArbitraryCutControlPanel->Height = UpAPButton->Height+6;
   UpAPButton->Top = 3;
   UpAPButton->Caption = "V";
   return;
   }

   if(UpAPButton->Caption == "V")
   {
   Label4->Visible=true;
   Label5->Visible=true;
   ArbitraryCutScrollBar->Visible=true;
   PuntoCentralScrollBar->Visible=true;
   ArbitraryCutControlPanel->Height = 233;
   UpAPButton->Caption = "^";
   UpAPButton->Top = 216;
   return;
   }

}
//---------------------------------------------------------------------------


void __fastcall TForm1::Normal1Click(TObject *Sender)
{
 int zf,zi;

        if(FinSliceEdit->Text!="")
                zf=atoi(FinSliceEdit->Text.c_str());
        else zf=SliceListBox->ItemIndex;

        if(IniSliceEdit->Text!="")
                zi=atoi(IniSliceEdit->Text.c_str());
        else zi=SliceListBox->ItemIndex-1;

        if(!ROIShape->Visible)
         return;

        int xi = ROIShape->Top;
        int xf = ROIShape->Top+ROIShape->Height;
        int yi = ROIShape->Left;
        int yf = ROIShape->Left+ROIShape->Width;

        float* p = new float[3];

        //Carga lista RenderPoints con todos los puntos de borde:
        ProgressBar1->Max = zf-zi;
        ProgressBar1->Position=0;

       // RenderPoints->Clear();

        short int umbral = Center;
        RenderPunto* rp;
        short int* color;
        float norma;

        for (int k=zi;k<zf;k++)
        {ProgressBar1->Position++;
           for(int i=xi; i<xf;i++)
                 for(int j=yi; j<yf;j++)
                    if(M->GetV(i,j,k)>umbral)
                    {
                      p[0]=i; p[1]=j; p[2]=k;
                       if(collect26neighConUmbral(p, M, umbral)) //true: es un punto de borde
                       {
                       rp = new RenderPunto;
                       rp->p.x=i; rp->p.y=j; rp->p.z=k;
                       rp->p.color = M->GetV(i,j,k);

                       rp->n.x = M->GetV(i+1,j,k)-M->GetV(i-1,j,k);
                       rp->n.y = M->GetV(i,j+1,k)-M->GetV(i,j-1,k);
                       rp->n.z = M->GetV(i,j,k+1)-M->GetV(i,j,k-1);

                       norma = sqrt(rp->n.x*rp->n.x + rp->n.y*rp->n.y + rp->n.z*rp->n.z);

                       rp->n.x = rp->n.x/norma;
                       rp->n.y = rp->n.y/norma;
                       rp->n.z = rp->n.z/norma;

                       rp->norma = norma;

                       RenderPoints->Add(rp);
                       }
                     }
        }
        ProgressBar1->Position=0;

        medX=(xf+xi)/2.0;
        medY=(yf+yi)/2.0;
        medZ=(zf+zi)/2.0;

        distX = (xf-xi);
        distY = (yf-yi);
        distZ = (zf-zi);

        int zmin=distZ/5.0;
        zINT[0]=zmin;

        for(int i=1;i<6;i++)        //?
        zINT[i]=zINT[i-1]+zmin;

        RenderFlag=true;

        RenderPoints->Sort(CompareColors);

        offset = 0;
        final = RenderPoints->Count;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Membrana1Click(TObject *Sender)
{
 int zf,zi;

        if(FinSliceEdit->Text!="")
                zf=atoi(FinSliceEdit->Text.c_str());
        else zf=SliceListBox->ItemIndex;

        if(IniSliceEdit->Text!="")
                zi=atoi(IniSliceEdit->Text.c_str());
        else zi=SliceListBox->ItemIndex-1;

        if(!ROIShape->Visible)
         return;

        int xi = ROIShape->Top;
        int xf = ROIShape->Top+ROIShape->Height;
        int yi = ROIShape->Left;
        int yf = ROIShape->Left+ROIShape->Width;

        float* p = new float[3];

        //Carga lista RenderPoints con todos los puntos de borde:
        ProgressBar1->Max = zf-zi;
        ProgressBar1->Position=0;

       // RenderPoints->Clear();

        short int umbral = Center;
        RenderPunto* rp;
        short int* color;
        float norma;

        for (int k=zi;k<zf;k++)
        {ProgressBar1->Position++;
           for(int i=xi; i<xf;i++)
                 for(int j=yi; j<yf;j++)
                    if(M->GetV(i,j,k)>umbral)
                    {
                      p[0]=i; p[1]=j; p[2]=k;
                       if(collect26neighConUmbral(p, M, umbral)) //true: es un punto de borde
                       {
                       rp = new RenderPunto;
                       rp->p.x=i; rp->p.y=j; rp->p.z=k;
                       rp->p.color = M->GetV(i,j,k);

                       rp->n.x = M->GetV(i+1,j,k)-M->GetV(i-1,j,k);
                       rp->n.y = M->GetV(i,j+1,k)-M->GetV(i,j-1,k);
                       rp->n.z = M->GetV(i,j,k+1)-M->GetV(i,j,k-1);

                       norma = sqrt(rp->n.x*rp->n.x + rp->n.y*rp->n.y + rp->n.z*rp->n.z);

                       rp->n.x = rp->n.x/norma;
                       rp->n.y = rp->n.y/norma;
                       rp->n.z = rp->n.z/norma;

                       rp->norma = norma;

                      // RenderPoints->Add(rp);
                       BorderPoints->Add(rp);
                       }
                     }
        }
        ProgressBar1->Position=0;

        medX=(xf+xi)/2.0;
        medY=(yf+yi)/2.0;
        medZ=(zf+zi)/2.0;

        distX = (xf-xi);
        distY = (yf-yi);
        distZ = (zf-zi);

        int zmin=distZ/5.0;
        zINT[0]=zmin;

        for(int i=1;i<6;i++)        //?
        zINT[i]=zINT[i-1]+zmin;

        RenderFlag=true;
    //    CantPuntosSegLabel->Caption=RenderPoints->Count;
  /* VolRenderLabel->Caption = RenderPoints->Count*
                             atof(RXEdit->Text.c_str())*
                             atof(RYEdit->Text.c_str())*
                             atof(RZEdit->Text.c_str());*/

   BorderPoints->Sort(CompareColors);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::GrabarVistasOblicuas1Click(TObject *Sender)
{
if(SaveDialog1->Execute())
        {
        FILE* fp;
        AnsiString FileName = SaveDialog1->FileName;

        if(!(FileName.AnsiPos(".bin") || FileName.AnsiPos(".BIN")))
        FileName=FileName+".bin";

        fp=fopen(FileName.c_str(),"w+b");

        ProgressBar1->Position=0;

        encabezado enc1;

        float dx, dy, dz;
        float d;
        float resx, resy, resz;
        float L;
        float delta;

        //Calculo de las dimensiones:
        dx=(PlanoCorte[0][0].x*res[0]-PlanoCorte[anchoPX-1][0].x*res[0]);
        dy=(PlanoCorte[0][0].y*res[1]-PlanoCorte[anchoPX-1][0].y*res[1]);
        dz=(PlanoCorte[0][0].z*res[2]-PlanoCorte[anchoPX-1][0].z*res[2]);
        d = sqrt(dx*dx+dy*dy+dz*dz);
        resx = d/anchoPX;

        dx=(PlanoCorte[0][0].x*res[0]-PlanoCorte[0][altoPX-1].x*res[0]);
        dy=(PlanoCorte[0][0].y*res[1]-PlanoCorte[0][altoPX-1].y*res[1]);
        dz=(PlanoCorte[0][0].z*res[2]-PlanoCorte[0][altoPX-1].z*res[2]);
        d = sqrt(dx*dx+dy*dy+dz*dz);
        resy = d/altoPX;

        L = sqrt((C2.x-C1.x)*(C2.x-C1.x) + (C2.y-C1.y)*(C2.y-C1.y) + (C2.z-C1.z)*(C2.z-C1.z));
        delta = L/float(PuntoCentralScrollBar->Max-PuntoCentralScrollBar->Min);

        dx = delta*vector2.x*res[0];
        dy = delta*vector2.y*res[1];
        dz = delta*vector2.z*res[2];
        d = sqrt(dx*dx+dy*dy+dz*dz);
        resz = d;

       enc1.dX=resx;
       enc1.dY=resy;
       enc1.dZ=resz;
       enc1.ancho=anchoPX;
       enc1.alto=altoPX;
       enc1.min=minPX;
       enc1.max=maxPX;

      float min = PuntoCentralScrollBar->Min;
      float max = PuntoCentralScrollBar->Max;
      ProgressBar1->Max=max-min;

      int numeroimgs = max-min;
      int minC = 0;
      //size_t fread(void *ptr, size_t num_bytes, size_t count, FILE *stream);
       fwrite(&numeroimgs,sizeof(short int),1,fp);
       fwrite(&minC,sizeof(short int),1,fp);


    //Interpolación TRILIEAL:
    //http://en.wikipedia.org/wiki/Trilinear_interpolation
    float x,y,z;
    float xd, yd, zd;
    float xp, xt, yp, yt, zp, zt;
    float i1, i2, j1, j2, w1, w2;
    short int valor;

     for (int k=min; k<max; ++k)
     {

     L = sqrt((C2.x-C1.x)*(C2.x-C1.x) + (C2.y-C1.y)*(C2.y-C1.y) + (C2.z-C1.z)*(C2.z-C1.z));
     delta = L/float(PuntoCentralScrollBar->Max-PuntoCentralScrollBar->Min);

     Cc.x = (C1.x+C2.x)/2.0+(float)k*delta*vector2.x;
     Cc.y = (C1.y+C2.y)/2.0+(float)k*delta*vector2.y;
     Cc.z = (C1.z+C2.z)/2.0+(float)k*delta*vector2.z;

     SetArbitraryPlane();

       ProgressBar1->Position=k-min;
       enc1.frame=k-min;
       fwrite(&enc1,sizeof(encabezado),1,fp);

         for (int i=0; i<enc1.ancho; ++i)
           for (int j=0; j<enc1.alto; ++j)
             {

               if(PlanoCorte[i][j].x<altoPX-1 && PlanoCorte[i][j].x>1 &&
                  PlanoCorte[i][j].y<anchoPX-1 && PlanoCorte[i][j].y>1 &&
                  PlanoCorte[i][j].z<cantImagenes-1 && PlanoCorte[i][j].z>1)
                  {

                   x = PlanoCorte[i][j].x;
                   y = PlanoCorte[i][j].y;
                   z = PlanoCorte[i][j].z;

                   xp = floor(PlanoCorte[i][j].x);
                   xt =  ceil(PlanoCorte[i][j].x);
                   yp = floor(PlanoCorte[i][j].y);
                   yt =  ceil(PlanoCorte[i][j].y);
                   zp = floor(PlanoCorte[i][j].z);
                   zt =  ceil(PlanoCorte[i][j].z);

                   xd = x-xp;
                   yd = y-yp;
                   zd = z-zp;

                   i1 = (M->GetV(xp,yp,zp)*(1-zd)+ M->GetV(xp,yp,zt)*zd);
                   i2 = (M->GetV(xp,yt,zp)*(1-zd)+ M->GetV(xp,yt,zt)*zd);
                   j1 = (M->GetV(xt,yp,zp)*(1-zd)+ M->GetV(xt,yp,zt)*zd);
                   j2 = (M->GetV(xt,yt,zp)*(1-zd)+ M->GetV(xt,yt,zt)*zd);
                   w1 = i1*(1-yd)+i2*yd;
                   w2 = j1*(1-yd)+j2*yd;

                   valor = (w1*(1-xd)+w2*xd);

                  fwrite(&valor,sizeof(short int),1,fp);

                     if(valor>maxPX)
                      enc1.max = valor;

               }else
               {
               fwrite(&minPX,sizeof(short int),1,fp);
               }

             }

       }
        fclose( fp );
           ProgressBar1->Position=0;
     }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::VOI4Click(TObject *Sender)
{
        //Coef High Boost: indica cuantas veces se conserva la imagen original:
        AnsiString A = "2";
        int a;

        if(InputQuery("Ingrese el coeficiente central del filtro A=(w+1)/9","A:",A))
        {
        a = atoi(A.c_str());
        }else {return;}

        float w = 27*float(a)-1.0;

        //High Boost VOI:
     int zf=atoi(FinSliceEdit->Text.c_str());
     int zi=atoi(IniSliceEdit->Text.c_str());

     int xi = ROIShape->Top;
     int xf = ROIShape->Top+ROIShape->Height;
     int yi = ROIShape->Left;
     int yf = ROIShape->Left+ROIShape->Width;

     //Máscara básica PasaAltos con coef High Boost Central:
     float msc[3][3][3] =  {
                           {{-1,-1,-1},{-1,-1,-1},{-1,-1,-1}},
                           {{-1,-1,-1},{-1,w,-1},{-1,1,-1}},
                           {{-1,-1,-1},{-1,-1,-1},{-1,-1,-1}}
                           };
     ApplyMasc333(msc, xi, xf, yi, yf, zi, zf);

     GraficarEnPantalla();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Global4Click(TObject *Sender)
{
     //Coef High Boost: indica cuantas veces se conserva la imagen original:
        AnsiString A = "2";
        int a;

        if(InputQuery("Ingrese el coeficiente central del filtro A=(w+1)/9","A:",A))
        {
        a = atoi(A.c_str());
        }else {return;}

        float w = 27*float(a)-1.0;

     int zf=cantImagenes-2;
     int zi=2;
     int cantCortes = zf-zi;

     int xi = 2;
     int xf = anchoPX-2;
     int yi = 2;
     int yf = altoPX-2;

      //Máscara básica PasaAltos con coef High Boost Central:
     float msc[3][3][3] =  {
                           {{-1,-1,-1},{-1,-1,-1},{-1,-1,-1}},
                           {{-1,-1,-1},{-1,w,-1},{-1,1,-1}},
                           {{-1,-1,-1},{-1,-1,-1},{-1,-1,-1}}
                           };
     ApplyMasc333(msc, xi, xf, yi, yf, zi, zf);

     GraficarEnPantalla();

}
//---------------------------------------------------------------------------

void __fastcall TForm1::PruebaFiltro1Click(TObject *Sender)
{

        AnsiString radioString = "10";
        int r;

        if(InputQuery("Ingrese Radio del Parche","Radio:",radioString))
        {
        r = atoi(radioString.c_str());
        }else {return;}


     int zi=atoi(IniSliceEdit->Text.c_str());
     int zf=atoi(FinSliceEdit->Text.c_str());

     int xi = ROIShape->Top;
     int xf = ROIShape->Top+ROIShape->Height;
     int yi = ROIShape->Left;
     int yf = ROIShape->Left+ROIShape->Width;

     int cantCortes = zf-zi;

     ProgressBar1->Max=cantCortes-2*r+1;

     TList* Parche = new TList;
     strel(r, Parche);

     double acum=0;
     double acum_2=0;

     float media;
     float desv;

     float valor;

     float R;

     //Volumen Auxiliar para guardar resultados:
     Volumen* Maux = new Volumen(xf-xi+1, yf-yi+1, zf-zi+1);

    for (int k=zi+r;k<zf-r;k++)
     {ProgressBar1->Position=k-zi-r;
      for(int j=yi+r; j<yf-r;j++)
            for(int i=xi+r; i<xf-r;i++)
            {
              for (int h = 0; h<Parche->Count; h++)
              {

              valor = M->GetV(i+((Punto*)Parche->Items[h])->x,
                         j+((Punto*)Parche->Items[h])->y,
                         k+((Punto*)Parche->Items[h])->z);

              acum = acum + valor;
              acum_2 = acum_2 + valor*valor;

              }

              media = acum/float(Parche->Count);
              desv = sqrt((acum_2 - 2.0*acum*media + float(Parche->Count)*media*media)/ float(Parche->Count));

              R = (1.0 - 1.0/(1.0 + desv*desv))*(maxPX-minPX)+minPX;

              Maux->SetV(i-xi-r, j-yi-r, k-zi-r, R);

              acum=0;
              acum_2=0;
            }
     }

           for (int k=zi+r;k<zf-r;k++)
             for(int j=yi+r; j<yf-r;j++)
                for(int i=xi+r; i<xf-r;i++)
                 M->SetV(i,j,k, Maux->GetV(i-xi-r,j-yi-r,k-zi-r));

     ProgressBar1->Position = 0;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CACCheckBoxClick(TObject *Sender)
{
      GraficarEnPantalla();
      TranspImage->Visible = CACCheckBox->Checked;
      TranspImage->Enabled = CACCheckBox->Checked;

      if(CACCheckBox->Checked)
      {
      CenterEdit->Text = CACGS;
      WidthEdit->Text = 500;
      }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CAC1Click(TObject *Sender)
{
        CACForm1->MCAC = M;
        CACForm1->LUT = LUT;
        CACForm1->alto = altoPX;
        CACForm1->ancho = anchoPX;
        CACForm1->minPX = minPX;
        CACForm1->maxPX = maxPX;
        CACForm1->dX = res[0];
        CACForm1->dY = res[1];
        CACForm1->dZ = res[2];
        CACForm1->cantCortes = cantImagenes-1;
        CACForm1->ShowModal();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::GuardarEsqButtonClick(TObject *Sender)
{
     if(SaveDialog1->Execute())
       {
         EsquelPoints->Pack();
         EsquelPoints->Capacity = RenderPoints->Count;

        FILE* fp;
        AnsiString FileName = SaveDialog1->FileName;

        if(!(FileName.AnsiPos(".dat") || FileName.AnsiPos(".DAT")))
        FileName=FileName+".dat";

        fp=fopen(FileName.c_str(),"w+b");

        Punto* ap = new Punto; //punto auxiliar

        int cantPuntos = EP->Count + LP->Count + JP->Count;
        fwrite(&cantPuntos, sizeof(int), 1, fp);

        int x,y,z;
        short int color;

         for(int k=0;k<EP->Count;k++)
         {
             if(ap = (Punto*)EP->Items[k])
             {
             x=ap->x; y=ap->y; z=ap->z;
             //size_t fwrite(const void *ptr, size_t size, size_t n, FILE *stream);
             fwrite(&x, sizeof(int), 1, fp);
             fwrite(&y, sizeof(int), 1, fp);
             fwrite(&z, sizeof(int), 1, fp);
             }
         }

         for(int k=0;k<LP->Count;k++)
         {
             if(ap = (Punto*)LP->Items[k])
             {
             x=ap->x; y=ap->y; z=ap->z;
             //size_t fwrite(const void *ptr, size_t size, size_t n, FILE *stream);
             fwrite(&x, sizeof(int), 1, fp);
             fwrite(&y, sizeof(int), 1, fp);
             fwrite(&z, sizeof(int), 1, fp);
             }
         }

         for(int k=0;k<JP->Count;k++)
         {
             if(ap = (Punto*)JP->Items[k])
             {
             x=ap->x; y=ap->y; z=ap->z;
             //size_t fwrite(const void *ptr, size_t size, size_t n, FILE *stream);
             fwrite(&x, sizeof(int), 1, fp);
             fwrite(&y, sizeof(int), 1, fp);
             fwrite(&z, sizeof(int), 1, fp);
             }
         }


        fclose(fp);
       }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Hueso1Click(TObject *Sender)
{
//Fuente de las ventanas: Libro: "Computed Tomography, Willi Kalender"
        CenterEdit->Text = 1000;
        WidthEdit->Text = 2500;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Mediastino1Click(TObject *Sender)
{
//Fuente de las ventanas: Libro:"Computed Tomography, Willi Kalender"
        CenterEdit->Text = -50;
        WidthEdit->Text = 400;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Pulmn1Click(TObject *Sender)
{
//Fuente de las ventanas: Libro:"Computed Tomography, Willi Kalender"
        CenterEdit->Text = -600;
        WidthEdit->Text = 1700;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AbrirconSobremuestreo1Click(TObject *Sender)
{
      OpenDialog1->Filter = "BIN files (*.bin)|*.BIN|Dicom files (*.dcm)|*.DCM";
      int N;

       if( OpenDialog1->Execute() )
       {

       AnsiString nstring;

        if(InputQuery("Ingrese número de slices interpolados","N:",nstring))
        {
        N = atoi(nstring.c_str());
        N++; //Cantidad de planos entre dos planos originales = N;
        }else return;

          //Si ya había un estudio cargado:
          if(ImagenEnPantalla)
          {
          delete[] LUT;
          delete[] histoCaja;
          ImagenEnPantalla=false;
          delete CAC;
          }

         //Archivo para abrir:
         FILE* fp;
         fp = NULL;
         AnsiString FullFileName = OpenDialog1->FileName;
         AnsiString VolFullPath;

         //Reconoce la forma de abrir según sea .bin o .dcm:
         if(ExtractFileExt(FullFileName)!=".bin")
         {
         AnsiString FilePath = ExtractFilePath(FullFileName);

         //Llama al programa auxiliar para leer DICOM:
         AnsiString comillas = char(34);
         AnsiString FilePath_ =  comillas + FilePath;
         AnsiString ExecProgPath = "ModuloPrueba\\prueba.exe " + FilePath_ ;

         //Ejecuta programa auxiliar:
         WinExec(ExecProgPath.c_str() , SW_SHOWNORMAL);
           //Crea e intenta abrir un archivo dummy para saber cuando terminó de leer todos
           //los archivos DICOM. Esto se hace ya que el archivo dummy se crea luego de toda
           //la lista de DICOM en el programa prueba.exe:
           while(fp==NULL)
           fp=fopen( (FilePath+"dummy.dum").c_str(),"r");
           //Una vez encontrado el archivo dummy, se elimina:
           fclose(fp);
           remove((FilePath+"dummy.dum").c_str());

           VolFullPath = FilePath + "vol.bin";

         }else VolFullPath = OpenDialog1->FileName;

         //FilePath:
         CarpetaLabel->Caption=ExtractFilePath(VolFullPath);
         ArchivoLabel->Caption=ExtractFileName(VolFullPath);

         //Apertura de los volumenes .bin:
         fp = fopen( VolFullPath.c_str(),  "r+b");

         encabezado enc1;
         //size_t fread(void *ptr, size_t num_bytes, size_t count, FILE *stream);

         //Lee la cantidad de imagenes y el valor de frame inicial: sirve para
         //archivos incompletos cuyo minframe != 0:
         fread(&cantImagenes,sizeof(short int),1,fp);
         fread(&minFrame,sizeof(short int),1,fp);

         //Sobremuestrea N veces, es decir, agrega N planos entre 2 planos originales:
         //Si hay M imégenes originales, y se le interponen M planos en el medio, la
         //cantidad final de imágenes siempre será: (M+1)*(N-1)+1 = M*N-M+N

         cantImagenes = cantImagenes*(N-1) + cantImagenes - (N-1);

         //Lee el primer encabezado, de donde extrae los valores de minPX y maxPX:
         //También se usa para obtener la resolución de un corte del estudio "rX" y "rY":
         fread(&enc1,sizeof(encabezado),1,fp);
         maxPX = enc1.max;
         minPX = enc1.min;

        //indica si se encontró el corte sucesivo a primercorte, para
        //calcular la distancia entre cortes "rZ":
        bool encontradoSucesivo = false;

       //Damian://maxPX=0;
                //minPX=0;

        //Obtiene las dimensiones del estudio del primer encabezado:
        anchoPX = enc1.ancho;
        altoPX = enc1.alto;
        res[0]=enc1.dX;
        res[1]=enc1.dY;
        //Obtiene dZ como la posición del primer corte:
        res[2]=enc1.dZ;
        RXEdit->Text = res[0];
        RYEdit->Text = res[1];

        //Creación del volumen que va a contener a las imágenes:
        M = new Volumen(anchoPX,altoPX,cantImagenes);

        int corte = enc1.frame; /*Damian: +1*/;
        corte = corte*N;

        int primerCorte = corte;

        ProcessLabel->Caption = "Cargando Estudio...";
        ProcessLabel->Refresh();
        ProgressBar1->Max=cantImagenes;
        ImageScrollBar->Enabled = true;

        ImageScrollBar->Min=0;
        ImageScrollBar->Max=cantImagenes-1;

       //Apertura de los archivos sucesivos, fila X fila, NO elemento X elemento:
       short int valoraux;

       for (int k=0;k<cantImagenes;k++)
       {
       ProgressBar1->Position=k;
        for(int i=0; i<enc1.ancho;i++)
                       fread(M->V[corte-minFrame*N][i],enc1.alto*sizeof(short int),1,fp);

        fread(&enc1,sizeof(encabezado),1,fp);
        corte = enc1.frame*N;

        if(corte==primerCorte+N && !encontradoSucesivo)
        {
        res[2]=fabs(res[2]-enc1.dZ);

        if(res[2]==0)  //Implica que se grabó de un estudio previamente
        res[2]=enc1.dZ;//pasado a .bin

        res[2]=res[2]/float(N);

        RZEdit->Text = res[2];

        encontradoSucesivo = true;
        }

        if(enc1.max>maxPX)
         maxPX = enc1.max;
       }

       //Cierra el archivo:
       fclose( fp );

/*for (int i=0; i<enc1.alto; i++) for (int j=0; j<enc1.alto; j++) for (int k=0; k<cantImagenes; k++)
{M[i][j][k]=M[i][j][k]/*Damian:-16384*//*Damian:if(M[i][j][k]==-16384)M[i][j][k]=-2048;}*/
/* Damian: minPX = -2048; maxPX = maxPX-16384;*/

        //Recetea el ProgressBar:
        ProgressBar1->Position=0;
        ProcessLabel->Caption = "ProcessLabel";

        //Ajusta los trackbars que modifican Width y Height
        CenterTrackBar->Min=minPX;
        CenterTrackBar->Max=maxPX;
        CenterTrackBar->Position=int((maxPX+minPX)/2);

        WidthTrackBar->Min=1;
        WidthTrackBar->Max=int((maxPX-minPX)/2);

        WidthTrackBar->Position=int((maxPX-minPX)/2);
        CenterEdit->Text = int((maxPX+minPX)/2);
        WidthEdit->Text = int((maxPX-minPX)/2);

        //Preparacion de las vistas ortogonales:
        //.....................................................
        CuadroXY = new Matriz(XYImage->Width, XYImage->Height);
        YScrollBar->Max = altoPX-1;
        YScrollBar->Position = YScrollBar->Max/2;

        CuadroYZ = new Matriz(YZImage->Width, YZImage->Height);
        ZScrollBar->Max = cantImagenes-1;
        ZScrollBar->Position = ZScrollBar->Max/2;

        CuadroZX = new Matriz(ZXImage->Width, ZXImage->Height);
        XScrollBar->Max = anchoPX-1;
        XScrollBar->Position = XScrollBar->Max/2;

        CuadroZoom = new Matriz(ZoomImage->Width, ZoomImage->Height);

        CuadroCorteOblicuo = new Matriz(ArbitraryCutForm1->Image1->Width,
                                        ArbitraryCutForm1->Image1->Height);

        OrthoViewSheet->Enabled = true;

        ZoomRadioGroup->ItemIndex = 0;
        //.....................................................

        Pantalla->Visible=true;
        ImagenEnPantalla=true;
        ROICheckBox->Enabled=true;
        ROI1->Enabled = true;

        MaxPxLabel->Caption=maxPX;
        MinPxLabel->Caption=minPX;

        CAC = new bool[altoPX*anchoPX];

        //Preparación de las LUTs:
        //*******************************************************
        //LUT B&N:
        LUT = new float[maxPX-minPX+1];

        //LUTs a Color para visualización 3D:
        LUT_Sangre = new float[maxPX-minPX+1];
        LUT_Musculo = new float[maxPX-minPX+1];
        LUT_Hueso = new float[maxPX-minPX+1];

        //Inicialización de LUTs
        for(int i=0;i<maxPX-minPX;i++)
        {
            LUT[i]=0;
            LUT_Sangre[i]=0;
            LUT_Musculo[i]=0;
            LUT_Hueso[i]=0;
        }


        for(int i=100-minPX;i<400-minPX;i++)
        LUT_Sangre[i]=205.0/300.0 * (i-(100-minPX))+50.0;
        for(int i=400-minPX;i<600-minPX;i++)
        LUT_Sangre[i]=205.0 -205.0/300.0 * (i-(400-minPX))+50.0;

        for(int i=-600-minPX;i<0.0-minPX;i++)
        LUT_Musculo[i]=255.0/600.0 *(i-(-600-minPX));
        for(int i=0.0-minPX;i<800-minPX;i++)
        LUT_Musculo[i]=255.0 -255.0/800.0 * (i-(0.0-minPX));

        for(int i=100-minPX;i<600-minPX;i++)
        LUT_Hueso[i]=155.0/500.0 *(i-(100-minPX))+100.0;
        for(int i=600-minPX;i<900-minPX;i++)
        LUT_Hueso[i]=200.0;
        for(int i=900-minPX;i<maxPX-minPX;i++)
        LUT_Hueso[i]=255.0;
        //*********************************************************

        AtenuacionTrackBar->Max = maxPX-minPX;
        AtenuacionTrackBar->Min = 1;
        AtenuacionTrackBar->Position = (maxPX-minPX)/2;

        }
        else return;  //if Execute()....

       if(ImagenEnPantalla == true)
       {

       //Llena el SliceListBox con los índices de las imágenes:
       for (int k=0;k<cantImagenes;k++)
       SliceListBox->Items->Strings[k]=k;

     //  int zindex = atoi(SliceListBox->Items->Strings[0].c_str());

       //Cargar primer imagen en pantalla:
       Cuadro = new Matriz(anchoPX,altoPX);

       SliceListBox->ItemIndex=0;
       //RefreshPantallaFiltros();

       //Inicia Thread Para Cálculo de Histogramas:
       //**********************************************************************
       histoCaja = new float[maxPX-minPX+1];

       HistoChart->Series[0]->Clear();
       HistoChart->Series[1]->Clear();
       HistoChart->Series[1]->AddXY(Center,100000,"",clRed);//double(histoCaja[Center-minPX]),"",clRed);
       HistoChart->Series[1]->AddXY(Ls,100000,"",clBlue);//double(histoCaja[Ls-minPX]),"",clBlue);
       HistoChart->Series[1]->AddXY(Li,100000,"",clBlue);//double(histoCaja[Li-minPX]),"",clBlue);

       //***********************************************************************

      //Creación de la LUT:
      Width =  WidthTrackBar->Position;
      Center = CenterTrackBar->Position;
      RefreshLUT();
      }

      //Creación de la Matriz de Agrupación para la Goma3D (MapaIndices) e
      //inicialización de matrices auxiliares para Marching Cubes:
      //************************************************************************
      IndicesBorrar = new TList;
      for(int i=0; i<750; i++)
        for(int j=0; j<650; j++)
          MapaIndices[i][j] = new TList;

      TriangList = new TList;
      TriangColors = new TList;
      Normals =  new TList;

      TriList = new TList;
      //************************************************************************

        PC1.x=altoPX/2;
        PC1.y=anchoPX/2;
        PC1.z=0;

        PlanoCorte=new PuntoF* [altoPX];
        for (int i=0; i<altoPX; ++i)
        {
        PlanoCorte[i]=new PuntoF[anchoPX];
        }

        PlanoOrig=new PuntoF* [altoPX];
        for (int i=0; i<altoPX; ++i)
        {
        PlanoOrig[i]=new PuntoF[anchoPX];
        }

        for(int i=0; i<altoPX; i++)
         for(int j=0; j<anchoPX; j++)
         {
         PlanoOrig[i][j].x=i;
         PlanoOrig[i][j].y=j;
         PlanoOrig[i][j].z=0;
         }


         vector1.x=1;
         vector1.y=0;
         vector1.z=0;
      //************************************************************************

      GraficarEnPantalla();

      CilindrosList = new TList;
      EsferasList = new TList;
      SelectedPoints = new TList;
      BorderPoints = new TList;

      //Interpolación lineal entre slice y slice:
      float colorinterp;
      int ki, kf;
      int l0 = 0;

        for (int k=0; k<cantImagenes-1; k=k+N)
        {
           for(int i=0; i<anchoPX; i++)
                 for(int j=0; j<altoPX; j++)
                 {
                 ki = M->GetV(i,j,k);
                 kf = M->GetV(i,j,k+N);

                 l0 = k;
                 
                        for(int l=k+1; l<k+N; l++)
                        {
                        colorinterp = ki + float(kf-ki)/float(N)*float(l-l0);
                        M->SetV(i,j,l, int(colorinterp));
                        }
                 }
        }

        histo_thread = new calcHistoThread(true);
        histo_thread->receiveParams(M, histoCaja, LUT, maxPX-minPX+1,
                                   minPX,HistoChart,HistoProcessLabel, &realminPX, &histo_thread_terminado);

        histo_thread->Resume();

        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::X1Click(TObject *Sender)
{
     for(int i=0; i<anchoPX; i++)
        for(int j=0; j<altoPX; j++)
           auxArray[i][j]=minPX;

       int offset = 5;
       int n=offset-1;
       double valor;

       double masc[5][5] ={{0 ,-1, 0, 1, 0},
                           {-1,-2, 0, 2, 1},
                           {-2,-4, 0, 4, 2},
                           {-1,-2, 0, 2, 1},
                           {0 ,-1, 0, 1, 0}
                           };


        for (int k=0;k<cantImagenes;k++)
        {
        ProgressBar1->Position=k;
        for(int i=offset; i<anchoPX-offset;i++)
                 for(int j=offset; j<altoPX-offset;j++)
                        {
                        valor = 0;
                        for(int x=-n/2; x<=n/2; x++)
                         for(int y=-n/2; y<=n/2; y++)
                          {
                          valor+=(M->GetV(i+x,j+y,k)*masc[x+n/2][y+n/2]);
                          }

                        auxArray[i][j]=abs(valor)/14.0;
                        }
            for(int i=0; i<512;i++)
                 for(int j=0; j<512;j++)
                     M->SetV(i,j,k,auxArray[i][j]);
        }

        ProgressBar1->Position=0;

        GraficarEnPantalla();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Y1Click(TObject *Sender)
{
 for(int i=0; i<anchoPX; i++)
        for(int j=0; j<altoPX; j++)
           auxArray[i][j]=minPX;

       int offset = 5;
       int n=offset-1;
       double valor;

       double masc[5][5] ={{ 0 ,-1, -2, -1, 0},
                           {-1,-2, -4, -2, -1},
                           { 0,0, 0, 0, 0},
                           {1,2, 4, 2, 1},
                           {0 ,1, 2, 1, 0}
                           };


        for (int k=0;k<cantImagenes;k++)
        {
        ProgressBar1->Position=k;
        for(int i=offset; i<anchoPX-offset;i++)
                 for(int j=offset; j<altoPX-offset;j++)
                        {
                        valor = 0;
                        for(int x=-n/2; x<=n/2; x++)
                         for(int y=-n/2; y<=n/2; y++)
                          {
                          valor+=(M->GetV(i+x,j+y,k)*masc[x+n/2][y+n/2]);
                          }

                        auxArray[i][j]=abs(valor)/14.0;
                        }
            for(int i=0; i<512;i++)
                 for(int j=0; j<512;j++)
                     M->SetV(i,j,k,auxArray[i][j]);
        }

        ProgressBar1->Position=0;

        GraficarEnPantalla();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LabelButtonClick(TObject *Sender)
{
     EsquelPoints->Pack();
     EP->Pack();
     LP->Pack();
     JP->Pack();

     if(EsquelPoints->Count==0)
     return;

     Punto* pp; //puntero a punto:

     TList* EsquelSinJPPoints = new TList;

     //Clasificacion de los Exit Points como End-Points:
     for(int i=0; i<EP->Count; i++)
     {
     pp = (Punto*)EP->Items[i];
     EsquelSinJPPoints->Add(pp);
     }

     for(int i=0; i<LP->Count; i++)
     {
     pp = (Punto*)LP->Items[i];
     EsquelSinJPPoints->Add(pp);
     }

     EP->Clear();
     LP->Clear();

     clasificacionEsqueleto(EsquelSinJPPoints,
                                minX, minY, minZ,
                                maxX, maxY, maxZ,
                                EP, LP, JP);
    //sp: Seeding Point
    //CP: Conjunto de puntos candidatos
    //AC: Conjunto acumulativo de puntos conectados al sp
    //PL: Conjunto de todos los puntos que satisfacen la condición dentro del volumen M
    //    Finalmente, se elimina de PL todo elementos que esté en AC, funcionando como
    //    una lista que agrupa todos los puntos NO conectados con sp.

    //Volumen Auxiliar: 
    Volumen* MAuxiliar = new Volumen(anchoPX,altoPX,cantImagenes);

    TList* PL = new TList;

    for(int i=0; i<JP->Count; i++)
    {
    pp = (Punto*)JP->Items[i];
    PL->Add(pp);
    MAuxiliar->SetV(pp->x,pp->y,pp->z,1);
    }

    Punto* sp; //seed-point:

    //Clasificación de Nodos: Los JP 26 conectados en el conjunto de JP:
    AnsiString auxstring;
    for(int i=0; i<JP->Count; i++)
    {
    sp = (Punto*)JP->Items[i];
      if(MAuxiliar->GetV(sp->x,sp->y,sp->z)==0)
      continue;
    TList* AC = new TList;
    getAllConnected(sp, MAuxiliar, PL, AC);
    NodeList->Add(AC);
    auxstring=i; auxstring+="("; auxstring+=AC->Count; auxstring+=")";
    LabelForm1->NodeListBox->Items->Add(auxstring);
    }

    LabelForm1->NodeListBox->ItemIndex=0;

    //Limpieza de la Matriz Auxiliar:
    for(int i=0; i<MAuxiliar->getAncho(); i++)
      for(int j=0; j<MAuxiliar->getAlto(); j++)
        for(int k=0; k<MAuxiliar->getProf(); k++)
         MAuxiliar->SetV(i,j,k,0);

    //Llenado de PL: ¿necesario?
    PL->Clear();

    for(int i=0; i<LP->Count; i++)
    {
    pp = (Punto*)LP->Items[i];
    PL->Add(pp);
    MAuxiliar->SetV(pp->x,pp->y,pp->z,1);
    }

    for(int i=0; i<EP->Count; i++)
    {
    pp = (Punto*)EP->Items[i];
    PL->Add(pp);
    MAuxiliar->SetV(pp->x,pp->y,pp->z,1);
    }

    //Clasificacion x rama: los EP y ExP son las semillas:
    for(int i=0; i<EP->Count; i++)
    {
    sp = (Punto*)EP->Items[i];
     if(MAuxiliar->GetV(sp->x,sp->y,sp->z)==0)
      continue;
    TList* AC = new TList;
    getAllConnected(sp, MAuxiliar, PL, AC);
    RamaList->Add(AC);
    }

    TList* auxList;
    for(int i=0; i<RamaList->Count; i++)
    {
    auxList = (TList*)RamaList->Items[i];
    auxstring = i; auxstring+="(";auxstring+=auxList->Count;auxstring+=")";
    LabelForm1->RamaListBox->Items->Add(auxstring);
    }

    LabelForm1->RamaListBox->ItemIndex=0;

    //Limpieza de la Matriz Auxiliar: Se llena con -1
    for(int i=0; i<MAuxiliar->getAncho(); i++)
      for(int j=0; j<MAuxiliar->getAlto(); j++)
        for(int k=0; k<MAuxiliar->getProf(); k++)
         MAuxiliar->SetV(i,j,k,-1);

    TList* AuxList;
    //Llenado de la matriz auxiliar con los indices de las ramas:
    for(int i=0; i<RamaList->Count; i++)
    {
      AuxList = (TList*)RamaList->Items[i];

      for(int j=0; j<AuxList->Count; j++)
      {
      pp = (Punto*)AuxList->Items[j];
      MAuxiliar->SetV(pp->x,pp->y,pp->z,i);
      }
    }

    //matrizNodoRama: es una matriz de NxR, con N=cant de nodos, y R=cant de ramas.
    //cada elemento fila de la matriz tiene un 0 si el nodo correspondiente a esa fila
    //esta conectado a esa rama, y tiene un 1 si el nodo conectado a esa rama tiene un 1. 
    matrizNodoRama = new int*[NodeList->Count];
    for(int i=0; i<NodeList->Count; i++)
    {
    matrizNodoRama[i] = new int[RamaList->Count];
        for(int j=0; j<RamaList->Count; j++)
           matrizNodoRama[i][j] = 0;
    }

    int ramaindex;

    //Búsqueda de las ramas conectadas a los nodos:
    for(int i=0; i<NodeList->Count; i++)
    {
     AuxList = (TList*)NodeList->Items[i];

      //Nodo i-esimo:
      for(int j=0; j<AuxList->Count; j++)
      {
      pp = (Punto*)AuxList->Items[j];

      //Búsqueda en los 26 vecinos:
      for(int x=-1;x<2;x++)
       for(int y=-1;y<2;y++)
        for(int z=-1;z<2;z++)
        {
        ramaindex = MAuxiliar->GetV(pp->x+x,pp->y+y,pp->z+z);
         if(ramaindex!=-1)
         matrizNodoRama[i][ramaindex]=1;
        }
      }
    }

    LabelForm1->Show();

    LargosRama = new double[RamaList->Count];
    LargosSmoothRama = new double[RamaList->Count];
    VolumenRama = new double[RamaList->Count];

    delete MAuxiliar;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CentrarButtonClick(TObject *Sender)
{
  double x=0,y=0,z=0;
  RenderPunto* ap;

  for(int i=0; i<RenderPoints->Count; i++)
     {ap = (RenderPunto*)RenderPoints->Items[i];
      x+=ap->p.x; y+=ap->p.y; z+=ap->p.z;}

      x/=RenderPoints->Count;y/=RenderPoints->Count;z/=RenderPoints->Count;
      medX = x; medY = y; medZ = z;
}
//---------------------------------------------------------------------------
void TForm1::orientarRamas(int nodo, int rama)
{
  matrizNodoRama[nodo][rama] = 2;

  TList* cola = new TList;
  int* prox;

  for(int i=0; i<RamaList->Count; i++)
  {
   if(matrizNodoRama[nodo][i]==1)
   {
   prox = new int[1];
   prox[0] = i;
   cola->Add(prox);
   //-1: indicador de que ya fue visitada
   matrizNodoRama[nodo][i]=-1;
   }
  }

  int ramaactual;

  while(cola->Count!=0)
  {
    ramaactual = *(int*)cola->Items[0];
    cola->Delete(0);

     for(int j=0; j<NodeList->Count; j++)
     {
      if(matrizNodoRama[j][ramaactual]==1)
      {
      matrizNodoRama[j][ramaactual]=2;

        for(int i=0; i<RamaList->Count; i++)
        {
         if(matrizNodoRama[j][i]==1)
         {
         prox = new int[1];
         prox[0] = i;
         cola->Add(prox);
         //-1: indicador de que ya fue visitada
         matrizNodoRama[j][i]=-1;
         }
        }
      }
     }
  }

  for(int i=0; i<NodeList->Count; i++)
   for(int j=0; j<RamaList->Count; j++)
    {
    //Restaura el valor original: 1-conectado, 0-no conectado, 2-rama Madre
    if(matrizNodoRama[i][j]==-1)
    matrizNodoRama[i][j]=1;
    }

    double x,y,z;
    TList* PuntosList;
    TList* AuxList;
    RenderPunto* rp;
    Punto* p;

    double mindist = maxX+maxY+maxZ;
    double dist;
    int indiceRama;
    TList* vr;

    maxPuntosRama = 0;
    minPuntosRama = RenderPoints->Count;

    for(int i=0; i<RamaList->Count; i++)
    {
     vr = new TList;
     VolRamaList->Add(vr);
    }

    RenderPoints->Pack();

    for(int i=0; i<RenderPoints->Count; i++)
    {
       //Tomo el primer punto a clasificar:
       rp = (RenderPunto*)RenderPoints->Items[i];

         //Lo comparo con todos los puntos de todas las ramas,
         //para buscar la más cercana:
         for(int j=0; j<RamaList->Count; j++)
         {
            PuntosList = (TList*)RamaList->Items[j];

            for(int k=0; k<PuntosList->Count; k++)
            {
             p = (Punto*)PuntosList->Items[k];

             dist = sqrt( pow((p->x-rp->p.x)*res[0],2) + pow((p->y-rp->p.y)*res[1],2) + pow((p->z-rp->p.z)*res[2],2));

              if(dist<mindist)
              {
              mindist = dist;
              indiceRama = j;
              }
            }
         }

       mindist =  maxX+maxY+maxZ;
       AuxList = (TList*)VolRamaList->Items[indiceRama];
       AuxList->Add(rp);
    }

    //Cálculo de los volumenes:
    volVoxel = res[0]*res[1]*res[2];

    for(int i=0; i<VolRamaList->Count; i++)
    {
    AuxList = (TList*)VolRamaList->Items[i];

    if(AuxList->Count>maxPuntosRama)
    maxPuntosRama = AuxList->Count;

    if(AuxList->Count<minPuntosRama)
    minPuntosRama = AuxList->Count;

    VolumenRama[i] = AuxList->Count*volVoxel;
    }

    //Cálculo de los Largos sin suavizar:
   Punto* pi, *pf;
   double largo;
   for(int j=0; j<RamaList->Count; j++)
         {
            PuntosList = (TList*)RamaList->Items[j];

            //Para las ramas demasiado cortas, de 1 solo elemento,
            //la longitud se toma como la media geométrica de las
            //dimensiones del voxel:
            if(PuntosList->Count<2)
            {
            LargosRama[j] = pow(res[0]*res[1]*res[2],1.0/3.0);
            continue;
            }

            largo = 0;
            //Para el resto de las Ramas:
            for(int k=0; k<PuntosList->Count-1; k++)
            {
             pi = (Punto*)PuntosList->Items[k];
             pf = (Punto*)PuntosList->Items[k+1];

             largo += sqrt( pow((pf->x-pi->x)*res[0],2) + pow((pf->y-pi->y)*res[1],2) + pow((pf->z-pi->z)*res[2],2));
            }

            LargosRama[j]=largo;
         }


       //Cálculo de las ramas suavizadas: Media móvil de 5 o 3 elementos
        //5 para ramas largas, 3 para ramas cortas o porciones iniciales de las ramas largas:
        Punto* p1, *p2, *p3, *p4, *p5;
        PuntoF* ps; //punto smoothed
        float *nuevox;
        float *nuevoy;
        float *nuevoz;
        TList* SmoothPuntosList;

        for(int j=0; j<RamaList->Count; j++)
         {
            PuntosList = (TList*)RamaList->Items[j];

            //Para las ramas demasiado cortas, de 1 solo elemento,
            //la longitud se toma como la media geométrica de las
            //dimensiones del voxel:
             SmoothPuntosList = new TList;

            //Si las ramas son muy cortas, las copia idénticas:
            if(PuntosList->Count<5)
            {
              for(int i=0; i<PuntosList->Count; i++)
              {
              p1 = (Punto*)PuntosList->Items[i];
              ps = new PuntoF;
              ps->x = p1->x;
              ps->y = p1->y;
              ps->z = p1->z;
              SmoothPuntosList->Add(ps);
              }
              RamaSmoothList->Add(SmoothPuntosList);
              continue;
            }

            //Para el resto de las Ramas:
            nuevox = new float[PuntosList->Count];
            nuevoy = new float[PuntosList->Count];
            nuevoz = new float[PuntosList->Count];

            p1 = (Punto*)PuntosList->Items[0];
            p2 = (Punto*)PuntosList->Items[1];
            p3 = (Punto*)PuntosList->Items[2];

            //El primer elemento se copia igual:
            nuevox[0] = float(p1->x);
            nuevoy[0] = float(p1->y);
            nuevoz[0] = float(p1->z);

            //Al segundo elemento se le hace un filtro de media movil de 3 elementos:
            nuevox[1] = float(p1->x + p2->x + p3->x)/3.0;
            nuevoy[1] = float(p1->y + p2->y + p3->y)/3.0;
            nuevoz[1] = float(p1->z + p2->z + p3->z)/3.0;

            //Al resto de los elementos (excepto penúltimo y úlltimo), se le hace un
            //filtro de media móvil de 5 elementos:
            for(int k=2; k<PuntosList->Count-2; k++)
            {
             p1 = (Punto*)PuntosList->Items[k-2];
             p2 = (Punto*)PuntosList->Items[k-1];
             p3 = (Punto*)PuntosList->Items[k];
             p4 = (Punto*)PuntosList->Items[k+1];
             p5 = (Punto*)PuntosList->Items[k+2];

            nuevox[k] = float(p1->x + p2->x + p3->x + p4->x + p5->x)/5.0;
            nuevoy[k] = float(p1->y + p2->y + p3->y + p4->y + p5->y)/5.0;
            nuevoz[k] = float(p1->z + p2->z + p3->z + p4->z + p5->z)/5.0;
            }

            //Al penúltimo elemento se le hace un filtro de media móvil de 3 el:
            p1 = (Punto*)PuntosList->Items[PuntosList->Count-3];
            p2 = (Punto*)PuntosList->Items[PuntosList->Count-2];
            p3 = (Punto*)PuntosList->Items[PuntosList->Count-1];

            nuevox[PuntosList->Count-2] = float(p1->x + p2->x + p3->x)/3.0;
            nuevoy[PuntosList->Count-2] = float(p1->y + p2->y + p3->y)/3.0;
            nuevoz[PuntosList->Count-2] = float(p1->z + p2->z + p3->z)/3.0;

            //El último elemento se copia igual:
            nuevox[PuntosList->Count-1] = float(p3->x);
            nuevoy[PuntosList->Count-1] = float(p3->y);
            nuevoz[PuntosList->Count-1] = float(p3->z);

            //Se crean las ramas suavizadas:
            for(int k=0; k<PuntosList->Count; k++)
            {
            ps = new PuntoF;
            ps->x = nuevox[k];
            ps->y = nuevoy[k];
            ps->z = nuevoz[k];
            SmoothPuntosList->Add(ps);
            }

            RamaSmoothList->Add(SmoothPuntosList);
            delete[] nuevox;
            delete[] nuevoy;
            delete[] nuevoz;
         }

   //Cálculo de los Largos suavizados:
   PuntoF* pfi;
   PuntoF* pff;

   for(int j=0; j<RamaSmoothList->Count; j++)
         {
            PuntosList = (TList*)RamaSmoothList->Items[j];

            //Para las ramas demasiado cortas, de 1 solo elemento,
            //la longitud se toma como la media geométrica de las
            //dimensiones del voxel:
            if(PuntosList->Count<2)
            {
            LargosSmoothRama[j] = pow(res[0]*res[1]*res[2],1.0/3.0);
            continue;
            }

            largo = 0;
            //Para el resto de las Ramas:
            for(int k=0; k<PuntosList->Count-1; k++)
            {
             pfi = (PuntoF*)PuntosList->Items[k];
             pff = (PuntoF*)PuntosList->Items[k+1];

             largo += sqrt( pow((pff->x-pfi->x)*res[0],2) + pow((pff->y-pfi->y)*res[1],2) + pow((pff->z-pfi->z)*res[2],2));
            }

            LargosSmoothRama[j]=largo;
         }

   LabelForm1->VolRamaCheckBox->Enabled = true;
  // LabelForm1->RamaListBox->Clear();
  // AnsiString LargosString;

  /* for(int j=0; j<RamaList->Count; j++)
   {
   LargosString = LargosRama[j];
   LargosString+= ";";
   LargosString+= LargosSmoothRama[j];
   LabelForm1->RamaListBox->Items->Add(LargosString);
   }*/

  //Creacioón de los nodos con sus respectivos valores acumulados:
  LargosAcum = new double[NodeList->Count];
  VolAcum = new double[NodeList->Count];

  double lacum = 0; //auxiliar largos acumulados
  double vacum = 0; //auxiliar volumenes acumulados
  cola->Clear();

  for(int i=0; i<NodeList->Count; i++)
  {
     for(int j=0; j<RamaList->Count; j++)
     {
       if(matrizNodoRama[i][j]==1)
       {
        matrizNodoRama[i][j]=-1;
        prox = new int[1];
        prox[0] = j;
        cola->Add(prox);
        lacum +=LargosSmoothRama[j];
        vacum +=VolumenRama[j];
       }
     }

    while(cola->Count!=0)
    {
    ramaactual = *(int*)cola->Items[0];
    cola->Delete(0);

     for(int j=0; j<NodeList->Count; j++)
     {
       if(matrizNodoRama[j][ramaactual]==2)
       {
          for(int k=0; k<RamaList->Count; k++)
          {
                if(matrizNodoRama[j][k]==1)
                {
                prox = new int[1];
                prox[0] = k;
                cola->Add(prox);
                //-1: indicador de que ya fue visitada
                matrizNodoRama[j][k]=-1;
                lacum +=LargosSmoothRama[k];
                vacum +=VolumenRama[k];
                } //end if
          } //end for k
       } //end if
     } //end for
    } //end while

    LargosAcum[i]=lacum;
    VolAcum[i]=vacum;
    lacum=0;
    vacum=0;

    //Restaura la matrizNodoRama
    for(int i=0; i<NodeList->Count;i++)
     for(int j=0; j<RamaList->Count; j++)
      if(matrizNodoRama[i][j]==-1)
       matrizNodoRama[i][j] = 1;

    cola->Clear();
  }

  LabelForm1->StringGrid1->Visible=true;
  LabelForm1->GrabarMapaButton->Visible = true;
  LabelForm1->StringGrid1->RowCount = NodeList->Count+1;

  LabelForm1->StringGrid1->Cells[1][0]="L";
  LabelForm1->StringGrid1->Cells[2][0]="V";
  LabelForm1->StringGrid1->Cells[3][0]="Am";
  LabelForm1->StringGrid1->Cells[4][0]="Dm";

  double area = 0;
  double diametro = 0;
   
  for(int i=1; i<=NodeList->Count; i++)
  {
  LabelForm1->StringGrid1->Cells[0][i] = i;
  LabelForm1->StringGrid1->Cells[1][i] = FloatToStrF(LargosAcum[i-1],ffNumber,7,2);
  LabelForm1->StringGrid1->Cells[2][i] = FloatToStrF(VolAcum[i-1]   ,ffNumber,7,2);

        for(int j=0; j<RamaList->Count; j++)
        {
         if(matrizNodoRama[i-1][j]==2)
         {
         area = VolumenRama[j]/LargosSmoothRama[j];
         diametro = sqrt(area*4.0/3.1416);
         break;
         }
        }
  LabelForm1->StringGrid1->Cells[3][i] = FloatToStrF(area,ffNumber,7,2);
  LabelForm1->StringGrid1->Cells[4][i] = FloatToStrF(diametro,ffNumber,7,2);
  }

  LabelForm1->RamaListBox->ItemIndex=0;
}

//---------------------------------------------------------------------------

void TForm1::prunning()
{

 //sumarama: variable auxiliar: suma la cantidad (y tipo de) nodo para cada rama
 //en la matriz de conectividad. Los nodos conectados a
 int sumarama = 0;
 int* ramasElim = new int[RamaList->Count];

 for(int i=0; i<RamaList->Count; i++)
   ramasElim[i] = 0;

 for(int r=0; r<RamaList->Count; r++)
 {
  for(int n=0; n<NodeList->Count; n++)
        {
        sumarama += matrizNodoRama[n][r];
        }

        if(sumarama<2) //ramas conectadas a un único nodo? (ramas laterales)=>podar:
         ramasElim[r]=1;

        sumarama = 0;
  }

  for(int r=RamaList->Count-1; r>0; r--)
  {
     if(ramasElim[r])
     {
     RamaList->Delete(r);
     LabelForm1->RamaListBox->Items->Delete(r);
     }
  }

  EsquelPoints->Clear();
  LP->Clear();
  JP->Clear();
  EP->Clear();

  TList* AuxList;
  Punto* pe;

  for(int i=0; i<RamaList->Count; i++)
  {
     AuxList = (TList*)RamaList->Items[i];

     for(int j=0; j<AuxList->Count; j++)
     {
      pe = (Punto*)AuxList->Items[j];
      EsquelPoints->Add(pe);
     }

  }

  for(int i=0; i<NodeList->Count; i++)
  {
     AuxList = (TList*)NodeList->Items[i];

     for(int j=0; j<AuxList->Count; j++)
     {
      pe = (Punto*)AuxList->Items[j];
      EsquelPoints->Add(pe);
     }
  }

 RamaList->Clear();
 NodeList->Clear();

 clasificacionEsqueleto(EsquelPoints,
                                minX, minY, minZ,
                                maxX, maxY, maxZ,
                                EP, LP, JP);

}
//---------------------------------------------------------------------------


void __fastcall TForm1::Anisotrpico1Click(TObject *Sender)
{
        //Implementación del filtro anisotrópico como resolución iterativa del proceso
        //difusivo anisotrópico:
        //It = div(c(x,y,t)*grad(I));
        //Referencia: Perona P, Malik J, "Scale-Space and Edge Detection Using Anisotropic Diffusion",
        //IEEE Transactions on Pattern Analysis and Machine Intelligence, vol. 12, Nº 7, July 1990.


        //Dentro del VOI:
           //Realiza filtro de mediana dentro de la ROI:
           int zf=ImageScrollBar->Position+1;
           int zi=ImageScrollBar->Position;
           if(zf==ImageScrollBar->Max)
           return;

           int xi = ROIShape->Top;
           int xf = ROIShape->Top+ROIShape->Height;
           int yi = ROIShape->Left;
           int yf = ROIShape->Left+ROIShape->Width;


        if(!ImagenEnPantalla)
        return;

        double K;
        double lambda = 0.0;
        int t;

       //Histograma de gradientes para el cálculo del mejor valor de K:
       //**************************************************************
       int* gradHisto = new int[maxPX-minPX+1];
       double* gradAcum = new double[maxPX-minPX+1];
       double* funcionG = new double[maxPX-minPX+1];

       for(int i=0; i<maxPX-minPX+1; i++)
       gradHisto[i] = 0;

       int gradix = 0;
       int gradiy = 0;
       double gradd = 0.0;
       double maxacum = 0.0;

       short int** aux;

       //Contenedor auxiliar para el filtrado corte a corte:
       aux = new short int*[M->getAlto()];
         for(int i=0; i<M->getAlto(); i++)
          aux[i] = new short int[M->getAncho()];


      //Variables del filtro anisotrópico:
      double cN, cS, cE, cW;  //coeficientes de difusión
      double gN, gS, gE, gW;  //gradientes locales

       ProgressBar1->Max = M->getProf();

        if(AnisotropicForm->ShowModal()==mrOk)
        {
           //Cálculo de los gradientes encontrados, para buscar el gradiente con percentil 90%:
           //Los límites de la imagen se corren en 1 para evitar problemas de contorno:
           //(Además, si los contornos se consideran con su gradiente, aumentarían espuriamente el valor de K
           //por no representar gradientes reales dentro de la imagen).
           for(int i=xi; i<xf; i++)
             for(int j=yi; j<yf; j++)
              for(int k=zi; k<zf; k++)
              {
              gradix = M->GetV(i+1,j,k)-M->GetV(i,j,k);
              gradd = sqrt(gradix*gradix);
              gradHisto[int(gradd)]++;

              gradiy = M->GetV(i,j+1,k)-M->GetV(i,j,k);
              gradd = sqrt(gradiy*gradiy);
              gradHisto[int(gradd)]++;
              }


           //Cálculo o asignación manual de K:
           if(AnisotropicForm->KRadioGroup->ItemIndex)  //0: automático, 1: manual
           K = atof(AnisotropicForm->KEdit->Text.c_str());
           else
              {
                     //Función acumulada:
                     gradAcum[0]=gradHisto[0];

                     for(int s=1; s<maxPX-minPX+1; s++)
                       gradAcum[s] = gradHisto[s]+gradAcum[s-1];

                     maxacum = gradAcum[maxPX-minPX];

                     for(int s=0; s<maxPX-minPX+1; s++)
                       gradAcum[s] /= maxacum;

                     for(int s=0; s<maxPX-minPX+1; s++)
                     {
                        if(gradAcum[s]>0.9)
                        {
                        K = s;
                        break;
                        }
                     }
              } //fin de cálculo de K

          lambda = atof(AnisotropicForm->LambdaEdit->Text.c_str());
          t = atoi(AnisotropicForm->ScaleEdit->Text.c_str());

          if(AnisotropicForm->FuncionGComboBox->ItemIndex==0) //0: gaussiana, 1: inversa sigmoidea
          {
               for(int s=0; s<maxPX-minPX+1; s++)
                funcionG[s] = exp(-(double(s)/K)*(double(s)/K));
          }
          else
          {
              for(int s=0; s<maxPX-minPX+1; s++)
               funcionG[s] = 1.0/(1.0 + double(s)/K*double(s)/K);
          }


           ProgressBar1->Max = zf-zi+1;

          //Comienzo del filtrado anisotrópico iterativo:
          for(int k=zi; k<zf; k++)
          {

          ProgressBar1->Position++;

          for(int st = 0; st<t+1; st++)
          {
           for(int i=xi; i<xf; i++)
             for(int j=yi; j<yf; j++)
               aux[i][j] = M->GetV(i,j,k);

           for(int i=xi+1; i<xf-1; i++)
             for(int j=yi+1; j<yf-1; j++)
             {
              //calculo de grads
              gN = aux[i-1][j]-aux[i][j];
              gS = aux[i+1][j]-aux[i][j];
              gE = aux[i][j+1]-aux[i][j];
              gW = aux[i][j-1]-aux[i][j];

              //calculo de coefs
              cN = funcionG[abs(gN)];
              cS = funcionG[abs(gS)];
              cE = funcionG[abs(gE)];
              cW = funcionG[abs(gW)];

              //pasaje de la nueva imagen a M
              M->SetV(i,j,k, aux[i][j]+lambda*(cN*gN+cS*gS+cE*gE+cW*gW));
             }
          }
         }

        }

          ProgressBar1->Position = 0;


          delete[] gradAcum;
          delete[] gradHisto;
          delete[] funcionG;

      //Contenedor auxiliar para el filtrado corte a corte:
         for(int i=0; i<M->getAlto(); i++)
          delete[] aux[i];

          delete[] aux;


}
//---------------------------------------------------------------------------

void __fastcall TForm1::Canny1Click(TObject *Sender)
{
       //Implementación del filtro de Canny:
       //Referencia: Jorge Valverde Rebaza, "Detección de bordes mediante el
       //algoritmo de Canny".

        if(!ImagenEnPantalla)
        return;

        bool automatico;

       //Contenedor auxiliar para el filtrado corte a corte:
       short int** auxG; //gradientes auxiliares:

       auxG = new short int*[M->getAlto()];
         for(int i=0; i<M->getAlto(); i++)
          auxG[i] = new short int[M->getAncho()];

       //Contenedor auxiliar para el filtrado corte a corte:
       short int** auxA; //ángulos auxiliares:

       auxA = new short int*[M->getAlto()];
         for(int i=0; i<M->getAlto(); i++)
          auxA[i] = new short int[M->getAncho()];

       //Contenedor auxiliar para el filtrado corte a corte:
       short int** auxI; //gradientes auxiliares:

       auxI = new short int*[M->getAlto()];
         for(int i=0; i<M->getAlto(); i++)
          auxI[i] = new short int[M->getAncho()];


       double angulo = 0;
       double gradX = 0;
       double gradY = 0;
       double magnitud = 0;

       short int t0,t1; //Gradientes de histéresis:
       int marcados = -1;

       double pM, pm; //percentiles máximo y mínimo de gradiente aceptados:

       if(CannyFilterForm->ShowModal()==mrOk)
       {
           automatico = CannyFilterForm->automatico;

           if(automatico)
           {
           pM = CannyFilterForm->pM;
           pM /=100.0;

           pm = CannyFilterForm->pm;
           pm /=100.0;
           }

           if(!automatico)
           {
           t0 = CannyFilterForm->vm;
           t1 = CannyFilterForm->vM;
           }

       }
       else return;


      if(automatico)
      {
       //Auxiliares para el cálculo de los gradientes:
       int* gradHisto = new int[maxPX-minPX+1];
       double* gradAcum = new double[maxPX-minPX+1];

       for(int s=0; s<maxPX-minPX+1; s++)
       {
       gradHisto[s]=0;
       gradAcum[s]=0;
       }

       //Creación del histograma de gradientes y el acumulado:
       for(int k=0; k<M->getProf(); k++)
          for(int i=1; i<M->getAlto()-1; i++)
            for(int j=1; j<M->getAncho()-1; j++)
            {
               gradX = M->GetV(i+1,j-1,k) +
                        M->GetV(i+1,j,k)   +
                        M->GetV(i+1,j+1,k) -
                        M->GetV(i-1,j-1,k) -
                        M->GetV(i-1,j,k)   -
                        M->GetV(i-1,j+1,k);
                gradX /=3.0;

                gradY = M->GetV(i-1,j+1,k) +
                        M->GetV(i,j+1,k)   +
                        M->GetV(i+1,j+1,k) -
                        M->GetV(i-1,j-1,k) -
                        M->GetV(i,j-1,k)   -
                        M->GetV(i+1,j-1,k);
                gradY /=3.0;

               gradHisto[abs(sqrt(gradX*gradX + gradY*gradY))]++;
            }


          gradAcum[0]=gradHisto[0];
          double maxacum;

          for(int s=1; s<maxPX-minPX+1; s++)
            gradAcum[s] = gradHisto[s]+gradAcum[s-1];

           maxacum = gradAcum[maxPX-minPX];

          for(int s=0; s<maxPX-minPX+1; s++)
            gradAcum[s] /= maxacum;

         for(int s=0; s<maxPX-minPX+1; s++)
            if(gradAcum[s]>pm)
             {
              t0 = s;
              break;
             }

         for(int s=t0; s<maxPX-minPX+1; s++)
            if(gradAcum[s]>pM)
             {
              t1 = s;
              break;
             }

      }// end if(automatico)...

       //Pasos: 1º Filtro Gaussiano? -> Reemplazar por filtro anisotrópico
       //       2º Cálculo de gradientes
       //       3º Supresión no máxima
       //       4º Histéresis de umbrales


       ProgressBar1->Max = M->getProf();

       for(int k=0; k<M->getProf(); k++)
       {
       ProgressBar1->Position++;

           //Corte a corte: Gradientes calculados mediante máscara Prewitt:
           //mX = [-1 0 1]x3
           //mY = [-1;0;1]x3
           for(int i=1; i<M->getAlto()-1; i++)
            for(int j=1; j<M->getAncho()-1; j++)
            {
                //Cáculo de gradientes locales y ángulos:
                 gradX = M->GetV(i+1,j-1,k) +
                         M->GetV(i+1,j,k)   +
                         M->GetV(i+1,j+1,k) -
                         M->GetV(i-1,j-1,k) -
                         M->GetV(i-1,j,k)   -
                         M->GetV(i-1,j+1,k);
                 gradX /= 3.0;

                 gradY = M->GetV(i-1,j+1,k) +
                         M->GetV(i,j+1,k)   +
                         M->GetV(i+1,j+1,k) -
                         M->GetV(i-1,j-1,k) -
                         M->GetV(i,j-1,k)   -
                         M->GetV(i+1,j-1,k);
                 gradY /=3.0;

                 magnitud = sqrt(gradX*gradX + gradY*gradY);

                 auxG[i][j] = magnitud;

                 //Convención:
                 //Todos los ángulos deben quedar contenidos en el intervalo 0º-135º
                 if(gradX == 0 && gradY==0) //Gradiente nulo:
                 angulo = 0.0;

                 if(gradX > 0 && gradY==0)  //Gradiente en el 1er cuadrante: 0 a 90º
                 angulo = 0.0;

                 if(gradX >0 && gradY>0)
                 angulo = atan(gradY/gradX)*180.0/3.1416;

                 if(gradX == 0 && gradY>0)
                 angulo = 90.0;

                 if(gradX < 0 && gradY>0)    //Gradiente en el 2º cuadrante: 90º a 180º
                 angulo = 180.0 + atan(gradY/gradX)*180.0/3.1416;

                 if(gradX<0 && gradY==0)
                 angulo = 180.0;

                 if(gradX<0 && gradY<0)     //Gradiente en el 3er cuadrante: 180º a 270º
                 angulo = atan(gradY/gradX)*180.0/3.1416; //=> se refleja al 1er cuadrante

                 if(gradX==0 && gradY<0)
                 angulo = 90.0;

                 if(gradX>0 && gradY<0)     //Gradiente en el 4to cuadrante: 270º a 360º
                 angulo = 180.0 + atan(gradY/gradX)*180.0/3.1416; //Se refleja al 2º cuadrante

                 //Asignar el ángulo en las 4 direcciones principales: 0, 45, 90, 135;
                 if(angulo>0.0 && angulo <22.5)
                 angulo =0.0;

                 if(angulo>22.5 && angulo <67.5)
                 angulo =45.0;

                 if(angulo>67.5 && angulo <112.5)
                 angulo =90.0;

                 if(angulo>112.5 && angulo <157.5)
                 angulo =135.0;

                 if(angulo>157.5 && angulo <180.0)
                 angulo=0.0;

                 auxA[i][j] = angulo;
            }

          //2º paso: supresión no-máxima:
          for(int i=1; i<M->getAlto()-1; i++)
            for(int j=1; j<M->getAncho()-1; j++)
            {

               switch ( auxA[i][j] )
               {

               case 0 :
                 if(auxG[i][j]<auxG[i+1][j] || auxG[i][j]<auxG[i-1][j])
                 auxI[i][j]=0;
                 else auxI[i][j] = auxG[i][j];
               break;

               case 45 :
                 if(auxG[i][j]<auxG[i+1][j+1] || auxG[i][j]<auxG[i-1][j-1])
                 auxI[i][j]=0;
                 else auxI[i][j] = auxG[i][j];
               break;

               case 90 :
                 if(auxG[i][j]<auxG[i][j+1] || auxG[i][j]<auxG[i][j-1])
                 auxI[i][j]=0;
                 else auxI[i][j] = auxG[i][j];
               break;

               case 135 :
                 if(auxG[i][j]<auxG[i+1][j-1] || auxG[i][j]<auxG[i-1][j+1])
                 auxI[i][j]=0;
                 else auxI[i][j] = auxG[i][j];
               break;
               }
            }

           //Paso 3: Histéresis de umbral (entre t0 y t1):

           //Primera búsqueda de gradientes que superan el umbral mayor, t1:
           for(int i=1; i<M->getAlto()-1; i++)
            for(int j=1; j<M->getAncho()-1; j++)
            {
            if(auxI[i][j]>t1)//Si supera el umbral => marcado (-1)
            auxI[i][j] = -1; //-1: marcado
            }

          marcados = -1;

          while(marcados != 0)
          {
           marcados = 0;

           //Búsqueda iterativa de bordes secundarios por histéresis.
           //Condición: ser 8 vecino de un pixel marcado (marcado como -1).
           for(int i=1; i<M->getAlto()-1; i++)
            for(int j=1; j<M->getAncho()-1; j++)
            {
             //Si está entre t0 y t1:
             //Recorro la 8-vecindad: si algún vecino está marcado, marcarlo también:
             if(auxI[i][j]>=t0 && auxI[i][j]<t1)
             {
              for(int x=-1; x<2; x++)    //Recorro su 8 vecindad:
               for(int y=-1; y<2; y++)
               {
                if((abs(i)+abs(j))!=0 && auxI[i+x][j+y] == -1)
                {
                auxI[i][j] = -1; //marcado (-1)
                marcados++;
                }//end if
               }//end for y
              }//end if
             }//end for j
          }//end while

         for(int i=1; i<M->getAlto()-1; i++)
           for(int j=1; j<M->getAncho()-1; j++)
            {
            if(auxI[i][j]==-1)//Si supera el umbral => marcado (-1)
            M->SetV(i,j,k, maxPX);
            else M->SetV(i,j,k, minPX);
            }

       }//end for k...

       ProgressBar1->Position = 0;

       //delete de los contenedores auxiliares para el filtrado corte a corte:
         for(int i=0; i<M->getAlto(); i++)
          delete[] auxG[i];

          delete[] auxG;

         for(int i=0; i<M->getAlto(); i++)
          delete[] auxI[i];

          delete[] auxI;

         for(int i=0; i<M->getAlto(); i++)
          delete[] auxA[i];

          delete[] auxA;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Houghcircular1Click(TObject *Sender)
{
       if(!ImagenEnPantalla)
       return;

       TList* Circle = new TList;

       int zf=atoi(FinSliceEdit->Text.c_str());
       int zi=atoi(IniSliceEdit->Text.c_str());

       ProgressBar1->Max = zf-zi;

       int xi = ROIShape->Top;
       int xf = ROIShape->Top+ROIShape->Height;
       int yi = ROIShape->Left;
       int yf = ROIShape->Left+ROIShape->Width;

       int ri = 20;
       int rf = 35;
       
       double H = 0;
       double rmax = 0;

      double contR = 0;
      double Hmax;
      double xMax, yMax;

       TList** Hs;
       Hs = new TList*[zf-zi];

       for(int i=0; i<(zf-zi); i++)
       Hs[i] = new TList;

       Punto* paux;
       dynPunto* dynpaux;

       //Espacio de Houg;

      for(int k=zi; k<zf; k++)
      {

      ProgressBar1->Position++;

       //Para todos los radios...
       for(int r=ri; r<rf; r++)
       {
       Hmax = 0;
       BressenhamCircle(0, 0, 0, r, Circle);

        //Recorrer un corte del volumen:
         for(int i=xi; i<xf; i++)
          for(int j=yi; j<yf; j++)
          {
            H = 0;
            contR = 0;

            //Convolución:
            for(int s=0; s<Circle->Count; s++)
            {
            paux = (Punto*)Circle->Items[s];
            //Si cae dentro de los límites de la VOI:

              if( (i+paux->x)>=xi && (i+paux->x)<=xf && (j+paux->y)>=yi && (j+paux->y)<=yf && (k+paux->z)>=zi && (k+paux->z)<=zf)
              {
                 if(M->GetV(i+paux->x, j+paux->y, k+paux->z) == maxPX)
                 {
                 H += 1;//double(M->GetV(i+paux->x, j+paux->y, k+paux->z) - minPX) / float(maxPX);
                 contR++;
                 }
              }
            }

            //Normalización??
           // if(contR==0)
           // H=0;
           // else
           // H /= Circle->Count;

           if(H > Hmax)
           {
           Hmax = H;
           xMax = i;
           yMax = j;
           }

         }//Termina de recorrer toda la imagen para el radio r

          //Acá puede utilizarse algún criterio para descartar máximos:
          dynpaux = new dynPunto;
          dynpaux->x = xMax;
          dynpaux->y = yMax;
          dynpaux->r = r;
          dynpaux->z = k;
          dynpaux->H = Hmax;
          dynpaux->visited = false;
          dynpaux->previous = NULL;

          Hs[k-zi]->Add(dynpaux);

         //Elimina el círculo:
         for(int s=0; s<Circle->Count; s++)
         {
         paux = (Punto*)Circle->Items[s];
         delete paux;
         }
         Circle->Clear();
       }
      }

      ProgressBar1->Position = 0;

      //A partir de este punto el array de Listas Hs (espacios de Hough) tiene
      //en sus k niveles los máximos para cada radio:

      //Búsqueda iterativa del camino de mínimo costo:

      dynPunto *p1, *p2;

      float* costovec;

     //Armado del Grafo de Costos:
     //La lista de Vecinos guardará los punteros a los grafos vecinos
     //La lista de Costos guardará los valores de moverse hacia el grafo vecino guardado en
     //la lista Vecinos:

     TList* G = new TList; //grafo para el algoritmo de Dijkstra
     int indice = 0; //Organiza los puntos con un índice global dentro del grafo G
     int infinito = 100000;

     for(int k=0; k<(zf-zi)-1; k++)
     {
        for(int j=0; j<Hs[k]->Count; j++)
        {
        p1 = (dynPunto*)Hs[k]->Items[j];

        p1->Vecinos = new TList;
        p1->Costos = new TList;

           //Armado del grafo a través de punteros, y calculo de los costos entre vecinos:
           for(int i=0; i<Hs[k+1]->Count; i++)
           {
           p2 = (dynPunto*)Hs[k+1]->Items[i];
           p1->Vecinos->Add(p2);

           costovec = new float[1];
           costovec[0] = infinito;

           if(p1->H != 0.0)
           costovec[0] = CostoHough(p1->x,p1->y,p1->r,p1->H,  p2->x,p2->y,p2->r);

           p1->Costos->Add(costovec);
           }

        //Asignación de un índice global:
        p1->index = indice;
        indice++;

        G->Add(p1);
        }
     }

     //Caso especial: último corte, sin vecinos, pero con nodos. Todos los nodos
     //deben tener inicializado el TList, pero con un tamaño 0 de vecinos.
     for(int j=0; j<Hs[zf-zi-1]->Count; j++)
     {
       p1 = (dynPunto*)Hs[zf-zi-1]->Items[j];

       p1->Vecinos = new TList;
       p1->Vecinos->Capacity = 0;
       p1->Costos = new TList;
       p1->Costos->Capacity = 0;

       //Asignación de un índice global:
       p1->index = indice;
       indice++;

       G->Add(p1);
     }

     int source;
     int destino;
     float* distancias;
     float distancia;

     float minimo;
     float infimo = 100000;
     float sourcePrincipal;


    //Busca la mínima distancia utilizando como source los puntos del primer corte:
    for(int i=0; i<Hs[0]->Count; i++)
    {
     //Una vez recopilada la distancia en el árbol de distancias, se buscará la mínima
     //distancia entre los nodos correspondientes al último corte:
     dynpaux = (dynPunto*)Hs[0]->Items[i];
     source = dynpaux->index;

     distancias = Dijkstra(G, source);
     minimo = 100000;

        //Búsqueda de la mínima distancia enjtr los nodos del último corte:
        for(int j=0; j<Hs[zf-zi-1]->Count; j++)
        {
          dynpaux = (dynPunto*)Hs[zf-zi-1]->Items[j];
          destino = dynpaux->index;
          distancia = distancias[destino];

           if(distancia<minimo)
           minimo = distancia;
        }

     //El source que obtenga un valor de distancia que sea mínima entre los mínimos (el ínifimo) será considerado el
     //source principal, y el nodo del último corte que haya dado esta mínima distancia será el source
     //final elegidos por la concatenación de transformadas de Hough para múltiples radios.
        if(minimo<infimo)
        {
        infimo = minimo;
        sourcePrincipal = i;
        }
    }

    //Repito el algoritmo para SourcePrincipal:
    distancias = Dijkstra(G, sourcePrincipal);
    int finPrincipal;

    //Repito la búsqueda del ínifimo:
    minimo = 100000;
    for(int j=0; j<Hs[zf-zi-1]->Count; j++)
        {
          dynpaux = (dynPunto*)Hs[zf-zi-1]->Items[j];
          destino = dynpaux->index;
          distancia = distancias[destino];

           if(distancia<minimo)
           {
           minimo = distancia;
           finPrincipal = destino;
           }
        }

    //Con el índice finPrincipal se puede obtener el camino óptimo siguiendo
    //los punteros previous:
    dynpaux = (dynPunto*)G->Items[finPrincipal];

    for(int i=0; i<zf-zi-1; i++)
    {
      BressenhamCircle(dynpaux->x,dynpaux->y,dynpaux->z,dynpaux->r, Circle);

       //Dibuja el círculo:
       for(int j=0; j<Circle->Count; j++)
       {
       paux = (Punto*)Circle->Items[j];
       M->SetV(paux->x, paux->y, paux->z, maxPX/2);
       }

       //Elimina el círculo:
         for(int s=0; s<Circle->Count; s++)
         {
         paux = (Punto*)Circle->Items[s];
         delete paux;
         }
         Circle->Clear();

      dynpaux = dynpaux->previous;
    }

    //Eliminación de la estructura dinámica G:
    for(int i=0; i<G->Count; i++)
    {
      dynpaux = (dynPunto*)G->Items[i];

      for(int j=0; j<dynpaux->Vecinos->Count;j++)
      dynpaux->Vecinos->Items[j] = NULL;

      for(int j=0; j<dynpaux->Costos->Count;j++)
      delete [] dynpaux->Costos->Items[j];

      delete dynpaux;

      G->Items[i] = NULL;
    }

    for(int i=0; i<zf-zi; i++)
    {
    Hs[i]->Clear();
    delete Hs[i];
    }

    G->Clear();
    delete G;

}
//---------------------------------------------------------------------------
float TForm1::CostoHough(float x1, float y1, float r1, float H1,  float x2, float y2, float r2)
{

float wr, wp, wh; //Pesos: de radio, de posicion, de valor de H

wr = 15.0;
wp = 300.0;
wh = 1.0;

float dr, dp;

dr = fabs(r1-r2);
dp = sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );

float Costo;
Costo = wr*dr + wp*dp + wh*(1.0/H1);

return Costo;
}
//---------------------------------------------------------------------------
float* TForm1::Dijkstra(TList* G, int source)
{

//source: índice al nodo de partida del grafo:

float *dist;
dist = new float[G->Count];

int undefined = -1;
int infinity = -1;

 //Inicializa todas las distancias a 'source', dist, como "infinito", y
 //todos los índices al anterior de la cadena óptima como "undefined"
 for(int i=0; i<G->Count; i++)
 {
 dist[i] = infinity;
 }

 TList* Q = new TList;  //Q: queue que guarda transitoriamente los nodos candidatos a pertenecer al camino óptimo

 //Copia G en Q:
 dynPunto* dynpaux;
 
 for(int i=0; i<G->Count; i++)
 {
 dynpaux = (dynPunto*)G->Items[i];
 Q->Add(dynpaux);
 }

 dist[source] = 0;

 int mindist;
 int u; //u: índice al nodo del grafo más cercano a 'source'. En la primera iteración será el mismo source
 bool encontrado; //booleano que indica que se encontró un camino por donde seguir, y el nodo no está rodeado de un costo infinito

 float alt; //auxiliar de distancias acumuladas
 float *cost; //auxiliar para el costo de una rama;

 dynPunto* pu; //puntero al nodo del índice u
 dynPunto* pv; //puntero a vecino de u

 bool Qlleno = true; //Indica si ya se recorrió todo la cola Q

 while(Qlleno)
 {
   mindist = 100000;
   encontrado = false;

   //Buscar el nodo de G con mínima distancia a 'source'
   for(int i=0; i<G->Count; i++)
   {
      if( dist[i]!=infinity && dist[i]<mindist && Q->Items[i]!= NULL) //Q != NULL implica que el nodo no fue revisado antes
      {
      mindist = dist[i];
      u = i;
      encontrado = true; //Se encontró un camino por donde seguir
      }
   }

   if(!encontrado) //si no se encontró un camino por donde seguir, todos los próximos nodos son inaccesibles
   break; //rompe el while..., sale de la función

   pu = (dynPunto*)Q->Items[u];

   //Para cada vecino de u...
   for(int i=0; i<pu->Vecinos->Count; i++)
   {
   cost = (float*)pu->Costos->Items[i];//Costo entre u y v
   alt = dist[u] + cost[0];
   pv = (dynPunto*)pu->Vecinos->Items[i];

     if(alt<dist[ pv->index ] || dist[ pv->index ]==infinity )
     {
     dist[ pv->index ] = alt;
     pv->previous = pu;
     //decrease-key v in Q...? ¿Qué significa?
     }
   }

   Q->Items[u] = NULL; //hace apuntar a null para no correr los índices.

   Qlleno = false; //Fija la condición para terminar el while, pero...

   for(int i=0; i<Q->Count; i++)
   {
     if(Q->Items[i] != NULL) //...si existe algún elemento distinto de null en Q
     {                    //vuelve a la condición para continuar el while.
     Qlleno = true;
     break;
     }
   }
 }

 //El resultado del algoritmo es el array dist. En el se puede ver cual es el nodo
 //que está a la mínima distancia de 'source', y luego seguir el camino inverso a través de
 //los punteros 'previous'

 for(int i=0; i<Q->Count; i++)
 Q->Items[i] = NULL;

 delete Q;

 return dist;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::InvertirEstudio1Click(TObject *Sender)
{
if(!ImagenEnPantalla)
return;

//Plano auxiliar, creación dinámica:
short int** Plaux;
Plaux = new short int*[altoPX];
 for (int i=0; i<altoPX; ++i)
  Plaux[i]=new short int [anchoPX];

  for(int k=0; k<cantImagenes/2; k++)
  {
    for(int i=0; i<altoPX; i++)
     for(int j=0; j<anchoPX; j++)
     {
     Plaux[i][j] = M->GetV(i,j,k);
     M->SetV(i,j,k, M->GetV(i,j,(cantImagenes-1)-k));
     M->SetV(i,j,(cantImagenes-1)-k, Plaux[i][j]);
     }
  }


//Plano auxiliar, destrucción:
 for (int i=0; i<altoPX; ++i)
        delete[] Plaux[i];
 delete[] Plaux;



}
//---------------------------------------------------------------------------

void __fastcall TForm1::CACWindowButtonClick(TObject *Sender)
{
Center = 0;
CenterEdit->Text = Center;

Width = 200;
WidthEdit->Text = Width;


}
//---------------------------------------------------------------------------


void TForm1::CentroGravedad(TList* ListaDePuntos, float& cgx, float& cgy, float& cgz)
{

    float N = ListaDePuntos->Count;

    Punto* paux;

    float acumx=0, acumy=0, acumz=0;


    for(int j=0; j<ListaDePuntos->Count; j++)
       {
       paux = (Punto*)ListaDePuntos->Items[j];
       acumx += paux->x;
       acumy += paux->y;
       acumz += paux->z;
       }

    cgx = acumx/N;
    cgy = acumy/N;
    cgz = acumz/N;
}

//---------------------------------------------------------------------------

float  TForm1::CalculoEnergia(TList* ListaDePuntos, float* weightsX, float* weightsY, float* weightsZ)
{
    Punto* paux;

    float energia = 0;

    for(int j=0; j<ListaDePuntos->Count; j++)
       {
       paux = (Punto*)ListaDePuntos->Items[j];
       energia += weightsZ[paux->z];
       energia += weightsX[paux->x];
       energia += weightsY[paux->y];
       }

       return energia;

}
//---------------------------------------------------------------------------
float TForm1::Perimetro(TList* ListaDePuntos, Volumen* MAux)
{
    Punto* paux;

    float perim = 0;

    bool esContorno = false;

    maxX = MAux->getAlto();
    maxY = MAux->getAncho();
    maxZ = MAux->getProf()*0.6;


    for(int j=0; j<ListaDePuntos->Count; j++)
       {
       paux = (Punto*)ListaDePuntos->Items[j];

       for(int x=-1; x<=1; x++)
       {
        if(esContorno)
        break;

        for(int y=-1; y<=1; y++)
          if(MAux->GetV(paux->x+x, paux->y+y, paux->z)==0 || paux->x+x<0 || paux->x+x>=maxX || paux->y+y<0 ||  paux->y+y>=maxY || paux->z<0 || paux->z>=maxZ)
          {
          esContorno = true;
          break;
          }
       }

          if(esContorno)
          {
        //  M->SetV(paux->x, paux->y, paux->z, 0);
          perim++;
          }


          esContorno = false;
       }

       return perim;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N2D1Click(TObject *Sender)
{
//Implementación del filtro de Canny:
       //Referencia: Jorge Valverde Rebaza, "Detección de bordes mediante el
       //algoritmo de Canny".

        if(!ImagenEnPantalla)
        return;

        bool automatico;

       //Contenedor auxiliar para el filtrado corte a corte:
       short int** auxG; //gradientes auxiliares:

       auxG = new short int*[M->getAlto()];
         for(int i=0; i<M->getAlto(); i++)
          auxG[i] = new short int[M->getAncho()];

       //Contenedor auxiliar para el filtrado corte a corte:
       short int** auxA; //ángulos auxiliares:

       auxA = new short int*[M->getAlto()];
         for(int i=0; i<M->getAlto(); i++)
          auxA[i] = new short int[M->getAncho()];

       //Contenedor auxiliar para el filtrado corte a corte:
       short int** auxI; //gradientes auxiliares:

       auxI = new short int*[M->getAlto()];
         for(int i=0; i<M->getAlto(); i++)
          auxI[i] = new short int[M->getAncho()];


       double angulo = 0;
       double gradX = 0;
       double gradY = 0;
       double magnitud = 0;

       short int t0,t1; //Gradientes de histéresis:
       int marcados = -1;

       double pM, pm; //percentiles máximo y mínimo de gradiente aceptados:

       if(CannyFilterForm->ShowModal()==mrOk)
       {
           automatico = CannyFilterForm->automatico;

           if(automatico)
           {
           pM = CannyFilterForm->pM;
           pM /=100.0;

           pm = CannyFilterForm->pm;
           pm /=100.0;
           }

           if(!automatico)
           {
           t0 = CannyFilterForm->vm;
           t1 = CannyFilterForm->vM;
           }

       }
       else return;


      if(automatico)
      {
       //Auxiliares para el cálculo de los gradientes:
       int* gradHisto = new int[maxPX-minPX+1];
       double* gradAcum = new double[maxPX-minPX+1];

       for(int s=0; s<maxPX-minPX+1; s++)
       {
       gradHisto[s]=0;
       gradAcum[s]=0;
       }

       //Creación del histograma de gradientes y el acumulado:
       for(int k=0; k<M->getProf(); k++)
          for(int i=1; i<M->getAlto()-1; i++)
            for(int j=1; j<M->getAncho()-1; j++)
            {
               gradX = M->GetV(i+1,j-1,k) +
                        M->GetV(i+1,j,k)   +
                        M->GetV(i+1,j+1,k) -
                        M->GetV(i-1,j-1,k) -
                        M->GetV(i-1,j,k)   -
                        M->GetV(i-1,j+1,k);
                gradX /=3.0;

                gradY = M->GetV(i-1,j+1,k) +
                        M->GetV(i,j+1,k)   +
                        M->GetV(i+1,j+1,k) -
                        M->GetV(i-1,j-1,k) -
                        M->GetV(i,j-1,k)   -
                        M->GetV(i+1,j-1,k);
                gradY /=3.0;

               gradHisto[abs(sqrt(gradX*gradX + gradY*gradY))]++;
            }


          gradAcum[0]=gradHisto[0];
          double maxacum;

          for(int s=1; s<maxPX-minPX+1; s++)
            gradAcum[s] = gradHisto[s]+gradAcum[s-1];

           maxacum = gradAcum[maxPX-minPX];

          for(int s=0; s<maxPX-minPX+1; s++)
            gradAcum[s] /= maxacum;

         for(int s=0; s<maxPX-minPX+1; s++)
            if(gradAcum[s]>pm)
             {
              t0 = s;
              break;
             }

         for(int s=t0; s<maxPX-minPX+1; s++)
            if(gradAcum[s]>pM)
             {
              t1 = s;
              break;
             }

      }// end if(automatico)...

       //Pasos: 1º Filtro Gaussiano? -> Reemplazar por filtro anisotrópico
       //       2º Cálculo de gradientes
       //       3º Supresión no máxima
       //       4º Histéresis de umbrales


       ProgressBar1->Max = M->getProf();

       for(int k=zCarina+1; k<zCarina+2; k++)
       {
       ProgressBar1->Position++;

           //Corte a corte: Gradientes calculados mediante máscara Prewitt:
           //mX = [-1 0 1]x3
           //mY = [-1;0;1]x3
           for(int i=1; i<M->getAlto()-1; i++)
            for(int j=1; j<M->getAncho()-1; j++)
            {
                //Cáculo de gradientes locales y ángulos:
                 gradX = M->GetV(i+1,j-1,k) +
                         M->GetV(i+1,j,k)   +
                         M->GetV(i+1,j+1,k) -
                         M->GetV(i-1,j-1,k) -
                         M->GetV(i-1,j,k)   -
                         M->GetV(i-1,j+1,k);
                 gradX /= 3.0;

                 gradY = M->GetV(i-1,j+1,k) +
                         M->GetV(i,j+1,k)   +
                         M->GetV(i+1,j+1,k) -
                         M->GetV(i-1,j-1,k) -
                         M->GetV(i,j-1,k)   -
                         M->GetV(i+1,j-1,k);
                 gradY /=3.0;

                 magnitud = sqrt(gradX*gradX + gradY*gradY);

                 auxG[i][j] = magnitud;

                 //Convención:
                 //Todos los ángulos deben quedar contenidos en el intervalo 0º-135º
                 if(gradX == 0 && gradY==0) //Gradiente nulo:
                 angulo = 0.0;

                 if(gradX > 0 && gradY==0)  //Gradiente en el 1er cuadrante: 0 a 90º
                 angulo = 0.0;

                 if(gradX >0 && gradY>0)
                 angulo = atan(gradY/gradX)*180.0/3.1416;

                 if(gradX == 0 && gradY>0)
                 angulo = 90.0;

                 if(gradX < 0 && gradY>0)    //Gradiente en el 2º cuadrante: 90º a 180º
                 angulo = 180.0 + atan(gradY/gradX)*180.0/3.1416;

                 if(gradX<0 && gradY==0)
                 angulo = 180.0;

                 if(gradX<0 && gradY<0)     //Gradiente en el 3er cuadrante: 180º a 270º
                 angulo = atan(gradY/gradX)*180.0/3.1416; //=> se refleja al 1er cuadrante

                 if(gradX==0 && gradY<0)
                 angulo = 90.0;

                 if(gradX>0 && gradY<0)     //Gradiente en el 4to cuadrante: 270º a 360º
                 angulo = 180.0 + atan(gradY/gradX)*180.0/3.1416; //Se refleja al 2º cuadrante

                 //Asignar el ángulo en las 4 direcciones principales: 0, 45, 90, 135;
                 if(angulo>0.0 && angulo <22.5)
                 angulo =0.0;

                 if(angulo>22.5 && angulo <67.5)
                 angulo =45.0;

                 if(angulo>67.5 && angulo <112.5)
                 angulo =90.0;

                 if(angulo>112.5 && angulo <157.5)
                 angulo =135.0;

                 if(angulo>157.5 && angulo <180.0)
                 angulo=0.0;

                 auxA[i][j] = angulo;
            }

          //2º paso: supresión no-máxima:
          for(int i=1; i<M->getAlto()-1; i++)
            for(int j=1; j<M->getAncho()-1; j++)
            {

               switch ( auxA[i][j] )
               {

               case 0 :
                 if(auxG[i][j]<auxG[i+1][j] || auxG[i][j]<auxG[i-1][j])
                 auxI[i][j]=0;
                 else auxI[i][j] = auxG[i][j];
               break;

               case 45 :
                 if(auxG[i][j]<auxG[i+1][j+1] || auxG[i][j]<auxG[i-1][j-1])
                 auxI[i][j]=0;
                 else auxI[i][j] = auxG[i][j];
               break;

               case 90 :
                 if(auxG[i][j]<auxG[i][j+1] || auxG[i][j]<auxG[i][j-1])
                 auxI[i][j]=0;
                 else auxI[i][j] = auxG[i][j];
               break;

               case 135 :
                 if(auxG[i][j]<auxG[i+1][j-1] || auxG[i][j]<auxG[i-1][j+1])
                 auxI[i][j]=0;
                 else auxI[i][j] = auxG[i][j];
               break;
               }
            }

           //Paso 3: Histéresis de umbral (entre t0 y t1):

           //Primera búsqueda de gradientes que superan el umbral mayor, t1:
           for(int i=1; i<M->getAlto()-1; i++)
            for(int j=1; j<M->getAncho()-1; j++)
            {
            if(auxI[i][j]>t1)//Si supera el umbral => marcado (-1)
            auxI[i][j] = -1; //-1: marcado
            }

          marcados = -1;

          while(marcados != 0)
          {
           marcados = 0;

           //Búsqueda iterativa de bordes secundarios por histéresis.
           //Condición: ser 8 vecino de un pixel marcado (marcado como -1).
           for(int i=1; i<M->getAlto()-1; i++)
            for(int j=1; j<M->getAncho()-1; j++)
            {
             //Si está entre t0 y t1:
             //Recorro la 8-vecindad: si algún vecino está marcado, marcarlo también:
             if(auxI[i][j]>=t0 && auxI[i][j]<t1)
             {
              for(int x=-1; x<2; x++)    //Recorro su 8 vecindad:
               for(int y=-1; y<2; y++)
               {
                if((abs(i)+abs(j))!=0 && auxI[i+x][j+y] == -1)
                {
                auxI[i][j] = -1; //marcado (-1)
                marcados++;
                }//end if
               }//end for y
              }//end if
             }//end for j
          }//end while

         for(int i=1; i<M->getAlto()-1; i++)
           for(int j=1; j<M->getAncho()-1; j++)
            {
            if(auxI[i][j]==-1)//Si supera el umbral => marcado (-1)
            M->SetV(i,j,k, maxPX);
            else M->SetV(i,j,k, minPX);
            }

       }//end for k...

       ProgressBar1->Position = 0;

       //delete de los contenedores auxiliares para el filtrado corte a corte:
         for(int i=0; i<M->getAlto(); i++)
          delete[] auxG[i];

          delete[] auxG;

         for(int i=0; i<M->getAlto(); i++)
          delete[] auxI[i];

          delete[] auxI;

         for(int i=0; i<M->getAlto(); i++)
          delete[] auxA[i];

          delete[] auxA;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CACD1Click(TObject *Sender)
{
        //Prueba para pintar circunferencias de ajuste:
        float x0A = xCA;
        float y0A = yCA;
        float z0A = zCarina+1;
        float rCA = rA;

        //Prueba para pintar circunferencias de ajuste:
        float x0D = xCD;
        float y0D = yCD;
        float z0D = zCarina+1;
        float rCD = rD;

        //Ajuste Ascendente:
        TList* CircuList = new TList;
        strelCirc2D(rA,x0A,y0A,z0A,CircuList);
        PuntoF* pa;
        float cont = 0;
        double cont2=0;

        for(int i=0; i<CircuList->Count; i++)
        {
                pa = (PuntoF*)CircuList->Items[i];
                cont += M->GetV(pa->x, pa->y, pa->z);
                cont2 +=  M->GetV(pa->x, pa->y, pa->z)*M->GetV(pa->x, pa->y, pa->z);
                delete pa;
        }

        float Nn = float(CircuList->Count);
        float mu = cont/Nn;
        float sigma = sqrt(cont2/Nn -mu*mu);

        //Parámetros de inicialización de condiciones iniciales del espacio SIMPLX:
        float usual_delta  = 0.01;
        float zero_delta = 0.0025;

        //Cálculo de las Condiciones Iniciales:
        //p0: Vector de condiciones iniciales: p0 = {x0, y0, theta0, rA, rB}
        float p0[3];

        //Valores inciales del Simplex:
        p0[0]=x0A;
        p0[1]=y0A;
        p0[2]= rCA/2.0;

        //p: Vector de vértices del subespacio SIMPLEX: p es de tamaño 6x5:
        //4=número de vértices, 3=cant de coeficientes a evaluar por vértice (x0,y0,r).
        float **p;

        p=new float*[4];

        for(int i=0;i<4;i++)
        {
        p[i]=new float[3];
                for(int j=0; j<3; j++)
                p[i][j]=p0[j];
        }

        p[0][0] = x0A;
        p[0][1] = y0A;
        p[0][2] = rCA/2.0;


        p[1][0] = x0A+5.0;
        p[1][1] = y0A;
        p[1][2] = rCA/2.0;

        p[2][0] = x0A;
        p[2][1] = y0A-5.0;
        p[2][2] = rCA/2.0;

        p[3][0] = x0A;
        p[3][1] = y0A;
        p[3][2] = rCA/2.0+5.0;

        int nfunc;

        //inicializa el subespacio SIMPLEX con vértices 'y':
        float y[4];

        //Circunferencia irrestricta, no obedece una ROI:
        y[0]=funkCircI(p[0], mu,sigma, z0A, M, x0A, y0A);
        y[1]=funkCircI(p[1], mu,sigma, z0A, M, x0A, y0A);
        y[2]=funkCircI(p[2], mu,sigma, z0A, M, x0A, y0A);
        y[3]=funkCircI(p[3], mu,sigma, z0A, M, x0A, y0A);

        amoebaCircI(p, y, 3, 1e-10,&nfunc,mu,sigma,z0A, M, x0A, y0A);
        CircuList->Clear();
        strelCirc2DContorno(p[0][2],p[0][0],p[0][1],z0A,CircuList);
        for(int i=0; i<CircuList->Count; i++)
        {
                pa = (PuntoF*)CircuList->Items[i];
                //M->SetV(pa->x, pa->y, pa->z, maxPX);
                delete pa;
        }
        CircuList->Clear();

        //Ajuste Descendente:
        strelCirc2D(rD,x0D,y0D,z0D,CircuList);

        cont = 0;
        cont2=0;

        for(int i=0; i<CircuList->Count; i++)
        {
                pa = (PuntoF*)CircuList->Items[i];
                cont += M->GetV(pa->x, pa->y, pa->z);
                cont2 +=  M->GetV(pa->x, pa->y, pa->z)*M->GetV(pa->x, pa->y, pa->z);
                delete pa;
        }

        Nn = float(CircuList->Count);
        mu = cont/Nn;
        sigma = sqrt(cont2/Nn -mu*mu);

        //Valores inciales del Simplex:
        p0[0]=x0D;
        p0[1]=y0D;
        p0[2]= rCD/2.0;


        for(int i=0;i<4;i++)
                for(int j=0; j<3; j++)
                        p[i][j]=p0[j];

        p[0][0] = x0D;
        p[0][1] = y0D;
        p[0][2] = rCD/2.0;

        p[1][0] = x0D+5.0;
        p[1][1] = y0D;
        p[1][2] = rCD/2.0;

        p[2][0] = x0D;
        p[2][1] = y0D-5.0;
        p[2][2] = rCD/2.0;

        p[3][0] = x0D;
        p[3][1] = y0D;
        p[3][2] = rCD/2.0+5.0;

        int nfunc2;


        y[0]=funkCircI(p[0], mu,sigma, z0D, M, x0D, y0D);
        y[1]=funkCircI(p[1], mu,sigma, z0D, M, x0D, y0D);
        y[2]=funkCircI(p[2], mu,sigma, z0D, M, x0D, y0D);
        y[3]=funkCircI(p[3], mu,sigma, z0D, M, x0D, y0D);

        amoebaCircI(p, y, 3, 1e-10,&nfunc2,mu,sigma,z0D, M, x0D, y0D);

        CircuList->Clear();
        strelCirc2DContorno(p[0][2],p[0][0],p[0][1],z0D,CircuList);
        for(int i=0; i<CircuList->Count; i++)
        {
                pa = (PuntoF*)CircuList->Items[i];
                //M->SetV(pa->x, pa->y, pa->z, maxPX);
                delete pa;
        }
        CircuList->Clear();
        delete CircuList;

        GraficarEnPantalla();



}
//---------------------------------------------------------------------------

void __fastcall TForm1::CirculoManual1Click(TObject *Sender)
{
       //Prueba para pintar circunferencias de ajuste:
        float x0 = xP;
        float y0 = yP;
        float z0 = float(ImageScrollBar->Position);
        float r = r0;

        //Limites de una region de interes:
        float xL0, yL0, xLf, yLf;
        xL0 = x0-50;
        xLf = x0+50;
        yL0 = y0-50;
        yLf = y0+50;

        //Ajuste Ascendente:
        TList* CircuList = new TList;
        strelCirc2D(r,x0,y0,z0,CircuList);
        PuntoF* pa;
        float cont = 0;
        double cont2=0;

        for(int i=0; i<CircuList->Count; i++)
        {
                pa = (PuntoF*)CircuList->Items[i];
                cont += M->GetV(pa->x, pa->y, pa->z);
                cont2 +=  M->GetV(pa->x, pa->y, pa->z)*M->GetV(pa->x, pa->y, pa->z);
                delete pa;
        }

        float Nn = float(CircuList->Count);
        float mu = cont/Nn;
        float sigma = sqrt(cont2/Nn -mu*mu);

        //pequeña disminución de radio para el simplex:
        r = r0*3.0/4.0;

        //Cálculo de las Condiciones Iniciales:
        //p0: Vector de condiciones iniciales: p0 = {x0, y0, theta0, rA, rB}
        float p0[3];

        //Valores inciales del Simplex:
        p0[0]=x0;
        p0[1]=y0;
        p0[2]= r;

        //p: Vector de vértices del subespacio SIMPLEX: p es de tamaño 6x5:
        //4=número de vértices, 3=cant de coeficientes a evaluar por vértice (x0,y0,r).
        float **p;

        p=new float*[4];

        for(int i=0;i<4;i++)
        {
        p[i]=new float[3];
                for(int j=0; j<3; j++)
                p[i][j]=p0[j];
        }

        p[0][0] = x0;
        p[0][1] = y0;
        p[0][2] = r;


        p[1][0] = x0+5.0;
        p[1][1] = y0;
        p[1][2] = r;

        p[2][0] = x0;
        p[2][1] = y0-5.0;
        p[2][2] = r;

        p[3][0] = x0;
        p[3][1] = y0;
        p[3][2] = r+5.0;

        int nfunc;

        //inicializa el subespacio SIMPLEX con vértices 'y':
        float y[4];

        y[0]=funkCirc(p[0], mu,sigma, z0, M, x0, y0, xL0,xLf,yL0,yLf);
        y[1]=funkCirc(p[1], mu,sigma, z0, M, x0, y0, xL0,xLf,yL0,yLf);
        y[2]=funkCirc(p[2], mu,sigma, z0, M, x0, y0, xL0,xLf,yL0,yLf);
        y[3]=funkCirc(p[3], mu,sigma, z0, M, x0, y0, xL0,xLf,yL0,yLf);

        amoebaCirc(p, y, 3, 1e-10,&nfunc,mu,sigma,z0, M, x0, y0, xL0,xLf,yL0,yLf);
        //CircuList->Clear();

        /*
        strelCirc2D(p[0][2],p[0][0],p[0][1],z0,CircuList);

        cont = 0;
        cont2 = 0;

        for(int i=0; i<CircuList->Count; i++)
        {
                pa = (PuntoF*)CircuList->Items[i];
                cont += M->GetV(pa->x, pa->y, pa->z);
                cont2 +=  M->GetV(pa->x, pa->y, pa->z)*M->GetV(pa->x, pa->y, pa->z);
              //  M->SetV(pa->x, pa->y, pa->z, maxPX);
                delete pa;
        }

        Nn = CircuList->Count;
        mu = cont/Nn;
        sigma = sqrt(cont2/Nn -mu*mu);

         //Valores inciales del Simplex:
        p0[0]=p[0][0];
        p0[1]=p[0][1];
        p0[2]=p[0][2];

        p[0][0] = p0[0];
        p[0][1] = p0[1];
        p[0][2] = p0[2];

        p[1][0] = p0[0]+5.0;
        p[1][1] = p0[1];
        p[1][2] = p0[2];

        p[2][0] = p0[0];
        p[2][1] = p0[1]-5.0;
        p[2][2] = p0[2];

        p[3][0] = p0[0];
        p[3][1] = p0[1];
        p[3][2] = p0[2]+5.0;

        y[0]=funkCirc(p[0], mu,sigma, z0, M, x0, y0);
        y[1]=funkCirc(p[1], mu,sigma, z0, M, x0, y0);
        y[2]=funkCirc(p[2], mu,sigma, z0, M, x0, y0);
        y[3]=funkCirc(p[3], mu,sigma, z0, M, x0, y0);

        int nfunc2;

        amoebaCirc(p, y, 3, 1e-15,&nfunc2,mu,sigma,z0, M, x0, y0);


        xP = p[0][0];
        yP = p[0][1];
        r0 = p[0][2];
        */

        /*
        //******************************
        //Cálculo de las Condiciones Iniciales:
        //p0: Vector de condiciones iniciales: p0 = {x0, y0, theta0, rA, rB}
        float p0E[5];

        //Valores inciales del Simplex:

        float theta = M_PI/4.0;

        p0E[0]=xP;
        p0E[1]=yP;
        p0E[2]= theta;
        p0E[3]= 3.0/4.0*r0;
        p0E[4]= 3.0/4.0*r0;

        //p: Vector de vértices del subespacio SIMPLEX: p es de tamaño 6x5:
        //4=número de vértices, 3=cant de coeficientes a evaluar por vértice (x0,y0,r).
        float **pE;

        pE=new float*[6];

        for(int i=0;i<6;i++)
        {
        pE[i]=new float[5];
                for(int j=0; j<5; j++)
                pE[i][j]=p0E[j];
        }

        pE[1][0] = xP+5.0;
        pE[1][1] = yP;
        pE[1][2] = theta;
        pE[1][3] = r0;
        pE[1][4] = r0;

        pE[2][0] = xP;
        pE[2][1] = yP-5.0;
        pE[2][2] = theta;
        pE[2][3] = r0;
        pE[2][4] = r0;

        pE[3][0] = xP;
        pE[3][1] = yP;
        pE[3][2] = 2*theta;
        pE[3][3] = r0;
        pE[3][4] = r0;

        pE[4][0] = xP;
        pE[4][1] = yP;
        pE[4][2] = theta;
        pE[4][3] = r0+3.0;
        pE[4][4] = r0;

        pE[5][0] = xP;
        pE[5][1] = yP;
        pE[5][2] = theta;
        pE[5][3] = r0;
        pE[5][4] = r0+3.0;

        int nfunc3;

        //inicializa el subespacio SIMPLEX con vértices 'y':
        float yE[6];

        yE[0]=funkElipse(pE[0], mu,sigma, z0, M);
        yE[1]=funkElipse(pE[1], mu,sigma, z0, M);
        yE[2]=funkElipse(pE[2], mu,sigma, z0, M);
        yE[3]=funkElipse(pE[3], mu,sigma, z0, M);
        yE[4]=funkElipse(pE[4], mu,sigma, z0, M);
        yE[5]=funkElipse(pE[5], mu,sigma, z0, M);

        amoebaElipse(pE, yE, 5, 1e-10,&nfunc3,mu,sigma,z0, M);

        //*****************************+

        CircuList->Clear();
        strelElipse2DContorno(pE[0][3], pE[0][4], pE[0][2], pE[0][0], pE[0][1], z0, CircuList);
        */

        CircuList->Clear();
        strelCirc2DContorno(p[0][2],p[0][0],p[0][1],z0,CircuList);

        for(int i=0; i<CircuList->Count; i++)
        {
                pa = (PuntoF*)CircuList->Items[i];
                M->SetV(pa->x, pa->y, pa->z, maxPX);
                delete pa;
        }

        xP = p[0][0];
        yP = p[0][1];
        r0 = p[0][2];

        delete CircuList;

        GraficarEnPantalla();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::ToroideScrollBarChange(TObject *Sender)
{
      vector1.x=0.0;
      vector1.y=0.0;
      vector1.z=1.0;

      //SetToroidalPlane():
      SetToroidalPlaneFromIndex(ToroideScrollBar->Position);
      //SetCenterLinePlaneFromIndex(ToroideScrollBar->Position);

      /*
      ArbitraryCutForm1->Show();
      for(int i=0; i<MPerp->getAncho(); i++)
       for(int j=0; j< MPerp->getAlto(); j++)
          CuadroCorteOblicuo->buffer(i,j)=LUT[MPerp->GetV(i,j,ToroideScrollBar->Position) -minPX];
          CuadroCorteOblicuo->Graficar(ArbitraryCutForm1->Image1);
     */
     //Interpolación TRILIEAL:
//http://en.wikipedia.org/wiki/Trilinear_interpolation
float x,y,z;
float xd, yd, zd;
float xp, xt, yp, yt, zp, zt;
float i1, i2, j1, j2, w1, w2;
short int valor;

     for (int i=anchoPX/5; i< 4*anchoPX/5; ++i)
        for (int j=altoPX/5; j<4*altoPX/5; ++j)
        {

         if(PlanoCorte[i][j].x<altoPX-1 && PlanoCorte[i][j].x>1 &&
           PlanoCorte[i][j].y<anchoPX-1 && PlanoCorte[i][j].y>1 &&
           PlanoCorte[i][j].z<cantImagenes-1 && PlanoCorte[i][j].z>1)
           {
           x = PlanoCorte[i][j].x;
           y = PlanoCorte[i][j].y;
           z = PlanoCorte[i][j].z;

           xp = floor(PlanoCorte[i][j].x);
           xt =  ceil(PlanoCorte[i][j].x);
           yp = floor(PlanoCorte[i][j].y);
           yt =  ceil(PlanoCorte[i][j].y);
           zp = floor(PlanoCorte[i][j].z);
           zt =  ceil(PlanoCorte[i][j].z);

           xd = x-xp;
           yd = y-yp;
           zd = z-zp;

           i1 = (M->GetV(xp,yp,zp)*(1-zd)+ M->GetV(xp,yp,zt)*zd);
           i2 = (M->GetV(xp,yt,zp)*(1-zd)+ M->GetV(xp,yt,zt)*zd);
           j1 = (M->GetV(xt,yp,zp)*(1-zd)+ M->GetV(xt,yp,zt)*zd);
           j2 = (M->GetV(xt,yt,zp)*(1-zd)+ M->GetV(xt,yt,zt)*zd);
           w1 = i1*(1-yd)+i2*yd;
           w2 = j1*(1-yd)+j2*yd;

           valor = (w1*(1-xd)+w2*xd);

           valor = M->GetV(x,y,z);

           CuadroCorteOblicuo->buffer(i,j)=LUT[valor-minPX];
           }
           else{CuadroCorteOblicuo->buffer(i,j)=LUT[minPX];
           }
         }


     CuadroCorteOblicuo->Graficar(ArbitraryCutForm1->Image1);

     float* aradio;
     float radio;
     aradio = (float*)AortaRadios->Items[ToroideScrollBar->Position];
     radio  = aradio[0]/res[0];

     ArbitraryCutForm1->SectionShape->Width  = 2*radio;
     ArbitraryCutForm1->SectionShape->Height = 2*radio;
     
     ArbitraryCutForm1->SectionShape->Left = anchoPX/2 - ArbitraryCutForm1->SectionShape->Width/2;
     ArbitraryCutForm1->SectionShape->Top  = altoPX/2 - ArbitraryCutForm1->SectionShape->Height/2;

     ArbitraryCutForm1->Repaint();

     //VistaOBButtonClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SetToroidalPlane()
{
     float norma;

     PuntoF* paux;

     //Recupera el vector tangente del toroide, que equivale a la normal del plano de visualización:
     paux = (PuntoF*)NormalToroide->Items[ ToroideScrollBar->Position ];

     vector2.x =  paux->x;
     vector2.y =  paux->y;
     vector2.z =  paux->z;

     //Lo normaliza:
     norma = sqrt(vector2.x*vector2.x+vector2.y*vector2.y+vector2.z*vector2.z);
     vector2.x=vector2.x/norma;
     vector2.y=vector2.y/norma;
     vector2.z=vector2.z/norma;

     //Centra y escala el plano original:
     for(int i=0; i<altoPX; i++)
         for(int j=0; j<anchoPX; j++)
         {
         PlanoCorte[i][j].x=res[0]*(PlanoOrig[i][j].x-PC1.x);
         PlanoCorte[i][j].y=res[1]*(PlanoOrig[i][j].y-PC1.y);
         PlanoCorte[i][j].z=res[2]*(PlanoOrig[i][j].z-PC1.z);
         }

    //Seteo de la matriz de Rotación:
    //1- calculo la matriz de rotación, con vector1 = (0,0,1):
    float alfa=acos((vector1.x*vector2.x+vector1.y*vector2.y+vector1.z*vector2.z));

    //2-Producto CRUZ para obtener el eje de rotación normalizado:
    vectorN.x=vector2.y*vector1.z-vector2.z*vector1.y;
    vectorN.y=vector2.z*vector1.x-vector2.x*vector1.z;
    vectorN.z=vector2.x*vector1.y-vector2.y*vector1.x;
    norma = sqrt(vectorN.x*vectorN.x + vectorN.y*vectorN.y + vectorN.z*vectorN.z);
    vectorN.x=vectorN.x/norma;
    vectorN.y=vectorN.y/norma;
    vectorN.z=vectorN.z/norma;

    //3- coficientes de la Matriz de Rotación:
    float c=cos(alfa);
    float s=sin(alfa);

    if(alfa>0.001)
    {
    R[0][0]=c+vectorN.x*vectorN.x*(1.0-c);
    R[0][1]=vectorN.x*vectorN.y*(1.0-c)-vectorN.z*s;
    R[0][2]=vectorN.x*vectorN.z*(1.0-c)+vectorN.y*s;

    R[1][0]=vectorN.y*vectorN.x*(1.0-c)+vectorN.z*s;
    R[1][1]=c+vectorN.y*vectorN.y*(1.0-c);
    R[1][2]=vectorN.y*vectorN.z*(1.0-c)-vectorN.x*s;

    R[2][0]=vectorN.z*vectorN.x*(1.0-c)-vectorN.y*s;
    R[2][1]=vectorN.z*vectorN.y*(1.0-c)+vectorN.x*s;
    R[2][2]=c+vectorN.z*vectorN.z*(1.0-c);
    }
    else
    {
    R[0][0]=1.0;
    R[0][1]=0.0;
    R[0][2]=0.0;

    R[1][0]=0.0;
    R[1][1]=1.0;
    R[1][2]=0.0;

    R[2][0]=0.0;
    R[2][1]=0.0;
    R[2][2]=1.0;
    }

    float x, y, z;
    //Roto los valores del plano de muestreo, ya centrados y escalados:
       for(int i=0; i<altoPX; i++)
         for(int j=0; j<anchoPX; j++)
         {

         x = PlanoCorte[i][j].x*R[0][0]+
             PlanoCorte[i][j].y*R[1][0]+
             PlanoCorte[i][j].z*R[2][0];

         y = PlanoCorte[i][j].x*R[0][1]+
             PlanoCorte[i][j].y*R[1][1]+
             PlanoCorte[i][j].z*R[2][1];

         z = PlanoCorte[i][j].x*R[0][2]+
             PlanoCorte[i][j].y*R[1][2]+
             PlanoCorte[i][j].z*R[2][2];

         PlanoCorte[i][j].x=x;
         PlanoCorte[i][j].y=y;
         PlanoCorte[i][j].z=z;
         }

       //Recupera el centro del Toroide:
       paux = (PuntoF*)CurvaToroide->Items[ ToroideScrollBar->Position ];

       //Vuelvo a las coordenadas de la imagen, y lo centro en el Toroide:
       for(int i=0; i<altoPX; i++)
         for(int j=0; j<anchoPX; j++)
         {
         PlanoCorte[i][j].x=(1.0/res[0])*(PlanoCorte[i][j].x + paux->x);
         PlanoCorte[i][j].y=(1.0/res[1])*(PlanoCorte[i][j].y + paux->y);
         PlanoCorte[i][j].z=(1.0/res[2])*(PlanoCorte[i][j].z + paux->z);
         }
}

//-----------------------------------------------------------------------------
void __fastcall TForm1::SetCenterLinePlaneFromIndex(int index, float alfa)
{

     float one_minus_alfa = 1.0 - alfa;

     double norma;

     PuntoF* paux;

     //Recupera el vector tangente del toroide, que equivale a la normal del plano de visualización:
     paux = (PuntoF*)AortaNormales->Items[ index ];

     vector2.x =  paux->x;
     vector2.y =  paux->y;
     vector2.z =  paux->z;

     //Lo normaliza:
     norma = sqrt(vector2.x*vector2.x+vector2.y*vector2.y+vector2.z*vector2.z);
     vector2.x=vector2.x/norma;
     vector2.y=vector2.y/norma;
     vector2.z=vector2.z/norma;

     //Centra y escala el plano original:
     for(int i=altoPX*alfa; i<altoPX*one_minus_alfa; i++)
         for(int j=anchoPX*alfa; j<anchoPX*one_minus_alfa; j++)
         {
         PlanoCorte[i][j].x=res[0]*(PlanoOrig[i][j].x-PC1.x);
         PlanoCorte[i][j].y=res[1]*(PlanoOrig[i][j].y-PC1.y);
         PlanoCorte[i][j].z=res[2]*(PlanoOrig[i][j].z-PC1.z);
         }

    //Seteo de la matriz de Rotación:
    //1- calculo la matriz de rotación, con vector1 = (0,0,1):
    float angulo=acos((vector1.x*vector2.x+vector1.y*vector2.y+vector1.z*vector2.z));

    //2-Producto CRUZ para obtener el eje de rotación normalizado:
    vectorN.x=vector2.y*vector1.z-vector2.z*vector1.y;
    vectorN.y=vector2.z*vector1.x-vector2.x*vector1.z;
    vectorN.z=vector2.x*vector1.y-vector2.y*vector1.x;
    norma = sqrt(vectorN.x*vectorN.x + vectorN.y*vectorN.y + vectorN.z*vectorN.z);
    vectorN.x=vectorN.x/norma;
    vectorN.y=vectorN.y/norma;
    vectorN.z=vectorN.z/norma;

    //3- coficientes de la Matriz de Rotación:
    float c=cos(angulo);
    float s=sin(angulo);

    if(angulo>0.001)
    {
    R[0][0]=c+vectorN.x*vectorN.x*(1.0-c);
    R[0][1]=vectorN.x*vectorN.y*(1.0-c)-vectorN.z*s;
    R[0][2]=vectorN.x*vectorN.z*(1.0-c)+vectorN.y*s;

    R[1][0]=vectorN.y*vectorN.x*(1.0-c)+vectorN.z*s;
    R[1][1]=c+vectorN.y*vectorN.y*(1.0-c);
    R[1][2]=vectorN.y*vectorN.z*(1.0-c)-vectorN.x*s;

    R[2][0]=vectorN.z*vectorN.x*(1.0-c)-vectorN.y*s;
    R[2][1]=vectorN.z*vectorN.y*(1.0-c)+vectorN.x*s;
    R[2][2]=c+vectorN.z*vectorN.z*(1.0-c);
    }
    else
    {
    R[0][0]=1.0;
    R[0][1]=0.0;
    R[0][2]=0.0;

    R[1][0]=0.0;
    R[1][1]=1.0;
    R[1][2]=0.0;

    R[2][0]=0.0;
    R[2][1]=0.0;
    R[2][2]=1.0;
    }

    float x, y, z;
    //Roto los valores del plano de muestreo, ya centrados y escalados:
       for(int i=altoPX*alfa; i<altoPX*one_minus_alfa; i++)
         for(int j=anchoPX*alfa; j<anchoPX*one_minus_alfa; j++)
         {

         x = PlanoCorte[i][j].x*R[0][0]+
             PlanoCorte[i][j].y*R[1][0]+
             PlanoCorte[i][j].z*R[2][0];

         y = PlanoCorte[i][j].x*R[0][1]+
             PlanoCorte[i][j].y*R[1][1]+
             PlanoCorte[i][j].z*R[2][1];

         z = PlanoCorte[i][j].x*R[0][2]+
             PlanoCorte[i][j].y*R[1][2]+
             PlanoCorte[i][j].z*R[2][2];

         PlanoCorte[i][j].x=x;
         PlanoCorte[i][j].y=y;
         PlanoCorte[i][j].z=z;
         }

       //Recupera el CenterLine:
       paux = (PuntoF*)AortaCenterLine->Items[ index ];

       //Vuelvo a las coordenadas de la imagen, y lo centro en el CenterLine:
       for(int i=altoPX*alfa; i<altoPX*one_minus_alfa; i++)
         for(int j=anchoPX*alfa; j<anchoPX*one_minus_alfa; j++)
         {
         PlanoCorte[i][j].x=(1.0/res[0])*(PlanoCorte[i][j].x + paux->x);
         PlanoCorte[i][j].y=(1.0/res[1])*(PlanoCorte[i][j].y + paux->y);
         PlanoCorte[i][j].z=(1.0/res[2])*(PlanoCorte[i][j].z + paux->z);
         }
}

//--------------------------------------------------------------------------

void __fastcall TForm1::SetToroidalPlaneFromIndex(int index)
{
     float norma;

     PuntoF* paux;

     //Recupera el vector tangente del toroide, que equivale a la normal del plano de visualización:
     paux = (PuntoF*)NormalToroide->Items[ index ];

     vector2.x =  paux->x;
     vector2.y =  paux->y;
     vector2.z =  paux->z;

     //Lo normaliza:
     norma = sqrt(vector2.x*vector2.x+vector2.y*vector2.y+vector2.z*vector2.z);
     vector2.x=vector2.x/norma;
     vector2.y=vector2.y/norma;
     vector2.z=vector2.z/norma;

     //Centra y escala el plano original:
     for(int i=anchoPX/5; i<4*altoPX/5; i++)
         for(int j=altoPX/5; j<4*altoPX/5; j++)
         {
         PlanoCorte[i][j].x=res[0]*(PlanoOrig[i][j].x-PC1.x);
         PlanoCorte[i][j].y=res[1]*(PlanoOrig[i][j].y-PC1.y);
         PlanoCorte[i][j].z=res[2]*(PlanoOrig[i][j].z-PC1.z);
         }

    //Seteo de la matriz de Rotación:
    //1- calculo la matriz de rotación, con vector1 = (0,0,1):
    float alfa=acos((vector1.x*vector2.x+vector1.y*vector2.y+vector1.z*vector2.z));

    //2-Producto CRUZ para obtener el eje de rotación normalizado:
    vectorN.x=vector2.y*vector1.z-vector2.z*vector1.y;
    vectorN.y=vector2.z*vector1.x-vector2.x*vector1.z;
    vectorN.z=vector2.x*vector1.y-vector2.y*vector1.x;
    norma = sqrt(vectorN.x*vectorN.x + vectorN.y*vectorN.y + vectorN.z*vectorN.z);
    vectorN.x=vectorN.x/norma;
    vectorN.y=vectorN.y/norma;
    vectorN.z=vectorN.z/norma;

    //3- coficientes de la Matriz de Rotación:
    float c=cos(alfa);
    float s=sin(alfa);

    if(alfa>0.001)
    {
    R[0][0]=c+vectorN.x*vectorN.x*(1.0-c);
    R[0][1]=vectorN.x*vectorN.y*(1.0-c)-vectorN.z*s;
    R[0][2]=vectorN.x*vectorN.z*(1.0-c)+vectorN.y*s;

    R[1][0]=vectorN.y*vectorN.x*(1.0-c)+vectorN.z*s;
    R[1][1]=c+vectorN.y*vectorN.y*(1.0-c);
    R[1][2]=vectorN.y*vectorN.z*(1.0-c)-vectorN.x*s;

    R[2][0]=vectorN.z*vectorN.x*(1.0-c)-vectorN.y*s;
    R[2][1]=vectorN.z*vectorN.y*(1.0-c)+vectorN.x*s;
    R[2][2]=c+vectorN.z*vectorN.z*(1.0-c);
    }
    else
    {
    R[0][0]=1.0;
    R[0][1]=0.0;
    R[0][2]=0.0;

    R[1][0]=0.0;
    R[1][1]=1.0;
    R[1][2]=0.0;

    R[2][0]=0.0;
    R[2][1]=0.0;
    R[2][2]=1.0;
    }

    float x, y, z;
    //Roto los valores del plano de muestreo, ya centrados y escalados:
       for(int i=altoPX/5; i<4*altoPX/5; i++)
         for(int j=anchoPX/5; j<4*anchoPX/5; j++)
         {

         x = PlanoCorte[i][j].x*R[0][0]+
             PlanoCorte[i][j].y*R[1][0]+
             PlanoCorte[i][j].z*R[2][0];

         y = PlanoCorte[i][j].x*R[0][1]+
             PlanoCorte[i][j].y*R[1][1]+
             PlanoCorte[i][j].z*R[2][1];

         z = PlanoCorte[i][j].x*R[0][2]+
             PlanoCorte[i][j].y*R[1][2]+
             PlanoCorte[i][j].z*R[2][2];

         PlanoCorte[i][j].x=x;
         PlanoCorte[i][j].y=y;
         PlanoCorte[i][j].z=z;
         }

       //Recupera el centro del Toroide:
       paux = (PuntoF*)CurvaToroide->Items[ index ];

       //Vuelvo a las coordenadas de la imagen, y lo centro en el Toroide:
       for(int i=anchoPX/5; i<4*altoPX/5; i++)
         for(int j=anchoPX/5; j<4*anchoPX/5; j++)
         {
         PlanoCorte[i][j].x=(1.0/res[0])*(PlanoCorte[i][j].x + paux->x);
         PlanoCorte[i][j].y=(1.0/res[1])*(PlanoCorte[i][j].y + paux->y);
         PlanoCorte[i][j].z=(1.0/res[2])*(PlanoCorte[i][j].z + paux->z);
         }
}


//---------------------------------------------------------------------------



void __fastcall TForm1::GuardarPlanosToroidales1Click(TObject *Sender)
{
      if(SaveDialog1->Execute())
      {
      FILE* fp;
      AnsiString FileName = SaveDialog1->FileName;

      if(!(FileName.AnsiPos(".bin") || FileName.AnsiPos(".BIN")))
      FileName=FileName+".bin";
      fp=fopen(FileName.c_str(),"w+b");

      encabezado enc1;
      float dx, dy, dz;
      float d;
      float resx, resy, resz;
      float L;
      float delta;

      //Calculo de las dimensiones:
      resx = res[0];//d/anchoPX;
      resy = res[1];
      resz = res[2];

      enc1.dX=resx;
      enc1.dY=resy;
      enc1.dZ=resz;
      enc1.ancho=anchoPX;
      enc1.alto=altoPX;
      enc1.min=minPX;
      enc1.max=maxPX;

      int cantSlices = CurvaToroide->Count-1;

      ProgressBar1->Max=cantSlices;

      ProgressBar1->Position = 0;

      int minC = 0; //Corte mínimo, por default = 0

      //size_t fread(void *ptr, size_t num_bytes, size_t count, FILE *stream);
      //fwrite(&numeroimgs,sizeof(short int),1,fp);
      fwrite(&cantSlices,sizeof(short int),1,fp);
      fwrite(&minC,sizeof(short int),1,fp);


      //Interpolación TRILIEAL:
      //http://en.wikipedia.org/wiki/Trilinear_interpolation
      float x,y,z;
      float xd, yd, zd;
      float xp, xt, yp, yt, zp, zt;
      float i1, i2, j1, j2, w1, w2;
      short int valor;

      for(int k=0; k<cantSlices-1; k++)
      {
       ProgressBar1->Position=k;
       //ToroideScrollBar->Position = k; //Automáticamente va a crear la vista toroidal

      vector1.x=0.0;
      vector1.y=0.0;
      vector1.z=1.0;

       SetToroidalPlaneFromIndex(k);

       enc1.frame=k;
       //enc1.frame = k+cantcortesaxiales-1;

       fwrite(&enc1,sizeof(encabezado),1,fp);


       for (int i=0; i<enc1.ancho; ++i)
           for (int j=0; j<enc1.alto; ++j)
             {

               if(PlanoCorte[i][j].x<altoPX-1 && PlanoCorte[i][j].x>1 &&
                  PlanoCorte[i][j].y<anchoPX-1 && PlanoCorte[i][j].y>1 &&
                  PlanoCorte[i][j].z<cantImagenes-1 && PlanoCorte[i][j].z>1)
                  {

                   x = PlanoCorte[i][j].x;
                   y = PlanoCorte[i][j].y;
                   z = PlanoCorte[i][j].z;

                   xp = floor(PlanoCorte[i][j].x);
                   xt =  ceil(PlanoCorte[i][j].x);
                   yp = floor(PlanoCorte[i][j].y);
                   yt =  ceil(PlanoCorte[i][j].y);
                   zp = floor(PlanoCorte[i][j].z);
                   zt =  ceil(PlanoCorte[i][j].z);

                   xd = x-xp;
                   yd = y-yp;
                   zd = z-zp;

                   i1 = (M->GetV(xp,yp,zp)*(1-zd)+ M->GetV(xp,yp,zt)*zd);
                   i2 = (M->GetV(xp,yt,zp)*(1-zd)+ M->GetV(xp,yt,zt)*zd);
                   j1 = (M->GetV(xt,yp,zp)*(1-zd)+ M->GetV(xt,yp,zt)*zd);
                   j2 = (M->GetV(xt,yt,zp)*(1-zd)+ M->GetV(xt,yt,zt)*zd);
                   w1 = i1*(1-yd)+i2*yd;
                   w2 = j1*(1-yd)+j2*yd;

                   valor = (w1*(1-xd)+w2*xd);

                  fwrite(&valor,sizeof(short int),1,fp);

                  if(valor>maxPX)
                  enc1.max = valor;

                  }
                  else{fwrite(&minPX,sizeof(short int),1,fp);}

             }

       }

        fclose( fp );
        ProgressBar1->Position=0;
   }



   AP->Items->Clear();


   PuntoF* paux;
   PuntoF* naux;

   for(int i=0; i<CurvaToroide->Count; i++)
   {
   paux = (PuntoF*)CurvaToroide->Items[i];
   naux = (PuntoF*)NormalToroide->Items[i];

   //Guarda el punto:
   AP->Items->Add(paux->x);
   AP->Items->Add(paux->y);
   AP->Items->Add(paux->z);

   //Guarda la normal:
   AP->Items->Add(naux->x);
   AP->Items->Add(naux->y);
   AP->Items->Add(naux->z);
   }

   AP->Items->SaveToFile("EsqueletoAxialToroidal.dat");


}
//---------------------------------------------------------------------------

void __fastcall TForm1::Slice1Click(TObject *Sender)
{
        for(int k=0; k<cantImagenes-1;k++)
                for(int i=0; i<anchoPX;i++)
                        for(int j=0; j<altoPX;j++)
                                if( M->GetV(i,j,k)<0 || M->GetV(i,j,k)>100)
                                  M->SetV(i,j,k,0);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::CirculoCavityFilling1Click(TObject *Sender)
{
        //Toma el punto incial en la posición del puntero del Mouse o alguna
        //semilla guardada en (xP,yP) previamente:

        float x0 = xP;
        float y0 = yP;
        float z0 = zP;

        float r = r0;  //r0 está originalmente seteado en 15

        float ax0,ay0,ar;
        ax0 = xP;
        ay0 = yP;
        ar = r0;

        //Seteo los limites de una region de interés alrededor del punto inicial:
        float xL0, yL0, xLf, yLf;

        //Región de interés "estática":
        int LadoROI = 45;

        xL0 = x0-LadoROI;
        xLf = x0+LadoROI;
        yL0 = y0-LadoROI;
        yLf = y0+LadoROI;

        if(xL0<0) xL0 = 0;
        if(xLf>anchoPX-1) xLf = anchoPX-1;
        if(yL0<0) yL0 = 0;
        if(yLf> altoPX-1) yLf = altoPX -1;

        //Activación de la ROI!:
        ROICheckBox->Checked = true;
        ROIShape->Left = yL0;
        ROIShape->Top = xL0;
        ROIShape->Width = yLf-yL0;
        ROIShape->Height= xLf-xL0;


        //Preprocesamiento!!:
        //1- Filtro de Mediana:  Probar también bilateral:
        FiltroMediana(M);
        
        //2- Apertura Morfológica:
        //AperturaClick(Sender);

        //3- Binarización?:


        //Obtención de media y desvío dentro de la región inicial:
        TList* CircuList = new TList;
        strelCirc2D(r,x0,y0,z0,CircuList);
        PuntoF* pa;
        float cont = 0;
        double cont2=0;

        for(int i=0; i<CircuList->Count; i++)
        {
                pa = (PuntoF*)CircuList->Items[i];
                cont += MToroide->GetV(pa->x, pa->y, pa->z);
                cont2 +=  MToroide->GetV(pa->x, pa->y, pa->z)*MToroide->GetV(pa->x, pa->y, pa->z);
                delete pa;
        }

        float Nn = float(CircuList->Count);
        float mu = cont/Nn;
        float sigma = sqrt(cont2/Nn -mu*mu);

        //Valores típicos de mu = 40
        //Valores típicos de sigma: 20 sin filtrar, 5 filtrado

        float n = nsigmas;
        float valorLimite = mu - n*sigma;


        for(int i=0; i<512; i++)
                for(int j=0; j<512; j++)
                        auxArray[i][j]=minPX;

        //Búsqueda de la cavidad en forma de 4-vecinos:
        int vN, vS, vE, vW;
        for(int i=xL0; i<xLf; i++)
          for(int j=yL0; j<yLf; j++)
          {

           vN = 0;
           vS = 0;
           vE = 0;
           vW = 0;

             if(M->GetV(i,j,z0) >= valorLimite)
             {
             if( M->GetV(i+1,j,z0)<valorLimite )
             vN = 1;
             if( M->GetV(i,j+1,z0)<valorLimite )
             vE = 1;
             if( M->GetV(i,j-1,z0)<valorLimite )
             vW = 1;
             if( M->GetV(i-1,j,z0)<valorLimite )
             vS = 1;
             }

           if(vN+vS+vE+vW > 1)
           auxArray[i][j]=maxPX;

          }

          //BoundaryList;

          TList* BL = new TList;
          Punto* paux;

          for(int i=xL0; i<xLf; i++)
                for(int j=yL0; j<yLf; j++)
                 if(auxArray[i][j]==maxPX)
                 {
                 M->SetV(i,j,z0,maxPX);

                 paux = new Punto;

                 paux->x = i;
                 paux->y = j;
                 paux->z = z0;

                 BL->Add(paux);
                 }


        float maxMinDist = 0;
        float minDist = 0;

        float aDist = 0;

        bool candidato = false;

        int posmaxX, posmaxY, rmax;


        for(int i=xL0; i<xLf; i++)
                for(int j=yL0; j<yLf; j++)
                 {

                 if(M->GetV(i,j,z0) >= valorLimite)
                 {
                 minDist = FLT_MAX;

                 //Búsqueda de la mínima distancia a los bordes:

                 if(BL->Count == 0)
                 {
                 //Le envía el mensaje de que no se detectaron bordes:
                 ImageScrollBar->Tag = 1;
                 //posmaxX = x0;
                 //posmaxY = y0;
                 //rmax = r0;
                 //break;
                 return;
                 }

                 for(int b = 0; b<BL->Count; b++)
                 {
                 paux = (Punto*)BL->Items[b];

                 aDist = sqrt( (i-paux->x)*(i-paux->x) + (j-paux->y)*(j-paux->y) );

                 if(aDist<minDist)
                 minDist = aDist;
                 }

                 if(minDist==FLT_MAX)
                 minDist = 0;

                 //Una vez encontrada la mínima distancia, si el círculo cae dentro de la
                 //ROI, es un candidato a ser el círculo CavityFilling:
                 candidato = false;
                 if( i-minDist>xL0 && i+minDist<xLf && j-minDist>yL0 && j+minDist<yLf )
                 candidato = true;

                 if(candidato)
                  if(minDist>maxMinDist)
                  {
                  maxMinDist = minDist;
                  posmaxX = i;
                  posmaxY = j;
                  rmax = minDist;
                  }

                 }
                 }

        CircuList->Clear();


        if(sqrt((posmaxX-ax0)*(posmaxX-ax0)+(posmaxY-ay0)*(posmaxY-ay0))>0.5*ar)
        {
        ImageScrollBar->Tag = 1;
        return;
        }

        strelCirc2DContorno(rmax,posmaxX,posmaxY,z0,CircuList);

       /* for(int i=0; i<CircuList->Count; i++)
        {
                pa = (PuntoF*)CircuList->Items[i];
                M->SetV(pa->x, pa->y, pa->z, maxPX);
                delete pa;
        }
        CircuList->Clear();
        delete CircuList; */

        xP = posmaxX;
        yP = posmaxY;
        r0 = rmax*0.75;


        //Cálculo de las coordenadas del punto encontrado, en función del plano
        //recosntruido al que pertenece:
        PuntoF* pE = new PuntoF;

        pE->x = PlanoCorte[posmaxX][posmaxY].x;
        pE->y = PlanoCorte[posmaxX][posmaxY].y;
        pE->z = PlanoCorte[posmaxX][posmaxY].z;

        //Escalar los puntos del centerline:
        pE->x *= res[0];
        pE->y *= res[1];
        pE->z *= res[2];

        float* aRadio; //actual Radio, radio del corte actual:
        aRadio  = new float[1];

        float valor = rmax*res[0];
        aRadio[0] = valor;

        //Aorta Centerline Escalado:
        AortaCenterLine->Add(pE);

        //Aorta radios escalados:
        AortaRadios->Add(aRadio);

        GraficarEnPantalla();

}
//---------------------------------------------------------------------------

void __fastcall TForm1::AortaCavityFilling1Click(TObject *Sender)
{
        AortaCavityFilling(MToroide);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::AortaCavityFilling(Volumen* MV)
{
        //Método similar a Axial Cavity Filling, pero considera la aorta completa
        //desde la semilla elegida hasta el final del bastón:

        int fail_counter = 0; //Contador de las veces seguidas que falló en el intento de ajustar un círculo.
        //Lo uso como indicador de que finalizó la búsqueda dentro de la aorta
        int fail_limit = 3;


        nsigmas = 3.0;
        float nsigmas0 = nsigmas;

        //AortaCenterLine = new TList;
        //AortaRadios = new TList;
        //AortaNormales = new TList;

        float ax0, ay0; //Puntos semilla
        //r0=rD/2;

        float ar;

        int pos0 = zP;

        //Cuidado!! Los puntos semillas del estudio original están corridos respecto
        //de los puntos semilla del corte toroidal. Los cortes toroidales son tales
        //que el centerline del bastón siempre estén en el centro de la imagen:
        vector1.x=0.0;
        vector1.y=0.0;
        vector1.z=1.0;

        //Recupera el bastón completo:
        for(int i=pos0; i<CurvaToroide->Count -pos0; i++)
        {
        ROIListBox->Tag = 0;
        SliceListBox->Tag = i;
        ROIShape->Tag = i; //El Tag de ROIShape se utiliza para indicar la posición del Corte en MToroide

        ax0 = xP;
        ay0 = yP;
        ar = r0;

        ImageScrollBar->Tag = 0;

        ArbitraryCutForm1->Show();

        //Marca el Centerline:
        ArbitraryCutForm1->CenterShape->Visible = true;

        //Setea el plano Toroidal en la posición donde se reconstruyó ese corte,
        //de manera que las coordenadas de la imagen coincidan con el plano
        //reconstruido:
        SetToroidalPlaneFromIndex(i);
        CirculoAxialCavityFilling(MV);

        if(ImageScrollBar->Tag == 0)
        fail_counter = 0;

        for(int ix=0; ix<MV->getAncho(); ix++)
        for(int jy=0; jy<MV->getAlto(); jy++)
        CuadroCorteOblicuo->buffer(ix,jy)=LUT[MV->GetV(ix,jy, i)-minPX];
        CuadroCorteOblicuo->Graficar(ArbitraryCutForm1->Image1);

         //Si no se detectaron bordes, o el círculo dio demasiado alejado,
         //se vuelve a intentar, disminuyendo nsigmas:
         if(ImageScrollBar->Tag == 1)
          {
          fail_counter++;

          ROIListBox->Tag = 1;
          ImageScrollBar->Tag = 0;
          nsigmas = 0.8*nsigmas;
          CirculoAxialCavityFilling(MV);


          for(int ix=0; ix<MV->getAncho(); ix++)
          for(int jy=0; jy<MV->getAlto(); jy++)
          CuadroCorteOblicuo->buffer(ix,jy)=LUT[MV->GetV(ix,jy, i)-minPX];
          CuadroCorteOblicuo->Graficar(ArbitraryCutForm1->Image1);
          }
          else
          {
          //Si corrigió "bien" luego del error, restaura el índice nsigmas y baja el fail_counter:
          nsigmas = nsigmas0;
          ROIListBox->Tag = 0;
          fail_counter--;
          }

        if(ImageScrollBar->Tag == 1 )
        {
        //Si vuelve a dar mal, restaura nsigmas, pasa al siguiente corte y aumenta fail_counter:
        fail_counter++;
        nsigmas = nsigmas0;
        ROIListBox->Tag = 0;
        }

        if(fail_counter == fail_limit)
        break;

       }

     //AortaCenterLine guarda ahora el nuevo centerline calculado a partir de las
     //reconstrucciones en la región axial. El último valor debe corresponder al
     //punto semilla!

     //AortaCenterLine guarda ahora el nuevo centerline calculado a partir de las
     //reconstrucciones en forma de "bastón" (axiales rectas + toroidales en el arco).

     //Para calcular la corrección del centerline (ej: por la aparición de regiones
     //elípticas), es necesario recalcular planos oblicuos tomando como vector normal
     //a los vectores normales a este nuevo centerline.

     //Debido a las variaciones abruptas del centerline (ej: ruido o detecciones
     //imperfectas) tomamos un CenterLine cúbico suavizado de 7 puntos (cuando es posible)
     //para el cálculo EXACTO de la nuevas normales, para una posterior repetición del ajuste
     // del Cavity Filling.

     //El polinomio resultante que ajusta en la vecindad del punto k,
     //tendrá la forma Pk = (Pkx, Pky, Pkz), cada uno con la forma:
     //Pki = C3i*t^3 + C2i*t^2 + C1i*t + C0i

     //(Fuente: 3-D Quantification of the Aortic Arch Morphology in 3-D CTA Data
     // for Endovascular Aortic Repair, IEEE TMI, Wörz, 2009)

     float r = 7.0; //rango de puntos a mirar;

     //Variación del parámetro t dentro de la curva de ajuste local:
     float tn[15];

     for(int n=0; n<2*r+1; n++)
     tn[n] = float(n)/r-1.0; //Así, el parámetro t varía entre -1 y 1 dentro del rango de puntos

     //Cálculo de los coeficientes Sum(X_n * t_n^j);
     float X3,X2,X1,X0; X3=0; X2=0; X1=0; X0=0;
     float Y3,Y2,Y1,Y0; Y3=0; Y2=0; Y1=0; Y0=0;
     float Z3,Z2,Z1,Z0; Z3=0; Z2=0; Z1=0; Z0=0;

     float X[4];
     float Y[4];
     float Z[4];

    //Utilizamos la matriz de coeficientes invertida mediante MATLAB:
    float invM[4][4] = {{2.0538,0.0,-1.3999,0.0},{0.0,0.5820,0.0,-0.2217},{-1.3999,0.0,1.1292,0.0},{0.0,-0.2217,0.0,0.15}};

    PuntoF* paux; //puntero auxiliar al CenterLine:
    PuntoF* naux; //puntero auxiliar a la normal al centerline calculada analíticamente
    double norma;

    //k: índice del punto cuya vecindad se va a aproximar:

    int indice;

    TList* AuxCenterLine = new TList;

    for(int k=0; k<AortaCenterLine->Count; k++)
    {
     X3=0; X2=0; X1=0; X0=0;
     Y3=0; Y2=0; Y1=0; Y0=0;
     Z3=0; Z2=0; Z1=0; Z0=0;

     for(int n=0; n<2*r+1; n++)
     {
     indice = k + (n-r);

     if(indice<0)
     indice=0;

     if(indice>=AortaCenterLine->Count)
     indice=AortaCenterLine->Count-1;

     paux = (PuntoF*)AortaCenterLine->Items[indice];

     //Escalar el centerLine:
     X3 += paux->x*tn[n]*tn[n]*tn[n]; //Sum(Xn*tn^3);
     X2 += paux->x*tn[n]*tn[n];       //Sum(Xn*tn^2);
     X1 += paux->x*tn[n];             //Sum(Xn*tn^1);
     X0 += paux->x;                   //Sum(Xn*tn^0);

     Y3 += paux->y*tn[n]*tn[n]*tn[n]; //Sum(Yn*tn^3);
     Y2 += paux->y*tn[n]*tn[n];       //Sum(Yn*tn^2);
     Y1 += paux->y*tn[n];             //Sum(Yn*tn^1);
     Y0 += paux->y;                   //Sum(Yn*tn^0);

     Z3 += paux->z*tn[n]*tn[n]*tn[n]; //Sum(Zn*tn^3);
     Z2 += paux->z*tn[n]*tn[n];       //Sum(Zn*tn^2);
     Z1 += paux->z*tn[n];             //Sum(Zn*tn^1);
     Z0 += paux->z;                   //Sum(Zn*tn^0);
     }

     X[3] = X0; X[2]=X1; X[1]=X2; X[0]=X3;
     Y[3] = Y0; Y[2]=Y1; Y[1]=Y2; Y[0]=Y3;
     Z[3] = Z0; Z[2]=Z1; Z[1]=Z2; Z[0]=Z3;

     float Cx[4]={0,0,0,0};
     float Cy[4]={0,0,0,0};
     float Cz[4]={0,0,0,0};

     for(int j=0; j<4; j++)
      for(int i=0; i<4; i++)
      {
       Cx[j] +=  X[i]*invM[j][i];
       Cy[j] +=  Y[i]*invM[j][i];
       Cz[j] +=  Z[i]*invM[j][i];
      }

    //A partir del ajuste polinómico, calculo las normales suavizadas y normalizadas:
    naux = new PuntoF;

    naux->x = Cx[2];
    naux->y = Cy[2];
    naux->z = Cz[2];

    norma = sqrt(naux->x*naux->x + naux->y*naux->y + naux->z*naux->z);

    naux->x /= norma;
    naux->y /= norma;
    naux->z /= norma;


    //Resultado: AortaCenterLine, AortaRadios y AortaNormales escalados
    AortaNormales->Add(naux);

    paux = new PuntoF;
    paux->x = Cx[3];
    paux->y = Cy[3];
    paux->z = Cz[3];

    AuxCenterLine->Add(paux);
   }

   PuntoF* paux2;

   for(int k=0; k<AortaCenterLine->Count; k++)
   {
   paux = (PuntoF*)AortaCenterLine->Items[k]; //Punto del centerline
   paux2 = (PuntoF*)AuxCenterLine->Items[k];  //punto del centerline suavizado

    if(!AP->Tag)
    {
    paux->x = (paux->x + paux2->x)/2.0;
    paux->y = (paux->y + paux2->y)/2.0;
    paux->z = (paux->z + paux2->z)/2.0;
    }
    delete paux2;
   }

   AuxCenterLine->Clear();
   delete AuxCenterLine;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::GuardarCarina1Click(TObject *Sender)
{
        //Prueba para pintar circunferencias de ajuste:
        //float x0A = xCA;
        //float y0A = yCA;
        //float z0A = zCarina+1;
        //float rCA = rA;

        //Prueba para pintar circunferencias de ajuste:
        //float x0D = xCD;
        //float y0D = yCD;
        //float z0D = zCarina+1;
        //float rCD = rD;

       AP->Items->Add(zCarina);
       AP->Items->Add(xCA);
       AP->Items->Add(yCA);
       AP->Items->Add(rA);

       AP->Items->Add(xCD);
       AP->Items->Add(yCD);
       AP->Items->Add(rD);

       AP->Items->SaveToFile("carina.dat");


}
//---------------------------------------------------------------------------

void __fastcall TForm1::AbrirCarina1Click(TObject *Sender)
{
       AP->Items->LoadFromFile("carina.dat");

       zCarina = atof(AP->Items->Strings[0].c_str());
       xCA = atof(AP->Items->Strings[1].c_str());
       yCA = atof(AP->Items->Strings[2].c_str());
       rA = atof(AP->Items->Strings[3].c_str());

       xCD = atof(AP->Items->Strings[4].c_str());
       yCD = atof(AP->Items->Strings[5].c_str());
       rD = atof(AP->Items->Strings[6].c_str());


}
//---------------------------------------------------------------------------

void __fastcall TForm1::GraficarCenterline1Click(TObject *Sender)
{
     PuntoF* paux;

     for(int i=0; i<AortaCenterLine->Count; i++)
     {
     paux = (PuntoF*)AortaCenterLine->Items[i];

     paux->x /= res[0];
     paux->y /= res[1];
     paux->z /= res[2];

     for(int di=-2; di<3; di++)
      for(int dj=-2; dj<3; dj++)
        M->SetV(paux->x+di, paux->y+dj, paux->z, maxPX);
     }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::GuardarCenterline1Click(TObject *Sender)
{
     AP->Items->Clear();

     PuntoF* paux;

     for(int i=0; i<AortaCenterLine->Count; i++)
     {
     paux = (PuntoF*)AortaCenterLine->Items[i];

     AP->Items->Add(paux->x);
     AP->Items->Add(paux->y);
     AP->Items->Add(paux->z);
     }

     AP->Items->SaveToFile("CenterLine.dat");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::GuardarPlanosOblicuos1Click(TObject *Sender)
{
if(SaveDialog1->Execute())
      {
      FILE* fp;
      AnsiString FileName = SaveDialog1->FileName;

      if(!(FileName.AnsiPos(".bin") || FileName.AnsiPos(".BIN")))
      FileName=FileName+".bin";
      fp=fopen(FileName.c_str(),"w+b");

      ProgressBar1->Position=0;

      encabezado enc1;
      float dx, dy, dz;
      float d;
      float resx, resy, resz;
      float L;
      float delta;

      //Calculo de las dimensiones:
      resx = res[0];//d/anchoPX;
      resy = res[1];
      resz = res[2];

      enc1.dX=resx;
      enc1.dY=resy;
      enc1.dZ=resz;
      enc1.ancho=anchoPX;
      enc1.alto=altoPX;
      enc1.min=minPX;
      enc1.max=maxPX;

      //int cantSlices = CurvaToroide->Count-1;
      int cantSlices = AortaCenterLine->Count-1;

      ProgressBar1->Max=cantSlices;
      ProgressBar1->Position = 0;

      int minC = 0; //Corte mínimo, por default = 0

      //size_t fread(void *ptr, size_t num_bytes, size_t count, FILE *stream);
      //fwrite(&numeroimgs,sizeof(short int),1,fp);
      fwrite(&cantSlices,sizeof(short int),1,fp);
      fwrite(&minC,sizeof(short int),1,fp);

      //Interpolación TRILIEAL:
      //http://en.wikipedia.org/wiki/Trilinear_interpolation
      float x,y,z;
      float xd, yd, zd;
      float xp, xt, yp, yt, zp, zt;
      float i1, i2, j1, j2, w1, w2;
      short int valor;

      vector1.x=0.0;
      vector1.y=0.0;
      vector1.z=1.0;

      for(int k=0; k<cantSlices-1; k++)
      {
      ProgressBar1->Position=k;

      SetCenterLinePlaneFromIndex(k, 0.33);

      enc1.frame=k;
      //enc1.frame = k+cantcortesaxiales-1;

      fwrite(&enc1,sizeof(encabezado),1,fp);


      for (int i=0; i<enc1.ancho; ++i)
           for (int j=0; j<enc1.alto; ++j)
             {

               if(PlanoCorte[i][j].x<altoPX-1 && PlanoCorte[i][j].x>1 &&
                  PlanoCorte[i][j].y<anchoPX-1 && PlanoCorte[i][j].y>1 &&
                  PlanoCorte[i][j].z<cantImagenes-1 && PlanoCorte[i][j].z>1)
                  {

                   x = PlanoCorte[i][j].x;
                   y = PlanoCorte[i][j].y;
                   z = PlanoCorte[i][j].z;

                   xp = floor(PlanoCorte[i][j].x);
                   xt =  ceil(PlanoCorte[i][j].x);
                   yp = floor(PlanoCorte[i][j].y);
                   yt =  ceil(PlanoCorte[i][j].y);
                   zp = floor(PlanoCorte[i][j].z);
                   zt =  ceil(PlanoCorte[i][j].z);

                   xd = x-xp;
                   yd = y-yp;
                   zd = z-zp;

                   i1 = (M->GetV(xp,yp,zp)*(1-zd)+ M->GetV(xp,yp,zt)*zd);
                   i2 = (M->GetV(xp,yt,zp)*(1-zd)+ M->GetV(xp,yt,zt)*zd);
                   j1 = (M->GetV(xt,yp,zp)*(1-zd)+ M->GetV(xt,yp,zt)*zd);
                   j2 = (M->GetV(xt,yt,zp)*(1-zd)+ M->GetV(xt,yt,zt)*zd);
                   w1 = i1*(1-yd)+i2*yd;
                   w2 = j1*(1-yd)+j2*yd;

                   valor = (w1*(1-xd)+w2*xd);

                  fwrite(&valor,sizeof(short int),1,fp);

                  if(valor>maxPX)
                  enc1.max = valor;

                  }
                  else{fwrite(&minPX,sizeof(short int),1,fp);}
             }

       }

        fclose( fp );
        ProgressBar1->Position=0;
   }


}
//---------------------------------------------------------------------------

void __fastcall TForm1::GuardarModelo1Click(TObject *Sender)
{
     AP->Items->Clear();

     PuntoF* paux;
     float* radio;

     for(int i=0; i<AortaCenterLine->Count; i++)
     {
     paux = (PuntoF*)AortaCenterLine->Items[i];

     AP->Items->Add(paux->x);
     AP->Items->Add(paux->y);
     AP->Items->Add(paux->z);

     paux = (PuntoF*)AortaNormales->Items[i];

     AP->Items->Add(paux->x);
     AP->Items->Add(paux->y);
     AP->Items->Add(paux->z);

     radio = (float*)AortaRadios->Items[i];

     AP->Items->Add(radio[0]);
     }

     AP->Items->SaveToFile("ModeloAorta.dat");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Aorta2Click(TObject *Sender)
{
//Segmentación de Carina:
//Carina1Click(Sender); //Resultado parcial: Posicion de carina (Zcarina) en Maux

//Detección Robusta de CA & CD:
//CACD1Click(Sender); //Resultado parcial: puntos CA, CD detectados, con sus respectivos radios inciiales.

//Cortes oblicuos: genera el centerline de la curva estimadora del centerline aórtico.
//Llamamos a esta curva "bastón", por su forma axial recta y su forma circular toroidal:

//Baston1Click(Sender); //Resultado parcial: "Bastón" guardado en las listas "CurvaToroide" con vectores normales
                      //                   calculados en forma exacta en la lista "NormalToroide"


//Creación de los planos Toroidales, ortogonales al Bastón, mediante Interpolación Trilineal:
int cantSlices = CurvaToroide->Count-1;
ProgressBar1->Max=cantSlices;

//Interpolación TRILIEAL:
//http://en.wikipedia.org/wiki/Trilinear_interpolation
float x,y,z;
float xd, yd, zd;
float xp, xt, yp, yt, zp, zt;
float i1, i2, j1, j2, w1, w2;
float valor;

//Creación de una matriz auxiliar, MToroide:
MToroide = new Volumen(M->getAncho(), M->getAlto(), cantSlices);
MToroide->LlenarTodo(minPX);

for(int k=0; k<cantSlices; k++)
{
ProgressBar1->Position=k;

vector1.x=0.0;
vector1.y=0.0;
vector1.z=1.0;

SetToroidalPlaneFromIndex(k);

    for (int i=anchoPX/5; i< 4*anchoPX/5; ++i)
        for (int j=altoPX/5; j<4*altoPX/5; ++j)
        {

         if(PlanoCorte[i][j].x<altoPX-1 && PlanoCorte[i][j].x>1 &&
           PlanoCorte[i][j].y<anchoPX-1 && PlanoCorte[i][j].y>1 &&
           PlanoCorte[i][j].z<cantImagenes-1 && PlanoCorte[i][j].z>1)
           {
           x = PlanoCorte[i][j].x;
           y = PlanoCorte[i][j].y;
           z = PlanoCorte[i][j].z;

           xp = floor(PlanoCorte[i][j].x);
           xt =  ceil(PlanoCorte[i][j].x);
           yp = floor(PlanoCorte[i][j].y);
           yt =  ceil(PlanoCorte[i][j].y);
           zp = floor(PlanoCorte[i][j].z);
           zt =  ceil(PlanoCorte[i][j].z);

           xd = x-xp;
           yd = y-yp;
           zd = z-zp;

           i1 = (M->GetV(xp,yp,zp)*(1.0-zd)+ M->GetV(xp,yp,zt)*zd);
           i2 = (M->GetV(xp,yt,zp)*(1.0-zd)+ M->GetV(xp,yt,zt)*zd);
           j1 = (M->GetV(xt,yp,zp)*(1.0-zd)+ M->GetV(xt,yp,zt)*zd);
           j2 = (M->GetV(xt,yt,zp)*(1.0-zd)+ M->GetV(xt,yt,zt)*zd);
           w1 = i1*(1.0-yd)+i2*yd;
           w2 = j1*(1.0-yd)+j2*yd;

           valor = (w1*(1.0-xd)+w2*xd);

           MToroide->SetV(i,j,k, valor);
           }
           else{MToroide->SetV(i,j,k, minPX);}
         }

   ProgressBar1->Position=0;
   }

//Resultado de esta etapa, MToroide posee todos los planos Toroidales.

//Encontrar el primer punto semilla para el algoritmo AortaCavityFill:
//Lógica: se realiza un AortaCavityFill hacia abajo de CD, buscando el último punto
//que coincida con no irse demasiado lejos del punto encontrado en el paso anterior:

AxialCavityFilling(MToroide);

AP->Tag = 0; //ROIListBox->Tag = 0 => No graba los resultados iniciales de la segmentación

//Resultado: AortaCenterLine tiene un Certerline parcial, calculado desde CD hacia
// el diafragma. El último punto detectado será considerado un punto semilla para
//el algoritmo de AortaCavityFilling

int aSeedX, aSeedY;
aSeedX = xP;
aSeedY = yP;
float rSeed = r0;

//Eliminación de AortaCenterLine parcial, y comienzo del algoritmo completo (1ra vuelta)
//en el punto semilla encontrado en la región axial en AxialCavityFilling.
////La posición de la semilla se encuentra en xP, yP con radio r0.
PuntoF* paux;
for(int i=0; i<AortaCenterLine->Count; i++)
{
paux = (PuntoF*)AortaCenterLine->Items[i];
delete paux;
}
AortaCenterLine->Clear();

float *pf;
for(int i=0; i<AortaRadios->Count; i++)
{
pf = (float*)AortaRadios->Items[i];
delete[] pf;
}
AortaRadios->Clear();

//Restauro los valores de MToroide para la segunda vuelta de Segmentación:
MToroide->LlenarTodo(minPX);

for(int k=0; k<cantSlices; k++)
{
ProgressBar1->Position=k;

vector1.x=0.0;
vector1.y=0.0;
vector1.z=1.0;

SetToroidalPlaneFromIndex(k);

    for (int i=anchoPX/5; i< 3*anchoPX/5; ++i)
        for (int j=altoPX/5; j<3*altoPX/5; ++j)
        {

         if(PlanoCorte[i][j].x<altoPX-1 && PlanoCorte[i][j].x>1 &&
           PlanoCorte[i][j].y<anchoPX-1 && PlanoCorte[i][j].y>1 &&
           PlanoCorte[i][j].z<cantImagenes-1 && PlanoCorte[i][j].z>1)
           {
           x = PlanoCorte[i][j].x;
           y = PlanoCorte[i][j].y;
           z = PlanoCorte[i][j].z;

           xp = floor(PlanoCorte[i][j].x);
           xt =  ceil(PlanoCorte[i][j].x);
           yp = floor(PlanoCorte[i][j].y);
           yt =  ceil(PlanoCorte[i][j].y);
           zp = floor(PlanoCorte[i][j].z);
           zt =  ceil(PlanoCorte[i][j].z);

           xd = x-xp;
           yd = y-yp;
           zd = z-zp;

           i1 = (M->GetV(xp,yp,zp)*(1.0-zd)+ M->GetV(xp,yp,zt)*zd);
           i2 = (M->GetV(xp,yt,zp)*(1.0-zd)+ M->GetV(xp,yt,zt)*zd);
           j1 = (M->GetV(xt,yp,zp)*(1.0-zd)+ M->GetV(xt,yp,zt)*zd);
           j2 = (M->GetV(xt,yt,zp)*(1.0-zd)+ M->GetV(xt,yt,zt)*zd);
           w1 = i1*(1.0-yd)+i2*yd;
           w2 = j1*(1.0-yd)+j2*yd;

           valor = (w1*(1.0-xd)+w2*xd);

           MToroide->SetV(i,j,k, valor);
           }
           else{MToroide->SetV(i,j,k, minPX);}
         }

   ProgressBar1->Position=0;
   }

AortaCavityFilling(MToroide);

//Creación del volumen de cortes perpendiculares a AortaCenterLine encontrado antes:
delete MToroide;

cantSlices = AortaCenterLine->Count;

MPerp = new Volumen(M->getAncho(), M->getAlto(), cantSlices);
MPerp->LlenarTodo(minPX);

ProgressBar1->Position=0;
ProgressBar1->Max=cantSlices;

vector1.x=0.0;
vector1.y=0.0;
vector1.z=1.0;

for(int k=0; k<cantSlices; k++)
{
ProgressBar1->Position=k;

SetCenterLinePlaneFromIndex(k, 0.33);

for (int i=anchoPX/3; i<2*anchoPX/3; ++i)
        for (int j=altoPX/3; j<2*altoPX/3; ++j)
        {

               if(PlanoCorte[i][j].x<altoPX-1 && PlanoCorte[i][j].x>1 &&
                  PlanoCorte[i][j].y<anchoPX-1 && PlanoCorte[i][j].y>1 &&
                  PlanoCorte[i][j].z<cantImagenes-1 && PlanoCorte[i][j].z>1)
                  {
                  x = PlanoCorte[i][j].x;
                  y = PlanoCorte[i][j].y;
                  z = PlanoCorte[i][j].z;

                  xp = floor(PlanoCorte[i][j].x);
                  xt =  ceil(PlanoCorte[i][j].x);
                  yp = floor(PlanoCorte[i][j].y);
                  yt =  ceil(PlanoCorte[i][j].y);
                  zp = floor(PlanoCorte[i][j].z);
                  zt =  ceil(PlanoCorte[i][j].z);

                  xd = x-xp;
                  yd = y-yp;
                  zd = z-zp;

                  i1 = (M->GetV(xp,yp,zp)*(1.0-zd)+ M->GetV(xp,yp,zt)*zd);
                  i2 = (M->GetV(xp,yt,zp)*(1.0-zd)+ M->GetV(xp,yt,zt)*zd);
                  j1 = (M->GetV(xt,yp,zp)*(1.0-zd)+ M->GetV(xt,yp,zt)*zd);
                  j2 = (M->GetV(xt,yt,zp)*(1.0-zd)+ M->GetV(xt,yt,zt)*zd);
                  w1 = i1*(1.0-yd)+i2*yd;
                  w2 = j1*(1.0-yd)+j2*yd;

                  valor = (w1*(1.0-xd)+w2*xd);

                  MPerp->SetV(i,j,k, valor);
                  }
                  else{MPerp->SetV(i,j,k, minPX);}
             }

       }
       ProgressBar1->Position=0;

//Elimino AortaCenterLine, AortaNormal y AortaRadios, para volver a calcularlos
//en MPerp (cortes perpendiculares). Antes, reemplazo el "bastón" por el centerline
//parcial encontrado, con sus normales:

for(int i=0; i<CurvaToroide->Count; i++)
{
paux = (PuntoF*)CurvaToroide->Items[i];
delete paux;
paux = (PuntoF*)NormalToroide->Items[i];
delete paux;
}
CurvaToroide->Clear();
NormalToroide->Clear();

float* raux;

for(int i=0; i<AortaCenterLine->Count; i++)
{
paux = (PuntoF*)AortaCenterLine->Items[i];
CurvaToroide->Add(paux);
paux = (PuntoF*)AortaNormales->Items[i];
NormalToroide->Add(paux);
raux = (float*)AortaRadios->Items[i];
delete[] raux;
}


AortaCenterLine->Clear();
AortaNormales->Clear();
AortaRadios->Clear();

zP = 0;
xP = anchoPX/2;
yP = altoPX/2;
r0 = rSeed;



ROIListBox->Tag = 1; //ROIListBox->Tag = 1 => Dibuja los resultados de la segmentación

AP->Tag = 1;

usarAortaProm = true;
AortaCavityFilling(MPerp);


//Restauro los planos perpendiculares para visualización del resultado:
/*
ProgressBar1->Position = 0;
ProgressBar1->Max = AortaCenterLine->Count;

cantSlices = AortaCenterLine->Count;

for(int k=0; k<cantSlices; k++)
{
ProgressBar1->Position=k;

SetCenterLinePlaneFromIndex(k);

for (int i=0; i<anchoPX; ++i)
        for (int j=0; j<altoPX; ++j)
        {

               if(PlanoCorte[i][j].x<altoPX-1 && PlanoCorte[i][j].x>1 &&
                  PlanoCorte[i][j].y<anchoPX-1 && PlanoCorte[i][j].y>1 &&
                  PlanoCorte[i][j].z<cantImagenes-1 && PlanoCorte[i][j].z>1)
                  {
                  x = PlanoCorte[i][j].x;
                  y = PlanoCorte[i][j].y;
                  z = PlanoCorte[i][j].z;

                  xp = floor(PlanoCorte[i][j].x);
                  xt =  ceil(PlanoCorte[i][j].x);
                  yp = floor(PlanoCorte[i][j].y);
                  yt =  ceil(PlanoCorte[i][j].y);
                  zp = floor(PlanoCorte[i][j].z);
                  zt =  ceil(PlanoCorte[i][j].z);

                  xd = x-xp;
                  yd = y-yp;
                  zd = z-zp;

                  i1 = (M->GetV(xp,yp,zp)*(1-zd)+ M->GetV(xp,yp,zt)*zd);
                  i2 = (M->GetV(xp,yt,zp)*(1-zd)+ M->GetV(xp,yt,zt)*zd);
                  j1 = (M->GetV(xt,yp,zp)*(1-zd)+ M->GetV(xt,yp,zt)*zd);
                  j2 = (M->GetV(xt,yt,zp)*(1-zd)+ M->GetV(xt,yt,zt)*zd);
                  w1 = i1*(1-yd)+i2*yd;
                  w2 = j1*(1-yd)+j2*yd;

                  valor = (w1*(1-xd)+w2*xd);

                  MPerp->SetV(i,j,k, valor);
                  }
                  else{MPerp->SetV(i,j,k, minPX);}
             }

       }
       ProgressBar1->Position=0;
*/

ToroideScrollBar->Min=0;
ToroideScrollBar->Max=AortaCenterLine->Count-1;


ArbitraryCutForm1->ROIShape->Visible = false;
ArbitraryCutForm1->CenterShape->Visible = false;


drawAorta = true;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::AxialCavityFilling1Click(TObject *Sender)
{
        AxialCavityFilling(MToroide);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::CirculoAxialCavityFilling1Click(TObject *Sender)
{
       CirculoAxialCavityFilling(MToroide);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AxialCavityFilling(Volumen *MV)
{


}
//---------------------------------------------------------------------------

void __fastcall TForm1::CirculoAxialCavityFilling(Volumen* MV)
{
//Toma el punto incial en la posición del puntero del Mouse o en alguna
//ubicación previa:

        PuntoF* nP;

        float x0 = xP;
        float y0 = yP;

        float z0 = float(SliceListBox->Tag);
        float r = r0;  //r0 está originalmente seteado proporcional al radio de la Aorta Descendete, rD

        float ax0,ay0,ar;
        ax0 = xP;
        ay0 = yP;
        ar = r0;

        float posmaxX, posmaxY, rmax;

        //vecindad a revisar:
        int v = 7;

        //Seteo los limites de una region de interés alrededor del punto inicial:
        float xL0, yL0, xLf, yLf;

        //Región de interés "fija":
        int LadoROI = 50;

        xL0 = x0-LadoROI;
        xLf = x0+LadoROI;
        yL0 = y0-LadoROI;
        yLf = y0+LadoROI;

        if(xL0<0) xL0 = 0;
        if(xLf>anchoPX-1) xLf = anchoPX-1;
        if(yL0<0) yL0 = 0;
        if(yLf> altoPX-1) yLf = altoPX -1;

        //Activación de la ROI!:
        //ROICheckBox->Checked = true;
        ROIShape->Left = yL0;
        ROIShape->Top = xL0;
        ROIShape->Width = yLf-yL0;
        ROIShape->Height= xLf-xL0;

        //Dibujo la ROI:
        ArbitraryCutForm1->ROIShape->Left = yL0;
        ArbitraryCutForm1->ROIShape->Top = xL0;
        ArbitraryCutForm1->ROIShape->Width = yLf-yL0;
        ArbitraryCutForm1->ROIShape->Height= xLf-xL0;
        ArbitraryCutForm1->ROIShape->Repaint();

        //Dibujo el círculo inicial:
        ArbitraryCutForm1->SectionShape->Pen->Color = clBlue;
        ArbitraryCutForm1->SectionShape->Width  = 2*redondeo(r0)+1;
        ArbitraryCutForm1->SectionShape->Height = 2*redondeo(r0)+1;
        ArbitraryCutForm1->SectionShape->Left = yP - ArbitraryCutForm1->SectionShape->Width/2;
        ArbitraryCutForm1->SectionShape->Top  = xP - ArbitraryCutForm1->SectionShape->Height/2;
        ArbitraryCutForm1->SectionShape->Repaint();

        //Dibujo el centro inicial:
        ArbitraryCutForm1->CenterShape->Brush->Color = clBlue;
        ArbitraryCutForm1->CenterShape->Visible = true;
        ArbitraryCutForm1->CenterShape->Left = yP-ArbitraryCutForm1->CenterShape->Width/2;
        ArbitraryCutForm1->CenterShape->Top = xP-ArbitraryCutForm1->CenterShape->Height/2;
        ArbitraryCutForm1->CenterShape->Repaint();

        ArbitraryCutForm1->Refresh();
        ArbitraryCutForm1->Repaint();

        TList* CircuList = new TList;

        //Calculo de media y desvio dentro de un circulo de exploracion dentro de la aorta:
        float r_explorador = 10.0;
        strelCirc2D(r_explorador,x0,y0,z0,CircuList);
        PuntoF* pa;
        float cont = 0;
        double cont2=0;

        for(int i=0; i<CircuList->Count; i++)
        {
                pa = (PuntoF*)CircuList->Items[i];
                cont += MV->GetV(pa->x, pa->y, pa->z);

               // if(ROIListBox->Tag)
               // ROIListBox->Items->Add(MV->GetV(pa->x, pa->y, pa->z));

                cont2 +=  MV->GetV(pa->x, pa->y, pa->z)*MV->GetV(pa->x, pa->y, pa->z);
                delete pa;
        }

       // if(ROIListBox->Tag)
       // ROIListBox->Items->SaveToFile(SliceListBox->Tag);

        float Nn = float(CircuList->Count);
        float mu = cont/Nn;
        float sigma = sqrt(cont2/Nn -mu*mu);

        nAorta++;
        muAorta +=mu;
        sigmaAorta +=sigma;

        if(usarAortaProm)
        {
        mu = muAorta/nAorta;
        sigma = sigmaAorta/nAorta;
        usarAortaProm = false;
        }

        //Valores típicos de mu = 40
        //Valores típicos de sigma: 20 sin filtrar, 5 filtrado
        //umbral = mu - float(nsigmas)*sigma;

        //Si el tag es uno, es porque ya viene de una prueba fallida, no
        //es necesario volver a filtrar:
        if(!ROIListBox->Tag)
        {
        //Preprocesamiento!!:
        //1- Filtro de Mediana:  Probar también bilateral:
        FiltroMediana(MV);

        //Apertura Morfologica:
        float radioAP = 10.0*res[0];
        AperturaMorfologica(radioAP,z0,MV);

        //Binarización por el método de Isodata:
        //Isodata(MV, z0, mu, sigma);
        //Probar Otsu?
        }

        float umbral;
        //umbral = Isodata(MV, z0, mu, sigma, nsigmas);
        umbral = mu - float(nsigmas)*sigma;


        for(int i=xL0-1; i<xLf+1; i++)
                for(int j=yL0-1; j<yLf+1; j++)
                        auxArray[i][j]=minPX;

        //Búsqueda de la cavidad en forma de 4-vecinos:
        int vN, vS, vE, vW;
        for(int i=xL0; i<xLf; i++)
          for(int j=yL0; j<yLf; j++)
          {

           vN = 0;
           vS = 0;
           vE = 0;
           vW = 0;

             if( MV->GetV(i,j,z0) >= umbral )
             {
              if( MV->GetV(i+1,j,z0)< umbral )
              vN = 1;
              if( MV->GetV(i,j+1,z0)< umbral )
              vE = 1;
              if( MV->GetV(i,j-1,z0)< umbral )
              vW = 1;
              if( MV->GetV(i-1,j,z0)< umbral )
              vS = 1;
             }

           if(vN+vS+vE+vW > 0)
           auxArray[i][j]=maxPX;

          }

          //BoundaryList;
          TList* BL = new TList;
          Punto* paux;

          for(int i=xL0; i<xLf; i++)
                for(int j=yL0; j<yLf; j++)
                 if(auxArray[i][j]==maxPX)
                 {
                 MV->SetV(i,j,z0,maxPX);

                 paux = new Punto;

                 paux->x = i;
                 paux->y = j;
                 paux->z = z0;

                 BL->Add(paux);
                 }


         //Luego de crear el BoundaryList, el auxArray se utilizará de manera que
         // 0: pixel no revisado, 1: pixel revisado
        for(int i=xL0; i<xLf; i++)
                for(int j=yL0; j<yLf; j++)
                auxArray[i][j] = 0;

        //Si no se detectaron contornos...
        if(BL->Count == 0)
        {
        //Y es la segunda vez que sucede, entonces repite el punto anterior:
               if(ImageScrollBar->Tag == 1)
               {
                ImageScrollBar->Tag = 0;

                posmaxX = ax0;
                posmaxY = ay0;
                rmax = ar;

                xP = posmaxX;
                yP = posmaxY;

                PuntoF* pE = new PuntoF;

                pE->x = PlanoCorte[xP][yP].x;
                pE->y = PlanoCorte[xP][yP].y;
                pE->z = PlanoCorte[xP][yP].z;

                //Escalar los puntos del centerline:
                pE->x *= res[0];
                pE->y *= res[1];
                pE->z *= res[2];

                float* aRadio; //actual Radio, radio del corte actual:
                aRadio  = new float[1];
                aRadio[0] = float(rmax)*res[0];

                //Aorta Centerline Escalado:

                nP = new PuntoF;
                pE = (PuntoF*)NormalToroide->Items[SliceListBox->Tag];


                nP->x = pE->x;
                nP->y = pE->y;
                nP->z = pE->z;

                AortaNormales->Add(nP);

                AortaCenterLine->Add(pE);

                //Aorta radios escalados:
                AortaRadios->Add(aRadio);

                return;

                }
                else
                {
                ImageScrollBar->Tag = 1;
                return;
                }
        }

         //Defino mínima distancia y distancia actual:
         float minDist, aDist, maxMinDist;

        //1: Búsqueda de la mínima distancia desde (x0,y0) a cualquier contorno:
         if(MV->GetV(x0,y0,z0) >= umbral)
         {
         minDist = FLT_MAX;

          for(int b = 0; b<BL->Count; b++)
          {
          paux = (Punto*)BL->Items[b];

          //Calculo la distancia del punto (x0,y0,z0) a todos los puntos contorno:
          aDist = sqrt( (x0-paux->x)*(x0-paux->x) + (y0-paux->y)*(y0-paux->y) );

            //Si el valor de mínima distancia es menor o igual a un cierto porcentaje del mínimo encontrado
            //hasta ese momento...:
            if(aDist < minDist)
            {
            //... lo hago minDist
            minDist = aDist;
            posmaxX = x0;
            posmaxY = y0;
            maxMinDist = aDist;
            }

          }

         }

         auxArray[int(x0)][int(y0)] = 1; //Marco el punto como "ya revisado".

         //Almaceno los primeros 8 puntos candidatos, vecino de x0,y0,z0
         TList* PuntosCandidatos = new TList;

         for(int i=-v; i<v+1; i++)
          for(int j=-v; j<v+1; j++)
          {
           if(abs(i)+abs(j)) //Cuando los indices no valen 0 simulatáneamente
           //(así evito volver a agregar el centro de la vecindad como candidato)...
           {
             //Agrego los primeros candidatos si cumplen con el requisito:
             if(MV->GetV(x0+i,y0+j,z0) >= umbral)
             {
             paux = new Punto;
             paux->x = x0+i;
             paux->y = y0+j;
             PuntosCandidatos->Add(paux);  //Los agrego a la lista y
             auxArray[int(x0+i)][int(y0+j)]=1; //los marco como ya revisados
             }
           }
          }


         //Puntero al punto actual:
         Punto* aPunto;

        while(PuntosCandidatos->Count)
        {
         minDist = FLT_MAX;

         //Tomo el primer elemento de la lista:
         aPunto = (Punto*)PuntosCandidatos->First();

         //Lo señalo como ya revisado:
          auxArray[aPunto->x][aPunto->y]=1;

          //Busco la mínima distancia al borde para ver si lo uso como punto candidato:
          for(int b = 0; b<BL->Count; b++)
          {
          paux = (Punto*)BL->Items[b];

           //Calculo la distancia del punto actual a todos los puntos contorno:
          aDist = sqrt( (aPunto->x-paux->x)*(aPunto->x-paux->x) + (aPunto->y-paux->y)*(aPunto->y-paux->y) );

            //Si el valor de mínima distancia es menor o igual a cierto porcentaje del mínimo encontrado
            //hasta ese momento...:
           if(aDist < minDist)
           {
           //... lo almaceno en minDist
           minDist = aDist;
           }
          }

         //Si la mínima distancia encontrada es MAYOR a cierto porcentaje de la máxima encontrada hasta el momento,
         //entonces ese punto es un candidato válido a examinar sus vecinos:
         if(minDist >= maxMinDist)
         {

         //Si la mínima distancia encontrada es MAYOR a la máxima encontrada hasta el momento, es momentaneamente
         //el punto elegido:
         if(minDist >= maxMinDist)
         {
         maxMinDist = minDist;
         posmaxX = aPunto->x;
         posmaxY = aPunto->y;
         }

          for(int i=-v; i<v+1; i++)
           for(int j=-v; j<v+1; j++)
           {
            if( abs(i)+abs(j)) //Cuando los indices no valen 0 simulatáneamente
            //(así evito volver a agregar el centro de la vecindad como candidato)...
            {
             //Agrego los primeros candidatos si cumplen con el requisito de superar el umbral y no haber sido agregados antes:
             if(MV->GetV(aPunto->x+i,aPunto->y+j,z0) >= umbral && auxArray[aPunto->x+i][aPunto->y+j]==0)
             {
              //Además, deben caer dentro de la ROI:
              if(aPunto->x+i<xLf && aPunto->x+i>xL0 && aPunto->y+j<yLf && aPunto->y+j>yL0)
              {
               paux = new Punto;
               paux->x = aPunto->x+i;
               paux->y = aPunto->y+j;
               PuntosCandidatos->Add(paux);
               auxArray[aPunto->x+i][aPunto->y+j] = 1;
              }
             }
            }
           }
         }

        //Elimino el punto ya revisado:
        delete aPunto;
        PuntosCandidatos->Delete(0);
        }

      rmax = redondeo(maxMinDist);
      PuntosCandidatos->Clear();
      delete PuntosCandidatos;


     //Si está demasiado lejos, y además es la segunda vez que da mal, lo reemplazo por el resultado anterior:
     if(sqrt((posmaxX-ax0)*(posmaxX-ax0)+(posmaxY-ay0)*(posmaxY-ay0)) > ar)
     {
      if(ImageScrollBar->Tag == 1)
      {
      posmaxX = ax0;
      posmaxY = ay0;
      rmax = ar*1.2;
      ImageScrollBar->Tag = 0;
      }
      else
      {
      ImageScrollBar->Tag = 1;
      return;
      }
     }


        xP = posmaxX;
        yP = posmaxY;
        zP = z0;
        r0 = rmax*0.8;

        //Cálculo de las coordenadas del punto encontrado, en función del plano
        //recosntruido al que pertenece:
        PuntoF* pE = new PuntoF;
        nP = new PuntoF;

        pE->x = PlanoCorte[xP][yP].x;
        pE->y = PlanoCorte[xP][yP].y;
        pE->z = PlanoCorte[xP][yP].z;

        //Escalar los puntos del centerline:
        pE->x *= res[0];
        pE->y *= res[1];
        pE->z *= res[2];

        float* aRadio; //actual Radio, radio del corte actual:
        aRadio  = new float[1];
        aRadio[0] = float(rmax)*res[0];

        //Aorta Centerline Escalado:
        AortaCenterLine->Add(pE);

        pE = (PuntoF*)NormalToroide->Items[SliceListBox->Tag];

        nP->x = pE->x;
        nP->y = pE->y;
        nP->z = pE->z;

        AortaNormales->Add(nP);
        //Aorta radios escalados:
        AortaRadios->Add(aRadio);

        //Dibujo el círculo final:
        ArbitraryCutForm1->SectionShape->Pen->Color = clRed;
        ArbitraryCutForm1->SectionShape->Width  = 2*rmax;
        ArbitraryCutForm1->SectionShape->Height = 2*rmax;
        ArbitraryCutForm1->SectionShape->Left = yP - ArbitraryCutForm1->SectionShape->Width/2;
        ArbitraryCutForm1->SectionShape->Top  = xP - ArbitraryCutForm1->SectionShape->Height/2;
        ArbitraryCutForm1->SectionShape->Repaint();

        //Dibujo el centro final:
        ArbitraryCutForm1->CenterShape->Brush->Color = clGreen;
        ArbitraryCutForm1->CenterShape->Visible = true;
        ArbitraryCutForm1->CenterShape->Left = yP-ArbitraryCutForm1->CenterShape->Width/2;
        ArbitraryCutForm1->CenterShape->Top = xP-ArbitraryCutForm1->CenterShape->Height/2;
        ArbitraryCutForm1->CenterShape->Repaint();

        ArbitraryCutForm1->Refresh();
        ArbitraryCutForm1->Repaint();



}
//-----------------------------------------------------------------------------

void __fastcall TForm1::FiltroMediana(Volumen* MV)
{

//Realiza filtro de mediana dentro de la ROI:
   int zf=ROIShape->Tag+1;
   int zi=ROIShape->Tag;

   if(zf==MV->getProf()-1)
   return;

   int xi = ROIShape->Top;
   int xf = ROIShape->Top+ROIShape->Height;
   int yi = ROIShape->Left;
   int yf = ROIShape->Left+ROIShape->Width;

   //ProgressBar1->Max = zf-zi+1;

   for(int i=0; i<512; i++)
     for(int j=0; j<512; j++)
       auxArray[i][j]=minPX;


        //Máscara de 5x5:
        int ladoMediana = 7;
        int semiLado = ladoMediana/2;
        int tamanoVector = ladoMediana*ladoMediana;

        short int* arrayMediana = new short int[tamanoVector];

        for (int k=zi;k<zf;k++)
       {
      //  ProgressBar1->Position=k;
        for(int i=xi; i<xf;i++)
                 for(int j=yi; j<yf;j++)
                        {
                              int h=0;
                              for(int k1=-semiLado;k1<=semiLado;k1++)
                                for(int k2=-semiLado;k2<=semiLado;k2++)
                                        {
                                        arrayMediana[h]=MV->GetV(i+k1,j+k2,k);
                                        h++;
                                        }

                                        int jj=0;

                                        for(int ii=0; ii<tamanoVector; ii++)
                                        {

                                                for(int lj=0;lj<tamanoVector-1-ii;lj++)
                                                        if(arrayMediana[lj]>arrayMediana[lj+1])
                                                        {
                                                        short int aux = arrayMediana[lj+1];
                                                        arrayMediana[lj+1]=arrayMediana[lj];
                                                        arrayMediana[lj] = aux;
                                                        }

                                        jj++;
                                        }

                            auxArray[i][j]= arrayMediana[tamanoVector/2];


                        }

            for(int i=xi; i<xf;i++)
                 for(int j=yi; j<yf;j++)
                     MV->SetV(i,j,k,auxArray[i][j]);
        }


        delete [] arrayMediana;
       // ProgressBar1->Position=0;

       // GraficarEnPantalla();

}
//----------------------------------------------------------------------------

void __fastcall TForm1::Setup1Click(TObject *Sender)
{

    	for(int i = 0; i < 256; i++)
           {
           lut2[i][0] = 1.0;
           lut2[i][1] = 1.0;
           lut2[i][2] = 0.5;
           }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
    glGenTextures(1, &lutex);
    glBindTexture(GL_TEXTURE_1D, lutex);

  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, lut2);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

//texturas = new GLuint[cantTexturas];

    RefreshOrtho(Sender);

     //for(int i=0; i<cantTexturas; i++)
     //{
     // SetupTexturesFromMatrixPosition(M, minPX,altoPX, anchoPX, cantImagenes,LUT, texturas[i], i);
     //}
   // SetupTexturesFromMatrixPosition(M, minPX,altoPX, anchoPX, cantImagenes,LUT, tex1, 0);
   // SetupTexturesFromMatrixPosition(M, minPX,altoPX, anchoPX, cantImagenes,LUT, tex2, 10);
   // SetupTexturesFromMatrixPosition(M, minPX,altoPX, anchoPX, cantImagenes,LUT, tex3, 20);
   // SetupTexturesFromMatrixPosition(M, minPX,altoPX, anchoPX, cantImagenes,LUT, tex4, 30);
   // SetupTexturesFromMatrixPosition(M, minPX,altoPX, anchoPX, cantImagenes,LUT, tex5, 40);
   // SetupTexturesFromMatrixPosition(M, minPX,altoPX, anchoPX, cantImagenes,LUT, tex6, 50);
   // SetupTexturesFromMatrixPosition(M, minPX,altoPX, anchoPX, cantImagenes,LUT, tex7, 60);
     IdleLoop(Sender,1);

     

}
//---------------------------------------------------------------------------

int __fastcall TForm1::CrearTexturas()
{
PFNGLTEXIMAGE3DPROC glTexImage3D=(PFNGLTEXIMAGE3DPROC)wglGetProcAddress("glTexImage3D");
PFNGLGENPROGRAMSARBPROC glGenProgramsARB=(PFNGLGENPROGRAMSARBPROC)wglGetProcAddress("glGenProgramsARB");
PFNGLBINDPROGRAMARBPROC glBindProgramARB=(PFNGLBINDPROGRAMARBPROC)wglGetProcAddress("glBindProgramARB");
PFNGLPROGRAMSTRINGARBPROC glProgramStringARB=(PFNGLPROGRAMSTRINGARBPROC)wglGetProcAddress("glProgramStringARB");
PFNGLDELETESHADERPROC glDeleteShader=(PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");


  //Set pixel storage modes: Specifies the alignment requirements for the start of
  //each pixel row in memory. Value 1 means (byte-alignment):
  glPixelStorei(GL_UNPACK_ALIGNMENT,1);

  //Pido dos nuevas texturas:
  glGenTextures(2,texName);

  //Uno una textura 3D a texName[0]: TEXTURE0
  glBindTexture(GL_TEXTURE_3D,texName[0]);

  //...defino como esta textura se comportará fuera de sus límites, o al expandir y al achicar.
  glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);   //No hay nada fuera de la textura: Clamp
  glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);      //Interpolación trilieal: GL_LINEAR
  glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

  //Forma en la que una porción de la textura se copiará a un glQuad existente:
  //GL_REPLACE, implica copiarse enteramente sin inportar si se tiene algo dibujado
  //previamente sobre el glQuad:
  glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);

  //Reservo el lugar para la textura 3D:
  plano=new GLushort[altoPX*anchoPX];

  int maxtexsize;

  glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &maxtexsize);

  //Creo la textura sin llenar los texels:
 // glTexImage3D(GL_PROXY_TEXTURE_3D,0,GL_INTENSITY16,altoPX,anchoPX,altoPX,0,GL_LUMINANCE,GL_UNSIGNED_SHORT,plano);
 // GLenum error= glGetError();
  glTexImage3D(GL_TEXTURE_3D,0,GL_INTENSITY16,altoPX,anchoPX,cantImagenes,0,GL_LUMINANCE,GL_UNSIGNED_SHORT,NULL);
  GLenum error= glGetError();

  AnsiString textoauxansi;
  if(error!= GL_NO_ERROR)
  {
  	const GLubyte *errortext=gluErrorString(error);
  	textoauxansi.printf("%s",errortext);
  	Application->MessageBox(textoauxansi.c_str(),"Error Textura3D",MB_OK);
  }

  //Leo el código del fragment-shader como texto:
  char program_string[16384];
  int len;

  FILE *fp=fopen(ShaderDir.c_str(),"rt");

  if(fp!=NULL)
  {
  	len=fread(program_string,sizeof(char),16384,fp);
  	program_string[len]='\0';
  	fclose(fp);
  }

  //Me fijo que no haya otro fragment-shader anter: (ver que pasa si se elimina esta parte)
  if(shader_num!=-1)
  {
  	glDeleteShader(shader_num);
  	shader_num=-1;
  }


  //Creo el shader:
  glGenProgramsARB(1,&shader_num);
  glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB,shader_num);
  glProgramStringARB(GL_FRAGMENT_PROGRAM_ARB,GL_PROGRAM_FORMAT_ASCII_ARB,strlen(program_string),program_string);

  error= glGetError();

  //LUT: textura 1-D:
  //Textura 1D como transfer function
  int size;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE,&size);
  tfSize=size; //Crea la LUT del mayor tamaño posible

  //Creo la LUT  principal y la de los colores:
  pTransferFunction=new GLfloat[4*tfSize]; //RGBA
  pTF=new GLfloat[4*tfSize]; //transfer function 1

  //Creo una textura 1-D, por ahora vacía:
  glBindTexture(GL_TEXTURE_1D,texName[1]);
  glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexImage1D(GL_TEXTURE_1D,0,GL_RGBA,tfSize,0,GL_RGBA,GL_FLOAT,pTransferFunction);
  error= glGetError();

}
//------------------------------------------------------------------------------

void __fastcall TForm1::ActualizarTextura3D(int z)
{
PFNGLTEXSUBIMAGE3DPROC glTexSubImage3D=(PFNGLTEXSUBIMAGE3DPROC)wglGetProcAddress("glTexSubImage3D");
PFNGLACTIVETEXTUREPROC glActiveTexture=(PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");

  //Lleno el plano Z de la textura 3D con "plano":

   if(plano==NULL || z>=cantImagenes)
   return;

   //Hago un bind de la textura3D y ajusto la escala de la transferencia de pixels
   glBindTexture(GL_TEXTURE_3D,texName[0]); //bind texture 3D

   //Amplío el rango dinámico de 12 bits de la TC a 16 bits que permite la GPU:
   //probar si se puede hacer antes de llamar a esta función:

   glPixelTransferf(GL_RED_SCALE,65535.0/(GLfloat)(tfSize));
   glPixelTransferf(GL_GREEN_SCALE,65535.0/(GLfloat)(tfSize));
   glPixelTransferf(GL_BLUE_SCALE,65535.0/(GLfloat)(tfSize));

   //Cargo el plano z:
   glTexSubImage3D(GL_TEXTURE_3D,0,0,0,z,altoPX,anchoPX,1,GL_LUMINANCE,GL_UNSIGNED_SHORT,plano);
   GLenum error= glGetError();
   if(error!= GL_NO_ERROR)
   	Application->MessageBox("Error al cargar textura3D","Error Textura3D",MB_OK);

   //Probar si se puede hacer antes de llamar a la función:
   glPixelTransferf(GL_RED_SCALE,1.0);
   glPixelTransferf(GL_GREEN_SCALE,1.0);
   glPixelTransferf(GL_BLUE_SCALE,1.0);


}

//------------------------------------------------------------------------------

void __fastcall TForm1::LlenarPlanoConMatriz(int z)
{

for(int i=0; i<anchoPX; i++)
        for(int j=0; j<altoPX; j++)
        {
        //Calcula el valor de matriz, desplazado minPX a la derecha, para
        //estar indexado en el rango [0; maxPX-minPX];
        float valor = float(M->GetV(i,j,z)-minPX)/float(maxPX-minPX)*tfSize;
        plano[i*altoPX + j] = valor;
        }
}

//------------------------------------------------------------------------------

bool __fastcall TForm1::MostrarVolumenTexturas3D()
{
PFNGLBLENDEQUATIONEXTPROC glBlendEquationEXT=(PFNGLBLENDEQUATIONPROC)wglGetProcAddress("glBlendEquationEXT");
PFNGLACTIVETEXTUREPROC glActiveTexture=(PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");
PFNGLBINDPROGRAMARBPROC glBindProgramARB=(PFNGLBINDPROGRAMARBPROC)wglGetProcAddress("glBindProgramARB");

   //Esta función calcula el BoundingBox en función de la posición actual de la camara
   //y grafica los planos perpendiculares al observador cortando la Textura3D:

  // gets the direction of the observer

  double  gl_model[16] ;
  double  gl_proj [16] ;
  double x,y,z,SliceThick;
  float texnx, texny, texnz;

  texnx = float(anchoPX);
  texny = float(altoPX);
  texnz=float(cantImagenes);

  int     gl_view [ 4] ;

  glGetDoublev(GL_MODELVIEW_MATRIX , gl_model);
  glGetDoublev(GL_PROJECTION_MATRIX, gl_proj );
  glGetIntegerv(GL_VIEWPORT, gl_view );
  // gets the bounding box of the grid in the screen coordinates
  double xmin=FLT_MAX, xmax=-FLT_MAX, ymin=FLT_MAX, ymax=-FLT_MAX, zmin=FLT_MAX, zmax=-FLT_MAX;
  //Bounding BOX de la ROI
  for( int i = 0; i < 8; ++i )
  {
	float bbx = (i&1) ? texnx: 0.0f ;
	float bby = (i&2) ? texny : 0.0f;
	float bbz = (i&4) ? texnz : 0.0f;

	//el z va entre 0.0 (sobre el viewport) y 1.0 sobre el fondo (2*distancia)
	gluProject( bbx ,bby ,bbz, gl_model, gl_proj, gl_view, &x, &y, &z ) ;

	if( x < xmin ) xmin = x;
	if( x > xmax ) xmax = x;
	if( y < ymin ) ymin = y;
	if( y > ymax ) ymax = y;
	if( z < zmin ) zmin = z;
	if( z > zmax ) zmax = z;
  }
  bbXmin=xmin;
  bbXmax=xmax;
  bbYmin=ymin;
  bbYmax=ymax;
  bbZmin=zmin;
  bbZmax=zmax;


  //Push the relevant parts of the OpenGL state
  glPushAttrib(GL_COLOR_BUFFER_BIT   |
				 GL_DEPTH_BUFFER_BIT   |
				 GL_ENABLE_BIT         |
				 GL_LIGHTING_BIT       |
				 GL_POLYGON_BIT        |
				 GL_TEXTURE_BIT);

  //OpenGL setup
  glDisable(GL_LIGHTING);
  glDisable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  //For each pixel rendered, there are 2 values: the color which is currently
  // stored in the buffer(source color) and the color which is about to be written
  // to the buffer(destination color)

  glEnable(GL_BLEND);
  glDepthMask(GL_FALSE); //enable or disable writing into the depth buffer

 // glBlendEquationEXT(GL_FUNC_ADD_EXT);

//  glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//  glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);

  glActiveTexture(GL_TEXTURE0);
  glEnable(GL_TEXTURE_3D);
  glBindTexture(GL_TEXTURE_3D,texName[0]); //bind texture 3D

  //textura 1D como transfer function
  glActiveTexture(GL_TEXTURE1);
  glEnable(GL_TEXTURE_1D);
  glBindTexture(GL_TEXTURE_1D,texName[1]);

  //Fragment Shader
  glEnable(GL_FRAGMENT_PROGRAM_ARB);
  glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB,shader_num);
  //set 6 clipping planes

  GLdouble plane[4];
  plane[0] = +1. ;  plane[1] =  0. ;  plane[2] =  0. ;  plane[3] = 0.;
  glEnable( GL_CLIP_PLANE0 ) ;
  glClipPlane( GL_CLIP_PLANE0, plane ) ;

  plane[0] = -1. ;  plane[1] =  0. ;  plane[2] =  0. ;  plane[3] = texnx ;
  glEnable( GL_CLIP_PLANE1 ) ;
  glClipPlane( GL_CLIP_PLANE1, plane ) ;

  plane[0] =  0. ;  plane[1] = +1. ;  plane[2] =  0. ;  plane[3] = 0. ;
  glEnable( GL_CLIP_PLANE2 ) ;
  glClipPlane( GL_CLIP_PLANE2, plane ) ;

  plane[0] =  0. ;  plane[1] = -1. ;  plane[2] =  0. ;  plane[3] = texny ;
  glEnable( GL_CLIP_PLANE3 ) ;
  glClipPlane( GL_CLIP_PLANE3, plane ) ;

  plane[0] =  0. ;  plane[1] =  0. ;  plane[2] = +1. ;  plane[3] = 0.1 ;
  glEnable( GL_CLIP_PLANE4 ) ;
  glClipPlane( GL_CLIP_PLANE4, plane ) ;

  plane[0] =  0. ;  plane[1] =  0. ;  plane[2] = -1. ;  plane[3] = texnz + 0.1 ;
  glEnable( GL_CLIP_PLANE5 ) ;
  glClipPlane( GL_CLIP_PLANE5, plane ) ;


  // draw each slice of the texture in the viewer coordinates
 // float nslices=TBNSlices->Position ;
  //restriccion de zzmin y max

  int nslices = paso*100.0;

  zmin += float(MinzScrollBar->Position-MinzScrollBar->Min)/float(MinzScrollBar->Max)*(zmax-zmin);
  zmax -= float(MaxzScrollBar->Max-MaxzScrollBar->Position)/float(MaxzScrollBar->Max)*(zmax-zmin);

  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  float Dz = zmax-zmin;
  float dz = Dz/float(nslices) ;

  int i;
  double point[3];

  double fx = 1.0/float(texnx);
  double fy = 1.0/float(texny);
  double fz = 1.0/float(texnz);


  z  = (float)zmax;

  glBegin(GL_QUADS);
  
  for( int n = nslices-1 ; n >= 0 ; --n, z -= dz )  //dibujo del fondo hacia la cam
  {
	  gluUnProject( bbXmin,bbYmin,z, gl_model, gl_proj, gl_view, point + 0, point + 1, point + 2 ) ;
      	  glTexCoord3d( fx*point[0], fy*point[1], fz*point[2] );
	  glVertex3dv( point ) ;

	  gluUnProject( bbXmax,bbYmin,z, gl_model, gl_proj, gl_view, point + 0, point + 1, point + 2 ) ;
     	  glTexCoord3d( fx*point[0], fy*point[1], fz*point[2]);
	  glVertex3dv( point ) ;

	  gluUnProject( bbXmax,bbYmax,z, gl_model, gl_proj, gl_view, point + 0, point + 1, point + 2 ) ;
     	  glTexCoord3d( fx*point[0], fy*point[1], fz*point[2] );
	  glVertex3dv( point ) ;

	  gluUnProject( bbXmin,bbYmax,z, gl_model, gl_proj, gl_view, point + 0, point + 1, point + 2 ) ;
   	  glTexCoord3d( fx*point[0], fy*point[1], fz*point[2] );
	  glVertex3dv( point ) ;

  }
  glEnd();


  // disable cube clip plane
  glDisable( GL_CLIP_PLANE0 ) ;
  glDisable( GL_CLIP_PLANE1 ) ;
  glDisable( GL_CLIP_PLANE2 ) ;
  glDisable( GL_CLIP_PLANE3 ) ;
  glDisable( GL_CLIP_PLANE4 ) ;
  glDisable( GL_CLIP_PLANE5 ) ;


  //disable texture
  glDisable(GL_TEXTURE_3D);
  glDisable(GL_TEXTURE_1D);
  glDisable(GL_FRAGMENT_PROGRAM_ARB);
  glPopAttrib();

}

//------------------------------------------------------------------------------

void __fastcall TForm1::MinzScrollBarChange(TObject *Sender)
{
paso = atoi(PasoEdit->Text.c_str());
IdleLoop(Sender, false);
paso = PasoTrackBar->Position;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MaxzScrollBarChange(TObject *Sender)
{
paso = atoi(PasoEdit->Text.c_str());
IdleLoop(Sender, false);
paso = PasoTrackBar->Position;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::PasoTrackBarChange(TObject *Sender)
{
paso = PasoTrackBar->Position;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AbrirModelo1Click(TObject *Sender)
{
AP->Clear();

AortaCenterLine = new TList;
AortaNormales = new TList;
AortaRadios = new TList;

OpenDialog1->Filter = "DAT files (*.dat)|*.dat";
if(OpenDialog1->Execute())
AP->Items->LoadFromFile(OpenDialog1->FileName);

PuntoF* paux;

float x,y,z;
AnsiString xs,ys,zs;


float nx,ny,nz;
AnsiString nxs,nys,nzs;

float* r;
AnsiString rs;


for(int i=0; i<AP->Count; i=i+7)
{

r = new float[1];
paux = new PuntoF;

TReplaceFlags flag;

xs = StringReplace(AP->Items->Strings[i], ',', '.', flag);
ys = StringReplace(AP->Items->Strings[i+1], ',', '.', flag);
zs = StringReplace(AP->Items->Strings[i+2], ',', '.', flag);

x = atof(xs.c_str());
y = atof(ys.c_str());
z = atof(zs.c_str());

paux->x = x;
paux->y = y;
paux->z = z;

AortaCenterLine->Add(paux);

paux = new PuntoF;

nxs = StringReplace(AP->Items->Strings[i+3], ',', '.', flag);
nys = StringReplace(AP->Items->Strings[i+4], ',', '.', flag);
nzs = StringReplace(AP->Items->Strings[i+5], ',', '.', flag);

nx = atof(nxs.c_str());
ny = atof(nys.c_str());
nz = atof(nzs.c_str());

paux->x = nx;
paux->y = ny;
paux->z = nz;

AortaNormales->Add(paux);

rs = StringReplace(AP->Items->Strings[i+6], ',', '.', flag);
r[0] = atof(rs.c_str());

AortaRadios->Add(r);

}


}
//---------------------------------------------------------------------------

void __fastcall TForm1::AperturaMorfologica(int r, int z, Volumen* MV)
{

       //Apertura:

        //Realiza filtro de apertura morfológica dentro de la ROI:

        //Región de interés "fija":
        int LadoROI = 50;

        int xL0, xLf, yL0, yLf;

        xL0 = xP-LadoROI-r;
        xLf = xP+LadoROI+r;
        yL0 = yP-LadoROI-r;
        yLf = yP+LadoROI+r;

        if(xL0<0) xL0 = 0;
        if(xLf>anchoPX-1) xLf = anchoPX-1;
        if(yL0<0) yL0 = 0;
        if(yLf> altoPX-1) yLf = altoPX -1;

        //Listas y arrays auxiliares para la creación del elemento estructurante circular de radio=r
       TList* Mascara = new TList;

       strel2D(r, Mascara);

       Volumen* MAux = new Volumen(MV->getAlto(), MV->getAncho(), 1);

       short int max = minPX;
       short int min = maxPX;

       Punto *paux;

         for(int j=yL0-r; j<yLf+r;j++)
            for(int i=xL0-r; i<xLf+r;i++)
            {

             for(int s = 0; s<Mascara->Count; s++)
             {
              paux = (Punto*)Mascara->Items[s];

               if(MV->GetV(i+paux->x, j+paux->y, z)<min)
                min = MV->GetV(i+paux->x, j+paux->y,z);
             }

             MAux->SetV(i,j,0,min);

             min = maxPX;
          }

         for(int j=yL0-r; j<yLf+r;j++)
            for(int i=xL0-r; i<xLf+r;i++)
            {

             for(int s = 0; s<Mascara->Count; s++)
             {
              paux = (Punto*)Mascara->Items[s];

               if(MAux->GetV(i+paux->x, j+paux->y, 0)>max)
               max = MAux->GetV(i+paux->x, j+paux->y, 0);
             }

             MV->SetV(i,j,z,max);

             max = minPX;
            }

        for(int i=0; i<Mascara->Count; i++)
               delete Mascara->Items[i];

        delete Mascara;
        Mascara = NULL;

        delete MAux;
        MAux = NULL;

}

//---------------------------------------------------------------------------
float __fastcall TForm1::Isodata(Volumen* MV, int z, float mu, float sigma, float n)
{
     //Región de interés "fija":
     int LadoROI = 50;

     int xL0, xLf, yL0, yLf;
        xL0 = xP-LadoROI;
        xLf = xP+LadoROI;
        yL0 = yP-LadoROI;
        yLf = yP+LadoROI;

        float* hh;

        hh = new float[maxPX-minPX+1];

        for(int i=0; i<maxPX-minPX+1; i++)
        hh[i] = 0;

        if(xL0<0) xL0 = 0;
        if(xLf>anchoPX-1) xLf = anchoPX-1;
        if(yL0<0) yL0 = 0;
        if(yLf> altoPX-1) yLf = altoPX -1;

        int valor;

        float umbralM = mu+2*n*sigma;
        float umbralm = mu-2*n*sigma;


        //Cálculo del histograma dentro de la ROI:
        for(int i=xL0; i<xLf; i++)
         for(int j=yL0; j<yLf; j++)
         {
         valor = MV->GetV(i,j,z);

         if(valor< umbralM && valor>umbralm )
         hh[valor-minPX]++;
         }

         /*AP->Clear();

         for(int i=0; i<maxPX-minPX+1; i++)
         AP->Items->Add(histograma[i]);

         AP->Items->SaveToFile("hh.txt");*/

         //Seteo el umbral inicial:
         float umbral = mu-n*sigma;

         float Ui, Uf;
         float Ni, Nf;

         int Niter = 10;

         for(int iter=0; iter<Niter; iter++)
         {

         Ui = 0;
         Uf = 0;

         Ni = 0;
         Nf = 0;

          for(float i=minPX; i<umbral; i++)
          {
          Ui += hh[int(i-minPX)]*float(i);
          Ni += hh[int(i-minPX)];
          }

          Ui = Ui/Ni;


          for(int j=umbral; j<maxPX; j++)
          {
          Uf += hh[int(j-minPX)]*float(j);
          Nf += hh[int(j-minPX)];
          }

          Uf = Uf/Nf;

          umbral = (Ui+Uf)/2.0;
         }

     delete [] hh;

     return umbral;



}
//---------------------------------------------------------------------------

void __fastcall TForm1::DetectCarina(Volumen* Maux, Volumen* MAux)
{


    ProgressBar1->Max = 8;
    ProgressBar1->Position = 0;
    ProcessLabel->Visible = true;

   //Búsqueda del umbral por histograma:
   int umbral_inf = -900;
   int umbral_sup = -800;

   int i_umbral = 0;
   float max_umbral = 0;

   for(int i=umbral_inf-minPX; i<umbral_sup-minPX; i++)
   {
    if(histoCaja[i]>max_umbral)
    {
    max_umbral = histoCaja[i];
    i_umbral = i;
    }
   }

   short int umbralVol = i_umbral + minPX;

  Punto* paux; //Puntero a Punto Auxiliar

  TList* AC = new TList; //Lista Auxiliar

  short int Center = atof(CenterEdit->Text.c_str());
  short int Width = atof(WidthEdit->Text.c_str());

   ProgressBar1->StepIt();

   ProcessLabel->Caption = "Deleting TC borders/air";
   ProcessLabel->Repaint();
   ProcessLabel->Refresh();

  //1- Eliminacion de los bordes de la tomografía, siempre con color minPX:
  //Semillas: Esquinas de la imagen:
  Punto* spEsquina = new Punto;

  //Reconoce el espacio en blanco del primer plano, que luego se replicará para
  //todos los planos siguientes:
  spEsquina->x = 0; spEsquina->y = 0; spEsquina->z = 0;   //Semilla superior izquierda
  getAllMatrixPlaneConnected(spEsquina, MAux, Maux, AC, minPX+1);

  spEsquina->x = anchoPX-1; spEsquina->y = 0; spEsquina->z = 0; //Semilla inferior derecha
  getAllMatrixPlaneConnected(spEsquina, MAux, Maux, AC, minPX+1);

  spEsquina->x = 0; spEsquina->y = altoPX-1; spEsquina->z = 0;  //Semilla superior derecha
  getAllMatrixPlaneConnected(spEsquina, MAux, Maux, AC, minPX+1);

  spEsquina->x = anchoPX-1; spEsquina->y = altoPX-1; spEsquina->z = 0; //semilla inferior izquierda
  getAllMatrixPlaneConnected(spEsquina, MAux, Maux, AC, minPX+1);

  for(int i=0; i<AC->Count; i++)
  {
  paux = (Punto*)AC->Items[i];
    for(int k=1; k<cantImagenes; k++)
     MAux->SetV(paux->x, paux->y, paux->z+k, 0);  //Los reconoce como puntos a no ser revisados nuevamente
  }                                               //en toda la profundidad del estudio, ahorrando tiempo de búsqueda en 3D.

  //Eliminar el aire fuera del cuerpo identificando un pequeño rectángulo de lado
  //rE. Por defecto le damos el valor de 25 pixels a un lado y 50 pixels al otro:
  int rE = 45;

  for(int i=0; i<rE; i++)
   for(int j=altoPX/2-rE; j<altoPX/2+rE; j++)
   {
   spEsquina->x = i; spEsquina->y = j; spEsquina->z = 0;
   getAllMatrixVolumeConnected(spEsquina, MAux, Maux, AC, umbralVol); //En el caso de encontrar al menos una semilla válida, elimina
   }                                                            //el aire en profundidad. El resto de las semillas, si están
                                                                //conectadas a la primera encontrada (caso más probable) serán
                                                                //ignoradas en la búsqueda, ahorrando tiempo.

  //Si ya fueron marcados como recorridos, quedan excluidos del análisis por ser
  //los bordes de la tomografía o el aire delante del paciente.

  //Luego, se elimina la lista auxiliar:
  for(int i=0; i<AC->Count; i++)
  {
  paux = (Punto*)AC->Items[i];
  delete paux;
  }

  AC->Clear();
  delete AC;


  //Segundo: Segmentación de traquea
  //****************************************************************************

   ProgressBar1->StepIt();
   ProcessLabel->Caption = "Segmenting Trachea: 2D-connected components";
   ProcessLabel->Refresh();


  ConnectedComponentList = new TList;

  //Busca TODOS los elementos conexos (por encima o por debajo del umbral) plano x plano:
  ConnectedComponentLabelling(MAux, Maux, ConnectedComponentList, umbralVol, true); //true = busca plano x plano

  //Variable para cálculo auxiliar del perímetro del elemento conexo encontrado:
  float perim = 0;

  //Variable para el cálculo del "Factor de Redondez" de la figura encontrada:
  float FR = 0;

  //El umbral de redondez es el de una circunferencia perfecta:
  float umbralredondez = 1.0/(4.0*M_PI);

  //Tolerancia para especificar que tan parecido a un círculo se le permite ser
  //al componente conexo encontrado: por defecto se considera "casi circular" si el
  //FR > 10% del umbralredondez:
  float tolerancia = 0.05;

  //Los componentes conexos encontrados plano x plano se eliminan por 3 umbrales:
  //1- Por color: los elementos conexos por encima del umbral nunca pueden ser aire, entonces son eliminados.
  //   Son considerados en primera instancia para tener una idea del tamaño del paciente, encontrando el elemento
  //   conexo más grande en todos los planos (generalmente perteneciente a la unión corazón-músculos-caja torácica.
  //2- Por tamaño: Aquellos elementos conexos que no sean al menos 10 veces más chicos que el máximo elemento conexo
  //               encontrado serán eliminados.
  //3- Por "factor de redondez": se permiten aquellos con forma cuasi-circular. Es ideal para eliminar restos de pulmón
  //   por tener contornos tortuosos y agujeros en su interior (lo que aumenta el perímetro y disminuye el área simultáneamente)

  float umbraltamanio = 10.0;
  float umbralMenor = 200.0;

  //Umbral de tamaño:
  //Búsqueda de los grupos con mayor tamaño:
  TList* ConnectedComponent;

  int maxSize = 0;
  int posMaxSize = 0;

  for(int i=0; i<ConnectedComponentList->Count; i++)
  {
   ConnectedComponent = (TList*)ConnectedComponentList->Items[i];

   if(ConnectedComponent->Count > maxSize)
   {
   maxSize = ConnectedComponent->Count;
   posMaxSize = i;
   }
  }

   ProgressBar1->StepIt();
   ProcessLabel->Caption = "Segmenting Trachea: connected components voting";
   ProcessLabel->Refresh();

  for(int i=0; i<ConnectedComponentList->Count; i++)
  {
   ConnectedComponent = (TList*)ConnectedComponentList->Items[i];

   paux = (Punto*)ConnectedComponent->First();

   //Elimina los elementos conexos plano x plano que sean mayores a cierto umbral del
   //máximo grupo encontrado, o cuyos pixels tengan un color por encima del threshold elegido
   if(ConnectedComponent->Count>maxSize/umbraltamanio || paux->color > umbralVol || ConnectedComponent->Count<umbralMenor)
   {
       for(int j=0; j<ConnectedComponent->Count; j++)
       {
       paux = (Punto*)ConnectedComponent->Items[j];
    //   M->SetV(paux->x, paux->y, paux->z,minPX/2+1); //Pinta el perimetro
       delete paux;
       }

       ConnectedComponent->Count = 0;
       ConnectedComponent->Clear();
   }
   else //De los componentes conexos sobrevivientes, se eliminan aquellos no "redondos"
   {
       for(int j=0; j<ConnectedComponent->Count; j++)
       {
         paux = (Punto*)ConnectedComponent->Items[j];   //Pone a los elementos conexos candidatos
         MAux->SetV(paux->x, paux->y, paux->z, 1);      //en 1, para poder ver su perímetro.
       }

       perim = Perimetro(ConnectedComponent, MAux);
       FR = float(ConnectedComponent->Count)/pow(perim,2.0);

       if(FR < umbralredondez*tolerancia)
       {
         for(int j=0; j<ConnectedComponent->Count; j++) //Elimina los elementos conexos que no son candidatos a ser traquea,
         {                                              //y los descarta como futuros candidatos a ser unidos por un algoritmo de
         paux = (Punto*)ConnectedComponent->Items[j];   //conexión de componentes en 3D. MAux = 0
      //   M->SetV(paux->x, paux->y, paux->z,minPX+1);
         MAux->SetV(paux->x, paux->y, paux->z, 0);
         delete paux;
         }
       }
       else
       {
         for(int j=0; j<ConnectedComponent->Count; j++) //Elimina los elementos conexos que son candidatos a ser traquea,
         {                                              //y los ubica como futuros candidatos a ser unidos por un algoritmo de
         paux = (Punto*)ConnectedComponent->Items[j];   //conexión de componentes en 3D. MAux = 1
         MAux->SetV(paux->x, paux->y, paux->z, 1);
         delete paux;
         }
       }

       ConnectedComponent->Count = 0;
       ConnectedComponent->Clear();

       delete ConnectedComponent;
   }
  }

  //Limpia la lista de elementos conexos plano x plano (previamente vaciada de puntos)
  //para utilizarla en la búsqueda de componentes conexos en 3D:
  ConnectedComponentList->Clear();

  //****************************************************************************
  //Segundo: Búsqueda de los componentes conexos en 3D de mayor tamaño en el
  //         tercio superior de la imagen:

   ProgressBar1->StepIt();
   ProcessLabel->Caption = "Segmenting Trachea: 3D-connected components";
   ProcessLabel->Refresh();

   ConnectedComponentLabelling(MAux, Maux, ConnectedComponentList, umbralVol, false); //false = busca volumétricamente

  float Energia;
  float maxEnergia = 0;
  float posMaxEnergia = 0;


   ProgressBar1->StepIt();
   ProcessLabel->Caption = "Segmenting Trachea: connected components voting";
   ProcessLabel->Refresh();

  //Pondera mejor los puntos ubicados en el tercio superior de la imagen:
  float* wZ = new float[cantImagenes];

  for(int i=0; i<cantImagenes/3; i++)
    wZ[i] = 1.0;

  for(int i=cantImagenes/3; i<cantImagenes; i++)
    wZ[i] = -3.0/float(cantImagenes)*float(i-cantImagenes/3)+1.0;


  //Pondera mejor los puntos ubicados en el tercio central de la imagen:
  float* wX = new float[anchoPX];

  for(int i=0; i<anchoPX/3; i++)
    wX[i] = 6.0/float(anchoPX)*float(i)-1.0;

  for(int i=anchoPX/3; i<2*anchoPX/3; i++)
    wX[i] = 1.0;

  for(int i=2*anchoPX/3; i<anchoPX; i++)
    wX[i] = -6.0/float(anchoPX)*float(i-2.0*anchoPX/3.0)+1.0;

  float* wY = new float[altoPX];

  for(int i=0; i<altoPX/3; i++)
    wY[i] = 6.0/float(altoPX)*float(i)-1.0;

  for(int i=altoPX/3; i<2*altoPX/3; i++)
    wY[i] = 1.0;

  for(int i=2*altoPX/3; i<altoPX; i++)
    wY[i] = -6.0/float(altoPX)*float(i-2.0*altoPX/3.0)+1.0;

  float cgx, cgy, cgz;
  float norma;

  float exponente = 1.5; //Asigna un peso a la norma, para ponderar mal aquellos puntos demasiado lejanos:

  for(int i=0; i<ConnectedComponentList->Count; i++)
  {
   ConnectedComponent = (TList*)ConnectedComponentList->Items[i];

  //Cálculo de los CG:
  CentroGravedad(ConnectedComponent, cgx, cgy, cgz);
  norma = sqrt( pow(cgx-anchoPX/2.0,2.0) + pow(cgy-altoPX/2.0,2.0) + pow(cgz-cantImagenes/2.0,2.0) );

  //La energía calcula la cantidad de puntos, ponderados por su posición en el volumen
  Energia = CalculoEnergia(ConnectedComponent, wX, wY, wZ);

  //Ayuda a eliminar los elementos de mucha energía, pero que están a una gran distancia del centro de la imagen
  //(ej: aire entre el paciente y la camilla y/o aire debajo de la camilla)
  Energia = Energia/pow(norma,exponente);

  //Se buca el elemento de mayor energía:
   if(Energia > maxEnergia)
   {
   maxEnergia = Energia;
   posMaxEnergia = i;
   }
  }

  //Si la heurística funcionó adecuadamente, el elemento de mayor energía corresponde
  //a la traquea o a una porción lo suficientemente apreciable de la misma como para poder encontrar la carina.
  TraqueaList = (TList*)ConnectedComponentList->Items[posMaxEnergia];

  TList* ListaAux;

  //Eliminación del resto de los elementos conexos que no son detectados como traquea:
  for(int i=0; i<posMaxEnergia; i++)
  {
  ListaAux = (TList*)ConnectedComponentList->Items[i];

        for(int j=0; j<ListaAux->Count; j++)
        {
        paux = (Punto*)ListaAux->Items[j];
        delete paux;
        }
  delete ListaAux;

  ListaAux = NULL;
  }


  for(int i=posMaxEnergia+1; i<ConnectedComponentList->Count; i++)
  {
  ListaAux = (TList*)ConnectedComponentList->Items[i];

        for(int j=0; j<ListaAux->Count; j++)
        {
        paux = (Punto*)ListaAux->Items[j];
        delete paux;
        }
  delete ListaAux;

  ListaAux = NULL;
  }

  ConnectedComponentList->Items[posMaxEnergia] = NULL;
  ConnectedComponentList->Clear();

//Fin de la identificación de traquea*********************************

   ProgressBar1->StepIt();
   ProcessLabel->Caption = "Trachea segmentation complete: searching for carina";
   ProcessLabel->Refresh();

//Paso 2: Identificación de la Carina:

//Auxiliares para identificación del centro de la carina
int zC = cantImagenes; int xC = 0; int yC = 0;

float distanciaC = 0;
float minDistanciaC = anchoPX;

//Busqueda del seed-point de la carina: es el punto más alto de la tráquea, y además
//el más centrado, para evitar que puntos de bronquios que pudieran alcanzar el corte más alto
//sean tomados como seed-point:

MAux->LlenarTodo(0);

  for(int j=0; j<TraqueaList->Count; j++)
       {
       paux = (Punto*)TraqueaList->Items[j];

       MAux->SetV(paux->x, paux->y, paux->z, 1); //Setea la matriz auxiliar en 1 en la traquea, para volver a ver plano x plano:
       distanciaC = sqrt(pow(paux->x-anchoPX/2,2.0)+pow(paux->y-altoPX/2,2.0));
       if(paux->z<=zC && distanciaC<minDistanciaC)
       {
       xC = paux->x;
       yC = paux->y;
       zC = paux->z;

       minDistanciaC = distanciaC;
       }

       delete paux;
       }

  //Encontrar ComponentesConexos corte a corte desde la semilla, y considerar todos los puntos siguientes como semilla.
  //Cuando se encuentre más de un elemento conexo, es porque se bifurcó la traquea:


  Punto* carinaSeedPoint = new Punto;
  carinaSeedPoint->x = xC; carinaSeedPoint->y = yC; carinaSeedPoint->z = zC;
  carinaSeedPoint->color = Maux->GetV(xC, yC, zC);

  ConnectedComponent = new TList;

  getAllMatrixPlaneConnected(carinaSeedPoint, MAux, Maux, ConnectedComponent, umbralVol);

  int tamanioActual = ConnectedComponent->Count;

  int tamanioFinal = tamanioActual;

  int cantidadDeComponentes = 0;

  int zActual = 0;

  float tamanioBronquio1 = 0;
  float tamanioBronquio2 = 0;

  float umbralCV = 1.5;

  TList* NextConnectedComponent = new TList;

  Punto* pnew;

  while(zActual < cantImagenes-2)
  {

   while(ConnectedComponent->Count>0)
   {

    paux = (Punto*)ConnectedComponent->First();

    zActual = paux->z;

    paux->z++;

  //  M->SetV(paux->x, paux->y, paux->z, minPX);

    if(MAux->GetV(paux->x, paux->y, paux->z)==0)
    {
    ConnectedComponent->Delete(0);
    continue;
    }

    tamanioActual = NextConnectedComponent->Count;

    getAllMatrixPlaneConnected(paux, MAux, Maux, NextConnectedComponent, umbralVol);

    CentroGravedad(NextConnectedComponent, cgx, cgy, cgz);

     if(NextConnectedComponent->Count > tamanioActual)
     {
     cantidadDeComponentes++;
     zC = paux->z;

     xCarina = cgx;
     yCarina = cgy;
     zCarina = cgz;

       if(cantidadDeComponentes == 1)
       tamanioBronquio1 = NextConnectedComponent->Count - tamanioActual;

       if(cantidadDeComponentes == 2)
       tamanioBronquio2 = NextConnectedComponent->Count - tamanioBronquio1;

     }

     ConnectedComponent->Delete(0);
   }

    if(cantidadDeComponentes==2)
    {
    //La bifurcación encontrada debe tener dos elementos no demasiado desiguales, para evitar que
    //se detecte como bifurcación alguna pequeña región de ruido que se desprende del camino principal
    //de la traquea:
    float variacion = 2.0*fabs(tamanioBronquio1-tamanioBronquio2)/(tamanioBronquio1+tamanioBronquio2);

    if(variacion<umbralCV)
    break;
    }
    else
    {
    cantidadDeComponentes = 0;
    }

    if(NextConnectedComponent->Count==0)
    break;

    for(int i=0; i<NextConnectedComponent->Count; i++)
    {
    paux = new Punto;
    pnew = (Punto*)NextConnectedComponent->Items[i];

    paux->x = pnew->x; paux->y = pnew->y; paux->z = pnew->z;

    ConnectedComponent->Add(paux);

    delete pnew;
    }

    NextConnectedComponent->Clear();

 }

   ProgressBar1->StepIt();
   ProcessLabel->Caption = "Carina detection complete: positioned 1cm below";
   ProcessLabel->Refresh();

   ProgressBar1->Position = 0;

  zCarina = zC+4.0; //1cm por debajo de la carina;

  //Encontró la Carina! Ubica la vista en la posición debajo de la carina!
  SliceListBox->ItemIndex =  zCarina;
  ImageScrollBar->Position = zCarina;




}
//---------------------------------------------------------------------------
void __fastcall TForm1::CarinaButtonClick(TObject *Sender)
{
//Pone la vista donde corresponde:
TabSheet->ActivePage = ImagenSheet;
TabSheet->OnChange(Sender);

//Realiza los cálculos en un Volumen Auxiliar
Mauxiliar = new Volumen(anchoPX, altoPX, cantImagenes);
Mvisitado = new Volumen(anchoPX, altoPX, cantImagenes);

for(int k=0; k<M->getProf();k++)
        for(int i=0; i<M->getAncho(); i++)
                for(int j=0; j<M->getAlto(); j++)
                {
                Mauxiliar->SetV(i,j,k,M->GetV(i,j,k));
                }
//Maux es un clon de M, para no arruinar los niveles de gris!
//No confundir con MAux, que es una matriz auxiliar creada para la detección de
//contornos y componentes conexos.

//Pre-procesamiento: eliminación de los bordes de la TC, para que no
  //interfieran con la caja pulmonar:

  Mvisitado->LlenarTodo(1); //LLena la matriz de 1's: o sea, ningún pixel fue recorrido

  //1º Detección de Carina.

  DetectCarina(Mauxiliar, Mvisitado);

  carinaDetectada = true;

  CarinaButton->Enabled = false;

  CACDButton->Enabled = true;

  if(AutoCheckBox->Checked)
  CACDButtonClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DetectCACD(Volumen* Maux, Volumen* MAux)
{
//Búsqueda de los puntos semilla CA  y CD:

  //Auxiliares:
  Punto* paux;

  //Region de Aorta Descendente:
  x0=xCarina;
  xf=xCarina+100;
  y0=yCarina;
  yf=yCarina+150;

  if(x0<0)
  x0=0;
  if(xf>anchoPX-1)
  xf=altoPX-1;
  if(y0<0)
  y0=0;
  if(yf>altoPX-1)
  yf=altoPX-1;

  //Region de Aorta Ascendente:
  x02=xCarina-175;
  xf2=xCarina;
  y02=yCarina-75;
  yf2=yCarina+100;


  if(x02<0)
  x02=0;
  if(xf2>anchoPX-1)
  xf2=altoPX-1;

  if(y02<0)
  y02=0;
  if(yf2>altoPX-1)
  yf2=altoPX-1;

  //Ventana de la aorta:
  short int t0 = 0;
  short int tf = 200;

  ProgressBar1->Max = 5;
  ProcessLabel->Caption = "";
  ProcessLabel->Refresh();
  ProcessLabel->Repaint();

  ImagenSheet->Repaint();

  ProcessLabel->Caption = "Bilateral Filtering";
  ProcessLabel->Refresh();

  //Filtro Bilateral (Aplicar, sin llamar a la ventana, en Maux):
  FiltroBilateral(Maux);

  ProgressBar1->StepIt();

  ProcessLabel->Caption = "High-pass Filtering";
  ProcessLabel->Refresh();

  for(int i=0;i<anchoPX;i++)
   for(int j=0;j<altoPX;j++)
    MAux->SetV(i,j,zCarina+1, Maux->GetV(i,j,zCarina+1));

  //Filtro Pasa Altos:
  FiltroPasaAltos(Maux);

  ProgressBar1->StepIt();

  ProcessLabel->Caption = "Binarization";
  ProcessLabel->Refresh();

  //Binarizacion!
  for(int i=0;i<anchoPX;i++)
   for(int j=0;j<altoPX;j++)
    if( Maux->GetV(i,j,zCarina+1) > 5)
    Maux->SetV(i,j,zCarina+1,maxPX);
    else
    Maux->SetV(i,j,zCarina+1,minPX);

  ProgressBar1->StepIt();
  ProcessLabel->Caption = "Circular Hough transform";
  ProcessLabel->Refresh();

  //Hough-Space: volumen que almacenará un puntaje en el espacio de Hough:
    int r0 = 22;
    int rf = 47;

  Volumen* HoughSpace = new Volumen(anchoPX, altoPX, rf-r0);

  TList* Circle = new TList;

  float contador = 0;

  //Búqueda del punto semilla dentro de la primer ROI:
  for(int r=r0; r<rf; r++)
  {
    //Crea el círculo:
    BressenhamCircle(0, 0, 0, r, Circle);

     //Mira los puntos dentro de la primer region de interés:
     for(int i=x0; i<xf; i++)
       for(int j=y0; j<yf; j++)
       {

      if(Maux->GetV(i,j,zCarina+1) == maxPX) //Si encontró un punto de contorno...
          for(int s=0; s<Circle->Count; s++) //...recorre la circunferencia creada para acumular.
            {
            paux = (Punto*)Circle->Items[s];

              if( (i+paux->x)>=x0 && (i+paux->x)<=xf && (j+paux->y)>=y0 && (j+paux->y)<=yf) //Si el punto cae dentro de los límites de la ROI...
              {
             // if(MAux->GetV(i+paux->x,j+paux->y,zCarina+1) >t0 && MAux->GetV(i+paux->x,j+paux->y,zCarina+1)<tf)
              HoughSpace->SetV(i+paux->x,j+paux->y,r-r0 ,HoughSpace->GetV(i+paux->x,j+paux->y,r-r0)+1); //acumula.
              }
            }
      }

    //Elimina el círculo:
    for(int s=0; s<Circle->Count; s++)
    {
    paux = (Punto*)Circle->Items[s];
    delete paux;
    }
    Circle->Clear();
  }


  x02--; y02++; xf2--; yf2--;

  //Búqueda del punto semilla dentro de la segunda ROI:
  for(int r=r0; r<rf; r++)
  {
    //Crea el círculo:
    BressenhamCircle(0, 0, 0, r, Circle);

     //Mira los puntos dentro de la segunda region de interés:
     for(int i=x02; i<xf2; i++)
       for(int j=y02; j<yf2; j++)
       {

      if(Maux->GetV(i,j,zCarina+1) == maxPX)
          for(int s=0; s<Circle->Count; s++) //...recorre la circunferencia creada.
            {
            paux = (Punto*)Circle->Items[s];
            //Si cae dentro de los límites de la VOI:

            if( (i+paux->x)>=x02 && (i+paux->x)<=xf2 && (j+paux->y)>=y02 && (j+paux->y)<=yf2) //Si el punto cae dentro de los límites de la ROI...
              {

            //  if(MAux->GetV(i+paux->x,j+paux->y,zCarina+1) >t0 && MAux->GetV(i+paux->x,j+paux->y,zCarina+1)<tf)
              HoughSpace->SetV(i+paux->x,j+paux->y,r-r0 ,HoughSpace->GetV(i+paux->x,j+paux->y,r-r0)+1); //acumula.
              }

            }
        }

    //Elimina el círculo:
    for(int s=0; s<Circle->Count; s++)
    {
    paux = (Punto*)Circle->Items[s];
    delete paux;
    }
    Circle->Clear();
  }


  //Arreglo de puntajes dentro de los círculos:
  //Se fija la cantidad de puntos que cayeron dentro de los colores típicos de la aorta:
  //Los puntos que no caen dentro de los límites de la ventana aórtica en HU se consideran 2 veces, mientras
  //que los puntos que si caen se consideran solo una vez:

    ProgressBar1->StepIt();
  ProcessLabel->Caption = "Traversing Hough-Space: voting process";
  ProcessLabel->Refresh();

  float contadorSI = 0;
  float contadorNO = 0;
  float contadorRatio = 0;

  TList* Circ = new TList;
  TList* Der = new TList;
  TList* Izq = new TList;

  bool primeravuelta = true;


   for(int r=r0; r<rf*0.8; r++)
   {

   //Para un cierto radio...
   strel2D(r, Circ);
   strel2D_der(r, Der);
   strel2D_izq(r, Izq);

   //...recorro todos los puntos de la ROI...
  for(int j=y0; j<yf; j++)
  {
   primeravuelta = true;
   contadorSI = 0;
   contadorNO = 0;

       for(int i=x0; i<xf; i++)
       {

          //...y reviso dentro de la circunferencia completa si es la primera vuelta:
          if(primeravuelta)
          {
           for(int p=0; p<Circ->Count; p++)
           {
           paux = (Punto*)Circ->Items[p];

           if(MAux->GetV(i+paux->x,j+paux->y,zCarina+1)<t0 || MAux->GetV(i+paux->x,j+paux->y,zCarina+1)>tf)
           contadorNO=contadorNO+2;
           else contadorSI++;
           }
          primeravuelta = false;
          }
          else
          {
           for(int p=0; p<Izq->Count; p++)
           {
           paux = (Punto*)Izq->Items[p];

           if(MAux->GetV(i+paux->x,j+paux->y,zCarina+1)<t0 || MAux->GetV(i+paux->x,j+paux->y,zCarina+1)>tf)
           contadorNO=contadorNO-2;
           else contadorSI--;
           }

           for(int p=0; p<Der->Count; p++)
           {
           paux = (Punto*)Der->Items[p];

           if(MAux->GetV(i+paux->x,j+paux->y,zCarina+1)<t0 || MAux->GetV(i+paux->x,j+paux->y,zCarina+1)>tf)
           contadorNO=contadorNO+2;
           else contadorSI++;
           }

          }

         contadorRatio = contadorSI/(contadorSI+contadorNO);
         HoughSpace->SetV(i,j,r-r0, HoughSpace->GetV(i,j,r-r0)*contadorRatio);
       }


   }


    //Elimino los círculos y fragmentos de círculo:
    for(int p=0; p<Circ->Count; p++)
    {
    paux = (Punto*)Circ->Items[p];
    delete paux;
    }
    Circ->Clear();

    for(int p=0; p<Der->Count; p++)
    {
    paux = (Punto*)Der->Items[p];
    delete paux;
    }
    Der->Clear();

    for(int p=0; p<Izq->Count; p++)
    {
    paux = (Punto*)Izq->Items[p];
    delete paux;
    }
    Izq->Clear();

  }


   //Arreglo de puntajes dentro de los círculos:
   for(int r=r0; r<rf; r++)
   {

   //Para un cierto radio...
   strel2D(r, Circ);
   strel2D_der(r, Der);
   strel2D_izq(r, Izq);

   //...recorro todos los puntos de la ROI...
  for(int j=y02; j<yf2; j++)
  {
   primeravuelta = true;
   contadorSI = 0;
   contadorNO = 0;

       for(int i=x02; i<xf2; i++)
       {

          //...y reviso dentro de la circunferencia completa si es la primera vuelta:
          if(primeravuelta)
          {
           for(int p=0; p<Circ->Count; p++)
           {
           paux = (Punto*)Circ->Items[p];

           if(MAux->GetV(i+paux->x,j+paux->y,zCarina+1)<t0 || MAux->GetV(i+paux->x,j+paux->y,zCarina+1)>tf)
           contadorNO=contadorNO+2;
           else contadorSI++;
           }
          primeravuelta = false;
          }
          else
          {
           for(int p=0; p<Izq->Count; p++)
           {
           paux = (Punto*)Izq->Items[p];

           if(MAux->GetV(i+paux->x,j+paux->y,zCarina+1)<t0 || MAux->GetV(i+paux->x,j+paux->y,zCarina+1)>tf)
           contadorNO=contadorNO-2;
           else contadorSI--;
           }

           for(int p=0; p<Der->Count; p++)
           {
           paux = (Punto*)Der->Items[p];

           if(MAux->GetV(i+paux->x,j+paux->y,zCarina+1)<t0 || MAux->GetV(i+paux->x,j+paux->y,zCarina+1)>tf)
           contadorNO=contadorNO+2;
           else contadorSI++;
           }

          }

         contadorRatio = contadorSI/(contadorSI+contadorNO);
         HoughSpace->SetV(i,j,r-r0, HoughSpace->GetV(i,j,r-r0)*contadorRatio);
       }


   }


    //Elimino los círculos y fragmentos de círculo:
    for(int p=0; p<Circ->Count; p++)
    {
    paux = (Punto*)Circ->Items[p];
    delete paux;
    }
    Circ->Clear();

    for(int p=0; p<Der->Count; p++)
    {
    paux = (Punto*)Der->Items[p];
    delete paux;
    }
    Der->Clear();

    for(int p=0; p<Izq->Count; p++)
    {
    paux = (Punto*)Izq->Items[p];
    delete paux;
    }
    Izq->Clear();

  }

  //Búsqueda de los máximos de ROI1 y ROI2:
  short int maximo1 = minPX-1;
  int centrox1,centroy1;
  int radiomaximo1;

  for(int r=r0; r<rf*0.8; r++)
   for(int i=x0; i<xf; i++)
       for(int j=y0; j<yf; j++)
       {
         if(HoughSpace->GetV(i,j,r-r0) > maximo1)
         {
         maximo1 =  HoughSpace->GetV(i,j,r-r0);
         centrox1 = i;
         centroy1 = j;
         radiomaximo1 = r;
         }
       }

  short int maximo2 = minPX-1;
  int centrox2,centroy2;
  int radiomaximo2;

  for(int r=r0; r<rf; r++)
   for(int i=x02; i<xf2; i++)
       for(int j=y02; j<yf2; j++)
       {
         if(HoughSpace->GetV(i,j,r-r0) > maximo2)
         {
         maximo2 =  HoughSpace->GetV(i,j,r-r0);
         centrox2 = i;
         centroy2 = j;
         radiomaximo2 = r;
         }
       }

   for(int i=0;i<anchoPX;i++)
    for(int j=0;j<altoPX;j++)
    {
     if(Maux->GetV(i,j,zCarina+1)==maxPX)
     Maux->SetV(i,j,zCarina+1, maxPX);
     else
     Maux->SetV(i,j,zCarina+1, MAux->GetV(i,j,zCarina+1));
    }

   BressenhamCircle(0, 0, 0, radiomaximo1, Circle);

    for(int s=0; s<Circle->Count; s++)
            {
            paux = (Punto*)Circle->Items[s];
           // M->SetV(centrox1+paux->x,centroy1+paux->y,zCarina+1, maxPX);
            delete paux;
            }

   Circle->Clear();

   BressenhamCircle(0, 0, 0, radiomaximo2, Circle);

    for(int s=0; s<Circle->Count; s++)
            {
            paux = (Punto*)Circle->Items[s];
           // M->SetV(centrox2+paux->x,centroy2+paux->y,zCarina+1, maxPX);
            delete paux;
            }

   Circle->Clear();
   delete Circle;

   xCA = centrox2;
   yCA = centroy2;
   rA  = radiomaximo2;

   xCD = centrox1;
   yCD = centroy1;
   rD = radiomaximo1;

   //CA & CD detectados!! Este paso es obviable?
   //Shape7: Rojo
        Shape7->Visible=true;
        RShape7->Visible=true;
        RShape7->Width=2*rD;
        RShape7->Height=2*rD;

        Shape7->Left = yCD - Shape7->Width/2;
        Shape7->Top  = xCD -Shape7->Height/2;

        RShape7->Left = yCD - RShape7->Width/2;
        RShape7->Top  = xCD - RShape7->Height/2;

   //Shape8: Azul
        Shape8->Visible=true;
        RShape8->Visible=true;
        RShape8->Width=2*rA;
        RShape8->Height=2*rA;

        Shape8->Left = yCA - Shape8->Width/2;
        Shape8->Top  = xCA -Shape8->Height/2;

        RShape8->Left = yCA - RShape8->Width/2;
        RShape8->Top  = xCA - RShape8->Height/2;

   ImageScrollBar->Position = zCarina;
  // CACWindowButtonClick(Form1); //Mejora la visualización de aorta en TC sin contraste
   GraficarEnPantalla();
   Form1->Refresh();
   Form1->Repaint();


   //Volver al original!!:

    //Prueba para pintar circunferencias de ajuste:
        float x0A = xCA;
        float y0A = yCA;
        float z0A = zCarina+1;
        float rCA = rA;

        //Prueba para pintar circunferencias de ajuste:
        float x0D = xCD;
        float y0D = yCD;
        float z0D = zCarina+1;
        float rCD = rD;


     //   return;

        //Ajuste Ascendente:
        TList* CircuList = new TList;
        strelCirc2D(rA,x0A,y0A,z0A,CircuList);
        PuntoF* pa;
        float cont = 0;
        double cont2=0;

        for(int i=0; i<CircuList->Count; i++)
        {
                pa = (PuntoF*)CircuList->Items[i];
                cont += M->GetV(pa->x, pa->y, pa->z);
                cont2 +=  M->GetV(pa->x, pa->y, pa->z)*M->GetV(pa->x, pa->y, pa->z);
                delete pa;
        }

        float Nn = float(CircuList->Count);
        float mu = cont/Nn;
        float sigma = sqrt(cont2/Nn -mu*mu);

        //Parámetros de inicialización de condiciones iniciales del espacio SIMPLX:
        float usual_delta  = 0.01;
        float zero_delta = 0.0025;

        //Cálculo de las Condiciones Iniciales:
        //p0: Vector de condiciones iniciales: p0 = {x0, y0, theta0, rA, rB}
        float p0[3];

        //Valores inciales del Simplex:
        p0[0]=x0A;
        p0[1]=y0A;
        p0[2]= rCA/2.0;

        //p: Vector de vértices del subespacio SIMPLEX: p es de tamaño 6x5:
        //4=número de vértices, 3=cant de coeficientes a evaluar por vértice (x0,y0,r).
        float **p;

        p=new float*[4];

        for(int i=0;i<4;i++)
        {
        p[i]=new float[3];
                for(int j=0; j<3; j++)
                p[i][j]=p0[j];
        }

        p[0][0] = x0A;
        p[0][1] = y0A;
        p[0][2] = rCA/2.0;


        p[1][0] = x0A+5.0;
        p[1][1] = y0A;
        p[1][2] = rCA/2.0;

        p[2][0] = x0A;
        p[2][1] = y0A-5.0;
        p[2][2] = rCA/2.0;

        p[3][0] = x0A;
        p[3][1] = y0A;
        p[3][2] = rCA/2.0+5.0;

        int nfunc;

        //inicializa el subespacio SIMPLEX con vértices 'y':
        float y[4];

        //Circunferencia irrestricta, no obedece una ROI:
        y[0]=funkCircI(p[0], mu,sigma, z0A, M, x0A, y0A);
        y[1]=funkCircI(p[1], mu,sigma, z0A, M, x0A, y0A);
        y[2]=funkCircI(p[2], mu,sigma, z0A, M, x0A, y0A);
        y[3]=funkCircI(p[3], mu,sigma, z0A, M, x0A, y0A);

        amoebaCircI(p, y, 3, 1e-10,&nfunc,mu,sigma,z0A, M, x0A, y0A);
        CircuList->Clear();
        strelCirc2DContorno(p[0][2],p[0][0],p[0][1],z0A,CircuList);
        for(int i=0; i<CircuList->Count; i++)
        {
                pa = (PuntoF*)CircuList->Items[i];
                //M->SetV(pa->x, pa->y, pa->z, maxPX);
                delete pa;
        }
        CircuList->Clear();

        //Ajuste Descendente:
        strelCirc2D(rD,x0D,y0D,z0D,CircuList);

        cont = 0;
        cont2=0;

        for(int i=0; i<CircuList->Count; i++)
        {
                pa = (PuntoF*)CircuList->Items[i];
                cont += M->GetV(pa->x, pa->y, pa->z);
                cont2 +=  M->GetV(pa->x, pa->y, pa->z)*M->GetV(pa->x, pa->y, pa->z);
                delete pa;
        }

        Nn = float(CircuList->Count);
        mu = cont/Nn;
        sigma = sqrt(cont2/Nn -mu*mu);

        //Valores inciales del Simplex:
        p0[0]=x0D;
        p0[1]=y0D;
        p0[2]= rCD/2.0;


        for(int i=0;i<4;i++)
                for(int j=0; j<3; j++)
                        p[i][j]=p0[j];

        p[0][0] = x0D;
        p[0][1] = y0D;
        p[0][2] = rCD/2.0;

        p[1][0] = x0D+5.0;
        p[1][1] = y0D;
        p[1][2] = rCD/2.0;

        p[2][0] = x0D;
        p[2][1] = y0D-5.0;
        p[2][2] = rCD/2.0;

        p[3][0] = x0D;
        p[3][1] = y0D;
        p[3][2] = rCD/2.0+5.0;

        int nfunc2;


        y[0]=funkCircI(p[0], mu,sigma, z0D, M, x0D, y0D);
        y[1]=funkCircI(p[1], mu,sigma, z0D, M, x0D, y0D);
        y[2]=funkCircI(p[2], mu,sigma, z0D, M, x0D, y0D);
        y[3]=funkCircI(p[3], mu,sigma, z0D, M, x0D, y0D);

        amoebaCircI(p, y, 3, 1e-10,&nfunc2,mu,sigma,z0D, M, x0D, y0D);

        CircuList->Clear();
        strelCirc2DContorno(p[0][2],p[0][0],p[0][1],z0D,CircuList);
        for(int i=0; i<CircuList->Count; i++)
        {
                pa = (PuntoF*)CircuList->Items[i];
                //M->SetV(pa->x, pa->y, pa->z, maxPX);
                delete pa;
        }
        CircuList->Clear();
        delete CircuList;

        GraficarEnPantalla();


  ProgressBar1->StepIt();
  ProcessLabel->Caption = "CA/CD CSAs detected";
  ProcessLabel->Refresh();

  ProgressBar1->Position = 0;

}
//------------------------------------------------------------------------------

void __fastcall TForm1::FiltroBilateral(Volumen* Maux)
{

     //   AnsiString sigmaSString; //desvio de la Gaussiana espacial
     //   AnsiString sigmaIString; //Desvío de la Gaussiana de intensidad
     //   AnsiString cantIterString; //cantidad de aplicaciones del filtro
     //   AnsiString mascaraString; //tamaño del Kernel del filtro

        double bilateral = 0;
        double valor = 0;
        double valornorm = 0;
        double sigmaS = 1.7;
        double sigmaI = 10.0;
        int cantIter = 1;
        int offset = 3;

       // if(BilateralForm->ShowModal()==mrOk)
       // {
     //   sigmaSString = BilateralForm->SigmaSEdit->Text;
     //   sigmaS=atof(sigmaSString.c_str());

     //   sigmaIString = BilateralForm->SigmaIEdit->Text;
     //   sigmaI=atof(sigmaIString.c_str());

     //   cantIterString = BilateralForm->NumIterEdit->Text;
     //   cantIter=atoi(cantIterString.c_str());

      //  mascaraString = BilateralForm->MascEdit->Text;
     //   offset=atoi(mascaraString.c_str());

       // }else return;

    double* mascara = new double[offset*offset]; //máscara 2D linealizada
    double* deltaint = new double[maxPX-minPX];// vector de diferencias de intensidad

    int n=offset-1;

    int l=0;

    for(double x=-n/2; x<=n/2; x++)
       for(double y=-n/2; y<=n/2; y++)
        {
        mascara[l]=exp((-x*x-y*y)/2.0/sigmaS/sigmaS);
        l++;
        }

    for(l=0; l<maxPX-minPX; l++)
      deltaint[l]=exp(-double(l*l)/2.0/sigmaI/sigmaI);


    //Bilateral en VOI1:
    for(int veces = 0; veces<cantIter; veces++)
    {

        //AnsiString vez;
        //vez = "Iteración #: ";
        //ProcessLabel->Caption = vez + (veces+1);
        //ProcessLabel->Refresh();

        for(int i=x0; i<xf; i++)
           for(int j=y0; j<yf; j++)
               auxArray[i][j]=minPX;

        for (int k=zCarina+1;k<zCarina+2;k++)
        {
       // ProgressBar1->Position=k;
        for(int i=x0; i<xf;i++)
                 for(int j=y0; j<yf;j++)
                        {

                        bilateral = 0;
                        valornorm = 0;
                        l=0;
                        for(int x=-n/2; x<=n/2; x++)
                         for(int y=-n/2; y<=n/2; y++)
                          {
                          valor=mascara[l];
                          valor*=deltaint[abs(Maux->GetV(i,j,k)-Maux->GetV(i+x,j+y,k))];
                          valornorm+=valor;
                          valor*=double(Maux->GetV(i+x,j+y,k));
                          bilateral+=valor;
                          l++;
                          }
                        if(bilateral/valornorm>maxPX)
                        bilateral = maxPX*valornorm;
                        if(bilateral/valornorm<minPX)
                        bilateral = minPX*valornorm;
                        auxArray[i][j]= bilateral/valornorm;
                        }
            for(int i=x0; i<xf;i++)
                 for(int j=y0; j<yf;j++)
                     Maux->SetV(i,j,k,auxArray[i][j]);
        }

        //ProgressBar1->Position=0;

    //    GraficarEnPantalla();

    }//veces


    //Bilateral en VOI2:
    for(int veces = 0; veces<cantIter; veces++)
    {

       // AnsiString vez;
       // vez = "Iteración #: ";
       // ProcessLabel->Caption = vez + (veces+1);
       // ProcessLabel->Refresh();

        for(int i=x02; i<xf2; i++)
           for(int j=y02; j<yf2; j++)
               auxArray[i][j]=minPX;

        for (int k=zCarina+1;k<zCarina+2;k++)
        {
       // ProgressBar1->Position=k;
        for(int i=x02; i<xf2;i++)
                 for(int j=y02; j<yf2;j++)
                        {

                        bilateral = 0;
                        valornorm = 0;
                        l=0;
                        for(int x=-n/2; x<=n/2; x++)
                         for(int y=-n/2; y<=n/2; y++)
                          {
                          valor=mascara[l];
                          valor*=deltaint[abs(Maux->GetV(i,j,k)-Maux->GetV(i+x,j+y,k))];
                          valornorm+=valor;
                          valor*=double(Maux->GetV(i+x,j+y,k));
                          bilateral+=valor;
                          l++;
                          }
                        if(bilateral/valornorm>maxPX)
                        bilateral = maxPX*valornorm;
                        if(bilateral/valornorm<minPX)
                        bilateral = minPX*valornorm;
                        auxArray[i][j]= bilateral/valornorm;
                        }
            for(int i=x02; i<xf2;i++)
                 for(int j=y02; j<yf2;j++)
                     Maux->SetV(i,j,k,auxArray[i][j]);
        }

       // ProgressBar1->Position=0;

     //   GraficarEnPantalla();

    }//veces


        delete []mascara;
        delete []deltaint;

      //  ProcessLabel->Caption = "ProcessLabel";
}
//------------------------------------------------------------------------------

void __fastcall TForm1::FiltroPasaAltos(Volumen* Maux)
{
 int zf=zCarina+2;
     int zi=zCarina+1;
     int cantCortes = zf-zi;

     int xi = 2;
     int xf = anchoPX-2;
     int yi = 2;
     int yf = altoPX-2;

      //Máscara básica PasaAltos:
        float msc[3][3][3] = {
                           {{0,0,0},{0,0,0},{0,0,0}},
                           {{-1,-1,-1},{-1,8,-1},{-1,-1,-1}},
                           {{0,0,0},{0,0,0},{0,0,0}}
                           };

     Volumen* auxVol = new Volumen(xf-xi,yf-yi,zf-zi);

   //  ProgressBar1->Max=zf-zi;

        float valor=0;

        for (int k=zi;k<zf;k++)
       {
     //   ProgressBar1->Position=k-zi-2;
        for(int i=xi+2; i<xf-2;i++)
                 for(int j=yi+2; j<yf-2;j++)
                        {
                        valor = (
                        Maux->GetV(i-1,j-1,k-1)*msc[0][0][0] +
                        Maux->GetV(i,j-1,k-1)*msc[0][0][1]   +
                        Maux->GetV(i+1,j-1,k-1)*msc[0][0][2] +
                        Maux->GetV(i-1,j,k-1)*msc[0][1][0]   +
                        Maux->GetV(i,j,k-1)*msc[0][1][1]     +
                        Maux->GetV(i+1,j,k-1)*msc[0][1][2]   +
                        Maux->GetV(i-1,j+1,k-1)*msc[0][2][0] +
                        Maux->GetV(i,j+1,k-1)*msc[0][2][1]   +
                        Maux->GetV(i+1,j+1,k-1)*msc[0][2][2] +

                        Maux->GetV(i-1,j-1,k)*msc[1][0][0]   +
                        Maux->GetV(i,j-1,k)*msc[1][1][0]     +
                        Maux->GetV(i+1,j-1,k)*msc[1][2][0]   +
                        Maux->GetV(i-1,j,k)*msc[1][0][1]     +
                        Maux->GetV(i,j,k)*msc[1][1][1]       +
                        Maux->GetV(i+1,j,k)*msc[1][2][1]     +
                        Maux->GetV(i-1,j+1,k)*msc[1][0][2]   +
                        Maux->GetV(i,j+1,k)*msc[1][1][2]     +
                        Maux->GetV(i+1,j+1,k)*msc[1][2][2]   +

                        Maux->GetV(i-1,j-1,k+1)*msc[2][0][0] +
                        Maux->GetV(i,j-1,k+1)*msc[2][0][1]   +
                        Maux->GetV(i+1,j-1,k+1)*msc[2][0][2] +
                        Maux->GetV(i-1,j,k+1)*msc[2][1][0]   +
                        Maux->GetV(i,j,k+1)*msc[2][1][1]     +
                        Maux->GetV(i+1,j,k+1)*msc[2][1][2]   +
                        Maux->GetV(i-1,j+1,k+1)*msc[2][2][0] +
                        Maux->GetV(i,j+1,k+1)*msc[2][2][1]   +
                        Maux->GetV(i+1,j+1,k+1)*msc[2][2][2])/9;

                        auxVol->SetV(i-xi-2, j-yi-2, k-zi, valor);
                        }
        }

     //  ProgressBar1->Position=0;

       for (int k=zi;k<zf;k++)
       {
    //   ProgressBar1->Position=k-zi-2;
        for(int i=xi+2; i<xf-2;i++)
                 for(int j=yi+2; j<yf-2;j++)
                 {
                 //M->SetV(i,j,k,auxVol->GetV(i-xi-2,j-yi-2,k-zi));
                 Maux->SetV(i,j,k,auxVol->GetV(i-xi-2,j-yi-2,k-zi));
                 }
        }

        delete auxVol;

     //   ProgressBar1->Position=0;


     //ApplyMasc333(msc, xi, xf, yi, yf, zi, zf);

    // GraficarEnPantalla();
}
//------------------------------------------------------------------------------

void __fastcall TForm1::CACDButtonClick(TObject *Sender)
{
        carinaDetectada = false;
        DetectCACD(Mauxiliar, Mvisitado);
        CACDDetectados = true;
        delete Mauxiliar;
        delete Mvisitado;
        Mauxiliar = NULL;
        Mvisitado = NULL;

        CACDButton->Enabled = false;
        CreateToroidButton->Enabled = true;

        if(AutoCheckBox->Checked)
        CreateToroidButtonClick(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Shape8MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

           //Movimiento del centro de la Aorta Ascendente:
          moviendoCACD=true;
          xP = X;
          yP = Y;
          SetCaptureControl(Pantalla);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Shape7MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
           //Movimiento del centro de la Aorta Ascendente:
          moviendoCACD=true;
          xP = X;
          yP = Y;
          SetCaptureControl(Pantalla);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CrearBaston(float xCA, float yCA, float xCD, float yCD)
{
   double vectorDA[3]; //Vector que une los puntos CA y CD

   vectorDA[0] = res[0]*(xCA-xCD);
   vectorDA[1] = res[1]*(yCA-yCD);
   vectorDA[2] = res[2]*(0);

   double AAW = sqrt(  vectorDA[0]*vectorDA[0] + vectorDA[1]*vectorDA[1] + vectorDA[2]*vectorDA[2] );
   double RadioToroide = AAW/2.0;

   //Normalización del vector DA:

   vectorDA[0] /= AAW;
   vectorDA[1] /= AAW;
   vectorDA[2] /= AAW;
   
   double theta, fi;

   theta = acos( -vectorDA[0] );

   //Matriz de Rotacion alrededor del eje Z: Rz = [c(t) -s(t) 0; s(t) c(t) 0; 0 0 1]
   //Pensando en un toroide ubicado sobre el plano XZ, centrado en el origen, el mismo
   //tiene la expresión (xT,yT,zT) = R(cos(fi), 0 , sin(fi))
   //Así, el toroide rotado theta grados para coincidir con la dirección de AAW, estará dado por la curva
   // C(fi) = Rz*(xT, yT, zT) = (R.cos(theta).cos(fi), R.sin(theta).cos(fi), R.sin(fi))

   // Los vectores tangentes al toroide, que serán las normales a los planos de muestreo, se caculan
   // como dC(fi)/dfi = (-R.cos(theta).sin(fi), -R.sin(theta).sin(fi), R.cos(fi))

   //Asi, el plano de muestreo para un punto del toroide en el ángulo fi0 tiene normal dC(fi0)/dfi y
   //estará centrado en C(fi0).

   //Para la creación de los cortes oblicuos, se utilizarán las estructuras auxiliares:
   //PlanoOrig[][]
   //PlanoCorte[][]
   //R[3][3]: matriz de rotación arbitraria

    //Grabación de la parte recta del bastón:
    int cantcortesaxiales = cantImagenes - (zCarina+1)-1;

    PuntoF* puntoAxial;

    for(int i=cantImagenes-1; i>zCarina+1; i--)
    {
    puntoAxial = new PuntoF;
    puntoAxial->x = res[0]*float(xCD);
    puntoAxial->y = res[1]*float(yCD);
    puntoAxial->z = res[2]*float(i);

    CurvaToroide->Add(puntoAxial);
    AortaCenterLine->Add(puntoAxial);

    puntoAxial = new PuntoF;
    puntoAxial->x = 0.0;
    puntoAxial->y = 0.0;
    puntoAxial->z = 1.0;
    NormalToroide->Add(puntoAxial);
    AortaNormales->Add(puntoAxial);
    }

   //Toroide de 180º, recorrido cada 2º:
   int angulomin = 0;
   int angulomax = 240;
   int pasoangular = 2;

    ToroideScrollBar->Min = 0;
    ToroideScrollBar->Max = cantcortesaxiales + angulomax/2-2;


   PuntoF* puntoToroide;

   for(int i=0; i<angulomax; i=i+pasoangular)
   {
        fi = -double(i)*M_PI/180.0;
        //fi = float(i)*M_PI/180.0;

        //Punto del toroide en coordenadas escaladas (reales):
        puntoToroide = new PuntoF;

        puntoToroide->x = RadioToroide*cos(theta)*cos(fi) + res[0]*(xCD+xCA)/2.0;
        puntoToroide->y = RadioToroide*sin(theta)*cos(fi) + res[1]*(yCD+yCA)/2.0;
        puntoToroide->z = RadioToroide*sin(fi) + res[2]*(zCarina+1);

        CurvaToroide->Add(puntoToroide);
        AortaCenterLine->Add(puntoToroide);

        //Vector normal al toroide punto a punto en coordenadas escaladas (reales):
        puntoToroide = new PuntoF;

        puntoToroide->x = -RadioToroide*cos(theta)*sin(fi);
        puntoToroide->y = -RadioToroide*sin(theta)*sin(fi);
        puntoToroide->z =  RadioToroide*cos(fi);

        NormalToroide->Add(puntoToroide);
        AortaNormales->Add(puntoToroide);
    }


}

//--------------------------------------------------------------------------

void __fastcall TForm1::CreateToroidButtonClick(TObject *Sender)
{
   //Crea el Bastón y los planos oblicuos Toroidales:

  ImagenSheet->Repaint();
  ProcessLabel->Caption = "Toroidal-Path Multi-Planar reconstruction";
  ProcessLabel->Refresh();

   xCA = Shape8->Top-Shape8->Height/2;
   yCA = Shape8->Left-Shape8->Width/2;

   xCD = Shape7->Top-Shape7->Height/2;
   yCD = Shape7->Left-Shape7->Width/2;

   AortaCenterLine = new TList;
   AortaNormales = new TList;

   CurvaToroide = new TList; //Guarda el centerline de la linea toroidal
   NormalToroide = new TList; //Guarda el vector tangente a la linea toroidal

   CrearBaston(xCA, yCA, xCD, yCD);

   isCenterLineDetected = true;

   //Creación de los planos Toroidales, ortogonales al Bastón, mediante Interpolación Trilineal:
int cantSlices = CurvaToroide->Count-1;
ProgressBar1->Max=cantSlices;

//Interpolación TRILIEAL:
//http://en.wikipedia.org/wiki/Trilinear_interpolation
float x,y,z;
float xd, yd, zd;
float xp, xt, yp, yt, zp, zt;
float i1, i2, j1, j2, w1, w2;
float valor;

//Creación de una matriz auxiliar, MToroide:
MToroide = new Volumen(M->getAncho(), M->getAlto(), cantSlices);
MToroide->LlenarTodo(minPX);

for(int k=0; k<cantSlices; k++)
{
ProgressBar1->Position=k;

vector1.x=0.0;
vector1.y=0.0;
vector1.z=1.0;

SetToroidalPlaneFromIndex(k);

    for (int i=anchoPX/5; i< 4*anchoPX/5; ++i)
        for (int j=altoPX/5; j<4*altoPX/5; ++j)
        {

         if(PlanoCorte[i][j].x<altoPX-1 && PlanoCorte[i][j].x>1 &&
           PlanoCorte[i][j].y<anchoPX-1 && PlanoCorte[i][j].y>1 &&
           PlanoCorte[i][j].z<cantImagenes-1 && PlanoCorte[i][j].z>1)
           {
           x = PlanoCorte[i][j].x;
           y = PlanoCorte[i][j].y;
           z = PlanoCorte[i][j].z;

           xp = floor(PlanoCorte[i][j].x);
           xt =  ceil(PlanoCorte[i][j].x);
           yp = floor(PlanoCorte[i][j].y);
           yt =  ceil(PlanoCorte[i][j].y);
           zp = floor(PlanoCorte[i][j].z);
           zt =  ceil(PlanoCorte[i][j].z);

           xd = x-xp;
           yd = y-yp;
           zd = z-zp;

           i1 = (M->GetV(xp,yp,zp)*(1.0-zd)+ M->GetV(xp,yp,zt)*zd);
           i2 = (M->GetV(xp,yt,zp)*(1.0-zd)+ M->GetV(xp,yt,zt)*zd);
           j1 = (M->GetV(xt,yp,zp)*(1.0-zd)+ M->GetV(xt,yp,zt)*zd);
           j2 = (M->GetV(xt,yt,zp)*(1.0-zd)+ M->GetV(xt,yt,zt)*zd);
           w1 = i1*(1.0-yd)+i2*yd;
           w2 = j1*(1.0-yd)+j2*yd;

           valor = (w1*(1.0-xd)+w2*xd);

           MToroide->SetV(i,j,k, valor);
           }
           else{MToroide->SetV(i,j,k, minPX);}
         }

   ProgressBar1->Position=0;
   }

//Resultado de esta etapa, MToroide posee todos los planos Toroidales.

//Encontrar el primer punto semilla para el algoritmo AortaCavityFill:
//Lógica: se realiza un AortaCavityFill hacia abajo de CD, buscando el último punto
//que coincida con no irse demasiado lejos del punto encontrado en el paso anterior:

CreateToroidButton->Enabled = false;

SegmentDAButton->Enabled = true;

TabSheet->ActivePage = ROISheet;

TabSheetChange(Sender);

Shape8->Visible = false;
RShape8->Visible = false;
Shape7->Visible = false;
RShape7->Visible = false;

if(AutoCheckBox->Checked)
SegmentDAButtonClick(Sender);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::GetAorticAxis()
{

nsigmas = 3.0;
float nsigmas0 = nsigmas;

float zinic = cantImagenes-(zCarina+1)-1; //Comienzo desde los puntos de la carina hacia abajo
float zfinal = 0; //Ignoro los últimos dos corte por ser generalmente demasiado ruidoso

float seedX = MToroide->getAlto()/2;
float seedY = MToroide->getAncho()/2;
float seedR = rD/2;

//Recupera la parte Axial del bastón:
SegmentarAorta(zinic, zfinal, seedX, seedY, seedR, nsigmas, MToroide);

//AortaCenterLine guarda ahora el nuevo centerline calculado a partir de las
//reconstrucciones en la región axial. El orden es: desde la carina hacia el diafragma
//

}

//---------------------------------------------------------------------------

void __fastcall TForm1::GetAorticArch()
{

nsigmas = 3.0;
float nsigmas0 = nsigmas;

float zinic = cantImagenes-(zCarina+1); //Comienzo desde los puntos de la carina hacia arriba
float zfinal = CurvaToroide->Count; //Llego hasta la última porción del Toroide o antes, si finaliza abruptamente en algoritmo

float seedX = MToroide->getAlto()/2;
float seedY = MToroide->getAncho()/2;
float seedR = rD/2;

//Como el orden del CenterLine hasta el momento es desde la Carina hacia el diafragma, lo voy a girar, para
//que todo siga el orden: desde el diafragma hasta el aortic annulus.

for(int i=0; i<AortaCenterLine->Count/2; i++)
{
AortaCenterLine->Exchange(i, AortaCenterLine->Count-1 - i);
AortaRadios->Exchange(i, AortaCenterLine->Count-1 - i);
AortaNormales->Exchange(i, AortaCenterLine->Count-1 - i);
}

int firstCDIndex = AortaCenterLine->Count-1;

//Recupera la parte Axial del bastón:
SegmentarAorta(zinic, zfinal, seedX, seedY, seedR, nsigmas, MToroide);

int firstCAIndex = firstCDIndex + 89;

//AortaCenterLine guarda ahora el nuevo centerline calculado a partir de las
//reconstrucciones en la región axial y el arco.

//SmoothCenterLine();

isAortaSegmented = true;

CDCheckBox->Checked = true;
CACheckBox->Checked = true;

CAIndex = firstCAIndex;
CDIndex = firstCDIndex;


DiaphragmCheckBox->Checked = true;
diaphragmIndex = 0;

}

//---------------------------------------------------------------------------

void __fastcall TForm1::SegmentarAorta(float zi, float zf, float seedX, float seedY, float seedR, float nsigmas, Volumen* MV)
{
//Segmenta la aorta mediante crecimiento de un círculo desde zi hasta zf, tomando como
//volumen a MV:

        nsigmas = nsigmas;
        float nsigmas0 = nsigmas;

        float ax0, ay0; //Puntos semilla
        float ar;

        //int pos0 = 0;   //ImageScrollBar->Position;

        //Cuidado!! Los puntos semillas del estudio original están corridos respecto
        //de los puntos semilla del corte toroidal. Los cortes toroidales son tales
        //que el centerline del bastón siempre estén en el centro de la imagen:

        vector1.x=0.0;
        vector1.y=0.0;
        vector1.z=1.0;

        xP = seedX;
        yP = seedY;
        r0 = seedR;

        int c;

        if(zi>zf)
        c = -1;  //Recorro hacia atrás
        else c=1; //Recorro hacia adelante

        float fail_counter;
        float fail_limit = 3;

        //for(int i=cantImagenes-(zCarina+1)-1; i>1; i = i+1)
        for(int i=zi; i!=zf; i += c)
        {
        ROIListBox->Tag = 0;
        SliceListBox->Tag = i;
        ROIShape->Tag = i; //El Tag de ROIShape se utiliza para indicar la posición del Corte en MToroide

        ax0 = xP;
        ay0 = yP;
        ar = r0;

        ImageScrollBar->Tag = 0;

        ArbitraryCutForm1->Show();

        //Marca el Centerline:
        ArbitraryCutForm1->CenterShape->Visible = true;

        //Setea el plano Toroidal en la posición donde se reconstruyó ese corte,
        //de manera que las coordenadas de la imagen coincidan con el plano
        //reconstruido:
        SetToroidalPlaneFromIndex(i);
        CirculoAxialCavityFilling(MV);

        if(isAortaSegmented || isCenterLineDetected)
        IdleLoop(Form1,1);

        if(ImageScrollBar->Tag == 0)
        fail_counter = 0;

        short int color;

        for(int ix=0; ix<MV->getAncho(); ix++)
        for(int jy=0; jy<MV->getAlto(); jy++)
        CuadroCorteOblicuo->buffer(ix,jy)=LUT[MV->GetV(ix,jy, i)-minPX];
        CuadroCorteOblicuo->Graficar(ArbitraryCutForm1->Image1);
        //

         //Si no se detectaron bordes, o el círculo dio demasiado alejado,
         //se vuelve a intentar, disminuyendo nsigmas:
         if(ImageScrollBar->Tag == 1)
          {
          fail_counter++;

          ROIListBox->Tag = 1;
          ImageScrollBar->Tag = 0;
          nsigmas = 0.8*nsigmas;
          CirculoAxialCavityFilling(MV);
          IdleLoop(Form1,1);

          for(int ix=0; ix<MV->getAncho(); ix++)
          for(int jy=0; jy<MV->getAlto(); jy++)
          CuadroCorteOblicuo->buffer(ix,jy)=LUT[MV->GetV(ix,jy, i)-minPX];
          CuadroCorteOblicuo->Graficar(ArbitraryCutForm1->Image1);
          }
          else
          {
          //Si corrigió "bien" luego del error, restaura el índice nsigmas y baja el fail_counter:
          nsigmas = nsigmas0;
          ROIListBox->Tag = 0;
          fail_counter--;
          }

        if(ImageScrollBar->Tag == 1 )
        {
        //Si vuelve a dar mal, prueba utilizar valores promedios hasta el momento:
        usarAortaProm = true;
        ROIListBox->Tag = 1;
        ImageScrollBar->Tag = 0;

        CirculoAxialCavityFilling(MV);
        IdleLoop(Form1,1);

        for(int ix=0; ix<MV->getAncho(); ix++)
        for(int jy=0; jy<MV->getAlto(); jy++)
        CuadroCorteOblicuo->buffer(ix,jy)=LUT[MV->GetV(ix,jy, i)-minPX];
        CuadroCorteOblicuo->Graficar(ArbitraryCutForm1->Image1);
        }
        else
        {
        //Si corrigió "bien" luego del error, restaura el índice nsigmas y baja el fail_counter:
        nsigmas = nsigmas0;
        ROIListBox->Tag = 0;
        fail_counter--;
        }

        if(ImageScrollBar->Tag == 1 )
        {
        // Si da mal una tercera vez, restaura nsigmas, pasa al siguiente corte y
        //aumenta fail_counter:

        nsigmas = 0.8*nsigmas;
        fail_counter++;
        nsigmas = nsigmas0;
        ROIListBox->Tag = 0;
        }

        if(fail_counter == fail_limit)
        break;

       }

}

//--------------------------------------------------------------------------

void __fastcall TForm1::SegmentDAButtonClick(TObject *Sender)
{
   AortaRadios = new TList;

   //Limpio AortaCenterLine y sus normales, que hasta el momento tiene todos sus punteros
   //apuntando hacia el bastón:

   AortaCenterLine->Clear();
   AortaNormales->Clear();

   GetAorticAxis();

   ArbitraryCutForm1->Close();

   SegmentDAButton->Enabled = false;
   SegmentArchButton->Enabled = true;

   if(AutoCheckBox->Checked)
   SegmentArchButtonClick(Sender);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::SegmentArchButtonClick(TObject *Sender)
{
    GetAorticArch();

    ArbitraryCutForm1->Close();
    CSATrackBar->Max = AortaCenterLine->Count-1;

    CSATrackBar->Enabled = true;
    CSATrackBar->Position = CSATrackBar->Max;

    //MinzScrollBar->Enabled = false;
    //MaxzScrollBar->Enabled = false;

    SegmentArchButton->Enabled = false;
    OrthoCorrectionButton->Enabled = true;
    AorticRegionsGroupBox->Enabled = true;

    VistaRadioGroup->ItemIndex = 0;
    VistaRadioGroup->Enabled = true;

    delete MToroide;
    MToroide = NULL;


}
//---------------------------------------------------------------------------

void __fastcall TForm1::SmoothCenterLine()
{

     //Para calcular la corrección del centerline (ej: por la aparición de regiones
     //elípticas), es necesario recalcular planos oblicuos tomando como vector normal
     //a los vectores normales a este nuevo centerline.

     //Debido a las variaciones abruptas del centerline (ej: ruido o detecciones
     //imperfectas) tomamos un CenterLine cúbico suavizado de 7 puntos (cuando es posible)
     //para el cálculo EXACTO de la nuevas normales, para una posterior repetición del ajuste
     // del Cavity Filling.

     //El polinomio resultante que ajusta en la vecindad del punto k,
     //tendrá la forma Pk = (Pkx, Pky, Pkz), cada uno con la forma:
     //Pki = C3i*t^3 + C2i*t^2 + C1i*t + C0i

     //(Fuente: 3-D Quantification of the Aortic Arch Morphology in 3-D CTA Data
     // for Endovascular Aortic Repair, IEEE TMI, Wörz, 2009)

     float r = 7.0; //rango de puntos a mirar;

     //Variación del parámetro t dentro de la curva de ajuste local:
     float tn[15];

     for(int n=0; n<2*r+1; n++)
     tn[n] = float(n)/r-1.0; //Así, el parámetro t varía entre -1 y 1 dentro del rango de puntos

     //Cálculo de los coeficientes Sum(X_n * t_n^j);
     float X3,X2,X1,X0; X3=0; X2=0; X1=0; X0=0;
     float Y3,Y2,Y1,Y0; Y3=0; Y2=0; Y1=0; Y0=0;
     float Z3,Z2,Z1,Z0; Z3=0; Z2=0; Z1=0; Z0=0;

     float X[4];
     float Y[4];
     float Z[4];

    //Utilizamos la matriz de coeficientes invertida mediante MATLAB:
    float invM[4][4] = {{2.0538,0.0,-1.3999,0.0},{0.0,0.5820,0.0,-0.2217},{-1.3999,0.0,1.1292,0.0},{0.0,-0.2217,0.0,0.15}};

    PuntoF* paux; //puntero auxiliar al CenterLine:
    PuntoF* naux; //puntero auxiliar a la normal al centerline calculada analíticamente
    double norma;

    //k: índice del punto cuya vecindad se va a aproximar:

    int indice;

    TList* AuxCenterLine = new TList;
    TList* AuxNormales = new TList;

    for(int k=0; k<AortaCenterLine->Count; k++)
    {
     X3=0; X2=0; X1=0; X0=0;
     Y3=0; Y2=0; Y1=0; Y0=0;
     Z3=0; Z2=0; Z1=0; Z0=0;

     for(int n=0; n<2*r+1; n++)
     {
     indice = k + (n-r);

     if(indice<0)
     indice=0;

     if(indice>=AortaCenterLine->Count)
     indice=AortaCenterLine->Count-1;

     paux = (PuntoF*)AortaCenterLine->Items[indice];

     //Escalar el centerLine:
     X3 += paux->x*tn[n]*tn[n]*tn[n]; //Sum(Xn*tn^3);
     X2 += paux->x*tn[n]*tn[n];       //Sum(Xn*tn^2);
     X1 += paux->x*tn[n];             //Sum(Xn*tn^1);
     X0 += paux->x;                   //Sum(Xn*tn^0);

     Y3 += paux->y*tn[n]*tn[n]*tn[n]; //Sum(Yn*tn^3);
     Y2 += paux->y*tn[n]*tn[n];       //Sum(Yn*tn^2);
     Y1 += paux->y*tn[n];             //Sum(Yn*tn^1);
     Y0 += paux->y;                   //Sum(Yn*tn^0);

     Z3 += paux->z*tn[n]*tn[n]*tn[n]; //Sum(Zn*tn^3);
     Z2 += paux->z*tn[n]*tn[n];       //Sum(Zn*tn^2);
     Z1 += paux->z*tn[n];             //Sum(Zn*tn^1);
     Z0 += paux->z;                   //Sum(Zn*tn^0);
     }

     X[3] = X0; X[2]=X1; X[1]=X2; X[0]=X3;
     Y[3] = Y0; Y[2]=Y1; Y[1]=Y2; Y[0]=Y3;
     Z[3] = Z0; Z[2]=Z1; Z[1]=Z2; Z[0]=Z3;

     float Cx[4]={0,0,0,0};
     float Cy[4]={0,0,0,0};
     float Cz[4]={0,0,0,0};

     for(int j=0; j<4; j++)
      for(int i=0; i<4; i++)
      {
       Cx[j] +=  X[i]*invM[j][i];
       Cy[j] +=  Y[i]*invM[j][i];
       Cz[j] +=  Z[i]*invM[j][i];
      }

    //A partir del ajuste polinómico, calculo las normales suavizadas y normalizadas:
    naux = new PuntoF;

    naux->x = Cx[2];
    naux->y = Cy[2];
    naux->z = Cz[2];

    norma = sqrt(naux->x*naux->x + naux->y*naux->y + naux->z*naux->z);

    naux->x /= norma;
    naux->y /= norma;
    naux->z /= norma;


    //Resultado: AortaCenterLine, AortaRadios y AortaNormales escalados
    //AortaNormales->Add(naux);
    AuxNormales->Add(naux);


    paux = new PuntoF;
    paux->x = Cx[3];
    paux->y = Cy[3];
    paux->z = Cz[3];

    AuxCenterLine->Add(paux);
   }

   PuntoF* paux2;
   PuntoF* naux2;

   for(int k=0; k<AortaCenterLine->Count; k++)
   {
   paux  = (PuntoF*)AortaCenterLine->Items[k]; //Punto del centerline
   naux  = (PuntoF*)AortaNormales->Items[k];
   paux2 = (PuntoF*)AuxCenterLine->Items[k];  //punto del centerline suavizado
   naux2 = (PuntoF*)AuxNormales->Items[k];

    if(!AP->Tag)
    {
    paux->x = (paux->x + paux2->x)/2.0;
    paux->y = (paux->y + paux2->y)/2.0;
    paux->z = (paux->z + paux2->z)/2.0;
    naux->x = naux2->x;
    naux->y = naux2->y;
    naux->z = naux2->z;
    }
    delete paux2;
    delete naux2;
   }

   AuxCenterLine->Clear();
   delete AuxCenterLine;

   AuxNormales->Clear();
   delete AuxNormales;
}

//-----------------------------------------------------------------------------

void __fastcall TForm1::OrthoCorrectionButtonClick(TObject *Sender)
{
isAortaSegmented = false;

//Las normales van a ser reemplazadas por las nuevas, calculadas a partir del
//smoothing del centerline:

SmoothCenterLine();

int cantSlices = AortaCenterLine->Count;

MPerp = new Volumen(M->getAncho(), M->getAlto(), cantSlices);
MPerp->LlenarTodo(minPX);

ProgressBar1->Position=0;
ProgressBar1->Max=cantSlices;

vector1.x=0.0;
vector1.y=0.0;
vector1.z=1.0;


float x,y,z;
float xp,yp,zp;
float xt,yt,zt;
float xd,yd,zd;
float i1,i2,j1,j2,w1,w2;
float valor;

for(int k=0; k<cantSlices; k++)
{
ProgressBar1->Position=k;

SetCenterLinePlaneFromIndex(k, 0.33);

for (int i=anchoPX/3; i<2*anchoPX/3; ++i)
        for (int j=altoPX/3; j<2*altoPX/3; ++j)
        {

               if(PlanoCorte[i][j].x<altoPX-1 && PlanoCorte[i][j].x>1 &&
                  PlanoCorte[i][j].y<anchoPX-1 && PlanoCorte[i][j].y>1 &&
                  PlanoCorte[i][j].z<cantImagenes-1 && PlanoCorte[i][j].z>1)
                  {
                  x = PlanoCorte[i][j].x;
                  y = PlanoCorte[i][j].y;
                  z = PlanoCorte[i][j].z;

                  xp = floor(PlanoCorte[i][j].x);
                  xt =  ceil(PlanoCorte[i][j].x);
                  yp = floor(PlanoCorte[i][j].y);
                  yt =  ceil(PlanoCorte[i][j].y);
                  zp = floor(PlanoCorte[i][j].z);
                  zt =  ceil(PlanoCorte[i][j].z);

                  xd = x-xp;
                  yd = y-yp;
                  zd = z-zp;

                  i1 = (M->GetV(xp,yp,zp)*(1.0-zd)+ M->GetV(xp,yp,zt)*zd);
                  i2 = (M->GetV(xp,yt,zp)*(1.0-zd)+ M->GetV(xp,yt,zt)*zd);
                  j1 = (M->GetV(xt,yp,zp)*(1.0-zd)+ M->GetV(xt,yp,zt)*zd);
                  j2 = (M->GetV(xt,yt,zp)*(1.0-zd)+ M->GetV(xt,yt,zt)*zd);
                  w1 = i1*(1.0-yd)+i2*yd;
                  w2 = j1*(1.0-yd)+j2*yd;

                  valor = (w1*(1.0-xd)+w2*xd);

                  MPerp->SetV(i,j,k, valor);
                  }
                  else{MPerp->SetV(i,j,k, minPX);}
             }

       }
       ProgressBar1->Position=0;

//Elimino AortaCenterLine, AortaNormal y AortaRadios, para volver a calcularlos
//en MPerp (cortes perpendiculares). Antes, reemplazo el "bastón" por el centerline
//parcial encontrado, con sus normales:

PuntoF* paux;

paux = (PuntoF*)AortaCenterLine->First();
float* rr = (float*)AortaRadios->First();

zP = 0;
xP = MPerp->getAncho()/2;
yP = MPerp->getAncho()/2;
r0 = rr[0];

for(int i=0; i<CurvaToroide->Count; i++)
{
paux = (PuntoF*)CurvaToroide->Items[i];
delete paux;
paux = (PuntoF*)NormalToroide->Items[i];
delete paux;

}
CurvaToroide->Clear();
NormalToroide->Clear();

float* raux;

for(int i=0; i<AortaCenterLine->Count; i++)
{
paux = (PuntoF*)AortaCenterLine->Items[i];
CurvaToroide->Add(paux);
paux = (PuntoF*)AortaNormales->Items[i];
NormalToroide->Add(paux);
raux = (float*)AortaRadios->Items[i];
delete[] raux;
}

AortaCenterLine->Clear();
AortaNormales->Clear();
AortaRadios->Clear();

ROIListBox->Tag = 1; //ROIListBox->Tag = 1 => Dibuja los resultados de la segmentación

AP->Tag = 1;

usarAortaProm = true;

nsigmas = 3.0;

SegmentarAorta(0, cantSlices, xP, yP, r0, nsigmas, MPerp);

delete MPerp;

CSATrackBar->Max = AortaCenterLine->Count-1;
CSATrackBar->Position = CSATrackBar->Max;

isAortaSegmented = true;

for(int i=0; i<CurvaToroide->Count; i++)
{
paux = (PuntoF*)CurvaToroide->Items[i];
delete paux;
paux = (PuntoF*)NormalToroide->Items[i];
delete paux;
}
CurvaToroide->Clear();
delete CurvaToroide;
CurvaToroide = NULL;
NormalToroide->Clear();
delete NormalToroide;
NormalToroide = NULL;


ArbitraryCutForm1->Close();

OrthoCorrectionButton->Enabled = false;



}
//---------------------------------------------------------------------------

void __fastcall TForm1::CSATrackBarChange(TObject *Sender)
{
       int i = CSATrackBar->Position;

       //Porcentaje de la pantalla a mostrar/procesar, etc
       float alfa = 0.2;


      //Interpolación BI/TRILIEAL:
      //http:en.wikipedia.org/wiki/Bilinear_interpolation
      //http://en.wikipedia.org/wiki/Trilinear_interpolation

      float x,y,z;
      float xd, yd, zd;
      float xp, xt, yp, yt, zp, zt;
      float i1, i2, j1, j2, w1, w2;
      short int valor;

       IdleLoop(Sender,1);

       SetCenterLinePlaneFromIndex(i, alfa);

      //VistaRadioGroup->ItemIndex: 0- MPR, 1-Axial
      if(VistaRadioGroup->ItemIndex == 1)
      {
        // Altura en z del centro del plano oblicuo:
        zp = PlanoCorte[anchoPX/2][altoPX/2].z;

        float factorx,factory;

        factorx = float(altoPX)/float(ObliqueImage->Height);
        factory = float(anchoPX)/float(ObliqueImage->Width);

        //Reducción del corte a la mitad:
        for (int i=0; i< ObliqueImage->Height; ++i)
         for (int j=0; j< ObliqueImage->Width; ++j)
         {

           x = float(i)*factorx;
           y = float(j)*factory;

           xp = floor(x);
           xt =  ceil(y);
           yp = floor(y);
           yt =  ceil(x);

           xd = x-xp;
           yd = y-yp;

           valor = M->GetV(xp,yp,zp)*(1.0-xd)*(1.0-yd) + xd*(1.0-yd)*M->GetV(xt,yp,zp)
                + (1.0-xd)*yd*M->GetV(xp,yt,zp)+ xd*yd*M->GetV(xt,yt,zp);

           CuadroCorteOblicuo->buffer(i,j) = LUT[(short int)valor-minPX];
          }

       CuadroCorteOblicuo->Graficar(ObliqueImage);
       return;
      }


      vector1.x=0.0;
      vector1.y=0.0;
      vector1.z=1.0;

        for (int i=ObliqueImage->Height/2; i< 3*ObliqueImage->Height/2; ++i)
        for (int j=ObliqueImage->Width/2; j<3*ObliqueImage->Width/2; ++j)
        {

         if(PlanoCorte[i][j].x<altoPX-1 && PlanoCorte[i][j].x>1 &&
           PlanoCorte[i][j].y<anchoPX-1 && PlanoCorte[i][j].y>1 &&
           PlanoCorte[i][j].z<cantImagenes-1 && PlanoCorte[i][j].z>1)
           {
           x = PlanoCorte[i][j].x;
           y = PlanoCorte[i][j].y;
           z = PlanoCorte[i][j].z;

           xp = floor(PlanoCorte[i][j].x);
           xt =  ceil(PlanoCorte[i][j].x);
           yp = floor(PlanoCorte[i][j].y);
           yt =  ceil(PlanoCorte[i][j].y);
           zp = floor(PlanoCorte[i][j].z);
           zt =  ceil(PlanoCorte[i][j].z);

           xd = x-xp;
           yd = y-yp;
           zd = z-zp;

           i1 = (M->GetV(xp,yp,zp)*(1.0-zd)+ M->GetV(xp,yp,zt)*zd);
           i2 = (M->GetV(xp,yt,zp)*(1.0-zd)+ M->GetV(xp,yt,zt)*zd);
           j1 = (M->GetV(xt,yp,zp)*(1.0-zd)+ M->GetV(xt,yp,zt)*zd);
           j2 = (M->GetV(xt,yt,zp)*(1.0-zd)+ M->GetV(xt,yt,zt)*zd);
           w1 = i1*(1.0-yd)+i2*yd;
           w2 = j1*(1.0-yd)+j2*yd;

           valor = (w1*(1.0-xd)+w2*xd);

           CuadroCorteOblicuo->buffer(i-ObliqueImage->Height/2,j-ObliqueImage->Width/2) = LUT[(short int)valor-minPX];
           }
           else{CuadroCorteOblicuo->buffer(i-ObliqueImage->Height/2,j-ObliqueImage->Width/2) = LUT[0];}
         }


        CuadroCorteOblicuo->Graficar(ObliqueImage);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::AnnulusCheckBoxClick(TObject *Sender)
{

if(AnnulusCheckBox->Checked)
{
annulusIndex = CSATrackBar->Position;
}else
{
annulusIndex = -1;
}


if(isModelComplete = IsModelComplete())
{
SaveModelButton->Enabled = true;
PrintReportButton->Enabled = true;
}
else
{
SaveModelButton->Enabled = false;
PrintReportButton->Enabled = false;
}

}
//---------------------------------------------------------------------------

void __fastcall TForm1::CACheckBoxClick(TObject *Sender)
{
if(CACheckBox->Checked)
{
CAIndex = CSATrackBar->Position;
}else
{
CAIndex = -1;
}

if(isModelComplete = IsModelComplete())
{
SaveModelButton->Enabled = true;
PrintReportButton->Enabled = true;
}
else
{
SaveModelButton->Enabled = false;
PrintReportButton->Enabled = false;
}

}
//---------------------------------------------------------------------------

void __fastcall TForm1::BCACheckBoxClick(TObject *Sender)
{
   if(BCACheckBox->Checked)
{
BCAIndex = CSATrackBar->Position;
}else
{
BCAIndex = -1;
}

if(isModelComplete = IsModelComplete())
{
SaveModelButton->Enabled = true;
PrintReportButton->Enabled = true;
}
else
{
SaveModelButton->Enabled = false;
PrintReportButton->Enabled = false;
}

}
//---------------------------------------------------------------------------

void __fastcall TForm1::LSACheckBoxClick(TObject *Sender)
{

if(LSACheckBox->Checked)
{
LSAIndex = CSATrackBar->Position;
}else
{
LSAIndex = -1;
}

if(isModelComplete = IsModelComplete())
{
SaveModelButton->Enabled = true;
PrintReportButton->Enabled = true;
}
else
{
SaveModelButton->Enabled = false;
PrintReportButton->Enabled = false;
}

}
//---------------------------------------------------------------------------

void __fastcall TForm1::CDCheckBoxClick(TObject *Sender)
{
if(CDCheckBox->Checked)
{
CDIndex = CSATrackBar->Position;
}else
{
CDIndex = -1;
}

if(isModelComplete = IsModelComplete())
{
SaveModelButton->Enabled = true;
PrintReportButton->Enabled = true;
}
else
{
SaveModelButton->Enabled = false;
PrintReportButton->Enabled = false;
}

}
//---------------------------------------------------------------------------

void __fastcall TForm1::ApexSinusCheckBoxClick(TObject *Sender)
{

if(ApexSinusCheckBox->Checked)
{
apexSinusIndex = CSATrackBar->Position;
}else
{
apexSinusIndex = -1;
}

if(isModelComplete = IsModelComplete())
{
SaveModelButton->Enabled = true;
PrintReportButton->Enabled = true;
}
else
{
SaveModelButton->Enabled = false;
PrintReportButton->Enabled = false;
}

}
//---------------------------------------------------------------------------

void __fastcall TForm1::DiaphragmCheckBoxClick(TObject *Sender)
{

if(DiaphragmCheckBox->Checked)
{
diaphragmIndex = CSATrackBar->Position;
}else
{
diaphragmIndex = -1;
}

if(isModelComplete = IsModelComplete())
{
SaveModelButton->Enabled = true;
PrintReportButton->Enabled = true;
}
else
{
SaveModelButton->Enabled = false;
PrintReportButton->Enabled = false;
}

}
//---------------------------------------------------------------------------


void __fastcall TForm1::VistaRadioGroupClick(TObject *Sender)
{
CSATrackBar->OnChange(Sender);
}
//---------------------------------------------------------------------------


bool __fastcall TForm1::IsModelComplete()
{
  if(AnnulusCheckBox->Checked&&CACheckBox->Checked&&BCACheckBox->Checked&&
   LSACheckBox->Checked&&CDCheckBox->Checked&&ApexSinusCheckBox->Checked&&
   DiaphragmCheckBox->Checked)
   return true;
   else return false;

}
//---------------------------------------------------------------------------


void __fastcall TForm1::SaveModelButtonClick(TObject *Sender)
{

        if(!IsModelComplete())
        {
        Application->MessageBox("Complete landmark selection", "Model Incomplete", MB_OK);
        return;
        }

        AP->Items->Clear();

        PuntoF* paux; //puntero a Punto auxiliar
        PuntoF* naux; //puntero a Normal auxiliar
        float* radio; //puntero a radio auxiliar

        AP->Items->Add(zCarina);

        AP->Items->Add(CAIndex);
        AP->Items->Add(BCAIndex);
        AP->Items->Add(LSAIndex);
        AP->Items->Add(CDIndex);
        AP->Items->Add(apexSinusIndex);
        AP->Items->Add(diaphragmIndex);


        for(int i=0; i<AortaCenterLine->Count; i++)
        {
        paux = (PuntoF*)AortaCenterLine->Items[i];
        AP->Items->Add(paux->x);
        AP->Items->Add(paux->y);
        AP->Items->Add(paux->z);

        naux = (PuntoF*)AortaNormales->Items[i];
        AP->Items->Add(naux->x);
        AP->Items->Add(naux->y);
        AP->Items->Add(naux->z);

        radio = (float*)AortaRadios->Items[i];
        AP->Items->Add(radio[0]);
        }

        AP->Items->SaveToFile("aorta_model.txt");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LoadModelButtonClick(TObject *Sender)
{


        //Modelo Aortico: *.txt
        //Secuencia de grabado/apertura:
        //zCarina, CAIndex, BCAIndex, LSAIndex, CDIndex, apexSinusIndex, diaphragmIndex,
        //CenterLine x, CenterLine y, CenterLine z, Normal x, Normal y, Normal z, radio, (...)

        if(!ImagenEnPantalla)
        {
        Application->MessageBox("Load study to associate with aortic model", "No Study Loaded", MB_OK);
        return;
        }

        OpenDialog1->Filter = "Text files (*.txt)|*.TXT";

        if( !OpenDialog1->Execute() )
        return;

        AP->Items->Clear();

        AP->Items->LoadFromFile(OpenDialog1->FileName);

        zCarina = atof(AP->Items->Strings[0].c_str());

        CAIndex = atof(AP->Items->Strings[1].c_str());
        BCAIndex =atof(AP->Items->Strings[2].c_str());
        LSAIndex =atof(AP->Items->Strings[3].c_str());
        CDIndex = atof(AP->Items->Strings[4].c_str());
        apexSinusIndex =atof(AP->Items->Strings[5].c_str());
        diaphragmIndex =atof(AP->Items->Strings[6].c_str());

        PuntoF* paux; //puntero a Punto auxiliar
        PuntoF* naux; //puntero a Normal auxiliar
        float* radio; //puntero a radio auxiliar

        float xs,ys,zs;
        float nxs,nys,nzs;
        float radio_s;

        for(int i=7; i<AP->Items->Count; i++)
        {

        xs      = atof(AP->Items->Strings[i  ].c_str());
        ys      = atof(AP->Items->Strings[i+1].c_str());
        zs      = atof(AP->Items->Strings[i+2].c_str());
        nxs     = atof(AP->Items->Strings[i+3].c_str());
        nys     = atof(AP->Items->Strings[i+4].c_str());
        nzs     = atof(AP->Items->Strings[i+5].c_str());
        radio_s = atof(AP->Items->Strings[i+6].c_str());

        paux = (PuntoF*)AortaCenterLine->Items[i];
        AP->Items->Add(paux->x);
        AP->Items->Add(paux->y);
        AP->Items->Add(paux->z);

        naux = (PuntoF*)AortaNormales->Items[i];
        AP->Items->Add(naux->x);
        AP->Items->Add(naux->y);
        AP->Items->Add(naux->z);

        radio = (float*)AortaRadios->Items[i];
        AP->Items->Add(radio[0]);
        }

        AP->Items->SaveToFile("aorta_model.txt");
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Malla1Click(TObject *Sender)
{
       double voxLength = res[0];

       for(int i=0; i<3; i++)
        if(res[i]<voxLength)
         voxLength = res[i];

       double radius = 10.0;
       double center_point[3];

       Mesh* myMesh;

       myMesh = new Mesh( 4.0*voxLength, radius, res[0], res[1], res[2] );
       myMesh->MoveMesh(xP, yP, ImageScrollBar->Position);
       myMesh->set_min_intensity_thresh(meanC-1.5*stdC);
       myMesh->set_max_intensity_thresh(meanC+1.5*stdC);

       center_point[0] = xP;
       center_point[1] = yP;
       center_point[2] = ImageScrollBar->Position;

       myMesh->set_center_point(center_point);
       myMesh->ResetBoundingBox();
       myMesh->set_scale_x(res[0]);
       myMesh->set_scale_y(res[1]);
       myMesh->set_scale_z(res[2]);

       myMesh->set_max_inactive_iterations(25);
       myMesh->set_min_movement_thresh(res[0]/25.0);

       meshOK = true;

       myMesh->color[0] = 1.0;
       myMesh->color[1] = 0.0;
       myMesh->color[2] = 0.0;

       Meshes->Add(myMesh);

       Mesh* mU;
       Mesh* mPU;
       Mesh* mF;

       double bpn1[3];
       double bpn2[3];
       double bpp1[3];
       double bpp2[3];
       
       const double* center_p1;
       const double* center_p2;

       //Compute bounding planes:
       if(Meshes->Count > 1)
       {
        mU = (Mesh*)Meshes->Items[Meshes->Count-1];    //Última malla
        mPU = (Mesh*)Meshes->Items[Meshes->Count-2]; //Penúltimo

        center_p1 = mU->get_center_point();
        center_p2 = mPU->get_center_point();

        bpn1[0] = center_p1[0]-center_p2[0];
        bpn1[1] = center_p1[1]-center_p2[1];
        bpn1[2] = center_p1[2]-center_p2[2];

        bpp1[0] = (center_p1[0]+center_p2[0])/2.0;
        bpp1[1] = (center_p1[1]+center_p2[1])/2.0;
        bpp1[2] = (center_p1[2]+center_p2[2])/2.0;

        mU->set_bound_plane(bpn1,bpp1,1);

        bpn2[0] = center_p1[0]-center_p2[0];
        bpn2[1] = center_p1[1]-center_p2[1];
        bpn2[2] = center_p1[2]-center_p2[2];

        bpp2[0] = (center_p1[0]+center_p2[0])/2.0;
        bpp2[1] = (center_p1[1]+center_p2[1])/2.0;
        bpp2[2] = (center_p1[2]+center_p2[2])/2.0;

        mPU->set_bound_plane(bpn2,bpp2,0);

        //Manual set of other limits:

        bpn2[0] = 0.0;
        bpn2[1] = 0.0;
        bpn2[2] = 1.0;

        bpp2[0] = 0.0;
        bpp2[1] = 0.0;
        bpp2[2] = M->getProf();

        mU->set_bound_plane(bpn1,bpp1,0);

        mF = (Mesh*)Meshes->First();
        bpn1[0] = 0.0;
        bpn1[1] = 0.0;
        bpn1[2] = 1.0;

        bpp1[0] = 0.0;
        bpp1[1] = 0.0;
        bpp1[2] = M->getProf();

        mF->set_bound_plane(bpn1, bpp1, 1);
       }

       myMesh = NULL;
}

//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
      //Adaptative Remeshing:
      //1) Búsqueda y eliminación de lados pequeños:
      //2) Búsqueda e inversión de triángulos puntiagudos:
      //3) Búsqueda y subdivisión de triángulos grandes:
      //************************************************************************

    // /*

     Mesh* myMesh;
     myMesh = (Mesh*)Meshes->First();
     myMesh->RefreshMesh();
     myMesh->set_is_active(true);

     double dt = 0.5;
     t += dt;


     //Duty Cycle:
     int duty_cycle = 60;
     int on_cycle   = 30;

     if(int(t)%duty_cycle < on_cycle) {
     empuje = true;

     //1) Búsqueda y eliminación de lados pequeños:
     myMesh->AdaptiveMelting();

     //2) Búsqueda e inversión de triangulos grandes puntiagudos:
     //************************************************************************
     myMesh->AdaptiveInversion();
     myMesh->set_rest_distance(myMesh->ComputeMeanEdgeDistance());
     }
     else {
       empuje = false;
     }

     Face* pF;

     const double* xyz; 

     //Cada N iteraciones, utiliza la subdivisión adaptativa:
     if( ( int(t)%250 )==0 )
     {
        //1) Búsqueda y eliminación de lados pequeños:
        myMesh->AdaptiveMelting();

        //2) Búsqueda e inversión de triangulos grandes puntiagudos:
        //************************************************************************
        myMesh->AdaptiveInversion();

        //3) Búsqueda y subdivisión de triángulos grandes:
        //************************************************************************
        myMesh->AdaptiveSubdivision();

        //Pre-Calcula el centro de masa de todas las caras, y resetea las fuerzas
        //Pre calcula la normal de cada nodo:

        myMesh->set_rest_distance(myMesh->ComputeMeanEdgeDistance());
     }

     myMesh->ComputeFaceNormals();

     Node* pN;

     for(int i=0; i<Nodes->Count; i++)
     {
        pN = (Node*)Nodes->Items[i];
        pN->ResetForce();
        pN->ComputeMassCenter();
        pN->ComputeNormal();
     }

     short int valor;
     double px=0,py=0,pz=0;
     double norma;

     //Posición:
     double x,y,z;
     //Normal:
     double nx,ny,nz;

     //Calcula las nuevas fuerzas/aceleracion/velocidad y posicion
     for (int i = 0; i < Nodes->Count; i++) {
        pN = (Node*)Nodes->Items[i];

        if(!pN->get_is_active())
        continue;

        pN->InternalForce();

        //Valores por defecto (eg: si caen fuera de la imagen):
        valor = minPX;

        xyz = pN->get_position();
        //parámetros para la detección difusa de bordes:
        //Chequea que se mantenga dentro de la imagen:
        /* if
        (
            (pN->xyz[0]-1>0&&pN->xyz[0]+1<altoPX) &&
            (pN->xyz[1]-1>0&&pN->xyz[1]+1<anchoPX) &&
            (pN->xyz[2]-1>33  &&  pN->xyz[2]+1<81)
        )*/
       ///*


        valor = M->GetV(xyz[0],xyz[1],xyz[2]);

        if(empuje)
            pN->ExternalForce(valor, px,py,pz);

            pN->ComputeAcceleration();
            pN->ComputeVelocity(dt);
            pN->ComputeNextPosition(dt);
     }

    //Actualiza las coordenadas en paralelo:
    for(int i=0; i<Nodes->Count; i++)
    {
        pN = (Node*)Nodes->Items[i];
        pN->RefreshPosition();
    }

    IdleLoop(this, 0);   //  */

    myMesh->set_is_active(false);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::UCSButtonClick(TObject *Sender)
{
      if(UFCSpline==NULL)
      UFCSpline = new TList;

      if(NormalesSpline==NULL)
      NormalesSpline = new TList;


    float xx[19] = {260, 254, 246, 261, 287, 301, 304, 302, 301, 300,
                    300, 298, 295, 290, 285, 283, 281, 280, 279};

    float yy[19] = {217, 211, 219, 234, 259, 293, 310, 329, 331, 330,
                    327, 324, 310, 304, 293, 289, 286, 278, 270};

    float zz[19] = {193, 178, 135, 106, 86 , 113, 135, 163, 194, 221,
                    235, 250, 280, 310, 330, 352, 370, 393, 415};

    PuntoF* nP;

    if(CLCheckBox->Checked)
    {
        for(int i=0; i<19; i++)
        {
        nP = new PuntoF;
        nP->x = yy[i];
        nP->y = xx[i];
        nP->z = zz[i];
        ManualCenterLine->Add(nP);
        nP = NULL;
        }
     }
     
      float d_MCL = 0;
      PuntoF* aP1;
      PuntoF* aP2;

      for(int i=0; i<ManualCenterLine->Count-1;i++)
      {
      aP1 = (PuntoF*)ManualCenterLine->Items[i];
      aP2 = (PuntoF*)ManualCenterLine->Items[i+1];
      d_MCL += sqrt( pow(aP1->x - aP2->x, 2.0) + pow(aP1->y - aP2->y, 2.0) + pow(aP1->z - aP2->z, 2.0) );
      }

      d_MCL = d_MCL /float(ManualCenterLine->Count-1);

      Faces = new TList;
      Edges = new TList;
      Nodes = new TList;

      //CylinderMeshing (TList* CL, TList* NL, int nA, float r, Mesh* cylMesh, TList* Faces, TList* Edges, TList* Nodes)
      double voxLength = res[0];

      for(int i=0; i<3; i++)
        if(res[i]<voxLength)
            voxLength = res[i];

      float r = 3.0;
      float deltaC   = 4.0*voxLength;
      float deltaAng = 2.0*asin(deltaC / 2.0 /r);
      int nA = 2.0*3.1415 / deltaAng;

      float deltaL = 2.0*r*sqrt( pow(sin(deltaAng/2.0), 2.0) - pow(sin(deltaAng/4.0), 2.0));
      int nL       = d_MCL / deltaL;

      UniformCubicSpline(ManualCenterLine, nL, UFCSpline,NormalesSpline);

      //myMesh = CylinderMeshing (UFCSpline, NormalesSpline, nA , r, Faces, Edges, Nodes, 2*voxLength, res[0], res[1], res[2]);

      //centerline initialization:
      double** center_line;
      center_line = new double*[UFCSpline->Count];

      for (int i = 0; i < UFCSpline->Count; ++i) {
        center_line[i] = new double[3];
      }

      //Centerline tangent initialization:
      double** normal_vector;
      normal_vector = new double*[NormalesSpline->Count];

      for (int i = 0; i < NormalesSpline->Count; ++i) {
        normal_vector[i] = new double[3];
      }

      PuntoF* aP;

      for (int i = 0; i < UFCSpline->Count; ++i) {
        aP = (PuntoF*)UFCSpline->Items[i];
        center_line[i][0] = aP->x;
        center_line[i][1] = aP->y;
        center_line[i][2] = aP->z;
      }

      for (int i = 0; i < NormalesSpline->Count; ++i) {
        aP = (PuntoF*)NormalesSpline->Items[i];
        normal_vector[i][0] = aP->x;
        normal_vector[i][1] = aP->y;
        normal_vector[i][2] = aP->z;
      }


      Mesh* myMesh = new Mesh(center_line, UFCSpline->Count, normal_vector,  NormalesSpline->Count,
                    nA, r, 2*voxLength, res[0], res[1], res[2]);

      myMesh->set_max_intensity_thresh(meanC+2.5*stdC);
      myMesh->set_min_intensity_thresh(meanC-2.5*stdC);
      myMesh->set_max_length(2.0*myMesh->ComputeMeanEdgeDistance());
      myMesh->set_min_length(0.5*myMesh->ComputeMeanEdgeDistance());
      myMesh->set_min_movement_thresh(0.0);
      myMesh->set_max_inactive_iterations(100);
      myMesh->set_rest_distance(0.0);
      myMesh->set_scale_x(res[0]);
      myMesh->set_scale_y(res[1]);
      myMesh->set_scale_z(res[2]);

      Nodes = myMesh->get_node_list();
      Faces = myMesh->get_face_list();
      Edges = myMesh->get_edge_list();
      myMesh->ComputeFaceNormals();

      myMesh->color[0] = 0.7;
      myMesh->color[1] = 0.25;
      myMesh->color[2] = 0.4;

      Meshes->Add(myMesh);

      meshOK = true;

      myMesh = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
     double dt = 1.0;
     t += dt;

     Mesh* pMesh;
     Face* pF;
     Node* pN;

     Punto* pP;

     double activityThr = 0.25;

     short int valor;
        double px=0,py=0,pz=0;
        double norma;

        //Normal:
        double nx,ny,nz;
        const double* xyz;

     for(int i=0; i<Meshes->Count; ++i) {
        pMesh = (Mesh*)Meshes->Items[i];
        pMesh->RefreshMesh();

        if(!pMesh->get_is_active())
        continue;

        Nodes = pMesh->get_node_list();
        Faces = pMesh->get_face_list();

        if(int(t)%20 == 0)
            pMesh->set_rest_distance(pMesh->ComputeMeanEdgeDistance());

       //Cada N iteraciones, utiliza la subdivisión adaptativa:
        if((int(t)%50) == 0)
        {
            //1) Búsqueda y eliminación de lados pequeños:
            pMesh->AdaptiveMelting();

            //2) Búsqueda e inversión de triangulos grandes puntiagudos:
            //************************************************************************
            pMesh->AdaptiveInversion();

            //3) Búsqueda y subdivisión de triángulos grandes:
            //************************************************************************
            pMesh->AdaptiveSubdivision();

            //Pre-Calcula el centro de masa de todas las caras, y resetea las fuerzas
            //Pre calcula la normal de cada nodo:
            pMesh->set_rest_distance(pMesh->ComputeMeanEdgeDistance());
        }

        pMesh->ComputeFaceNormals();

       /* for(int i=0; i<Faces->Count; i++)
        {
            pF = (Face*)Faces->Items[i];
            pF->computeCG();

            pP = new Punto;
            pP->x = int(pF->cgx + pF->xn);
            pP->y = int(pF->cgy + pF->yn);
            pP->z = int(pF->cgz + pF->zn);
            pP->color = M->GetV(pP->x, pP->y, pP->z);
            PointMap->Add(pP);
        } */

        for(int i=0; i<PointMap->Count; i++)
        {
            pP = (Punto*)PointMap->Items[i];
            M->SetV(pP->x, pP->y, pP->z, minPX);
        }

        for(int i=0; i<Nodes->Count; i++)
        {
            pN = (Node*)Nodes->Items[i];
            pN->ResetForce();
            pN->ComputeMassCenter();
            pN->ComputeNormal();
        }

        //Calcula las nuevas fuerzas/aceleracion/velocidad y posicion
        for(int i=0; i<Nodes->Count; i++)
        {
            pN = (Node*)Nodes->Items[i];

            if(!pN->get_is_active())
            continue;

            pN->InternalForce();

            //Valores por defecto (eg: si caen fuera de la imagen):
            valor = minPX;

            //parámetros para la detección difusa de bordes:

            xyz = pN->get_position();
            //Chequea que se mantenga dentro de la imagen:
            valor = M->GetV(xyz[0],xyz[1],xyz[2]);

            pN->ExternalForce(valor, px,py,pz);

            pN->ComputeAcceleration();
            pN->ComputeVelocity(dt);
            pN->ComputeNextPosition(dt);
        }

        //Actualiza las coordenadas en paralelo:
        for(int i=0; i<Nodes->Count; i++)
        {
            pN = (Node*)Nodes->Items[i];
            pN->RefreshPosition();
        }

     //if(pMesh->ComputeActivity() < activityThr )
     //  pMesh->set_is_active(false);
     //ListBox1->Items->Add( pMesh->CountActivity() );
     pMesh->ComputeBoundingBox();
     }

    /* while(PointMap->Count)
        {
            pP = (Punto*)PointMap->First();
            M->SetV(pP->x,pP->y,pP->z,pP->color);
            delete pP;
            PointMap->Delete(0);
            pP = NULL;
        }
        PointMap->Clear();  */

    IdleLoop(this, 0);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button3Click(TObject *Sender)
{
ListBox1->Items->SaveToFile("activity.txt");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{

    Mesh* pM;
    for(int i=0; i<Meshes->Count; i++)
    {
        pM = (Mesh*)Meshes->Items[i];
        pM->RefreshMesh();
        pM->RefineToSubPixel();
    }

    Node* pN;

    const double* xyz;

    for(int i = 0; i < Meshes->Count; ++i)
    {
        pM = (Mesh*)Meshes->Items[i];
        pM->RefreshMesh();

        Nodes = pM->get_node_list();

        for (int j = 0; j < Nodes->Count; ++j) {
          xyz = pN->get_position();
          pN = (Node*)Nodes->Items[j];
          M->SetV(xyz[0], xyz[1], xyz[2], minPX);
        }
    }

    //int clroffset = 0;
    //for(int i=0; i<Meshes->Count; i++)
    //{
    //    pM = (Mesh*)Meshes->Items[i];
    //    pM->activate();
    //    fastfloodfill3D( M, Center+Width, Center-Width, 0, pM->Center[0], pM->Center[1], pM->Center[2]);
    //}


    TList* CP = new TList;
    Punto* sp = new punto;

    const double* center_p;

    for(int i=0; i<Meshes->Count; i++)
    {
        pM = (Mesh*)Meshes->Items[i];
        pM->RefreshMesh();
        center_p = pM->get_center_point();
        sp->x = center_p[0];
        sp->y = center_p[1];
        sp->z = center_p[2];
        getAllMatrixConnected(sp, M, CP, minPX, minPX);
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button5Click(TObject *Sender)
{
        Mesh* pMesh;

        for(int i=0; i<Meshes->Count; i++)
        {
            pMesh = (Mesh*)Meshes->Items[i];

            if(SaveDialog1->Execute())
                pMesh->SaveMeshToFile( SaveDialog1->FileName.c_str() );
        }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button6Click(TObject *Sender)
{
        Mesh* pMesh;
        pMesh = new Mesh();

        if(OpenDialog1->Execute())
        {
        pMesh->LoadMeshFromFile( OpenDialog1->FileName.c_str() );
        pMesh->RefreshMesh();
        Meshes->Add(pMesh);
        }
        meshOK = true;
}
//---------------------------------------------------------------------------

