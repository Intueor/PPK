//== ВКЛЮЧЕНИЯ.
#include "mainwindow.h"
#include <QApplication>

//== ФУНКЦИИ.
// Точка входа в приложение.
int main(int iArgc, char* p_mArgv[])
{
#ifdef WIN32
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
#endif
	QApplication oApp(iArgc, p_mArgv);
	MainWindow oMW;
	oMW.show();
	return oApp.exec();
}
