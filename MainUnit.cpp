//---------------------------------------------------------------------------

#include <vcl.h>
#include <windows.h>
#include <stdio.h>
#include <shellapi.h>
#include <inifiles.hpp>
#pragma hdrstop

#include "MainUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmAutoPilotRestart *frmAutoPilotRestart;
String PARAM = "" ;
String DB_WMS = "SKLAD1" ;
String DB_WMS_TABLE_CONST = "_1SCONST" ;
String DB_WMS_CONST_AUTOPILOT_STOP = "3358" ;
String DB_MONITOR = "DB1S_monitor" ;
String DB_MON_OPTABLE = "OPTIONS" ;
String DB_MON_LOGTABLE = "LOGS" ;
//---------------------------------------------------------------------------
__fastcall TfrmAutoPilotRestart::TfrmAutoPilotRestart(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
bool db_connect()
{
	TIniFile *ini ;

	ini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini")) ;
	String Address = ini->ReadString("DB_Param", "Address", 1) ;
	String Server = ini->ReadString("DB_Param", "Server", 1) ;
	String UserName = ini->ReadString("DB_Param", "UserName", 1) ;
	String Password = ini->ReadString("DB_Param", "Password", 1) ;
	ini->Free() ;

	try{
		if (!frmAutoPilotRestart->FDConnection1->Connected){
			frmAutoPilotRestart->FDConnection1->Params->Add("Address=" + Address) ;
			frmAutoPilotRestart->FDConnection1->Params->Add("Server=" + Server) ;
			frmAutoPilotRestart->FDConnection1->Params->Add("UserName=" + UserName) ;
			frmAutoPilotRestart->FDConnection1->Params->Add("Password=" + Password) ;
			frmAutoPilotRestart->FDConnection1->Connected = true ;
			if (frmAutoPilotRestart->FDConnection1->Connected)
				return true ;
		}
	}
	catch(...){
		int q_conn = Application->MessageBox(String("Проблемы при подключении к БД " + GetLastError()).w_str(),String("Проблема").w_str(),MB_OK) ;
		switch(q_conn){
			case IDYES:
				break ;
			case IDNO:
				break ;
		}
		return false ;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAutoPilotRestart::FormCreate(TObject *Sender)
{
   LPWSTR *szArglist;
   int nArgs;
   int i;

//	String param[3] = "" ; // тут мы пытаемся провести эксперимент на тему получения массива параметров
//	для того, чтобы получать несколько параметров и обращаться ним по индексам массива, а не по строковым обозначениям


   szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
   if( NULL == szArglist )
   {
	  wprintf(L"CommandLineToArgvW failed\n");
	  return ;
   }
   else
   	for( i=0; i<nArgs; i++){
//		Edit1->Text = szArglist[i] ; //printf("%d: %ws\n", i, szArglist[i]);
		PARAM = szArglist[i] ;
	}

	Application->ShowMainForm = false ;

	if("-r" == PARAM)

// Free memory allocated for CommandLineToArgvW arguments.
   LocalFree(szArglist);
   return;
}
//---------------------------------------------------------------------------
void __fastcall TfrmAutoPilotRestart::Button1Click(TObject *Sender)
{

	if("-r" == PARAM){
		if(db_connect()){
			FDCommand1->CommandText->Add("update " + DB_WMS + ".dbo." + DB_WMS_TABLE_CONST + " set value = '1' where id = '" + DB_WMS_CONST_AUTOPILOT_STOP + "' ;") ;
			FDCommand1->CommandText->Add("update " + DB_MONITOR + ".dbo." + DB_MON_OPTABLE + " set value = 'Y' where option_name = 'AutopilotRestart' ;") ;
			try{
				FDCommand1->Execute() ;
			}catch(...){

			}
		}
		TimerToRestart->Enabled = true ;
	}else{
		if("-d" == PARAM){
			if(db_connect()){
				FDCommand1->CommandText->Add("update " + DB_WMS + ".dbo." + DB_WMS_TABLE_CONST + " set value = '1' where id = '" + DB_WMS_CONST_AUTOPILOT_STOP + "' ;") ;
				try{
					FDCommand1->Execute() ;
				}catch(...){

				}
			}
            Application->Terminate() ;
		}
	}

//	FDCommand1->CommandText->Add("update DB_1SMonitor.dbo.options set value = 'No' where option_name = 'AutopilotRestart' ;") ;
//	FDCommand1->Execute() ;



}
//---------------------------------------------------------------------------
void __fastcall TfrmAutoPilotRestart::TimerToRestartTimer(TObject *Sender)
{
	String val = "" ;
	FDQuery1->SQL->Text = String("select value from " + DB_MONITOR + ".dbo." + DB_MON_OPTABLE + " where option_name = 'AutopilotRestart'") ;
	FDQuery1->Active = true ;
	FDQuery1->First() ;
	while(!FDQuery1->Eof){
		val = FDQuery1->FieldByName("value")->AsString ;
		FDQuery1->Next() ;
	}
	if("Y" == val){
	// стартуем
		STARTUPINFO StartInfo = { sizeof(TStartupInfo) } ;
		PROCESS_INFORMATION ProcInfo ;
		LPCTSTR s ;
		StartInfo.cb = sizeof(StartInfo) ;
		StartInfo.dwFlags = STARTF_USESHOWWINDOW ;
		StartInfo.wShowWindow = SW_SHOWNORMAL ;
		String strProg = "Calc.exe" ;
		/*
		if(!CreateProcess(NULL, strProg.w_str(),NULL,NULL,false,
			CREATE_NEW_CONSOLE|HIGH_PRIORITY_CLASS,NULL,NULL,&StartInfo,&ProcInfo)){
//		ShowMessage("Ошибка: " + SysErrorMessage(GetLastError())) ;
		}
		else{
			if(WaitForSingleObject(ProcInfo.hProcess,10000) == WAIT_TIMEOUT) {}
//				ShowMessage("Процесс пока идет") ;
//			CloseHandle(ProcInfo.hProcess) ;
		}
		*/
        CreateProcess(NULL, strProg.w_str(),NULL,NULL,false,
			CREATE_NEW_CONSOLE|HIGH_PRIORITY_CLASS,NULL,NULL,&StartInfo,&ProcInfo) ;
	}
	Edit1->Text = "Нужен интервал 300000 миллисекунд" ;
	Application->Terminate() ;
}
//---------------------------------------------------------------------------

void __fastcall TfrmAutoPilotRestart::TrayIcon1DblClick(TObject *Sender)
{
	Application->ShowMainForm = true ;
}
//---------------------------------------------------------------------------


