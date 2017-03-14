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
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
   LPWSTR *szArglist;
   int nArgs;
   int i;

   String param[3] = "" ;

   szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
   if( NULL == szArglist )
   {
	  wprintf(L"CommandLineToArgvW failed\n");
	  return ;
   }
   else
	for( i=0; i<nArgs; i++){
		Edit1->Text = szArglist[i] ; //printf("%d: %ws\n", i, szArglist[i]);
		param = szArglist[i] ;
	}


	if("-r" == param)
		Edit1->Text = "123" ;
// Free memory allocated for CommandLineToArgvW arguments.

   LocalFree(szArglist);

   return;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
	if("-r" == Edit1->Text)
		Edit1->Text = "132" ;
}
//---------------------------------------------------------------------------
