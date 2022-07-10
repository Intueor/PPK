#ifndef MTABLEVIEW_H
#define MTABLEVIEW_H

//== ВКЛЮЧЕНИЯ.
#include <QTableView>

//== КЛАССЫ.
/// Класс модифицированного виджета вида таблицы.
class MTableView : public QTableView
{
	Q_OBJECT

private:
	int iLastRQ = 0; ///< Сохраняет последнее значение кол-ва строк для предотвращения повторных попыток авторасширения.
	bool _bCanScroll = true; ///< При false - авторасширение под контент.
	int _iColumnForSort = 0; ///< При не 0 - колонка для автосортировки.
	std::vector<MTableView*> v_p_MTableViews; ///< Вектор указателей на виджеты вида таблиц.

private:
	/// Переопределение обновления геометрии для авторасширения без вертикального скроллинга.
	void updateGeometries() override;
	/// Переопределение отправки данных после редактирования для перезагрузки зависимых видов таблиц.
	void commitData(QWidget* p_Editor) override;
													///< \param[in] p_Editor Указатель на виджет редактора.
public:
	/// Конструктор.
	explicit MTableView(QWidget* p_Parent = nullptr) : QTableView(p_Parent) {}
													///< \param[in] p_Parent Указатель на родительский виджет.
	/// Установка или отмена возможности вертикального скроллинга виджета.
	void SetCanScroll(bool bCanScroll) {_bCanScroll = bCanScroll;}
													///< \param[in] bCan При true - возможен.
	/// Установка колонки для автосортировки или 0.
	void SetColumnForSort(int iColumnForSort);
													///< \param[in] bCan Колонка для автосортировки или 0.
	/// Добавление зависимого виджета вида таблицы.
	void AddRelatedTableView(MTableView* p_MTableView);
													///< \param[in] p_MTableView Указатель на виджет вида таблицы.
	/// Удаление зависимого виджета вида таблицы.
	void RemoveRelatedTableView(MTableView* p_MTableView);
													///< \param[in] p_MTableView Указатель на виджет вида таблицы.
};

#endif // MTABLEVIEW_H
