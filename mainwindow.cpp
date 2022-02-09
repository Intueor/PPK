//== ВКЛЮЧЕНИЯ.
#include <QSqlError>

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
	p_Settings = new QSettings(SETTINGS_NAME, QSettings::IniFormat);
	up_Logger = make_unique<Logger>("./", "main");
	p_UI->statusbar->addPermanentWidget(&oLabelStatus);
	up_WidgetsSerializer = make_unique<WidgetsSerializer>(*p_Settings);
	// Загрузка настроек.
	up_WidgetsSerializer->RegisterChildren(*this);
	up_WidgetsSerializer->RegisterChildren(oDialogSettings);
	if(p_Settings->allKeys().count() == 0)
	{
		Log(up_Logger, LogCat::W, "Файл настроек не обнаружен, загрузка значений по умолчанию");
		SM("Настройки не загружены, будут созданы новые.");
	}
	else
	{
		up_WidgetsSerializer->LoadStates(*this);
		up_WidgetsSerializer->LoadStates(oDialogSettings, false);
		up_Logger->ChangeLogLevel(p_Settings->value("DialogSettings-spinBoxLogLevel-V").toInt());
		LogS(up_Logger, LogCat::I, "Уровень логирования: " << up_Logger->LogLevel(), 1);
		Log(up_Logger, LogCat::I, "Файл настроек загружен и применён.", 2);
		SM("Настройки загружены.");
	}
	oDB = QSqlDatabase::addDatabase("QSQLITE");
	oDB.setHostName("localhost");
	oDB.setDatabaseName("./default.db");
	if(!oDB.open())
	{
		LogS(up_Logger, LogCat::E, "Ошибка инициализации интерфейса базы данных: " << oDB.lastError().databaseText().toStdString());
		oDialogMessage.exec(MsgCat::E, "Сбой подключения к базе данных.<br>Приложение будет закрыто.");
		exit(EXIT_FAILURE);
	}
	ST("Готов.");
}

// Деструктор.
MainWindow::~MainWindow()
{
	ST("Отключение от базы данных...");
	oDB.close();
	up_WidgetsSerializer->SaveStates(*this);
	up_WidgetsSerializer->SaveStates(oDialogSettings, false);
	ST("Сохранение настроек...");
	p_Settings->sync();
	QString strMsg;
	switch(p_Settings->status())
	{
		case QSettings::NoError:
			Log(up_Logger, LogCat::I, "Настройки сохранены в файл.", 2);
			break;
		case QSettings::AccessError:
			strMsg = "Ошибка доступа к файлу настроек.";
			Log(up_Logger, LogCat::E, strMsg.toStdString());
			oDialogMessage.exec(MsgCat::I, strMsg);
			break;
		case QSettings::FormatError:
			strMsg = "Ошибка формата файла настроек.";
			Log(up_Logger, LogCat::E, strMsg.toStdString());
			oDialogMessage.exec(MsgCat::I, strMsg);
			break;
	}
	Log(up_Logger, LogCat::I, "Завершение работы.", 2);
	ST("Выход...");
	delete p_Settings;
	delete p_UI;
}

// Обработка пункта "О программе".
void MainWindow::on_actionAbout_triggered() { oDialogAbout.exec(); }

// Обработка пункта "Настройки".
void MainWindow::on_actionSettings_triggered() { CancelableDialogExec(oDialogSettings); }

