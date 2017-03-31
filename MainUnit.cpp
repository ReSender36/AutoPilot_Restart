//---------------------------------------------------------------------------

#include <vcl.h>
#include <windows.h>
#include <stdio.h>
#include <shellapi.h>
#include <inifiles.hpp>
#pragma hdrstop

#include "MainUnit.h"
#include "OptionsUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmAutoPilotRestart *frmAutoPilotRestart;

static String PARAM = "" ;
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

//---------------------------------------------------------------------------
__fastcall TfrmAutoPilotRestart::TfrmAutoPilotRestart(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmAutoPilotRestart::FormCreate(TObject *Sender)
{
   LPWSTR *szArglist;
   int nArgs;
   int i;

//	String param[3] = "" ; // тут мы пытаемс€ провести эксперимент на тему получени€ массива параметров
//	дл€ того, чтобы получать несколько параметров и обращатьс€ ним по индексам массива, а не по строковым обозначени€м


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

//	Application->ShowMainForm = false ;
//	frmAutoPilotRestart->Visible = false ;
	if("-o" == PARAM){
	   Application->CreateForm(__classid(TfrmLauncherOptions), &frmLauncherOptions) ;
	   frmLauncherOptions->Visible = true ;
	}

// Free memory allocated for CommandLineToArgvW arguments.
   LocalFree(szArglist);
   return;
}
//---------------------------------------------------------------------------
String getOptionValue(String db, String table, String option)
{
	String strResult = "" ;
	frmAutoPilotRestart->FDQuery1->SQL->Text = String("select value from " + db + ".dbo." + table + " where option_name = '" + option + "'") ;
	frmAutoPilotRestart->FDQuery1->Active = true ;
	frmAutoPilotRestart->FDQuery1->First() ;
	while(!frmAutoPilotRestart->FDQuery1->Eof){
		strResult = frmAutoPilotRestart->FDQuery1->FieldByName("value")->AsString ;
		frmAutoPilotRestart->FDQuery1->Next() ;
	}
	return strResult ;
}
//---------------------------------------------------------------------------
void __fastcall TfrmAutoPilotRestart::Button1Click(TObject *Sender)
{

	if("-r" == PARAM){
		if(db_connect()){
			FDCommand1->CommandText->Add("update " + DB_WMS + ".dbo." + DB_WMS_TABLE_CONST + " set value = '1' where id = '" + DB_WMS_CONST_AUTOPILOT_STOP + "' ;") ;
			FDCommand1->CommandText->Add("update " + DB_MONITOR + ".dbo." + DB_MON_OPTABLE + " set value = 'Y' where option_name = '" + DB_MON_OPTION_AUTORESTART + "' ;") ;
			try{
				FDCommand1->Execute() ;
			}catch(...){

			}
		}
		int iInterval = StrToInt(getOptionValue(DB_MONITOR, DB_MON_OPTABLE, DB_MON_OPTION_TIMERRESTARTDELAY)) ;
		TimerToRestart->Interval = iInterval ;
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
}
//---------------------------------------------------------------------------
void __fastcall TfrmAutoPilotRestart::TimerToRestartTimer(TObject *Sender)
{
	String strVal = getOptionValue(DB_MONITOR, DB_MON_OPTABLE, DB_MON_OPTION_AUTORESTART) ;
	String strAutopilotShortcut = getOptionValue(DB_MONITOR, DB_MON_OPTABLE, DB_MON_OPTION_AUTOPILOTSHORTCUT) ;
	String strServiceShortcut = getOptionValue(DB_MONITOR, DB_MON_OPTABLE, DB_MON_OPTION_SERVICESHORTCUT) ;

	if("Y" == strVal){
	// снимаем флаг, что происходит автоматический рестарт. «ащита от рестарта с иконки на рабочем столе
		FDCommand1->CommandText->Add("update " + DB_MONITOR + ".dbo." + DB_MON_OPTABLE + " set value = 'N' where option_name = '" + DB_MON_OPTION_AUTORESTART + "' ;") ;
		try{
			FDCommand1->Execute() ;
		}catch(...){
		}
	// запускаем
		STARTUPINFO StartInfo = { sizeof(TStartupInfo) } ;
		PROCESS_INFORMATION ProcInfo ;
		LPCTSTR s ;
		StartInfo.cb = sizeof(StartInfo) ;
		StartInfo.dwFlags = STARTF_USESHOWWINDOW ;
		StartInfo.wShowWindow = SW_SHOWNORMAL ;
		String strProg = strAutopilotShortcut ; //   "calc.exe" ;

		if(!CreateProcess(NULL, strProg.w_str(),NULL,NULL,false,CREATE_NEW_CONSOLE|HIGH_PRIORITY_CLASS,NULL,NULL,&StartInfo,&ProcInfo)){
			ShowMessage("ќшибка запуска автопилота: " + SysErrorMessage(GetLastError())) ;
		}
//		else{
//			if(WaitForSingleObject(ProcInfo.hProcess,10000) == WAIT_TIMEOUT) {}
//				ShowMessage("ѕроцесс пока идет") ;
//			CloseHandle(ProcInfo.hProcess) ;
//		}

//		Sleep(10000) ;
//		strProg = strServiceShortcut ;// "notepad.exe" ;
//		if(!CreateProcess(NULL, strProg.w_str(),NULL,NULL,false,CREATE_NEW_CONSOLE|HIGH_PRIORITY_CLASS,NULL,NULL,&StartInfo,&ProcInfo)){
//			ShowMessage("ќшибка запуска сервиса: " + SysErrorMessage(GetLastError())) ;
//		}
//        CreateProcess(NULL, strProg.w_str(),NULL,NULL,false,
//			CREATE_NEW_CONSOLE|HIGH_PRIORITY_CLASS,NULL,NULL,&StartInfo,&ProcInfo) ;
	}
//	Edit1->Text = getOptionValue(DB_MONITOR, DB_MON_OPTABLE, DB_MON_OPTION_TIMERRESTARTDELAY) ;
//	Edit1->Text = "Ќужен интервал 300000 миллисекунд" ;
	TimerToRestart->Enabled = false ;
	Application->Terminate() ;
}
//---------------------------------------------------------------------------

void __fastcall TfrmAutoPilotRestart::TrayIcon1DblClick(TObject *Sender)
{
	Application->ShowMainForm = true ;
	frmAutoPilotRestart->Visible = true ;
}
//---------------------------------------------------------------------------


void __fastcall TfrmAutoPilotRestart::N1Click(TObject *Sender)
{
//	Application->ShowMainForm = true ;
	frmAutoPilotRestart->Show() ;
	TrayIcon1->Visible = false ;
}
//---------------------------------------------------------------------------

void __fastcall TfrmAutoPilotRestart::N3Click(TObject *Sender)
{
// снимаем флаг, что происходит автоматический рестарт. «ащита от рестарта с иконки на рабочем столе
	if(db_connect()){
		FDCommand1->CommandText->Add("update " + DB_MONITOR + ".dbo." + DB_MON_OPTABLE + " set value = 'N' where option_name = '" + DB_MON_OPTION_AUTORESTART + "' ;") ;
		try{
			FDCommand1->Execute() ;
		}catch(...){
		}
	}

	Application->Terminate() ;
}
//---------------------------------------------------------------------------

void __fastcall TfrmAutoPilotRestart::Button2Click(TObject *Sender)
{
	TrayIcon1->Visible = true ;
 	frmAutoPilotRestart->Hide() ;
}
//---------------------------------------------------------------------------

void __fastcall TfrmAutoPilotRestart::FormShow(TObject *Sender)
{
//	Edit1->Text = frmAutoPilotRestart->DB_MON_OPTION_AUTORESTART ;
}
//---------------------------------------------------------------------------

