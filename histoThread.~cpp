//---------------------------------------------------------------------------

#include <vcl.h>
#include <math.h>
#pragma hdrstop

#include "histoThread.h"
#pragma package(smart_init)

//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall calcHistoThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall calcHistoThread::calcHistoThread(bool CreateSuspended)
        : TThread(CreateSuspended)
{

}
//---------------------------------------------------------------------------
void __fastcall calcHistoThread::receiveParams(Volumen* V, float* histo, float* LUT,
                                               int sizeHisto, short int minPX, TChart *chart,
                                               TLabel* label, short int* realminPX, bool* terminado)
{
   this->V = V;
   this->LUT = LUT;
   this->histo = histo;
   this->sizeHisto = sizeHisto;
   this->offset = minPX;
   this->chart=chart;
   this->label =label;
   this->realminPX = realminPX;
   this->terminado = terminado;
}
//---------------------------------------------------------------------------

void __fastcall calcHistoThread::Execute()
{
      FreeOnTerminate=true;

      label->Caption = "Calculando Histograma...";
      CalcularHist();
      label->Caption = "Graficando Histograma...";
      GraficarHist();
      label->Caption = "Histograma";

      terminado[0] = true;
}

//---------------------------------------------------------------------------
void calcHistoThread::CalcularHist()
{
        for(int i=0; i<sizeHisto;i++)
        histo[i]=0;

        for (int i=0; i<V->getAlto(); i++)
                for (int j=0; j<V->getAncho(); j++)
                      for (int k=0; k<V->getProf(); k++)
                          histo[V->GetV(i,j,k)-offset]++;

       for(int i=1; i<sizeHisto; i++)
       {
        if(histo[i]>0)
        {
        realminPX[0] = i+offset;
        break;
        }
       }
}
//---------------------------------------------------------------------------
void calcHistoThread::GraficarHist()
{
       int color = 0;

       for(int i=1; i<sizeHisto;i++)
       {
       //Calcular Color por LUT
       color = int(LUT[i]);
       chart->Series[0]->AddXY(double(i+offset),double(histo[i]),"",
       color*16*16*16*16+color*16*16+color);
       }

       chart->Refresh();
       chart->Repaint();
       // HistoChart->Series[1]->AddXY(0, 20, "", clOlive);

}
//---------------------------------------------------------------------------

