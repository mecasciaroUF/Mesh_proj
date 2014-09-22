//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "LabelForm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TLabelForm1 *LabelForm1;
//---------------------------------------------------------------------------
__fastcall TLabelForm1::TLabelForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TLabelForm1::Button1Click(TObject *Sender)
{

    Form1->orientarRamas(NodeListBox->ItemIndex, RamaListBox->ItemIndex);
    
}
//---------------------------------------------------------------------------

void __fastcall TLabelForm1::GrabarMapaButtonClick(TObject *Sender)
{
        if(SaveDialog1->Execute())
        {
         FILE* fp;
         AnsiString FileName = SaveDialog1->FileName;

         fp=fopen(FileName.c_str(),"w");

         AnsiString valor;
         char separador = ';';
         char prox = '\n';

         for(int j=0; j<StringGrid1->RowCount; j++)
         {
           for(int i=0; i<StringGrid1->ColCount; i++)
           {
           valor = StringGrid1->Cells[i][j];
           fwrite(valor.c_str(),sizeof(valor),1,fp);
           fwrite(&separador,sizeof(char),1,fp);
           }
           fwrite(&prox,sizeof(char),1,fp);
         }

         fclose(fp);
        }//end if(SaveDialog->Execute)
}
//---------------------------------------------------------------------------

void __fastcall TLabelForm1::PodaButtonClick(TObject *Sender)
{
     Form1->prunning();
}
//---------------------------------------------------------------------------

