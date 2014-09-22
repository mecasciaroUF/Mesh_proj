//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TListBox *PointsListBox;
        TListBox *ListBox1;
private:	// User declarations

public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
        float *PX;
        float *PY;
        float *PZ;
        float CantPuntos;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
 