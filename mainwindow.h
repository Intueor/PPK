#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//== ВКЛЮЧЕНИЯ.
#include <QMainWindow>
#include <QSettings>
#include <QLabel>
#include <QSqlDatabase>
#include <memory>

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "logger.h"
#include "utilites.h"
#include "widgets-serializer.h"
#include "dialogs/dialog-about.h"
#include "dialogs/dialog-settings.h"
#include "dialogs/dialog-message.h"

//== МАКРОСЫ.
//! \file mainwindow.h
#define SETTINGS_NAME	"settings.ini" ///< Имя файла настроек.

//== ПРОСТРАНСТВА ИМЁН.
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//== КЛАССЫ.
/// Класс главного окна.
class MainWindow : public QMainWindow
{
	Q_OBJECT

private:
	Ui::MainWindow* p_UI; ///< Указатель на объект пользовательского интерфейса.
	std::unique_ptr<rapidxml::xml_document<>> up_docDB; ///< Указатель на документ базы данных.
	QSettings* p_Settings; ///< Указатель на установки приложения.
	std::unique_ptr<Logger> up_Logger; ///< Указатель на логгер.
	std::unique_ptr<WidgetsSerializer> up_WidgetsSerializer; ///< Указатель на сериализатор виджетов.
	QLabel oLabelStatus; ///< Метка статуса.
	DialogAbout oDialogAbout; ///< Диалог "О программе".
	DialogSettings oDialogSettings; ///< Диалог "Настройки".
	DialogMessage oDialogMessage; ///< Диалог "Сообщение".
	QSqlDatabase oDB; ///< Интерфейс базы данных.

public:
	/// Конструктор.
	MainWindow(QWidget* p_WidgetParent = nullptr);
										///< \param[in] p_WidgetParent Указатель на родительский виджет.
	/// Деструктор.
	~MainWindow();

private slots:
	/// Обработка пункта "О программе".
	void on_actionAbout_triggered();

	/// Обработка пункта "Настройки".
	void on_actionSettings_triggered();
};
#endif // MAINWINDOW_H
