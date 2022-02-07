//== ВКЛЮЧЕНИЯ.
#include "mainwindow.h"
#include "ui_mainwindow.h"

//== МАКРОСЫ.
#define STATUS_DELAY	3000
#define ST(m)			oLabelStatus.setText(m)
#define SM(m)			p_UI->statusbar->showMessage(m, STATUS_DELAY)

//== ПРОСТРАНСТВА ИМЁН.
using namespace rapidxml;
using namespace std;

//== ФУНКЦИИ КЛАССОВ.
//== Класс главного окна.
// Конструктор.
MainWindow::MainWindow(QWidget* p_Parent) : QMainWindow(p_Parent), p_UI(new Ui::MainWindow)
{
	p_UI->setupUi(this);
	up_Logger = make_unique<Logger>("./", "main");
	ST("Загрузка настроек...");
	p_UI->statusbar->addPermanentWidget(&oLabelStatus);
	sp_Settings = make_shared<QSettings>(SETTINGS_NAME, QSettings::IniFormat);
	up_WidgetSerializer = make_unique<WidgetSerializer>(sp_Settings);
	up_WidgetSerializer->RegisterChildren(this);
	if(sp_Settings->allKeys().count() == 0)
	{
		Log(up_Logger, LogCat::W, "Файл настроек не обнаружен, загрузка по умолчанию");
		SM("Настройки не загружены, будут созданы новые.");
	}
	else
	{
		up_Logger->ChangeLogLevel(sp_Settings->value("LogLevel").toInt());
		LogS(up_Logger, LogCat::I, "Уровень логирования: " << up_Logger->LogLevel(), 1);
		up_WidgetSerializer->LoadStates(this);
		Log(up_Logger, LogCat::I, "Файл настроек загружен и применён.", 2);
		SM("Настройки загружены.");
	}
	ST("Готов.");
}

// Деструктор.
MainWindow::~MainWindow()
{
	sp_Settings->setValue("LogLevel", up_Logger->LogLevel());
	up_WidgetSerializer->SaveStates(this);
	ST("Сохранение настроек...");
	sp_Settings->sync();
	switch(sp_Settings->status())
	{
		case QSettings::NoError:
			Log(up_Logger, LogCat::I, "Настройки сохранены в файл.", 2);
			Log(up_Logger, LogCat::I, "Завершение работы.", 2);
			break;
		case QSettings::AccessError:
			Log(up_Logger, LogCat::E, "Ошибка доступа к файлу настроек.");
			break;
		case QSettings::FormatError:
			Log(up_Logger, LogCat::E, "Ошибка формата файла настроек.");
			break;
	}
	SM("Настройки сохранены.");
	ST("Выход...");
	delete p_UI;
}

// Обработка пункта "О программе".
void MainWindow::on_actionAbout_triggered()
{

}

