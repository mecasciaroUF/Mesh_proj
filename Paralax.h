//---------------------------------------------------------------------------

#include <Chart.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <Series.hpp>
#include <StdCtrls.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>

#ifndef ParalaxH
#define ParalaxH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>                
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

#include <time.h>

#include "Mesh.h"
#include "Face.h"
#include "Edge.h"
#include "Node.h"

//Clases utilizadas:
#include "Vector.h"
#include "Matriz.h"
#include "Volumen.h"

#include "CV.h"

//Threads:
#include "histoThread.h"

//estructuras:
#include "structpunto.h"

//Métodos:
#include "openGLfuncs.h" //Funciones básicas de inicialización de OpenGL
#include "fastfloodfill3D.h"
#include "CilindricalMesh.h"


//Forms Auxiliares:
#include "SegmentaForm.h"
#include "Unit3.h"
#include "LabelForm.h"
#include "CACForm.h"
#include "AjusteForm.h"
#include "ROISaveForm.h"
#include "ArbitraryCutForm.h"
#include "AnisForm.h"
#include "CannyFilter.h"

//Algoritmos utilizados:
#include "neighopers.h" //Operaciones de vecindad entre voxels. Reconocimiento de tipos: tailpoint, endpoint, etc

//#include "esqueletonizacion.h"//Algoritmo de esqueletonización de Tao-Wang, variación de Ma-Sonka
#include "getAllConnected.h"  //GetAllConnected: algoritmos de conectividad a un seed-point
#include "thinning.h" //Algoritmo de esqueletonización por búsqueda paralela subiterativa de puntos simples de Palagy et. al.
#include "kmeans.h"  //Algoritmo de segmentación por k-means clustering
#include "strel.h"   //Creación del elemento Estructurante esférico de Radio = r
#include "simplex.h" //** Incluye nrutil.c y nrutil.h **: Minimización por método SIMPLEX (R) de Nelder & Mead

#include "balloon.h" //Función de crecimiento de región para segmentación del corazón
#include "balloon3D.h"
#include "Bresenham3D.h" //Algoritmo de Bresenham para unir partes del esqueleto.
#include "marchingCubes.h"

#include <Chart.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <Menus.hpp>
#include <Series.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>

#include <Dialogs.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <Graphics.hpp>
#include <DBCtrls.hpp>
#include <Chart.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include <Buttons.hpp>
#include <Series.hpp>
#include <ArrowCha.hpp>

//Librerias de OpenGL
#include <gl\gl.h>
#include <gl\glu.h>
#include "glut.h"
#include "glext.h"
//#include <wglext.h>

