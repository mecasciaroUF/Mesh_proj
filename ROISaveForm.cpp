//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ROISaveForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TROISaveForm1 *ROISaveForm1;
//---------------------------------------------------------------------------
__fastcall TROISaveForm1::TROISaveForm1(TComponent* Owner)
        : TForm(Owner)
{
ROIRadioGroup->ItemIndex=0;
}
//---------------------------------------------------------------------------
