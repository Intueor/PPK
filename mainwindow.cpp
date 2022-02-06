//== ВКЛЮЧЕНИЯ.
#include "mainwindow.h"
#include "ui_mainwindow.h"

//== ПРОСТРАНСТВА ИМЁН.
using namespace rapidxml;
using namespace std;

//== ФУНКЦИИ КЛАССОВ.
//== Класс главного окна.
// Конструктор.
MainWindow::MainWindow(QWidget* p_Parent) : QMainWindow(p_Parent), up_UI(new Ui::MainWindow)
{
	up_Logger = make_unique<Logger>("./", "main");
	up_UI->setupUi(this);
	up_QSettings = make_unique<QSettings>(SETTINGS_NAME, QSettings::IniFormat);
	if(up_QSettings->allKeys().count() == 0)
	{
		Log(up_Logger, LogCat::W, "Файл настроек не обнаружен, загрузка по умолчанию");
	}
	else
	{
		Log(up_Logger, LogCat::I, "Файл настроек загружен", 3);
	}
}

// Деструктор.
MainWindow::~MainWindow()
{
	up_QSettings->setValue("LogLevel", up_Logger->LogLevel());
	up_QSettings->sync();
	QSettings::Status eStatus = up_QSettings->status();
	switch(eStatus)
	{
		case QSettings::NoError:
			break;
		case QSettings::AccessError:
			Log(up_Logger, LogCat::E, "Ошибка доступа к файлу настроек");
			break;
		case QSettings::FormatError:
			Log(up_Logger, LogCat::E, "Ошибка формата файла настроек");
			break;
	}
}
