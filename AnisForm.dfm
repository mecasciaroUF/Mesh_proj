object AnisotropicForm: TAnisotropicForm
  Left = 466
  Top = 194
  Width = 319
  Height = 235
  Caption = 'Filtro Anisotr'#243'pico'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 32
    Width = 80
    Height = 13
    Caption = 'Lambda [0-0.25]:'
  end
  object Label2: TLabel
    Left = 48
    Top = 72
    Width = 40
    Height = 13
    Caption = 't (scale):'
  end
  object Label3: TLabel
    Left = 8
    Top = 112
    Width = 50
    Height = 13
    Caption = 'Funci'#243'n g:'
  end
  object LambdaEdit: TEdit
    Left = 96
    Top = 24
    Width = 49
    Height = 21
    TabOrder = 0
    Text = '0.2'
  end
  object ScaleEdit: TEdit
    Left = 96
    Top = 64
    Width = 49
    Height = 21
    TabOrder = 1
    Text = '10'
  end
  object KRadioGroup: TRadioGroup
    Tag = 1
    Left = 192
    Top = 8
    Width = 89
    Height = 89
    Caption = 'K'
    ItemIndex = 0
    Items.Strings = (
      'Autom'#225'tico'
      'Manual')
    TabOrder = 2
    OnClick = KRadioGroupClick
  end
  object KEdit: TEdit
    Left = 192
    Top = 104
    Width = 89
    Height = 21
    Enabled = False
    ReadOnly = True
    TabOrder = 3
    Text = '500'
  end
  object FuncionGComboBox: TComboBox
    Left = 64
    Top = 104
    Width = 121
    Height = 21
    ItemHeight = 13
    ItemIndex = 0
    TabOrder = 4
    Text = 'Gaussiana'
    Items.Strings = (
      'Gaussiana'
      'Sigmoidea Inversa')
  end
  object Button1: TButton
    Left = 112
    Top = 152
    Width = 75
    Height = 25
    Caption = 'Aceptar'
    ModalResult = 1
    TabOrder = 5
  end
end
