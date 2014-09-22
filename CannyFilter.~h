//---------------------------------------------------------------------------

#ifndef CannyFilterH
#define CannyFilterH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TCannyFilterForm : public TForm
{
__published:	// IDE-managed Components
        TEdit *SupEdit;
        TLabel *SupLabel;
        TEdit *InfEdit;
        TLabel *InfLabel;
        TRadioGroup *RadioGroup1;
        TButton *OKButton;
        TButton *CancelButton;
        void __fastcall RadioGroup1Click(TObject *Sender);
        void __fastcall SupEditChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TCannyFilterForm(TComponent* Owner);
        float pM, pm; //percentil máximo y mínimo del histograma de gradientes
        float vM, vm; //valor de umbral máximo y mínimo

        bool automatico;

};
//---------------------------------------------------------------------------
extern PACKAGE TCannyFilterForm *CannyFilterForm;
//---------------------------------------------------------------------------
#endif
