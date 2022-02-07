#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//== ВКЛЮЧЕНИЯ.
#include <QMainWindow>
#include <QSettings>
#include <memory>

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "logger.h"
#include "widget-serializer.h"

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
	std::unique_ptr<Ui::MainWindow> up_UI; ///< Указатель на объект пользовательского интерфейса.
	std::unique_ptr<rapidxml::xml_document<>> up_docDB; ///< Указатель на документ базы данных.
	std::shared_ptr<QSettings> sp_QSettings; ///< Указатель на установки приложения.
	std::unique_ptr<Logger> up_Logger; ///< Указатель на логгер.
	std::unique_ptr<WidgetSerializer> up_WidgetSerializer; ///< Указатель на сериализатор виджетов.

public:
	/// Конструктор.
	MainWindow(QWidget* p_Parent = nullptr);
										///< \param[in] p_Parent Указатель на родительский виджет.
	/// Деструктор.
	~MainWindow();
};
#endif // MAINWINDOW_H
