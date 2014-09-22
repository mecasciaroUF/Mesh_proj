//---------------------------------------------------------------------------

#ifndef ArbitraryCutFormH
#define ArbitraryCutFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

//---------------------------------------------------------------------------
class TArbitraryCutForm1 : public TForm
{
__published:	// IDE-managed Components
        TPanel *ArbitraryCutPlane;
        TImage *Image1;
        TShape *CenterShape;
        TShape *ROIShape;
        TShape *SectionShape;
private:	// User declarations
public:		// User declarations
        __fastcall TArbitraryCutForm1(TComponent* Owner);
         float anguloRotArb;
};
//---------------------------------------------------------------------------
extern PACKAGE TArbitraryCutForm1 *ArbitraryCutForm1;
//---------------------------------------------------------------------------
#endif
