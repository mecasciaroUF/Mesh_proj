object AjusteForm1: TAjusteForm1
  Left = 245
  Top = 229
  Width = 392
  Height = 199
  Caption = 'Ajuste Cil'#237'ndrico'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object DeltaLabel: TLabel
    Left = 80
    Top = 40
    Width = 28
    Height = 13
    Caption = 'Delta:'
  end
  object ZTDeltaLabel: TLabel
    Left = 24
    Top = 80
    Width = 80
    Height = 13
    Caption = 'Zero-Term-Delta:'
  end
  object UsualDeltaEdit: TEdit
    Left = 120
    Top = 32
    Width = 65
    Height = 21
    TabOrder = 0
    Text = '0.05'
  end
  object ZeroTermDeltaEdit: TEdit
    Left = 120
    Top = 72
    Width = 65
    Height = 21
    TabOrder = 1
    Text = '0.00025'
  end
  object AjustarButton: TButton
    Left = 120
    Top = 128
    Width = 75
    Height = 25
    Caption = 'Ajustar'
    ModalResult = 1
    TabOrder = 2
  end
  object CancelAjustarButton: TButton
    Left = 216
    Top = 128
    Width = 75
    Height = 25
    Caption = 'Cancelar'
    ModalResult = 2
    TabOrder = 3
  end
  object AutovectorRadioGroup: TRadioGroup
    Left = 208
    Top = 16
    Width = 161
    Height = 97
    Caption = 'Autovalor de Ajuste'
    Items.Strings = (
      'Primario'
      'Secundario'
      'Terciario')
    TabOrder = 4
  end
end
