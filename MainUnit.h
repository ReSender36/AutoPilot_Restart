//---------------------------------------------------------------------------

#ifndef MainUnitH
#define MainUnitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Data.DB.hpp>
#include <FireDAC.Comp.Client.hpp>
#include <FireDAC.Comp.DataSet.hpp>
#include <FireDAC.Comp.UI.hpp>
#include <FireDAC.DApt.hpp>
#include <FireDAC.DApt.Intf.hpp>
#include <FireDAC.DatS.hpp>
#include <FireDAC.Phys.hpp>
#include <FireDAC.Phys.Intf.hpp>
#include <FireDAC.Phys.MSSQL.hpp>
#include <FireDAC.Phys.MSSQLDef.hpp>
#include <FireDAC.Phys.ODBCBase.hpp>
#include <FireDAC.Stan.Async.hpp>
#include <FireDAC.Stan.Def.hpp>
#include <FireDAC.Stan.Error.hpp>
#include <FireDAC.Stan.Intf.hpp>
#include <FireDAC.Stan.Option.hpp>
#include <FireDAC.Stan.Param.hpp>
#include <FireDAC.Stan.Pool.hpp>
#include <FireDAC.UI.Intf.hpp>
#include <FireDAC.VCLUI.Wait.hpp>
#include <Vcl.Menus.hpp>
//---------------------------------------------------------------------------
class TfrmAutoPilotRestart : public TForm
{
__published:	// IDE-managed Components
	TEdit *Edit1;
	TButton *Button1;
	TTimer *TimerToRestart;
	TFDPhysMSSQLDriverLink *FDPhysMSSQLDriverLink1;
	TFDConnection *FDConnection1;
	TFDGUIxWaitCursor *FDGUIxWaitCursor1;
	TFDCommand *FDCommand1;
	TFDQuery *FDQuery1;
	TTrayIcon *TrayIcon1;
	TPopupMenu *PopupMenu1;
	TMenuItem *N1;
	TMenuItem *N2;
	TMenuItem *N3;
	TButton *Button2;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall TimerToRestartTimer(TObject *Sender);
	void __fastcall TrayIcon1DblClick(TObject *Sender);
	void __fastcall N1Click(TObject *Sender);
	void __fastcall N3Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmAutoPilotRestart(TComponent* Owner);
//---------------------------------------------------------------------------
bool db_connect()
{
	TIniFile *ini ;
	ini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini")) ;
 /*	if ("" == ini){
		Application->MessageBoxW(String("Не найден файл с настройками в каталоге с программой " + Application->ExeName).w_str(),,String("Проблема").w_str(),MB_OK) ;
	}
 */
	String Address = ini->ReadString("DB_Param", "Address", 1) ;
	String Server = ini->ReadString("DB_Param", "Server", 1) ;
	String UserName = ini->ReadString("DB_Param", "UserName", 1) ;
	String Password = ini->ReadString("DB_Param", "Password", 1) ;
	ini->Free() ;


	if (!FDConnection1->Connected){
		FDConnection1->Params->Add("Address=" + Address) ;
		FDConnection1->Params->Add("Server=" + Server) ;
		FDConnection1->Params->Add("UserName=" + UserName) ;
		FDConnection1->Params->Add("Password=" + Password) ;
		try{
			FDConnection1->Connected = true ;
			if (FDConnection1->Connected)
				return true ;
		}
		catch(...){
			int q_conn = Application->MessageBox(String("Проблемы при подключении к БД " + SysErrorMessage(GetLastError())).w_str(),String("Проблема").w_str(),MB_OK) ;
			switch(q_conn){
				case IDYES:
					break ;
				case IDNO:
					break ;
			}
			return false ;
		}
	}
}
//---------------------------------------------------------------------------
bool db_disconnect()
{
	try{
		if(FDConnection1->Connected){
			FDConnection1->Connected = false ;
			return true ;
		}
	}
	catch(...){
		return false ;
	}
}
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmAutoPilotRestart *frmAutoPilotRestart;
//---------------------------------------------------------------------------
#endif
