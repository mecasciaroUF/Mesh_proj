//---------------------------------------------------------------------------

#ifndef histoThreadH
#define histoThreadH

#include "Volumen.h"
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Chart.hpp>

//---------------------------------------------------------------------------
class calcHistoThread : public TThread
{            
private:
        //Thread Auxiliar para el cálculo del histograma: 
        float* histo;
        int sizeHisto;
        Volumen* V;
        short int offset;
        TChart *  chart;
        TLabel * label;
        float* LUT;
        short int* realminPX;
        bool* terminado;

protected:
        void __fastcall Execute();

public:
         __fastcall calcHistoThread(bool CreateSuspended);
       void CalcularHist();
       void GraficarHist();
       void __fastcall receiveParams(Volumen* V, float* histo, float* LUT,
                                     int sizeHisto, short int offset, TChart*, TLabel*, short int* realminPX, bool* terminado);
};
//---------------------------------------------------------------------------
#endif
 