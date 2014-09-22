//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//Crea ya una instancia de cada Form para utilizar:
USEFORM("Paralax.cpp", Form1);
USEFORM("SegmentaForm.cpp", SegmentaForm1);
USEFORM("ROISaveForm.cpp", ROISaveForm1);
USEFORM("AjusteForm.cpp", AjusteForm1);
USEFORM("ArbitraryCutForm.cpp", ArbitraryCutForm1);
USEFORM("CACForm.cpp", CACForm1);
USEFORM("Unit3.cpp", BilateralForm);
USEFORM("LabelForm.cpp", LabelForm1);
USEFORM("AnisForm.cpp", AnisotropicForm);
USEFORM("CannyFilter.cpp", CannyFilterForm);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TForm1), &Form1);
                 Application->CreateForm(__classid(TSegmentaForm1), &SegmentaForm1);
                 Application->CreateForm(__classid(TROISaveForm1), &ROISaveForm1);
                 Application->CreateForm(__classid(TAjusteForm1), &AjusteForm1);
                 Application->CreateForm(__classid(TArbitraryCutForm1), &ArbitraryCutForm1);
                 Application->CreateForm(__classid(TCACForm1), &CACForm1);
                 Application->CreateForm(__classid(TBilateralForm), &BilateralForm);
                 Application->CreateForm(__classid(TLabelForm1), &LabelForm1);
                 Application->CreateForm(__classid(TAnisotropicForm), &AnisotropicForm);
                 Application->CreateForm(__classid(TCannyFilterForm), &CannyFilterForm);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
