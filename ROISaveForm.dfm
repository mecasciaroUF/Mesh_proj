object ROISaveForm1: TROISaveForm1
  Left = 192
  Top = 114
  Width = 206
  Height = 185
  Caption = 'ROI/VOI/Window'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object AceptarROIButton: TButton
    Left = 16
    Top = 120
    Width = 75
    Height = 25
    Caption = 'Grabar'
    ModalResult = 1
    TabOrder = 0
  end
  object CancelarROIButton: TButton
    Left = 104
    Top = 120
    Width = 75
    Height = 25
    Caption = 'Cancelar'
    ModalResult = 2
    TabOrder = 1
  end
  object ROIRadioGroup: TRadioGroup
    Left = 24
    Top = 8
    Width = 145
    Height = 73
    Caption = 'ROI/VOI'
    Items.Strings = (
      'ROI'
      'VOI')
    TabOrder = 2
  end
  object WindowSaveCheckBox: TCheckBox
    Left = 24
    Top = 88
    Width = 145
    Height = 17
    Caption = 'Window'
    TabOrder = 3
  end
end
