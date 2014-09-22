//---------------------------------------------------------------------------

#ifndef CACFormH
#define CACFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>

//---------------------------------------------------------------------------
class TCACForm1 : public TForm
{
__published:	// IDE-managed Components
        TUpDown *UpDown1;
        TLabel *CACSliceLabel;
        TLabel *CACLabel;
        TLabel *CACResultLabel;
        TButton *TerminarCACButton;
        TPanel *CACPanel;
        TImage *CACImage;
        TImage *TranspCACImage;
        TImage *SelectionImage;
        TLabel *CACProcessLabel;
        TProgressBar *CACProgressBar;
        TButton *GuardarCACButton;
        TListBox *ListBox1;
        TSaveDialog *SaveDialog1;
        TShape *Shape1;
        TCheckBox *CheckBox1;
        TTrackBar *TrackBar1;
        TListBox *XListBox;
        TListBox *YListBox;
        TListBox *ZListBox;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TListBox *CACListBox;
        TLabel *Label4;
        TListBox *AreaListBox;
        TLabel *Label5;
        TListBox *MaxHUListBox;
        TLabel *MaxHULabel;
        TButton *BorrarScoreButton;
        void __fastcall FormActivate(TObject *Sender);
        void __fastcall UpDown1Click(TObject *Sender, TUDBtnType Button);
        void __fastcall SelectionImageMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall SelectionImageMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall FormDeactivate(TObject *Sender);
        void __fastcall GuardarCACButtonClick(TObject *Sender);
        void __fastcall TrackBar1Change(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall BorrarScoreButtonClick(TObject *Sender);
        void __fastcall CACSliceLabelDblClick(TObject *Sender);

private:	// User declarations
public:		// User declarations
        __fastcall TCACForm1(TComponent* Owner);
        void __fastcall GraficarEnPantalla();

        Volumen* MCAC;
        Matriz* CuadroCAC;
        short int alto;
        short int ancho;
        short int minPX;
        short int maxPX;
        int cantCortes;
        float* LUT;
        Volumen* CAC;
        bool* CACcorte;
        bool* CACSeleccion;
        
        float CACvalue;

        int Swidth;
        int Sheight;

        int seleccionado;

        //Guarda Las placas encontradas. 
        TList* ListaPlacas;

        //
        TList* ListaPuntosCAC;

        double dX, dY, dZ;

        //Umbral espacial para definir una región con calcio como placa cálcica.
        //Actualmente el Gold Standard es A>=1mm^2 => Umbral = ceil(1/dX*dY)
        float CACAreaUmbral;


};
//---------------------------------------------------------------------------
extern PACKAGE TCACForm1 *CACForm1;
//---------------------------------------------------------------------------
#endif
