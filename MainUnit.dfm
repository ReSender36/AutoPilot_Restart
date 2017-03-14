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
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Edit1: TEdit
    Left = 104
    Top = 72
    Width = 121
    Height = 21
    TabOrder = 0
    Text = 'Edit1'
  end
  object Button1: TButton
    Left = 296
    Top = 152
    Width = 75
    Height = 25
    Caption = 'Button1'
    TabOrder = 1
    OnClick = Button1Click
  end
  object TimerToRestart: TTimer
    Left = 32
    Top = 136
  end
  object FDPhysMSSQLDriverLink1: TFDPhysMSSQLDriverLink
    DriverID = 'MS_SQL'
    ODBCDriver = 'SQL Server'
    Left = 304
    Top = 64
  end
  object FDConnection1: TFDConnection
    Params.Strings = (
      'DriverID=MS_SQL')
    LoginPrompt = False
    Left = 208
    Top = 136
  end
  object FDGUIxWaitCursor1: TFDGUIxWaitCursor
    Provider = 'Forms'
    Left = 224
    Top = 16
  end
  object FDCommand1: TFDCommand
    Connection = FDConnection1
    Left = 24
    Top = 24
  end
  object FDQuery1: TFDQuery
    Connection = FDConnection1
    Left = 24
    Top = 80
  end
end
