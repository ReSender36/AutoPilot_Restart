object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 202
  ClientWidth = 447
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Button1: TButton
    Left = 168
    Top = 128
    Width = 75
    Height = 25
    Caption = 'Button1'
    TabOrder = 0
    OnClick = Button1Click
  end
  object Edit1: TEdit
    Left = 56
    Top = 40
    Width = 361
    Height = 21
    TabOrder = 1
  end
  object FDGUIxWaitCursor1: TFDGUIxWaitCursor
    Provider = 'Forms'
    Left = 40
    Top = 144
  end
  object FDPhysMySQLDriverLink1: TFDPhysMySQLDriverLink
    DriverID = 'MySQL1'
    Left = 40
    Top = 96
  end
  object fdc: TFDConnection
    Params.Strings = (
      'DriverID=MySQL1'
      'CharacterSet=cp1251'
      'Server=localhost'
      'User_Name=root'
      'Password=TykadKme27'
      'Database=SM')
    LoginPrompt = False
    Left = 24
    Top = 32
  end
  object FDQuery1: TFDQuery
    Connection = fdc
    Left = 352
    Top = 112
  end
  object FDCommand1: TFDCommand
    Connection = fdc
    Left = 280
    Top = 112
  end
end
