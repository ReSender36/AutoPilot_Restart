//---------------------------------------------------------------------------

#include <vcl.h>
#include <windows.h>
#include <stdio.h>
#include <shellapi.h>
#include <inifiles.hpp>
#pragma hdrstop

#include "MainUnit.h"
#include "DMMainUnit.h"
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
const String DB_MON_OPTION_TIMERRESTARTREPEAT = "TimerRestartRepeat" ;

const String DB_MON_LOGTABLE = "LOGS" ;
const String DB_MON_LOG_EVENTNUM = "event_num" ;
const String DB_MON_LOG_TRAN_DATE = "tran_date" ;

const short DB_MON_EVENT_AUTOPILOT_SHUTDOWN = 158 ;
const short DB_MON_EVENT_AUTOPILOT_STARTED = 157 ;

//---------------------------------------------------------------------------
__fastcall TfrmAutoPilotRestart::TfrmAutoPilotRestart(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void hideMainForm(){
	frmAutoPilotRestart->TrayIcon1->Visible = true ;
	frmAutoPilotRestart->Hide() ;
	frmAutoPilotRestart->Visible = False ;
}
//---------------------------------------------------------------------------
String getOptionValue(String db, String table, String option)
{
	String strResult = "" ;

	String str_query = String("select value from " + db + ".dbo." + table + " where option_name = '" + option + "'") ;

	frmAutoPilotRestart->FDQuery1->SQL->Text = str_query ;
	frmAutoPilotRestart->FDQuery1->Active = true ;
	frmAutoPilotRestart->FDQuery1->First() ;
	while(!frmAutoPilotRestart->FDQuery1->Eof){
		strResult = frmAutoPilotRestart->FDQuery1->FieldByName("value")->AsString ;
		frmAutoPilotRestart->FDQuery1->Next() ;
	}
	return strResult ;
}
//---------------------------------------------------------------------------
String getLastTogRecord(String db, String table, int event_num_first, int event_num_last = 0)
{
	String strResult = "" ;

	String str_query = String("SELECT event_num AS lastEvent FROM " + DB_MONITOR + ".dbo." + DB_MON_LOGTABLE +
		" WHERE " + DB_MON_LOG_EVENTNUM + " in(" + IntToStr(DB_MON_EVENT_AUTOPILOT_STARTED) + "," + IntToStr(DB_MON_EVENT_AUTOPILOT_SHUTDOWN) +
		") AND " + DB_MON_LOG_TRAN_DATE + " > (SELECT max("+ DB_MON_LOG_TRAN_DATE +") FROM " + DB_MONITOR + ".dbo."  + DB_MON_LOGTABLE +
		" WHERE " + DB_MON_LOG_EVENTNUM + " = " + IntToStr(DB_MON_EVENT_AUTOPILOT_SHUTDOWN) + ") ;") ;

	frmAutoPilotRestart->FDQuery1->SQL->Text = str_query ;
	frmAutoPilotRestart->FDQuery1->Active = true ;
	frmAutoPilotRestart->FDQuery1->First() ;
	while(!frmAutoPilotRestart->FDQuery1->Eof){
		strResult = frmAutoPilotRestart->FDQuery1->FieldByName("lastEvent")->AsString ;
		frmAutoPilotRestart->FDQuery1->Next() ;
	}
	return strResult ;
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
       recToLog(164) ;
	   Application->CreateForm(__classid(TfrmLauncherOptions), &frmLauncherOptions) ;
	   frmLauncherOptions->Visible = true ;
	}else{
		if("-r" == PARAM){
			recToLog(161) ;
			if(db_connect()){
				FDCommand1->CommandText->Add("update " + DB_WMS + ".dbo." + DB_WMS_TABLE_CONST + " set value = '1' where id = '" + DB_WMS_CONST_AUTOPILOT_STOP + "' ;") ;
				FDCommand1->CommandText->Add("update " + DB_MONITOR + ".dbo." + DB_MON_OPTABLE + " set value = 'Y' where option_name = '" + DB_MON_OPTION_AUTORESTART + "' ;") ;
				try{
					FDCommand1->Execute() ;
				}catch(...){}
				FDCommand1->CommandText->Clear() ;
			}
			int iInterval = StrToInt(getOptionValue(DB_MONITOR, DB_MON_OPTABLE, DB_MON_OPTION_TIMERRESTARTDELAY)) ;
			TimerToRestart->Interval = iInterval ;
			TimerToRestart->Enabled = true ;

//			TrayIcon1->Visible = true ;
//			frmAutoPilotRestart->Visible = false ;
//			frmAutoPilotRestart->Hide() ;
		}else{
			if("-d" == PARAM){
				recToLog(162) ;
				if(db_connect()){
					FDCommand1->CommandText->Add("update " + DB_WMS + ".dbo." + DB_WMS_TABLE_CONST + " set value = '1' where id = '" + DB_WMS_CONST_AUTOPILOT_STOP + "' ;") ;
					try{
						FDCommand1->Execute() ;
					}catch(...){}
					FDCommand1->CommandText->Clear() ;
				}
				Application->Terminate() ;
			}
		}
	}


// Free memory allocated for CommandLineToArgvW arguments.
   LocalFree(szArglist);
   return;
}
//---------------------------------------------------------------------------
void __fastcall TfrmAutoPilotRestart::Button1Click(TObject *Sender)
{

	if("-r" == PARAM){
		recToLog(161) ;
		if(db_connect()){
			FDCommand1->CommandText->Add("update " + DB_WMS + ".dbo." + DB_WMS_TABLE_CONST + " set value = '1' where id = '" + DB_WMS_CONST_AUTOPILOT_STOP + "' ;") ;
			FDCommand1->CommandText->Add("update " + DB_MONITOR + ".dbo." + DB_MON_OPTABLE + " set value = 'Y' where option_name = '" + DB_MON_OPTION_AUTORESTART + "' ;") ;
			try{
				FDCommand1->Execute() ;

			}catch(...){}
			FDCommand1->CommandText->Clear() ;
		}
		int iInterval = StrToInt(getOptionValue(DB_MONITOR, DB_MON_OPTABLE, DB_MON_OPTION_TIMERRESTARTDELAY)) ;
		TimerToRestart->Interval = iInterval ;
		TimerToRestart->Enabled = true ;

		TrayIcon1->Visible = true ;
		frmAutoPilotRestart->Hide() ;
	}else{
		if("-d" == PARAM){
			recToLog(162) ;
			if(db_connect()){
				FDCommand1->CommandText->Add("update " + DB_WMS + ".dbo." + DB_WMS_TABLE_CONST + " set value = '1' where id = '" + DB_WMS_CONST_AUTOPILOT_STOP + "' ;") ;
				try{
					FDCommand1->Execute() ;
				}catch(...){}
				FDCommand1->CommandText->Clear() ;
			}
			Application->Terminate() ;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAutoPilotRestart::TimerToRestartTimer(TObject *Sender)
{
	recToLog(163) ;
	String strGo = getLastTogRecord(DB_MONITOR, DB_MON_LOGTABLE, DB_MON_EVENT_AUTOPILOT_SHUTDOWN, DB_MON_EVENT_AUTOPILOT_STARTED) ;
	if ("157" != strGo) {
		String strVal = getOptionValue(DB_MONITOR, DB_MON_OPTABLE, DB_MON_OPTION_AUTORESTART);
		String strAutopilotShortcut = getOptionValue(DB_MONITOR, DB_MON_OPTABLE,DB_MON_OPTION_AUTOPILOTSHORTCUT);
		String strServiceShortcut = getOptionValue(DB_MONITOR, DB_MON_OPTABLE, DB_MON_OPTION_SERVICESHORTCUT);

		if ("Y" == strVal) {
			// снимаем флаг, что происходит автоматический рестарт. «ащита от рестарта с иконки на рабочем столе
			FDCommand1->CommandText->Add("update " + DB_MONITOR + ".dbo." +
				DB_MON_OPTABLE + " set value = 'N' where option_name = '" +
				DB_MON_OPTION_AUTORESTART + "' ;");
			try {
				FDCommand1->Execute();

			}
			catch (...) {}
			FDCommand1->CommandText->Clear() ;

			// запускаем
			STARTUPINFO StartInfo = {sizeof(TStartupInfo)};
			PROCESS_INFORMATION ProcInfo;
			LPCTSTR s;
			StartInfo.cb = sizeof(StartInfo);
			StartInfo.dwFlags = STARTF_USESHOWWINDOW;
			StartInfo.wShowWindow = SW_SHOWNORMAL;
			String strProg = strAutopilotShortcut; // "calc.exe" ;

			if (!CreateProcess(NULL, strProg.w_str(), NULL, NULL, false,
				CREATE_NEW_CONSOLE | HIGH_PRIORITY_CLASS, NULL, NULL,
				&StartInfo, &ProcInfo)) {
				ShowMessage("ќшибка запуска автопилота: " + SysErrorMessage
					(GetLastError()));
			}
			// else{
			// if(WaitForSingleObject(ProcInfo.hProcess,10000) == WAIT_TIMEOUT) {}
			// ShowMessage("ѕроцесс пока идет") ;
			// CloseHandle(ProcInfo.hProcess) ;
			// }

			// Sleep(10000) ;
			// strProg = strServiceShortcut ;// "notepad.exe" ;
			// if(!CreateProcess(NULL, strProg.w_str(),NULL,NULL,false,CREATE_NEW_CONSOLE|HIGH_PRIORITY_CLASS,NULL,NULL,&StartInfo,&ProcInfo)){
			// ShowMessage("ќшибка запуска сервиса: " + SysErrorMessage(GetLastError())) ;
			// }
			// CreateProcess(NULL, strProg.w_str(),NULL,NULL,false,
			// CREATE_NEW_CONSOLE|HIGH_PRIORITY_CLASS,NULL,NULL,&StartInfo,&ProcInfo) ;
		}
	}else{
        recToLog(167) ;
		TimerToRestart->Enabled = false ;
		int iInterval = StrToInt(getOptionValue(DB_MONITOR, DB_MON_OPTABLE, DB_MON_OPTION_TIMERRESTARTREPEAT)) ;
		TimerToRestart->Interval = iInterval ;
		TimerToRestart->Enabled = true ;

	}
	// Edit1->Text = getOptionValue(DB_MONITOR, DB_MON_OPTABLE, DB_MON_OPTION_TIMERRESTARTDELAY) ;
	// Edit1->Text = "Ќужен интервал 300000 миллисекунд" ;
	TimerToRestart->Enabled = false;
	Application->Terminate();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmAutoPilotRestart::TrayIcon1DblClick(TObject *Sender) {
	Application->ShowMainForm = true;
	frmAutoPilotRestart->Visible = true;
}
// ---------------------------------------------------------------------------

void __fastcall TfrmAutoPilotRestart::N1Click(TObject *Sender)
{
//	Application->ShowMainForm = true ;
	frmAutoPilotRestart->Show() ;
	TrayIcon1->Visible = false ;
}
//---------------------------------------------------------------------------

void __fastcall TfrmAutoPilotRestart::N3Click(TObject *Sender)
{
	recToLog(166) ;
// снимаем флаг, что происходит автоматический рестарт. «ащита от рестарта с иконки на рабочем столе
	if(db_connect()){
		FDCommand1->CommandText->Add("update " + DB_MONITOR + ".dbo." + DB_MON_OPTABLE + " set value = 'N' where option_name = '" + DB_MON_OPTION_AUTORESTART + "' ;") ;
		try{
			FDCommand1->Execute() ;
		}catch(...){}
		FDCommand1->CommandText->Clear() ;
	}

	Application->Terminate() ;
}
//---------------------------------------------------------------------------

void __fastcall TfrmAutoPilotRestart::Button2Click(TObject *Sender)
{
	hideMainForm() ;
}
//---------------------------------------------------------------------------
void __fastcall TfrmAutoPilotRestart::N2Click(TObject *Sender)
{
	recToLog(164) ;
	Application->CreateForm(__classid(TfrmLauncherOptions), &frmLauncherOptions) ;
	frmLauncherOptions->Visible = true ;
}
//---------------------------------------------------------------------------

void __fastcall TfrmAutoPilotRestart::N4Click(TObject *Sender)
{
	recToLog(165) ;
	String strVal = getOptionValue(DB_MONITOR, DB_MON_OPTABLE, DB_MON_OPTION_AUTORESTART);
	String strAutopilotShortcut = getOptionValue(DB_MONITOR, DB_MON_OPTABLE,DB_MON_OPTION_AUTOPILOTSHORTCUT);
	String strServiceShortcut = getOptionValue(DB_MONITOR, DB_MON_OPTABLE, DB_MON_OPTION_SERVICESHORTCUT);

	if ("Y" == strVal) {
	// снимаем флаг, что происходит автоматический рестарт. «ащита от рестарта с иконки на рабочем столе
	FDCommand1->CommandText->Add("update " + DB_MONITOR + ".dbo." +
		DB_MON_OPTABLE + " set value = 'N' where option_name = '" +
		DB_MON_OPTION_AUTORESTART + "' ;");
	try {
		FDCommand1->Execute();
	}
	catch (...) {}
	FDCommand1->CommandText->Clear() ;
	}
	// запускаем
	STARTUPINFO StartInfo = {sizeof(TStartupInfo)};
	PROCESS_INFORMATION ProcInfo;
	LPCTSTR s;
	StartInfo.cb = sizeof(StartInfo);
	StartInfo.dwFlags = STARTF_USESHOWWINDOW;
	StartInfo.wShowWindow = SW_SHOWNORMAL;
	String strProg = strAutopilotShortcut;
	if (!CreateProcess(NULL, strProg.w_str(), NULL, NULL, false,
		CREATE_NEW_CONSOLE | HIGH_PRIORITY_CLASS, NULL, NULL,
		&StartInfo, &ProcInfo)) {
		ShowMessage("ќшибка запуска автопилота: " + SysErrorMessage
			(GetLastError()));
		}
}
//---------------------------------------------------------------------------

void __fastcall TfrmAutoPilotRestart::timerToHideFormTimer(TObject *Sender)
{
	hideMainForm() ;
}
//---------------------------------------------------------------------------

