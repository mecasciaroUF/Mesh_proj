object SegmentaForm1: TSegmentaForm1
  Left = 296
  Top = 202
  Width = 272
  Height = 202
  Caption = 'Par'#225'metros de Segmentaci'#243'n'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object umbralLabel: TLabel
    Left = 48
    Top = 16
    Width = 74
    Height = 13
    Caption = 'Umbral (HU):'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object radio0Label: TLabel
    Left = 48
    Top = 48
    Width = 80
    Height = 13
    Caption = 'Radio Inicial: '
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object umbralPCLabel: TLabel
    Left = 48
    Top = 80
    Width = 117
    Height = 13
    Caption = 'Puntos de Contacto:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object umbralEdit: TEdit
    Left = 136
    Top = 8
    Width = 49
    Height = 21
    TabOrder = 0
    Text = '-224'
  end
  object radio0Edit: TEdit
    Left = 136
    Top = 40
    Width = 49
    Height = 21
    TabOrder = 1
    Text = '20'
  end
  object umbralPCEdit: TEdit
    Left = 168
    Top = 72
    Width = 49
    Height = 21
    TabOrder = 2
    Text = '25'
  end
  object AceptaButton: TButton
    Left = 56
    Top = 120
    Width = 75
    Height = 25
    Caption = 'Aceptar'
    ModalResult = 1
    TabOrder = 3
  end
  object CancelaButton: TButton
    Left = 144
    Top = 120
    Width = 75
    Height = 25
    Caption = 'Cancelar'
    ModalResult = 2
    TabOrder = 4
  end
end
