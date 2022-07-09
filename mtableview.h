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
	int iLastRQ = 0;
	bool bCanScroll = true;
	std::vector<MTableView*> v_p_MTableViews;

public:
	/// Конструктор.
	explicit MTableView(QWidget* p_Parent = nullptr) : QTableView(p_Parent) {}
													///< \param[in] p_Parent Указатель на родительский виджет.
	/// Переопределение обновления геометрии для авторасширения без вертикального скроллинга.
	void updateGeometries() override;
	/// Переопределение отправки данных после редактирования для перезагрузки зависимых видов таблиц.
	void commitData(QWidget* p_Editor) override;
													///< \param[in] p_Editor Указатель на виджет редактора.
	/// Установка или отмена возможности вертикального скроллинга виджета.
	void SetCanScroll(bool bCan) {bCanScroll = bCan;}
													///< \param[in] bCan При true - возможен.
	/// Добавление зависимого вида таблицы.
	void AddRelatedTableView(MTableView* p_MTableView);
													///< \param[in] p_MTableView Указатель на виджет вида таблицы.
	// Удаление зависимого вида таблицы.
	void RemoveRelatedTableView(MTableView* p_MTableView);
													///< \param[in] p_MTableView Указатель на виджет вида таблицы.
};

#endif // MTABLEVIEW_H