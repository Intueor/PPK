#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//== ВКЛЮЧЕНИЯ.
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "logger.h"
#include "utilites.h"
#include "widgets-serializer.h"
#include "dialogs/dialog-about.h"
#include "dialogs/dialog-settings.h"
#include "dialogs/dialog-message.h"
#include "mtableview.h"
#include <QMainWindow>
#include <QSettings>
#include <QLabel>
#include <QSqlDatabase>
#include <QFile>
#include <QSqlRelationalTableModel>

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
	/// Стрктура, описывающая отношение столбцов таблицы к столбцам других.
	struct ColumnRelation
	{
		int iColumn; ///< Номер столбца текущей таблицы.
		const QSqlRelation& r_QSqlRelation; ///< Описание отношения к другой.
	};

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
	QString strDBName; ///< Имя текущей базы данных.
	QString strDBPath; ///< Путь к базе данных.
	const QString strDBTempExt = ".temp"; ///< Расширение временного файла базы данных.
	QString strDBTempPath; ///< Путь к временному файлу базы данных.
	QFile oDbFile; ///< Интерфейс для работы с файлом базы данных.
	QSqlDatabase oDB; ///< Интерфейс базы данных.

private:
	/// Приминение значений из диалога настроек.
	void ApplySettingsDialogValues();
	/// Инициализация вида и модели таблицы.
	QSqlRelationalTableModel* InitTable(QObject* p_Parent, const QString& r_strTableName, MTableView* p_MTableView, const QString& r_strFilter = "", bool bSortEnabled = false, int iColumnForSort = 0,
										std::vector<MTableView*>* p_v_p_InfluencingTableViews = nullptr, std::vector<ColumnRelation>* p_vColumnsRelations = nullptr);
													///< \param[in] p_Parent Указатель на родительский объект.
													///< \param[in] r_strTableName Ссылка на конст. строку с именем таблицы.
													///< \param[in] p_MTableView Указатель на виджет вида таблицы.
													///< \param[in] r_strFilter Ссылка на конст. строку с фильтром.
													///< \param[in] bCanScroll При false - расширение вертикального разм. таблицы под контент.
													///< \param[in] iColumnForSort 0 - для отсутствия сортировки или номер колонки.
													///< \param[in] p_v_p_InfluencingTableViews Указатель на вектор указателей на виджеты вида таблиц, которые могут менять контент текущего виджета вида.
													///< \param[in] p_vColumnsRelations Указатель на вектор стрктур, описывающих отношение столбцов таблицы к столбцам других.
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
