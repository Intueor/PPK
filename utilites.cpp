//== ВКЛЮЧЕНИЯ.
#include "utilites.h"
#include "widgets-serializer.h"

//== ПРОСТРАНСТВА ИМЁН.
using namespace std;

//== ФУНКЦИИ.
// Исполнение диалога с возможностью отмены результатов.
bool CancelableDialogExec(QDialog& r_Dialog)
{
	QSettings oSettings("Temp");
	WidgetsSerializer oWidgetsSerializerTemp(oSettings);
	oWidgetsSerializerTemp.RegisterChildren(r_Dialog);
	oWidgetsSerializerTemp.SaveStates(r_Dialog, false);
	if(r_Dialog.exec() == QDialog::Rejected) { oWidgetsSerializerTemp.LoadStates(r_Dialog, false); return false; }
	else return true;
}
