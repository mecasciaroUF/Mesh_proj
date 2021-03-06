#ifndef TMyTransparentPanel
#define TMyTransparentPanel

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "Matriz.h"
#include "Vector.h"
#include <Dialogs.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <Graphics.hpp>
#include <gl\gl.h>
#include <gl\glu.h>
#include <GL\glut.h>

 class TMyTransparentPanel : public TPanel
 {
        protected:
        virtual void __fastcall CreateParams(Controls::TCreateParams &Params);
        virtual void __fastcall Paint(void);

 };

 TMyTransparentPanel::CreateParams(Controls::TCreateParams &Params)
{
 TComponent* GetParentComponent(void);
 TComponentState ComponentState;

  CreateParams(Params);

  if (ComponentState.Contains(csDesigning)==false)
      Params.ExStyle = WS_EX_TRANSPARENT;
}

 TMyTransparentPanel::Paint(void)
{
  Windows::TBitmap XBitMap;
  HDC XOldDC;
  TRect XRect;

 TComponent* GetParentComponent(void);
 TComponentState ComponentState;


  if (ComponentState.Contains(csDesigning)==true)
    {Paint(); return;}


    XRect =ClientRect;
    XOldDC =Canvas.Handle;
    XBitMap =TBitMap.Create;

    try
      XBitMap.Height=Height; XBitMap.Width=Width;
      Canvas.Handle==XBitMap.Canvas.Handle;
      inherited Paint;
      RedrawWindow(Parent.Handle, @XRect, 0,
                   RDW_ERASE or RDW_INVALIDATE or
                   RDW_NOCHILDREN or RDW_UPDATENOW);
    finally
      Canvas.Handle:=XOldDC;
      Canvas.BrushCopy(XRect, XBitMap, XRect, Color);
      XBitMap.Free;
      end;
    end;
  end;*/
}

#endif

