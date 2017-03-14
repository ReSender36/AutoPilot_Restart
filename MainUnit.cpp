//---------------------------------------------------------------------------

#include <vcl.h>
#include <windows.h>
#include <stdio.h>
#include <shellapi.h>
#pragma hdrstop

#include "MainUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
String PARAM = "" ;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
bool db_connect()
{
	try{
		if (!Form1->FDConnection1->Connected){
			Form1->FDConnection1->Connected = true ;
				if (Form1->FDConnection1->Connected)
					return true ;
		}
	}
	catch(...){
		int q_conn = Application->MessageBox(String("БД не найдена по указанному пути. Отредактируйте файл INI").w_str(),String("Проблема").w_str(),MB_YESNO + MB_ICONQUESTION) ;
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
void __fastcall TForm1::FormCreate(TObject *Sender)
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


	if("-r" == PARAM)



// Free memory allocated for CommandLineToArgvW arguments.
   LocalFree(szArglist);
   return;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
	String val = "" ;
	if("-r" == PARAM){
		if(db_connect()){
			FDQuery1->SQL->Text = String("select value from DB_1SMonitor.dbo.options where option_name = 'AutopilotRestart'") ;
			FDQuery1->Active = true ;

			FDQuery1->First() ;
			while(!FDQuery1->Eof){
				val = FDQuery1->FieldByName("value")->AsString ;
				FDQuery1->Next() ;
			}
		}
	}
	Edit1->Text = val ;

	FDCommand1->CommandText->Add("update DB_1SMonitor.dbo.options set value = 'No' where option_name = 'AutopilotRestart' ;") ;
	FDCommand1->Execute() ;

	TimerToRestart->Enabled = true ;

}
//---------------------------------------------------------------------------
void __fastcall TForm1::TimerToRestartTimer(TObject *Sender)
{
		Edit1->Text = "Нужен интервал 300000 миллисекунд" ;
		Application->Terminate() ;
}
//---------------------------------------------------------------------------

