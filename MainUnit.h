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
	TFDGUIxWaitCursor *FDGUIxWaitCursor1;
	TFDCommand *FDCommand1;
	TFDQuery *FDQuery1;
	TTrayIcon *TrayIcon1;
	TPopupMenu *PopupMenu1;
	TMenuItem *N2;
	TMenuItem *N3;
	TButton *Button2;
	TMenuItem *N4;
	TFDConnection *FDConnection1;
	TTimer *timerToHideForm;
	TTimer *timerToCloseEvent;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall TimerToRestartTimer(TObject *Sender);
	void __fastcall TrayIcon1DblClick(TObject *Sender);
	void __fastcall N1Click(TObject *Sender);
	void __fastcall N3Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall N2Click(TObject *Sender);
	void __fastcall N4Click(TObject *Sender);
	void __fastcall timerToHideFormTimer(TObject *Sender);
	void __fastcall timerToCloseEventTimer(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmAutoPilotRestart(TComponent* Owner);
//---------------------------------------------------------------------------
bool db_connect()
{
	bool state ;
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
				state = true ;
		}
		catch(...){
			int q_conn = Application->MessageBox(String("Проблемы при подключении к БД " + SysErrorMessage(GetLastError())).w_str(),String("Проблема").w_str(),MB_OK) ;
			switch(q_conn){
				case IDYES:
					break ;
				case IDNO:
					break ;
			}
			state = false ;
		}
	}else{
    	state = true ;
	}

	return state ;
}
//---------------------------------------------------------------------------
bool db_disconnect()
{
	bool state ;
	try{
		if(FDConnection1->Connected){
			FDConnection1->Connected = false ;
			state = true ;
		}
	}
	catch(...){
		state = false ;
	}
	return state ;
}
//---------------------------------------------------------------------------
void recToDB(String strEventNum, String strMessage, int iEventNum){
	String strRestEvent = IntToStr(iEventNum) ;
	String strComm = String("insert into DB1S_monitor.dbo.logs (tran_date, doc_num, message_txt, event_num, p_filename, p_ArchFilename, Procedure_name, doc_type, filename, directions, interface, doc_date, bailor, doc_state, performer, RESTART_EVENT) \
	 values(current_timestamp, '', '" + strMessage + "','" + strEventNum + "','','','AutopilotRestart','','',0,'','','','','space', " + strRestEvent + ") ;") ;
	FDCommand1->CommandText->Clear() ;
	FDCommand1->CommandText->Add(strComm) ;
	try{
		FDCommand1->Execute() ;
		FDCommand1->CommandText->Clear() ;
	}catch(...){}
}
//---------------------------------------------------------------------------
String getEventMsg(String strEventNum){
	String strRes = "" ;

	String strQ = String("SELECT caption FROM DB1S_monitor.dbo.sys_events where kind = " + strEventNum + " ;") ;
	FDQuery1->Active = false ;
	FDQuery1->SQL->Text = strQ ;
	FDQuery1->Active = true ;
	FDQuery1->First() ;
	while(!FDQuery1->Eof){
		strRes = FDQuery1->FieldByName("caption")->AsString ;
		FDQuery1->Next() ;
	}
	return strRes ;
}
//---------------------------------------------------------------------------
void recToLog(short shEventNum, String strMsg = "", int iRestart_Event = 0){
	if(db_connect()){
		String strEventNum = IntToStr(shEventNum) ;
		String strMessage = getEventMsg(strEventNum) ;
		strMessage = strMessage + " " + strMsg ;
		recToDB(strEventNum, strMessage, iRestart_Event) ;
	}
 //	db_disconnect() ;
}
//---------------------------------------------------------------------------
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmAutoPilotRestart *frmAutoPilotRestart;
//---------------------------------------------------------------------------
#endif
