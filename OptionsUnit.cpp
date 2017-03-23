//---------------------------------------------------------------------------
#include <vcl.h>
#include <inifiles.hpp>

#pragma hdrstop

#include "OptionsUnit.h"
#include "MainUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

String static DB_MONITOR = "DB1S_monitor" ;
String static DB_MON_OPTABLE = "OPTIONS" ;

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
void __fastcall TfrmLauncherOptions::FormCreate(TObject *Sender)
{
	TIniFile *ini ;
	ini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini")) ;

	ADDRESS = ini->ReadString("DB_Param", "Address", 1) ;
	SERVER_NAME = ini->ReadString("DB_Param", "Server", 1) ;
	USERNAME = ini->ReadString("DB_Param", "UserName", 1) ;
	PASSWORD = ini->ReadString("DB_Param", "Password", 1) ;

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
	}

	edServerIP->Text = ADDRESS ;
	edServerName->Text = SERVER_NAME ;
	edUserName->Text = USERNAME ;
	edPassword->Text = PASSWORD ;
	ini->Free() ;
}
//---------------------------------------------------------------------------
void __fastcall TfrmLauncherOptions::BitBtn2Click(TObject *Sender)
{
	frmLauncherOptions->Visible = false ;
	Application->Terminate() ;
}
//---------------------------------------------------------------------------
