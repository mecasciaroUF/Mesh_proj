object Form1: TForm1
  Left = 2
  Top = 1
  Width = 1655
  Height = 980
  Cursor = crAppStart
  Caption = 'Visor3D'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -14
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  OnResize = FormResize
  PixelsPerInch = 120
  TextHeight = 16
  object Label15: TLabel
    Left = 1487
    Top = 25
    Width = 34
    Height = 16
    Caption = 'Zfrom'
  end
  object Label16: TLabel
    Left = 1536
    Top = 25
    Width = 19
    Height = 16
    Caption = 'Zto'
  end
  object ProcessLabel: TLabel
    Left = 945
    Top = 806
    Width = 119
    Height = 25
    Caption = 'ProcessLabel'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -20
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Visible = False
  end
  object TabSheet: TPageControl
    Left = 0
    Top = 0
    Width = 937
    Height = 818
    ActivePage = ImagenSheet
    TabIndex = 0
    TabOrder = 0
    OnChange = TabSheetChange
    object ImagenSheet: TTabSheet
      Caption = 'Axial View'
      object IniSliceLabel: TLabel
        Left = 69
        Top = 699
        Width = 61
        Height = 16
        Caption = 'First Slice:'
        Visible = False
      end
      object FinSliceLabel: TLabel
        Left = 69
        Top = 738
        Width = 61
        Height = 16
        Caption = 'Last Slice:'
        Visible = False
      end
      object CarpetaLabel: TLabel
        Left = 10
        Top = 0
        Width = 52
        Height = 20
        Caption = 'Folder'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -17
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label7: TLabel
        Left = 463
        Top = 709
        Width = 23
        Height = 16
        Caption = 'HU:'
      end
      object HULabel: TLabel
        Left = 492
        Top = 709
        Width = 4
        Height = 16
        Caption = '-'
      end
      object Label8: TLabel
        Left = 463
        Top = 729
        Width = 37
        Height = 16
        Caption = 'Mean:'
        Visible = False
      end
      object Label9: TLabel
        Left = 463
        Top = 748
        Width = 25
        Height = 16
        Caption = 'StD:'
        Visible = False
      end
      object MeanLabel: TLabel
        Left = 512
        Top = 729
        Width = 4
        Height = 16
        Caption = '-'
        Visible = False
      end
      object StDLabel: TLabel
        Left = 502
        Top = 748
        Width = 4
        Height = 16
        Caption = '-'
        Visible = False
      end
      object Label10: TLabel
        Left = 689
        Top = 709
        Width = 4
        Height = 16
        Caption = '-'
      end
      object Label11: TLabel
        Left = 689
        Top = 729
        Width = 4
        Height = 16
        Caption = '-'
      end
      object ArchivoLabel: TLabel
        Left = 10
        Top = 30
        Width = 30
        Height = 20
        Caption = 'File'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -17
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label17: TLabel
        Left = 650
        Top = 709
        Width = 35
        Height = 16
        Caption = 'XPos:'
      end
      object Label18: TLabel
        Left = 650
        Top = 729
        Width = 35
        Height = 16
        Caption = 'Ypos:'
      end
      object Label23: TLabel
        Left = 788
        Top = 39
        Width = 81
        Height = 16
        Caption = 'Slice browser'
        Layout = tlCenter
      end
      object PanelPantalla: TPanel
        Left = 158
        Top = 59
        Width = 630
        Height = 630
        Color = clInfoBk
        TabOrder = 0
        object Pantalla: TImage
          Left = 0
          Top = 0
          Width = 630
          Height = 630
          OnMouseDown = PantallaMouseDown
          OnMouseMove = PantallaMouseMove
          OnMouseUp = ROIShapeMouseUp
        end
        object TranspImage: TImage
          Left = 138
          Top = 118
          Width = 314
          Height = 314
          Enabled = False
          Transparent = True
          Visible = False
          OnMouseMove = PantallaMouseMove
        end
        object ROIShape: TShape
          Left = 222
          Top = 197
          Width = 157
          Height = 157
          Brush.Style = bsClear
          ParentShowHint = False
          Pen.Color = clActiveCaption
          Pen.Width = 3
          ShowHint = True
          Visible = False
          OnMouseDown = ROIShapeMouseDown
          OnMouseMove = PantallaMouseMove
          OnMouseUp = ROIShapeMouseUp
        end
        object RShape8: TShape
          Left = 0
          Top = 20
          Width = 80
          Height = 80
          Brush.Style = bsClear
          Pen.Color = clBlue
          Pen.Width = 2
          Shape = stCircle
          Visible = False
        end
        object RShape7: TShape
          Left = 0
          Top = 108
          Width = 80
          Height = 80
          Brush.Style = bsClear
          Pen.Color = clRed
          Pen.Width = 2
          Shape = stCircle
          Visible = False
        end
        object Shape8: TShape
          Left = 10
          Top = 10
          Width = 11
          Height = 11
          Pen.Color = clBlue
          Pen.Style = psDot
          Pen.Width = 2
          Visible = False
          OnMouseDown = Shape8MouseDown
          OnMouseMove = PantallaMouseMove
          OnMouseUp = ROIShapeMouseUp
        end
        object Shape7: TShape
          Left = 10
          Top = 98
          Width = 11
          Height = 12
          Pen.Color = clRed
          Pen.Style = psDot
          Pen.Width = 2
          Visible = False
          OnMouseDown = Shape7MouseDown
        end
      end
      object ImageScrollBar: TScrollBar
        Left = 820
        Top = 59
        Width = 21
        Height = 630
        Kind = sbVertical
        PageSize = 0
        TabOrder = 1
        OnChange = ImageScrollBarChange
      end
      object ROIListBox: TListBox
        Left = 345
        Top = 699
        Width = 40
        Height = 41
        ItemHeight = 16
        TabOrder = 2
        Visible = False
      end
      object ROICheckBox: TCheckBox
        Left = 236
        Top = 699
        Width = 51
        Height = 21
        Caption = 'ROI'
        TabOrder = 3
        Visible = False
        OnClick = ROICheckBoxClick
      end
      object FinSliceEdit: TEdit
        Left = 158
        Top = 738
        Width = 40
        Height = 24
        TabOrder = 4
        Visible = False
      end
      object IniSliceEdit: TEdit
        Left = 158
        Top = 699
        Width = 40
        Height = 24
        TabOrder = 5
        Visible = False
      end
      object SliceListBox: TListBox
        Left = 70
        Top = 62
        Width = 70
        Height = 631
        ItemHeight = 16
        TabOrder = 6
        OnClick = SliceListBoxClick
      end
      object AP: TListBox
        Left = 295
        Top = 699
        Width = 41
        Height = 41
        ItemHeight = 16
        TabOrder = 7
        Visible = False
      end
    end
    object ROISheet: TTabSheet
      Caption = '3D Rendering'
      ImageIndex = 1
      object ROIPanel: TPanel
        Left = 10
        Top = 10
        Width = 897
        Height = 759
        Caption = 'ROIPanel'
        TabOrder = 0
        OnMouseDown = ROIPanelMouseDown
        OnMouseMove = ROIPanelMouseMove
        OnMouseUp = ROIPanelMouseUp
      end
      object CheckBox2: TCheckBox
        Left = 935
        Top = 748
        Width = 120
        Height = 21
        Caption = 'CheckBox2'
        TabOrder = 1
        Visible = False
      end
    end
    object OrthoViewSheet: TTabSheet
      Tag = 1
      Caption = 'Ortho View'
      Enabled = False
      ImageIndex = 2
      object Shape3: TShape
        Left = 853
        Top = 430
        Width = 377
        Height = 376
        Brush.Style = bsClear
        Pen.Color = clBlue
        Pen.Width = 3
      end
      object Shape2: TShape
        Left = 430
        Top = 430
        Width = 376
        Height = 376
        Brush.Style = bsClear
        Pen.Color = clGreen
        Pen.Width = 3
      end
      object Shape1: TShape
        Left = 16
        Top = 430
        Width = 377
        Height = 376
        Brush.Style = bsClear
        Pen.Color = clRed
        Pen.Width = 3
      end
      object XYImage: TImage
        Left = 20
        Top = 433
        Width = 369
        Height = 369
      end
      object ZXImage: TImage
        Left = 857
        Top = 433
        Width = 369
        Height = 369
      end
      object YZImage: TImage
        Left = 433
        Top = 433
        Width = 369
        Height = 369
      end
      object YShape: TShape
        Left = 199
        Top = 433
        Width = 4
        Height = 369
        Brush.Style = bsClear
        Pen.Color = clBlue
      end
      object XShape: TShape
        Left = 1040
        Top = 433
        Width = 4
        Height = 369
        Brush.Style = bsClear
        Pen.Color = clGreen
      end
      object ZShape: TShape
        Left = 617
        Top = 433
        Width = 3
        Height = 369
        Brush.Style = bsClear
        Pen.Color = clRed
      end
      object XShape2: TShape
        Left = 20
        Top = 617
        Width = 369
        Height = 3
        Brush.Style = bsClear
        Pen.Color = clGreen
      end
      object YShape2: TShape
        Left = 433
        Top = 617
        Width = 369
        Height = 3
        Brush.Style = bsClear
        Pen.Color = clBlue
      end
      object ZShape2: TShape
        Left = 857
        Top = 617
        Width = 369
        Height = 3
        Brush.Style = bsClear
        Pen.Color = clRed
      end
      object Shape4: TShape
        Left = 14
        Top = 390
        Width = 384
        Height = 32
        Brush.Style = bsClear
        Pen.Color = clBlue
        Pen.Width = 2
      end
      object Shape5: TShape
        Left = 430
        Top = 390
        Width = 384
        Height = 32
        Brush.Style = bsClear
        Pen.Color = clRed
        Pen.Width = 2
      end
      object Shape6: TShape
        Left = 849
        Top = 390
        Width = 384
        Height = 32
        Brush.Style = bsClear
        Pen.Color = clGreen
        Pen.Width = 2
      end
      object Shape9: TShape
        Left = 853
        Top = 4
        Width = 377
        Height = 376
        Brush.Style = bsClear
        Pen.Color = clPurple
        Pen.Style = psDot
      end
      object ZoomImage: TImage
        Left = 857
        Top = 7
        Width = 369
        Height = 370
      end
      object ZoomShape: TShape
        Left = 91
        Top = 512
        Width = 217
        Height = 217
        Brush.Style = bsClear
        Pen.Color = clPurple
        Pen.Style = psDot
      end
      object OrthoPanel: TPanel
        Left = 20
        Top = 10
        Width = 720
        Height = 365
        Caption = 'OrthogonalPanel'
        TabOrder = 0
        OnMouseDown = ROIPanelMouseDown
        OnMouseMove = ROIPanelMouseMove
        OnMouseUp = ROIPanelMouseUp
        object ArbitraryCutControlPanel: TPanel
          Left = 0
          Top = 0
          Width = 139
          Height = 287
          TabOrder = 0
          object Label4: TLabel
            Left = 6
            Top = 10
            Width = 21
            Height = 16
            Caption = 'Rot'
          end
          object Label5: TLabel
            Left = 43
            Top = 10
            Width = 27
            Height = 16
            Caption = 'Cent'
          end
          object Label14: TLabel
            Left = 73
            Top = 10
            Width = 60
            Height = 16
            Caption = 'Centerline'
          end
          object ArbitraryCutScrollBar: TScrollBar
            Left = 6
            Top = 30
            Width = 21
            Height = 198
            Kind = sbVertical
            Max = 360
            PageSize = 0
            TabOrder = 0
            OnChange = ArbitraryCutScrollBarChange
          end
          object PuntoCentralScrollBar: TScrollBar
            Left = 44
            Top = 30
            Width = 21
            Height = 198
            Kind = sbVertical
            Max = 250
            Min = -250
            PageSize = 0
            TabOrder = 1
            OnChange = PuntoCentralScrollBarChange
          end
          object ParaOrthoButton: TButton
            Left = 2
            Top = 236
            Width = 71
            Height = 21
            Caption = 'Otho'
            TabOrder = 2
            OnClick = ParaOrthoButtonClick
          end
          object UpAPButton: TButton
            Left = 20
            Top = 266
            Width = 30
            Height = 21
            Caption = '^'
            TabOrder = 3
            OnClick = UpAPButtonClick
          end
          object ToroideScrollBar: TScrollBar
            Left = 89
            Top = 30
            Width = 21
            Height = 198
            Kind = sbVertical
            Max = 135
            PageSize = 0
            Position = 1
            TabOrder = 4
            OnChange = ToroideScrollBarChange
          end
        end
      end
      object YScrollBar: TScrollBar
        Left = 17
        Top = 394
        Width = 377
        Height = 24
        PageSize = 0
        Position = 50
        TabOrder = 1
        OnChange = YScrollBarChange
      end
      object ZScrollBar: TScrollBar
        Tag = 1
        Left = 433
        Top = 394
        Width = 377
        Height = 24
        PageSize = 0
        Position = 50
        TabOrder = 2
        OnChange = YScrollBarChange
      end
      object XScrollBar: TScrollBar
        Tag = 2
        Left = 853
        Top = 394
        Width = 377
        Height = 24
        PageSize = 0
        Position = 50
        TabOrder = 3
        OnChange = YScrollBarChange
      end
      object ViewRadioGroup: TRadioGroup
        Left = 758
        Top = 0
        Width = 90
        Height = 110
        Caption = 'Vista'
        Items.Strings = (
          'Axial'
          'Coronal'
          'Sagital')
        TabOrder = 4
      end
      object ZoomRadioGroup: TRadioGroup
        Left = 758
        Top = 118
        Width = 90
        Height = 139
        Caption = 'Zoom'
        Color = clBtnFace
        Items.Strings = (
          '1x'
          '2x'
          '4x'
          '6x'
          '8x')
        ParentColor = False
        TabOrder = 5
        OnClick = YScrollBarChange
      end
      object CursorRadioGroup: TRadioGroup
        Left = 758
        Top = 256
        Width = 90
        Height = 50
        Caption = 'Cursores'
        Items.Strings = (
          'C1'
          'C2')
        TabOrder = 6
      end
      object SetCursorButton: TButton
        Left = 758
        Top = 305
        Width = 90
        Height = 21
        Caption = 'Set Cursor'
        TabOrder = 7
        OnClick = SetCursorButtonClick
      end
      object VistaOBButton: TButton
        Left = 758
        Top = 354
        Width = 90
        Height = 21
        Caption = 'Vista OB'
        TabOrder = 8
        OnClick = VistaOBButtonClick
      end
      object FlipCursorButton: TButton
        Left = 758
        Top = 325
        Width = 90
        Height = 21
        Caption = 'Flip Cursor'
        TabOrder = 9
        OnClick = FlipCursorButtonClick
      end
    end
  end
  object PageControl1: TPageControl
    Left = 940
    Top = 30
    Width = 538
    Height = 729
    ActivePage = TabSheet3
    TabIndex = 2
    TabOrder = 1
    object TabSheet1: TTabSheet
      Caption = 'Window/Level'
      object HistoProcessLabel: TLabel
        Left = 10
        Top = 10
        Width = 62
        Height = 16
        Caption = 'Histogram'
      end
      object Label1: TLabel
        Left = 149
        Top = 335
        Width = 49
        Height = 16
        Caption = 'Azimuth:'
      end
      object Label6: TLabel
        Left = 207
        Top = 335
        Width = 4
        Height = 16
        Caption = '-'
      end
      object Label12: TLabel
        Left = 159
        Top = 345
        Width = 49
        Height = 16
        Caption = 'Azimuth:'
      end
      object Label13: TLabel
        Left = 217
        Top = 345
        Width = 4
        Height = 16
        Caption = '-'
      end
      object Label21: TLabel
        Left = 167
        Top = 542
        Width = 66
        Height = 16
        Caption = '3D Opacity'
      end
      object RXLabel: TLabel
        Left = 12
        Top = 620
        Width = 18
        Height = 16
        Caption = 'RX'
      end
      object RYLabel: TLabel
        Left = 71
        Top = 620
        Width = 19
        Height = 16
        Caption = 'RY'
      end
      object RZLabel: TLabel
        Left = 130
        Top = 620
        Width = 18
        Height = 16
        Caption = 'RZ'
      end
      object Label22: TLabel
        Left = 305
        Top = 620
        Width = 107
        Height = 16
        Caption = 'On-move Opacity:'
      end
      object NegativoCheckBox: TCheckBox
        Left = 10
        Top = 522
        Width = 119
        Height = 21
        Caption = 'Negativo'
        TabOrder = 0
        Visible = False
        OnClick = NegativoCheckBoxClick
      end
      object HistoChart: TChart
        Left = 0
        Top = 30
        Width = 513
        Height = 286
        BackWall.Brush.Color = clWhite
        BackWall.Brush.Style = bsClear
        MarginBottom = 0
        MarginLeft = 4
        MarginRight = 5
        Title.Font.Charset = DEFAULT_CHARSET
        Title.Font.Color = clBlack
        Title.Font.Height = -11
        Title.Font.Name = 'Arial'
        Title.Font.Style = []
        Title.Text.Strings = (
          'Histograma')
        Title.Visible = False
        BottomAxis.Title.Caption = 'HU'
        LeftAxis.Labels = False
        LeftAxis.LabelsOnAxis = False
        LeftAxis.LabelStyle = talNone
        LeftAxis.Logarithmic = True
        LeftAxis.RoundFirstLabel = False
        LeftAxis.TickInnerLength = 4
        LeftAxis.Visible = False
        Legend.Visible = False
        View3D = False
        BevelOuter = bvNone
        TabOrder = 1
        OnDblClick = HistoChartDblClick
        object Series1: TBarSeries
          Marks.ArrowLength = 20
          Marks.Visible = False
          SeriesColor = 4194368
          BarPen.Visible = False
          BarWidthPercent = 100
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Bar'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object Series2: TBarSeries
          Marks.ArrowLength = 20
          Marks.Visible = False
          SeriesColor = clRed
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Bar'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
      end
      object WindowGroupBox: TGroupBox
        Left = 10
        Top = 325
        Width = 513
        Height = 198
        Caption = 'Window'
        TabOrder = 2
        object CenterLabel: TLabel
          Left = 7
          Top = 54
          Width = 9
          Height = 16
          Caption = 'C'
        end
        object MaxPxLabel: TLabel
          Left = 404
          Top = 25
          Width = 4
          Height = 16
          Caption = '-'
        end
        object MinPxLabel: TLabel
          Left = 39
          Top = 25
          Width = 4
          Height = 16
          Caption = '-'
        end
        object WidthLabel: TLabel
          Left = 6
          Top = 103
          Width = 13
          Height = 16
          Caption = 'W'
        end
        object CenterTrackBar: TTrackBar
          Left = 30
          Top = 44
          Width = 424
          Height = 56
          Orientation = trHorizontal
          Frequency = 1
          Position = 0
          SelEnd = 0
          SelStart = 0
          TabOrder = 0
          TickMarks = tmBottomRight
          TickStyle = tsAuto
          OnChange = CenterTrackBarChange
        end
        object CenterEdit: TEdit
          Left = 463
          Top = 44
          Width = 40
          Height = 24
          TabOrder = 1
          Text = '0'
          OnChange = CenterEditChange
        end
        object WidthEdit: TEdit
          Left = 463
          Top = 94
          Width = 40
          Height = 24
          TabOrder = 2
          Text = '1000'
          OnChange = WidthEditChange
        end
        object WidthTrackBar: TTrackBar
          Left = 30
          Top = 94
          Width = 424
          Height = 55
          Orientation = trHorizontal
          Frequency = 1
          Position = 0
          SelEnd = 0
          SelStart = 0
          TabOrder = 3
          TickMarks = tmBottomRight
          TickStyle = tsAuto
          OnChange = WidthTrackBarChange
        end
        object CropCheckBox: TCheckBox
          Left = 49
          Top = 158
          Width = 61
          Height = 20
          Caption = 'Crop'
          TabOrder = 4
          OnClick = CropCheckBoxClick
        end
        object FixCheckBox: TCheckBox
          Left = 394
          Top = 158
          Width = 70
          Height = 20
          Caption = 'Fix Left'
          TabOrder = 5
          Visible = False
          OnClick = FixCheckBoxClick
        end
        object CACWindowButton: TButton
          Left = 284
          Top = 148
          Width = 53
          Height = 30
          Caption = 'CAC'
          TabOrder = 6
          Visible = False
          OnClick = CACWindowButtonClick
        end
        object RGCheckBox: TCheckBox
          Left = 134
          Top = 158
          Width = 51
          Height = 20
          Caption = 'RG'
          TabOrder = 7
        end
        object BinCheckBox: TCheckBox
          Left = 49
          Top = 138
          Width = 120
          Height = 21
          Caption = 'Binarize'
          TabOrder = 8
        end
        object CLCheckBox: TCheckBox
          Left = 136
          Top = 136
          Width = 49
          Height = 17
          Caption = 'CL'
          TabOrder = 9
        end
        object UCSButton: TButton
          Left = 192
          Top = 136
          Width = 73
          Height = 25
          Caption = 'UC-Spline'
          TabOrder = 10
          OnClick = UCSButtonClick
        end
      end
      object AtenuacionTrackBar: TTrackBar
        Left = 266
        Top = 660
        Width = 119
        Height = 55
        Max = 1000
        Min = 1
        Orientation = trHorizontal
        Frequency = 1
        Position = 256
        SelEnd = 0
        SelStart = 0
        TabOrder = 3
        TickMarks = tmBottomRight
        TickStyle = tsAuto
        Visible = False
      end
      object MCFaceOpacityTrackBar: TTrackBar
        Left = 374
        Top = 660
        Width = 149
        Height = 55
        Orientation = trHorizontal
        Frequency = 1
        Position = 10
        SelEnd = 0
        SelStart = 0
        TabOrder = 4
        TickMarks = tmBottomRight
        TickStyle = tsAuto
        Visible = False
      end
      object CACCheckBox: TCheckBox
        Left = 89
        Top = 522
        Width = 60
        Height = 21
        Caption = 'CAC'
        TabOrder = 5
        Visible = False
        OnClick = CACCheckBoxClick
      end
      object PasoTrackBar: TTrackBar
        Left = 158
        Top = 561
        Width = 365
        Height = 41
        Min = 1
        Orientation = trHorizontal
        Frequency = 1
        Position = 4
        SelEnd = 0
        SelStart = 0
        TabOrder = 6
        TickMarks = tmBottomRight
        TickStyle = tsAuto
        OnChange = PasoTrackBarChange
      end
      object RXEdit: TEdit
        Left = 10
        Top = 640
        Width = 50
        Height = 24
        ReadOnly = True
        TabOrder = 7
        Text = '-'
        OnChange = RXEditChange
      end
      object RYEdit: TEdit
        Left = 69
        Top = 640
        Width = 50
        Height = 24
        ReadOnly = True
        TabOrder = 8
        Text = '-'
        OnChange = RYEditChange
      end
      object RZEdit: TEdit
        Left = 128
        Top = 640
        Width = 50
        Height = 24
        ReadOnly = True
        TabOrder = 9
        Text = '-'
        OnChange = RZEditChange
      end
      object PasoEdit: TEdit
        Left = 453
        Top = 610
        Width = 31
        Height = 24
        TabOrder = 10
        Text = '2'
      end
      object ZnormCheckBox: TCheckBox
        Left = 10
        Top = 571
        Width = 129
        Height = 21
        Caption = 'Invert 3D-Z axis'
        Checked = True
        State = cbChecked
        TabOrder = 11
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'VOI Control'
      ImageIndex = 1
      object CantPuntosSegLabel: TLabel
        Left = 57
        Top = 335
        Width = 4
        Height = 16
        Caption = '-'
      end
      object CantPuntosLabel: TLabel
        Left = 6
        Top = 335
        Width = 44
        Height = 16
        Caption = 'Puntos:'
      end
      object VolLabel: TLabel
        Left = 10
        Top = 354
        Width = 63
        Height = 16
        Caption = 'Vol [mm3]:'
      end
      object VolRenderLabel: TLabel
        Left = 76
        Top = 354
        Width = 4
        Height = 16
        Caption = '-'
      end
      object ElevacionLabel: TLabel
        Left = 139
        Top = 315
        Width = 63
        Height = 16
        Caption = 'Elevacion:'
      end
      object Label2: TLabel
        Left = 10
        Top = 571
        Width = 45
        Height = 16
        Caption = 'Px/Dist:'
      end
      object Label3: TLabel
        Left = 167
        Top = 571
        Width = 41
        Height = 16
        Caption = 'Label3'
      end
      object ValorElLabel: TLabel
        Left = 207
        Top = 315
        Width = 4
        Height = 16
        Caption = '-'
      end
      object AzimuthLabel: TLabel
        Left = 149
        Top = 335
        Width = 49
        Height = 16
        Caption = 'Azimuth:'
      end
      object ValorAzLabel: TLabel
        Left = 207
        Top = 335
        Width = 4
        Height = 16
        Caption = '-'
      end
      object DistanciaLabel: TLabel
        Left = 139
        Top = 354
        Width = 59
        Height = 16
        Caption = 'Distancia:'
      end
      object ValorDistLabel: TLabel
        Left = 207
        Top = 354
        Width = 4
        Height = 16
        Caption = '-'
      end
      object MediaRenderLabel: TLabel
        Left = 148
        Top = 256
        Width = 4
        Height = 16
        Caption = '-'
      end
      object DesvRenderLabel: TLabel
        Left = 148
        Top = 276
        Width = 4
        Height = 16
        Caption = '-'
      end
      object Goma3DCheckBox: TCheckBox
        Left = 0
        Top = 0
        Width = 80
        Height = 21
        Caption = 'Goma3D'
        TabOrder = 0
        OnClick = Goma3DCheckBoxClick
      end
      object Goma3DEdit: TEdit
        Left = 0
        Top = 20
        Width = 90
        Height = 24
        TabOrder = 1
        Text = '10'
      end
      object VolcarCheckBox: TCheckBox
        Left = 0
        Top = 49
        Width = 129
        Height = 21
        Caption = 'Volcar en Estudio'
        TabOrder = 2
      end
      object DoneButton: TButton
        Left = 0
        Top = 98
        Width = 119
        Height = 31
        Caption = 'Resaltar'
        TabOrder = 3
        OnClick = DoneButtonClick
      end
      object ResaltarEdit: TEdit
        Left = 128
        Top = 98
        Width = 90
        Height = 24
        TabOrder = 4
        Text = '0'
      end
      object ElimButton: TButton
        Left = 0
        Top = 138
        Width = 119
        Height = 31
        Caption = 'Borrar'
        TabOrder = 5
        OnClick = ElimButtonClick
      end
      object EsconderCheckBox: TCheckBox
        Left = 128
        Top = 138
        Width = 119
        Height = 21
        Caption = 'Esconder'
        TabOrder = 6
      end
      object EliminaRenderButton: TButton
        Left = 89
        Top = 177
        Width = 99
        Height = 31
        Caption = 'Eliminar'
        TabOrder = 7
        OnClick = EliminaRenderButtonClick
      end
      object GuardarPTSbutton: TButton
        Left = 20
        Top = 217
        Width = 119
        Height = 30
        Caption = 'Guardar'
        TabOrder = 8
        OnClick = GuardarPTSbuttonClick
      end
      object CargarPTSbutton: TButton
        Left = 148
        Top = 217
        Width = 119
        Height = 30
        Caption = 'Cargar'
        TabOrder = 9
        OnClick = CargarPTSbuttonClick
      end
      object ParedesCheckBox: TCheckBox
        Left = 10
        Top = 305
        Width = 80
        Height = 21
        Caption = 'Paredes'
        TabOrder = 10
      end
      object MascaraButton: TButton
        Left = 17
        Top = 256
        Width = 93
        Height = 31
        Caption = 'M'#225'scara'
        TabOrder = 11
        OnClick = MascaraButtonClick
      end
      object XYButton: TButton
        Left = 10
        Top = 443
        Width = 31
        Height = 21
        Caption = 'XY'
        TabOrder = 12
        OnClick = XYButtonClick
      end
      object EsqueletoButton: TButton
        Left = 10
        Top = 473
        Width = 139
        Height = 30
        Caption = 'Esqueletonizaci'#243'n'
        TabOrder = 13
        OnClick = EsqueletoButtonClick
      end
      object SelectEsqCheckBox: TCheckBox
        Left = 207
        Top = 394
        Width = 119
        Height = 21
        Caption = 'Seleccionar'
        TabOrder = 14
        OnClick = SelectEsqCheckBoxClick
      end
      object UnirCheckBox: TCheckBox
        Left = 207
        Top = 414
        Width = 119
        Height = 20
        Caption = 'Unir'
        TabOrder = 15
      end
      object EliminarEsqButton: TButton
        Left = 177
        Top = 473
        Width = 120
        Height = 30
        Caption = 'Eliminar Esqueleto'
        TabOrder = 16
        OnClick = EliminarEsqButtonClick
      end
      object PxDistEdit: TEdit
        Left = 69
        Top = 571
        Width = 41
        Height = 24
        TabOrder = 17
        Text = '120.0'
      end
      object SmoothCheckBox: TCheckBox
        Left = 224
        Top = 576
        Width = 70
        Height = 21
        Caption = 'Smooth'
        TabOrder = 18
      end
      object AjusteCheckBox: TCheckBox
        Left = 0
        Top = 69
        Width = 119
        Height = 21
        Caption = 'Ajustar'
        TabOrder = 19
      end
      object DesmarcarButton: TButton
        Left = 138
        Top = 10
        Width = 80
        Height = 40
        Caption = 'Desmarcar'
        TabOrder = 20
        OnClick = DesmarcarButtonClick
      end
      object CheckBox1: TCheckBox
        Left = 10
        Top = 670
        Width = 119
        Height = 20
        Caption = 'CheckBox1'
        TabOrder = 21
      end
      object GuardarEsqButton: TButton
        Left = 10
        Top = 512
        Width = 139
        Height = 31
        Caption = 'Guardar Esqueleto'
        TabOrder = 22
        OnClick = GuardarEsqButtonClick
      end
      object LabelButton: TButton
        Left = 177
        Top = 512
        Width = 120
        Height = 31
        Caption = 'Nomenclar'
        TabOrder = 23
        OnClick = LabelButtonClick
      end
      object CentrarButton: TButton
        Left = 49
        Top = 443
        Width = 31
        Height = 21
        Caption = 'C'
        TabOrder = 24
        OnClick = CentrarButtonClick
      end
      object EsqOnTopCheckBox: TCheckBox
        Left = 207
        Top = 433
        Width = 90
        Height = 21
        Caption = 'Esq Arriba'
        TabOrder = 25
      end
    end
    object TabSheet3: TTabSheet
      Caption = 'Segmentation'
      ImageIndex = 2
      object Label19: TLabel
        Left = 10
        Top = 295
        Width = 108
        Height = 16
        Caption = 'Diaphragm region'
      end
      object Label20: TLabel
        Left = 414
        Top = 305
        Width = 88
        Height = 16
        Caption = 'Annulus region'
      end
      object CarinaButton: TButton
        Left = 0
        Top = 10
        Width = 110
        Height = 31
        Caption = 'Detect Carina'
        Enabled = False
        TabOrder = 0
        OnClick = CarinaButtonClick
      end
      object CACDButton: TButton
        Left = 0
        Top = 49
        Width = 110
        Height = 31
        Caption = 'Detect CA/CD'
        Enabled = False
        TabOrder = 1
        OnClick = CACDButtonClick
      end
      object CreateToroidButton: TButton
        Left = 0
        Top = 89
        Width = 110
        Height = 30
        Caption = 'Create Toroid'
        Enabled = False
        TabOrder = 2
        OnClick = CreateToroidButtonClick
      end
      object SegmentDAButton: TButton
        Left = 0
        Top = 128
        Width = 110
        Height = 31
        Caption = 'Segment DA'
        Enabled = False
        TabOrder = 3
        OnClick = SegmentDAButtonClick
      end
      object SegmentArchButton: TButton
        Left = 0
        Top = 167
        Width = 110
        Height = 31
        Caption = 'Segment Arch'
        Enabled = False
        TabOrder = 4
        OnClick = SegmentArchButtonClick
      end
      object OrthoCorrectionButton: TButton
        Left = 0
        Top = 217
        Width = 110
        Height = 30
        Caption = 'Ortho Correction'
        Enabled = False
        TabOrder = 5
        OnClick = OrthoCorrectionButtonClick
      end
      object AorticRegionsGroupBox: TGroupBox
        Left = 128
        Top = 0
        Width = 228
        Height = 247
        Caption = 'Aortic Regions'
        Enabled = False
        TabOrder = 6
        object AnnulusLabel: TLabel
          Left = 39
          Top = 30
          Width = 102
          Height = 16
          Caption = 'Aortic Annulus (1)'
        end
        object CALabel: TLabel
          Left = 39
          Top = 59
          Width = 141
          Height = 16
          Caption = 'Ascending CSA (CA) (2)'
        end
        object BSALabel: TLabel
          Left = 39
          Top = 89
          Width = 133
          Height = 16
          Caption = 'Brachicephalic root (3)'
        end
        object LSALabel: TLabel
          Left = 39
          Top = 118
          Width = 135
          Height = 16
          Caption = 'Left Subclavian root (4)'
        end
        object CDLabel: TLabel
          Left = 39
          Top = 148
          Width = 151
          Height = 16
          Caption = 'Descending CSA (CD) (5)'
        end
        object ApexSinusLabel: TLabel
          Left = 39
          Top = 177
          Width = 175
          Height = 16
          Caption = 'Heart apex/coronary sinus (6)'
        end
        object DiaphragmLabel: TLabel
          Left = 39
          Top = 207
          Width = 85
          Height = 16
          Caption = 'Diaphragm (7)'
        end
        object AnnulusCheckBox: TCheckBox
          Left = 10
          Top = 30
          Width = 21
          Height = 20
          TabOrder = 0
          OnClick = AnnulusCheckBoxClick
        end
        object CACheckBox: TCheckBox
          Left = 10
          Top = 59
          Width = 21
          Height = 21
          TabOrder = 1
          OnClick = CACheckBoxClick
        end
        object BCACheckBox: TCheckBox
          Left = 10
          Top = 89
          Width = 21
          Height = 21
          TabOrder = 2
          OnClick = BCACheckBoxClick
        end
        object LSACheckBox: TCheckBox
          Left = 10
          Top = 118
          Width = 21
          Height = 21
          TabOrder = 3
          OnClick = LSACheckBoxClick
        end
        object CDCheckBox: TCheckBox
          Left = 10
          Top = 148
          Width = 21
          Height = 21
          TabOrder = 4
          OnClick = CDCheckBoxClick
        end
        object ApexSinusCheckBox: TCheckBox
          Left = 10
          Top = 177
          Width = 21
          Height = 21
          TabOrder = 5
          OnClick = ApexSinusCheckBoxClick
        end
        object DiaphragmCheckBox: TCheckBox
          Left = 10
          Top = 207
          Width = 21
          Height = 21
          TabOrder = 6
          OnClick = DiaphragmCheckBoxClick
        end
      end
      object CSATrackBar: TTrackBar
        Left = 0
        Top = 256
        Width = 523
        Height = 41
        Orientation = trHorizontal
        Frequency = 1
        Position = 0
        SelEnd = 0
        SelStart = 0
        TabOrder = 7
        TickMarks = tmBottomRight
        TickStyle = tsAuto
        OnChange = CSATrackBarChange
      end
      object MPRPanel: TPanel
        Left = 89
        Top = 335
        Width = 315
        Height = 315
        Caption = 'Multi-Planar Reconstruction'
        Color = clActiveCaption
        TabOrder = 8
        object ObliqueImage: TImage
          Left = 0
          Top = 0
          Width = 315
          Height = 315
        end
      end
      object VistaRadioGroup: TRadioGroup
        Left = 414
        Top = 335
        Width = 80
        Height = 60
        Caption = 'View'
        Enabled = False
        ItemIndex = 0
        Items.Strings = (
          'MPR'
          'Axial')
        TabOrder = 9
        OnClick = VistaRadioGroupClick
      end
      object SaveModelButton: TButton
        Left = 364
        Top = 10
        Width = 159
        Height = 31
        Caption = 'Save Model'
        Enabled = False
        TabOrder = 10
        OnClick = SaveModelButtonClick
      end
      object LoadModelButton: TButton
        Left = 364
        Top = 217
        Width = 159
        Height = 30
        Caption = 'Load Model'
        TabOrder = 11
        OnClick = LoadModelButtonClick
      end
      object PrintReportButton: TButton
        Left = 364
        Top = 49
        Width = 159
        Height = 31
        Caption = 'Print Report'
        Enabled = False
        TabOrder = 12
      end
      object AutoCheckBox: TCheckBox
        Left = 364
        Top = 98
        Width = 159
        Height = 21
        Caption = 'Automatic Sequence'
        TabOrder = 13
      end
      object Button1: TButton
        Left = 415
        Top = 409
        Width = 93
        Height = 30
        Caption = 'Pump'
        TabOrder = 14
        OnClick = Button1Click
      end
      object TresDCheckBox: TCheckBox
        Left = 10
        Top = 414
        Width = 40
        Height = 20
        Caption = '3D'
        TabOrder = 15
      end
      object Button2: TButton
        Left = 415
        Top = 450
        Width = 93
        Height = 31
        Caption = 'Grow'
        TabOrder = 16
        OnClick = Button2Click
      end
    end
    object TabSheet4: TTabSheet
      Caption = 'Mesh'
      ImageIndex = 3
    end
  end
  object MinzScrollBar: TScrollBar
    Left = 1487
    Top = 54
    Width = 40
    Height = 755
    Kind = sbVertical
    Min = 50
    PageSize = 0
    Position = 50
    TabOrder = 2
    OnChange = MinzScrollBarChange
  end
  object MaxzScrollBar: TScrollBar
    Left = 1536
    Top = 54
    Width = 41
    Height = 755
    Kind = sbVertical
    Min = 50
    PageSize = 0
    Position = 100
    TabOrder = 3
    OnChange = MaxzScrollBarChange
  end
  object ProgressBar1: TProgressBar
    Left = 945
    Top = 768
    Width = 533
    Height = 31
    Min = 0
    Max = 100
    Smooth = True
    Step = 1
    TabOrder = 4
  end
  object ListBox1: TListBox
    Left = 1080
    Top = 808
    Width = 41
    Height = 33
    ItemHeight = 16
    TabOrder = 5
    Visible = False
  end
  object Button3: TButton
    Left = 1360
    Top = 552
    Width = 89
    Height = 25
    Caption = 'Flood Fill'
    TabOrder = 6
    OnClick = Button3Click
  end
  object Button4: TButton
    Left = 1360
    Top = 592
    Width = 89
    Height = 25
    Caption = 'Refinement'
    TabOrder = 7
    OnClick = Button4Click
  end
  object Button5: TButton
    Left = 1032
    Top = 720
    Width = 113
    Height = 25
    Caption = 'Save Mesh'
    TabOrder = 8
    OnClick = Button5Click
  end
  object Button6: TButton
    Left = 1160
    Top = 720
    Width = 97
    Height = 25
    Caption = 'Load 1 Mesh'
    TabOrder = 9
    OnClick = Button6Click
  end
  object MainMenu1: TMainMenu
    Left = 952
    Top = 65535
    object Archivo1: TMenuItem
      Caption = 'File'
      object AbrirVolumen1: TMenuItem
        Caption = 'Open Volume'
        OnClick = AbrirVolumen1Click
      end
      object GuardarVolumen1: TMenuItem
        Caption = 'Save Volume'
        OnClick = GuardarVolumen1Click
      end
      object Cerrar1: TMenuItem
        Caption = 'Close Volume'
        OnClick = Cerrar1Click
      end
      object Salir1: TMenuItem
        Caption = 'Exit Program'
        OnClick = Salir1Click
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object Histograma1: TMenuItem
        Caption = 'Guardar Histograma'
        Visible = False
        OnClick = Histograma1Click
      end
      object GrabarAC1: TMenuItem
        Caption = 'Grabar AC'
        Visible = False
        OnClick = GrabarAC1Click
      end
      object GrabarVistasOblicuas1: TMenuItem
        Caption = 'Grabar Vistas Oblicuas'
        Visible = False
        OnClick = GrabarVistasOblicuas1Click
      end
      object N7: TMenuItem
        Caption = '-'
      end
      object AbrirconSobremuestreo1: TMenuItem
        Caption = 'Abrir con Sobremuestreo'
        Visible = False
        OnClick = AbrirconSobremuestreo1Click
      end
      object InvertirEstudio1: TMenuItem
        Caption = 'Invertir Estudio'
        Visible = False
        OnClick = InvertirEstudio1Click
      end
      object GuardarPlanosToroidales1: TMenuItem
        Caption = 'Guardar Planos Toroidales'
        Visible = False
        OnClick = GuardarPlanosToroidales1Click
      end
      object N8: TMenuItem
        Caption = '-'
      end
      object GuardarCarina1: TMenuItem
        Caption = 'Guardar Carina'
        Visible = False
        OnClick = GuardarCarina1Click
      end
      object AbrirCarina1: TMenuItem
        Caption = 'Abrir Carina'
        Visible = False
        OnClick = AbrirCarina1Click
      end
      object N9: TMenuItem
        Caption = '-'
      end
      object GuardarCenterline1: TMenuItem
        Caption = 'Guardar Centerline'
        Visible = False
        OnClick = GuardarCenterline1Click
      end
      object GuardarPlanosOblicuos1: TMenuItem
        Caption = 'Guardar Planos Perpendiculares'
        Visible = False
        OnClick = GuardarPlanosOblicuos1Click
      end
      object GuardarModelo1: TMenuItem
        Caption = 'Guardar Modelo'
        Visible = False
        OnClick = GuardarModelo1Click
      end
      object AbrirModelo1: TMenuItem
        Caption = 'Abrir Modelo'
        Visible = False
        OnClick = AbrirModelo1Click
      end
    end
    object ROI1: TMenuItem
      Caption = 'ROI'
      object Guardar1: TMenuItem
        Caption = 'Guardar'
        OnClick = Guardar1Click
      end
      object Cargar1: TMenuItem
        Caption = 'Cargar'
        OnClick = Cargar1Click
      end
    end
    object Filtros1: TMenuItem
      Caption = 'Filtros'
      object PasaBajos1: TMenuItem
        Caption = 'Pasa Bajos'
        object VOI2: TMenuItem
          Caption = 'VOI'
          OnClick = VOI2Click
        end
        object Global2: TMenuItem
          Caption = 'Global'
          OnClick = Global2Click
        end
      end
      object PasaAltos1: TMenuItem
        Caption = 'Pasa Altos'
        object VOI3: TMenuItem
          Caption = 'VOI'
          OnClick = VOI3Click
        end
        object Global3: TMenuItem
          Caption = 'Global'
        end
      end
      object HighBoost1: TMenuItem
        Caption = 'High Boost'
        object VOI4: TMenuItem
          Caption = 'VOI'
          OnClick = VOI4Click
        end
        object Global4: TMenuItem
          Caption = 'Global'
          OnClick = Global4Click
        end
      end
      object Bilateral1: TMenuItem
        Caption = 'Bilateral'
      end
      object Anisotrpico1: TMenuItem
        Caption = 'Anisotr'#243'pico'
        OnClick = Anisotrpico1Click
      end
      object Canny1: TMenuItem
        Caption = 'Canny'
        OnClick = Canny1Click
        object N2D1: TMenuItem
          Caption = '2D'
          OnClick = N2D1Click
        end
      end
      object Sobel1: TMenuItem
        Caption = 'Sobel'
        object X1: TMenuItem
          Caption = 'X'
          OnClick = X1Click
        end
        object Y1: TMenuItem
          Caption = 'Y'
          OnClick = Y1Click
        end
        object Z1: TMenuItem
          Caption = 'Z'
        end
      end
      object N2: TMenuItem
        Caption = '-'
      end
      object Mediana1: TMenuItem
        Caption = 'Mediana'
        OnClick = Mediana1Click
      end
      object Erosion1: TMenuItem
        Caption = 'Erosi'#243'n'
        OnClick = Erosion1Click
      end
      object Dilatacin1: TMenuItem
        Caption = 'Dilataci'#243'n'
        OnClick = Dilatacin1Click
      end
      object Apertura: TMenuItem
        Caption = 'Apertura'
        OnClick = AperturaClick
      end
      object Cierre1: TMenuItem
        Caption = 'Cierre'
        OnClick = Cierre1Click
      end
      object Suma1: TMenuItem
        Caption = 'Suma'
        OnClick = Suma1Click
      end
      object Resta1: TMenuItem
        Caption = 'Resta'
        OnClick = Resta1Click
      end
      object N3: TMenuItem
        Caption = '-'
      end
      object KMeansCluster: TMenuItem
        Caption = 'K-Means Cluster'
        OnClick = KMeansClusterClick
      end
      object SquareTracing1: TMenuItem
        Caption = 'Square Tracing'
        OnClick = SquareTracing1Click
      end
      object Houghcircular1: TMenuItem
        Caption = 'Hough (circular)'
        OnClick = Houghcircular1Click
      end
      object N4: TMenuItem
        Caption = '-'
      end
      object EliminarPulmon1: TMenuItem
        Caption = 'Eliminar Pulmon'
        OnClick = EliminarPulmon1Click
      end
      object Clustering1: TMenuItem
        Caption = 'Separar M-S-P'
        OnClick = Clustering1Click
      end
      object SegmentaCorazn1: TMenuItem
        Caption = 'Segmenta Coraz'#243'n'
        OnClick = SegmentaCorazn1Click
      end
      object N5: TMenuItem
        Caption = '-'
      end
      object CAC1: TMenuItem
        Caption = 'CAC'
        OnClick = CAC1Click
      end
      object Slice1: TMenuItem
        Caption = 'Slice'
        OnClick = Slice1Click
      end
    end
    object Render1: TMenuItem
      Caption = 'Render'
      object Bordes1: TMenuItem
        Caption = 'Bordes'
        object Normal1: TMenuItem
          Caption = 'Normal'
          OnClick = Normal1Click
        end
        object Membrana1: TMenuItem
          Caption = 'Membrana'
          OnClick = Membrana1Click
        end
      end
      object LUT1: TMenuItem
        Caption = 'LUT'
        OnClick = LUT1Click
      end
      object ConnnectedComponent1: TMenuItem
        Caption = 'Connnected Component'
        object Global1: TMenuItem
          Caption = 'Global'
          OnClick = Global1Click
        end
        object VOI1: TMenuItem
          Tag = 1
          Caption = 'VOI'
          OnClick = Global1Click
        end
      end
      object Umbral1: TMenuItem
        Caption = 'Umbral'
        OnClick = Umbral1Click
      end
      object N6: TMenuItem
        Caption = '-'
      end
      object MarchingCubes1: TMenuItem
        Caption = 'Marching Cubes'
        OnClick = MarchingCubes1Click
      end
    end
    object Ajuste1: TMenuItem
      Caption = 'Ajuste'
      object Cilindro1: TMenuItem
        Caption = 'Cilindro'
        OnClick = Cilindro1Click
      end
      object Esfera1: TMenuItem
        Caption = 'Esfera'
        OnClick = Esfera1Click
      end
      object CirculoManual1: TMenuItem
        Caption = 'Circulo-Manual'
        OnClick = CirculoManual1Click
      end
      object CirculoCavityFilling1: TMenuItem
        Caption = 'Circulo CavityFilling'
        OnClick = CirculoCavityFilling1Click
      end
      object AortaCavityFilling1: TMenuItem
        Caption = 'Aorta CavityFilling'
        OnClick = AortaCavityFilling1Click
      end
      object GraficarCenterline1: TMenuItem
        Caption = 'Graficar Centerline'
        OnClick = GraficarCenterline1Click
      end
      object AxialCavityFilling1: TMenuItem
        Caption = 'Axial CavityFilling'
        OnClick = AxialCavityFilling1Click
      end
    end
    object Ventana1: TMenuItem
      Caption = 'Ventana'
      object Hueso1: TMenuItem
        Caption = 'Hueso'
        OnClick = Hueso1Click
      end
      object Mediastino1: TMenuItem
        Caption = 'Mediastino'
        OnClick = Mediastino1Click
      end
      object Pulmn1: TMenuItem
        Caption = 'Pulm'#243'n'
        OnClick = Pulmn1Click
      end
    end
    object Segmentacin1: TMenuItem
      Caption = 'Segmentaci'#243'n'
      object Carina1: TMenuItem
        Caption = 'Carina'
      end
      object CACD1: TMenuItem
        Caption = 'CA/CD'
        OnClick = CACD1Click
      end
      object Baston1: TMenuItem
        Caption = 'Crear Bast'#243'n'
      end
      object Aorta2: TMenuItem
        Caption = 'Aorta'
        OnClick = Aorta2Click
      end
    end
    object Texturas1: TMenuItem
      Caption = 'Texturas'
      object Setup1: TMenuItem
        Caption = 'Setup'
        OnClick = Setup1Click
      end
    end
    object Malla1: TMenuItem
      Caption = 'Malla'
      OnClick = Malla1Click
    end
  end
  object OpenDialog1: TOpenDialog
    Options = [ofHideReadOnly, ofAllowMultiSelect, ofEnableSizing]
    Left = 1016
  end
  object SaveDialog1: TSaveDialog
    Left = 984
    Top = 65535
  end
  object ColorDialog1: TColorDialog
    Ctl3D = True
    Left = 1048
  end
end
