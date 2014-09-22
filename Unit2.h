//---------------------------------------------------------------------------

#ifndef Unit2H
#define Unit2H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TForm2 : public TForm
{
__published:	// IDE-managed Components
        TEdit *umbralEdit;
        TEdit *radio0Edit;
        TEdit *umbralPCEdit;
        TLabel *umbralLabel;
        TLabel *radio0Label;
        TLabel *umbralPCLabel;
        TButton *AceptaButton;
        TButton *CancelaButton;
private:	// User declarations
public:		// User declarations
        __fastcall TForm2(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm2 *Form2;
//---------------------------------------------------------------------------
#endif
