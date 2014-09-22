object Form1: TForm1
  Left = 192
  Top = 128
  Width = 766
  Height = 484
  Caption = 'Form1'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object FileDirLabel: TLabel
    Left = 16
    Top = 40
    Width = 3
    Height = 13
    Caption = '-'
  end
  object CountLabel: TLabel
    Left = 136
    Top = 8
    Width = 3
    Height = 13
    Caption = '-'
  end
  object ProgramLabel: TLabel
    Left = 8
    Top = 384
    Width = 3
    Height = 13
    Caption = '-'
  end
  object ListBox1: TListBox
    Left = 8
    Top = 64
    Width = 497
    Height = 313
    ItemHeight = 13
    TabOrder = 0
  end
  object AbrirButton: TButton
    Left = 8
    Top = 8
    Width = 75
    Height = 25
    Caption = 'Abrir'
    TabOrder = 1
    OnClick = AbrirButtonClick
  end
  object ProgressBar1: TProgressBar
    Left = 8
    Top = 416
    Width = 497
    Height = 17
    Min = 0
    Max = 100
    Smooth = True
    TabOrder = 2
  end
  object DirectoryListBox1: TDirectoryListBox
    Left = 520
    Top = 64
    Width = 217
    Height = 313
    ItemHeight = 16
    TabOrder = 3
  end
  object OpenDialog1: TOpenDialog
    Left = 96
    Top = 8
  end
end
