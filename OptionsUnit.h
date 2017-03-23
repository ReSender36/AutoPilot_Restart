//---------------------------------------------------------------------------

#ifndef OptionsUnitH
#define OptionsUnitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
//---------------------------------------------------------------------------
class TfrmLauncherOptions : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TEdit *edServerIP;
	TLabel *lblIPSrv;
	TLabel *lblNameSrv;
	TEdit *edServerName;
	TEdit *edUserName;
	TEdit *edPassword;
	TLabel *lblLoginSrv;
	TLabel *lblPasswordSrv;
	TGroupBox *GroupBox2;
	TLabel *lblAutopilotPath;
	TLabel *lblServicePath;
	TLabel *lblTimer;
	TEdit *edAutopilotPath;
	TEdit *edServicePath;
	TEdit *edTimer;
	TBitBtn *BitBtn1;
	TBitBtn *BitBtn2;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall BitBtn2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmLauncherOptions(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmLauncherOptions *frmLauncherOptions;
//---------------------------------------------------------------------------
#endif
