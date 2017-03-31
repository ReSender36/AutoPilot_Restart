object frmLauncherOptions: TfrmLauncherOptions
  Left = 0
  Top = 0
  Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080' '#1079#1072#1087#1091#1089#1082#1072#1090#1086#1088#1072
  ClientHeight = 148
  ClientWidth = 782
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
  object GroupBox1: TGroupBox
    Left = 0
    Top = 8
    Width = 249
    Height = 132
    Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080' '#1087#1086#1076#1082#1083#1102#1095#1077#1085#1080#1103
    TabOrder = 0
    object lblIPSrv: TLabel
      Left = 3
      Top = 24
      Width = 47
      Height = 13
      Caption = 'IP '#1072#1076#1088#1077#1089':'
    end
    object lblNameSrv: TLabel
      Left = 3
      Top = 48
      Width = 67
      Height = 13
      Caption = #1048#1084#1103' '#1089#1077#1088#1074#1077#1088#1072':'
    end
    object lblLoginSrv: TLabel
      Left = 3
      Top = 80
      Width = 97
      Height = 13
      Caption = #1048#1084#1103' '#1087#1086#1083#1100#1079#1086#1074#1072#1090#1077#1083#1103':'
    end
    object lblPasswordSrv: TLabel
      Left = 3
      Top = 104
      Width = 115
      Height = 13
      Caption = #1055#1072#1088#1086#1083#1100' '#1087#1086#1083#1100#1079#1086#1074#1072#1090#1077#1083#1103':'
    end
    object edServerIP: TEdit
      Left = 120
      Top = 21
      Width = 121
      Height = 21
      TabOrder = 0
    end
    object edServerName: TEdit
      Left = 120
      Top = 45
      Width = 121
      Height = 21
      TabOrder = 1
    end
    object edUserName: TEdit
      Left = 120
      Top = 77
      Width = 121
      Height = 21
      TabOrder = 2
    end
    object edPassword: TEdit
      Left = 120
      Top = 101
      Width = 121
      Height = 21
      TabOrder = 3
    end
  end
  object GroupBox2: TGroupBox
    Left = 255
    Top = 8
    Width = 519
    Height = 105
    Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080' '#1087#1077#1088#1077#1079#1072#1087#1091#1089#1082#1072
    TabOrder = 1
    object lblAutopilotPath: TLabel
      Left = 3
      Top = 24
      Width = 204
      Height = 13
      Caption = #1055#1091#1090#1100' '#1079#1072#1087#1091#1089#1082#1072' '#1087#1086#1083#1100#1079#1086#1074#1072#1090#1077#1083#1103' '#1040#1074#1090#1086#1087#1080#1083#1086#1090':'
    end
    object lblServicePath: TLabel
      Left = 3
      Top = 48
      Width = 185
      Height = 13
      Caption = #1055#1091#1090#1100' '#1079#1072#1087#1091#1089#1082#1072' '#1087#1086#1083#1100#1079#1086#1074#1072#1090#1077#1083#1103' '#1057#1077#1088#1074#1080#1089':'
    end
    object lblTimer: TLabel
      Left = 3
      Top = 80
      Width = 191
      Height = 13
      Hint = 
        #1044#1083#1080#1090#1077#1083#1100#1085#1086#1089#1090#1100' '#1087#1072#1091#1079#1099' '#1085#1072' '#1086#1089#1090#1072#1085#1086#1074#1082#1091' '#1072#1074#1090#1086#1087#1080#1083#1086#1090#1072' '#1087#1077#1088#1077#1076' '#1087#1086#1074#1090#1086#1088#1085#1099#1084' '#1079#1072#1087#1091#1089 +
        #1082#1086#1084
      Caption = #1055#1077#1088#1080#1086#1076' '#1089#1088#1072#1073#1072#1090#1099#1074#1072#1085#1080#1103' '#1090#1072#1081#1084#1077#1088#1072' ('#1089#1077#1082'):'
      ParentShowHint = False
      ShowHint = True
    end
    object edAutopilotPath: TEdit
      Left = 213
      Top = 21
      Width = 303
      Height = 21
      TabOrder = 0
    end
    object edServicePath: TEdit
      Left = 213
      Top = 45
      Width = 303
      Height = 21
      TabOrder = 1
    end
    object edTimer: TEdit
      Left = 213
      Top = 77
      Width = 121
      Height = 21
      TabOrder = 2
    end
  end
  object BitBtn1: TBitBtn
    Left = 600
    Top = 119
    Width = 75
    Height = 25
    Kind = bkOK
    NumGlyphs = 2
    TabOrder = 2
    OnClick = BitBtn1Click
  end
  object BitBtn2: TBitBtn
    Left = 699
    Top = 119
    Width = 75
    Height = 25
    Kind = bkCancel
    NumGlyphs = 2
    TabOrder = 3
    OnClick = BitBtn2Click
  end
end
