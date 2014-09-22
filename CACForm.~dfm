object CACForm1: TCACForm1
  Left = 251
  Top = 140
  Width = 1010
  Height = 561
  Caption = 'Medici'#243'n de CAC'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnActivate = FormActivate
  OnClose = FormClose
  OnDeactivate = FormDeactivate
  PixelsPerInch = 96
  TextHeight = 13
  object CACSliceLabel: TLabel
    Left = 536
    Top = 16
    Width = 78
    Height = 24
    Caption = 'SLICE N'#186
    Color = clMoneyGreen
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
    OnDblClick = CACSliceLabelDblClick
  end
  object CACLabel: TLabel
    Left = 536
    Top = 160
    Width = 44
    Height = 24
    Caption = 'CAC:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object CACResultLabel: TLabel
    Left = 592
    Top = 160
    Width = 5
    Height = 20
    Caption = '-'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label1: TLabel
    Left = 656
    Top = 56
    Width = 13
    Height = 20
    Caption = 'X'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label2: TLabel
    Left = 712
    Top = 56
    Width = 13
    Height = 20
    Caption = 'Y'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label3: TLabel
    Left = 760
    Top = 56
    Width = 12
    Height = 20
    Caption = 'Z'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label4: TLabel
    Left = 800
    Top = 56
    Width = 48
    Height = 20
    Caption = 'Score'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label5: TLabel
    Left = 864
    Top = 56
    Width = 39
    Height = 20
    Caption = 'Area'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object MaxHULabel: TLabel
    Left = 920
    Top = 56
    Width = 64
    Height = 20
    Caption = 'Max HU'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object UpDown1: TUpDown
    Left = 536
    Top = 56
    Width = 81
    Height = 73
    Min = 0
    Position = 0
    TabOrder = 0
    Wrap = False
    OnClick = UpDown1Click
  end
  object TerminarCACButton: TButton
    Left = 536
    Top = 224
    Width = 81
    Height = 25
    Caption = 'Terminar'
    ModalResult = 1
    TabOrder = 1
  end
  object CACPanel: TPanel
    Left = 8
    Top = 8
    Width = 512
    Height = 512
    FullRepaint = False
    TabOrder = 2
    object CACImage: TImage
      Left = 0
      Top = 0
      Width = 512
      Height = 512
    end
    object TranspCACImage: TImage
      Left = 0
      Top = 0
      Width = 512
      Height = 512
      Transparent = True
    end
    object SelectionImage: TImage
      Left = 0
      Top = 0
      Width = 512
      Height = 512
      Transparent = True
      OnMouseDown = SelectionImageMouseDown
      OnMouseMove = SelectionImageMouseMove
    end
    object CACProcessLabel: TLabel
      Left = 168
      Top = 240
      Width = 85
      Height = 13
      Caption = 'CACProcessLabel'
      Visible = False
    end
    object Shape1: TShape
      Left = 208
      Top = 296
      Width = 50
      Height = 50
      Brush.Style = bsClear
      Pen.Color = clActiveCaption
      Pen.Width = 3
    end
    object CACProgressBar: TProgressBar
      Left = 168
      Top = 264
      Width = 150
      Height = 17
      Min = 0
      Max = 100
      TabOrder = 0
      Visible = False
    end
  end
  object GuardarCACButton: TButton
    Left = 536
    Top = 280
    Width = 81
    Height = 25
    Caption = 'Guardar CAC'
    TabOrder = 3
    OnClick = GuardarCACButtonClick
  end
  object ListBox1: TListBox
    Left = 536
    Top = 328
    Width = 81
    Height = 49
    ItemHeight = 13
    TabOrder = 4
    Visible = False
  end
  object CheckBox1: TCheckBox
    Left = 528
    Top = 200
    Width = 97
    Height = 17
    Caption = 'CheckBox1'
    TabOrder = 5
  end
  object TrackBar1: TTrackBar
    Left = 536
    Top = 424
    Width = 73
    Height = 45
    Max = 2
    Min = -2
    Orientation = trHorizontal
    Frequency = 1
    Position = 0
    SelEnd = 0
    SelStart = 0
    TabOrder = 6
    TickMarks = tmBottomRight
    TickStyle = tsAuto
    OnChange = TrackBar1Change
  end
  object XListBox: TListBox
    Left = 640
    Top = 80
    Width = 41
    Height = 401
    ItemHeight = 13
    TabOrder = 7
  end
  object YListBox: TListBox
    Left = 696
    Top = 80
    Width = 41
    Height = 401
    ItemHeight = 13
    TabOrder = 8
  end
  object ZListBox: TListBox
    Left = 744
    Top = 80
    Width = 41
    Height = 401
    ItemHeight = 13
    TabOrder = 9
  end
  object CACListBox: TListBox
    Left = 792
    Top = 80
    Width = 57
    Height = 401
    ItemHeight = 13
    TabOrder = 10
  end
  object AreaListBox: TListBox
    Left = 856
    Top = 80
    Width = 57
    Height = 401
    ItemHeight = 13
    TabOrder = 11
  end
  object MaxHUListBox: TListBox
    Left = 920
    Top = 80
    Width = 57
    Height = 401
    ItemHeight = 13
    TabOrder = 12
  end
  object BorrarScoreButton: TButton
    Left = 536
    Top = 472
    Width = 75
    Height = 25
    Caption = 'Borrar Score'
    TabOrder = 13
    OnClick = BorrarScoreButtonClick
  end
  object SaveDialog1: TSaveDialog
    Left = 536
    Top = 384
  end
end
