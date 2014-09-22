//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class DrawThread : public TThread
{            
private:
        TList* RenderPoints;
        float *LUT,*LH,*LM,*LS;
        int k0, paso;

        short int medX, medY, medZ;
        short int minPX;

        float tamanioPixel;
        float umbralGrad;

protected:

public:
        void __fastcall Execute();

        void __fastcall SetParams(TList*, float*, float*, float*, float*,
                                  short int, short int, short int, short int,
                                  float, int, int, float);
        __fastcall DrawThread(bool CreateSuspended);
};
//---------------------------------------------------------------------------
#endif
 