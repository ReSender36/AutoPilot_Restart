#include <vcl.h>
#include <stdio.h>
#include <winbase.h>
#include <shellapi.h>
//---------------------------------------------------------------------------
#pragma hdrstop

#include "runCalc.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
//	HWND hserv ;
//	HINSTANCE res ;
//	res = ShellExecuteA(hserv, "open", "Calc",NULL, NULL, SW_RESTORE) ;
//	ShowMessage((int)res) ;
	AnsiString command = Edit1->Text ;
//	WinExec(command.c_str(), SW_RESTORE) ;

	STARTUPINFO StartInfo = { sizeof(TStartupInfo) } ;
	PROCESS_INFORMATION ProcInfo ;
	LPCTSTR s ;
	StartInfo.cb = sizeof(StartInfo) ;
	StartInfo.dwFlags = STARTF_USESHOWWINDOW ;
	StartInfo.wShowWindow = SW_SHOWNORMAL ;
	WideString strProg = "Calc.exe" ;
	if(!CreateProcess(NULL, strProg.c_bstr(),NULL,NULL,false,
		CREATE_NEW_CONSOLE|HIGH_PRIORITY_CLASS,NULL,NULL,&StartInfo,&ProcInfo))
		ShowMessage("Ошибка: " + SysErrorMessage(GetLastError())) ;
	else{
		if(WaitForSingleObject(ProcInfo.hProcess,10000) == WAIT_TIMEOUT)
			ShowMessage("Процесс пока идет") ;
		CloseHandle(ProcInfo.hProcess) ;
	}

	String str = String("SELECT TIME_TO_RUN FROM SM_TST.OPTIONS") ;
	FDQuery1->SQL->Text = str ;
	FDQuery1->Active = false ;
	FDQuery1->Active = true ;
	Edit1->Text = FDQuery1->FieldByName("TIME_TO_RUN")->AsString ;

	str = String("UPDATE SM_TST.OPTIONS SET TIME_TO_RUN = TRUE") ;
	FDCommand1->CommandText->Add(str) ;
	FDCommand1->Execute() ;
	
	FILE *fl ;
//	AnsiString sFileName = ChangeFileExt(Vcl::Forms::Application->ExeName,".log") ;
	AnsiString sFileName = "C:\\123.log" ;
	if(fl = fopen(sFileName.c_str(), "a+")){
		AnsiString write_str = "галка установлена" ;
		try{
			fprintf(fl, "%s \n", write_str) ;
			fclose(fl) ;
//			return true ;
		}
		catch(...){
//			return false ;
		}
	}

}
//---------------------------------------------------------------------------
