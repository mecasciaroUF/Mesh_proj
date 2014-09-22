//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AbrirButtonClick(TObject *Sender)
{
      ListBox1->Items->Clear();

      OpenDialog1->Filter = "Dicom files (*.dcm)|*.dcm";

      AnsiString FileName;
      AnsiString FileDir;

       if(OpenDialog1->Execute())
        {
         FileName = OpenDialog1->FileName ;
         FileDir = ExtractFileDir(FileName);
         FileDirLabel->Caption = FileDir;
        }
       else return;

     TSearchRec sr;

     if (FindFirst(FileDir+"\\*.*", faAnyFile, sr) == 0)
     {
      do
      {ListBox1->Items->Add(sr.Name);
      }while (FindNext(sr) == 0);
     FindClose(sr);
     }

     CountLabel->Caption = AnsiString(ListBox1->Items->Count-2) + " Files";

     ProgressBar1->Max = ListBox1->Items->Count-2;
     AnsiString ExecProgPath;
     AnsiString FileName_Path;
     AnsiString DestinyFolder;
     char Quote = '"';

     for(int i=2; i<ListBox1->Items->Count; i++)
     {
     FileName = FileDir + "\\" + ListBox1->Items->Strings[i];
     FileName_Path = ExtractFileName(FileName);
     DestinyFolder = DirectoryListBox1->Directory;

     ExecProgPath = "D:\\Estudios\\DCMTKBIN\\dcmdjpeg.exe "
     +  AnsiQuotedStr(FileName, Quote) +
     + " "
     +  AnsiQuotedStr(DestinyFolder + "\\" + FileName_Path, Quote);

     WinExec(ExecProgPath.c_str() ,  SW_HIDE);
     ProgramLabel->Caption =  ExecProgPath;
     ProgressBar1->Position++;
     }

     ProgressBar1->Position = 0;
}
//---------------------------------------------------------------------------

