//---------------------------------------------------------------------------


#pragma hdrstop

#include "DMMainUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma classgroup "Vcl.Controls.TControl"
#pragma resource "*.dfm"
TdmMain *dmMain;
//---------------------------------------------------------------------------
__fastcall TdmMain::TdmMain(TComponent* Owner)
	: TDataModule(Owner)
{
}
//---------------------------------------------------------------------------
