//---------------------------------------------------------------------------

#ifndef AjusteFormH
#define AjusteFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TAjusteForm1 : public TForm
{
__published:	// IDE-managed Components
        TEdit *UsualDeltaEdit;
        TEdit *ZeroTermDeltaEdit;
        TButton *AjustarButton;
        TLabel *DeltaLabel;
        TLabel *ZTDeltaLabel;
        TButton *CancelAjustarButton;
        TRadioGroup *AutovectorRadioGroup;


private:	// User declarations
public:		// User declarations
        __fastcall TAjusteForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TAjusteForm1 *AjusteForm1;
//---------------------------------------------------------------------------
#endif
