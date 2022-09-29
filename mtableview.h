#ifndef MTABLEVIEW_H
#define MTABLEVIEW_H

//== ВКЛЮЧЕНИЯ.
#include "msqlrelationaldelegate.h"
#include <QTableView>
#include <QSqlRelationalDelegate>
#include <QHeaderView>
#include <QTimer>

//== МАКРОСЫ.
//! \file mtableview.h
#define STARTUP_UPDATE_ANTISPAM_DELAY_MS			100 ///< Величина задержки антиспама стартовых обновлений геометрии.
#define BOTTOM_FREE_SPACE_PX						22 ///< Размер отступа снизу от строк таблицы.

//== КЛАССЫ.
/// Класс модифицированного заголовка виджета вида таблицы.
class Q_WIDGETS_EXPORT MHorizontalHeaderView : public QHeaderView
{
	Q_OBJECT

private:
	bool bMPressed = false; ///< Признак нажатой мыши при имеющемся векторе указателей на зависимые виджеты вида заголовков окон.
	std::vector<MHorizontalHeaderView*>* _p_v_p_MHorizontalHeaderViewsRelated; ///< Внутренний указатель на вектор указателей на зависимые виджеты вида заголовков окон.

public:
	/// Конструктор.
	explicit MHorizontalHeaderView(Qt::Orientation oOrientation, QWidget* p_Parent = nullptr) : QHeaderView(oOrientation, p_Parent)
													///< \param[in] oOrientation Ориентация заголовка.
													///< \param[in] p_Parent Указатель на родительский виджет.
	{}
	/// Установка укзателя на вектор указателей на зависимые виджеты вида заголовков окон или nullptr.
	void SetRelatedHorizontalHeaderViews(std::vector<MHorizontalHeaderView*>* p_v_p_MHorizontalHeaderViewsRelated)
													///< \param[in] p_v_p_MHorizontalHeaderViewsRelated Указатель на вектор указателей на зависимые виджеты вида заголовков окон.
	{ _p_v_p_MHorizontalHeaderViewsRelated = p_v_p_MHorizontalHeaderViewsRelated; }
protected:
	/// Переопределение обработчика события нажатия мыши.
	void mousePressEvent(QMouseEvent* p_E) override;
													///< \param[in] p_E Событие мыши.
	/// Переопределение обработчика события смещения мыши.
	void mouseMoveEvent(QMouseEvent* p_E) override;
													///< \param[in] p_E Событие мыши.
	/// Переопределение обработчика события отпускания мыши.
	void mouseReleaseEvent(QMouseEvent* p_E) override;
													///< \param[in] p_E Событие мыши.
};

/// Класс модифицированного виджета вида таблицы.
class MTableView : public QTableView
{
	Q_OBJECT

private:
	int iLastRQ = 0; ///< Сохраняет последнее значение кол-ва строк для предотвращения повторных попыток авторасширения.
	bool _bCanScroll = true; ///< При false - авторасширение под контент.
	std::vector<MTableView*> v_p_MTableViewsRelated; ///< Вектор указателей на зависимые виджеты вида таблиц.
	std::vector<MHorizontalHeaderView*> v_p_MHorizontalHeaderViewsRelated; ///< Вектор указателей на зависимые виджеты вида заголовков таблиц.
	int _iColumnForSort = 0; ///< При не 0 - колонка для автосортировки.
	bool bOverridedStretchLastSection = false; ///< Признак переопределённого растяжения последней видимой секции горизонтального заголовка.
	QTimer oAntiSpamTimer; ///< Таймер стартового антиспама.
	bool bStartupSpamIsOver = false; ///< Признак окончания стартового спама обновлений геометрии.

private:
	/// Переопределение обновления геометрии для авторасширения без вертикального скроллинга.
	void updateGeometries() override;
	/// Переопределение отправки данных после редактирования для перезагрузки зависимых видов таблиц.
	void commitData(QWidget* p_Editor) override;
													///< \param[in] p_Editor Указатель на виджет редактора.
private slots:
	/// Слот антиспам-таймера.
	void AntiSpamTimerSlot();

public:
	/// Конструктор.
	explicit MTableView(QWidget* p_Parent = nullptr);
													///< \param[in] p_Parent Указатель на родительский виджет.
	/// Выравнивание высоты вида таблицы по содержимому.
	void AdjustVerticalSize(uchar uchBottomFreeSpace = BOTTOM_FREE_SPACE_PX);
													///< \param[in] uchBottomFreeSpace Размер отступа снизу от строк таблицы.
	/// Перезагрузка зависимых видов таблиц.
	void UpdateRelatedTableViews();
	/// Установка колонки для автосортировки или 0.
	void SetColumnForSort(int iColumnForSort);
													///< \param[in] iColumnForSort Колонка для автосортировки или 0.
	/// Установка растяжения последней секции горизонтального заголовка (дефолтный не ипользовать).
	void SetOverridedStretchLastHSection(bool bOn)
													///< \param[in] bOn Переопределённый признак растяжения.
	{ bOverridedStretchLastSection = bOn; }
	/// Установка или отмена возможности вертикального скроллинга виджета.
	void SetCanScroll(bool bCanScroll)
													///< \param[in] bCanScroll При true - возможен.
	{_bCanScroll = bCanScroll;}
	/// Добавление зависимого виджета вида таблицы.
	void AddRelatedTableView(MTableView* p_MTableViewRelated);
													///< \param[in] p_MTableViewRelated Указатель на виджет вида таблицы.
	/// Удаление зависимого виджета вида таблицы.
	void RemoveRelatedTableView(MTableView* p_MTableViewRelated);
													///< \param[in] p_MTableViewRelated Указатель на виджет вида таблицы.
	/// Добавление зависимого виджета вида заголовка таблицы.
	void AddRelatedHorizontalHeaderView(MHorizontalHeaderView* p_MHorizontalHeaderViewRelated);
													///< \param[in] p_MHorizontalHeaderViewRelated Указатель на виджет вида заголовка таблицы.
	/// Удаление зависимого виджета вида заголовка таблицы.
	void RemoveRelatedHorizontalHeaderView(MHorizontalHeaderView* p_MHorizontalHeaderViewRelated);
													///< \param[in] p_MHorizontalHeaderViewRelated Указатель на виджет вида заголовка таблицы.

};

#endif // MTABLEVIEW_H
