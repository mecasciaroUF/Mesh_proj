//---------------------------------------------------------------------------

#ifndef SegmentaFormH
#define SegmentaFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TSegmentaForm1 : public TForm
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
        __fastcall TSegmentaForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSegmentaForm1 *SegmentaForm1;
//---------------------------------------------------------------------------
#endif
