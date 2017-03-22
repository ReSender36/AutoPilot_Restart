//---------------------------------------------------------------------------
#include <vcl.h>
#include <inifiles.hpp>

#pragma hdrstop

#include "OptionsUnit.h"
#include "MainUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

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



	edServerIP->Text = ADDRESS ;
	edServerName->Text = SERVER_NAME ;
	edUserName->Text = USERNAME ;
	edPassword->Text = PASSWORD ;
	ini->Free() ;
}
//---------------------------------------------------------------------------
