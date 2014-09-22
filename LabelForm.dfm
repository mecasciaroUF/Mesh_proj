object LabelForm1: TLabelForm1
  Left = 360
  Top = 183
  Width = 494
  Height = 413
  Caption = 'Nomenclatura del Esqueleto'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object NodeLabel: TLabel
    Left = 16
    Top = 8
    Width = 34
    Height = 13
    Caption = 'Nodos:'
  end
  object RamaLabel: TLabel
    Left = 120
    Top = 8
    Width = 36
    Height = 13
    Caption = 'Ramas:'
  end
  object NodeListBox: TListBox
    Left = 8
    Top = 32
    Width = 65
    Height = 241
    ItemHeight = 13
    TabOrder = 0
  end
  object RamaListBox: TListBox
    Left = 96
    Top = 32
    Width = 89
    Height = 241
    ItemHeight = 13
    TabOrder = 1
  end
  object Button1: TButton
    Left = 8
    Top = 288
    Width = 89
    Height = 25
    Caption = 'Orientar'
    TabOrder = 2
    OnClick = Button1Click
  end
  object VolRamaCheckBox: TCheckBox
    Left = 8
    Top = 328
    Width = 97
    Height = 17
    Caption = 'VolXRama'
    Enabled = False
    TabOrder = 3
  end
  object StringGrid1: TStringGrid
    Left = 208
    Top = 32
    Width = 265
    Height = 241
    DefaultColWidth = 50
    TabOrder = 4
    Visible = False
  end
  object GrabarMapaButton: TButton
    Left = 304
    Top = 280
    Width = 75
    Height = 25
    Caption = 'Guardar'
    TabOrder = 5
    Visible = False
    OnClick = GrabarMapaButtonClick
  end
  object PodaButton: TButton
    Left = 112
    Top = 288
    Width = 75
    Height = 25
    Caption = 'Podar'
    TabOrder = 6
    OnClick = PodaButtonClick
  end
  object SaveDialog1: TSaveDialog
    Left = 328
    Top = 312
  end
end
