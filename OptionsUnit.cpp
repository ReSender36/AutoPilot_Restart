//---------------------------------------------------------------------------
#include <vcl.h>
#include <inifiles.hpp>

#pragma hdrstop

#include "OptionsUnit.h"
#include "MainUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

const String DB_WMS = "SKLAD1" ;
const String DB_WMS_TABLE_CONST = "_1SCONST" ;
const String DB_WMS_CONST_AUTOPILOT_STOP = "3358" ;
const String DB_MONITOR = "DB1S_monitor" ;
const String DB_MON_OPTABLE = "OPTIONS" ;
const String DB_MON_OPTION_AUTORESTART = "AutopilotRestart" ;
const String DB_MON_OPTION_AUTOPILOTSHORTCUT = "Str_AutopilotShortcut" ;
const String DB_MON_OPTION_SERVICESHORTCUT = "Str_ServiceShortcut" ;
const String DB_MON_OPTION_TIMERRESTARTDELAY = "TimerRestartDelay" ;
const String DB_MON_LOGTABLE = "LOGS" ;

String static ADDRESS = "" ;
String static SERVER_NAME = "" ;
String static USERNAME = "" ;
String static PASSWORD = "" ;

TfrmLauncherOptions *frmLauncherOptions;
//---------------------------------------------------------------------------
__fastcall TfrmLauncherOptions::TfrmLauncherOptions(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void setOptionValue(String db, String table, String option)
{

}
//---------------------------------------------------------------------------
void __fastcall TfrmLauncherOptions::FormCreate(TObject *Sender)
{
	TIniFile *ini ;
	ini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini")) ;

	ADDRESS = ini->ReadString("DB_Param", "Address", 1) ;
	SERVER_NAME = ini->ReadString("DB_Param", "Server", 1) ;
	USERNAME = ini->ReadString("DB_Param", "UserName", 1) ;
	PASSWORD = ini->ReadString("DB_Param", "Password", 1) ;
	ini->Free() ;

	String str = "SELECT VALUE, OPTION_NAME FROM " + DB_MONITOR + ".DBO." + DB_MON_OPTABLE + " WHERE OPTION_NAME IN('AutopilotRestart','Str_AutopilotShortcut','Str_ServiceShortcut','TimerRestartDelay') ;"  ;
	if(frmAutoPilotRestart->db_connect()){
		frmAutoPilotRestart->FDQuery1->Active = false ;
		frmAutoPilotRestart->FDQuery1->SQL->Text = String(str) ;
		frmAutoPilotRestart->FDQuery1->Active = true ;
		frmAutoPilotRestart->FDQuery1->First() ;
		while(!frmAutoPilotRestart->FDQuery1->Eof){
			if("Str_AutopilotShortcut" == frmAutoPilotRestart->FDQuery1->FieldByName("OPTION_NAME")->AsString)
				edAutopilotPath->Text = frmAutoPilotRestart->FDQuery1->FieldByName("VALUE")->AsString ;
			if("Str_ServiceShortcut" == frmAutoPilotRestart->FDQuery1->FieldByName("OPTION_NAME")->AsString)
				edServicePath->Text = frmAutoPilotRestart->FDQuery1->FieldByName("VALUE")->AsString ;
			if("TimerRestartDelay" == frmAutoPilotRestart->FDQuery1->FieldByName("OPTION_NAME")->AsString)
				edTimer->Text = IntToStr(frmAutoPilotRestart->FDQuery1->FieldByName("VALUE")->AsInteger / 1000) ;

			frmAutoPilotRestart->FDQuery1->Next() ;
		}
		if(frmAutoPilotRestart->db_disconnect()) {}
	}

	edServerIP->Text = ADDRESS ;
	edServerName->Text = SERVER_NAME ;
	edUserName->Text = USERNAME ;
	edPassword->Text = PASSWORD ;

}
//---------------------------------------------------------------------------
void __fastcall TfrmLauncherOptions::BitBtn2Click(TObject *Sender)
{
	frmLauncherOptions->Visible = false ;
	Application->Terminate() ;
}
//---------------------------------------------------------------------------
void __fastcall TfrmLauncherOptions::BitBtn1Click(TObject *Sender)
{
	TIniFile *ini ;
	ini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini")) ;

	ini->WriteString("DB_Param", "Address", edServerIP->Text) ;
	ini->WriteString("DB_Param", "Server", edServerName->Text) ;
	ini->WriteString("DB_Param", "UserName", edUserName->Text) ;
	ini->WriteString("DB_Param", "Password", edPassword->Text) ;
	ini->Free() ;

//	setOptionValue(DB_MONITOR, DB_MON_OPTABLE,)

	frmLauncherOptions->Visible = false ;
//	frmAutoPilotRestart->DB_MON_OPTION_AUTORESTART = "mega stroka" ;
//	frmAutoPilotRestart->Visible = true ;
	if (!frmAutoPilotRestart->TimerToRestart->Enabled) {
		Application->Terminate() ;
	}
}
//---------------------------------------------------------------------------

