//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ArbitraryCutForm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TArbitraryCutForm1 *ArbitraryCutForm1;
//---------------------------------------------------------------------------
__fastcall TArbitraryCutForm1::TArbitraryCutForm1(TComponent* Owner)
        : TForm(Owner)
{
    ArbitraryCutPlane->DoubleBuffered=true;
}
//---------------------------------------------------------------------------
