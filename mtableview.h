#ifndef MTABLEVIEW_H
#define MTABLEVIEW_H

//== ВКЛЮЧЕНИЯ.
#include "msqlrelationaldelegate.h"
#include <QTableView>
#include <QSqlRelationalDelegate>

//== КЛАССЫ.
/// Класс модифицированного виджета вида таблицы.
class MTableView : public QTableView
{
	Q_OBJECT

private:
	int iLastRQ = 0; ///< Сохраняет последнее значение кол-ва строк для предотвращения повторных попыток авторасширения.
	bool _bCanScroll = true; ///< При false - авторасширение под контент.
	std::vector<MTableView*> v_p_MTableViewsRelated; ///< Вектор указателей на зависимые виджеты вида таблиц.
	int _iColumnForSort = 0; ///< При не 0 - колонка для автосортировки.

private:
	/// Переопределение обновления геометрии для авторасширения без вертикального скроллинга.
	void updateGeometries() override;
	/// Переопределение отправки данных после редактирования для перезагрузки зависимых видов таблиц.
	void commitData(QWidget* p_Editor) override;
													///< \param[in] p_Editor Указатель на виджет редактора.
public:
	/// Конструктор.
	explicit MTableView(QWidget* p_Parent = nullptr) : QTableView(p_Parent)
													///< \param[in] p_Parent Указатель на родительский виджет.
	{}
	/// Перезагрузка зависимых видов таблиц.
	void UpdateRelatedTableViews();
	/// Установка колонки для автосортировки или 0.
	void SetColumnForSort(int iColumnForSort);
													///< \param[in] bCan Колонка для автосортировки или 0.
	/// Установка или отмена возможности вертикального скроллинга виджета.
	void SetCanScroll(bool bCanScroll)
													///< \param[in] bCan При true - возможен.
	{_bCanScroll = bCanScroll;}
													///< \param[in] bCan Колонка для автосортировки или 0.
	/// Добавление зависимого виджета вида таблицы.
	void AddRelatedTableView(MTableView* p_MTableViewRelated);
													///< \param[in] p_MTableViewRelated Указатель на виджет вида таблицы.
	/// Удаление зависимого виджета вида таблицы.
	void RemoveRelatedTableView(MTableView* p_MTableViewRelated);
													///< \param[in] p_MTableViewRelated Указатель на виджет вида таблицы.
};

#endif // MTABLEVIEW_H
