object frmAutoPilotRestart: TfrmAutoPilotRestart
  Left = 0
  Top = 0
  Caption = #1055#1077#1088#1077#1079#1072#1087#1091#1089#1082' '#1072#1074#1090#1086#1087#1080#1083#1086#1090#1072
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
  object Button2: TButton
    Left = 296
    Top = 112
    Width = 75
    Height = 25
    Caption = 'Button2'
    TabOrder = 2
    OnClick = Button2Click
  end
  object TimerToRestart: TTimer
    Enabled = False
    Interval = 3000
    OnTimer = TimerToRestartTimer
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
  object TrayIcon1: TTrayIcon
    Icon.Data = {
      0000010001002020100000000000E80200001600000028000000200000004000
      0000010004000000000000020000000000000000000000000000000000000000
      0000000080000080000000808000800000008000800080800000C0C0C0008080
      80000000FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000077000000000000000000000000000000
      0770000000000000000000000000000000770000000000000000000000000000
      7707700777000000000000000000000777707707777700000000000000000007
      7777777777777000000000000000000700000000007777000000000000000000
      7777777777707770000000000000000007777777777707770000000000000000
      0077777777777077700000000000000000077777777777077700000000000000
      0000777777777770770000000000000000000777777777777700000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000FFFFFFFFFFFFFFF000000
      00000000000F0F0F0F0F0F0F0F00000000000000000FFFFFFFFFFFFFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000077000000000000000000000000000000077000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF3FFFFFFE1FFFFFFF0FF
      FFFFF003FFFFE000FFFFC0007FFFC0003FFFC0001FFFE0000FFFF00007FFF800
      03FFFC0001FFFE0001FFFF0001FFFF8003FFFFFFFFFFFFFE0003FFFC0001FFFC
      0000FFFC0000FFFE0000FFFF0001FFFFFE1FFFFFFF0FFFFFFF8FFFFFFFFF}
    PopupMenu = PopupMenu1
    OnDblClick = TrayIcon1DblClick
    Left = 384
    Top = 32
  end
  object PopupMenu1: TPopupMenu
    Left = 120
    Top = 136
    object N1: TMenuItem
      Caption = #1056#1072#1079#1074#1077#1088#1085#1091#1090#1100
      OnClick = N1Click
    end
    object N2: TMenuItem
      Caption = #1055#1072#1088#1072#1084#1077#1090#1088#1099
    end
    object N3: TMenuItem
      Caption = #1055#1088#1077#1088#1074#1072#1090#1100' '#1087#1088#1086#1094#1077#1076#1091#1088#1091' '#1087#1077#1088#1077#1079#1072#1087#1091#1089#1082#1072
      OnClick = N3Click
    end
  end
end