//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TMainMenu *MainMenu1;
        TOpenDialog *OpenDialog1;
        TMenuItem *Archivo1;
        TMenuItem *Cerrar1;
        TMenuItem *Salir1;
        TMenuItem *ROI1;
        TMenuItem *Guardar1;
        TMenuItem *Cargar1;
        TSaveDialog *SaveDialog1;
        TMenuItem *AbrirVolumen1;
        TMenuItem *Filtros1;
        TMenuItem *Mediana1;
        TMenuItem *Apertura;
        TMenuItem *GuardarVolumen1;
        TMenuItem *Histograma1;
        TMenuItem *Resta1;
        TMenuItem *HighBoost1;
        TMenuItem *GrabarAC1;
        TMenuItem *EliminarPulmon1;
        TMenuItem *Render1;
        TMenuItem *Bordes1;
        TMenuItem *LUT1;
        TMenuItem *Clustering1;
        TMenuItem *KMeansCluster;
        TMenuItem *ConnnectedComponent1;
        TMenuItem *SegmentaCorazn1;
        TMenuItem *Cierre1;
        TMenuItem *Umbral1;
        TMenuItem *SquareTracing1;
        TMenuItem *Global1;
        TMenuItem *VOI1;
        TMenuItem *Erosion1;
        TMenuItem *Dilatacin1;
        TPageControl *TabSheet;
        TTabSheet *ImagenSheet;
        TLabel *IniSliceLabel;
        TLabel *FinSliceLabel;
        TLabel *CarpetaLabel;
        TLabel *Label7;
        TLabel *HULabel;
        TLabel *Label8;
        TLabel *Label9;
        TLabel *MeanLabel;
        TLabel *StDLabel;
        TLabel *Label10;
        TLabel *Label11;
        TPanel *PanelPantalla;
        TImage *Pantalla;
        TShape *ROIShape;
        TShape *Shape7;
        TShape *Shape8;
        TScrollBar *ImageScrollBar;
        TListBox *ROIListBox;
        TCheckBox *ROICheckBox;
        TEdit *FinSliceEdit;
        TEdit *IniSliceEdit;
        TListBox *SliceListBox;
        TTabSheet *ROISheet;
        TPanel *ROIPanel;
        TTabSheet *OrthoViewSheet;
        TImage *XYImage;
        TImage *ZXImage;
        TImage *YZImage;
        TPanel *OrthoPanel;
        TScrollBar *YScrollBar;
        TScrollBar *ZScrollBar;
        TScrollBar *XScrollBar;
        TShape *Shape1;
        TShape *Shape2;
        TShape *Shape3;
        TShape *YShape;
        TShape *XShape;
        TShape *ZShape;
        TShape *XShape2;
        TShape *YShape2;
        TShape *ZShape2;
        TShape *Shape4;
        TShape *Shape5;
        TShape *Shape6;
        TShape *Shape9;
        TRadioGroup *ViewRadioGroup;
        TRadioGroup *ZoomRadioGroup;
        TImage *ZoomImage;
        TShape *ZoomShape;
        TColorDialog *ColorDialog1;
        TMenuItem *MarchingCubes1;
        TMenuItem *Ajuste1;
        TMenuItem *Cilindro1;
        TMenuItem *Esfera1;
    TPageControl *PageControl1;
        TTabSheet *TabSheet1;
        TTabSheet *TabSheet2;
        TCheckBox *NegativoCheckBox;
        TChart *HistoChart;
        TBarSeries *Series1;
        TBarSeries *Series2;
        TGroupBox *WindowGroupBox;
        TLabel *CenterLabel;
        TLabel *MaxPxLabel;
        TLabel *MinPxLabel;
        TLabel *WidthLabel;
        TTrackBar *CenterTrackBar;
        TEdit *CenterEdit;
        TEdit *WidthEdit;
        TTrackBar *WidthTrackBar;
        TCheckBox *CropCheckBox;
        TCheckBox *FixCheckBox;
        TCheckBox *Goma3DCheckBox;
        TEdit *Goma3DEdit;
        TCheckBox *VolcarCheckBox;
        TButton *DoneButton;
        TEdit *ResaltarEdit;
        TButton *ElimButton;
        TCheckBox *EsconderCheckBox;
        TButton *EliminaRenderButton;
        TButton *GuardarPTSbutton;
        TButton *CargarPTSbutton;
        TCheckBox *ParedesCheckBox;
        TButton *MascaraButton;
        TLabel *CantPuntosSegLabel;
        TLabel *CantPuntosLabel;
        TLabel *VolLabel;
        TLabel *VolRenderLabel;
        TLabel *ElevacionLabel;
        TButton *XYButton;
        TButton *EsqueletoButton;
        TCheckBox *SelectEsqCheckBox;
        TCheckBox *UnirCheckBox;
        TButton *EliminarEsqButton;
        TListBox *AP;
        TTrackBar *AtenuacionTrackBar;
        TMenuItem *Suma1;
        TEdit *PxDistEdit;
        TLabel *Label2;
        TLabel *Label3;
        TMenuItem *PasaBajos1;
        TLabel *HistoProcessLabel;
        TCheckBox *SmoothCheckBox;
        TCheckBox *AjusteCheckBox;
        TButton *DesmarcarButton;
        TMenuItem *VOI2;
        TMenuItem *Global2;
        TTrackBar *MCFaceOpacityTrackBar;
        TMenuItem *PasaAltos1;
        TMenuItem *VOI3;
        TMenuItem *Global3;
        TRadioGroup *CursorRadioGroup;
        TButton *SetCursorButton;
        TButton *VistaOBButton;
        TButton *FlipCursorButton;
        TPanel *ArbitraryCutControlPanel;
        TScrollBar *ArbitraryCutScrollBar;
        TScrollBar *PuntoCentralScrollBar;
        TButton *ParaOrthoButton;
        TButton *UpAPButton;
        TLabel *Label4;
        TLabel *Label5;
        TMenuItem *Normal1;
        TMenuItem *Membrana1;
        TMenuItem *GrabarVistasOblicuas1;
        TMenuItem *VOI4;
        TMenuItem *Global4;
        TCheckBox *CACCheckBox;
        TImage *TranspImage;
        TMenuItem *CAC1;
        TMenuItem *N1;
        TMenuItem *N2;
        TMenuItem *N3;
        TMenuItem *N4;
        TMenuItem *N5;
        TMenuItem *N6;
        TCheckBox *CheckBox1;
        TButton *GuardarEsqButton;
        TMenuItem *Ventana1;
        TMenuItem *Hueso1;
        TMenuItem *Mediastino1;
        TMenuItem *Pulmn1;
        TMenuItem *N7;
        TMenuItem *AbrirconSobremuestreo1;
        TMenuItem *Bilateral1;
        TMenuItem *Sobel1;
        TMenuItem *X1;
        TMenuItem *Y1;
        TMenuItem *Z1;
        TButton *LabelButton;
        TButton *CentrarButton;
        TCheckBox *EsqOnTopCheckBox;
        TLabel *ValorElLabel;
        TLabel *AzimuthLabel;
        TLabel *ValorAzLabel;
        TLabel *Label1;
        TLabel *Label6;
        TLabel *Label12;
        TLabel *Label13;
        TLabel *DistanciaLabel;
        TLabel *ValorDistLabel;
        TLabel *MediaRenderLabel;
        TLabel *DesvRenderLabel;
        TMenuItem *Anisotrpico1;
        TMenuItem *Canny1;
        TMenuItem *Houghcircular1;
        TLabel *ArchivoLabel;
        TMenuItem *InvertirEstudio1;
        TButton *CACWindowButton;
        TMenuItem *Segmentacin1;
        TMenuItem *Carina1;
        TMenuItem *N2D1;
        TMenuItem *CACD1;
        TMenuItem *CirculoManual1;
        TMenuItem *Baston1;
        TScrollBar *ToroideScrollBar;
        TLabel *Label14;
        TMenuItem *GuardarPlanosToroidales1;
        TMenuItem *Slice1;
        TMenuItem *CirculoCavityFilling1;
        TMenuItem *AortaCavityFilling1;
        TMenuItem *N8;
        TMenuItem *GuardarCarina1;
        TMenuItem *AbrirCarina1;
        TMenuItem *GraficarCenterline1;
        TMenuItem *N9;
        TMenuItem *GuardarCenterline1;
        TMenuItem *GuardarPlanosOblicuos1;
        TMenuItem *GuardarModelo1;
        TMenuItem *Aorta2;
        TMenuItem *AxialCavityFilling1;
        TShape *RShape7;
        TShape *RShape8;
        TMenuItem *Texturas1;
        TMenuItem *Setup1;
        TCheckBox *CheckBox2;
        TScrollBar *MinzScrollBar;
        TScrollBar *MaxzScrollBar;
        TLabel *Label15;
        TLabel *Label16;
        TMenuItem *AbrirModelo1;
        TTabSheet *TabSheet3;
        TButton *CarinaButton;
        TButton *CACDButton;
        TButton *CreateToroidButton;
        TButton *SegmentDAButton;
        TButton *SegmentArchButton;
        TButton *OrthoCorrectionButton;
        TGroupBox *AorticRegionsGroupBox;
        TCheckBox *AnnulusCheckBox;
        TLabel *AnnulusLabel;
        TCheckBox *CACheckBox;
        TLabel *CALabel;
        TCheckBox *BCACheckBox;
        TLabel *BSALabel;
        TCheckBox *LSACheckBox;
        TLabel *LSALabel;
        TCheckBox *CDCheckBox;
        TLabel *CDLabel;
        TCheckBox *ApexSinusCheckBox;
        TLabel *ApexSinusLabel;
        TCheckBox *DiaphragmCheckBox;
        TLabel *DiaphragmLabel;
        TTrackBar *CSATrackBar;
        TPanel *MPRPanel;
        TImage *ObliqueImage;
        TRadioGroup *VistaRadioGroup;
        TLabel *Label17;
        TLabel *Label18;
        TLabel *Label19;
        TLabel *Label20;
        TTrackBar *PasoTrackBar;
        TLabel *Label21;
        TEdit *RXEdit;
        TLabel *RXLabel;
        TEdit *RYEdit;
        TLabel *RYLabel;
        TEdit *RZEdit;
        TLabel *RZLabel;
        TEdit *PasoEdit;
        TLabel *Label22;
        TCheckBox *ZnormCheckBox;
        TProgressBar *ProgressBar1;
        TLabel *ProcessLabel;
        TLabel *Label23;
        TButton *SaveModelButton;
        TButton *LoadModelButton;
        TButton *PrintReportButton;
        TCheckBox *AutoCheckBox;
        TMenuItem *Malla1;
        TButton *Button1;
        TCheckBox *TresDCheckBox;
        TButton *Button2;
        TCheckBox *RGCheckBox;
        TCheckBox *BinCheckBox;
    TCheckBox *CLCheckBox;
    TButton *UCSButton;
    TListBox *ListBox1;
    TButton *Button3;
    TTabSheet *TabSheet4;
    TButton *Button4;
    TButton *Button5;
    TButton *Button6;
        void __fastcall Cerrar1Click(TObject *Sender);
        void __fastcall Salir1Click(TObject *Sender);
        void __fastcall ROIShapeMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall ROIShapeMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall Guardar1Click(TObject *Sender);
        void __fastcall Cargar1Click(TObject *Sender);
        void __fastcall PantallaMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall ROICheckBoxClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall XYButtonClick(TObject *Sender);
        void __fastcall AbrirVolumen1Click(TObject *Sender);
        void __fastcall SliceListBoxClick(TObject *Sender);
        void __fastcall ImageScrollBarChange(TObject *Sender);
        void __fastcall NegativoCheckBoxClick(TObject *Sender);
        void __fastcall Mediana1Click(TObject *Sender);
        void __fastcall AperturaClick(TObject *Sender);
        void __fastcall PantallaMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall GuardarVolumen1Click(TObject *Sender);
        void __fastcall Histograma1Click(TObject *Sender);
        void __fastcall Resta1Click(TObject *Sender);
        void __fastcall GrabarAC1Click(TObject *Sender);
        void __fastcall Segmentacion21Click(TObject *Sender);
        void __fastcall EliminarPulmon1Click(TObject *Sender);
        void __fastcall LUT1Click(TObject *Sender);
        void __fastcall Goma3DCheckBoxClick(TObject *Sender);
        void __fastcall DoneButtonClick(TObject *Sender);
        void __fastcall Clustering1Click(TObject *Sender);
        void __fastcall KMeansClusterClick(TObject *Sender);
        void __fastcall GuardarPTSbuttonClick(TObject *Sender);
        void __fastcall SegmentaCorazn1Click(TObject *Sender);
        void __fastcall EliminaRenderButtonClick(TObject *Sender);
        void __fastcall CargarPTSbuttonClick(TObject *Sender);
        void __fastcall Cierre1Click(TObject *Sender);
        void __fastcall Umbral1Click(TObject *Sender);
        void __fastcall ElimButtonClick(TObject *Sender);
        void __fastcall SquareTracing1Click(TObject *Sender);
        void __fastcall Global1Click(TObject *Sender);
        void __fastcall Erosion1Click(TObject *Sender);
        void __fastcall Dilatacin1Click(TObject *Sender);
        void __fastcall ROIPanelMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall ROIPanelMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall ROIPanelMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall TabSheetChange(TObject *Sender);
        void __fastcall YScrollBarChange(TObject *Sender);
        void __fastcall RXEditChange(TObject *Sender);
        void __fastcall MascaraButtonClick(TObject *Sender);
        void __fastcall FixCheckBoxClick(TObject *Sender);
        void __fastcall EsqueletoButtonClick(TObject *Sender);
        void __fastcall EliminarEsqButtonClick(TObject *Sender);
        void __fastcall RYEditChange(TObject *Sender);
        void __fastcall RZEditChange(TObject *Sender);
        void __fastcall SelectEsqCheckBoxClick(TObject *Sender);
        void __fastcall MarchingCubes1Click(TObject *Sender);
        void __fastcall Suma1Click(TObject *Sender);
        void __fastcall HistoChartDblClick(TObject *Sender);
        void __fastcall CenterTrackBarChange(TObject *Sender);
        void __fastcall WidthTrackBarChange(TObject *Sender);
        void __fastcall CenterEditChange(TObject *Sender);
        void __fastcall WidthEditChange(TObject *Sender);
        void __fastcall Cilindro1Click(TObject *Sender);
        void __fastcall DesmarcarButtonClick(TObject *Sender);
        void __fastcall Esfera1Click(TObject *Sender);
        void __fastcall Global2Click(TObject *Sender);
        void __fastcall VOI2Click(TObject *Sender);
        void __fastcall VOI3Click(TObject *Sender);
        void __fastcall SetCursorButtonClick(TObject *Sender);
        void __fastcall VistaOBButtonClick(TObject *Sender);
        void __fastcall ArbitraryCutScrollBarChange(TObject *Sender);
        void __fastcall FlipCursorButtonClick(TObject *Sender);
        void __fastcall ParaOrthoButtonClick(TObject *Sender);
        void __fastcall PuntoCentralScrollBarChange(TObject *Sender);
        void __fastcall CropCheckBoxClick(TObject *Sender);
        void __fastcall UpAPButtonClick(TObject *Sender);
        void __fastcall Normal1Click(TObject *Sender);
        void __fastcall Membrana1Click(TObject *Sender);
        void __fastcall GrabarVistasOblicuas1Click(TObject *Sender);
        void __fastcall VOI4Click(TObject *Sender);
        void __fastcall Global4Click(TObject *Sender);
        void __fastcall PruebaFiltro1Click(TObject *Sender);
        void __fastcall CACCheckBoxClick(TObject *Sender);
        void __fastcall CAC1Click(TObject *Sender);
        void __fastcall GuardarEsqButtonClick(TObject *Sender);
        void __fastcall Hueso1Click(TObject *Sender);
        void __fastcall Mediastino1Click(TObject *Sender);
        void __fastcall Pulmn1Click(TObject *Sender);
        void __fastcall AbrirconSobremuestreo1Click(TObject *Sender);
        void __fastcall X1Click(TObject *Sender);
        void __fastcall Y1Click(TObject *Sender);
        void __fastcall LabelButtonClick(TObject *Sender);
        void __fastcall CentrarButtonClick(TObject *Sender);
        void __fastcall Anisotrpico1Click(TObject *Sender);
        void __fastcall Canny1Click(TObject *Sender);
        void __fastcall Houghcircular1Click(TObject *Sender);
        void __fastcall InvertirEstudio1Click(TObject *Sender);
        void __fastcall CACWindowButtonClick(TObject *Sender);
        void __fastcall N2D1Click(TObject *Sender);
        void __fastcall CACD1Click(TObject *Sender);
        void __fastcall CirculoManual1Click(TObject *Sender);
        void __fastcall ToroideScrollBarChange(TObject *Sender);
        void __fastcall GuardarPlanosToroidales1Click(TObject *Sender);
        void __fastcall Slice1Click(TObject *Sender);
        void __fastcall CirculoCavityFilling1Click(TObject *Sender);
        void __fastcall AortaCavityFilling1Click(TObject *Sender);
        void __fastcall GuardarCarina1Click(TObject *Sender);
        void __fastcall AbrirCarina1Click(TObject *Sender);
        void __fastcall GraficarCenterline1Click(TObject *Sender);
        void __fastcall GuardarCenterline1Click(TObject *Sender);
        void __fastcall GuardarPlanosOblicuos1Click(TObject *Sender);
        void __fastcall GuardarModelo1Click(TObject *Sender);
        void __fastcall Aorta2Click(TObject *Sender);
        void __fastcall AxialCavityFilling1Click(TObject *Sender);
        void __fastcall CirculoAxialCavityFilling1Click(TObject *Sender);
        void __fastcall Setup1Click(TObject *Sender);
        void __fastcall MinzScrollBarChange(TObject *Sender);
        void __fastcall MaxzScrollBarChange(TObject *Sender);
        void __fastcall PasoTrackBarChange(TObject *Sender);
        void __fastcall AbrirModelo1Click(TObject *Sender);
        void __fastcall CarinaButtonClick(TObject *Sender);
        void __fastcall CACDButtonClick(TObject *Sender);
        void __fastcall Shape8MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall Shape7MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall CreateToroidButtonClick(TObject *Sender);
        void __fastcall SegmentDAButtonClick(TObject *Sender);
        void __fastcall SegmentArchButtonClick(TObject *Sender);
        void __fastcall OrthoCorrectionButtonClick(TObject *Sender);
        void __fastcall CSATrackBarChange(TObject *Sender);
        void __fastcall AnnulusCheckBoxClick(TObject *Sender);
        void __fastcall CACheckBoxClick(TObject *Sender);
        void __fastcall BCACheckBoxClick(TObject *Sender);
        void __fastcall LSACheckBoxClick(TObject *Sender);
        void __fastcall CDCheckBoxClick(TObject *Sender);
        void __fastcall ApexSinusCheckBoxClick(TObject *Sender);
        void __fastcall DiaphragmCheckBoxClick(TObject *Sender);
        void __fastcall VistaRadioGroupClick(TObject *Sender);
        void __fastcall SaveModelButtonClick(TObject *Sender);
        void __fastcall LoadModelButtonClick(TObject *Sender);
        void __fastcall Malla1Click(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
    void __fastcall UCSButtonClick(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall Button3Click(TObject *Sender);
    void __fastcall Button4Click(TObject *Sender);
    void __fastcall Button5Click(TObject *Sender);
    void __fastcall Button6Click(TObject *Sender);

private:
	// User declarations

        int xP; //Almacena coord X del punto donde se seleccionó la ROI
        int yP; //Almacena coord Y del punto donde se selecciono la ROI
        int zP; //Almacena el corte (coord Z) del punto donde se seleccionó la ROI
        float r0; //valor de radio de aorta auxiliar, por defecto vale 15

    //Medición de Rendimientos:
    clock_t start, end;

    //Necesarios para graficar con OpenGL
    HDC hDC;
    HGLRC hRC;
    int PixelFormat;
    int PanelHeight;
    int PanelWidth;
    
      GLfloat x, y, xstep, ystep, w, h;
      GLsizei size;
    //  Graphics::TBitmap* bitmap;
   GLuint textura1;
   GLuint textura2;
   GLuint textura3;
   GLuint texturaOB; //Para corte Oblicuo


   GLuint tex1;
   GLuint tex2;
   GLuint tex3;
   GLuint tex4;
   GLuint tex5;
   GLuint tex6;
   GLuint tex7;

   float lut2[256][3];

   int cantTexturas;

   GLuint* texturas;

   GLuint lutex;

   Graphics::TBitmap* bitmap;

      float factorX;
      float factorY;
      float factorZ;

      //  void FiltroContornos();
      //  void ModificarVertices();
        AnsiString dir;  //Almacena directorio de las imágenes
        AnsiString ext;  //Almacena tipo de extensión (.BMP)
        AnsiString name; //Almacena el nombre de los archivos (no el ~numero)
      //  int zeros;//Almacena la cantidad de ceros antes del primer "1":
                  //ej: DicomWorks guarda los BMP convertidos en la forma:
                  //"Nombre~000n" al aumentar se tranforma en  "Nombre~00mn" etc

      //  AnsiString zeroString;//Array con ceros antes del numero
        bool ImagenEnPantalla; //Indica si hay imagenes en pantalla

        bool ModificaX; //Indica si la ROI se está agrandando/achicando en X
        bool ModificaY; //Indica si la ROI se está agrandando/achicando en Y

        bool ModificaEsq;
        bool ModificaLado;
        bool ModificaCentro;
        int  Lado;
           /*
         3----4----5
         |         |
         2    C    6
         |         |
         1----8----7
         */
        //Indica si el cubo rojo está siendo arrastrado.
        bool ModificaCentro2;//Indica si el cubo verde está siendo arrastrado

        bool GraficarResultados;//Indica si se deben graficar las 3 vistas en la otra solapa
        bool RotandoImagen; //Indica si la imagen está siendo rotada
        bool RenderFlag;
        bool SimpleFlag;

        //Pintar LUT:
     __int32 colorLUT;
     unsigned char rojo, verde, azul;
     __int32 redmask;
     __int32 greenmask;
     __int32 bluemask;

        //Pintar LUT:
     __int32 colorLUT2;
     unsigned char rojo2, verde2, azul2;

         //Pintar LUT:
     __int32 colorLUT3;
     unsigned char rojo3, verde3, azul3;

         //Variables de Ventaneo:
         short int Center; //Centro de la ventana
         short int Width;  //Ancho de la ventana
         short int Li;     //Nivel inferior de la ventana: C-W
         short int Ls;     //Nivel superior de la ventana: C+W

        //Variables para modificar ViewPort de OpenGL
        GLfloat xCameraAngle, xCameraPos, yCameraAngle, yCameraPos, zCameraPos;

        //Matriz Principal: sobre Cuadro se efectúan los filtros
        Matriz* Cuadro;

        //Matrices para vistas ortogonales y zoom:
        Matriz* CuadroXY;
        Matriz* CuadroYZ;
        Matriz* CuadroZX;
        Matriz* CuadroZoom;
        Matriz* CuadroCorteOblicuo;

        bool firstRender;
        AnsiString ShaderDir;

        //Matrices auxiliares
      //  Matriz Cuadro_trans;
      //  Matriz Auxiliar;
      //  Matriz *CuadroOblicuo;
      //  Matriz *CuadroOrtho;
        int zindex;

        //variables de color: por si se desea color plano o un gradiente continuo en Z (Vista 3D)
        float color[3];
        float zi;
        float zINT[6];

        int CantPuntos; //Cantidad total de puntos;

        //maxX,maxY: máximo valor de X e Y
        float maxX;
        float maxY;
        float maxZ;

        //Esqueletonizacion:
        TList* EsquelPoints; //Puntos del esqueleto completo. 
        TList* EP; //End Points
        TList* LP; //line points
        TList* JP; //joint points
        TList* BP; //Puntos a unir por bresenham3D
        TList* EraseP; //Puntos a borrar. 

        //minX,minY: mínimo valor en X e Y
        float minX;
        float minY;
        float minZ;

        //med = (max+min)/2
        float medX;
        float medY;
        float medZ;
        //dist = max-min
        float distX;
        float distY;
        float distZ;

        float mean;      //MEdia y desvio del punto sobre el cual flota el puntero.
        float StD;
        float meanSel;   //MEdia y desvio del punto xP,yP seleccionado para segmentar
        float StDSel;

        //resolución del estudio: RX, RY, RZ
        float res[3];

        //variables de rotacion
        float elevacion,azimuth;
        GLdouble *viewmatrix;
        float modelview[16];

        //Variables de ampliación
        float distancia;
        bool Aumentando;

        //Variables de proyección: permite mapear los puntos rotados para la Goma3D
        float XV,YV;

        calcHistoThread* histo_thread;

        TList* RenderPoints;

        float tamanioPixel;

        //Sirven para borrar
        TList* RenderRotatedPoints;
        TList* MapaIndices[750][650];
        TList* IndicesBorrar;

        //Tamaño de la Goma3D
        float Goma3DCoef;

        //variables de traslación: modificados por flechas laterales de Ventana3D
        float trHor, trVert;
        float xTrans, yTrans;
        bool Transladando;

        //Variables de Ajuste
      //  float cil[7]; //cil[0]:vx; cil[1]:vy; cil[2]:P0x; cil[3]:P0y; cil[4]:R; cil[5]:func; cil[6]:Niter
      //  float cilR[6]; //Igual que cil, pero considera la resolución en X,Y,Z
      //  float P1[3];  //Puntos proyectados sobre (vx,vy,1): marcan los extremos del cilindro
      //  float P2[3];
      //  bool cilFlag; //variable auxiliar: puede indicar si el cilindro está dibujado en pantalla.
      //  float signoZ; //Variable que permite indicar el sentido del vector.
                      //Sirve para obtener el ángulo correcto en el producto escalar. Toma valor 1 ó -1.

      //  float vLCX[3]; //Almacena v=(vx,vy,vz)
      //  float vLAD[3];
      //  float vLM[3];

        //Variables utilizadas para la medicion manual (planar) del ángulo
        int centX;
        int centY;
        int angX1;
        int angY1;
        int angX2;
        int angY2;
        bool auxiliar;
                //Variable que permite reconocer cuantas veces se ha clickeado sobre la imagen
                int contClicks;

        // Visualización de volumen
        short int maxPX, minPX; //almacena max y min valor de px en HU
        short int realminPX;

        float* LUT;
        bool histo_thread_terminado;

        float* LUT_Sangre;
        float* LUT_Musculo;
        float* LUT_Hueso;

        int offset;
        int final;
        int paso;

        Volumen* M;

        Volumen* Mauxiliar;
        Volumen* Mvisitado;

        bool carinaDetectada;
        bool CACDDetectados;
        bool moviendoCACD;

        bool isAortaSegmented;
        bool isCenterLineDetected;
        bool isModelComplete;

        int annulusIndex;
        int CAIndex;
        int CDIndex;
        int BCAIndex;
        int LSAIndex;
        int apexSinusIndex;
        int diaphragmIndex;



        Volumen* MToroide; //Guarda cortes toroidales
        Volumen* MPerp;    //Guarda cortes perpendiculares

        short int cantImagenes;
        short int minFrame; //Sirve para estudios incompletos
        short int anchoPX, altoPX;
        GLfloat* interwined;
        short int pasoX;
        short int pasoY;
        short int pasoZ;

        int chosenAgent;
        //Agents==Shapes: 1:XYrojo 2:XYverde 3:XYazul 4:YZr 5:YZv
        //                6:YZa 7:XZr 8:XZv 9:XZa
        bool agentWasJustChosen;  //Algun Shape fue seleccionado?
        int shapeDownX;           //Valor anterior de X
        int shapeDownY;           //Valor anterior de Y
        float centro[3];
        float centro2[3];
        float Dcentro[3];  //deltaCentro: traslacion del POI
        float pV[3];
        float pR[3];
        float norm1[3];
        float norm2[3];
        float RotMatrix[3][3]; //Matriz de rotacion arbitraria

        //Marching Cubes:
        TList* VertexList;
        TList* TriangColors;
        TList* Normals;

        TList* TriangList; //Lista de triangulos: T = (i1, i2, i3), donde los indices
                           //son la posicion de los verices en VertexList

        TList* TriList;                   

      //Segmentación:
      float* histoCaja;
      float* histoOrdenada;
        float* mu;    //mu, omega: momentos de orden 0 y 1 para algoritmo de otsu
          float* omega;
          TList* AAC;  //Guarda todos los elementos adentro de AC para cada cubo

    CV** arrayCV;

    //Bordes:
    TList* BorderPoints;


     //Ajustes:
     TList* SelectedPoints;

        //Cilíndrico:
        TList* CilindrosList;
        PuntoF cg;
        PuntoF princvec;
        bool dibujaCils;

        //Esférico:
        TList* EsferasList;
        bool dibujaEsfs;

        //Array Auxiliar para filtros
        //short int auxArray[512][512];
        short int auxArray[512][512];
        short int*** auxVol;

        PuntoF C1, C2;
        PuntoF Cc; //Punto central entre C1 y C2
        PuntoF vector1;
        PuntoF vector2; //Vector de corte arbitrario
        PuntoF PC1;
        PuntoF vectorN;
        PuntoF** PlanoCorte;
        PuntoF** PlanoOrig;
        float R[3][3];
        float anguloRotArb;

        TList* CurvaToroide;
        TList* NormalToroide;

        //Marca los lugares con posible (true) score cálcico.
        //El mismo se detecta como HU>=130 (Gold Standard Actual);
        bool* CAC;

        TList* NodeList;
        TList* RamaList;
        TList* RamaSmoothList;
        TList* VolRamaList;

        int** matrizNodoRama;

        double* LargosRama;
        double* LargosSmoothRama;
        double* VolumenRama;

        double* LargosAcum;
        double* VolAcum;

        double volVoxel;

        double minPuntosRama;
        double maxPuntosRama;

        double mediaRender;
        double desvRender;

        //Connected Component Labelling:
        TList* ConnectedComponentList;



public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
        void __fastcall IdleLoop(TObject*, bool&);
        void __fastcall RenderGLScene();
        void __fastcall DrawObjects();
        void __fastcall GraficarEnPantalla();
        void __fastcall SetArbitraryPlane();
        void __fastcall SetToroidalPlane();
        void __fastcall SetToroidalPlaneFromIndex(int index);
        void __fastcall SetCenterLinePlaneFromIndex(int index, float alfa);
        void __fastcall RefreshOrtho(TObject *Sender);
        void __fastcall RefreshLUT();
        void __fastcall ApplyMasc333(float msc[3][3][3], int xi, int xf, int yi, int yf, int zi, int zf);

        void __fastcall AxialCavityFilling(Volumen* MV);
        void __fastcall AortaCavityFilling(Volumen* MV);
        void __fastcall CirculoAxialCavityFilling(Volumen* MV);
        void __fastcall FiltroMediana(Volumen* MV);

        float __fastcall Isodata(Volumen* MV, int z, float mu, float sigma, float n);

        void __fastcall AperturaMorfologica(int r, int z, Volumen* MV);

        void orientarRamas(int n, int r);
        void prunning();
        float CostoHough(float x1, float y1, float r1, float H1,  float x2, float y2, float r2);
        float* Dijkstra(TList* G, int source);

        void CentroGravedad(TList* ListaDePuntos, float& cgx, float& cgy, float& cgz);
        float CalculoEnergia(TList* ListaDePuntos, float* weightsX, float* weightsY, float* weightsZ);

        float Perimetro(TList* ListaDePuntos, Volumen* MAux);

        int __fastcall CrearTexturas();
        void __fastcall ActualizarTextura3D(int z); //Llena la textura 3D corte a corte
        void __fastcall LlenarPlanoConMatriz(int z); //Llena el plano con el que llena la textura 3D

        void __fastcall DetectCarina(Volumen* Maux, Volumen* MAux);
        void __fastcall DetectCACD(Volumen* Maux, Volumen* MAux);
        void __fastcall FiltroBilateral(Volumen* Maux);
        void __fastcall FiltroPasaAltos(Volumen* Maux);

        void __fastcall CrearBaston(float x1, float y1, float x2, float y2);

        void __fastcall SegmentarAorta(float zi, float zf, float seedX, float seedY, float seedR, float nsigmas0, Volumen* MV);
        void __fastcall GetAorticAxis();
        void __fastcall GetAorticArch();
        void __fastcall SmoothCenterLine();
        bool __fastcall IsModelComplete();

        bool __fastcall MostrarVolumenTexturas3D();
        float bbXmin;
        float bbXmax;
        float bbYmin;
        float bbYmax;
        float bbZmin;
        float bbZmax;

        bool mostrarTex3D;
        int tfSize;

        unsigned int texName[2]; //Espacio para guardar el alias de 2 texturas

        GLushort* plano; //Plano explorador donde se reproducirá parte de una textura 3D

        unsigned int shader_num;

        GLfloat* pTransferFunction; //Puntero a una transfer function
        GLfloat* pTF;  //Puntero a otra transfer function

        TList* TraqueaList;
        float xCarina, yCarina, zCarina; //Punto central de la carina
        int x0,y0,xf,yf,x02,y02,xf2,yf2;

        float nsigmas;

        float xCA, xCD, yCA, yCD;
        float rA, rD;

        float muAorta;
        float sigmaAorta;
        float nAorta;
        bool usarAortaProm;

        TList* AortaCenterLine;
        TList* AortaNormales;
        TList* AortaRadios;

        bool drawAorta;

        bool  meshOK;

        TList* Faces;
        TList* Edges;
        TList* Nodes;

        TList* Meshes;

        TList* ManualCenterLine;
        TList* UFCSpline;
        TList* NormalesSpline;

        double t;
        bool empuje;

        double meanC;
        double stdC;

        bool activemesh;

        TList* PointMap;


};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
