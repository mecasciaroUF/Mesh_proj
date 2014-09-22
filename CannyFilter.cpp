//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "CannyFilter.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCannyFilterForm *CannyFilterForm;
//---------------------------------------------------------------------------
__fastcall TCannyFilterForm::TCannyFilterForm(TComponent* Owner)
        : TForm(Owner)
{
          automatico = true;
          vM = 500;
          vm = 250;
          pM = 90;
          pm = 60;
}
//---------------------------------------------------------------------------

void __fastcall TCannyFilterForm::RadioGroup1Click(TObject *Sender)
{
        if(RadioGroup1->ItemIndex == 0)
        {
        //Cambia y luego restaura el Tag para que no se afecte por el
        //evento de OnChange de los TEdits correspondientes:
        automatico = true;
        RadioGroup1->Tag = -1;
        SupEdit->Text = pM;
        InfEdit->Text = pm;
        SupLabel->Caption = "% superior:";
        InfLabel->Caption = "% inferior:";
        RadioGroup1->Tag = 0;
        }

        if(RadioGroup1->ItemIndex == 1)
        {
        automatico = false;
        RadioGroup1->Tag = -1;
        SupEdit->Text = vM;
        InfEdit->Text = vm;
        SupLabel->Caption = "Máximo";
        InfLabel->Caption = "Mínimo";
        RadioGroup1->Tag = 0;
        }
}
//---------------------------------------------------------------------------
void __fastcall TCannyFilterForm::SupEditChange(TObject *Sender)
{
        if(RadioGroup1->ItemIndex == 0 && RadioGroup1->Tag == 0)
        {
        pM = atof(SupEdit->Text.c_str());
        pm = atof(InfEdit->Text.c_str());
        }

        if(RadioGroup1->ItemIndex == 1 && RadioGroup1->Tag == 0)
        {
        vM = atof(SupEdit->Text.c_str());
        vm = atof(InfEdit->Text.c_str());
        }
}
//---------------------------------------------------------------------------
