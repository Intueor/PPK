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
MainWindow::MainWindow(QWidget* p_WidgetParent) : QMainWindow(p_WidgetParent), p_UI(new Ui::MainWindow)
{
	p_UI->setupUi(this);
	up_Logger = make_unique<Logger>("./", "main");
	ST("Загрузка настроек...");
	p_UI->statusbar->addPermanentWidget(&oLabelStatus);
	sp_Settings = make_shared<QSettings>(SETTINGS_NAME, QSettings::IniFormat);
	up_WidgetsSerializer = make_unique<WidgetsSerializer>(sp_Settings);
	// Загрузка настроек.
	up_WidgetsSerializer->RegisterChildren(*this);
	up_WidgetsSerializer->RegisterChildren(oDialogSettings);
	if(sp_Settings->allKeys().count() == 0)
	{
		Log(up_Logger, LogCat::W, "Файл настроек не обнаружен, загрузка по умолчанию");
		SM("Настройки не загружены, будут созданы новые.");
	}
	else
	{
		up_WidgetsSerializer->LoadStates(*this);
		up_WidgetsSerializer->LoadStates(oDialogSettings, false);
		up_Logger->ChangeLogLevel(sp_Settings->value("DialogSettings-spinBoxLogLevel-V").toInt());
		LogS(up_Logger, LogCat::I, "Уровень логирования: " << up_Logger->LogLevel(), 1);
		Log(up_Logger, LogCat::I, "Файл настроек загружен и применён.", 2);
		SM("Настройки загружены.");
	}
	ST("Готов.");
}

// Деструктор.
MainWindow::~MainWindow()
{
	up_WidgetsSerializer->SaveStates(*this);
	up_WidgetsSerializer->SaveStates(oDialogSettings, false);
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
void MainWindow::on_actionAbout_triggered() { oDialogAbout.exec(); }

// Обработка пункта "Настройки".
void MainWindow::on_actionSettings_triggered() { CancelableDialogExec(oDialogSettings); }

