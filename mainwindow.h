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
#include <QMenu>
#include <QMouseEvent>

//== МАКРОСЫ.
//! \file mainwindow.h
#define SETTINGS_NAME	"settings.ini" ///< Имя файла настроек.
#define STATUS_SHOW_TIME_MS	3000 ///< Время демонстрации статуса.

//== ПРОСТРАНСТВА ИМЁН.
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//== КЛАССЫ.
/// Класс модифицированного меню.
class SafeMenu : public QMenu
{
	Q_OBJECT

public:
	/// Конструктор.
	SafeMenu(QWidget* p_Parent = nullptr)
													///< \param[in] p_Parent Указатель на родительский объект.
									: QMenu(p_Parent)
	{}

public slots:
	/// Переопределение функции обработки нажатия мыши.
	void mousePressEvent(QMouseEvent* p_Event);
													///< \param[in] p_Event Указатель на событие.
	/// Переопределение функции обработки перемещения мыши.
	void mouseMoveEvent(QMouseEvent* p_Event);
													///< \param[in] p_Event Указатель на событие.
	/// Переопределение функции обработки отпускания мыши.
	void mouseReleaseEvent(QMouseEvent* p_Event);
													///< \param[in] p_Event Указатель на событие.
};

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
	Ui::MainWindow* p_UI = nullptr; ///< Указатель на объект пользовательского интерфейса.
	std::unique_ptr<rapidxml::xml_document<>> up_docDB; ///< Указатель на документ базы данных.
	QSettings* p_Settings = nullptr; ///< Указатель на установки приложения.
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
	const std::map<uchar, MSqlRelationalDelegate::CustomDelegateType> mpSchedColsDataTypes = {{1, MSqlRelationalDelegate::CustomDelegateType::FormattedTime}}; ///< Типы данных делегата сетки занятий.
	const std::map<uchar, MSqlRelationalDelegate::CustomDelegateType> mpLenColsDataTypes = {{1, MSqlRelationalDelegate::CustomDelegateType::PositiveNotNull}}; ///< Типы данных делегата длин занятий.
	const std::map<uchar, QString> mpTimeTableColsFilters = {{2, "Время != ''"}, {3, "Имя != ''"}, {4, "Предмет != ''"}, {6, "Длина > 0"}}; ///< Фильтры всех делегатов расписания.
	SafeMenu oMenuNewLesson; ///< Объект меню нового урока.

private:
	/// Приминение значений из диалога настроек.
	void ApplySettingsDialogValues();
	/// Инициализация вида и модели таблицы.
	QSqlRelationalTableModel* InitMTable(QObject* p_Parent, const QString& r_strTableName, MTableView* p_MTableView, const QString& r_strFilter = "", bool bCanScroll = true, int iColumnForSort = 0,
										 bool bStretchLastHSection = false, const std::map<uchar, MSqlRelationalDelegate::CustomDelegateType>* const p_mpColumnsDataTypes = nullptr,
										 const std::map<uchar, QString>* const p_mpColumnsFilters = nullptr,
										 std::vector<MTableView*>* const p_v_p_InfluencingTableViews = nullptr, std::vector<ColumnRelation>* const p_vColumnsRelations = nullptr,
										 std::vector<MHorizontalHeaderView*>* const p_v_p_MHorizontalHeaderViewsRelated = nullptr);
													///< \param[in] p_Parent Указатель на родительский объект.
													///< \param[in] r_strTableName Ссылка на конст. строку с именем таблицы.
													///< \param[in] p_MTableView Указатель на виджет вида таблицы.
													///< \param[in] r_strFilter Ссылка на конст. строку с фильтром.
													///< \param[in] bCanScroll При false - расширение вертикального разм. таблицы под контент.
													///< \param[in] iColumnForSort 0 - для отсутствия сортировки или номер колонки.
													///< \param[in] bStretchLastHSection true - для переопределённого растяжения последней видимой секции горизонтального заголовка.
													///< \param[in] p_mpColumnsDataTypes Константный указатель на константную карту 'колонка в тип данных' или nullptr.
													///< \param[in] p_mpColumnsFilters Константный указатель на константную карту 'колонка в фильтр'.
													///< \param[in] p_v_p_InfluencingTableViews Константный ук. на вектор указателей на виджеты вида таблиц, которые могут менять контент текущего виджета вида.
													///< \param[in] p_vColumnsRelations Константный указатель на вектор стрктур, описывающих отношение столбцов таблицы к столбцам других.
													///< \param[in] p_v_p_MHorizontalHeaderViewsRelated Константный указатель на вектор указателей на зависимые виджеты вида заголовков окон.
													///< \return Ссылка на модель таблицы.
	/// Добавление записи в БД.
	bool AddRecord(MTableView* p_MTableView, const QString& r_strKeyNameForRemove, const std::map<uchar, QVariant>* const p_mpColumnsData);
													///< \param[in] p_MTableView Указатель на виджет вида таблицы.
													///< \param[in] r_strKeyNameForRemove Константная ссылка на имя колонки ключа для удаления (при необходимости).
													///< \param[in] p_mpColumnsData Константный указатель на константную карту 'колонка в данные'.
													///< \return true при успехе.
	/// Добавление урока в БД и таблицу.
	void AddLesson(const QPoint& r_Pos, MTableView* p_MTableViewDay, const QString& r_strDayColName);
													///< \param[in] r_Pos Точка клика.
													///< \param[in] p_MTableViewDay Указатель на виджет вида таблицы дня недели.
													///< \param[in] r_strDayColName Константная ссылка на имя колонки дня недели.
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
	/// При вызове контекстного меню на таблице понедельника.
	void on_tableViewTimetablePon_customContextMenuRequested(const QPoint& r_Pos);
													///< \param[in] r_Pos Точка клика.
	/// При вызове контекстного меню на таблице вторника.
	void on_tableViewTimetableVtor_customContextMenuRequested(const QPoint& r_Pos);
													///< \param[in] r_Pos Точка клика.
	/// При вызове контекстного меню на таблице среды.
	void on_tableViewTimetableSred_customContextMenuRequested(const QPoint& r_Pos);
													///< \param[in] r_Pos Точка клика.
	/// При вызове контекстного меню на таблице четверга.
	void on_tableViewTimetableChetv_customContextMenuRequested(const QPoint& r_Pos);
													///< \param[in] r_Pos Точка клика.
	/// При вызове контекстного меню на таблице пятницы.
	void on_tableViewTimetablePyatn_customContextMenuRequested(const QPoint& r_Pos);
													///< \param[in] r_Pos Точка клика.
	/// При вызове контекстного меню на таблице субботы.
	void on_tableViewTimetableSubb_customContextMenuRequested(const QPoint& r_Pos);
													///< \param[in] r_Pos Точка клика.
};

#endif // MAINWINDOW_H
