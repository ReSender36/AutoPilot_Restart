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

const String DB_MON_CONTTABLE = "NAME_MAIL" ;
const String DB_MON_CONTACTS_NAME = "emp_login" ;
const String DB_MON_CONTACTS_ID = "row_id" ;

const String DB_MON_RESTEVTABLE = "RESTART_EVENTS" ;
const String DB_MON_RESTEV_EVENTID = "REST_ID" ;
const String DB_MON_RESTEV_RESTARTUSER = "RESTART_USER_ID" ;
const String DB_MON_RESTEV_STATE = "STATE" ;

const String DB_MON_LOGTABLE = "LOGS" ;
const String DB_MON_LOG_EVENTNUM = "event_num" ;
const String DB_MON_LOG_TRAN_DATE = "tran_date" ;

const short DB_MON_EVENT_AUTOPILOT_SHUTDOWN = 158 ;
const short DB_MON_EVENT_AUTOPILOT_STARTED = 157 ;

static int iEVENT_RESTART ;
static String strRESTART_USER ;
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
int getOpenEventRestart(){
	int iResult = 0 ;
	String str_query = String("select " + DB_MON_RESTEV_EVENTID + " from " + DB_MONITOR + ".dbo." + DB_MON_RESTEVTABLE + " where " + DB_MON_RESTEV_STATE + " = 1 ;") ;
	frmAutoPilotRestart->FDQuery1->SQL->Text = str_query ;
	frmAutoPilotRestart->FDQuery1->Active = false ;
	frmAutoPilotRestart->FDQuery1->Filtered = false ;
	frmAutoPilotRestart->FDQuery1->Active = true ;
	while(!frmAutoPilotRestart->FDQuery1->Eof){
		iResult = frmAutoPilotRestart->FDQuery1->FieldByName(DB_MON_RESTEV_EVENTID)->AsInteger ;
		frmAutoPilotRestart->FDQuery1->Next() ;
	}
	return iResult ;
}
//---------------------------------------------------------------------------
int createEventRestart(short sUserID){
	int result = 0 ;
	String str = String("insert INTO " + DB_MONITOR + ".dbo." + DB_MON_RESTEVTABLE + " (" + DB_MON_RESTEV_RESTARTUSER + ", " + DB_MON_RESTEV_STATE + ") values(" + sUserID + ", 1) ;") ;
	frmAutoPilotRestart->FDCommand1->CommandText->Clear() ;
	frmAutoPilotRestart->FDCommand1->CommandText->Add(str) ;
	frmAutoPilotRestart->FDCommand1->Execute() ;
	result = getOpenEventRestart() ;
	return result ;
}
//---------------------------------------------------------------------------
int getRestartUser(String restartUser){
	short result = 0 ;
	String com_str = String("SELECT " + DB_MON_CONTACTS_ID + " FROM " + DB_MONITOR + ".dbo." + DB_MON_CONTTABLE + " nm WHERE nm." + DB_MON_CONTACTS_NAME + " = '" + restartUser + "' ;") ;
	frmAutoPilotRestart->FDQuery1->SQL->Text = com_str ;
	frmAutoPilotRestart->FDQuery1->Active = false ;
	frmAutoPilotRestart->FDQuery1->Filtered = false ;
	frmAutoPilotRestart->FDQuery1->Active = true ;
	frmAutoPilotRestart->FDQuery1->First() ;
	while(!frmAutoPilotRestart->FDQuery1->Eof){
		result = frmAutoPilotRestart->FDQuery1->FieldByName(DB_MON_CONTACTS_ID)->AsInteger ;
		frmAutoPilotRestart->FDQuery1->Next() ;
	}
	return result ;
}
//---------------------------------------------------------------------------
void closeEventRestart(){
    strRESTART_USER = "автопилот" ;
	String strMsg = "" ;
	int iRestartUser = getRestartUser(strRESTART_USER) ;
//	iEVENT_RESTART = createEventRestart(iRestartUser) ;
	frmAutoPilotRestart->recToLog(168, strMsg,iEVENT_RESTART) ;

	String str = String("UPDATE " + DB_MONITOR + ".dbo." + DB_MON_RESTEVTABLE + " SET " + DB_MON_RESTEV_STATE + " = 2 WHERE " + DB_MON_RESTEV_EVENTID + " = " + iEVENT_RESTART + " ;") ;
	frmAutoPilotRestart->FDCommand1->CommandText->Clear() ;
	frmAutoPilotRestart->FDCommand1->CommandText->Add(str) ;
	frmAutoPilotRestart->FDCommand1->Execute() ;

	Application->Terminate() ;
}
//---------------------------------------------------------------------------
void stopRestartAndRun(){
	strRESTART_USER = "автопилот" ;
	String strMsg = "" ;
	int iRestartUser = getRestartUser(strRESTART_USER) ;
//	iEVENT_RESTART = createEventRestart(iRestartUser) ;
	frmAutoPilotRestart->recToLog(165, strMsg,iEVENT_RESTART) ;

	String strVal = getOptionValue(DB_MONITOR, DB_MON_OPTABLE, DB_MON_OPTION_AUTORESTART);
	String strAutopilotShortcut = getOptionValue(DB_MONITOR, DB_MON_OPTABLE,DB_MON_OPTION_AUTOPILOTSHORTCUT);
	String strServiceShortcut = getOptionValue(DB_MONITOR, DB_MON_OPTABLE, DB_MON_OPTION_SERVICESHORTCUT);

	if ("Y" == strVal) {
	// снимаем флаг, что происходит автоматический рестарт. «ащита от рестарта с иконки на рабочем столе
		frmAutoPilotRestart->FDCommand1->CommandText->Add("update " + DB_MONITOR + ".dbo." +
		DB_MON_OPTABLE + " set value = 'N' where option_name = '" +
		DB_MON_OPTION_AUTORESTART + "' ;");
	try {
		frmAutoPilotRestart->FDCommand1->Execute();
	}
	catch (...) {}
	frmAutoPilotRestart->FDCommand1->CommandText->Clear() ;
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
		}else
			closeEventRestart() ;
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
void stopRestart()
{
	strRESTART_USER = "автопилот" ;
	String strMsg = "" ;
	int iRestartUser = getRestartUser(strRESTART_USER) ;
//	iEVENT_RESTART = createEventRestart(iRestartUser) ;
	frmAutoPilotRestart->recToLog(166, strMsg,iEVENT_RESTART) ;

// снимаем флаг, что происходит автоматический рестарт. «ащита от рестарта с иконки на рабочем столе
	if(frmAutoPilotRestart->db_connect()){
		frmAutoPilotRestart->FDCommand1->CommandText->Add("update " + DB_MONITOR + ".dbo." + DB_MON_OPTABLE + " set value = 'N' where option_name = '" + DB_MON_OPTION_AUTORESTART + "' ;") ;
		try{
			frmAutoPilotRestart->FDCommand1->Execute() ;
		}catch(...){}
		frmAutoPilotRestart->FDCommand1->CommandText->Clear() ;
	}
    closeEventRestart() ;
	Application->Terminate() ;
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
			if(db_connect()){
				strRESTART_USER = "автопилот" ;
				String strMsg = "" ;
				int iRestartUser = getRestartUser(strRESTART_USER) ;
				iEVENT_RESTART = createEventRestart(iRestartUser) ;
				recToLog(161,strMsg,iEVENT_RESTART) ;

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
			}else{
				if("-off" == PARAM)
					stopRestart() ;
				else{
					if("-or" == PARAM)
						stopRestartAndRun() ;
				}
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
		if(db_connect()){
			strRESTART_USER = "автопилот" ;
			String strMsg = "" ;
			int iRestartUser = getRestartUser(strRESTART_USER) ;
			iEVENT_RESTART = createEventRestart(iRestartUser) ;
			recToLog(161,strMsg,iEVENT_RESTART) ;

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
	bool restartComplete ;
	strRESTART_USER = "автопилот" ;
	String strMsg = "" ;
	int iRestartUser = getRestartUser(strRESTART_USER) ;
//	iEVENT_RESTART = createEventRestart(iRestartUser) ;
	recToLog(163,strMsg,iEVENT_RESTART) ;

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
			}else
				restartComplete = true ;

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
		strRESTART_USER = "автопилот" ;
		String strMsg = "" ;
		int iRestartUser = getRestartUser(strRESTART_USER) ;
//		iEVENT_RESTART = createEventRestart(iRestartUser) ;
		recToLog(167,strMsg,iEVENT_RESTART) ;

		TimerToRestart->Enabled = false ;
		int iInterval = StrToInt(getOptionValue(DB_MONITOR, DB_MON_OPTABLE, DB_MON_OPTION_TIMERRESTARTREPEAT)) ;
		TimerToRestart->Interval = iInterval ;
		TimerToRestart->Enabled = true ;

	}
	// Edit1->Text = getOptionValue(DB_MONITOR, DB_MON_OPTABLE, DB_MON_OPTION_TIMERRESTARTDELAY) ;
	// Edit1->Text = "Ќужен интервал 300000 миллисекунд" ;
	if (restartComplete) {
		timerToCloseEvent->Enabled = true ;
		TimerToRestart->Enabled = false;
//		Application->Terminate();
	}
	
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
//	recToLog(166) ;
// снимаем флаг, что происходит автоматический рестарт. «ащита от рестарта с иконки на рабочем столе
//	if(db_connect()){
//		FDCommand1->CommandText->Add("update " + DB_MONITOR + ".dbo." + DB_MON_OPTABLE + " set value = 'N' where option_name = '" + DB_MON_OPTION_AUTORESTART + "' ;") ;
//		try{
//			FDCommand1->Execute() ;
//		}catch(...){}
//		FDCommand1->CommandText->Clear() ;
//	}

//	Application->Terminate() ;
	stopRestart() ;
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
//	recToLog(165) ;
//	String strVal = getOptionValue(DB_MONITOR, DB_MON_OPTABLE, DB_MON_OPTION_AUTORESTART);
//	String strAutopilotShortcut = getOptionValue(DB_MONITOR, DB_MON_OPTABLE,DB_MON_OPTION_AUTOPILOTSHORTCUT);
//	String strServiceShortcut = getOptionValue(DB_MONITOR, DB_MON_OPTABLE, DB_MON_OPTION_SERVICESHORTCUT);

//	if ("Y" == strVal) {
	// снимаем флаг, что происходит автоматический рестарт. «ащита от рестарта с иконки на рабочем столе
//		FDCommand1->CommandText->Add("update " + DB_MONITOR + ".dbo." +
//		DB_MON_OPTABLE + " set value = 'N' where option_name = '" +
//		DB_MON_OPTION_AUTORESTART + "' ;");
//	try {
//		FDCommand1->Execute();
//	}
//	catch (...) {}
//	FDCommand1->CommandText->Clear() ;
//	}
	// запускаем
//	STARTUPINFO StartInfo = {sizeof(TStartupInfo)};
//	PROCESS_INFORMATION ProcInfo;
//	LPCTSTR s;
//	StartInfo.cb = sizeof(StartInfo);
//	StartInfo.dwFlags = STARTF_USESHOWWINDOW;
//	StartInfo.wShowWindow = SW_SHOWNORMAL;
//	String strProg = strAutopilotShortcut;
//	if (!CreateProcess(NULL, strProg.w_str(), NULL, NULL, false,
//		CREATE_NEW_CONSOLE | HIGH_PRIORITY_CLASS, NULL, NULL,
//		&StartInfo, &ProcInfo)) {
//		ShowMessage("ќшибка запуска автопилота: " + SysErrorMessage
//			(GetLastError()));
//		}
	stopRestartAndRun() ;
}
//---------------------------------------------------------------------------

void __fastcall TfrmAutoPilotRestart::timerToHideFormTimer(TObject *Sender)
{
	hideMainForm() ;
}
//---------------------------------------------------------------------------

void __fastcall TfrmAutoPilotRestart::timerToCloseEventTimer(TObject *Sender)
{
	closeEventRestart() ;
}
//---------------------------------------------------------------------------

