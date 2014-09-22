//Marianittoooo

//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AnisForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAnisotropicForm *AnisotropicForm;
//---------------------------------------------------------------------------
__fastcall TAnisotropicForm::TAnisotropicForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TAnisotropicForm::KRadioGroupClick(TObject *Sender)
{
         if(KRadioGroup->ItemIndex==1)
         {
         KEdit->Enabled=true;
         KEdit->ReadOnly=false;
         return;
         }

         if(KRadioGroup->ItemIndex==0)
         {
         KEdit->Enabled=false;
         KEdit->ReadOnly=true;
         return;
         }
}
//---------------------------------------------------------------------------
