//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <ComCtrls.hpp>
#include <FileCtrl.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TListBox *ListBox1;
        TButton *AbrirButton;
        TOpenDialog *OpenDialog1;
        TLabel *FileDirLabel;
        TLabel *CountLabel;
        TLabel *ProgramLabel;
        TProgressBar *ProgressBar1;
        TDirectoryListBox *DirectoryListBox1;
        void __fastcall AbrirButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
