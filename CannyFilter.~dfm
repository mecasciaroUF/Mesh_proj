object CannyFilterForm: TCannyFilterForm
  Left = 508
  Top = 244
  Width = 325
  Height = 227
  Caption = 'Filtro Canny'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object SupLabel: TLabel
    Left = 192
    Top = 48
    Width = 51
    Height = 13
    Caption = '% superior:'
  end
  object InfLabel: TLabel
    Left = 192
    Top = 80
    Width = 45
    Height = 13
    Caption = '% inferior:'
  end
  object SupEdit: TEdit
    Left = 248
    Top = 40
    Width = 41
    Height = 21
    TabOrder = 0
    Text = '90'
    OnChange = SupEditChange
  end
  object InfEdit: TEdit
    Left = 248
    Top = 72
    Width = 41
    Height = 21
    TabOrder = 1
    Text = '60'
    OnChange = SupEditChange
  end
  object RadioGroup1: TRadioGroup
    Left = 8
    Top = 8
    Width = 169
    Height = 129
    Caption = 'Umbrales'
    ItemIndex = 0
    Items.Strings = (
      'Autom'#225'tico (% gradientes)'
      'Manual')
    TabOrder = 2
    OnClick = RadioGroup1Click
  end
  object OKButton: TButton
    Left = 80
    Top = 152
    Width = 75
    Height = 25
    Caption = 'OK'
    ModalResult = 1
    TabOrder = 3
  end
  object CancelButton: TButton
    Left = 168
    Top = 152
    Width = 75
    Height = 25
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 4
  end
end
