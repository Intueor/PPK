//== ВКЛЮЧЕНИЯ.
#include "mainwindow.h"
#include <QApplication>

//== ФУНКЦИИ.
// Точка входа в приложение.
int main(int iArgc, char* p_mArgv[])
{
	QApplication oApp(iArgc, p_mArgv);
	MainWindow oMW;
	oMW.show();
	return oApp.exec();
}
